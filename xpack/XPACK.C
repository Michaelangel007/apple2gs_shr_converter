
/* ------------------------------------------------------------------------ */
/* XPACK.C (C) Copyright Bill Buckels 2013                                */
/* All Rights Reserved.                                                     */
/*                                                                          */
/* Licence Agreement                                                        */
/* -----------------                                                        */
/*                                                                          */
/* You have a royalty-free right to use, modify, reproduce and              */
/* distribute this source code in any way you find useful,                  */
/* provided that you agree that Bill Buckels has no warranty obligations    */
/* or liability resulting from said distribution in any way whatsoever.     */
/* If you don't agree, remove this source code from your computer now.      */
/*                                                                          */
/* Written by   : Bill Buckels                                              */
/* Email:         bbuckels@escape.ca                                        */
/*                                                                          */
/* Purpose      : This utility will allow you to convert from 				*/
/*                Apple II Double Hi-Res 140 x 192 x 16 color images to     */
/*				  packed equivalent Apple II Double Hi-Res images.          */
/*                                                                          */
/*                A single purpose packing utility for A2FC and             */
/*                BIN/AUX DHGR Images for the Apple II.                     */
/*
					Output:

                      DHX (Double Hi-Res Xpacked) images.
                      DHR (Double Hi-Res Raster) images.

	The DHX is a raster based image with bytes alternating between
	auxiliary and main memory of Apple II DHGR data encoded as a
	chunk using ZSoft PCX run length encoding. The DHR is the non-ecoded
	equivalent but with 40 bytes of auxiliary memory data followed by
	40 bytes of main memory data.

	The byte arrangement of the DHX works best for repeats of black or
	white pixel data. Other colors are quite another matter, but the DHX
    displays reasonably quickly when compared to decoding pixel level
    indices like the files produced by XCrunch (another of my utilities).

    The DHR like the DHX loads raster by raster and displays as quickly as
    a buffered read can display on the Apple II. At 15360 bytes it provides
    a modest disk space saving over the 16384 bytes of a BSaved equivalent.

    Either format provides potential for image fragments because
    like my RAG and RAX image formats for standard Hi-Res they are
    native Apple II image data and raster oriented.

    A caveat for either DHGR raster format however is the 4 byte / 7 pixel
    pattern of the DHGR display. The descriptor in the header is given
    in byte width rather than pixel width. Image fragments in DHGR must
    necessarily be aligned on 4 byte boundaries to display properly.

    By comparison, HGR image fragments are aligned on 2 byte boundaries for
    proper display but they are still somewhat recognizable if not aligned
    properly.

																			*/
/* Revision     : 1.0 First Release                                         */
/* ------------------------------------------------------------------------ */
/* Written in Large Model 16 bit Microsoft C (MSC) Version 8.00c            */
/* Note: Run in an MS-DOS emulator like DOSBox if you can't run it raw.     */
/* ------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------------ */
/* Declarations, Vars. etc.                                                 */
/* ------------------------------------------------------------------------ */
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef short sshort;

#define ASCIIZ 0

char *szTextTitle =
    "XPACK(C) Version 2.0 Copyright Bill Buckels 2013,2015\n"
    "All Rights Reserved.";

#define SUCCESS  0
#define VALID    SUCCESS
#define FAILURE  -1
#define INVALID  FAILURE


int auxbin = 0, a2fc = 0, longnames = 1, interlace = 0;

/* Apple 2 Double Hires Format */

/* provides base address for page1 hires scanlines  */
unsigned HB[]={
0x2000, 0x2400, 0x2800, 0x2C00, 0x3000, 0x3400, 0x3800, 0x3C00,
0x2080, 0x2480, 0x2880, 0x2C80, 0x3080, 0x3480, 0x3880, 0x3C80,
0x2100, 0x2500, 0x2900, 0x2D00, 0x3100, 0x3500, 0x3900, 0x3D00,
0x2180, 0x2580, 0x2980, 0x2D80, 0x3180, 0x3580, 0x3980, 0x3D80,
0x2200, 0x2600, 0x2A00, 0x2E00, 0x3200, 0x3600, 0x3A00, 0x3E00,
0x2280, 0x2680, 0x2A80, 0x2E80, 0x3280, 0x3680, 0x3A80, 0x3E80,
0x2300, 0x2700, 0x2B00, 0x2F00, 0x3300, 0x3700, 0x3B00, 0x3F00,
0x2380, 0x2780, 0x2B80, 0x2F80, 0x3380, 0x3780, 0x3B80, 0x3F80,
0x2028, 0x2428, 0x2828, 0x2C28, 0x3028, 0x3428, 0x3828, 0x3C28,
0x20A8, 0x24A8, 0x28A8, 0x2CA8, 0x30A8, 0x34A8, 0x38A8, 0x3CA8,
0x2128, 0x2528, 0x2928, 0x2D28, 0x3128, 0x3528, 0x3928, 0x3D28,
0x21A8, 0x25A8, 0x29A8, 0x2DA8, 0x31A8, 0x35A8, 0x39A8, 0x3DA8,
0x2228, 0x2628, 0x2A28, 0x2E28, 0x3228, 0x3628, 0x3A28, 0x3E28,
0x22A8, 0x26A8, 0x2AA8, 0x2EA8, 0x32A8, 0x36A8, 0x3AA8, 0x3EA8,
0x2328, 0x2728, 0x2B28, 0x2F28, 0x3328, 0x3728, 0x3B28, 0x3F28,
0x23A8, 0x27A8, 0x2BA8, 0x2FA8, 0x33A8, 0x37A8, 0x3BA8, 0x3FA8,
0x2050, 0x2450, 0x2850, 0x2C50, 0x3050, 0x3450, 0x3850, 0x3C50,
0x20D0, 0x24D0, 0x28D0, 0x2CD0, 0x30D0, 0x34D0, 0x38D0, 0x3CD0,
0x2150, 0x2550, 0x2950, 0x2D50, 0x3150, 0x3550, 0x3950, 0x3D50,
0x21D0, 0x25D0, 0x29D0, 0x2DD0, 0x31D0, 0x35D0, 0x39D0, 0x3DD0,
0x2250, 0x2650, 0x2A50, 0x2E50, 0x3250, 0x3650, 0x3A50, 0x3E50,
0x22D0, 0x26D0, 0x2AD0, 0x2ED0, 0x32D0, 0x36D0, 0x3AD0, 0x3ED0,
0x2350, 0x2750, 0x2B50, 0x2F50, 0x3350, 0x3750, 0x3B50, 0x3F50,
0x23D0, 0x27D0, 0x2BD0, 0x2FD0, 0x33D0, 0x37D0, 0x3BD0, 0x3FD0};


uchar dhrbuf[16384], interlacebuf[16384];

/* read 2 input files */
int read_binaux(uchar *basename)
{

	FILE *fp;
	uchar infile[256];

    /* the bsaved images are split into two files
       the first file is loaded into aux mem */
    sprintf(infile,"%s.AUX",basename);
	fp = fopen(infile,"rb");
	if (NULL == fp)return INVALID;
	fread(dhrbuf,1,8192,fp);
	fclose(fp);

    /* the second file is loaded into main mem */
    sprintf(infile,"%s.BIN",basename);
	fp = fopen(infile,"rb");
	if (NULL == fp)return INVALID;
	fread(&dhrbuf[8192],1,8192,fp);
	fclose(fp);

	return SUCCESS;

}

/* read one input file */
int read_2fc(char *infile)
{
	FILE *fp;

	fp = fopen(infile,"rb");
	if (NULL == fp){
		return INVALID;
	}
	fread(dhrbuf,1,16384,fp);
	fclose(fp);

	return SUCCESS;

}


int interlace_2fc(char *infile)
{
	FILE *fp;

    interlace = 0;
	fp = fopen(infile,"rb");
	if (NULL == fp){
		return INVALID;
	}
	fread(interlacebuf,1,16384,fp);
	fclose(fp);
    interlace = 1;
	return SUCCESS;

}



/* pcx encoder helper */
/* the writer for the encline function */
int encput(uchar byt,uchar cnt, FILE *pcxfile)
{
          if(cnt){
            if((cnt==1)&& (0xc0 != (0xc0 &byt))){
                if(EOF == fputc((int)byt,pcxfile))
                return(0);
                return(1);
            }
            else{
                if(EOF==fputc((int)0xc0|cnt,pcxfile))
                return(0);
                if(EOF==fputc((int)byt,pcxfile))
                return(0);
                return(2);
            }
        }
        return(0);

}

/* encode a line in pcxformat encoding */
/* encodes a raw line and writes it out to disk */
int encline(uchar *inbuff,int inlen, FILE *pcxfile)
{
    uchar this,last;
    int srcindex,i;
    int total;
    uchar runcount;
    total=0;
    last = *(inbuff); runcount=1;

for(srcindex=1;srcindex!=inlen;srcindex++){
    this= *(++inbuff);
    if(this==last){
        runcount++;
        if(runcount==63){
            if(!(i=encput(last,runcount,pcxfile)))
            return(0);
            total+=i;
            runcount=0;
        }
    }
    else{
        if(runcount){
            if(!(i=encput(last,runcount,pcxfile)))
            return(0);
            total+=i;
        }
        last=this;
        runcount=1;
      }
   }

   if(runcount){
    if(!(i=encput(last,runcount,pcxfile)))
    return(0);
    return(total+i);
    }
    return (total);

}

uchar bigbuf[15360];

int dhrmakedhx(FILE *fp2)
{
    int x, y, cnt, xoff;
    uchar *ptraux, *ptrmain, a, m;
    uchar auxbuf[40], mainbuf[40];

    cnt = 0;
    for (y = 0; y < 192; y++) {
	    xoff = HB[y];
	    if (interlace == 1 && y%2 == 0) {
			ptraux  = (uchar *) &interlacebuf[xoff-0x2000];
			ptrmain = (uchar *) &interlacebuf[xoff];
		}
		else {
    		ptraux  = (uchar *) &dhrbuf[xoff-0x2000];
    		ptrmain = (uchar *) &dhrbuf[xoff];
		}

    	for (x = 0; x < 40; x++) {
			auxbuf[x] =  a = ptraux[x];
			mainbuf[x] = m = ptrmain[x];
		    bigbuf[cnt] = a; cnt++;
		    bigbuf[cnt] = m; cnt++;
		}
		fwrite(auxbuf,1,40,fp2);
		fwrite(mainbuf,1,40,fp2);

	}

    return 0;

}



int save_to_X(uchar *basename)
{

    FILE *fp,*fp2;
    uchar dhxfile[256], dhrfile[256];

    sprintf(dhxfile,"%s.DHX",basename);
    sprintf(dhrfile,"%s.DHR",basename);

 	fp = fopen(dhxfile,"wb");
	if (NULL == fp)return INVALID;
	fp2 = fopen(dhrfile,"wb");
	if (NULL == fp2) {
		fclose(fp);
		return INVALID;
	}

    /* write a 5 byte header of sorts */

    /* some kind of identifier */
    fputc('D',fp);	   fputc('D',fp2);
    fputc('H',fp);	   fputc('H',fp2);
    fputc('X',fp);	   fputc('R',fp2);

    /* width in bytes x height in rasters */
    fputc(80,fp);	 fputc(80,fp2);
    fputc(192,fp);	 fputc(192,fp2);

	dhrmakedhx(fp2);
	fclose(fp2);
    encline(bigbuf,15360,fp);
    fclose(fp);

    return SUCCESS;

}


int main(int argc, char **argv)
{

  int status = 1, idx, jdx;
  char fname[256],sname[256],outfile[256], interfile[256], c, d, e, f;

  fname[0] = sname[0] = outfile[0] = ASCIIZ;

#ifdef MSDOS
  longnames = 0;
  system("cls");
#endif

  if(argc == 1) {
    puts(szTextTitle);
    puts("Command line Usage is \"XPACK MyHires.2FC\"");
    puts("                      \"XPACK MyHires.BIN\"");
    puts("                      \"XPACK MyHires.AUX\"");
    puts("                      \"XPACK MyHires.2FC OutfileBaseName\"");
    puts("                      \"XPACK MyHires.BIN OutfileBaseName\"");
    puts("                      \"XPACK MyHires.AUX OutfileBaseName\"");
    puts("If converting .BIN and .AUX file pairs, both must be present.");
    printf("Enter Input FileName (Blank to Exit): ");
    gets(fname);
    if (fname[0] == ASCIIZ) return 1;

    printf("Enter Output FileBaseName (Blank for Same) : ");
    gets(outfile);
  }
  else {
    strcpy(fname, argv[1]);
    if (argc > 2) {
      strcpy(outfile, argv[2]);
      if (argc > 3) {
	  	 strcpy(interfile, argv[3]);
	  	 interlace = 1;
	  }
  	}
    else
      outfile[0] = ASCIIZ;
  }

  jdx = 999;
  for (idx = 0; fname[idx] != ASCIIZ; idx++) {
      if (fname[idx] == '.') jdx = idx;
  }

  if (jdx != 999)
  {
	  strcpy(sname, fname);
	  sname[jdx] = ASCIIZ;
	  if (outfile[0] == ASCIIZ)strcpy(outfile,sname);

	  c = toupper(fname[jdx + 1]);
	  d = toupper(fname[jdx + 2]);
	  e = toupper(fname[jdx + 3]);

      if (longnames != 0) {
	    /* full-screen formats */
	  	f = toupper(fname[jdx + 4]);
      	if (c == 'A' && d == '2' && e == 'F' && f == 'C') a2fc = 1;
      	if (c == 'A' && d == '2' && e == 'F' && f == 'M') a2fc = 1;
	  	/* support for Sheldon Simms tohgr */
	  	if (c == 'D' && d == 'H' && e == 'G' && f == 'R') a2fc = 1;
	  }

      /* full-screen formats */
	  if (c == '2' && d == 'F' && e == 'C') a2fc = 1;
	  if (c == 'B' && d == 'I' && e == 'N') auxbin = 1;
	  if (c == 'A' && d == 'U' && e == 'X') auxbin = 1;

   }

  if (auxbin == 1 || a2fc == 1) {


     if (a2fc == 1) status = read_2fc(fname);
     else status = read_binaux(sname);

     if (status == SUCCESS) {

		 if (interlace == 1)interlace_2fc(interfile);

		 status = save_to_X(outfile);
		 if (status == SUCCESS) {
			 printf("%s.DHX and %s.DHR Saved!\n", outfile,outfile);
		 }
		 else {
			printf("Error saving %s.DHX and %s.DHR!\n",outfile,outfile);
			status = 1;
		 }
	 }
	 else {
		 printf("%s cannot be opened.\n", fname);
		 status = 1;

	 }

  }
  else {
	  printf("%s is an Unsupported Format.\n", fname);
  }


#ifdef MSDOS
 puts("Have a Nice Dos!");
#endif

	return status;

}
