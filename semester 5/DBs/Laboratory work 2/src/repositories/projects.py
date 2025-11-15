from __future__ import annotations

from typing import Any
from sqlalchemy import text
from .base import BaseRepository


class ProjectsRepository(BaseRepository):
    async def create_project(
        self,
        client_id: int,
        category_id: int | None,
        title: str,
        description: str | None,
        budget: float | None,
        actor_id: int,
    ) -> int:
        stmt = text(
            "CALL create_project(:client_id, :category_id, :title, :description, :budget, :actor_id, NULL)"
        )
        await self.session.execute(
            stmt,
            {
                "client_id": client_id,
                "category_id": category_id,
                "title": title,
                "description": description,
                "budget": budget,
                "actor_id": actor_id,
            },
        )
        fetch_stmt = text(
            """
            SELECT id FROM projects
            WHERE client_id = :client_id AND title = :title
            ORDER BY created_at DESC, id DESC
            LIMIT 1
            """
        )
        row = (await self.session.execute(fetch_stmt, {"client_id": client_id, "title": title})).one()
        return int(row.id)

    async def update_status(self, project_id: int, status: str, actor_id: int) -> None:
        stmt = text("CALL update_project_status(:project_id, :status, :actor_id)")
        await self.session.execute(stmt, {"project_id": project_id, "status": status, "actor_id": actor_id})

    async def list_active(self) -> list[dict[str, Any]]:
        stmt = text("SELECT * FROM view_active_projects ORDER BY created_at DESC")
        result = await self.session.execute(stmt)
        return [dict(r) for r in result.mappings().all()]
