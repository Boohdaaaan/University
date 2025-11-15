-- =========================
-- Add portfolios tables for PostgreSQL (normalized structure)
-- =========================

-- Основна таблиця портфоліо
CREATE TABLE IF NOT EXISTS portfolios (
    id SERIAL PRIMARY KEY,
    freelancer_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    title VARCHAR(255) NOT NULL,
    description TEXT,
    category VARCHAR(100),
    url VARCHAR(500),
    created_at TIMESTAMP DEFAULT NOW(),
    updated_at TIMESTAMP
);

-- Таблиця зображень портфоліо (1:N)
CREATE TABLE IF NOT EXISTS portfolio_images (
    id SERIAL PRIMARY KEY,
    portfolio_id INTEGER NOT NULL REFERENCES portfolios(id) ON DELETE CASCADE,
    image_url VARCHAR(500) NOT NULL,
    display_order INTEGER DEFAULT 0,
    created_at TIMESTAMP DEFAULT NOW()
);

-- Таблиця тегів (спільна для всіх портфоліо)
CREATE TABLE IF NOT EXISTS tags (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) UNIQUE NOT NULL,
    created_at TIMESTAMP DEFAULT NOW()
);

-- Зв'язуюча таблиця портфоліо-теги (M:N)
CREATE TABLE IF NOT EXISTS portfolio_tags (
    portfolio_id INTEGER NOT NULL REFERENCES portfolios(id) ON DELETE CASCADE,
    tag_id INTEGER NOT NULL REFERENCES tags(id) ON DELETE CASCADE,
    PRIMARY KEY (portfolio_id, tag_id)
);

-- Таблиця метрик портфоліо
-- metric_type: 'performance', 'app_store', 'model', 'infrastructure', 'qa', 'general'
-- metric_data: Для складних структур типу performance_metrics, model_metrics
CREATE TABLE IF NOT EXISTS portfolio_metrics (
    id SERIAL PRIMARY KEY,
    portfolio_id INTEGER NOT NULL REFERENCES portfolios(id) ON DELETE CASCADE,
    metric_name VARCHAR(100) NOT NULL,
    metric_value NUMERIC,
    metric_type VARCHAR(50),
    metric_data JSONB,
    created_at TIMESTAMP DEFAULT NOW()
);
