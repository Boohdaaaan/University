from flask import Flask, render_template
from sqlalchemy import create_engine, select
from sqlalchemy.orm import sessionmaker

from database.books import Book
from config import DatabaseSettings


app = Flask(__name__)

# Configure SQLAlchemy
settings = DatabaseSettings()
DATABASE_URL = settings.get_database_url()
engine = create_engine(DATABASE_URL)
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)


def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


@app.route('/')
def index():
    db = SessionLocal()
    try:
        books = db.execute(select(Book)).scalars().all()
        return render_template('index.html', books=books)
    finally:
        db.close()


if __name__ == '__main__':
    app.run(debug=True)
    