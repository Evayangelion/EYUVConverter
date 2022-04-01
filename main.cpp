#include <iostream>
#include "YUVConverter.h"
int main(int argc, char** argv) {

    FILE *infile;
    FILE *outfile;

    char *infile_name, *outfile_name;
    int width, height, inframesize, outframesize; 
    unsigned char *ptrbufinframe, *ptrbufoutframe;


    /*
    infile_name     = "stefan_cif.yuv";
    outfile_name     = "out_yu16_new_UV.yuv";
    */
    infile_name = "stefan_cif(I420-YUV420P).yuv";
    outfile_name = "420toRGB.txt";

    width              = 352;
    height             = 288;

    /*420->422
    inframesize = width * height * 3 / 2;
    outframesize = width * height * 2;
    */

    /*422->420
    inframesize = width * height *  2;
    outframesize = width * height * 3 / 2;
    */

    /*420->rgb888*/
    inframesize = width * height *  3 / 2;
    outframesize = width * height * 3;
    
    infile  = fopen(infile_name, "rb");
    outfile = fopen(outfile_name, "wb");
    if(infile == NULL) 
    {
        printf("\nCannot open file %s",infile_name); 
        printf("\n\n");
        fclose(infile);
        return 2;
    }
    if(outfile == NULL) 
    {
        printf("\nCannot open file %s",outfile_name); 
        printf("\n\n");
        fclose(infile);
        fclose(outfile);
        return 2;
    }

    ptrbufinframe = (unsigned char *) malloc(inframesize);
    ptrbufoutframe = (unsigned char *) malloc(outframesize);
    fread(ptrbufinframe, 1, inframesize, infile);

    while (!feof(infile))
    {
        //YUV422toYUV420(ptrbufinframe, ptrbufoutframe, width, height);
        YUV420toRGB(ptrbufinframe, ptrbufoutframe, width, height);
        fwrite(ptrbufoutframe, 1, outframesize, outfile);
        fread(ptrbufinframe, 1, inframesize, infile);
    }
  
    free(ptrbufinframe);
    free(ptrbufoutframe);
    fclose(infile);
    fclose(outfile);
    return 0;
}