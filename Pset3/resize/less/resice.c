/**********
 *
 * peter bke243
 * cs50- peset3-resize-less
 *resize a BMP file
 *
**********/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // convert the scale to interger
    int scale = atoi(argv[1]);
    if (scale <= 0 || scale > 100)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf, bfw;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    bfw = bf;

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, biw;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    biw = bi ;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    // Compute value of biWidth  & biHeight for outfile and determine padding for  outfile and infile
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    biw.biWidth = bi.biWidth * scale;
    biw.biHeight = bi.biHeight * scale ;
    int outpadding = (4 - (biw.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //  Compute the biSizeImage & bfSize for the outfile
    biw.biSizeImage = (sizeof(RGBTRIPLE) * (biw.biWidth) + outpadding) * abs(biw.biHeight);
    bfw.bfSize = biw.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfw, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&biw, sizeof(BITMAPINFOHEADER), 1, outptr);

    // write RGBTRIPLE resized to outfile
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // variable n which takes the value of our scale
        int n = scale ;

        while (n > 0)
        {
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                for (int k = 0; k < scale; k++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            //  add  outfile padding
            for (int k = 0; k < outpadding; k++)
            {
                fputc(0x00, outptr);
            }
            // send the cusrsor in  infile back
            fseek(inptr, - sizeof(RGBTRIPLE)*bi.biWidth, SEEK_CUR);
            n--;
        }
        // send the cursor in  infile forward
        fseek(inptr, sizeof(RGBTRIPLE)*bi.biWidth, SEEK_CUR);

        // skip over padding, if any in  infile
        fseek(inptr, padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success @bke
    return 0;
}