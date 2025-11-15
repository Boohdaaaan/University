from __future__ import annotations

from datetime import datetime
from typing import Any

from bson.objectid import ObjectId
from motor.motor_asyncio import AsyncIOMotorDatabase
from pydantic import BaseModel, ConfigDict, Field


class PortfolioItem(BaseModel):
    """Модель портфоліо для MongoDB з гнучкою структурою"""
    model_config = ConfigDict(
        extra="allow",  # Дозволяє додаткові поля
        arbitrary_types_allowed=True,
        populate_by_name=True,
    )

    id: ObjectId | None = Field(default_factory=ObjectId, alias="_id")
    freelancer_id: int
    title: str
    description: str | None = None
    category: str | None = None
    created_at: datetime = Field(default_factory=datetime.utcnow)

    # Опціональні типізовані поля
    images: list[str] | None = None
    tags: list[str] | None = None
    url: str | None = None


class PortfolioMongoRepository:
    """Репозиторій для роботи з портфоліо в MongoDB"""

    def __init__(self, db: AsyncIOMotorDatabase) -> None:
        self.collection = db.portfolios

    async def create(self, item: PortfolioItem) -> str:
        """Створити портфоліо"""
        doc = item.model_dump(by_alias=True, exclude_none=True)
        result = await self.collection.insert_one(doc)
        return str(result.inserted_id)

    async def get_by_id(self, portfolio_id: str) -> dict[str, Any] | None:
        """Отримати портфоліо за ID"""
        try:
            doc = await self.collection.find_one({"_id": ObjectId(portfolio_id)})
            if doc:
                doc["_id"] = str(doc["_id"])
            return doc
        except Exception:
            return None

    async def list_by_freelancer(
        self, freelancer_id: int, category: str | None = None
    ) -> list[dict[str, Any]]:
        """Отримати всі портфоліо фрілансера"""
        query: dict[str, Any] = {"freelancer_id": freelancer_id}
        if category:
            query["category"] = category

        cursor = self.collection.find(query).sort("created_at", -1)
        docs = await cursor.to_list(length=None)  # length=None - усі документи
        
        portfolios = []
        for doc in docs:
            doc["_id"] = str(doc["_id"])
            portfolios.append(doc)
        return portfolios

    async def update(
        self, portfolio_id: str, updates: dict[str, Any]
    ) -> bool:
        """Оновити портфоліо"""
        try:
            updates["updated_at"] = datetime.utcnow()
            result = await self.collection.update_one(
                {"_id": ObjectId(portfolio_id)}, {"$set": updates}
            )
            return result.modified_count > 0
        except Exception:
            return False

    async def delete(self, portfolio_id: str) -> bool:
        """Видалити портфоліо"""
        try:
            result = await self.collection.delete_one({"_id": ObjectId(portfolio_id)})
            return result.deleted_count > 0
        except Exception:
            return False

    async def add_custom_field(
        self, portfolio_id: str, field_name: str, value: Any
    ) -> bool:
        """Додати довільне поле до портфоліо"""
        return await self.update(portfolio_id, {field_name: value})

