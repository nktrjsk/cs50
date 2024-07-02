#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // asks for name of the user
    string name = get_string("What's your name? ");
    printf("hello, %s!\n", name);
}