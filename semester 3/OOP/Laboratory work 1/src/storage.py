import json

from models import Grid, Cell


class GridStorage:
    @staticmethod
    def save_to_json(filepath: str, grid: Grid):
        data = {
            "rows": grid.rows,
            "columns": grid.columns,
            "cells": {
                key: {
                    "value": cell.value,
                    "formula": cell.formula
                }
                for key, cell in grid.cells.items()
            }
        }
        
        with open(filepath, 'w') as f:
            json.dump(data, f, indent=4)


    @staticmethod
    def load_from_json(filepath: str) -> Grid:
        with open(filepath, 'r') as f:
            data = json.load(f)
        
        grid = Grid(rows=data["rows"], columns=data["columns"])
        for key, cell_data in data["cells"].items():
            row, col = map(int, key.split(','))
            grid.set_cell(row, col, Cell(
                value=cell_data["value"],
                formula=cell_data.get("formula")
            ))
        
        return grid
    