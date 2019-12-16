/**********
 *
 * peter bke243
 *
 * vigenere
 *
 * get the key(alpha) in (CLI) and the plaintext from  the user and print the ciphertext
 *
**********/
#include<cs50.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>

int keycheck(string a);
int shift(char a);

int main(int argc, string argv[])
{
    // Check if two CL arguments were entered
    if (argc == 2)
    {
        string key = argv[1];

        // Check if the key entered is good
        int verkey = keycheck(key);
        if (verkey == 1)
        {
            // Get palain text from the user and check if it is too big 
            string text = get_string("type your palintext here : ");
            if (!text)
            {
                return 1;
            }

            // print ciphertext phrase 
            printf("ciphertext: ");
            
            /* iteration for printing ciphertext
             * i  to iterate through the plain text 
             * j and k to keep truck of which character in the keyword to use 
             *  l to remenber the length of plaintext 
             * */
            int i, j, k, l;
            for (i = 0, j = 0, k = strlen(key), l = strlen(text); i < l; i++) 
            {
                // check if the character of the plaintext at index i is alpha
                if (isalpha(text[i]))
                {
                    // get the cypher key c_key 
                    int c_key = shift((key[j % k]));
                    // initialize the a variable to hold the the cipher char c_char
                    int c_char; 
                    if (isupper(text[i]))
                    {
                        // cipher for uppercase characters 
                        c_char = ((text[i] - 65) + c_key) % 26 ;
                        c_char = 65 + c_char;    
                    }
                    else
                    {
                        // cipher for lowercase characters 
                        c_char = ((text[i] - 97) + c_key) % 26 ;
                        c_char = 97 + c_char;
                    }
                    //print the cipher character
                    printf("%c", c_char);
                    j++;
                }
                else
                {
                    //print the  characters which are not letters 
                    printf("%c", text[i]);
                }
            }
            printf("\n");
        }
        else
        {
            printf("Usage: ./vigenere keyword\n");
            return 1;
        }
    }
    else
    {
        //tell to the users about the number of arguments should be computed in the CLI
        printf("Usage: ./vigenere keyword\n"); 
        return 1;
    }
    printf("\n");
}


// to check if the string contains only alpha characters
int keycheck(string a)
{
    int verkey = 1;
    for (int i = 0, n = strlen(a); i < n; i++)
    {
        if (! isalpha(a[i]))
        {
            verkey = 0;
        }
    }
    return verkey ;
}

// this function allow us to keep upper and lower cases unchanged
int shift(char a)
{
    // c local variable to return 
    int c;
    if (isupper(a))
    {
        c = a - 65;
    }
    else
    {
        c = a - 97;
    }
    return c;
}
