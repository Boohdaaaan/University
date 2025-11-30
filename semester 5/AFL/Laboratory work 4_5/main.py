import re
import sys

# Регулярні вирази для полів. Номер накладної: рівно 10 символів (цифри, латинські літери)
INVOICE_NUMBER = r"[A-Za-z0-9]{10}"

# Кількість: дійсне число ≥0 з рівно 3 знаками після крапки
QUANTITY = r"\d+\.\d{3}"

# Назва товару
PRODUCT_NAME = r"[A-Za-z0-9\'\-_]{1,22}"

# Вартість: дійсне число ≥0
COST = r"\d+(?:\.\d+)?"

# Ціна: дійсне число ≥0 з рівно 3 знаками після крапки
PRICE = r"\d+\.\d{3}"

# Номер товарної позиції: натуральне число
POSITION = r"\d+"

# Роздільники: ; : ? з можливими білими символами навколо
SEPARATOR = r"\s*[;:?]\s*"


def create_validation_regex():
    pattern = (
        r"^\s*"  # Початок з можливими білими символами
        + INVOICE_NUMBER
        + SEPARATOR
        + QUANTITY
        + SEPARATOR
        + PRODUCT_NAME
        + SEPARATOR
        + COST
        + SEPARATOR
        + PRICE
        + SEPARATOR
        + POSITION
        + r"\s*$"  # Кінець з можливими білими символами
    )

    return re.compile(pattern)


def parse_line(line):
    line = line.strip()
    if not line:
        return None

    # Регулярний вираз для виділення полів
    pattern = re.compile(
        r"^\s*"
        + f"({INVOICE_NUMBER})"
        + SEPARATOR
        + f"({QUANTITY})"
        + SEPARATOR
        + f"({PRODUCT_NAME})"
        + SEPARATOR
        + f"({COST})"
        + SEPARATOR
        + f"({PRICE})"
        + SEPARATOR
        + f"({POSITION})"
        + r"\s*$",
        re.UNICODE,
    )

    match = pattern.match(line)
    if match:
        return {
            "invoice_number": match.group(1),
            "quantity": match.group(2),
            "product_name": match.group(3),
            "cost": match.group(4),
            "price": match.group(5),
            "position": match.group(6),
        }
    return None


def is_price_zero(parsed_record):
    try:
        price = float(parsed_record["price"])
        return price == 0.0
    except Exception:
        return False


def filter_and_reformat(input_file, output_file):
    validation_regex = create_validation_regex()
    filtered_count = 0

    with open(input_file, "r", encoding="utf-8") as infile:
        with open(output_file, "w", encoding="utf-8") as outfile:
            for line_num, line in enumerate(infile, 1):
                # Перевіряємо валідність рядка
                if validation_regex.match(line.strip()):
                    # Парсимо рядок
                    parsed = parse_line(line)
                    if parsed and is_price_zero(parsed):
                        # Переформатовуємо рядок
                        reformatted = re.sub(SEPARATOR, " ; ", line.strip())
                        outfile.write(reformatted + "\n")
                        filtered_count += 1

    print(f"Обробку завершено. Знайдено {filtered_count} записів з нульовою ціною.")


def main():
    input_file = sys.argv[1]
    output_file = sys.argv[2]

    filter_and_reformat(input_file, output_file)
    print(f"Результат записано у файл '{output_file}'.")


if __name__ == "__main__":
    main()
