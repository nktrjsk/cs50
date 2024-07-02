#include <cs50.h>
#include <stdio.h>
#include <math.h>

bool luhnAlgorithm(long cardNumber);
string cardChecker(long cardNumber);

int main(void)
{
    long cardNumber;
    int length;

    // Asks for card number
    cardNumber = get_long("Enter a card number: ");

    printf("%s", cardChecker(cardNumber));
}

// Checks if card is valid
string cardChecker(long cardNumber)
{
    if (!luhnAlgorithm(cardNumber))
    {
        return "INVALID\n";
    }

    // Gets card length and provider
    int cardLength = (int)floor(1 + log10(cardNumber));
    int provider = cardNumber / pow(10, cardLength - 2);

    // Returns right provider if card is valid
    if (provider == 34 || provider == 37)
    {
        if (cardLength == 15)
        {
            return "AMEX\n";
        }
    }
    else if (provider >= 51 && provider <= 55)
    {
        if (cardLength == 16)
        {
            return "MASTERCARD\n";
        }
    }
    else if (provider >= 40 && provider  <= 49)
    {
        if (cardLength == 13 || cardLength == 16)
        {
            return "VISA\n";
        }
    }

    return "INVALID\n";
}

// Own implementation of Luhn Algorithm
bool luhnAlgorithm(long cardNumber)
{
    // Cal
    int cardLength = (int)floor(1 + log10(cardNumber));

    int sum = 0;

    bool isSecond = false;
    int number;
    // Calculation of sum
    for (int i = 0; i < cardLength; i++)
    {
        number = cardNumber % 10;
        if (isSecond)
        {
            number *= 2;
            if (number < 10)
            {
                sum += number;
            }
            else
            {
                sum += number % 10;
                sum += number / 10;
            }
        }
        else
        {
            sum += number;
        }

        cardNumber /= 10;
        isSecond = !isSecond;
    }

    if (sum % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}