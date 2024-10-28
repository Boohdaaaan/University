import tkinter as tk
from tkinter import ttk, messagebox, filedialog

from models import Grid, Cell
from storage import GridStorage
from calculator import FormulaCalculator


class ExcelGUI:
    def __init__(self, root: tk.Tk, grid: 'Grid', calculator: 'FormulaCalculator'):
        self.root = root
        self.grid = grid
        self.calculator = calculator
        self.setup_ui()


    def setup_ui(self):
        """Initialize the main UI components"""
        self.root.title("Laboratory work 1")
        self.main_frame = ttk.Frame(self.root)
        self.main_frame.pack(fill=tk.BOTH, expand=True)
        
        self.create_toolbar()
        self.create_grid_frame()
        self.create_grid()


    def create_toolbar(self):
        """Create the button toolbar"""
        toolbar = ttk.Frame(self.main_frame)
        toolbar.pack(fill=tk.X, padx=5, pady=5)
        
        buttons = [
            ("Зберегти", self.save_table),
            ("Відкрити", self.load_table),
            ("Додати рядок", self.add_row),
            ("Додати колонку", self.add_column),
            ("Видалити рядок", self.delete_row),
            ("Видалити колонку", self.delete_column),
            ("Довідка", self.show_help),
            ("Вийти", self.exit_app)
        ]
        
        for text, command in buttons:
            ttk.Button(toolbar, text=text, command=command).pack(side=tk.LEFT, padx=2)


    def create_grid_frame(self):
        """Create the scrollable frame for the grid"""
        self.grid_frame = ttk.Frame(self.main_frame)
        self.grid_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # Create canvas and scrollbars
        self.canvas = tk.Canvas(self.grid_frame)
        self.vsb = ttk.Scrollbar(self.grid_frame, orient="vertical", command=self.canvas.yview)
        self.hsb = ttk.Scrollbar(self.grid_frame, orient="horizontal", command=self.canvas.xview)
        
        self.canvas.configure(yscrollcommand=self.vsb.set, xscrollcommand=self.hsb.set)
        self.table_frame = ttk.Frame(self.canvas)
        
        # Grid layout
        self.vsb.pack(side=tk.RIGHT, fill=tk.Y)
        self.hsb.pack(side=tk.BOTTOM, fill=tk.X)
        self.canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        
        self.canvas.create_window((0, 0), window=self.table_frame, anchor="nw")
        self.table_frame.bind("<Configure>", self._on_frame_configure)
        self.canvas.bind("<Configure>", self._on_canvas_configure)


    def _on_frame_configure(self, event=None):
        """Reset the scroll region to encompass the inner frame"""
        self.canvas.configure(scrollregion=self.canvas.bbox("all"))


    def _on_canvas_configure(self, event):
        """Reset the canvas window to encompass inner frame when required"""
        self.canvas.itemconfig("all", width=event.width)


    def save_table(self):
        """Save the table data to a JSON file"""
        try:
            file_path = filedialog.asksaveasfilename(
                defaultextension=".json",
                filetypes=[("JSON files", "*.json"), ("All files", "*.*")]
            )
            
            if file_path:
                # Before saving, ensure all formulas are properly stored with their current values
                formula_cells = {
                    key: cell
                    for key, cell in self.grid.cells.items()
                    if cell.formula
                }
                
                # Update values for formula cells
                for key, cell in formula_cells.items():
                    row, col = map(int, key.split(','))
                    try:
                        result = self.calculator.evaluate(cell.formula[1:], row, col)
                        cell.value = result
                    except Exception as e:
                        print(f"Error evaluating formula at {key}: {str(e)}")
                        cell.value = "ERROR"
                
                # Save the grid
                GridStorage.save_to_json(file_path, self.grid)
                messagebox.showinfo("Успіх", "Файл збережено!")
                
        except Exception as e:
            messagebox.showerror("Помилка", f"Не вдалося зберегти файл: {str(e)}")
    

    def create_grid(self):
        """Create the Excel-like grid"""
        # Clear existing grid
        for widget in self.table_frame.winfo_children():
            widget.destroy()

        # Create headers
        ttk.Label(self.table_frame, text="").grid(row=0, column=0)
        for col in range(self.grid.columns):
            ttk.Label(self.table_frame, text=self.grid.get_column_name(col + 1)).grid(row=0, column=col + 1)
        
        # Create cells
        for row in range(self.grid.rows):
            ttk.Label(self.table_frame, text=str(row + 1)).grid(row=row + 1, column=0)
            for col in range(self.grid.columns):
                cell = ttk.Entry(self.table_frame, width=10)
                cell.grid(row=row + 1, column=col + 1, padx=1, pady=1)
                
                # Bind events for cell interaction
                cell.bind('<FocusOut>', 
                    lambda e, row=row, col=col: self._on_cell_changed(row, col))
                cell.bind('<FocusIn>', 
                    lambda e, row=row, col=col: self._on_cell_focused(row, col))
                
                # Populate existing values
                existing_cell = self.grid.get_cell(row, col)
                if existing_cell.value is not None:
                    cell.insert(0, str(existing_cell.value))


    def _on_cell_focused(self, row: int, col: int):
        """Handle cell focus event - show formula if exists"""
        cell = self.grid.get_cell(row, col)
        if cell.formula:
            widget = self.table_frame.grid_slaves(row=row + 1, column=col + 1)[0]
            widget.delete(0, tk.END)
            widget.insert(0, cell.formula)
            widget.select_range(0, tk.END)  # Select all text for easy editing


    def _on_cell_changed(self, row: int, col: int):
        """Handle cell value changes and formula evaluation"""
        widget = self.table_frame.grid_slaves(row=row + 1, column=col + 1)[0]
        value = widget.get().strip()
        
        if not value:
            self.grid.clear_cell(row, col)
            return
            
        if value.startswith('='):
            try:
                result = self.calculator.evaluate(value[1:], row, col)
                self.grid.set_cell(row, col, Cell(value=result, formula=value))
                
                # Only update display if the cell is not focused
                if widget != self.root.focus_get():
                    widget.delete(0, tk.END)
                    widget.insert(0, str(result))
            except Exception as e:
                messagebox.showerror("Помилка формули", str(e))
                widget.delete(0, tk.END)
                widget.insert(0, "ERROR")
        else:
            try:
                float_value = float(value)
                self.grid.set_cell(row, col, Cell(value=float_value))
            except ValueError:
                self.grid.set_cell(row, col, Cell(value=value))

        self._update_dependent_cells(row, col)


    def _update_dependent_cells(self, changed_row: int, changed_col: int):
        """Update all cells that depend on the changed cell"""
        changed_ref = f"{self.grid.get_column_name(changed_col + 1)}{changed_row + 1}"
        
        for key, cell in self.grid.cells.items():
            if cell.formula and changed_ref in cell.formula:
                row, col = map(int, key.split(','))
                widget = self.table_frame.grid_slaves(row=row + 1, column=col + 1)[0]
                
                # Skip updating display if cell is currently being edited
                if widget == self.root.focus_get():
                    continue
                    
                try:
                    result = self.calculator.evaluate(cell.formula[1:], row, col)
                    cell.value = result
                    widget.delete(0, tk.END)
                    widget.insert(0, str(result))
                except Exception as e:
                    print(f"Error evaluating formula at {row},{col}: {str(e)}")
                    widget.delete(0, tk.END)
                    widget.insert(0, "ERROR")


    def _evaluate_all_formulas(self):
        """Evaluate all formulas in the grid"""
        # First, collect all cells with formulas
        formula_cells = [(int(row), int(col), cell) 
                        for (row, col), cell in 
                        ((tuple(map(int, key.split(','))), cell) 
                         for key, cell in self.grid.cells.items())
                        if cell.formula]
        
        # Then evaluate each formula and update the display
        for row, col, cell in formula_cells:
            try:
                # Get the widget for this cell
                widget = self.table_frame.grid_slaves(row=row + 1, column=col + 1)[0]
                
                # Evaluate the formula
                result = self.calculator.evaluate(cell.formula[1:], row, col)
                
                # Update the cell value and display
                cell.value = result
                widget.delete(0, tk.END)
                widget.insert(0, str(result))
            except Exception as e:
                print(f"Error evaluating formula at {row},{col}: {str(e)}")
                widget.delete(0, tk.END)
                widget.insert(0, "ERROR")


    def load_table(self):
        """Load table data from a JSON file"""
        try:
            file_path = filedialog.askopenfilename(
                filetypes=[("JSON files", "*.json"), ("All files", "*.*")]
            )
            
            if file_path:
                new_grid = GridStorage.load_from_json(file_path)
                self.grid.rows = new_grid.rows
                self.grid.columns = new_grid.columns
                self.grid.cells = new_grid.cells
                
                self.create_grid()
                self._evaluate_all_formulas()
                messagebox.showinfo("Успіх", "Файл завантажено!")
        except Exception as e:
            messagebox.showerror("Помилка", f"Не вдалося відкрити файл: {str(e)}")


    def add_row(self):
        """Add a new row to the grid"""
        self.grid.rows += 1
        row = self.grid.rows - 1
        
        ttk.Label(self.table_frame, text=str(self.grid.rows)).grid(row=self.grid.rows, column=0)
        for col in range(self.grid.columns):
            cell = ttk.Entry(self.table_frame, width=10)
            cell.grid(row=self.grid.rows, column=col + 1, padx=1, pady=1)
            cell.bind('<FocusOut>', lambda e, r=row, c=col: self._on_cell_changed(r, c))
        
        self._on_frame_configure()


    def add_column(self):
        """Add a new column to the grid"""
        self.grid.columns += 1
        col = self.grid.columns - 1
        
        ttk.Label(self.table_frame, text=self.grid.get_column_name(self.grid.columns)).grid(
            row=0, column=self.grid.columns)
            
        for row in range(self.grid.rows):
            cell = ttk.Entry(self.table_frame, width=10)
            cell.grid(row=row + 1, column=self.grid.columns, padx=1, pady=1)
            cell.bind('<FocusOut>', lambda e, r=row, c=col: self._on_cell_changed(r, c))
        
        self._on_frame_configure()


    def delete_row(self):
        """Delete the last row from the grid"""
        if self.grid.rows > 1:
            # Remove widgets in the last row
            for widget in self.table_frame.grid_slaves(row=self.grid.rows):
                widget.destroy()
            
            # Remove data for deleted row
            keys_to_delete = [key for key in self.grid.cells.keys() 
                            if int(key.split(',')[0]) == self.grid.rows - 1]
            for key in keys_to_delete:
                self.grid.clear_cell(*map(int, key.split(',')))
            
            self.grid.rows -= 1
            self._on_frame_configure()
            self._update_all_formulas()


    def delete_column(self):
        """Delete the last column from the grid"""
        if self.grid.columns > 1:
            # Remove widgets in the last column
            for widget in self.table_frame.grid_slaves(column=self.grid.columns):
                widget.destroy()
            
            # Remove data for deleted column
            keys_to_delete = [key for key in self.grid.cells.keys() 
                            if int(key.split(',')[1]) == self.grid.columns - 1]
            for key in keys_to_delete:
                self.grid.clear_cell(*map(int, key.split(',')))
            
            self.grid.columns -= 1
            self._on_frame_configure()
            self._update_all_formulas()


    def _update_all_formulas(self):
        """Update all formula cells"""
        for key, cell in list(self.grid.cells.items()):
            if cell.formula:
                row, col = map(int, key.split(','))
                self._on_cell_changed(row, col)


    def show_help(self):
        """Show help information"""
        help_text = """\
Лабораторна робота №1

Доступні операції з формулами:
1. Основні арифметичні операції: +, -, *, /, ^
2. Спеціальні операції:
   - mod: операція модуля (остача від ділення)
   - div: цілочисельне ділення
   - inc(): збільшення на 1
   - dec(): зменшення на 1

Приклади формул:
1. =A1 + B1
2. =A1 mod 3
3. =A1 div 2
4. =inc(A1)
5. =dec(B2)
6. =inc(A1 + B1)
7. =(A1 mod 3) + B1

Посилання на комірки:
- Використовуйте літери для позначення стовпців та цифри для позначення рядків
- Приклад: A1, B2, C3

Богдан Кузнецов К-25"""
        messagebox.showinfo("Help", help_text)


    def exit_app(self):
        """Exit the application with confirmation"""
        if messagebox.askyesno("Зберегти зміни?", "Бажаєте зберегти зміни перед виходом?"):
            self.save_table()
        if messagebox.askyesno("Підтвердити вихід", "Ви впевнені, що хочете вийти?"):
            self.root.quit()
            