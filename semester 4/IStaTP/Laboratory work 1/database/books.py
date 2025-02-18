from sqlalchemy.orm import Mapped, relationship, mapped_column
from sqlalchemy import ForeignKey
from uuid import UUID
from datetime import datetime

from database.base import BaseEntity, BaseAssociativeEntity
# from database.reviews import Review

# ---- authors ----

class Author(BaseEntity):
    __tablename__ = "authors"

    name: Mapped[str]
    biography: Mapped[str | None]

    written_books: Mapped[list["Book"]] = relationship(back_populates="written_by", secondary="book_author", default_factory=list)


# ---- books ----
class Book(BaseEntity):
    __tablename__ = "books"

    title: Mapped[str]
    isbn: Mapped[str | None]
    description: Mapped[str]
    average_rating: Mapped[float | None]
    text_link: Mapped[str | None]

    written_by: Mapped[list["Author"]] = relationship(back_populates="written_books", secondary="book_author", default=None)
    genres: Mapped[list["Genre"]] = relationship(back_populates="books", secondary="book_genre", default=None)
    libraries: Mapped[list["Library"]] = relationship(back_populates="books", secondary="library_book", default=None)


class BookAuthor(BaseAssociativeEntity):
    __tablename__ = "book_author"

    book_id: Mapped[UUID] = mapped_column(ForeignKey("books.id"), primary_key=True)
    author_id: Mapped[UUID] = mapped_column(ForeignKey("authors.id"), primary_key=True)

# ----- Genres -----

class Genre(BaseEntity):
    __tablename__ = "genres"

    name: Mapped[str]
    description: Mapped[str]

    books: Mapped[list["Book"]] = relationship(back_populates="genres", secondary="book_genre", default=None)


class BookGenre(BaseAssociativeEntity):
    __tablename__ = "book_genre"

    genre_id: Mapped[UUID] = mapped_column(ForeignKey("genres.id"), primary_key=True)
    book_id: Mapped[UUID] = mapped_column(ForeignKey("books.id"), primary_key=True)


# ----- Libraries -----

class Library(BaseEntity):
    __tablename__ = "libraries"

    name: Mapped[str]
    user_id: Mapped[UUID] = mapped_column(ForeignKey("users.id"))

    books: Mapped[list["Book"]] = relationship(back_populates="libraries", secondary="library_book")
    user: Mapped["User"] = relationship(back_populates="libraries")



class LibraryBook(BaseAssociativeEntity):
    __tablename__ = "library_book"

    library_id: Mapped[int] = mapped_column(ForeignKey("libraries.id"), primary_key=True)
    book_id: Mapped[int] = mapped_column(ForeignKey("books.id"), primary_key=True)


# ---- users ----
class User(BaseEntity):
    __tablename__ = "users"

    name: Mapped[str]
    email: Mapped[str]

    libraries: Mapped["Library"] = relationship(back_populates="user")
    reviews: Mapped["Review"] = relationship(back_populates="user")
    reading_pocesses: Mapped[list["ReadingProcess"]] = relationship(back_populates="user")


# ---- reading process ----

class ReadingProcess(BaseAssociativeEntity): # TODO
    __tablename__ = "reading_process"

    user_id: Mapped[UUID] = mapped_column(ForeignKey("users.id"), primary_key=True)
    book_id: Mapped[UUID] = mapped_column(ForeignKey("books.id"), primary_key=True)
    current_page: Mapped[int]
    start_date: Mapped[datetime]
    end_date: Mapped[datetime | None]

    user: Mapped["User"] = relationship(back_populates="reading_pocesses")
    # books: Mapped["Book"] = relationship


# ---- reviews ----

class Review(BaseEntity):
    __tablename__ = "reviews"

    book_id: Mapped[UUID] = mapped_column(ForeignKey("books.id"))
    user_id: Mapped[UUID] = mapped_column(ForeignKey("users.id"))
    rating: Mapped[float]
    comment: Mapped[str | None]

    user: Mapped["User"] = relationship(back_populates="reviews")
