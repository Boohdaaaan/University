Лабороторна Робота #1
студента Кузнецова Богдана ТК-31

1. Було розроблено схему бази даних відповідно до вимог варіанту лабороторної роботи. ER діа

ER діаграма: [ER.pdf](ER.pdf)

2. Створено 17 таблиць.

3. Сутності зі спец. полями змін:

- Підтримують soft delete: `users`, `projects`, `contracts`, `messages`.
- Зберігають дату останньої зміни та користувача: `users`, `projects`, `tasks`.

4. Модель реалізовано в PostgreSQL

5. Збережені функції/процедури/тригери/представлення

- Views:
  - `view_active_projects`: спрощує вибірки активних проєктів без soft-deleted, додає email клієнта та назву категорії — менше дублів SQL і швидший рендер UI.
  - `view_contracts_with_payments`: агрегує суми платежів по контракту — зручно для фінансових списків/звітів без складних підзапитів у коді.
  - `view_user_summary`: дає узагальнену статистику по користувачах (проєкти, контракти, відгуки) — корисно для адмін-огляду.
  - `view_unread_notifications`: швидкий список непрочитаних сповіщень — мінімізує бізнес-логіку у застосунку.
  - `view_chat_last_message`: повертає останнє повідомлення для кожного чату — для списку чатів без важких підзапитів.
- Функції/процедури:
  - `create_user`: інкапсулює створення користувача і аудит; повертає id, забезпечує єдину точку логіки.
  - `soft_delete_user`: централізований soft-delete з фіксацією виконавця та аудиту — цілісність історії змін.
  - `create_project`: транзакційне створення проєкту з ініціалізацією полів і аудиту — уніфікований бізнес-процес.
  - `update_project_status`: уніфікує зміну статусу проєкту, логуваня дії — простіший і прозорий флоу.
  - `add_payment`: єдина точка додавання платежу (валідації/правила легко зосередити тут).
  - `accept_bid_and_create_contract`: атомарно приймає ставку, відхиляє інші і створює контракт — гарантія узгодженості.
- Тригери:
  - `trg_set_updated_fields` (+ `set_updated_*`): автоматично оновлює `updated_at` — менше людських помилок.
  - `trg_audit_log` (+ `audit_projects`): прозорий аудит CRUD для відтворюваності дій.
  - `trg_validate_contract_dates`: перевіряє коректність дат контракту (кінець не раніше початку) — бізнес-інваріант на рівні БД.

6. Індекси

- BTREE (композитний, частковий): `idx_projects_active_status_created_at` на `projects(status, created_at DESC)` WHERE `is_deleted=FALSE AND status IN ('open','in_progress') - список активних проєктів із сортуванням по даті.
- BTREE (композитний): `idx_projects_client_title_created_at` на `projects(client_id, title, created_at DESC)` — швидке отримання щойно створеного проєкту у fallback-запиті.
- BTREE (одноколонкові): `idx_projects_client_id`, `idx_contracts_client_id`, `idx_reviews_target_id`, `idx_payments_contract_id` — швидкі джойни/агрегації.
- BTREE: `idx_audit_log_timestamp` на `audit_log(timestamp DESC)` — швидкий вивід останніх подій у /audit.
- GIN (pg_trgm): `idx_users_email_trgm` на `users(email)` — швидкий пошук email по підрядку (`LIKE/ILIKE`).
- BRIN: `idx_audit_log_timestamp_brin` на `audit_log(timestamp)` — ефективні діапазонні скани для великого журналу.

7. Repository + Unit of Work

- Unit of Work: `src/uow.py`
- Репозиторії (використовують тільки процедури/представлення):
  - `src/repositories/users.py` — `create_user`, `soft_delete`, `list_user_summary (VIEW)`
  - `src/repositories/projects.py` — `create_project`, `update_status`, `list_active (VIEW)`
  - `src/repositories/contracts.py` — `accept_bid_and_create_contract`, `add_payment`, `list_contracts_with_payments (VIEW)`
  - `src/repositories/audit.py` — `list_recent (view_audit_log_recent)`