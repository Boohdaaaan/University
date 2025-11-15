-- =========================
-- Views
-- =========================

CREATE OR REPLACE VIEW view_active_projects AS
SELECT
    p.id,
    p.title,
    p.description,
    p.budget,
    p.status,
    p.created_at,
    u.email AS client_email,
    c.name  AS category_name
FROM projects p
JOIN users u ON u.id = p.client_id
LEFT JOIN categories c ON c.id = p.category_id
WHERE p.is_deleted = FALSE
  AND p.status IN ('open','in_progress');

CREATE OR REPLACE VIEW view_contracts_with_payments AS
SELECT
    c.id AS contract_id,
    c.project_id,
    c.client_id,
    c.freelancer_id,
    c.status,
    COALESCE(SUM(pay.amount), 0)::numeric(12,2) AS total_paid,
    COUNT(pay.id) AS payments_count
FROM contracts c
LEFT JOIN payments pay ON pay.contract_id = c.id
GROUP BY c.id;

CREATE OR REPLACE VIEW view_user_summary AS
WITH user_projects AS (
    SELECT client_id AS user_id, COUNT(*) AS project_count
    FROM projects
    WHERE is_deleted = FALSE
    GROUP BY client_id
),
user_contracts AS (
    SELECT client_id AS user_id, COUNT(*) AS as_client_contracts
    FROM contracts
    GROUP BY client_id
),
user_reviews AS (
    SELECT target_id AS user_id, COUNT(*) AS reviews_received
    FROM reviews
    GROUP BY target_id
)
SELECT
    u.id,
    u.email,
    u.role,
    COALESCE(up.project_count, 0) AS project_count,
    COALESCE(uc.as_client_contracts, 0) AS contracts_as_client,
    COALESCE(ur.reviews_received, 0) AS reviews_received
FROM users u
LEFT JOIN user_projects up ON up.user_id = u.id
LEFT JOIN user_contracts uc ON uc.user_id = u.id
LEFT JOIN user_reviews ur ON ur.user_id = u.id
WHERE u.is_deleted = FALSE;

CREATE OR REPLACE VIEW view_unread_notifications AS
SELECT n.id, n.user_id, u.email, n.type, n.message, n.created_at
FROM notifications n
JOIN users u ON u.id = n.user_id
WHERE n.is_read = FALSE
ORDER BY n.created_at DESC;

CREATE OR REPLACE VIEW view_chat_last_message AS
SELECT
    m.chat_id,
    MAX(m.id)              AS last_message_id,
    MAX(m.created_at)      AS last_message_at,
    (ARRAY_AGG(m.text ORDER BY m.created_at DESC))[1] AS last_message_text
FROM messages m
WHERE m.is_deleted = FALSE
GROUP BY m.chat_id;

-- Recent audit log with user emails
CREATE OR REPLACE VIEW view_audit_log_recent AS
SELECT
    a.id,
    a.entity_name,
    a.entity_id,
    a.action,
    a.timestamp,
    u.email AS user_email
FROM audit_log a
LEFT JOIN users u ON u.id = a.user_id;

-- =========================
-- Procedures and Functions
-- =========================

-- 1) create_user function returns created user id
CREATE OR REPLACE FUNCTION create_user(p_email text, p_password_hash text, p_role text DEFAULT 'client')
RETURNS integer
LANGUAGE plpgsql
AS $$
DECLARE
    v_user_id integer;
BEGIN
    INSERT INTO users(email, password_hash, role, is_deleted, created_at)
    VALUES (p_email, p_password_hash, p_role, FALSE, NOW())
    RETURNING id INTO v_user_id;

    INSERT INTO audit_log(user_id, entity_name, entity_id, action, timestamp)
    VALUES (v_user_id, 'users', v_user_id, 'INSERT', NOW());

    RETURN v_user_id;
END;
$$;

-- 2) soft delete user as procedure
CREATE OR REPLACE PROCEDURE soft_delete_user(p_user_id integer, p_actor_id integer)
LANGUAGE plpgsql
AS $$
BEGIN
    UPDATE users
    SET is_deleted = TRUE,
        updated_at = NOW(),
        updated_by = p_actor_id
    WHERE id = p_user_id;

    INSERT INTO audit_log(user_id, entity_name, entity_id, action, timestamp)
    VALUES (p_actor_id, 'users', p_user_id, 'SOFT_DELETE', NOW());
END;
$$;

-- 3) create_project procedure with OUT id
CREATE OR REPLACE PROCEDURE create_project(
    IN p_client_id integer,
    IN p_category_id integer,
    IN p_title text,
    IN p_description text,
    IN p_budget numeric(12,2),
    IN p_actor_id integer,
    OUT o_project_id integer
)
LANGUAGE plpgsql
AS $$
BEGIN
    INSERT INTO projects(client_id, category_id, title, description, budget, status, is_deleted, created_at, updated_by)
    VALUES (p_client_id, p_category_id, p_title, p_description, p_budget, 'open', FALSE, NOW(), p_actor_id)
    RETURNING id INTO o_project_id;

    INSERT INTO audit_log(user_id, entity_name, entity_id, action, timestamp)
    VALUES (p_actor_id, 'projects', o_project_id, 'INSERT', NOW());
END;
$$;

-- 4) update_project_status procedure
CREATE OR REPLACE PROCEDURE update_project_status(p_project_id integer, p_status text, p_actor_id integer)
LANGUAGE plpgsql
AS $$
BEGIN
    UPDATE projects
    SET status = p_status,
        updated_at = NOW(),
        updated_by = p_actor_id
    WHERE id = p_project_id;

    INSERT INTO audit_log(user_id, entity_name, entity_id, action, timestamp)
    VALUES (p_actor_id, 'projects', p_project_id, 'STATUS_CHANGE', NOW());
END;
$$;

-- 5) add_payment procedure
CREATE OR REPLACE PROCEDURE add_payment(p_contract_id integer, p_amount numeric(12,2), p_method text)
LANGUAGE plpgsql
AS $$
BEGIN
    INSERT INTO payments(contract_id, amount, payment_date, method, status)
    VALUES (p_contract_id, p_amount, CURRENT_DATE, p_method, 'completed');
END;
$$;

-- 6) accept bid and create contract; returns new contract id
CREATE OR REPLACE PROCEDURE accept_bid_and_create_contract(p_bid_id integer, OUT o_contract_id integer)
LANGUAGE plpgsql
AS $$
DECLARE
    v_project_id integer;
    v_freelancer_id integer;
    v_client_id integer;
BEGIN
    SELECT b.project_id, b.freelancer_id INTO v_project_id, v_freelancer_id
    FROM bids b WHERE b.id = p_bid_id FOR UPDATE;

    SELECT p.client_id INTO v_client_id FROM projects p WHERE p.id = v_project_id;

    UPDATE bids SET status = 'accepted' WHERE id = p_bid_id;
    UPDATE bids SET status = 'rejected' WHERE project_id = v_project_id AND id <> p_bid_id AND status = 'pending';

    INSERT INTO contracts(project_id, client_id, freelancer_id, start_date, status, is_deleted)
    VALUES (v_project_id, v_client_id, v_freelancer_id, CURRENT_DATE, 'active', FALSE)
    RETURNING id INTO o_contract_id;
END;
$$;

-- =========================
-- Triggers
-- =========================

-- A) generic updated_at trigger function
CREATE OR REPLACE FUNCTION trg_set_updated_fields()
RETURNS trigger
LANGUAGE plpgsql
AS $$
BEGIN
    NEW.updated_at := NOW();
    RETURN NEW;
END;
$$;

-- B) audit trigger function
CREATE OR REPLACE FUNCTION trg_audit_log()
RETURNS trigger
LANGUAGE plpgsql
AS $$
DECLARE
    v_action text;
BEGIN
    IF TG_OP = 'INSERT' THEN
        v_action := 'INSERT';
        INSERT INTO audit_log(user_id, entity_name, entity_id, action, timestamp)
        VALUES (NEW.updated_by, TG_TABLE_NAME, NEW.id, v_action, NOW());
        RETURN NEW;
    ELSIF TG_OP = 'UPDATE' THEN
        v_action := 'UPDATE';
        INSERT INTO audit_log(user_id, entity_name, entity_id, action, timestamp)
        VALUES (NEW.updated_by, TG_TABLE_NAME, NEW.id, v_action, NOW());
        RETURN NEW;
    ELSIF TG_OP = 'DELETE' THEN
        v_action := 'DELETE';
        INSERT INTO audit_log(user_id, entity_name, entity_id, action, timestamp)
        VALUES (OLD.updated_by, TG_TABLE_NAME, OLD.id, v_action, NOW());
        RETURN OLD;
    END IF;
    RETURN NULL;
END;
$$;

-- C) contract dates validation trigger function
CREATE OR REPLACE FUNCTION trg_validate_contract_dates()
RETURNS trigger
LANGUAGE plpgsql
AS $$
BEGIN
    IF NEW.end_date IS NOT NULL AND NEW.start_date IS NOT NULL AND NEW.end_date < NEW.start_date THEN
        RAISE EXCEPTION 'end_date (%) cannot be earlier than start_date (%)', NEW.end_date, NEW.start_date;
    END IF;
    RETURN NEW;
END;
$$;

-- Attach triggers idempotently
DO $$
BEGIN
    IF NOT EXISTS (
        SELECT 1 FROM pg_trigger WHERE tgname = 'set_updated_users'
    ) THEN
        CREATE TRIGGER set_updated_users
        BEFORE UPDATE ON users
        FOR EACH ROW EXECUTE FUNCTION trg_set_updated_fields();
    END IF;

    IF NOT EXISTS (
        SELECT 1 FROM pg_trigger WHERE tgname = 'set_updated_projects'
    ) THEN
        CREATE TRIGGER set_updated_projects
        BEFORE UPDATE ON projects
        FOR EACH ROW EXECUTE FUNCTION trg_set_updated_fields();
    END IF;

    IF NOT EXISTS (
        SELECT 1 FROM pg_trigger WHERE tgname = 'set_updated_tasks'
    ) THEN
        CREATE TRIGGER set_updated_tasks
        BEFORE UPDATE ON tasks
        FOR EACH ROW EXECUTE FUNCTION trg_set_updated_fields();
    END IF;

    IF NOT EXISTS (
        SELECT 1 FROM pg_trigger WHERE tgname = 'audit_projects'
    ) THEN
        CREATE TRIGGER audit_projects
        AFTER INSERT OR UPDATE OR DELETE ON projects
        FOR EACH ROW EXECUTE FUNCTION trg_audit_log();
    END IF;

    IF NOT EXISTS (
        SELECT 1 FROM pg_trigger WHERE tgname = 'validate_contract_dates'
    ) THEN
        CREATE TRIGGER validate_contract_dates
        BEFORE INSERT OR UPDATE ON contracts
        FOR EACH ROW EXECUTE FUNCTION trg_validate_contract_dates();
    END IF;
END $$;

-- =========================
-- Indexes
-- =========================
-- Active projects list: filter by status + is_deleted=false, sort by created_at desc
CREATE INDEX IF NOT EXISTS idx_projects_active_status_created_at
ON projects (status, created_at DESC)
WHERE is_deleted = FALSE AND status IN ('open','in_progress');

-- Projects lookup by client and title (used in repo fallback)
CREATE INDEX IF NOT EXISTS idx_projects_client_title_created_at
ON projects (client_id, title, created_at DESC);

-- Foreign-key access patterns used in summaries and joins
CREATE INDEX IF NOT EXISTS idx_projects_client_id ON projects (client_id);
CREATE INDEX IF NOT EXISTS idx_contracts_client_id ON contracts (client_id);
CREATE INDEX IF NOT EXISTS idx_reviews_target_id ON reviews (target_id);
CREATE INDEX IF NOT EXISTS idx_payments_contract_id ON payments (contract_id);

-- Audit page: recent first
CREATE INDEX IF NOT EXISTS idx_audit_log_timestamp ON audit_log (timestamp DESC);

-- Enable trigram search for emails
CREATE EXTENSION IF NOT EXISTS pg_trgm;
CREATE INDEX IF NOT EXISTS idx_users_email_trgm ON users USING gin (email gin_trgm_ops);

-- BRIN index for large append-only audit logs (range scans by timestamp)
CREATE INDEX IF NOT EXISTS idx_audit_log_timestamp_brin ON audit_log USING brin (timestamp);
