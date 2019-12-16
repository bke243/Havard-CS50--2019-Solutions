/**********
 *
 * peter bke243
 *
 * cs50- peset1- greedy
 *
 * get the amount of change in $ from the user and specifies the minimum number of coins that can be returned from that amount
 *
**********/

#include<cs50.h>
#include<stdio.h>
#include<math.h>

int main(void)
{
    float  amount;

    // itialization of default values (quater, dime, nickel, penny)
    int quater = 0, dime = 0, nickel = 0, penny = 0;
    // get the amount in $ from the user
    do
    {
        amount = get_float("chane owed :");
    }
    while (amount <= 0);

    // converting the amount in its equivalent  in ¢
    amount = round (amount * 100);
    printf("amount change %f \n", amount);

    // getting the number of coins using independant if statement
    if (amount >= 25)
    {
        quater =  amount / 25;
        amount -= quater * 25;
    }
    if (amount >= 10)
    {
        dime = amount / 10;
        amount -= dime * 10;
    }
    if (amount >= 5)
    {
        nickel = amount / 5;
        amount -= nickel * 5;
    }
    if (amount > 0  && amount <5)
    {
        penny = amount;
    }
    // print the number  of coins used
    printf("%i\n", quater + dime + nickel + penny);
}