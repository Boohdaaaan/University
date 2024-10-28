import re
import operator

from models import Grid, CellReference, OperatorType


class FormulaCalculator:
    def __init__(self, grid: Grid):
        self.grid = grid
        self.operators = {
            '+': (operator.add, OperatorType.BINARY),
            '-': (operator.sub, OperatorType.BINARY),
            '*': (operator.mul, OperatorType.BINARY),
            '/': (operator.truediv, OperatorType.BINARY),
            '**': (operator.pow, OperatorType.BINARY),
            'mod': (operator.mod, OperatorType.BINARY),
            'div': (operator.floordiv, OperatorType.BINARY),
            'inc': (lambda x: x + 1, OperatorType.UNARY),
            'dec': (lambda x: x - 1, OperatorType.UNARY)
        }


    def evaluate(self, formula: str, current_row: int, current_col: int) -> float:
        """Evaluate a formula string and return the result"""
        formula = formula.strip()
        
        # Handle unary operators
        for op, (func, op_type) in self.operators.items():
            if op_type == OperatorType.UNARY and formula.lower().startswith(f"{op}(") and formula.endswith(")"):
                inner_expr = formula[len(op)+1:-1]
                inner_value = self.evaluate(inner_expr, current_row, current_col)
                return func(inner_value)

        # Replace operators and cell references
        formula = self._replace_word_operators(formula)
        formula = self._replace_cell_references(formula, current_row, current_col)
        
        try:
            # Create safe evaluation environment
            safe_dict = {
                op: func for op, (func, _) in self.operators.items() 
                if _ == OperatorType.BINARY
            }
            # Add the power operator specifically
            safe_dict['**'] = operator.pow
            
            # Evaluate and return result
            return float(eval(formula.replace(' ', ''), {"__builtins__": {}}, safe_dict))
        except Exception as e:
            raise ValueError(f"Invalid formula: {str(e)}")


    def _replace_word_operators(self, formula: str) -> str:
        """Replace word operators with their symbol equivalents"""
        # Replace power operator ^ with **
        formula = formula.replace('^', '**')
        formula = re.sub(r'\bmod\b', '%', formula, flags=re.IGNORECASE)
        formula = re.sub(r'\bdiv\b', '//', formula, flags=re.IGNORECASE)
        return formula


    def _replace_cell_references(self, formula: str, current_row: int, current_col: int) -> str:
        while True:
            cell_ref_match = re.search(r'[A-Z]+\d+', formula)
            if not cell_ref_match:
                break
            
            cell_ref = cell_ref_match.group()
            cell_coords = CellReference.from_string(cell_ref)
            
            if cell_coords.row == current_row and cell_coords.column == current_col:
                raise ValueError("Circular reference detected")
            
            cell = self.grid.get_cell(cell_coords.row, cell_coords.column)
            try:
                value = float(cell.value) if cell.value is not None else 0
            except (ValueError, TypeError):
                raise ValueError(f"Cell {cell_ref} does not contain a number")
            
            formula = formula.replace(cell_ref, str(value))
        
        return formula
        