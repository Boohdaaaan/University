from __future__ import annotations

from typing import Any

from sqlalchemy import text

from .base import BaseRepository


class ContractsRepository(BaseRepository):
    async def accept_bid_and_create_contract(self, bid_id: int) -> int:
        await self.session.execute(text("CALL accept_bid_and_create_contract(:bid_id, NULL)"), {"bid_id": bid_id})
        row = (
            await self.session.execute(
                text("SELECT id FROM contracts ORDER BY id DESC LIMIT 1")
            )
        ).one()
        return int(row.id)

    async def add_payment(self, contract_id: int, amount: float, method: str = "card") -> None:
        await self.session.execute(
            text("CALL add_payment(:contract_id, :amount, :method)"),
            {"contract_id": contract_id, "amount": amount, "method": method},
        )

    async def list_contracts_with_payments(self) -> list[dict[str, Any]]:
        result = await self.session.execute(text("SELECT * FROM view_contracts_with_payments ORDER BY contract_id DESC"))
        return [dict(r) for r in result.mappings().all()]
