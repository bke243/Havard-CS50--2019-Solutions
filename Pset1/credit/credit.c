
/**********
 *
 * peter bke243
 *
 * cs50- peset1- credit
 *
 * this program get the credit card number fro the user and verify if the input credit card number is valid or not
 *
**********/
#include<cs50.h>
#include<stdio.h>

int main(void)
{

    // variable to get input(credit card number) from the user
    long long int CredCardNum;

    // get  the credit card number from the user
    do
    {
        CredCardNum = get_long_long("enter the credit card number:");
    }
    while (CredCardNum < 0);

    // creating a copy of credCardNum  CpcredCardNum
    long long int CpCredCardNum = CredCardNum;

    // calculation of last and blast(using m for blast)  && last and blast(before the last ) to store the sum of digits according to the process given in the video
    int m, last = 0, blast = 0;

    while (CredCardNum > 0)
    {
        last += CredCardNum % 10;
        CredCardNum = CredCardNum / 10;
        m = (CredCardNum % 10) * 2;
        if (m >= 10)
        {
            m = m / 10 + m % 10;
        }
        blast += m;
        CredCardNum = CredCardNum / 10;
    }

    // calculation of the  final sum
    int sum = last + blast;

    // checking if the last digit of the sum is equal to zero
    if (sum % 10 == 0)
    {
        CredCardNum = CpCredCardNum;

        // calculation of the number of the  credit card digits
        int digits = 0;  //  to store number of digits
        while (CredCardNum > 0)
        {
            CredCardNum = CredCardNum / 10;
            digits++;
        }

        // identifying the company using variable a and b
        int a, b;
        // companies of 15 digits
        if (digits == 15)
        {
            a = CpCredCardNum / 10000000000000;
            if (a == 37 || a == 34)
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        // companies of 16 digits
        else if (digits == 16)
        {
            a = CpCredCardNum / 100000000000000;
            b = CpCredCardNum / 1000000000000000;
            if (a == 51 || a == 52 || a == 53 || a == 54 || a == 54 || a == 55)
            {
                printf("MASTERCARD\n");
            }
            else if (b == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        // compagnies of 13 digits
        else if (digits == 13)
        {
            a = CpCredCardNum / 1000000000000;
            if (a == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }

        }
        else
        {
            printf("INVALID\n");
        }


    }
    else
    {
        printf("INVALID\n");
    }

}