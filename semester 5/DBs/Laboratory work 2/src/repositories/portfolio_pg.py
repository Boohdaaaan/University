from __future__ import annotations

from typing import Any

from sqlalchemy import select
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy.orm import selectinload

from ..database.portfolio import Portfolio, PortfolioImage, PortfolioMetric, Tag
from .base import BaseRepository


class PortfolioPGRepository(BaseRepository):
    """Репозиторій для роботи з портфоліо в PostgreSQL (нормалізована структура)"""

    async def create(
        self,
        freelancer_id: int,
        title: str,
        description: str | None = None,
        category: str | None = None,
        images: list[str] | None = None,
        tags: list[str] | None = None,
        url: str | None = None,
        extra_data: dict | None = None,
    ) -> int:
        """Створити портфоліо з пов'язаними даними"""
        # Створюємо основне портфоліо
        portfolio = Portfolio(
            freelancer_id=freelancer_id,
            title=title,
            description=description,
            category=category,
            url=url,
        )
        self.session.add(portfolio)
        await self.session.flush()

        # Додаємо зображення
        if images:
            for idx, image_url in enumerate(images):
                image = PortfolioImage(
                    portfolio_id=portfolio.id,
                    image_url=image_url,
                    display_order=idx,
                )
                self.session.add(image)

        # Додаємо теги
        if tags:
            for tag_name in tags:
                # Знаходимо або створюємо тег
                tag_stmt = select(Tag).where(Tag.name == tag_name)
                tag_result = await self.session.execute(tag_stmt)
                tag = tag_result.scalar_one_or_none()
                
                if not tag:
                    tag = Tag(name=tag_name)
                    self.session.add(tag)
                    await self.session.flush()
                
                # Додаємо зв'язок портфоліо-тег
                portfolio.tags.append(tag)

        # Додаємо метрики з extra_data
        if extra_data:
            for key, value in extra_data.items():
                metric_type = self._determine_metric_type(key)
                
                # Для складних структур зберігаємо в metric_data
                if isinstance(value, dict):
                    metric = PortfolioMetric(
                        portfolio_id=portfolio.id,
                        metric_name=key,
                        metric_type=metric_type,
                        metric_data=value,
                    )
                elif isinstance(value, (int, float)):
                    metric = PortfolioMetric(
                        portfolio_id=portfolio.id,
                        metric_name=key,
                        metric_value=float(value),
                        metric_type=metric_type,
                    )
                else:
                    # Для інших типів зберігаємо в metric_data
                    metric = PortfolioMetric(
                        portfolio_id=portfolio.id,
                        metric_name=key,
                        metric_type=metric_type,
                        metric_data={"value": value},
                    )
                self.session.add(metric)

        await self.session.flush()
        return portfolio.id

    def _determine_metric_type(self, metric_name: str) -> str:
        """Визначає тип метрики за назвою"""
        if "performance" in metric_name.lower() or "requests" in metric_name.lower():
            return "performance"
        elif "app_store" in metric_name.lower() or "downloads" in metric_name.lower() or "rating" in metric_name.lower():
            return "app_store"
        elif "model" in metric_name.lower() or "accuracy" in metric_name.lower() or "training" in metric_name.lower():
            return "model"
        elif "infrastructure" in metric_name.lower() or "nodes" in metric_name.lower() or "pods" in metric_name.lower():
            return "infrastructure"
        elif "test" in metric_name.lower() or "coverage" in metric_name.lower():
            return "qa"
        else:
            return "general"

    async def get_by_id(self, portfolio_id: int) -> dict[str, Any] | None:
        """Отримати портфоліо за ID з усіма пов'язаними даними"""
        stmt = (
            select(Portfolio)
            .where(Portfolio.id == portfolio_id)
            .options(
                selectinload(Portfolio.images),
                selectinload(Portfolio.tags),
                selectinload(Portfolio.metrics),
            )
        )
        result = await self.session.execute(stmt)
        portfolio = result.scalar_one_or_none()
        
        if not portfolio:
            return None

        return self._portfolio_to_dict(portfolio)

    async def list_by_freelancer(
        self, freelancer_id: int, category: str | None = None
    ) -> list[dict[str, Any]]:
        """Отримати всі портфоліо фрілансера з пов'язаними даними"""
        stmt = (
            select(Portfolio)
            .where(Portfolio.freelancer_id == freelancer_id)
            .options(
                selectinload(Portfolio.images),
                selectinload(Portfolio.tags),
                selectinload(Portfolio.metrics),
            )
        )
        
        if category:
            stmt = stmt.where(Portfolio.category == category)
        
        stmt = stmt.order_by(Portfolio.created_at.desc())

        result = await self.session.execute(stmt)
        portfolios = result.scalars().all()
        
        return [self._portfolio_to_dict(p) for p in portfolios]

    def _portfolio_to_dict(self, portfolio: Portfolio) -> dict[str, Any]:
        """Конвертує Portfolio об'єкт у словник з нормалізованими даними"""
        # Збираємо зображення
        images = [img.image_url for img in sorted(portfolio.images, key=lambda x: x.display_order)]
        
        # Збираємо теги
        tags = [tag.name for tag in portfolio.tags]
        
        # Збираємо метрики в extra_data формат
        extra_data = {}
        for metric in portfolio.metrics:
            if metric.metric_data:
                extra_data[metric.metric_name] = metric.metric_data
            elif metric.metric_value is not None:
                extra_data[metric.metric_name] = float(metric.metric_value)
            else:
                extra_data[metric.metric_name] = None

        return {
            "id": portfolio.id,
            "freelancer_id": portfolio.freelancer_id,
            "title": portfolio.title,
            "description": portfolio.description,
            "category": portfolio.category,
            "images": images if images else None,
            "tags": tags if tags else None,
            "url": portfolio.url,
            "extra_data": extra_data if extra_data else None,
            "created_at": portfolio.created_at,
            "updated_at": portfolio.updated_at,
            }

    async def update(
        self,
        portfolio_id: int,
        title: str | None = None,
        description: str | None = None,
        category: str | None = None,
    ) -> bool:
        """Оновити портфоліо"""
        stmt = select(Portfolio).where(Portfolio.id == portfolio_id)
        result = await self.session.execute(stmt)
        portfolio = result.scalar_one_or_none()
        if not portfolio:
            return False

        if title is not None:
            portfolio.title = title
        if description is not None:
            portfolio.description = description
        if category is not None:
            portfolio.category = category

        await self.session.flush()
        return True

    async def delete(self, portfolio_id: int) -> bool:
        """Видалити портфоліо (каскадно видаляться всі пов'язані дані)"""
        stmt = select(Portfolio).where(Portfolio.id == portfolio_id)
        result = await self.session.execute(stmt)
        portfolio = result.scalar_one_or_none()
        if not portfolio:
            return False

        await self.session.delete(portfolio)
        await self.session.flush()
        return True
