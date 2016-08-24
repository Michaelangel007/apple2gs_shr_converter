/*
#define GRAVITY_CENTER    0
#define GRAVITY_EAST      1
#define GRAVITY_NORTHEAST 2
#define GRAVITY_NORTH     3
#define GRAVITY_NORTHWEST 4
#define GRAVITY_SOUTHEAST 5
#define GRAVITY_SOUTH     6
#define GRAVITY_SOUTHWEST 7
#define GRAVITY_WEST      8
*/
#define NUM_GRAVITIES     9
 
/* gravity options for cropped output - center (typical) */
char *geometry[] = {
"Center",
"East",
"NorthEast",
"North",
"NorthWest",
"SouthEast",
"South",
"SouthWest",
"West",
NULL};
 
char *gravity = "center";
 
/* command line helper function to set ImageMagick geometry for gravity option */
void SetMagickAspectScript(char *wordptr)
{
    int idx;
 
    if (wordptr[0] != (char) 0) {
        for (idx = 0;idx < NUM_GRAVITIES;idx++) {
            if (cmpstr(wordptr,geometry[idx]) == SUCCESS) {
                gravity = (char *) &geometry[idx][0];
                return;
            }
        }
    }
}
 
 
/* args are image file name, type of output 0 or 1 (.bat or .sh),
   and whether output is cropped 0 or padded 1 */
int MakeMagickAspectScript(char *name,int script, int pad)
{
    FILE *fp, *fp2=NULL;
    char buf[128], c, d, idfile[256], outfile[256];
    int i, j, status = 1;
    float fwidth, fheight;
    int width, height, cropwidth, cropheight, xtent;
 
    /* use image file base name for id file name and batch file or script file name */
    strcpy(idfile,name);
    strcpy(outfile,name);
    j = 999;
    for (i=0;name[i]!=(char)0;i++){
        if (name[i] == '.')j=i;
    }
 
    /* the image file name must have an extension */
    if (j==999) return status;
    idfile[j] = outfile[j]=(char)0;
 
    /* the id file must already exist */
    strcat(idfile,".id");
    fp = fopen(idfile,"r");
    if (fp == NULL) return status;
 
    if (script == 0) strcat(outfile,".bat");
    else strcat(outfile,".sh");
 
    for (;;) {
        if (NULL == fgets(buf, 128, fp)) {
            fclose(fp);
            break;
        }
        nocr(buf);
        /* this parser is really simplistic... it uses the first widthxheight string preceded by a space...
           it will probably usually work since we don't usually put widthxheight strings in filenames */
        width = height = 0;
        d = (char)0;
        for (i=0;buf[i]!=(char)0;i++){
            if (d == (char)32) {
                width = atoi((char *)&buf[i]);
            }
            c = buf[i];
            if (c=='x' && width > 0) {
                if (d > (char)47 && d < (char)58) {
                    height = atoi((char *)&buf[i+1]);
                    if (height > 0) break;
                }
            }
            d = c;
        }
        if (width == 0) break;
        if (height == 0) break;
        if (pad == 0) {
            /* cropping (not padding) */
            /* for widescreen images trim both sides, the height remains the same */
            fwidth = (float)1.3334 * height;
            cropwidth = (int)fwidth;
            if (cropwidth > width) {
                /* if the cropping width is not 1/3 greater than the height, this is a portrait image
                   and must be cropped in the vertical axis only */
                cropwidth = width;
                fheight = (float)0.75 * width;
                cropheight = (int)fheight;
            }
            else {
                cropheight = height;
            }
        }
        else {
            fheight = (float)0.75 * width;
            cropheight = (int)fheight;
            if (cropheight == height) {
               /* if the height is equal to 75% of the width, the output is verbatim (no padding is needed) */
               cropwidth = width;
               pad = 0;
            }
            else if (height < cropheight) {
               /* if the height is less than 75% of the width, the output is letterboxed with padding above and below */
               xtent = cropwidth = width;
            }
            else {
               /* if the height is greater than 75% of the width, the output is pillarboxed with padding on the sides */
               cropheight = height;
               fwidth = (float)1.3334 * height;
               xtent = cropwidth = (int)fwidth;
            }
        }
        fp2 = fopen(outfile,"w");
        if (NULL == fp2) break;
 
        if (script == 0) {
            /* batch file output */
            fprintf(fp2,"@echo off\n");
            if (pad == 0) {
                /* %MAGICK% %%f -gravity center -crop 1440x1080+0+0 -define format:BMP %%~nf.bmp */
                fprintf(fp2,"%%1 %%2 -gravity %s -crop %dx%d+0+0 -define format:BMP %%3.bmp\n",gravity,cropwidth,cropheight);
            }
            else {
               /* letterbox */
               /* %MAGICK% %%f -gravity center -background black -extent 1920x1920 -crop 1920x1440+0+0 +repage -define format:BMP %%~nf.bmp */
               /* pillarbox */
               /* %MAGICK% %%f -gravity center -background black -extent 2730x2730 -crop 2730x2048+0+0 +repage -define format:BMP %%~nf.bmp */
               fprintf(fp2,
               "%%1 %%2 -gravity %s -background black -extent %dx%d -crop %dx%d+0+0 +repage -define format:BMP %%3.bmp\n",
               gravity,xtent,xtent,cropwidth,cropheight);
 
            }
        }
        else {
            /* shell script output */
            fprintf(fp2,"#!/bin/sh\n");
            if (pad == 0) {
                /* $MAGICK $src -gravity center -crop 1440x1080+0+0 -define format:BMP $tgt.bmp */
                fprintf(fp2,"$1 $2 -gravity %s -crop %dx%d+0+0 -define format:BMP $3.bmp\n",gravity,cropwidth,cropheight);
            }
            else {
               fprintf(fp2,
               "$1 $2 -gravity %s -background black -extent %dx%d -crop %dx%d+0+0 +repage -define format:BMP $3.bmp\n",
               gravity,xtent,xtent,cropwidth,cropheight);
 
            }
        }
        status = 0;
    }
    fclose(fp);
    if (NULL != fp2) fclose(fp2);
 
    return status;
}
 
