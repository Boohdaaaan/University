from __future__ import annotations

from typing import Any
from ..redis_db import get_redis_client


class RedisRepository:
    """Репозиторій для роботи з Redis (key-value store)"""

    def __init__(self) -> None:
        self.client = get_redis_client()

    async def set(self, key: str, value: str, ttl: int | None = None) -> bool:
        """Встановити значення для ключа"""
        try:
            if ttl:
                await self.client.setex(key, ttl, value)
            else:
                await self.client.set(key, value)
            return True
        except Exception:
            return False

    async def get(self, key: str) -> str | None:
        """Отримати значення за ключем"""
        try:
            return await self.client.get(key)
        except Exception:
            return None

    async def delete(self, key: str) -> bool:
        """Видалити ключ"""
        try:
            result = await self.client.delete(key)
            return result > 0
        except Exception:
            return False

    async def exists(self, key: str) -> bool:
        """Перевірити існування ключа"""
        try:
            result = await self.client.exists(key)
            return result > 0
        except Exception:
            return False

    async def set_hash(self, key: str, mapping: dict[str, str]) -> bool:
        """Встановити hash (об'єкт)"""
        try:
            await self.client.hset(key, mapping=mapping)
            return True
        except Exception:
            return False

    async def get_hash(self, key: str) -> dict[str, str] | None:
        """Отримати hash (об'єкт)"""
        try:
            result = await self.client.hgetall(key)
            return result if result else None
        except Exception:
            return None

    async def set_list(self, key: str, values: list[str]) -> bool:
        """Встановити список"""
        try:
            await self.client.delete(key)  # Очистити перед додаванням
            if values:
                await self.client.rpush(key, *values)
            return True
        except Exception:
            return False

    async def get_list(self, key: str, start: int = 0, end: int = -1) -> list[str] | None:
        """Отримати список"""
        try:
            result = await self.client.lrange(key, start, end)
            return result if result else None
        except Exception:
            return None

    async def add_to_list(self, key: str, value: str) -> bool:
        """Додати значення до списку"""
        try:
            await self.client.rpush(key, value)
            return True
        except Exception:
            return False

    async def get_all_keys(self, pattern: str = "*") -> list[str]:
        """Отримати всі ключі за паттерном"""
        try:
            keys = []
            async for key in self.client.scan_iter(match=pattern):
                keys.append(key)
            return keys
        except Exception:
            return []

    async def get_ttl(self, key: str) -> int | None:
        """Отримати TTL ключа (-1 якщо без TTL, -2 якщо не існує)"""
        try:
            return await self.client.ttl(key)
        except Exception:
            return None

    async def increment(self, key: str, amount: int = 1) -> int | None:
        """Інкрементувати числове значення"""
        try:
            return await self.client.incrby(key, amount)
        except Exception:
            return None

    async def set_json(self, key: str, data: dict[str, Any], ttl: int | None = None) -> bool:
        """Зберегти JSON об'єкт"""
        try:
            import json
            json_str = json.dumps(data)
            if ttl:
                await self.client.setex(key, ttl, json_str)
            else:
                await self.client.set(key, json_str)
            return True
        except Exception:
            return False

    async def get_json(self, key: str) -> dict[str, Any] | None:
        """Отримати JSON об'єкт"""
        try:
            import json
            value = await self.client.get(key)
            if value:
                return json.loads(value)
            return None
        except Exception:
            return None

