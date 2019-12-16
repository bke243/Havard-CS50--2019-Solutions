/*
 * peter-bke
 * take an input plaintext from the user and print the ciphertext 
 * using the key passed through the command line while runing the program  
 * 
 */
#include<cs50.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

int main(int argc, string argv[]) 
{
    // cipher the plaintext
    if (argc == 2)
    {
        // check the key value of key using ascii codes 
        string checker = (argv[1]);
        for (int i = 0, j = strlen(checker); i < j; i++)
        {
            if (checker[i] < 48 || checker[i] > 57)
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }    
        }
        // conver the keyvalue to int
        int key = atoi(argv[1]);
        
 
       

        //get the plaintext
        string text = get_string("plaintext: ");
        if (!text)
        {
            return 1;
        }

        // n is the string lengh
        int n = strlen(text);

        printf("ciphertext: ");

        // text encryption
        for (int i = 0; i < n; i++)
        {
            if (isalpha(text[i]))
            {
                // keep the uppercase and cipher  all of them them
                if (isupper(text[i]))
                {
                    int c = ((text[i] - 65) + key) % 26;
                    c = 65 + c;
                    printf("%c", c);
                }

                // keep the lowercase and cipher all of them
                else
                {
                    int c = ((text[i] - 97) + key) % 26;
                    c = 97 + c;
                    printf("%c", c);
                }
            }
            else
            {
                printf("%c", text[i]);
            }
        }
        printf("\n");
        return 0;

    }

    // if number of CLI arguments is not correct
    else
    {
        /*tell to the users about the number of arguments 
         * should be computed in the CLI*/
        printf("Usage: ./caesar key\n"); 
        return 1;
    }
}
