from __future__ import annotations

try:
    from redis import asyncio as redis
except ImportError:
    import redis.asyncio as redis

from os import getenv


_redis_client: redis.Redis | None = None


def get_redis_client() -> redis.Redis:
    """Отримати клієнт Redis"""
    global _redis_client
    if _redis_client is None:
        redis_url = getenv("REDIS_URL", "redis://localhost:6379")
        _redis_client = redis.from_url(redis_url, decode_responses=True)
    return _redis_client


async def close_redis() -> None:
    """Закрити з'єднання з Redis"""
    global _redis_client
    if _redis_client:
        await _redis_client.close()
        _redis_client = None

