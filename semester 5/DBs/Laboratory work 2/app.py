from __future__ import annotations

from pathlib import Path
from fastapi import FastAPI, Form, Request
from fastapi.responses import HTMLResponse, RedirectResponse
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates

from src.repositories.contracts import ContractsRepository
from src.repositories.projects import ProjectsRepository
from src.repositories.users import UsersRepository
from src.repositories.audit import AuditRepository
from src.repositories.portfolio_mongo import PortfolioMongoRepository, PortfolioItem
from src.repositories.portfolio_pg import PortfolioPGRepository
from src.repositories.messages_mongo import MessagesMongoRepository, MessageItem
from src.repositories.redis_repo import RedisRepository
from src.uow import UnitOfWork
import time
import json


app = FastAPI(title="DBs Lab 1 Demo")

BASE_DIR = Path(__file__).resolve().parent
templates = Jinja2Templates(directory=str(BASE_DIR / "templates"))
app.mount("/static", StaticFiles(directory=str(BASE_DIR / "static")), name="static")


@app.get("/", response_class=HTMLResponse)
async def index(request: Request) -> HTMLResponse:
    return templates.TemplateResponse("index.html", {"request": request})


@app.post("/users/create")
async def users_create(
    email: str = Form(...),
    password: str = Form(...),
    role: str = Form("client"),
) -> RedirectResponse:
    async with UnitOfWork() as uow:
        users = UsersRepository(uow.get_session())
        await users.create_user(email=email, password_hash=password, role=role)
    return RedirectResponse(url="/users/summary", status_code=303)


@app.post("/users/delete")
async def users_delete(user_id: int = Form(...), actor_id: int = Form(...)) -> RedirectResponse:
    async with UnitOfWork() as uow:
        users = UsersRepository(uow.get_session())
        await users.soft_delete(user_id=user_id, actor_id=actor_id)
    return RedirectResponse(url="/users/summary", status_code=303)


@app.get("/users/summary", response_class=HTMLResponse)
async def users_summary(request: Request) -> HTMLResponse:
    async with UnitOfWork() as uow:
        users = UsersRepository(uow.get_session())
        data = await users.list_user_summary()
    return templates.TemplateResponse("users_summary.html", {"request": request, "rows": data})


@app.post("/projects/create")
async def projects_create(
    client_id: int = Form(...),
    title: str = Form(...),
    category_id: int | None = Form(None),
    description: str | None = Form(None),
    budget: float | None = Form(None),
    actor_id: int = Form(...),
) -> RedirectResponse:
    async with UnitOfWork() as uow:
        repo = ProjectsRepository(uow.get_session())
        await repo.create_project(
            client_id=client_id,
            category_id=category_id,
            title=title,
            description=description,
            budget=budget,
            actor_id=actor_id,
        )
    return RedirectResponse(url="/projects/active", status_code=303)


@app.post("/projects/status")
async def projects_status(project_id: int = Form(...), status: str = Form(...), actor_id: int = Form(...)) -> RedirectResponse:
    async with UnitOfWork() as uow:
        repo = ProjectsRepository(uow.get_session())
        await repo.update_status(project_id=project_id, status=status, actor_id=actor_id)
    return RedirectResponse(url="/projects/active", status_code=303)


@app.get("/projects/active", response_class=HTMLResponse)
async def projects_active(request: Request) -> HTMLResponse:
    async with UnitOfWork() as uow:
        repo = ProjectsRepository(uow.get_session())
        data = await repo.list_active()
    return templates.TemplateResponse("projects_active.html", {"request": request, "rows": data})


@app.get("/contracts", response_class=HTMLResponse)
async def contracts_view(request: Request) -> HTMLResponse:
    async with UnitOfWork() as uow:
        repo = ContractsRepository(uow.get_session())
        rows = await repo.list_contracts_with_payments()
    return templates.TemplateResponse("contracts.html", {"request": request, "rows": rows})


@app.post("/contracts/accept_bid")
async def accept_bid(bid_id: int = Form(...)) -> RedirectResponse:
    async with UnitOfWork() as uow:
        repo = ContractsRepository(uow.get_session())
        await repo.accept_bid_and_create_contract(bid_id)
    return RedirectResponse(url="/contracts", status_code=303)

@app.get("/audit", response_class=HTMLResponse)
async def audit_view(request: Request) -> HTMLResponse:
    async with UnitOfWork() as uow:
        repo = AuditRepository(uow.get_session())
        rows = await repo.list_recent(limit=200)
    return templates.TemplateResponse("audit.html", {"request": request, "rows": rows})


@app.post("/contracts/add_payment")
async def add_payment(contract_id: int = Form(...), amount: float = Form(...), method: str = Form("card")) -> RedirectResponse:
    async with UnitOfWork() as uow:
        repo = ContractsRepository(uow.get_session())
        await repo.add_payment(contract_id=contract_id, amount=amount, method=method)
    return RedirectResponse(url="/contracts", status_code=303)


# =========================
# Ендпоінти портфоліо
# =========================

@app.post("/portfolios/mongo/create")
async def portfolio_mongo_create(
    freelancer_id: int = Form(...),
    title: str = Form(...),
    description: str | None = Form(None),
    category: str | None = Form(None),
    url: str | None = Form(None),
    images: str | None = Form(None),
    tags: str | None = Form(None),
    extra_data: str | None = Form(None),
) -> RedirectResponse:
    async with UnitOfWork() as uow:
        # Парсимо images (один URL на рядок)
        images_list = None
        if images:
            images_list = [img.strip() for img in images.split('\n') if img.strip()]
        
        # Парсимо tags (через кому)
        tags_list = None
        if tags:
            tags_list = [tag.strip() for tag in tags.split(',') if tag.strip()]
        
        # Парсимо extra_data (JSON)
        extra_data_dict = None
        if extra_data:
            try:
                import json
                extra_data_dict = json.loads(extra_data)
            except json.JSONDecodeError:
                pass
        
        item = PortfolioItem(
            freelancer_id=freelancer_id,
            title=title,
            description=description,
            category=category,
            url=url,
            images=images_list,
            tags=tags_list,
        )
        
        # Додаємо extra_data безпосередньо в документ
        doc = item.model_dump(by_alias=True, exclude_none=True)
        if extra_data_dict:
            doc.update(extra_data_dict)

        await uow.get_mongo_db().portfolios.insert_one(doc)

    return RedirectResponse(url="/portfolios/mongo/list", status_code=303)


@app.get("/portfolios/mongo/list", response_class=HTMLResponse)
async def portfolio_mongo_list(
    request: Request,
    freelancer_id: int | None = None,
    category: str | None = None,
) -> HTMLResponse:
    async with UnitOfWork() as uow:
        repo = PortfolioMongoRepository(uow.get_mongo_db())
        if freelancer_id:
            data = await repo.list_by_freelancer(freelancer_id, category)
        else:
            data = []
    return templates.TemplateResponse(
        "portfolios_mongo.html",
        {"request": request, "rows": data, "db_type": "MongoDB"},
    )


@app.post("/portfolios/pg/create")
async def portfolio_pg_create(
    freelancer_id: int = Form(...),
    title: str = Form(...),
    description: str | None = Form(None),
    category: str | None = Form(None),
    url: str | None = Form(None),
    images: str | None = Form(None),
    tags: str | None = Form(None),
    extra_data: str | None = Form(None),
) -> RedirectResponse:
    async with UnitOfWork() as uow:
        repo = PortfolioPGRepository(uow.get_session())
        
        # Парсимо images (один URL на рядок)
        images_list = None
        if images:
            images_list = [img.strip() for img in images.split('\n') if img.strip()]
        
        # Парсимо tags (через кому)
        tags_list = None
        if tags:
            tags_list = [tag.strip() for tag in tags.split(',') if tag.strip()]
        
        # Парсимо extra_data (JSON)
        extra_data_dict = None
        if extra_data:
            try:
                import json
                extra_data_dict = json.loads(extra_data)
            except json.JSONDecodeError:
                pass
        
        await repo.create(
            freelancer_id=freelancer_id,
            title=title,
            description=description,
            category=category,
            url=url,
            images=images_list,
            tags=tags_list,
            extra_data=extra_data_dict,
        )
    return RedirectResponse(url="/portfolios/pg/list", status_code=303)


@app.get("/portfolios/pg/list", response_class=HTMLResponse)
async def portfolio_pg_list(
    request: Request,
    freelancer_id: int | None = None,
    category: str | None = None,
) -> HTMLResponse:
    async with UnitOfWork() as uow:
        repo = PortfolioPGRepository(uow.get_session())
        if freelancer_id:
            data = await repo.list_by_freelancer(freelancer_id, category)
        else:
            data = []
    return templates.TemplateResponse(
        "portfolios_pg.html",
        {"request": request, "rows": data, "db_type": "PostgreSQL"},
    )


@app.get("/portfolios/compare", response_class=HTMLResponse)
async def portfolio_compare(
    request: Request,
    freelancer_id: int = 1,
    category: str | None = None,
    iterations: int = 2000,
) -> HTMLResponse:
    """Порівняння швидкості запитів MongoDB vs PostgreSQL"""
    results = {"mongo": [], "pg": []}

    # MongoDB тест
    async with UnitOfWork() as uow:
        mongo_repo = PortfolioMongoRepository(uow.get_mongo_db())
        for _ in range(iterations):
            start = time.time()
            await mongo_repo.list_by_freelancer(freelancer_id, category)
            results["mongo"].append(time.time() - start)

    # PostgreSQL тест
    async with UnitOfWork() as uow:
        pg_repo = PortfolioPGRepository(uow.get_session())
        for _ in range(iterations):
            start = time.time()
            await pg_repo.list_by_freelancer(freelancer_id, category)
            results["pg"].append(time.time() - start)

    mongo_avg = sum(results["mongo"]) / len(results["mongo"]) if results["mongo"] else 0
    pg_avg = sum(results["pg"]) / len(results["pg"]) if results["pg"] else 0

    return templates.TemplateResponse(
        "portfolios_compare.html",
        {
            "request": request,
            "mongo_avg": mongo_avg * 1000,  # в мс
            "pg_avg": pg_avg * 1000,  # в мс
            "iterations": iterations,
            "freelancer_id": freelancer_id,
            "category": category,
        },
    )


# =========================
# Ендпоінти повідомлень
# =========================

@app.post("/messages/mongo/create")
async def message_mongo_create(
    chat_id: int = Form(...),
    sender_id: int = Form(...),
    text: str = Form(...),
    reactions: str | None = Form(None),
    attachments: str | None = Form(None),
) -> RedirectResponse:
    async with UnitOfWork() as uow:
        # Парсимо reactions (JSON)
        reactions_dict = None
        if reactions:
            try:
                reactions_dict = json.loads(reactions)
            except json.JSONDecodeError:
                pass
        
        # Парсимо attachments (один URL на рядок)
        attachments_list = None
        if attachments:
            attachments_list = [att.strip() for att in attachments.split('\n') if att.strip()]
        
        item = MessageItem(
            chat_id=chat_id,
            sender_id=sender_id,
            text=text,
            reactions=reactions_dict,
            attachments=attachments_list,
        )
        
        doc = item.model_dump(by_alias=True, exclude_none=True)
        await uow.get_mongo_db().messages.insert_one(doc)
    
    return RedirectResponse(url="/messages/mongo/list?chat_id=" + str(chat_id), status_code=303)


@app.get("/messages/mongo/list", response_class=HTMLResponse)
async def message_mongo_list(
    request: Request,
    chat_id: int,
    limit: int = 50,
    skip: int = 0,
) -> HTMLResponse:
    async with UnitOfWork() as uow:
        repo = MessagesMongoRepository(uow.get_mongo_db())
        data = await repo.list_by_chat(chat_id, limit=limit, skip=skip)
    
    return templates.TemplateResponse(
        "messages_mongo.html",
        {"request": request, "rows": data, "chat_id": chat_id, "limit": limit, "skip": skip},
    )


# =========================
# Ендпоінти Redis
# =========================

@app.get("/redis", response_class=HTMLResponse)
async def redis_page(request: Request) -> HTMLResponse:
    """Сторінка для роботи з Redis"""
    return templates.TemplateResponse(
        "redis.html",
        {
            "request": request,
            "get_result": None,
            "hash_result": None,
            "list_result": None,
            "keys": None,
        },
    )


@app.post("/redis/set")
async def redis_set(
    key: str = Form(...),
    value: str = Form(...),
    ttl: int | None = Form(None),
) -> RedirectResponse:
    """Встановити значення в Redis"""
    repo = RedisRepository()
    await repo.set(key, value, ttl)
    return RedirectResponse(url="/redis", status_code=303)


@app.get("/redis/get", response_class=HTMLResponse)
async def redis_get(request: Request, key: str) -> HTMLResponse:
    """Отримати значення з Redis"""
    repo = RedisRepository()
    result = await repo.get(key)
    # Завжди передаємо результат, навіть якщо None
    return templates.TemplateResponse(
        "redis.html",
        {
            "request": request,
            "get_result": result,  # Може бути None, але це нормально
            "hash_result": None,
            "list_result": None,
            "keys": None,
        },
    )


@app.post("/redis/delete")
async def redis_delete(key: str = Form(...)) -> RedirectResponse:
    """Видалити ключ з Redis"""
    repo = RedisRepository()
    await repo.delete(key)
    return RedirectResponse(url="/redis", status_code=303)


@app.post("/redis/set-hash")
async def redis_set_hash(
    key: str = Form(...),
    json_data: str = Form(...),
) -> RedirectResponse:
    """Встановити hash в Redis"""
    try:
        data = json.loads(json_data)
        repo = RedisRepository()
        await repo.set_hash(key, {str(k): str(v) for k, v in data.items()})
    except json.JSONDecodeError:
        pass
    return RedirectResponse(url="/redis", status_code=303)


@app.get("/redis/get-hash", response_class=HTMLResponse)
async def redis_get_hash(request: Request, key: str) -> HTMLResponse:
    """Отримати hash з Redis"""
    repo = RedisRepository()
    result = await repo.get_hash(key)
    result_str = json.dumps(result, indent=2, ensure_ascii=False) if result else None
    return templates.TemplateResponse(
        "redis.html",
        {
            "request": request,
            "get_result": None,
            "hash_result": result_str,  # Може бути None
            "list_result": None,
            "keys": None,
        },
    )


@app.post("/redis/set-list")
async def redis_set_list(
    key: str = Form(...),
    values: str = Form(...),
) -> RedirectResponse:
    """Встановити список в Redis"""
    repo = RedisRepository()
    values_list = [v.strip() for v in values.split('\n') if v.strip()]
    await repo.set_list(key, values_list)
    return RedirectResponse(url="/redis", status_code=303)


@app.get("/redis/get-list", response_class=HTMLResponse)
async def redis_get_list(request: Request, key: str) -> HTMLResponse:
    """Отримати список з Redis"""
    repo = RedisRepository()
    result = await repo.get_list(key)
    return templates.TemplateResponse(
        "redis.html",
        {
            "request": request,
            "get_result": None,
            "hash_result": None,
            "list_result": result,  # Може бути None
            "keys": None,
        },
    )


@app.get("/redis/keys", response_class=HTMLResponse)
async def redis_keys(request: Request, pattern: str = "*") -> HTMLResponse:
    """Отримати всі ключі за паттерном"""
    repo = RedisRepository()
    keys = await repo.get_all_keys(pattern)
    return templates.TemplateResponse(
        "redis.html",
        {
            "request": request,
            "get_result": None,
            "hash_result": None,
            "list_result": None,
            "keys": keys,  # Завжди список, навіть якщо порожній
        },
    )

