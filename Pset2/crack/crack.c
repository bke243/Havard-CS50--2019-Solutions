/**********
 *
 * peter bke243
 *
 * cs50-crack
 *
 * get the crypt password as CLI  and print the user password
 *
**********/
#include<stdio.h>
#define _XOPEN_SOURCE
#include<cs50.h>
#include<string.h>
#include<unistd.h>
#include<crypt.h>
#define x 46

int main(int argc, string argv[])
{
    if (argc == 2)
    {
        char array[6];

        //  Get from the user the hash password 
        string code = argv[1];

        // extracting the salt
        char salt[] = {code[0], code[1]};

        // string containing letters to iterate through
        string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

        // Case 1 one letter password
        for (int i = 0; i < x; i++)  //char 1
        {
            array[0] = letters[i];
            array[1] = '\0';
            string newcode = crypt(array, salt);
            if (strcmp(newcode, code) == 0)
            {
                printf("%s\n", array);
                return 0;
            }

        }
        // Case 2   two letters password
        for (int i = 0; i < x; i++)  //char 1
        {
            array[0] = letters[i];
            for (int j = 0; j < x; j++)  //char 2
            {
                array[1] = letters[j];
                array[2] = '\0';
                string newcode = crypt(array, salt);
                if (strcmp(newcode, code) == 0)
                {
                    printf("%s\n", array);
                    return 0;
                }
            }
        }
        // Case 3 three letters password
        for (int i = 0; i < x ; i++)  //char 1
        {
            array[0] = letters[i];
            for (int j = 0; j < x; j++)  //char 2
            {
                array[1] = letters[j];
                for (int k = 0; k < x; k++)  //char 3
                {
                    array[2] = letters[k];
                    array[3] = '\0';
                    string newcode = crypt(array, salt);
                    if (strcmp(newcode, code) == 0)
                    {
                        printf("%s\n", array);
                        return 0;
                    }
                }

            }
        }
        // Case 4 four letters  password
        for (int i = 0; i < x; i++)
        {
            array[0] = letters[i];
            for (int j = 0; j < x; j++) //char 2
            {
                array[1] = letters[j];
                for (int k = 0; k < x; k++) //char 3
                {
                    array[2] = letters[k];
                    for (int l = 0; l < x; l++) /* char 3 */
                    {
                        array[3] = letters[l];  /* char 4*/
                        array[4] = '\0';
                        string newcode = crypt(array, salt);
                        if (strcmp(newcode, code) == 0)
                        {
                            printf("%s\n", array);
                            return 0;
                        }
                    }
                }
            }
        }
        //Case 5 five letters  password
        for (int i = 0; i < x; i++)
        {
            array[0] = letters[i];
            for (int j = 0; j < x; j++) //char 2
            {
                array[1] = letters[j];
                for (int k = 0; k < x; k++) //char 3
                {
                    array[2] = letters[k];
                    for (int l = 0; l < x; l++) /* char 3 */
                    {
                        array[3] = letters[l];  /* char 4*/
                        for (int m = 0; m < x; m++)
                        {
                            array[4] = letters[m]; /* char 5*/
                            array[5] = '\0';
                            string newcode = crypt(array, salt);
                            if (strcmp(newcode, code) == 0)
                            {
                                printf("%s\n", array);
                                return 0;
                            }
                        }
                    }
                }
            }
        }
        printf("size of password greater than four \n");
        return 0;

    }

    //print the usage of the program 
    else
    {
        printf("USage: ./crack hash\n");
        return 1;
    }
}
