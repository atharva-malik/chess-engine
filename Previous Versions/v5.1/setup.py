def int_to_binary(num):
    """Converts a 64-bit integer to its binary representation as a string.

    Args:
      num: The 64-bit integer.

    Returns:
      A string representing the binary representation of the integer.
    """
    if num == 0:
        return "0"

    binary_string = ""
    is_negative = False

    if num < 0:
        is_negative = True
        num = abs(num)  # work with the positive value, then handle the negative.

    while num > 0:
        binary_string = str(num % 2) + binary_string
        num //= 2

    if is_negative:
        # Two's complement for negative numbers.
        # 1. Invert the bits.
        inverted_string = "".join(['1' if bit == '0' else '0' for bit in binary_string.zfill(64)]) #zfill to 64 bits

        # 2. Add 1.
        carry = 1
        temp_result = ""
        for bit in reversed(inverted_string):
            if bit == '1' and carry == 1:
                temp_result = "0" + temp_result
            elif bit == '0' and carry == 1:
                temp_result = "1" + temp_result
                carry = 0
            else:
                temp_result = bit + temp_result

        binary_string = temp_result

    else:
        binary_string = binary_string.zfill(64) #if postive, pad to 64 bits.

    return binary_string

number = 18374403900871474942
binary_representation = int_to_binary(number)
print(f"The binary representation of {number} is: {binary_representation}")

number = 9187201950435737471
binary_representation = int_to_binary(number)
print(f"The binary representation of {number} is: {binary_representation}")

number = 4557430888798830399
binary_representation = int_to_binary(number)
print(f"The binary representation of {number} is: {binary_representation}")

number = 18229723555195321596
binary_representation = int_to_binary(number)
print(f"The binary representation of {number} is: {binary_representation}")