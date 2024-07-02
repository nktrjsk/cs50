#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");

    printf("%s\n", text);

    printf("%i letters\n", count_letters(text));
    printf("%i words\n", count_words(text));
    printf("%i sentences\n", count_sentences(text));

    float L = count_letters(text) / (count_words(text) / 100.0);
    float S = count_sentences(text) / (count_words(text) / 100.0);

    int index = round(0.0588 * L - 0.296 * S - 15.8);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

int count_letters(string text)
{
    int count = 0;

    for (int i = 0, c = strlen(text); i < c; i++)
    {
        int letter = toupper(text[i]);

        if (letter >= 65 && letter <= 90)
        {
            count++;
        }
    }

    return count;
}

int count_words(string text)
{
    int count = 1;

    for (int i = 0, c = strlen(text); i < c; i++)
    {
        if (text[i] == 32)
        {
            count++;
        }
    }

    return count;
}

int count_sentences(string text)
{
    int count = 0;

    for (int i = 0, c = strlen(text); i < c; i++)
    {
        int letter = text[i];

        if (letter == 46 || letter == 63 || letter == 33)
        {
            count++;
        }
    }

    return count;
}