#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Asking for height
    int height;
    do
    {
        height = get_int("How tall? ");
    }
    while (height < 1 || height > 8);

    // Building a pyramid
    for (int i = 1; i <= height; i++)
    {
        // Building a layer of pyramid
        for (int j = height; j >= -i; j--)
        {
            // Making a space in front
            if (j > i)
            {
                printf(" ");
            }
            // Inserting a double space in between a pyramid
            else if (j == 0)
            {
                printf("  ");
            }
            // Inserting a block
            else
            {
                printf("#");
            }
        }
        printf("\n");
    }
}