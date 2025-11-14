from datetime import datetime
from uuid import UUID
from sqlalchemy.orm import DeclarativeBase, Mapped, mapped_column
from sqlalchemy.sql import func


class Base(DeclarativeBase):
    pass


class BaseEntity(Base):
    __abstract__ = True

    id: Mapped[UUID] = mapped_column(primary_key=True, server_default=func.gen_random_uuid())
    created_at: Mapped[datetime] = mapped_column(server_default=func.current_timestamp())
    updated_at: Mapped[datetime] = mapped_column(
        server_default=func.current_timestamp(),
        onupdate=func.current_timestamp()
    )


class BaseAssociativeEntity(Base):
    __abstract__ = True

    created_at: Mapped[datetime] = mapped_column(server_default=func.current_timestamp())
    updated_at: Mapped[datetime] = mapped_column(
        server_default=func.current_timestamp(),
        onupdate=func.current_timestamp()
    )
