/**********
 *
 * peter bke243
 * cs50- peset4-recover
 *recover JPEG files
 *
**********/
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<stdint.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage : ./recover image\n");
        return 1;
    }

    // remenber image name  (card)
    char *image = argv[1];
    printf("image %s", image);

    // open image (card)
    FILE *inptr = fopen(image, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "couldn't open %s \n", image);
        return 2;
    }

    //  remenber number JPEG files found
    int file_number = 0;

    // remember filename of number of JPEG files found
    char  filename[100];

    // remember number of bytes read
    int end = 512 ;

    // remenber if already found a  JPEG
    char *started = "No";

    // creating a pointer for filename and point it to NULL
    FILE *outptr = NULL ;

    while (end == 512)
    {
        // temprorary storage for our bytes
        uint8_t buffer[512];

        // read and stock 512 byte in buffer
        end = fread(buffer, 1, 512, inptr);

        // check if the buffer read is the beginning of a JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0 && end == 512)
        {
            // creating JPEG files
            // creating the first JPEG
            if (file_number == 0)
            {
                sprintf(filename, "%03i.jpg", file_number);
                file_number++ ;
                outptr  = fopen(filename, "w") ;
                if (outptr == NULL)
                {
                    fclose(inptr);
                    fprintf(stderr, "couldn't open %s \n", filename);
                    return 3;
                }
            }
            // create  JPEG  if the first is already found
            else
            {
                fclose(outptr);
                sprintf(filename, "%03i.jpg", file_number);
                file_number++ ;
                outptr  = fopen(filename, "w");
                if (outptr == NULL)
                {
                    fclose(inptr);
                    fprintf(stderr, "couldn't open %s \n", filename);
                    return 3;
                }
            }

            // write the first 512 JPEG bytes to the file newly opened
            fwrite(buffer, sizeof(buffer[1]), 512, outptr) ;
            started = "Yes";
        }
        // write to new file its JPEG
        else if (strcmp(started, "Yes") == 0 && end == 512)
        {
            fwrite(buffer, sizeof(buffer[1]), 512, outptr) ;
        }
    }

    // close the last JPEG file
    fclose(outptr);

    //close image
    fclose(inptr);

    // success @bke
    return 0 ;
}