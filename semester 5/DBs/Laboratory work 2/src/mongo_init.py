from __future__ import annotations

from .mongo_db import get_mongo_db


async def init_mongo_indexes() -> None:
    """Ініціалізація індексів для MongoDB колекцій"""
    db = get_mongo_db()

    # Індекси для портфоліо
    portfolios_collection = db.portfolios
    await portfolios_collection.create_index("freelancer_id")
    await portfolios_collection.create_index("category")
    await portfolios_collection.create_index([("freelancer_id", 1), ("category", 1)])
    await portfolios_collection.create_index([("freelancer_id", 1), ("created_at", -1)])

    # Індекси для повідомлень
    messages_collection = db.messages
    await messages_collection.create_index("chat_id")
    await messages_collection.create_index("sender_id")
    await messages_collection.create_index([("chat_id", 1), ("created_at", -1)])
    await messages_collection.create_index([("chat_id", 1), ("is_deleted", 1)])

