from datetime import UTC, datetime
from uuid import UUID, uuid4

from sqlalchemy import func
from sqlalchemy.ext.asyncio import AsyncAttrs
from sqlalchemy.orm import (
    DeclarativeBase,
    Mapped,
    MappedAsDataclass,
    mapped_column,
)


class Base(AsyncAttrs, MappedAsDataclass, DeclarativeBase):
    pass


class BaseEntity(Base):
    __abstract__ = True

    id: Mapped[UUID] = mapped_column(init=False, primary_key=True, default_factory=uuid4)
    created_at: Mapped[datetime] = mapped_column(
        init=False,
        default_factory=lambda: datetime.now(UTC),
        insert_default=func.now(),
    )
    updated_at: Mapped[datetime | None] = mapped_column(init=False, default=None, onupdate=func.now())


class BaseAssociativeEntity(BaseEntity):
    __abstract__ = True
