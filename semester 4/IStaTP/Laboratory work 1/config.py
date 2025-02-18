from pydantic_settings import BaseSettings, SettingsConfigDict
from sqlalchemy.engine import URL


class DatabaseSettings(BaseSettings):
    model_config = SettingsConfigDict(env_file=".env", env_prefix="DATABASE_", case_sensitive=False)

    host: str
    port: int
    user: str
    password: str
    name: str

    def get_database_url(self) -> str:
        return URL.create(
            drivername="postgresql",
            username=self.user,
            password=self.password,
            host=self.host,
            port=self.port,
            database=self.name,
        )
    