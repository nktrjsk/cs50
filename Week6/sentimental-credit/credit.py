# TODO
from cs50 import get_string

number = get_string("Number: ")


def main():
    if not luhns_algorithm([int(i) for i in number]):
        print("INVALID")

    provider = int(number[:2])
    numlen = len(number)
    if (provider == 34 or provider == 37) and numlen == 15:
        print("AMEX")
    elif 55 >= provider >= 51 and numlen == 16:
        print("MASTERCARD")
    elif 49 >= provider >= 40 and (numlen == 13 or numlen == 16):
        print("VISA")
    else:
        print("INVALID")


def luhns_algorithm(card_number: list[int]) -> bool:
    check_sum = 0

    for i, number in enumerate(card_number[::-1], 1):
        number = int(number)

        if i % 2:
            number *= 2
            check_sum += number % 10 + number // 10
        else:
            check_sum += number

    return check_sum % 10 == 0


if __name__ == "__main__":
    main()