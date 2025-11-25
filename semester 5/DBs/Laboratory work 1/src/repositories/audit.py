from __future__ import annotations

from typing import Any

from sqlalchemy import text

from .base import BaseRepository


class AuditRepository(BaseRepository):
    async def list_recent(self, limit: int = 200) -> list[dict[str, Any]]:
        result = await self.session.execute(
            text(
                """
                SELECT id, entity_name, entity_id, action, timestamp, user_email
                FROM view_audit_log_recent
                ORDER BY timestamp DESC
                LIMIT :limit
                """
            ),
            {"limit": limit},
        )
        return [dict(r) for r in result.mappings().all()]
