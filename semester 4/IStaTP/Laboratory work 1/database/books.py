from sqlalchemy.orm import Mapped, relationship, mapped_column
from sqlalchemy import ForeignKey, Column, Integer, String, Float, Text, Boolean
from uuid import UUID
from datetime import datetime
from sqlalchemy.orm import DeclarativeBase
from typing import List
from werkzeug.security import generate_password_hash, check_password_hash
from flask_login import UserMixin

from database.base import BaseEntity, BaseAssociativeEntity
# from database.reviews import Review

# ---- authors ----

class Author(BaseEntity):
    __tablename__ = "authors"

    name: Mapped[str]
    biography: Mapped[str | None]

    written_books: Mapped[List["Book"]] = relationship(
        back_populates="written_by",
        secondary="book_author"
    )


# ---- books ----
class Book(BaseEntity):
    __tablename__ = "books"

    title: Mapped[str]
    year: Mapped[int]
    price: Mapped[float]
    description: Mapped[str]
    isbn: Mapped[str | None]
    average_rating: Mapped[float | None]
    text_link: Mapped[str | None]

    written_by: Mapped[List["Author"]] = relationship(
        back_populates="written_books",
        secondary="book_author"
    )
    genres: Mapped[List["Genre"]] = relationship(
        back_populates="books",
        secondary="book_genre"
    )
    libraries: Mapped[List["Library"]] = relationship(
        back_populates="books",
        secondary="library_book"
    )


class BookAuthor(BaseAssociativeEntity):
    __tablename__ = "book_author"

    book_id: Mapped[UUID] = mapped_column(ForeignKey("books.id"), primary_key=True)
    author_id: Mapped[UUID] = mapped_column(ForeignKey("authors.id"), primary_key=True)

# ----- Genres -----

class Genre(BaseEntity):
    __tablename__ = "genres"

    name: Mapped[str]
    description: Mapped[str]

    books: Mapped[List["Book"]] = relationship(
        back_populates="genres",
        secondary="book_genre"
    )


class BookGenre(BaseAssociativeEntity):
    __tablename__ = "book_genre"

    genre_id: Mapped[UUID] = mapped_column(ForeignKey("genres.id"), primary_key=True)
    book_id: Mapped[UUID] = mapped_column(ForeignKey("books.id"), primary_key=True)


# ----- Libraries -----

class Library(BaseEntity):
    __tablename__ = "libraries"

    name: Mapped[str]
    user_id: Mapped[UUID] = mapped_column(ForeignKey("users.id"))

    books: Mapped[List["Book"]] = relationship(
        back_populates="libraries",
        secondary="library_book"
    )
    user: Mapped["User"] = relationship(back_populates="libraries")


class LibraryBook(BaseAssociativeEntity):
    __tablename__ = "library_book"

    library_id: Mapped[UUID] = mapped_column(ForeignKey("libraries.id"), primary_key=True)
    book_id: Mapped[UUID] = mapped_column(ForeignKey("books.id"), primary_key=True)


# ---- users ----
class User(BaseEntity, UserMixin):
    __tablename__ = "users"

    name: Mapped[str]
    email: Mapped[str]
    password_hash: Mapped[str]
    is_admin: Mapped[bool] = mapped_column(default=False)
    
    libraries: Mapped[List["Library"]] = relationship(back_populates="user")
    reviews: Mapped[List["Review"]] = relationship(back_populates="user")
    reading_processes: Mapped[List["ReadingProcess"]] = relationship(back_populates="user")
    
    def set_password(self, password):
        self.password_hash = generate_password_hash(password)
        
    def check_password(self, password):
        return check_password_hash(self.password_hash, password)


# ---- reading process ----

class ReadingProcess(BaseAssociativeEntity):
    __tablename__ = "reading_process"

    user_id: Mapped[UUID] = mapped_column(ForeignKey("users.id"), primary_key=True)
    book_id: Mapped[UUID] = mapped_column(ForeignKey("books.id"), primary_key=True)
    current_page: Mapped[int]
    start_date: Mapped[datetime]
    end_date: Mapped[datetime | None]

    user: Mapped["User"] = relationship(back_populates="reading_processes")


# ---- reviews ----

class Review(BaseEntity):
    __tablename__ = "reviews"

    book_id: Mapped[UUID] = mapped_column(ForeignKey("books.id"))
    user_id: Mapped[UUID] = mapped_column(ForeignKey("users.id"))
    rating: Mapped[float]
    comment: Mapped[str | None]

    user: Mapped["User"] = relationship(back_populates="reviews")
