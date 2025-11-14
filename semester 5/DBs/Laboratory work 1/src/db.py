from __future__ import annotations

import os
from typing import AsyncIterator

from dotenv import load_dotenv
from sqlalchemy.ext.asyncio import AsyncEngine, AsyncSession, async_sessionmaker, create_async_engine

load_dotenv()


engine: AsyncEngine = create_async_engine(os.getenv("DATABASE_URL"), echo=False, pool_pre_ping=True)
AsyncSessionMaker = async_sessionmaker(engine, class_=AsyncSession, expire_on_commit=False)


async def get_session() -> AsyncIterator[AsyncSession]:
    async with AsyncSessionMaker() as session:
        yield session
