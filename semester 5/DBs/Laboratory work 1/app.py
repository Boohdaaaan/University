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
from src.uow import UnitOfWork


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

