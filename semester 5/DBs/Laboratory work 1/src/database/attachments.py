from __future__ import annotations

from datetime import datetime

from sqlalchemy import DateTime, ForeignKey, Integer, String
from sqlalchemy.orm import Mapped, mapped_column

from .base import Base


class Attachments(Base):
    __tablename__ = "attachments"

    id: Mapped[int] = mapped_column(Integer, primary_key=True)
    file_path: Mapped[str] = mapped_column(String(255), nullable=False)
    entity_type: Mapped[str | None] = mapped_column(String(50))
    entity_id: Mapped[int] = mapped_column(Integer, nullable=False)
    uploaded_at: Mapped[datetime | None] = mapped_column(DateTime, default=datetime.utcnow)
    uploaded_by: Mapped[int | None] = mapped_column(ForeignKey("users.id"))


