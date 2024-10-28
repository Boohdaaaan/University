import tkinter as tk

from models import Grid
from calculator import FormulaCalculator
from gui import ExcelGUI


def main():
    root = tk.Tk()
    root.geometry("1200x800")
    
    grid = Grid()
    calculator = FormulaCalculator(grid)
    gui = ExcelGUI(root, grid, calculator)
    
    root.mainloop()

if __name__ == "__main__":
    main()
    