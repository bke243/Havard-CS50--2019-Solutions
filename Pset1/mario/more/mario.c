#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;    // height of our pyramid
    int hash = 1 ;  // number of hash

    // get the height from the user
    do
    {
        height = get_int("Height :");
    }
    while (height <= 0 || height > 8);

    while (height >= 1)
    {
        // print spaces
        int i, j, k, l;
        for ( i = 0; i < height-1; i++)
        {
            printf(" ");
        }

        // print hashes
        for ( j = 0; j < hash; j++)
        {
            printf("#");
        }
        printf("  ");  // add two spaces in the middle

        // draw the  second part of our pyramid
        for( k = 0; k < hash; k++)
        {
            printf("#");
        }
        printf("\n");
        hash++;
        height--;
    }
}
