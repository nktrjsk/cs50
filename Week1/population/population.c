#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int lammas = 1000;
    do
    {
        lammas = get_int("Enter a number of lammas: ");
    }
    while (lammas < 9);

    // TODO: Prompt for end size
    int endLammas = 10000;
    do
    {
        endLammas = get_int("Enter a number of lammas at the end: ");
    }
    while (endLammas < lammas);

    // TODO: Calculate number of years until we reach threshold
    int years = 0;
    while (lammas < endLammas)
    {
        lammas += lammas / 3 - lammas / 4;
        years++;
        // printf("Lammas: %i, Born: %i, Dead: %i\n", lammas, lammas / 3, lammas / 4);
    }

    // TODO: Print number of years
    printf("Years: %i\n", years);
}
