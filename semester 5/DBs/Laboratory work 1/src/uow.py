from __future__ import annotations

from sqlalchemy.ext.asyncio import AsyncSession

from .db import AsyncSessionMaker


class UnitOfWork:
    def __init__(self) -> None:
        self.session: AsyncSession | None = None

    async def __aenter__(self) -> "UnitOfWork":
        self.session = AsyncSessionMaker()
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

    # Helper accessor
    def get_session(self) -> AsyncSession:
        assert self.session is not None
        return self.session


