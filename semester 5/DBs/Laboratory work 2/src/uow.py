from __future__ import annotations

from sqlalchemy.ext.asyncio import AsyncSession
from motor.motor_asyncio import AsyncIOMotorDatabase

from .db import AsyncSessionMaker
from .mongo_db import get_mongo_db


class UnitOfWork:
    def __init__(self) -> None:
        self.session: AsyncSession | None = None
        self.mongo_db: AsyncIOMotorDatabase | None = None

    async def __aenter__(self) -> "UnitOfWork":
        self.session = AsyncSessionMaker()
        self.mongo_db = get_mongo_db()
        return self

    async def __aexit__(self, exc_type, exc, tb) -> None:
        assert self.session is not None
        try:
            if exc is None:
                await self.session.commit()
            else:
                await self.session.rollback()
        finally:
            await self.session.close()

    # Допоміжні методи доступу
    def get_session(self) -> AsyncSession:
        assert self.session is not None
        return self.session

    def get_mongo_db(self) -> AsyncIOMotorDatabase:
        assert self.mongo_db is not None
        return self.mongo_db


