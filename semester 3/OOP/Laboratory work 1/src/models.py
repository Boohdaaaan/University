import re
import string
from typing import Dict, Any, Optional
from dataclasses import dataclass
from enum import Enum


class OperatorType(Enum):
    BINARY = "binary"
    UNARY = "unary"


@dataclass
class CellReference:
    row: int
    column: int
    

    @staticmethod
    def from_string(cell_ref: str) -> 'CellReference':
        match = re.match(r'([A-Z]+)(\d+)', cell_ref)
        if not match:
            raise ValueError(f"Invalid cell reference: {cell_ref}")
        
        col_str, row_str = match.groups()
        
        # Convert column letters to number (A=0, B=1, etc.)
        col = 0
        for char in col_str:
            col = col * 26 + (ord(char) - ord('A') + 1)
        
        return CellReference(row=int(row_str) - 1, column=col - 1)


class Cell:
    def __init__(self, value: Any = None, formula: Optional[str] = None):
        self.value = value
        self.formula = formula


    def has_formula(self) -> bool:
        return self.formula is not None


class Grid:
    def __init__(self, rows: int = 10, columns: int = 10):
        self.rows = rows
        self.columns = columns
        self.cells: Dict[str, Cell] = {}


    def get_cell(self, row: int, column: int) -> Cell:
        key = f"{row},{column}"
        return self.cells.get(key, Cell())


    def set_cell(self, row: int, column: int, cell: Cell):
        key = f"{row},{column}"
        self.cells[key] = cell


    def clear_cell(self, row: int, column: int):
        key = f"{row},{column}"
        if key in self.cells:
            del self.cells[key]


    @staticmethod
    def get_column_name(col_index: int) -> str:
        result = ""
        while col_index > 0:
            col_index -= 1
            result = string.ascii_uppercase[col_index % 26] + result
            col_index //= 26
        return result
    