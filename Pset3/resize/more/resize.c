/**********
 *
 * peter bke243
 * cs50-resize
 *resize  more confortable a BMP file
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
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }

    // convert the scale to float and check its value
    float scale = atof(argv[1]);
    if (scale < 0.0 || scale > 100.0)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
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
    //  value of biwidth and biheight for outfile
    int oldWidth   = bi.biWidth;
    int oldHeight  = bi.biHeight;
    double newWidth   = (bi.biWidth * scale);
    double newHeight  = (bi.biHeight * scale);

    // Compute value of biWidth  & biHeight for outfile and determine padding for  outfile and infile
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    biw.biWidth = round(bi.biWidth * scale);
    biw.biHeight = round(bi.biHeight * scale);
    int outpadding = (4 - (biw.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //  Compute the biSizeImage & bfSize for the outfile
    biw.biSizeImage = (sizeof(RGBTRIPLE) * (biw.biWidth) + outpadding) * abs(biw.biHeight);
    bfw.bfSize = biw.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfw, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&biw, sizeof(BITMAPINFOHEADER), 1, outptr);

    //  Array  of dimension 2 for the storage of our infile pixels
    RGBTRIPLE infilePixels[abs(bi.biHeight)][bi.biWidth];

    // storage of  all infile pixel into our infilepixel array
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight ; i++)
    {
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple ;

            /*Read infile pixels and store them into infilePixels array  very good
            because we read onlyonce in the file read once in  infile  all RGBTRIPLE*/
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            infilePixels[i][j] = triple ;
        }
        // skip over infile's padding if any
        fseek(inptr, padding, SEEK_CUR);
    }

    /*  Ratio   of image Height between  infile and outfile ;
    This ratio is known as scale factor
    (scale factor is The ratio of the length in a drawing (or model) to the length on the real thing)*/
    double HeightRatio =  oldHeight   / newHeight;
    double WidthRatio =   oldWidth   / newWidth;

    // write outfile pixels
    for (int i = 0, biHeight = abs(biw.biHeight); i < biHeight; i++)
    {
        int pixel_inline_towrite =  i *  HeightRatio ;
        // iterate over RGB triple in a scanline
        for (int j = 0; j < biw.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple ;

            int pixel_incoloumn_towrite =  j *  WidthRatio;
            triple = infilePixels[pixel_inline_towrite][pixel_incoloumn_towrite];

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // then add outfile padding if any
        for (int k = 0; k < outpadding; k++)
        {
            fputc(0x00, outptr);
        }
    }
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success @bke
    return 0;
}