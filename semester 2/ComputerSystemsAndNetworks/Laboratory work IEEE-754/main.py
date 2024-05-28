import struct
import math


def float_to_bin(f, bits=32):
    """Convert float to binary string representation with specified bits."""
    if bits == 32:
        # Convert 32-bit float to binary representation
        [d] = struct.unpack(">L", struct.pack(">f", f))
        return f'{d:032b}'
    elif bits == 64:
        # Convert 64-bit float to binary representation
        [d] = struct.unpack(">Q", struct.pack(">d", f))
        return f'{d:064b}'
    else:
        raise ValueError("Unsupported bit size")


def bin_to_float(b):
    """Convert binary string representation to float."""
    if len(b) == 32:
        # Convert 32-bit binary string to float
        bf = int(b, 2).to_bytes(4, byteorder='big')
        return struct.unpack('>f', bf)[0]
    elif len(b) == 64:
        # Convert 64-bit binary string to float
        bf = int(b, 2).to_bytes(8, byteorder='big')
        return struct.unpack('>d', bf)[0]
    else:
        raise ValueError("Unsupported bit size")


def ieee754_components(f, bits=32):
    """Get IEEE 754 components of the floating-point number."""
    bin_str = float_to_bin(f, bits)
    if bits == 32:
        sign = bin_str[0]  # First bit is the sign bit
        exponent = bin_str[1:9]  # Next 8 bits are the exponent
        mantissa = bin_str[9:]  # Remaining 23 bits are the mantissa
    elif bits == 64:
        sign = bin_str[0]  # First bit is the sign bit
        exponent = bin_str[1:12]  # Next 11 bits are the exponent
        mantissa = bin_str[12:]  # Remaining 52 bits are the mantissa
    return sign, exponent, mantissa


def print_fpn(f, bits=32):
    """Print floating-point number in IEEE 754 format."""
    sign, exponent, mantissa = ieee754_components(f, bits)
    print(f"Sign: {sign}")
    print(f"Exponent: {exponent}")
    print(f"Implicit bit: 1")
    print(f"Mantissa: {mantissa}")


def example_fpn_representations(bits=32):
    """Print example FPN representations."""

    # Smallest positive denormalized value (2^-126 for 32-bit)
    print("Smallest non-zero representation by absolute value:")
    smallest_non_zero = math.ldexp(1.0, -126)
    print_fpn(smallest_non_zero, bits)
    print(f"Decimal: {smallest_non_zero}\n")

    # Maximum normalized value for 32-bit
    print("Maximum positive representation:")
    max_val = (2 - math.ldexp(1.0, -23)) * math.ldexp(1.0, 127)
    print_fpn(max_val, bits)
    print(f"Decimal: {max_val}\n")

    # Smallest negative denormalized value (2^-126 for 32-bit)
    print("Smallest negative representation:")
    smallest_negative = -math.ldexp(1.0, -126)
    print_fpn(smallest_negative, bits)
    print(f"Decimal: {smallest_negative}\n")

    # Number +1.0E0
    print("Number +1.0E0:")
    print_fpn(1.0, bits)
    print(f"Decimal: 1.0\n")

    # Positive infinity
    print("Positive infinity:")
    inf = float('inf')
    print_fpn(inf, bits)
    print(f"Decimal: {inf}\n")

    # Negative infinity
    print("Negative infinity:")
    ninf = float('-inf')
    print_fpn(ninf, bits)
    print(f"Decimal: {ninf}\n")

    # Example denormalized value (smallest positive denormalized for 32-bit)
    print("Denormalized FPN example:")
    denorm = math.ldexp(1.0, -149)
    print_fpn(denorm, bits)
    print(f"Decimal: {denorm}\n")

    # NaN (Not a Number)
    print("NaN value:")
    nan = float('nan')
    print_fpn(nan, bits)
    print(f"Decimal: {nan}\n")


def main():
    # Display standard FPN representations
    example_fpn_representations(bits=32)

    # User input for decimal FPN conversion
    while True:
        try:
            user_input = input("Enter a decimal number in the format ±d.d…dE±d…d: ")
            user_number = float(user_input)
            print_fpn(user_number, bits=32)
            print(f"Decimal: {user_number}\n")
        except ValueError:
            print("Invalid input. Please try again.")
        except KeyboardInterrupt:
            print("\nExiting.")
            break


if __name__ == "__main__":
    main()
