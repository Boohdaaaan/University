from flask import Flask, render_template, request, redirect, url_for, abort, flash, session
from sqlalchemy import create_engine, select, or_, func, desc, insert, update, delete
from sqlalchemy.orm import sessionmaker, joinedload
from uuid import UUID
from functools import wraps
from flask_login import LoginManager, login_user, logout_user, login_required, current_user
from werkzeug.security import generate_password_hash

from database.books import Book, Author, Genre, User
from config import DatabaseSettings

app = Flask(__name__)
app.secret_key = 'your_secret_key_here'  # Change this to a secure random key in production

# Configure SQLAlchemy
settings = DatabaseSettings()
DATABASE_URL = settings.get_database_url()
engine = create_engine(DATABASE_URL)
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)

# Configure Flask-Login
login_manager = LoginManager()
login_manager.init_app(app)
login_manager.login_view = 'login'

@login_manager.user_loader
def load_user(user_id):
    db = SessionLocal()
    try:
        user = db.execute(select(User).where(User.id == UUID(user_id))).scalar_one_or_none()
        return user
    finally:
        db.close()

def admin_required(f):
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if not current_user.is_authenticated or not current_user.is_admin:
            flash('You need administrator privileges to access this page.', 'danger')
            return redirect(url_for('login'))
        return f(*args, **kwargs)
    return decorated_function

# Authentication routes
@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        email = request.form.get('email')
        password = request.form.get('password')
        
        db = SessionLocal()
        try:
            user = db.execute(select(User).where(User.email == email)).scalar_one_or_none()
            
            if user and user.check_password(password):
                login_user(user)
                next_page = request.args.get('next', url_for('index'))
                return redirect(next_page)
            
            flash('Invalid email or password', 'danger')
            return render_template('login.html')
        finally:
            db.close()
    
    return render_template('login.html')

@app.route('/register', methods=['GET', 'POST'])
def register():
    if request.method == 'POST':
        name = request.form.get('name')
        email = request.form.get('email')
        password = request.form.get('password')
        
        db = SessionLocal()
        try:
            existing_user = db.execute(select(User).where(User.email == email)).scalar_one_or_none()
            
            if existing_user:
                flash('Email already registered', 'danger')
                return render_template('register.html')
            
            # Create new user
            new_user = User(
                name=name,
                email=email,
                is_admin=False
            )
            new_user.set_password(password)
            
            db.add(new_user)
            db.commit()
            
            flash('Registration successful! Please log in.', 'success')
            return redirect(url_for('login'))
        finally:
            db.close()
    
    return render_template('register.html')

@app.route('/logout')
@login_required
def logout():
    logout_user()
    return redirect(url_for('index'))

# Existing routes

@app.route('/')
def index():
    db = SessionLocal()
    try:
        result = db.execute(
            select(Book).options(joinedload(Book.written_by))
        )
        books = result.unique().scalars().all()
        return render_template('index.html', books=books)
    finally:
        db.close()

@app.route('/book/<string:book_id>')
def book_detail(book_id):
    try:
        book_uuid = UUID(book_id)
    except ValueError:
        abort(404)

    # If this is a search submission from the navbar, redirect to search
    if request.args.get('_redirect') == '/search' and request.args.get('q'):
        return redirect(url_for('search', q=request.args.get('q')))

    db = SessionLocal()
    try:
        result = db.execute(
            select(Book)
            .where(Book.id == book_uuid)
            .options(
                joinedload(Book.written_by),
                joinedload(Book.genres)
            )
        )
        book = result.unique().scalar_one_or_none()
        if book is None:
            abort(404)
        return render_template('book_detail.html', book=book)
    finally:
        db.close()

@app.route('/authors')
def authors():
    sort = request.args.get('sort', 'name')  # Default sort by name
    search_query = request.args.get('search', '')  # Get search query
    db = SessionLocal()
    try:
        # Query for the main authors view
        query = select(Author).options(
            joinedload(Author.written_books)
        )
        
        # Add search filter if search_query is present
        if search_query:
            query = query.filter(Author.name.ilike(f'%{search_query}%'))

        if sort == 'books':
            # Sort by number of books
            query = query.order_by(func.count(Book.id).desc())
            query = query.join(Author.written_books).group_by(Author.id)
        else:
            # Sort by name
            query = query.order_by(Author.name)

        result = db.execute(query)
        authors = result.unique().scalars().all()
        
        # Create data for top 5 authors chart
        # Get the authors with most books
        top_authors_query = select(Author, func.count(Book.id).label('book_count'))\
            .join(Author.written_books)\
            .group_by(Author.id)\
            .order_by(func.count(Book.id).desc())\
            .limit(5)
            
        top_authors_result = db.execute(top_authors_query)
        
        # Prepare data for chart
        author_names = []
        book_counts = []
        
        for author, count in top_authors_result:
            author_names.append(author.name)
            book_counts.append(count)
        
        # Create plotly chart with nicer styling
        import plotly.graph_objects as go
        import json
        
        fig = go.Figure([
            go.Bar(
                x=author_names, 
                y=book_counts,
                marker=dict(
                    color='rgba(50, 171, 96, 0.7)',
                    line=dict(color='rgba(50, 171, 96, 1.0)', width=2)
                )
            )
        ])
        fig.update_layout(
            title={
                'text': 'Top 5 Authors by Number of Books',
                'y':0.9,
                'x':0.5,
                'xanchor': 'center',
                'yanchor': 'top'
            },
            xaxis_title='Author',
            yaxis_title='Number of Books',
            template='plotly_white',
            plot_bgcolor='rgba(240, 240, 240, 0.5)',
            margin=dict(l=40, r=40, t=80, b=40),
            height=400
        )
        
        # Convert the figure to JSON for embedding in the template
        authors_chart_json = json.dumps(fig.to_dict())
        
        return render_template(
            'authors.html', 
            authors=authors, 
            search_query=search_query,
            authors_chart_json=authors_chart_json
        )
    finally:
        db.close()

@app.route('/genres')
def genres():
    sort = request.args.get('sort', 'name')  # Default sort by name
    search_query = request.args.get('search', '')  # Get search query
    db = SessionLocal()
    try:
        # Base query with all necessary relationships
        query = select(Genre).options(
            joinedload(Genre.books).joinedload(Book.written_by)
        )
        
        # Add search filter if search_query is present
        if search_query:
            query = query.filter(Genre.name.ilike(f'%{search_query}%'))

        if sort == 'books':
            # Sort by number of books
            query = query.order_by(func.count(Book.id).desc())
            query = query.join(Genre.books).group_by(Genre.id)
        elif sort == 'avg_price':
            # Sort by average book price
            query = query.order_by(func.avg(Book.price).desc())
            query = query.join(Genre.books).group_by(Genre.id)
        else:
            # Sort by name
            query = query.order_by(Genre.name)

        result = db.execute(query)
        genres = result.unique().scalars().all()
        
        # Create data for genre popularity pie chart
        genre_book_count_query = select(Genre.name, func.count(Book.id).label('book_count'))\
            .join(Genre.books)\
            .group_by(Genre.id)\
            .order_by(func.count(Book.id).desc())
            
        genre_book_counts = db.execute(genre_book_count_query).all()
        
        # Prepare data for chart
        genre_names = []
        book_counts = []
        
        for name, count in genre_book_counts:
            genre_names.append(name)
            book_counts.append(count)
        
        # Create plotly chart with nicer styling
        import plotly.graph_objects as go
        import json
        
        fig = go.Figure(data=[
            go.Pie(
                labels=genre_names, 
                values=book_counts, 
                hole=.3,
                textinfo='label+percent',
                insidetextorientation='radial',
                marker=dict(
                    line=dict(color='#ffffff', width=2)
                )
            )
        ])
        fig.update_layout(
            title={
                'text': 'Book Distribution by Genre',
                'y':0.95,
                'x':0.5,
                'xanchor': 'center',
                'yanchor': 'top'
            },
            template='plotly_white',
            margin=dict(l=20, r=20, t=80, b=20),
            height=400,
            showlegend=False if len(genre_names) > 10 else True,
            legend=dict(
                orientation="h",
                yanchor="bottom",
                y=-0.2,
                xanchor="center",
                x=0.5
            )
        )
        
        # Convert the figure to JSON for embedding in the template
        genres_chart_json = json.dumps(fig.to_dict())
        
        return render_template(
            'genres.html', 
            genres=genres, 
            search_query=search_query,
            genres_chart_json=genres_chart_json
        )
    finally:
        db.close()

@app.route('/search')
def search():
    query = request.args.get('q', '')
    db = SessionLocal()
    try:
        result = db.execute(
            select(Book)
            .join(Book.written_by)
            .where(
                or_(
                    Book.title.ilike(f'%{query}%'),
                    Author.name.ilike(f'%{query}%')
                )
            )
            .options(joinedload(Book.written_by))
        )
        books = result.unique().scalars().all()
        return render_template('search.html', books=books, query=query)
    finally:
        db.close()

# Admin CRUD routes for Books
@app.route('/admin/books')
@admin_required
def admin_books():
    db = SessionLocal()
    try:
        result = db.execute(
            select(Book).options(
                joinedload(Book.written_by),
                joinedload(Book.genres)
            )
        )
        books = result.unique().scalars().all()
        return render_template('admin/books.html', books=books)
    finally:
        db.close()

@app.route('/admin/books/new', methods=['GET', 'POST'])
@admin_required
def admin_new_book():
    if request.method == 'POST':
        db = SessionLocal()
        try:
            # Create new book
            new_book = Book(
                title=request.form.get('title'),
                year=int(request.form.get('year')),
                price=float(request.form.get('price')),
                description=request.form.get('description'),
                isbn=request.form.get('isbn'),
                average_rating=float(request.form.get('average_rating')) if request.form.get('average_rating') else None,
                text_link=request.form.get('text_link')
            )
            
            # Add authors
            author_ids = request.form.getlist('authors')
            if author_ids:
                authors = db.execute(
                    select(Author).where(Author.id.in_([UUID(aid) for aid in author_ids]))
                ).scalars().all()
                new_book.written_by = authors
            
            # Add genres
            genre_ids = request.form.getlist('genres')
            if genre_ids:
                genres = db.execute(
                    select(Genre).where(Genre.id.in_([UUID(gid) for gid in genre_ids]))
                ).scalars().all()
                new_book.genres = genres
            
            db.add(new_book)
            db.commit()
            
            flash('Book added successfully!', 'success')
            return redirect(url_for('admin_books'))
        except Exception as e:
            db.rollback()
            flash(f'Error adding book: {str(e)}', 'danger')
            return redirect(url_for('admin_new_book'))
        finally:
            db.close()
    
    # GET request - show form
    db = SessionLocal()
    try:
        authors = db.execute(select(Author).order_by(Author.name)).scalars().all()
        genres = db.execute(select(Genre).order_by(Genre.name)).scalars().all()
        return render_template('admin/book_form.html', book=None, authors=authors, genres=genres)
    finally:
        db.close()

@app.route('/admin/books/edit/<string:book_id>', methods=['GET', 'POST'])
@admin_required
def admin_edit_book(book_id):
    try:
        book_uuid = UUID(book_id)
    except ValueError:
        abort(404)
    
    db = SessionLocal()
    
    if request.method == 'POST':
        try:
            book = db.execute(
                select(Book).where(Book.id == book_uuid)
            ).scalar_one_or_none()
            
            if not book:
                abort(404)
            
            # Update book fields
            book.title = request.form.get('title')
            book.year = int(request.form.get('year'))
            book.price = float(request.form.get('price'))
            book.description = request.form.get('description')
            book.isbn = request.form.get('isbn')
            book.average_rating = float(request.form.get('average_rating')) if request.form.get('average_rating') else None
            book.text_link = request.form.get('text_link')
            
            # Update authors
            author_ids = request.form.getlist('authors')
            if author_ids:
                authors = db.execute(
                    select(Author).where(Author.id.in_([UUID(aid) for aid in author_ids]))
                ).scalars().all()
                book.written_by = authors
            else:
                book.written_by = []
            
            # Update genres
            genre_ids = request.form.getlist('genres')
            if genre_ids:
                genres = db.execute(
                    select(Genre).where(Genre.id.in_([UUID(gid) for gid in genre_ids]))
                ).scalars().all()
                book.genres = genres
            else:
                book.genres = []
            
            db.commit()
            
            flash('Book updated successfully!', 'success')
            return redirect(url_for('admin_books'))
        except Exception as e:
            db.rollback()
            flash(f'Error updating book: {str(e)}', 'danger')
            return redirect(url_for('admin_edit_book', book_id=book_id))
        finally:
            db.close()
    
    # GET request - show form with book data
    try:
        book = db.execute(
            select(Book)
            .where(Book.id == book_uuid)
            .options(
                joinedload(Book.written_by),
                joinedload(Book.genres)
            )
        ).scalar_one_or_none()
        
        if not book:
            abort(404)
        
        authors = db.execute(select(Author).order_by(Author.name)).scalars().all()
        genres = db.execute(select(Genre).order_by(Genre.name)).scalars().all()
        
        return render_template('admin/book_form.html', book=book, authors=authors, genres=genres)
    finally:
        db.close()

@app.route('/admin/books/delete/<string:book_id>', methods=['POST'])
@admin_required
def admin_delete_book(book_id):
    try:
        book_uuid = UUID(book_id)
    except ValueError:
        abort(404)
    
    db = SessionLocal()
    try:
        book = db.execute(select(Book).where(Book.id == book_uuid)).scalar_one_or_none()
        
        if not book:
            abort(404)
        
        db.delete(book)
        db.commit()
        
        flash('Book deleted successfully!', 'success')
        return redirect(url_for('admin_books'))
    except Exception as e:
        db.rollback()
        flash(f'Error deleting book: {str(e)}', 'danger')
        return redirect(url_for('admin_books'))
    finally:
        db.close()

# Similar CRUD routes for authors and genres
# I'll provide the structure for authors as an example:

@app.route('/admin/authors')
@admin_required
def admin_authors():
    db = SessionLocal()
    try:
        authors = db.execute(
            select(Author).options(joinedload(Author.written_books))
        ).unique().scalars().all()
        return render_template('admin/authors.html', authors=authors)
    finally:
        db.close()

@app.route('/admin/authors/new', methods=['GET', 'POST'])
@admin_required
def admin_new_author():
    if request.method == 'POST':
        db = SessionLocal()
        try:
            new_author = Author(
                name=request.form.get('name'),
                biography=request.form.get('biography')
            )
            db.add(new_author)
            db.commit()
            
            flash('Author added successfully!', 'success')
            return redirect(url_for('admin_authors'))
        except Exception as e:
            db.rollback()
            flash(f'Error adding author: {str(e)}', 'danger')
            return redirect(url_for('admin_new_author'))
        finally:
            db.close()
    
    return render_template('admin/author_form.html', author=None)

@app.route('/admin/authors/edit/<string:author_id>', methods=['GET', 'POST'])
@admin_required
def admin_edit_author(author_id):
    try:
        author_uuid = UUID(author_id)
    except ValueError:
        abort(404)
    
    db = SessionLocal()
    
    if request.method == 'POST':
        try:
            author = db.execute(
                select(Author).where(Author.id == author_uuid)
            ).scalar_one_or_none()
            
            if not author:
                abort(404)
            
            # Update author fields
            author.name = request.form.get('name')
            author.biography = request.form.get('biography')
            
            db.commit()
            
            flash('Author updated successfully!', 'success')
            return redirect(url_for('admin_authors'))
        except Exception as e:
            db.rollback()
            flash(f'Error updating author: {str(e)}', 'danger')
            return redirect(url_for('admin_edit_author', author_id=author_id))
        finally:
            db.close()
    
    # GET request - show form with author data
    try:
        author = db.execute(
            select(Author)
            .where(Author.id == author_uuid)
            .options(joinedload(Author.written_books))
        ).scalar_one_or_none()
        
        if not author:
            abort(404)
        
        return render_template('admin/author_form.html', author=author)
    finally:
        db.close()

@app.route('/admin/authors/delete/<string:author_id>', methods=['POST'])
@admin_required
def admin_delete_author(author_id):
    try:
        author_uuid = UUID(author_id)
    except ValueError:
        abort(404)
    
    db = SessionLocal()
    try:
        author = db.execute(select(Author).where(Author.id == author_uuid)).scalar_one_or_none()
        
        if not author:
            abort(404)
        
        db.delete(author)
        db.commit()
        
        flash('Author deleted successfully!', 'success')
        return redirect(url_for('admin_authors'))
    except Exception as e:
        db.rollback()
        flash(f'Error deleting author: {str(e)}', 'danger')
        return redirect(url_for('admin_authors'))
    finally:
        db.close()

@app.route('/profile')
@login_required
def profile():
    return render_template('profile.html')

# Genre admin routes
@app.route('/admin/genres')
@admin_required
def admin_genres():
    db = SessionLocal()
    try:
        genres = db.execute(
            select(Genre).options(joinedload(Genre.books))
        ).unique().scalars().all()
        return render_template('admin/genres.html', genres=genres)
    finally:
        db.close()

@app.route('/admin/genres/new', methods=['GET', 'POST'])
@admin_required
def admin_new_genre():
    if request.method == 'POST':
        db = SessionLocal()
        try:
            new_genre = Genre(
                name=request.form.get('name'),
                description=request.form.get('description')
            )
            db.add(new_genre)
            db.commit()
            
            flash('Genre added successfully!', 'success')
            return redirect(url_for('admin_genres'))
        except Exception as e:
            db.rollback()
            flash(f'Error adding genre: {str(e)}', 'danger')
            return redirect(url_for('admin_new_genre'))
        finally:
            db.close()
    
    return render_template('admin/genre_form.html', genre=None)

@app.route('/admin/genres/edit/<string:genre_id>', methods=['GET', 'POST'])
@admin_required
def admin_edit_genre(genre_id):
    try:
        genre_uuid = UUID(genre_id)
    except ValueError:
        abort(404)
    
    db = SessionLocal()
    
    if request.method == 'POST':
        try:
            genre = db.execute(
                select(Genre).where(Genre.id == genre_uuid)
            ).scalar_one_or_none()
            
            if not genre:
                abort(404)
            
            # Update genre fields
            genre.name = request.form.get('name')
            genre.description = request.form.get('description')
            
            db.commit()
            
            flash('Genre updated successfully!', 'success')
            return redirect(url_for('admin_genres'))
        except Exception as e:
            db.rollback()
            flash(f'Error updating genre: {str(e)}', 'danger')
            return redirect(url_for('admin_edit_genre', genre_id=genre_id))
        finally:
            db.close()
    
    # GET request - show form with genre data
    try:
        genre = db.execute(
            select(Genre)
            .where(Genre.id == genre_uuid)
            .options(joinedload(Genre.books))
        ).scalar_one_or_none()
        
        if not genre:
            abort(404)
        
        return render_template('admin/genre_form.html', genre=genre)
    finally:
        db.close()

@app.route('/admin/genres/delete/<string:genre_id>', methods=['POST'])
@admin_required
def admin_delete_genre(genre_id):
    try:
        genre_uuid = UUID(genre_id)
    except ValueError:
        abort(404)
    
    db = SessionLocal()
    try:
        genre = db.execute(select(Genre).where(Genre.id == genre_uuid)).scalar_one_or_none()
        
        if not genre:
            abort(404)
        
        db.delete(genre)
        db.commit()
        
        flash('Genre deleted successfully!', 'success')
        return redirect(url_for('admin_genres'))
    except Exception as e:
        db.rollback()
        flash(f'Error deleting genre: {str(e)}', 'danger')
        return redirect(url_for('admin_genres'))
    finally:
        db.close()

if __name__ == '__main__':
    app.run(debug=True)
    