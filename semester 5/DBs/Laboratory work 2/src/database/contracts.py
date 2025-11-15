from __future__ import annotations

from datetime import date, datetime

from sqlalchemy import CheckConstraint, Date, DateTime, ForeignKey, Integer, Numeric, String, Text
from sqlalchemy.orm import Mapped, mapped_column, relationship

from .base import Base


class Bids(Base):
    __tablename__ = "bids"

    id: Mapped[int] = mapped_column(Integer, primary_key=True)
    project_id: Mapped[int] = mapped_column(ForeignKey("projects.id", ondelete="CASCADE"))
    freelancer_id: Mapped[int] = mapped_column(ForeignKey("users.id"))
    proposal_text: Mapped[str | None] = mapped_column(Text)
    bid_amount: Mapped[float | None] = mapped_column(Numeric(12, 2))
    status: Mapped[str | None] = mapped_column(String(50), default="pending")
    created_at: Mapped[datetime | None] = mapped_column(DateTime, default=datetime.utcnow)

    __table_args__ = (
        CheckConstraint("status IN ('pending','accepted','rejected')", name="ck_bids_status"),
    )


class Contracts(Base):
    __tablename__ = "contracts"

    id: Mapped[int] = mapped_column(Integer, primary_key=True)
    project_id: Mapped[int] = mapped_column(ForeignKey("projects.id"))
    client_id: Mapped[int] = mapped_column(ForeignKey("users.id"))
    freelancer_id: Mapped[int] = mapped_column(ForeignKey("users.id"))
    start_date: Mapped[date | None] = mapped_column(Date, default=date.today)
    end_date: Mapped[date | None] = mapped_column(Date)
    status: Mapped[str | None] = mapped_column(String(50), default="active")
    is_deleted: Mapped[bool] = mapped_column(default=False)

    __table_args__ = (
        CheckConstraint(
            "status IN ('active','completed','cancelled')",
            name="ck_contracts_status",
        ),
    )


