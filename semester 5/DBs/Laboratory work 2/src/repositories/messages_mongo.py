from __future__ import annotations

from datetime import datetime
from typing import Any

from bson.objectid import ObjectId
from motor.motor_asyncio import AsyncIOMotorDatabase
from pydantic import BaseModel, ConfigDict, Field


class MessageItem(BaseModel):
    """Модель повідомлення для MongoDB"""
    model_config = ConfigDict(
        extra="allow",  # Дозволяє додаткові поля
        arbitrary_types_allowed=True,
        populate_by_name=True,
    )

    id: ObjectId | None = Field(default_factory=ObjectId, alias="_id")
    chat_id: int
    sender_id: int
    text: str | None = None
    created_at: datetime = Field(default_factory=datetime.utcnow)
    is_deleted: bool = False

    # Опціональні поля для метаданих
    edited_at: datetime | None = None
    reactions: dict[str, list[int]] | None = None  # {"👍": [user_id1, user_id2], "❤️": [user_id3]}
    attachments: list[str] | None = None  # URL файлів


class MessagesMongoRepository:
    """Репозиторій для роботи з повідомленнями в MongoDB"""

    def __init__(self, db: AsyncIOMotorDatabase) -> None:
        self.collection = db.messages

    async def create(self, item: MessageItem) -> str:
        """Створити повідомлення"""
        doc = item.model_dump(by_alias=True, exclude_none=True)
        result = await self.collection.insert_one(doc)
        return str(result.inserted_id)

    async def get_by_id(self, message_id: str) -> dict[str, Any] | None:
        """Отримати повідомлення за ID"""
        try:
            doc = await self.collection.find_one({"_id": ObjectId(message_id), "is_deleted": False})
            if doc:
                doc["_id"] = str(doc["_id"])
            return doc
        except Exception:
            return None

    async def list_by_chat(
        self, chat_id: int, limit: int = 50, skip: int = 0
    ) -> list[dict[str, Any]]:
        """Отримати повідомлення чату з пагінацією"""
        query = {"chat_id": chat_id, "is_deleted": False}
        
        cursor = (
            self.collection.find(query)
            .sort("created_at", -1)
            .skip(skip)
            .limit(limit)
        )
        
        docs = await cursor.to_list(length=limit)
        
        messages = []
        for doc in docs:
            doc["_id"] = str(doc["_id"])
            messages.append(doc)
        
        return messages

    async def update(
        self, message_id: str, updates: dict[str, Any]
    ) -> bool:
        """Оновити повідомлення"""
        try:
            updates["edited_at"] = datetime.utcnow()
            result = await self.collection.update_one(
                {"_id": ObjectId(message_id)}, {"$set": updates}
            )
            return result.modified_count > 0
        except Exception:
            return False

    async def soft_delete(self, message_id: str) -> bool:
        """М'яке видалення повідомлення"""
        try:
            result = await self.collection.update_one(
                {"_id": ObjectId(message_id)}, {"$set": {"is_deleted": True}}
            )
            return result.modified_count > 0
        except Exception:
            return False

    async def add_reaction(self, message_id: str, reaction: str, user_id: int) -> bool:
        """Додати реакцію до повідомлення"""
        try:
            result = await self.collection.update_one(
                {"_id": ObjectId(message_id)},
                {"$addToSet": {f"reactions.{reaction}": user_id}}
            )
            return result.modified_count > 0
        except Exception:
            return False

    async def add_custom_field(
        self, message_id: str, field_name: str, value: Any
    ) -> bool:
        """Додати довільне поле до повідомлення"""
        return await self.update(message_id, {field_name: value})

