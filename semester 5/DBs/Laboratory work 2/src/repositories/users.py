from __future__ import annotations

from typing import Any

from sqlalchemy import text

from .base import BaseRepository


class UsersRepository(BaseRepository):
    async def create_user(self, email: str, password_hash: str, role: str = "client") -> int:
        stmt = text("SELECT create_user(:email, :password_hash, :role) AS id")
        result = await self.session.execute(
            stmt, {"email": email, "password_hash": password_hash, "role": role}
        )
        row = result.one()
        return int(row.id)

    async def soft_delete(self, user_id: int, actor_id: int) -> None:
        stmt = text("CALL soft_delete_user(:user_id, :actor_id)")
        await self.session.execute(stmt, {"user_id": user_id, "actor_id": actor_id})

    async def list_user_summary(self) -> list[dict[str, Any]]:
        stmt = text("SELECT * FROM view_user_summary ORDER BY id")
        result = await self.session.execute(stmt)
        rows = result.mappings().all()
        return [dict(r) for r in rows]


