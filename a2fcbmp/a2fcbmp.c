/* ------------------------------------------------------------------------ */
/* A2FCBMP.C (C) Copyright Bill Buckels 2012-2015                           */
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
/* Email:         bbuckels@mts.net                                          */
/*                                                                          */
/* Purpose      : This utility will allow you to convert from               */
/*                Apple II Double Hi-Res 140 x 192 x 16 color images to:    */
/*                                                                          */
/*      280 x 192 x 24 Bit Windows .BMP Files - Default                     */
/*      140 x 192 x 24 Bit Windows .BMP Files - Option "140"                */
/*                                                                          */
/*      Variable sized color BMP output from DHR Image Fragments in both    */
/*      nominal sizes listed above is also supported.                       */
/*                                                                          */
/*      Option "P" (p0-p16) Color Mapping is in one of 16 optional palettes.*/
/*                The palette numbers are the same as Bmp2DHR:              */
/*                                                                          */
/*                          0 - Kegs32 (RGB)                                */
/*                          1 - CiderPress (RGB)                            */
/*                          2 - Old AppleWin (NTSC ?)                       */
/*                          3 - New AppleWin (NTSC ?)                       */
/*                          4 - Wikipedia (NTSC)                            */
/*                          5 - tohgr (NTSC)                                */
/*                          palette 6 is a user palette file                */
/*                          palettes 7-11 are IBM-PC legacy palettes        */
/*                          palette 12 is Super Convert RGB palette         */
/*                          palette 13 is Jace NTSC palette                 */
/*                          palette 14 is Cybernesto's VBMP NTSC palette    */
/*                          palette 15 is merged RGB-NTSC colors (p5+p12)/2 */
/*                          palette 16 is tohgr's old NTSC colors           */
/*                                                                          */
/*                This utility will also allow you to convert from          */
/*                Apple II Double Hi-Res 560 x 192 monochrome images to:    */
/*                                                                          */
/*      560 x 384 monochrome Windows .BMP Files - Option "384"              */
/*      560 x 192 monochrome Windows .BMP Files - Option "192"              */
/*                                                                          */
/*                This utility also converts from an AppleWin "Mono"        */
/*                screen capture 256 color BMP to any of the Above.         */
/*                                                                          */
/*                During conversion of the AppleWin 256 color BMP it also   */
/*                generates Apple II Double Hi-Res images and renames the   */
/*                AppleWin BMP.                                             */
/*                                                                          */
/* Revision     : 3.0 Added AppleWin Support and odds and ends.             */
/*                4.0 Added Different Palettes                              */
/*                    Added Long FileNames for 32 bit compilation           */
/*                    Added optional Ciderpress tags for Long Filenames     */
/*                    Added support for XPACK DHR image fragments           */
/*                    Added support for Sheldon Simms DHGR naming for A2FC  */
/*                5.0 Expanded Features and output beyond DHGR              */
/*                6.0 May 2015                                              */
/*                7.0 July 2015                                             */
/*                8.0 November 2015                                         */
/* ------------------------------------------------------------------------ */
/* Originally Written in Large Model 16 bit Microsoft C (MSC) Version 8.00c */
/*                    Cleaned-up c++ code comments for old Turbo C 2.0      */
/*                    16-bit MS-DOS version now compiles under both         */
/*                    MSC and Turbo C                                       */
/* ------------------------------------------------------------------------ */
/*                    Added Conditional Compilation for MinGW and gcc       */
/*                    Added Conditional Compilation etc. for 32 bit support */
/*                    32-bit version now compiles under Visual Studio Win32 */
/*                    and MinGW Win32 and gcc with support for long names.  */
/*                    Should now run everywhere (Windows, Linux, OSX)       */
/* ------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>

#define LOBLACK     0
#define LORED       1
#define LODKBLUE    2
#define LOPURPLE    3
#define LODKGREEN   4
#define LOGRAY      5
#define LOMEDBLUE   6
#define LOLTBLUE    7
#define LOBROWN     8
#define LOORANGE    9
#define LOGREY      10
#define LOPINK      11
#define LOLTGREEN   12
#define LOYELLOW    13
#define LOAQUA      14
#define LOWHITE     15

/* dither types */
#define FLOYDSTEINBERG 1
#define JARVIS 2
#define STUCKI 3
#define ATKINSON 4
#define BURKES 5
#define SIERRA 6
#define SIERRATWO 7
#define SIERRALITE 8
#define BUCKELS 9
#define ATKINSON2 10

/* ------------------------------------------------------------------------ */
/* Declarations, Vars. etc.                                                 */
/* ------------------------------------------------------------------------ */

/* options flags */

int longnames = 1, bmp = 0, bm2 = 0, bmp3 = 0, a2fc = 0, auxbin = 0, tohgr = 0, dhr = 0, frag = 0,
    mono = 0, doublepixel = 1, applesoft = 0, palnumber = 5, doublegrey = 0, vbmp = 0,
    dither = 0, dithertype = 9, randomdither = 0, errorsum = 0, outline = 0, lores = 0, doublelores = 0, tags=0, dosheader=0;
int shr = 0, shrgrey = 0, usegscolors = 0, usegspalette = 0, hsl = 1, shrpalette = 15, brooks = 0, shrmode = 0, shrpalettes = 1,
    shr256 = 0, useimagetone = 0, usepalettedistance = 0, quietmode = 1, m2s = 0, shrinput = 0, mix256 = 0,
    imnumpalettes = 0, fourbit = 0, fourplay = 0, fourpal = 0;

double desaturate[16];

char fullname[256], shortname[256], outname[256], mainfile[256],auxfile[256],a2fcfile[256];

char *palname[] = {
    "Kegs32 RGB",
    "CiderPress RGB",
    "Old AppleWin NTSC",
    "New AppleWin NTSC",
    "Wikipedia NTSC",
    "tohgr NTSC DHGR",
    "Imported",
    "Legacy Canvas",
    "Legacy Win16",
    "Legacy Win32",
    "Legacy VGA BIOS",
    "Legacy VGA PCX",
    "Super Convert RGB",
    "Jace NTSC",
    "Cybernesto-Munafo NTSC",
    "Pseudo Palette",
    "tohgr NTSC HGR"};

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef short sshort;

/* put this here for now */
/* shr colors start */
uchar ColorsUsed[16][16][16];
int shrcolorcount = 0;

/* for checking duplicate 12-bit colors in 24-bit conversion palettes */
/* expand as necessary */
int shrdupedebug = 0, shrdupes = 0, shrdupecount = 0;

void shrcolorsused(uchar r, uchar g, uchar b)
{
     r = r >> 4;
     g = g >> 4;
     b = b >> 4;
     if (ColorsUsed[r][g][b] == 0) {
        ColorsUsed[r][g][b] = 1;
        shrcolorcount++;
     }
     else if (shrdupedebug != 0 && r !=0 && g != 0 && b != 0) {
		/* do not count black as dupes */
		if (ColorsUsed[r][g][b] == 1) {
			ColorsUsed[r][g][b] = 2;
			shrdupes = 1;
		}
		shrdupecount++;
	}
}

void clearcolorsused()
{
    shrcolorcount = shrdupecount = shrdupes = 0;
    memset(&ColorsUsed[0][0][0],0,4096);
}
/* shr colors end */

/* Bitmap Header structures */
#ifdef MINGW
typedef struct __attribute__((__packed__)) tagRGBQUAD
#else
typedef struct tagRGBQUAD
#endif
{
    unsigned char    rgbBlue;
    unsigned char    rgbGreen;
    unsigned char    rgbRed;
    unsigned char    rgbReserved;
} RGBQUAD;

/* Bitmap Header structures */
#ifdef MINGW
typedef struct __attribute__((__packed__)) tagBITMAPINFOHEADER
#else
typedef struct tagBITMAPINFOHEADER
#endif
{
    ulong   biSize;
    ulong   biWidth;
    ulong   biHeight;
    ushort  biPlanes;
    ushort  biBitCount;
    ulong   biCompression;
    ulong   biSizeImage;
    ulong   biXPelsPerMeter;
    ulong   biYPelsPerMeter;
    ulong   biClrUsed;
    ulong   biClrImportant;
} BITMAPINFOHEADER;


/* Not officially documented - BITMAPV2INFOHEADER */
#ifdef MINGW
typedef struct __attribute__((__packed__)) tagBITMAPINFOHEADERV2
#else
typedef struct tagBITMAPINFOHEADERV2
#endif
{
    ulong   biSize;
    ulong   biWidth;
    ulong   biHeight;
    ushort  biPlanes;
    ushort  biBitCount;
    ulong   biCompression;
    ulong   biSizeImage;
    ulong   biXPelsPerMeter;
    ulong   biYPelsPerMeter;
    ulong   biClrUsed;
    ulong   biClrImportant;
  ulong        biRedMask;
  ulong        biGreenMask;
  ulong        biBlueMask;
} BITMAPINFOHEADERV2;

/* Not officially documented - BITMAPV3INFOHEADER */
#ifdef MINGW
typedef struct __attribute__((__packed__)) tagBITMAPINFOHEADERV3
#else
typedef struct tagBITMAPINFOHEADERV3
#endif
{
    ulong   biSize;
    ulong   biWidth;
    ulong   biHeight;
    ushort  biPlanes;
    ushort  biBitCount;
    ulong   biCompression;
    ulong   biSizeImage;
    ulong   biXPelsPerMeter;
    ulong   biYPelsPerMeter;
    ulong   biClrUsed;
    ulong   biClrImportant;
  ulong        biRedMask;
  ulong        biGreenMask;
  ulong        biBlueMask;
  ulong        biAlphaMask;

} BITMAPINFOHEADERV3;

#ifdef MINGW
typedef struct __attribute__((__packed__)) tagBITMAPINFOHEADERV4
#else
typedef struct tagBITMAPINFOHEADERV4
#endif
{
    ulong   biSize;
    ulong   biWidth;
    ulong   biHeight;
    ushort  biPlanes;
    ushort  biBitCount;
    ulong   biCompression;
    ulong   biSizeImage;
    ulong   biXPelsPerMeter;
    ulong   biYPelsPerMeter;
    ulong   biClrUsed;
    ulong   biClrImportant;
  ulong        biRedMask;
  ulong        biGreenMask;
  ulong        biBlueMask;
  ulong        biAlphaMask;
  ulong        biCSType;
  uchar        biEndpoints[36]; /* CIEXYZTRIPLE */
  ulong        biGammaRed;
  ulong        biGammaGreen;
  ulong        biGammaBlue;
} BITMAPINFOHEADERV4;

#ifdef MINGW
typedef struct __attribute__((__packed__)) tagBITMAPINFOHEADERV5
#else
typedef struct tagBITMAPINFOHEADERV5
#endif
{
    ulong   biSize;
    ulong   biWidth;
    ulong   biHeight;
    ushort  biPlanes;
    ushort  biBitCount;
    ulong   biCompression;
    ulong   biSizeImage;
    ulong   biXPelsPerMeter;
    ulong   biYPelsPerMeter;
    ulong   biClrUsed;
    ulong   biClrImportant;
  ulong        biRedMask;
  ulong        biGreenMask;
  ulong        biBlueMask;
  ulong        biAlphaMask;
  ulong        biCSType;
  uchar        biEndpoints[36]; /* CIEXYZTRIPLE */
  ulong        biGammaRed;
  ulong        biGammaGreen;
  ulong        biGammaBlue;
  ulong        biIntent;
  ulong        biProfileData;
  ulong        biProfileSize;
  ulong        biReserved;
} BITMAPINFOHEADERV5;


#ifdef MINGW
typedef struct __attribute__((__packed__)) tagBITMAPFILEHEADER
#else
typedef struct tagBITMAPFILEHEADER
#endif
{
    uchar   bfType[2];
    ulong   bfSize;
    ushort  bfReserved1;
    ushort  bfReserved2;
    ulong   bfOffBits;
} BITMAPFILEHEADER;

#ifdef MINGW
typedef struct __attribute__((__packed__)) tagBMPHEADER
#else
typedef struct tagBMPHEADER
#endif
{
BITMAPFILEHEADER bfi;
BITMAPINFOHEADER bmi;
} BMPHEADER;

/* picfile trailer structure */
#ifdef MINGW
typedef struct __attribute__((__packed__)) tagPICFILE
#else
typedef struct tagPICFILE
#endif
{
    uchar scb[200];
    uchar padding[56];
    uchar pal[200][32]; /* note the PIC file has 16 palette entries */
} PICFILE;

/* constant for the biCompression field */
#define BI_RGB      0L

/* static structures for processing bmp input files */
BITMAPFILEHEADER BitMapFileHeader;
RGBQUAD sbmp[256]; /* super vga - applewin */
sshort palettesused = 0, palettecolorsused[16];

BITMAPINFOHEADER bmi;
/* support for BMP version 4 but not sure about other versions */
BITMAPINFOHEADERV5 bmiV5;

/* DHR Image Fragments - Sprites */
BMPHEADER mybmp;
ushort bmpwidth = 0, bmpheight = 0, fragwidth = 140, fragheight = 192, fragx = 0, fragy = 0;

/* SHR PIC file */
PICFILE mypic;

#define ASCIIZ  0
#define CRETURN 13
#define LFEED   10

uchar *szTextTitle =
    "A2FCBmp(C) Version 8.0 Copyright Bill Buckels 2012-2015\n"
    "All Rights Reserved.";

#define SUCCESS  0
#define VALID    SUCCESS
#define FAILURE  -1
#define INVALID  FAILURE

#define NUM_RGB_COLORS     3
#define NUM_VGA_COLORS    16

/* 280 x 192 - the default output format */
uchar BMP_header[] ={
0x42, 0x4D, 0x36, 0x76, 0x02, 0x00, 0x00, 0x00,
0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
0x00, 0x00, 0x18, 0x01, 0x00, 0x00, 0xC0, 0x00,
0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x76, 0x02, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* 140 x 192 - verbatim output format */
uchar BMP140_header[] ={
0x42, 0x4D, 0x36, 0x3B, 0x01, 0x00, 0x00, 0x00,
0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
0x00, 0x00, 0x8C, 0x00, 0x00, 0x00, 0xC0, 0x00,
0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x3B, 0x01, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* monochrome output routines - ported from dmono */
/* headers for monochrome BMP files */

/* 560 x 384 - the default output format */
uchar mono384[62] ={
0x42, 0x4D, 0x3E, 0x6C, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x28, 0x00,
0x00, 0x00, 0x30, 0x02, 0x00, 0x00, 0x80, 0x01,
0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x6C, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00};

/* 560 x 192 - verbatim output format */
uchar mono192[62] ={
0x42, 0x4D, 0x3E, 0x36, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x28, 0x00,
0x00, 0x00, 0x30, 0x02, 0x00, 0x00, 0xC0, 0x00,
0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00};

uchar msk[]={0x80,0x40,0x20,0x10,0x8,0x4,0x2,0x1};
uchar bmpscanline[1680], bmpscanline2[1680];

uchar buf560[560];

/* built-in palette options */
/* based on DHGR, DLGR, LGR colors */
/* LGR color order */
uchar kegs32colors[16][3] = {
    0,0,0,       /* black    */
    221,0,51,    /* red      */
    0,0,153,     /* dk blue  */
    221,0,221,   /* purple   */
    0,119,0,     /* dk green */
    85,85,85,    /* gray     */
    34,34,255,   /* med blue */
    102,170,255, /* lt blue  */
    136,85,34,   /* brown    */
    255,102,0,   /* orange   */
    170,170,170, /* grey     */
    255,153,136, /* pink     */
    0,221,0,     /* lt green */
    255,255,0,   /* yellow   */
    0,255,153,   /* aqua     */
    255,255,255};/* white    */

uchar ciderpresscolors[16][3] = {
    0,0,0,       /* black    */
    221,0,51,    /* red      */
    0,0,153,     /* dk blue  */
    221,34,221,  /* purple   */
    0,119,34,    /* dk green */
    85,85,85,    /* gray     */
    34,34,255,   /* med blue */
    102,170,255, /* lt blue  */
    136,85,0,    /* brown    */
    255,102,0,   /* orange   */
    170,170,170, /* grey     */
    255,153,136, /* pink     */
    17,221,0,    /* lt green */
    255,255,0,   /* yellow   */
    68,255,153,  /* aqua     */
    255,255,255};/* white    */

uchar awinoldcolors[16][3] = {
    0,0,0,       /* black    */
    208,0,48,    /* red      */
    0,0,128,     /* dk blue  */
    255,0,255,   /* purple   */
    0,128,0,     /* dk green */
    128,128,128, /* gray     */
    0,0,255,     /* med blue */
    96,160,255,  /* lt blue  */
    128,80,0,    /* brown    */
    255,128,0,   /* orange   */
    192,192,192, /* grey     */
    255,144,128, /* pink     */
    0,255,0,     /* lt green */
    255,255,0,   /* yellow   */
    64,255,144,  /* aqua     */
    255,255,255};/* white    */

uchar awinnewcolors[16][3] = {
    0,0,0,       /* black    */
    157,9,102,   /* red      */
    42,42,229,   /* dk blue  */
    199,52,255,  /* purple   */
    0,118,26,    /* dk green */
    128,128,128, /* gray     */
    13,161,255,  /* med blue */
    170,170,255, /* lt blue  */
    85,85,0,     /* brown    */
    242,94,0,    /* orange   */
    192,192,192, /* grey     */
    255,137,229, /* pink     */
    56,203,0,    /* lt green */
    213,213,26,  /* yellow   */
    98,246,153,  /* aqua     */
    255,255,255};/* white    */

/* http://en.wikipedia.org/wiki/List_of_8-bit_computer_hardware_palettes */
uchar wikipedia[16][3] = {
    0,0,0,       /* black */
    114,38,64,   /* red */
    64,51,127,   /* dk blue */
    228,52,254,  /* purple */
    14,89,64,    /* dk green */
    128,128,128, /* gray */
    27,154,254,  /* med blue */
    191,179,255, /* lt blue */
    64,76,0,     /* brown */
    228,101,1,   /* orange */
    128,128,128, /* grey */
    241,166,191, /* pink */
    27,203,1,    /* lt green */
    191,204,128, /* yellow */
    141,217,191, /* aqua */
    255,255,255};/* white */

/* http://wsxyz.net/tohgr.html */
/* Sheldon Simm's palette from todhr */
/*

Sheldon is clipping the black and white ranges.

The usual reason for doing this is for dirty images (caused by poor digitizing or
sampling or re-sampling color depth loss due to scaling). By using a clipping threshold
at either end of the rgb range, blacks that are not quite black don't match to some other
dark color and whites that are not quite white don't match to some other light color.

I too put this in place as a clipping option when I wrote Bmp2SHR about a year ago in my
Brooks output routines but it worked a little differently. I didn't build it into a
palette for one thing.

Sheldon's weighting favours clipping blue gun values at both ends of the range.

Red is clipped more than green in the high range, and green is clipped more than
red in the low range.

 static Pixel pal[] = {
    {   1,   4,   8}, // 0 black
    {  32,  54, 212}, // 1 dk blue
    {  51, 111,   0}, // 2 dk green
    {   7, 168, 225}, // 3 med blue
    {  99,  77,   0}, // 4 brown
    { 126, 126, 126}, // 5 gray
    {  67, 200,   0}, // 6 lt green
    {  93, 248, 133}, // 7 aqua
    { 148,  12, 125}, // 8 red
    { 188,  55, 255}, // 9 purple
    { 126, 126, 126}, // A grey
    { 158, 172, 255}, // B lt blue
    { 249,  86,  29}, // C orange
    { 255, 129, 236}, // D pink
    { 221, 206,  23}, // E yellow
    { 248, 250, 244}  // F white

    Also note that I use the array name grpal. This was used as a palette in
    one of Sheldon's previous versions. Since I have propagated this array name
    to my code and my code is working fine with it, I have no plans to change it
    to maintain currency with Sheldon's code.

*/
uchar grpal[16][3] = {
    0,0,0,       /* black */
    148,12,125,  /* red - hgr 0*/
    32,54,212,   /* dk blue - hgr 0 */
    188,55,255,  /* purple - default HGR overlay color */
    51,111,0,    /* dk green - hgr 0 */
    126,126,126, /* gray - hgr 0 */
    7,168,225,   /* med blue */
    158,172,255, /* lt blue - hgr 0 - alternate HGR overlay color*/
    99,77,0,     /* brown - hgr 0 */
    249,86,29,   /* orange */
    126,126,126, /* grey - hgr 0 */
    255,129,236, /* pink - hgr 0 */
    67,200,0,    /* lt green */
    221,206,23,  /* yellow - hgr 0 */
    93,248,133,  /* aqua - hgr 0 */
    255,255,255};/* white */

/* Sheldon still uses the RGB values from his old palette for HGR conversion */
uchar hgrpal[16][3] = {
    0x00,0x00,0x00, /* black */
    0xad,0x18,0x28, /* red */
    0x55,0x1b,0xe1, /* dk blue */
    0xe8,0x2c,0xf8, /* purple 232,44,248 - default hgr overlay color */
    0x01,0x73,0x63, /* dk green */
    0x7e,0x82,0x7f, /* gray */
    0x34,0x85,0xfc, /* med blue - 52,133,252 - alternate HGR overlay color */
    0xd1,0x95,0xff, /* lt blue */
    0x33,0x6f,0x00, /* brown */
    0xd0,0x81,0x01, /* orange */
    0x7f,0x7e,0x77, /* grey */
    0xfe,0x93,0xa3, /* pink */
    0x1d,0xd6,0x09, /* lt green */
    0xae,0xea,0x22, /* yellow */
    0x5b,0xeb,0xd9, /* aqua */
    0xff,0xff,0xff};/* white */

/* imported palette file */
uchar rgbUser[16][3];

uchar SuperConvert[16][3] = {
    0,0,0,       /* black */
    221,0,51,    /* red */
    0,0,153,     /* dk blue */
    221,0,221,   /* purple */
    0,119,0,     /* dk green */
    85,85,85,    /* gray */
    34,34,255,   /* med blue */
    102,170,255, /* lt blue */
    136,85,34,   /* brown */
    255,102,0,   /* orange */
    170,170,170, /* grey */
    255,153,136, /* pink */
    0,221,0,     /* lt green */
    255,255,0,   /* yellow */
    0,255,153,   /* aqua */
    255,255,255};/* white */

uchar Jace[16][3] = {
    0,0,0,        /* black */
    177,0,93,     /* red */
    32,41,255,    /* dk blue */
    210,41,255,   /* purple */
    0,127,34,     /* dk green */
    127,127,127,  /* gray */
    0,168,255,    /* med blue */
    160,168,255,  /* lt blue */
    94,86,0,      /* brown */
    255,86,0,     /* orange */
    127,127,127,  /* grey */
    255,127,220,  /* pink */
    44,213,0,     /* lt green */
    222,213,0,    /* yellow */
    77,255,161,   /* aqua */
    255,255,255}; /* white */

/* https://github.com/cybernesto/VBMP/wiki/Converting-a-picture-into-a-DHGR-color-image-using-the-GIMP */
/* Robert Munafo - http://mrob.com/pub/xapple2/colors.html */
uchar Cybernesto[16][3] = {
      0,  0,  0,    /* 0  Black       */
    227, 30, 96,    /* 1  Magenta     */
     96, 78,189,    /* 8  Dark Blue   */
    255, 68,253,    /* 9  Violet      */
      0,163, 96,    /* 4  Dark Green  */
    156,156,156,    /* 5  Grey1       */
     20,207,253,    /* 12 Medium Blue */
    208,195,255,    /* 13 Light Blue  */
     96,114,  3,    /* 2  Brown       */
    255,106, 60,    /* 3  Orange      */
    156,156,156,    /* 10 Grey2       */
    255,160,208,    /* 11 Pink        */
     20,245, 60,    /* 6  Green       */
    208,221,141,    /* 7  Yellow      */
    114,255,208,    /* 14 Aqua        */
    255,255,255};   /* 15 White       */

/* merged palette */
/* initial values are the average values of p5 NTSC and p12 RGB */
uchar PseudoPalette[16][3] = {
    0,0,0,
    184,6,88,
    16,27,182,
    204,27,238,
    25,115,0,
    105,105,105,
    20,101,240,
    130,171,255,
    117,81,17,
    252,94,14,
    148,148,148,
    255,141,186,
    33,210,0,
    238,230,11,
    46,251,143,
    255,255,255};

/* goes with the canvas bmp that I put out there with AppleX */
uchar rgbCanvasArray[16][3] = {
0  , 0  , 0  ,
208, 0  , 48 ,
0  , 0  , 128,
255, 0  , 255,
0  , 128, 0  ,
128, 128, 128,
0  , 0  , 255,
96 , 160, 255,
128, 80 , 0  ,
255, 128, 0  ,
192, 192, 192,
255, 144, 128,
0  , 255, 0  ,
255, 255, 0  ,
64 , 255, 144,
255, 255, 255};

/* this might work with old Win16 16 color BMP's */
uchar rgbBmpArray[16][3] = {
0  ,0  , 0  ,
191,0  , 0  ,
0  ,0  , 191,
191,0  , 191,
0  ,191, 0  ,
128,128, 128,
0  ,191, 191,
0  ,0  , 255,
191,191, 0  ,
255,0  , 0  ,
192,192, 192,
255,0  , 255,
0  ,255, 0  ,
255,255, 0  ,
0  ,255, 255,
255,255, 255};

/* this might work with new Win32 16 color BMP's */
uchar rgbXmpArray[16][3] = {
0  , 0  , 0  ,
128, 0  , 0  ,
0  , 0  , 128,
128, 0  , 128,
0  , 128, 0  ,
128, 128, 128,
0  , 128, 128,
0  , 0  , 255,
128, 128, 0  ,
255, 0  , 0  ,
192, 192, 192,
255, 0  , 255,
0  , 255, 0  ,
255, 255, 0  ,
0  , 255, 255,
255, 255, 255};

/* from the bios in some PC I had */
uchar rgbVgaArray[16][3] = {
0  , 0  , 0  ,
255, 0  , 0  ,
0  , 0  , 255,
255, 0  , 255,
0  , 255, 0  ,
85 , 85 , 85 ,
0  , 255, 255,
85 , 85 , 255,
255, 255, 0  ,
255, 85 , 85 ,
192, 192, 192,
255, 85 , 255,
85 , 255, 85 ,
255, 255, 85 ,
85 , 255, 255,
255, 255, 255};

/* some old ZSoft VGA Pcx Colors */
uchar rgbPcxArray[16][3] = {
0  , 0  , 0  ,
170, 0  , 0  ,
0  , 0  , 170,
170, 0  , 170,
0  , 170, 0  ,
85 , 85 , 85 ,
0  , 170, 170,
85 , 85 , 255,
170, 170, 0  ,
255, 85 , 85 ,
170, 170, 170,
255, 85 , 255,
85 , 255, 85 ,
255, 255, 85 ,
85 , 255, 255,
255, 255, 255};

/* our working copy of the apple II double hires colors */
/* this is in Apple II lo-res color order */
/* todhr palette */
uchar rgbArray[NUM_VGA_COLORS][NUM_RGB_COLORS]={
    0,0,0,       /* black */
    148,12,125,  /* red */
    32,54,212,   /* dk blue */
    188,55,255,  /* purple */
    51,111,0,    /* dk green */
    126,126,126, /* gray */
    7,168,225,   /* med blue */
    158,172,255, /* lt blue */
    99,77,0,     /* brown */
    249,86,29,   /* orange */
    126,126,126, /* grey */
    255,129,236, /* pink */
    67,200,0,    /* lt green */
    221,206,23,  /* yellow */
    93,248,133,  /* aqua */
    255,255,255};/* white */

char *colornames[] = {
"black",
"red",
"dkblue",
"purple",
"dkgreen",
"gray",
"medblue",
"ltblue",
"brown",
"orange",
"grey",
"pink",
"ltgreen",
"yellow",
"aqua",
"white"};


/* for SHR output */
/* 200 brooks palettes */
uchar rgbArrays[200][16][3];
uchar rgbUsed[200][16];
double rgbDistance[200][16];
/* save palettes for palette distance resassignment */
uchar savepalettes[200][16][3];
uchar savescb[200];

/* 16 pic palettes */
uchar rgb256Arrays[200][16][3];
uchar rgb256Used[16][16];
double rgb256Distance[16][16];

/* all possible EGA ECD Values in Indexed Order */
uchar rgbEgaArray[64][3] = {
0x00, 0x00, 0x00,
0x00, 0x00, 0xaa,
0x00, 0xaa, 0x00,
0x00, 0xaa, 0xaa,
0xaa, 0x00, 0x00,
0xaa, 0x00, 0xaa,
0xaa, 0xaa, 0x00,
0xaa, 0xaa, 0xaa,
0x00, 0x00, 0x55,
0x00, 0x00, 0xff,
0x00, 0xaa, 0x55,
0x00, 0xaa, 0xff,
0xaa, 0x00, 0x55,
0xaa, 0x00, 0xff,
0xaa, 0xaa, 0x55,
0xaa, 0xaa, 0xff,
0x00, 0x55, 0x00,
0x00, 0x55, 0xaa,
0x00, 0xff, 0x00,
0x00, 0xff, 0xaa,
0xaa, 0x55, 0x00,
0xaa, 0x55, 0xaa,
0xaa, 0xff, 0x00,
0xaa, 0xff, 0xaa,
0x00, 0x55, 0x55,
0x00, 0x55, 0xff,
0x00, 0xff, 0x55,
0x00, 0xff, 0xff,
0xaa, 0x55, 0x55,
0xaa, 0x55, 0xff,
0xaa, 0xff, 0x55,
0xaa, 0xff, 0xff,
0x55, 0x00, 0x00,
0x55, 0x00, 0xaa,
0x55, 0xaa, 0x00,
0x55, 0xaa, 0xaa,
0xff, 0x00, 0x00,
0xff, 0x00, 0xaa,
0xff, 0xaa, 0x00,
0xff, 0xaa, 0xaa,
0x55, 0x00, 0x55,
0x55, 0x00, 0xff,
0x55, 0xaa, 0x55,
0x55, 0xaa, 0xff,
0xff, 0x00, 0x55,
0xff, 0x00, 0xff,
0xff, 0xaa, 0x55,
0xff, 0xaa, 0xff,
0x55, 0x55, 0x00,
0x55, 0x55, 0xaa,
0x55, 0xff, 0x00,
0x55, 0xff, 0xaa,
0xff, 0x55, 0x00,
0xff, 0x55, 0xaa,
0xff, 0xff, 0x00,
0xff, 0xff, 0xaa,
0x55, 0x55, 0x55,
0x55, 0x55, 0xff,
0x55, 0xff, 0x55,
0x55, 0xff, 0xff,
0xff, 0x55, 0x55,
0xff, 0x55, 0xff,
0xff, 0xff, 0x55,
0xff, 0xff, 0xff};


sshort rgb2ega(uchar red, uchar green, uchar blue)
{

    /* Converts from 24 bit color to Ega color index
       EGA with ECD 16 color registers (0-63) 1 byte each */
    sshort idx = 0;

    /* find gun value index for EGA table comparison using thresholds */
    if (red < 43) red = 0;         /* 0x00 */
    else if (red < 128) red = 85;  /* 0x55 */
    else if (red < 224) red = 170; /* 0xaa */
    else red = 255;                /* 0xff */

    if (green < 43) green = 0;
    else if (green < 128) green = 85;
    else if (green < 213) green = 170;
    else green = 255;

    if (blue < 43) blue = 0;
    else if (blue < 128) blue = 85;
    else if (blue < 213) blue = 170;
    else blue = 255;

     /* values are now identicalized so use comparators in arrays to
     determine ega color index

     visual verification can be found at:
     standard - http:en.wikipedia.org/wiki/Enhanced_Graphics_Adapter
     extended - http:en.wikipedia.org/wiki/Image:EGA_Table.PNG

     discussion about bit patterns and visuals also there */

    for (idx = 0; idx < 64; idx++) {
        if (red == rgbEgaArray[idx][0] &&
            green == rgbEgaArray[idx][1] &&
            blue == rgbEgaArray[idx][2]) return idx;
    }

    /* never gets to here */
    return 0;

}

sshort globalthreshold = 4, rhold = 4, ghold = 4, bhold = 4;


/* set thresholds for color reduction on 24 bit values */
uchar sethold(uchar ch, sshort hold)
{

    if (hold == 0) return ch;


    /* just hand-bombed some hardcoded color thresholds here
       they probably work just about as well as anything */
    switch(hold) {
        case 0:
            if (ch < 192) ch = 0;
            else ch = 255;
            break;
        case 1:
            if (ch < 64) ch = 0;
            else ch = 255;
            break;
        case 2:
            if (ch < 128) ch = 0;
            else ch = 255;
            break;
        case 3:
            /* adjust gun values using EGA-like thresholds */
            /* mode640 optimized threshold option */
            if (ch < 85) ch = 0;         /* 0x00 */
            else if (ch < 170) ch = 128;
            else ch = 255;               /* 0xff */
            break;
        case 4:
            /* adjust gun values using EGA-like thresholds */
            /* threshold default option */
            if (ch < 43) ch = 0;         /* 0x00 */
            else if (ch < 128) ch = 85;  /* 0x55 */
            else if (ch < 224) ch = 170; /* 0xaa */
            else ch = 255;               /* 0xff */
            break;
            /* additional thresholds */
        case 5:
            if (ch < 25) ch = 0;    /* 0x00 */
            else if (ch < 76) ch = 51;
            else if (ch < 127) ch = 102;
            else if (ch < 178) ch = 153;
            else if (ch < 229) ch = 204;
            else ch = 255;           /* 0xff */
            break;
        case 6:
            /* mode320 optimized threshold option */
            if (ch < 0x15) ch = 0;    /* 0x00 */
            else if (ch < 0x3F) ch = 0x2A;
            else if (ch < 0x69) ch = 0x54;
            else if (ch < 0x93) ch = 0x7E;
            else if (ch < 0xbd) ch = 0xa8;
            else if (ch < 0xe7) ch = 0xd6;
            else ch = 255;           /* 0xff */
            break;
        case 7:
            if (ch < 18) ch = 0;    /* 0x00 */
            else if (ch < 54) ch = 36;
            else if (ch < 90) ch = 72;
            else if (ch < 126) ch = 108;
            else if (ch < 162) ch = 144;
            else if (ch < 198) ch = 180;
            else if (ch < 234) ch = 216;
            else ch = 255;           /* 0xff */
            break;
        case 8:
            if (ch < 0x10) ch = 0;    /* 0x00 */
            else if (ch < 0x30) ch = 0x20;
            else if (ch < 0x50) ch = 0x40;
            else if (ch < 0x70) ch = 0x60;
            else if (ch < 0x90) ch = 0x80;
            else if (ch < 0xb0) ch = 0xa0;
            else if (ch < 0xd0) ch = 0xc0;
            else if (ch < 0xf0) ch = 0xe0;
            else ch = 255;           /* 0xff */
            break;
        case 9:
            if (ch < 14) ch = 0;    /* 0x00 */
            else if (ch < 42) ch = 28;
            else if (ch < 70) ch = 56;
            else if (ch < 98) ch = 84;
            else if (ch < 126) ch = 112;
            else if (ch < 154) ch = 140;
            else if (ch < 182) ch = 168;
            else if (ch < 210) ch = 196;
            else if (ch < 238) ch = 224;
            else ch = 255;           /* 0xff */
            break;
        case 10:
            if (ch < 10) ch = 0;    /* 0x00 */
            else if (ch < 35) ch = 25;
            else if (ch < 60) ch = 50;
            else if (ch < 85) ch = 75;
            else if (ch < 110) ch = 100;
            else if (ch < 135) ch = 125;
            else if (ch < 160) ch = 150;
            else if (ch < 185) ch = 175;
            else if (ch < 210) ch = 200;
            else if (ch < 235) ch = 225;
            else ch = 255;           /* 0xff */
            break;
        case 11:
            if (ch < 10) ch = 0;    /* 0x00 */
            else if (ch < 33) ch = 23;
            else if (ch < 56) ch = 46;
            else if (ch < 79) ch = 69;
            else if (ch < 102) ch = 92;
            else if (ch < 125) ch = 115;
            else if (ch < 148) ch = 138;
            else if (ch < 171) ch = 161;
            else if (ch < 194) ch = 184;
            else if (ch < 217) ch = 207;
            else if (ch < 240) ch = 230;
            else ch = 255;           /* 0xff */
            break;
    }
    return ch;
}



unsigned char RemapLoToHi[16] = {
    LOBLACK,
    LORED,
    LOBROWN,
    LOORANGE,
    LODKGREEN,
    LOGRAY,
    LOLTGREEN,
    LOYELLOW,
    LODKBLUE,
    LOPURPLE,
    LOGREY,
    LOPINK,
    LOMEDBLUE,
    LOLTBLUE,
    LOAQUA,
    LOWHITE};

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


unsigned char *dhrbuf=NULL;

/*

The following is logically reordered to match the lores
color order...

                                                Repeated
                                                Binary
          Color         aux1  main1 aux2  main2 Pattern
          Black          00    00    00    00    0000
          Magenta        08    11    22    44    0001
          Dark Blue      11    22    44    08    1000
          Violet         19    33    66    4C    1001
          Dark Green     22    44    08    11    0100
          Grey1          2A    55    2A    55    0101
          Medium Blue    33    66    4C    19    1100
          Light Blue     3B    77    6E    5D    1101
          Brown          44    08    11    22    0010
          Orange         4C    19    33    66    0011
          Grey2          55    2A    55    2A    1010
          Pink           5D    3B    77    6E    1011
          Green          66    4C    19    33    0110
          Yellow         6E    5D    3B    77    0111
          Aqua           77    6E    5D    3B    1110
          White          7F    7F    7F    7F    1111
*/

/* repeated binary pattern */
unsigned char dhrbits[16][28] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  /* Black       */
  0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,  /* Magenta     */
  1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,  /* Dark Blue   */
  1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,  /* Violet      */
  0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,  /* Dark Green  */
  0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,  /* Grey1       */
  1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,  /* Medium Blue */
  1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,  /* Light Blue  */
  0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,  /* Brown       */
  0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,  /* Orange      */
  1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,  /* Grey2       */
  1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,  /* Pink        */
  0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,  /* Green       */
  0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,  /* Yellow      */
  1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,  /* Aqua        */
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}; /* White       */

/* the following array is based on the above */
unsigned char dhrbytes[16][4] = {
    0x00,0x00,0x00,0x00,
    0x08,0x11,0x22,0x44,
    0x11,0x22,0x44,0x08,
    0x19,0x33,0x66,0x4C,
    0x22,0x44,0x08,0x11,
    0x2A,0x55,0x2A,0x55,
    0x33,0x66,0x4C,0x19,
    0x3B,0x77,0x6E,0x5D,
    0x44,0x08,0x11,0x22,
    0x4C,0x19,0x33,0x66,
    0x55,0x2A,0x55,0x2A,
    0x5D,0x3B,0x77,0x6E,
    0x66,0x4C,0x19,0x33,
    0x6E,0x5D,0x3B,0x77,
    0x77,0x6E,0x5D,0x3B,
    0x7F,0x7F,0x7F,0x7F};


/* the following is used to remap
   double lo res 4 bit colors
   from bank 0 to bank 1 */
unsigned char dloauxcolor[16] = {
    0,8,1,9,2,10,3,11,4,12,5,13,6,14,7,15};

/* the following is used to remap
   double lo res 4 bit colors
   from bank 1 to bank 0 */
unsigned char dlomaincolor[16] = {
    0,2,4,6,8,10,12,14,1,3,5,7,9,11,13,15};


/* -------------------------------------------------------------- */
/* local random number generator                                  */
/* -------------------------------------------------------------- */
/* http://stackoverflow.com/questions/7602919/how-do-i-generate-random-numbers-without-rand-function */
ushort RandomSeed = (ushort)0xACE1;
ushort random()
{
    ushort bit = ((RandomSeed >> 0) ^ (RandomSeed >> 2) ^ (RandomSeed >> 3) ^ (RandomSeed >> 5) ) & 1;

    RandomSeed =  (RandomSeed >> 1) | (bit << 15);

    return RandomSeed;
}

/* ------------------------------------------------------------- */
/* RandomRange                                                   */
/*   returns a random number in a usuable range  (1 to MaxValue) */
/* args                                                          */
/*   MaxValue - the highest number we want                       */
/* ------------------------------------------------------------- */
int RandomRange(int iMaxValue)
{
  int iRetVal;

  do {
    /* get random number */
    iRetVal = (int)random();
    /* get a positive value */
    if (iRetVal < 0) iRetVal *= -1;

  } while(iRetVal < 1);

  /* use modulus of MaxValue if not in range */
  if (iRetVal > iMaxValue)
    iRetVal = (iRetVal%iMaxValue)+1;

  return iRetVal;               /* return a value in range */
}


/* convert from native format to Apple II format short integer */
ushort Motorola16(ushort val)
{
    uchar buf[2];
    ushort *ptr;

    /* msb in smallest address */
    buf[0] = (uchar) (val % 256); val = val/256;
    buf[1] = (uchar) (val % 256);

    ptr = (ushort *)&buf[0];
    val = ptr[0];

    return val;
}

/* this is also used to write width, height pairs for LGR and DLGR file headers
   when the file is wider than 255 pixels */
void WriteDosHeader(FILE *fp, ushort fl, ushort fa)
{
        fa = Motorola16(fa);/* file bload address - not including this header */
        fl = Motorola16(fl);/* file length - not including this header */

        fwrite((char *)&fa,sizeof(ushort),1,fp);
        fwrite((char *)&fl,sizeof(ushort),1,fp);
}

/* upper case name for Apple II Output */
void ucase(char *str)
{
    int idx;

    for (idx = 0; str[idx] != (char)0; idx++) {
        str[idx] = toupper(str[idx]);
    }
}

/* sets up a line oriented write buffer for lores and double lo-res files */
/* this uses a different LGR and DLGR buffer organization method than Bmp2DHR */
void setlopixel(unsigned char color,int x, int y)
{
     unsigned char *crt, c1, c2;
     int y1, offset;

     if (doublelores == 1) {
         if (x%2 == 0) {
             /* auxiliary memory uses a different color index value */
             color = dloauxcolor[color];
             /* first 160 bytes goes to auxiliary memory (even pixels) */
             offset = (x/2);
         }
         else {
             /* next 160 bytes goes to main memory (odd pixels) */
             offset = 160 + (x/2);
         }
     }
     else {
         offset = x;
     }

     y1 = y / 2;

     c2 = (unsigned char ) (color & 15);

     if (y%2 == 0) {
         /* even rows in low nibble */
         /* mask value to preserve high nibble */
         c1 = 240;
     }
     else {
         /* odd rows in high nibble */
         /* mask value to preserve low nibble */
         c1 = 15;
         c2 = c2 * 16;
     }

     /* each paired scanline is offset by 320 bytes in our write buffer */
     offset += (y1 * 320);

     crt = (unsigned char *)&dhrbuf[offset];
     crt[0] &= c1;
     crt[0] |= c2;
}

uchar getlopixel(int x, int y)
{
     unsigned char *crt, color;
     int y1, offset;



     if (doublelores == 1) {
         if (x%2 == 0) {
             /* first 160 bytes goes to auxiliary memory (even pixels) */
             offset = (x/2);
         }
         else {
             /* next 160 bytes goes to main memory (odd pixels) */
             offset = 160 + (x/2);
         }
     }
     else {
         offset = x;
     }

     y1 = y / 2;

     /* each paired scanline is offset by 320 bytes in our write buffer */
     offset += (y1 * 320);

     crt = (unsigned char *)&dhrbuf[offset];

     if (y%2 == 0) {
         /* even rows in low nibble */
         color = crt[0] & 15;
     }
     else {
         /* odd rows in high nibble */
         color = crt[0] >> 4;

     }

     if (doublelores == 1 && x%2 == 0) {
        /* auxiliary memory uses a different color index value */
         color = dlomaincolor[color];
     }

     return color;
}

ushort WriteDIBHeader(FILE *fp, ushort pixels, ushort rasters)
{
    ushort outpacket;
    int c;

    memset((char *)&mybmp.bfi.bfType[0],0,sizeof(BMPHEADER));

    /* create the info header */
    mybmp.bmi.biSize = (ulong)sizeof(BITMAPINFOHEADER);
    mybmp.bmi.biWidth  = (ulong)pixels;
    mybmp.bmi.biHeight = (ulong)rasters;
    mybmp.bmi.biPlanes = 1;
    mybmp.bmi.biBitCount = 24;
    mybmp.bmi.biCompression = (ulong) BI_RGB;

    /* BMP scanlines are padded to a multiple of 4 bytes (DWORD) */
    outpacket = (ushort)mybmp.bmi.biWidth * 3;
    while (outpacket%4 != 0)outpacket++;
    mybmp.bmi.biSizeImage = (ulong)outpacket;
    mybmp.bmi.biSizeImage *= mybmp.bmi.biHeight;

    /* create the file header */
    mybmp.bfi.bfType[0] = 'B';
    mybmp.bfi.bfType[1] = 'M';
    mybmp.bfi.bfOffBits = (ulong) sizeof(BMPHEADER);
    mybmp.bfi.bfSize = mybmp.bmi.biSizeImage + mybmp.bfi.bfOffBits;

    /* write the header for the output BMP */
    c = fwrite((char *)&mybmp.bfi.bfType[0],sizeof(BMPHEADER),1,fp);

    if (c!= 1)outpacket = 0;

return outpacket;
}

/* DHGR file input Helper Function for HGR raster format image fragments */
int read_dhr(uchar *basename)
{

    FILE *fp;
    uchar infile[256], lodebuf[80];
    int c,y,status=INVALID,width,height,packet;
    unsigned dest;

    sprintf(infile,"%s.DHR",basename);
    fp = fopen(infile,"rb");

    if (NULL == fp && longnames == 1) {
        sprintf(infile,"%s.DHR#062000",basename);
        fp = fopen(infile,"rb");
    }
    if (NULL == fp)return INVALID;

    memset(dhrbuf,0,16384);

    for (;;) {
        /* read 5 byte header */
        c = fread(lodebuf,1,5,fp);
        if (c != 5) break;
        if (lodebuf[0] != 'D' || lodebuf[1] != 'H' || lodebuf[2] != 'R') break;
        width = (int)lodebuf[3];
        height = (int)lodebuf[4];
        /* must be in a valid range */
        if (width < 4 || width > 80) break;
        if (height< 1 || height > 192) break;

        /* set some globals for BMP output */
        bmpwidth = (ushort)((width / 4) * 7);
        bmpheight = (ushort) height;

        status = SUCCESS;

        packet = width / 2;

        for (y = 0;y < height;y++) {

            c = fread(lodebuf,1,width,fp);
            if (c!= width) {
                status = INVALID;
                break;
            }
            dest = HB[y];
            /* move to auxiliary screen memory */
            memcpy((char *)&dhrbuf[dest-0x2000],(char *)&lodebuf[0],packet);
            /* move to main screen memory */
            memcpy((char *)&dhrbuf[dest],(char *)&lodebuf[packet],packet);

        }
        break;
    }

   fclose(fp);
   return status;
}

/* DHGR file input Helper Function for AUX, BIN file pairs */
/* a double hi-res color pixel can occur at any one of 7 positions */
/* in a 4 byte block which spans aux and main screen memory */
/* the horizontal resolution is 140 pixels */
/* read 2 input files */
int read_binaux(uchar *basename)
{

    FILE *fp;
    uchar infile[256];

    /* the bsaved images are split into two files
       the first file is loaded into aux mem */
    sprintf(infile,"%s.AUX",basename);
    fp = fopen(infile,"rb");

    if (NULL == fp && longnames == 1) {
        sprintf(infile,"%s.AUX#062000",basename);
        fp = fopen(infile,"rb");
    }
    if (NULL == fp)return INVALID;
    fread(dhrbuf,1,8192,fp);
    fclose(fp);

    /* the second file is loaded into main mem */
    sprintf(infile,"%s.BIN",basename);
    fp = fopen(infile,"rb");

    if (NULL == fp && longnames == 1) {
        sprintf(infile,"%s.BIN#062000",basename);
        fp = fopen(infile,"rb");
    }
    if (NULL == fp)return INVALID;
    fread(&dhrbuf[8192],1,8192,fp);
    fclose(fp);

    return SUCCESS;

}

/* DHGR file input Helper Function for A2FC files */
/* read one input file */
int read_2fc(uchar *basename)
{
    FILE *fp;
    uchar infile[256];

    if (longnames == 0) {
        sprintf(infile,"%s.2FC",basename);
        fp = fopen(infile,"rb");
    }
    else {
        for (;;) {
            /* support for longnames and ciderpress tags */
            /* support for tohgr output */
            if (tohgr == 0)
                sprintf(infile,"%s.2FC",basename);
            else
                sprintf(infile,"%s.dhgr",basename);

            fp = fopen(infile,"rb");
            if (NULL != fp)break;

            if (tohgr == 0) {
                if (mono == 0) sprintf(infile,"%s.A2FC",basename);
                else sprintf(infile,"%s.A2FM",basename);
            }
            else {
                sprintf(infile,"%s.DHGR",basename);
            }

            fp = fopen(infile,"rb");
            if (NULL != fp)break;

            if (tohgr == 0)
                sprintf(infile,"%s.2FC#062000",basename);
            else
                sprintf(infile,"%s.dhgr#062000",basename);

            fp = fopen(infile,"rb");
            if (NULL != fp)break;

            if (tohgr == 0) {
                if (mono == 0) sprintf(infile,"%s.A2FC#062000",basename);
                else sprintf(infile,"%s.A2FM#062000",basename);
            }
            else {
                sprintf(infile,"%s.DHGR#062000",basename);
            }

            fp = fopen(infile,"rb");
            break;
        }
    }

    if (NULL == fp)return INVALID;
    fread(dhrbuf,1,16384,fp);
    fclose(fp);

    return SUCCESS;

}

/* Color DHGR Pixel-Mapping  Helper Function */
/* returns the Apple II Double Hi-res drawcolor 0-15 */
/* a double hi-res color pixel can occur at any one of 7 positions */
/* in a 4 byte block which spans aux and main screen memory */
/* the horizontal resolution is 140 pixels */
int dhrgetpixel(int x,int y)
{
    int xoff, pattern, idx;
    unsigned char *ptraux, *ptrmain,c1, c2, d1, d2;


    if (x < 0 || x > 139 || y < 0 || y > 192) return INVALID;

    pattern = (x%7);
    xoff = HB[y] + ((x/7) * 2);
    ptraux  = (unsigned char *) &dhrbuf[xoff-0x2000];
    ptrmain = (unsigned char *) &dhrbuf[xoff];


    switch(pattern)
    {
        /* left this here for reference

        unsigned char dhrpattern[7][4] = {
        0,0,0,0,
        0,0,0,1,
        1,1,1,1,
        1,1,2,2,
        2,2,2,2,
        2,3,3,3,
        3,3,3,3};
        */

        /* compare colors in the input file to color patterns and return drawcolor */
        /* somewhat inelegant but lazy to read and debug if a problem */
        case 0: c1 = ptraux[0] &0x0f;
                for (idx = 0; idx < 16; idx++) {
                  d1 = dhrbytes[idx][0] & 0x0f;
                  if (d1 == c1) return idx;
                }
                break;
        case 1: c1 = ptraux[0] & 0x70;
                c2 = ptrmain[0] & 0x01;
                for (idx = 0; idx < 16; idx++) {
                  d1 = dhrbytes[idx][0] & 0x70;
                  d2 = dhrbytes[idx][1] & 0x01;
                  if (d1 == c1 && d2 == c2) return idx;
                }
                break;
        case 2: c1 = ptrmain[0] & 0x1e;
                for (idx = 0; idx < 16; idx++) {
                  d1 = dhrbytes[idx][1] & 0x1e;
                  if (d1 == c1) return idx;
                }
                break;
        case 3: c1 = ptrmain[0] & 0x60;
                c2 = ptraux[1] & 0x03;
                for (idx = 0; idx < 16; idx++) {
                  d1 = dhrbytes[idx][1] & 0x60;
                  d2 = dhrbytes[idx][2] & 0x03;
                  if (d1 == c1 && d2 == c2) return idx;
                }
                break;
        case 4: c1 = ptraux[1] & 0x3c;
                for (idx = 0; idx < 16; idx++) {
                  d1 = dhrbytes[idx][2] & 0x3c;
                  if (d1 == c1) return idx;
                }
                break;
        case 5: c1 = ptraux[1] & 0x40;
                c2 = ptrmain[1] & 0x07;
                for (idx = 0; idx < 16; idx++) {
                  d1 = dhrbytes[idx][2] & 0x40;
                  d2 = dhrbytes[idx][3] & 0x07;
                  if (d1 == c1 && d2 == c2) return idx;
                }
                break;
        case 6: c1 = ptrmain[1] & 0x78;
                for (idx = 0; idx < 16; idx++) {
                  d1 = dhrbytes[idx][3] & 0x78;
                  if (d1 == c1) return idx;
                }
                break;
    }

    return INVALID;
}

ushort WriteVbmpHeader(FILE *fp)
{
    ushort outpacket;
    int c, i, j;

    /* BMP scanlines are padded to a multiple of 4 bytes (DWORD) */
    outpacket = (ushort)72;

    if (mono != 0) {
        c = fwrite(mono192,1,sizeof(mono192),fp);
        if (c!= sizeof(mono192))return 0;
        return outpacket;
    }

    memset((char *)&mybmp.bfi.bfType[0],0,sizeof(BMPHEADER));

    /* create the info header */
    mybmp.bmi.biSize = (ulong)40;
    mybmp.bmi.biWidth  = (ulong)140;
    mybmp.bmi.biHeight = (ulong)192;
    mybmp.bmi.biPlanes = 1;
    mybmp.bmi.biBitCount = 4;
    mybmp.bmi.biCompression = (ulong) BI_RGB;

    mybmp.bmi.biSizeImage = (ulong)outpacket;
    mybmp.bmi.biSizeImage *= mybmp.bmi.biHeight;

    /* create the file header */
    mybmp.bfi.bfType[0] = 'B';
    mybmp.bfi.bfType[1] = 'M';
    mybmp.bfi.bfOffBits = (ulong) sizeof(BMPHEADER) + sizeof(RGBQUAD) * 16;
    mybmp.bfi.bfSize = mybmp.bmi.biSizeImage + mybmp.bfi.bfOffBits;

    /* write the header for the output BMP */
    c = fwrite((char *)&mybmp.bfi.bfType[0],sizeof(BMPHEADER),1,fp);

    if (c!= 1)return 0;

    for (i=0;i<16;i++) {
        j = RemapLoToHi[i];
        sbmp[i].rgbRed   = rgbArray[j][0];
        sbmp[i].rgbGreen = rgbArray[j][1];
        sbmp[i].rgbBlue  = rgbArray[j][2];
        sbmp[i].rgbReserved = 0;

    }

    /* write the palette for the output bmp */
    c = fwrite((char *)&sbmp[0].rgbBlue, sizeof(RGBQUAD)*16,1,fp);
    if (c!= 1)return 0;

return outpacket;
}

/* writes VBMP compatible 140 x 192 x 16 color bmp or VBMP compatible 560 x 192 monochrome bmp */
int WriteVBMPFile(unsigned char *vbmpfile)
{

    FILE *fp;
    uchar ch;
    int x,x1,y,y2,idx,j,packet=72;

    fp = fopen(vbmpfile,"wb");

    if (fp == NULL) {
        printf("Error opening %s for writing!\n",vbmpfile);
        return INVALID;
    }

    if (WriteVbmpHeader(fp) == 0) {
        fclose(fp);
        remove(vbmpfile);
        printf("Error writing header to %s!\n",vbmpfile);
        return INVALID;
    }
    memset(&bmpscanline[0],0,packet);

    /* write 4 bit packed scanlines */
    /* remap from LORES color order to DHGR color order */
    /* VBMP does not use the colors in the palette, just the color order */

    y2 = 191;
    for (y = 0; y< 192; y++) {
       for (x = 0, x1=0; x < 140; x++) {
          if (x%2 == 0) {
            idx = dhrgetpixel(x,y2);
            /* range check */
            if (idx < 0 || idx > 15)idx = 0; /* default black */
            j = RemapLoToHi[idx];
            ch = (uchar)j << 4;
          }
          else {
            idx = dhrgetpixel(x,y2);
            /* range check */
            if (idx < 0 || idx > 15)idx = 0; /* default black */
            j = RemapLoToHi[idx];
            bmpscanline[x1] = ch | (uchar)j; x1++;
          }
       }

       fwrite((char *)&bmpscanline[0],1,packet,fp);
       y2 -= 1;
    }

    fclose(fp);
    return SUCCESS;

}

/* Color Output Helper Function */
int save_to_bmp24(uchar *basename)
{

    FILE *fp;
    uchar outfile[256], tempr, tempg, tempb;
    int x,x1,y,y2,idx,packet,xoffset=0,yoffset=0;

    sprintf(outfile,"%s.bmp",basename);

    if (vbmp == 1) return WriteVBMPFile(outfile);

    if (frag == 1) {
        /* create BMP image fragment from full-screen Apple II input */
        dhr = 1;
        bmpwidth  =  fragwidth;
        bmpheight =  fragheight;
        xoffset   =  fragx;
        yoffset   =  fragy;
    }

    fp = fopen(outfile,"wb");
    if (NULL == fp)return INVALID;

    /* write rgb triples and double each pixel to preserve the aspect ratio */
    if (doublepixel == 1) {
        /* write header for 280 x 192 x 24 bit bmp */
        if (dhr == 1) packet = (int)WriteDIBHeader(fp,bmpwidth*2,bmpheight);
        else fwrite(BMP_header,1,sizeof(BMP_header),fp);
    }
    else {
        /* write header for 140 x 192 x 24 bit bmp */
        if (dhr == 1) packet = (int)WriteDIBHeader(fp,bmpwidth,bmpheight);
        else fwrite(BMP140_header,1,sizeof(BMP140_header),fp);
    }

    if (dhr == 1) {
        memset(&bmpscanline[0],0,840);
        y2 = (bmpheight) - 1;
        for (y = 0; y< bmpheight; y++) {
           for (x = 0, x1=0; x < bmpwidth; x++) {
              idx = dhrgetpixel(x+xoffset,y2+yoffset);

              if (idx < 0 || idx > 15)idx = 0;

              tempr = rgbArray[idx][0];
              tempg = rgbArray[idx][1];
              tempb = rgbArray[idx][2];

              bmpscanline[x1] = tempb; x1++;
              bmpscanline[x1] = tempg; x1++;
              bmpscanline[x1] = tempr; x1++;

              if (doublepixel == 1) {
                bmpscanline[x1] = tempb; x1++;
                bmpscanline[x1] = tempg; x1++;
                bmpscanline[x1] = tempr; x1++;
              }
           }
           fwrite((char *)&bmpscanline[0],1,packet,fp);
           y2 -= 1;
        }

    }
    else {
        y2 = 191;
        for (y = 0; y< 192; y++) {

           for (x = 0; x < 140; x++) {
              idx = dhrgetpixel(x,y2);

              /* range check */
              if (idx < 0 || idx > 15)idx = 0; /* default to black */

              tempr = rgbArray[idx][0];
              tempg = rgbArray[idx][1];
              tempb = rgbArray[idx][2];

              /* reverse order */
              fputc(tempb, fp);
              fputc(tempg, fp);
              fputc(tempr, fp);

              if (doublepixel == 1) {
                 /* double-up */
                 fputc(tempb, fp);
                 fputc(tempg, fp);
                 fputc(tempr, fp);
              }
           }
           y2 -= 1;
        }
    }

    fclose(fp);
    return SUCCESS;

}


/* encodes apple II dhgr scanline into buffer */
int applewinbits(int y)
{
        int xoff,idx,jdx;
        unsigned char *ptraux, *ptrmain, bits[7];

        xoff = HB[y];
        ptraux  = (unsigned char *) &dhrbuf[xoff-0x2000];
        ptrmain = (unsigned char *) &dhrbuf[xoff];

        xoff = 0;
        for (idx = 0; idx < 40; idx++) {

            for (jdx = 0; jdx < 7; jdx++) {
                bits[jdx] = bmpscanline[xoff]; xoff++;
            }
            ptraux[idx] = (bits[6]<<6|bits[5]<<5|bits[4]<<4|
                           bits[3]<<3|bits[2]<<2|bits[1]<<1|bits[0]);

            for (jdx = 0; jdx < 7; jdx++) {
                bits[jdx] = bmpscanline[xoff]; xoff++;
            }
            ptrmain[idx] = (bits[6]<<6|bits[5]<<5|bits[4]<<4|
                            bits[3]<<3|bits[2]<<2|bits[1]<<1|bits[0]);

        }
        return SUCCESS;

}


/* routines for optional SHR grey-scaling using hsl */
/* by default, average RGB is used */

/*
http://www.vbaccelerator.com/home/VB/Code/vbMedia/Colour_Models/Hue__Luminance_and_Saturation/article.asp
*/

/* Creative Commons Section Start */
float Minimum(float rr, float rg, float rb)
{
    if (rr < rg) {
        if (rr < rb)return rr;
        return rb;
    }
    if (rb < rg) return rb;
    return rg;
}

float Maximum(float rr, float rg, float rb)
{
    if (rr > rg) {
        if (rr > rb)return rr;
        return rb;
    }
    if (rb > rg) return rb;
    return rg;
}

void rgb2hsl(uchar r,uchar g,uchar b,float *h,float *s,float *l)
{
    float rr,rg,rb,min,max,delta;

    rr = (float) r;
    rg = (float) g;
    rb = (float) b;

    rr /= 255;
    rg /= 255;
    rb /= 255;

    max = Maximum(rr, rg, rb);
    min = Minimum(rr, rg, rb);
    l[0] = (max + min) / 2;

    if (max == min) {
        /* achromatic - grey */
        s[0] = h[0] = (float)0.0;
    }
    else {
        /* chromatic - colors */
        if (l[0] <= 0.5) s[0] = (max - min) / (max + min);
        else s[0] = (max - min) / (2.0 - max - min);
        delta = max - min;
        if (rr == max) h[0] = (rg - rb) / delta;
        else if (rg == max) h[0] = 2.0 + (rb - rr) / delta;
        else if (rb == max) h[0] = 4.0 + (rr - rg) / delta;
    }
}

void hsl2rgb(float h,float s,float l,uchar *r,uchar *g,uchar *b)
{
    float rr,rg,rb,min,max;

    if (s == 0) {
        /* achromatic - greyscale */
        rr = rg = rb = l;
    }
    else {
        /* chromatic - colors */
        if (l <= 0.5) min = l * (1.0 - s);
        else min = l - s * (1.0 - l);
        max = 2.0 * l - min;

        if (h < 1) {
            rr = max;
            if (h < 0) {
                rg = min;
                rb = rg - h * (max - min);
            }
            else {
                rb = min;
                rg = h * (max - min) + rb;
            }
        }
        else if (h < 3) {
            rg = max;
            if (h < 2) {
                rb = min;
                rr = rb - (h - 2) * (max - min);
            }
            else {
                rr = min;
                rb = (h - 2.0) * (max - min) + rr;
            }
        }
        else {
            rb = max;
            if (h < 4) {
                rr = min;
                rg = rr - (h - 4) * (max - min);
            }
            else {
                rg = min;
                rr = (h - 4.0) * (max - min) + rg;
            }
        }
    }

    r[0] = (uchar)(float)(rr * 255);
    g[0] = (uchar)(float)(rg * 255);
    b[0] = (uchar)(float)(rb * 255);
}


/* Creative Commons Section End */

/* set luma to different values for closest color */
/* read this first - https://en.wikipedia.org/wiki/YCbCr */
/* also read https://en.wikipedia.org/wiki/Color_space */
/* also read https://en.wikipedia.org/wiki/RGB_color_space */

int lumaREQ = 601, lumaRED = 299, lumaGREEN = 587, lumaBLUE = 114;
double dlumaRED, dlumaGREEN, dlumaBLUE;

/* strip line feeds from ascii file lines... */
void nocr(char *ptr) {
  int idx;
  for (idx = 0; ptr[idx] != 0; idx++)
    if (ptr[idx] == LFEED || ptr[idx] == CRETURN || ptr[idx] == '#')
      ptr[idx] = 0;
}

void setluma()
{

	FILE *fp;
	char buf[128];
	double dred, dgreen, dblue;
	int status = -1;

	/* optional text file with 3-lines,
	   each with a luma coefficient for red,green and blue respectively */
	/* over-rides hard-coded luma values
	   this is for someone who really knows what they are doing */
	fp = fopen("luma.txt","r");
	if (NULL != fp) {
	   for (;;) {
		    /* read custom luma values */
	        if (NULL == fgets(buf, 128, fp)) break;
	        nocr(buf);
	        dred = atof(buf);
	        if (NULL == fgets(buf, 128, fp)) break;
	        nocr(buf);
	        dgreen = atof(buf);
	        if (NULL == fgets(buf, 128, fp)) break;
	        nocr(buf);
	        dblue = atof(buf);
	        status = -2;
	        /* generous range check */
	        if (dred > 0.999 || dred < 0.001) break;
	        if (dgreen > 0.999 || dgreen < 0.001) break;
	        if (dblue > 0.999 || dblue < 0.001) break;
	        /* set custom luma values */
	        dlumaRED = dred; lumaRED = (int)(dred * 1000);
	        dlumaGREEN = dgreen; lumaGREEN = (int)(dgreen * 1000);
	        dlumaBLUE = dblue; lumaBLUE = (int)(dblue * 1000);
	        status = SUCCESS;
	        break;
		}
		fclose(fp);
		if (status == -1) puts("luma.txt: read error!");
		else if (status == -2) puts("luma.txt: invalid coefficient!");
		else puts("luma.txt: external coefficients in effect!");
	}

	if (status == SUCCESS) return;

    switch(lumaREQ)
    {
        /* HDMI II - Rec. 2020 specifies that if a luma (Y') signal is made that it
        uses the R’G’B’ coefficients
        0.2627 for red, 0.6780 for green, and 0.0593 for blue */
        case 2020:
                  lumaRED = 263;     lumaGREEN = 678;    lumaBLUE = 59;
                  dlumaRED = 0.2627;  dlumaGREEN = 0.6780; dlumaBLUE = 0.0593;
                  break;

        case 240: /* SMPTE 240M transitional coefficients */
                  /* http://www.chromapure.com/colorscience-decoding.asp */
                  lumaRED = 212;     lumaGREEN = 701;    lumaBLUE = 87;
                  dlumaRED = 0.2124;  dlumaGREEN = 0.7011; dlumaBLUE = 0.0866;
                  break;

        case 911: /* Sheldon Simms - tohgr - probably not useful */
                  lumaRED = 77;    lumaGREEN = 151;    lumaBLUE = 28;
                  dlumaRED = 0.077;dlumaGREEN = 0.151; dlumaBLUE = 0.028;
                  break;

        case 411: /* The GIMP color managed */
                  /* https://mail.gnome.org/archives/gimp-user-list/2013-November/msg00173.html */
                  /* sRGB color managed */
                  /* http://ninedegreesbelow.com/photography/srgb-luminance.html */
                  lumaRED = 223;     lumaGREEN = 717;      lumaBLUE = 61;
                  dlumaRED = 0.2225; dlumaGREEN = 0.7169;  dlumaBLUE = 0.0606;
                  break;

        case 709: /* CCIR 709 - modern */
                  /* ImageMagick non-color managed */
                  /* also The GIMP 2.8 - http://fossies.org/dox/gimp-2.8.14/gimprgb_8h_source.html */
                  /* also PhotoShop
                     http://www.beneaththewaves.net/Photography/Secrets_of_Photoshops_Colour_Blend_Mode_Revealed_Sort_Of.html
                  */
                  lumaRED = 213;      lumaGREEN = 715;       lumaBLUE = 72;
                  dlumaRED = 0.212656;dlumaGREEN = 0.715158; dlumaBLUE = 0.072186;
                  break;

        case 601: /* CCIR 601 - most digital standard definition formats */
                  /* for monitors having phosphors that were contemporary
                     at the introduction of NTSC television in 1953.
                  /* these coefficients do not accurately compute luminance for contemporary monitors */

        default:  lumaRED = 299;     lumaGREEN = 587;      lumaBLUE = 114;
                  dlumaRED = 0.298839;  dlumaGREEN = 0.586811;   dlumaBLUE = 0.114350;
                  break;


    }
}


double rgbLuma[16], rgbDouble[16][3];
int brooksline = 999;

/* intialize the values for the current palette */
void InitDoubleArrays()
{
    int i;
    double dr, dg, db;

   /* array for matching closest color in palette */
    for (i=0;i<16;i++) {
        rgbDouble[i][0] = dr = (double) rgbArray[i][0];
        rgbDouble[i][1] = dg = (double) rgbArray[i][1];
        rgbDouble[i][2] = db = (double) rgbArray[i][2];
        rgbLuma[i] = (dr*lumaRED + dg*lumaGREEN + db*lumaBLUE) / (255.0*1000);
    }

}

/* for SHR output */
/* intialize the closest color palette for each scanline */
void InitDoubleLineArrays(int y)
{

    int i;
    double dr, dg, db;

    /* array for matching closest color in palette */
    for (i=0;i<16;i++) {
        rgbDouble[i][0] = dr = (double) rgbArrays[y][i][0];
        rgbDouble[i][1] = dg = (double) rgbArrays[y][i][1];
        rgbDouble[i][2] = db = (double) rgbArrays[y][i][2];
        rgbLuma[i] = (dr*lumaRED + dg*lumaGREEN + db*lumaBLUE) / (255.0*1000);
    }

    brooksline = y;
}

void InitDoubleLine256Arrays(int idx)
{

    int i;
    double dr, dg, db;

    /* array for matching closest color in palette */
    for (i=0;i<16;i++) {
        rgbDouble[i][0] = dr = (double) rgb256Arrays[idx][i][0];
        rgbDouble[i][1] = dg = (double) rgb256Arrays[idx][i][1];
        rgbDouble[i][2] = db = (double) rgb256Arrays[idx][i][2];
        rgbLuma[i] = (dr*lumaRED + dg*lumaGREEN + db*lumaBLUE) / (255.0*1000);
    }

    for (i=0,brooksline=0;i<200;i++) {
        if (idx == (int)mypic.scb[i]) {
            brooksline = i;
            break;
        }
    }
}



double globaldistance = 0.0, indexdistance = 0.0, brooksdistance = 0.0;

/* use CCIR 601 luminosity to get color distance value */
uchar GetColorDistance(uchar r, uchar g, uchar b, uchar idx)
{
    uchar drawcolor, i;
    double dr, dg, db, diffR, diffG, diffB, luma, lumadiff, distance, prevdistance;

    indexdistance = 0.0;

    /* use nearest color */
    dr = (double)r;
    dg = (double)g;
    db = (double)b;
    luma = (dr*lumaRED + dg*lumaGREEN + db*lumaBLUE) / (255.0*1000);
    lumadiff = rgbLuma[0]-luma;

    /* Compare the difference of RGB values, weigh by CCIR 601 luminosity */
    /* set palette index to color with shortest distance */

    /* get color distance to first palette color */
    diffR = (rgbDouble[0][0]-dr)/255.0;
    diffG = (rgbDouble[0][1]-dg)/255.0;
    diffB = (rgbDouble[0][2]-db)/255.0;

    prevdistance = (diffR*diffR*dlumaRED + diffG*diffG*dlumaGREEN + diffB*diffB*dlumaGREEN)*0.75
         + lumadiff*lumadiff;
    /* set palette index to first color */
    drawcolor = 0;

    /* get color distance to rest of palette colors */
    for (i=1;i<16;i++) {

        if (i<15) {
            if (i != idx) continue;
        }
        /* get color distance of this index */
        lumadiff = rgbLuma[i]-luma;
        diffR = (rgbDouble[i][0]-dr)/255.0;
        diffG = (rgbDouble[i][1]-dg)/255.0;
        diffB = (rgbDouble[i][2]-db)/255.0;
        distance = (diffR*diffR*dlumaRED + diffG*diffG*dlumaGREEN + diffB*diffB*dlumaGREEN)*0.75
            + lumadiff*lumadiff;

        /* if distance is smaller use this index */
        if (distance < prevdistance) {
           prevdistance = distance;
           drawcolor = (uchar)i;
        }

    }
    indexdistance = prevdistance;
    return drawcolor;
}


/* use CCIR 601 luminosity to get closest color in current palette */
/* based on palette that has been selected for conversion */
uchar GetClosestColor(uchar r, uchar g, uchar b)
{
    uchar drawcolor;
    double dr, dg, db, diffR, diffG, diffB, luma, lumadiff, distance, prevdistance;
    int i,j=brooksline;

    globaldistance = 0.0;

    /* look for exact match */
    for (i=0;i<16;i++) {
        if (brooksline == 999) {
            if (r == rgbArray[i][0] && g == rgbArray[i][1] && b == rgbArray[i][2]) return (uchar)i;
        }
        else {
            if (r == rgbArrays[j][i][0] && g == rgbArrays[j][i][1] && b == rgbArrays[j][i][2]) return (uchar)i;
        }
    }

    /* if no exact match use nearest color */
    dr = (double)r;
    dg = (double)g;
    db = (double)b;
    luma = (dr*lumaRED + dg*lumaGREEN + db*lumaBLUE) / (255.0*1000);
    lumadiff = rgbLuma[0]-luma;

    /* Compare the difference of RGB values, weigh by CCIR 601 luminosity */
    /* set palette index to color with shortest distance */

    /* get color distance to first palette color */
    diffR = (rgbDouble[0][0]-dr)/255.0;
    diffG = (rgbDouble[0][1]-dg)/255.0;
    diffB = (rgbDouble[0][2]-db)/255.0;

    prevdistance = (diffR*diffR*dlumaRED + diffG*diffG*dlumaGREEN + diffB*diffB*dlumaGREEN)*0.75
         + lumadiff*lumadiff;
    /* set palette index to first color */
    drawcolor = 0;

    /* get color distance to rest of palette colors */
    for (i=1;i<16;i++) {

        /* get color distance of this index */
        lumadiff = rgbLuma[i]-luma;
        diffR = (rgbDouble[i][0]-dr)/255.0;
        diffG = (rgbDouble[i][1]-dg)/255.0;
        diffB = (rgbDouble[i][2]-db)/255.0;
        distance = (diffR*diffR*dlumaRED + diffG*diffG*dlumaGREEN + diffB*diffB*dlumaBLUE)*0.75
            + lumadiff*lumadiff;

        /* if distance is smaller use this index */
        if (distance < prevdistance) {
           prevdistance = distance;
           drawcolor = (uchar)i;
        }

    }
    globaldistance = prevdistance;
    return drawcolor;
}


/* use CCIR 601 luminosity to get closest color in current palette */
/* based on palette that has been selected for conversion */
uchar GetClosest256Color(uchar r, uchar g, uchar b, int palno)
{
    uchar drawcolor;
    double dr, dg, db, diffR, diffG, diffB, luma, lumadiff, distance, prevdistance;
    int i,j=palno;

    globaldistance = 0.0;

    /* look for exact match */
    for (i=0;i<16;i++) {
        if (palno == 999) {
            if (r == rgbArray[i][0] && g == rgbArray[i][1] && b == rgbArray[i][2]) return (uchar)i;
        }
        else {
            if (r == rgb256Arrays[j][i][0] && g == rgb256Arrays[j][i][1] && b == rgb256Arrays[j][i][2]) return (uchar)i;
        }
    }

    /* if no exact match use nearest color */
    dr = (double)r;
    dg = (double)g;
    db = (double)b;
    luma = (dr*lumaRED + dg*lumaGREEN + db*lumaBLUE) / (255.0*1000);
    lumadiff = rgbLuma[0]-luma;

    /* Compare the difference of RGB values, weigh by CCIR 601 luminosity */
    /* set palette index to color with shortest distance */

    /* get color distance to first palette color */
    diffR = (rgbDouble[0][0]-dr)/255.0;
    diffG = (rgbDouble[0][1]-dg)/255.0;
    diffB = (rgbDouble[0][2]-db)/255.0;

    prevdistance = (diffR*diffR*dlumaRED + diffG*diffG*dlumaGREEN + diffB*diffB*dlumaGREEN)*0.75
         + lumadiff*lumadiff;

      /* set palette index to first color */
    drawcolor = 0;

    /* get color distance to rest of palette colors */
    for (i=1;i<16;i++) {

        /* get color distance of this index */
        lumadiff = rgbLuma[i]-luma;
        diffR = (rgbDouble[i][0]-dr)/255.0;
        diffG = (rgbDouble[i][1]-dg)/255.0;
        diffB = (rgbDouble[i][2]-db)/255.0;
        distance = (diffR*diffR*dlumaRED + diffG*diffG*dlumaGREEN + diffB*diffB*0.114)*0.75
            + lumadiff*lumadiff;

        /* if distance is smaller use this index */
        if (distance < prevdistance) {
           prevdistance = distance;
           drawcolor = (uchar)i;
        }

    }
    globaldistance = prevdistance;
    return drawcolor;
}


/* routines to save to Apple 2 Double Hires Format */
/* a double hi-res pixel can occur at any one of 7 positions */
/* in a 4 byte block which spans aux and main screen memory */
/* the horizontal resolution is 140 pixels */
void dhrplot(int x,int y,uchar drawcolor)
{
    int xoff, pattern;
    uchar *ptraux, *ptrmain;


    if (x < 0 || x > 139 || y < 0 || y > 192) return;


    pattern = (x%7);
    xoff = HB[y] + ((x/7) * 2);
    ptraux  = (uchar *) &dhrbuf[xoff-0x2000];
    ptrmain = (uchar *) &dhrbuf[xoff];


    switch(pattern)
    {
        /* left this here for reference

        uchar dhrpattern[7][4] = {
        0,0,0,0,
        0,0,0,1,
        1,1,1,1,
        1,1,2,2,
        2,2,2,2,
        2,3,3,3,
        3,3,3,3};
        */

        case 0: ptraux[0] &= 0x70;
                ptraux[0] |= dhrbytes[drawcolor][0] &0x0f;
                break;
        case 1: ptraux[0] &= 0x0f;
                ptraux[0] |= dhrbytes[drawcolor][0] & 0x70;
                ptrmain[0] &= 0x7e;
                ptrmain[0] |= dhrbytes[drawcolor][1] & 0x01;
                break;
        case 2: ptrmain[0] &= 0x61;
                ptrmain[0] |= dhrbytes[drawcolor][1] & 0x1e;
                break;
        case 3: ptrmain[0] &= 0x1f;
                ptrmain[0] |= dhrbytes[drawcolor][1] & 0x60;
                ptraux[1] &= 0x7c;
                ptraux[1] |= dhrbytes[drawcolor][2] & 0x03;
                break;
        case 4: ptraux[1] &= 0x43;
                ptraux[1] |= dhrbytes[drawcolor][2] & 0x3c;
                break;
        case 5: ptraux[1] &= 0x3f;
                ptraux[1] |= dhrbytes[drawcolor][2] & 0x40;
                ptrmain[1] &= 0x78;
                ptrmain[1] |= dhrbytes[drawcolor][3] & 0x07;
                break;
        case 6: ptrmain[1] &= 0x07;
                ptrmain[1] |= dhrbytes[drawcolor][3] & 0x78;
                break;
    }

}

/* monochrome DHGR - 560 x 192 */
unsigned char dhbmono[] = {0x7e,0x7d,0x7b,0x77,0x6f,0x5f,0x3f};
unsigned char dhwmono[] = {0x1,0x2,0x4,0x8,0x10,0x20,0x40};

void dhrmonoplot(int x, int y, uchar drawcolor)
{

    int xoff, pixel;
    uchar *ptr;

    if (x < 0 || x > 559 || y < 0 || y > 192) return;

    xoff = HB[y] + (x/14);
    pixel = (x%14);
    if (pixel > 6) {
        /* main memory */
        pixel -= 7;
        ptr = (uchar *) &dhrbuf[xoff];
    }
    else {
        /* auxiliary memory */
        ptr  = (uchar *) &dhrbuf[xoff-0x2000];
    }

    if (drawcolor != 0) {
        /* white */
        ptr[0] |= dhwmono[pixel]; /* inclusive OR */
    }
    else {
        /* black */
        ptr[0] &= dhbmono[pixel]; /* bitwise AND */
    }

}


sshort redDither[640],greenDither[640],blueDither[640];
/* seed values from previous line */
sshort redSeed[640],greenSeed[640],blueSeed[640];
sshort redSeed2[640],greenSeed2[640],blueSeed2[640];
/* save and restore values for SHR palette matching */
sshort redSave[640],greenSave[640], blueSave[640];

/* in this implementation color bleed is fixed for my dither */
/* it will be either full (8/8) like Floyd-Steinberg or reduced 20% (8/10) */
int bleed = 8;

/* setting clip to 0 increases the potential amount of retained error */
/* error is accumulated in a short integer and may be negative or positive */
uchar AdjustShortPixel(int clip,sshort *buf,sshort value)
{
    value = (sshort)(buf[0] + value);
    if (clip != 0) {
        if (value < 0) value = 0;
        else if (value > 255) value = 255;
    }
    buf[0] = value;
    if (clip == 0) {
        if (value < 0) value = 0;
        else if (value > 255) value = 255;
    }
    return (uchar) value;
}

/* this is set-up to handle image fragments as well as full-screen dithering */
/* nominal DHGR resolutions supported are:
   140 x 192 - 4-bit pixels - same as Bmp2DHR and tohgr DHGR color
   280 x 192 - 2-bit pixels - same as Bmp2DHR HGR monochrome (not sure about Outlaw Editor HGR color)
   560 x 192 - 1-bit pixels - same as Bmp2DHR DHGR monochrome and Outlaw Editor DHGR color

   This is somewhat confusing I admit.

   To make things even more confusing this also handles LGR and DLGR, and SHR PIC files
*/

void BuckelsDither(int y, int width, int pixels)
{

    sshort *colorptr, *seedptr, *seed2ptr, color_error, random_error, errbuf[6];
    sshort red, green, blue, red_error, green_error, blue_error;
    int i,x,x1,x2;
    int total_difference, total_error, total_used;
    uchar drawcolor,r,g,b,idx;

    /* bits per pixel */
    switch(pixels) {
        case 4: if (width > 280) pixels = 1;
                else if (width > 140) pixels = 2;
                break;
        case 2: if (width > 280) pixels = 1;
                break;
        default: pixels = 1;

    }

    for (x=0;x<width;x++) {

        red   = redDither[x];
        green = greenDither[x];
        blue  = blueDither[x];

        drawcolor = GetClosestColor((uchar)red,(uchar)green,(uchar)blue);

        if (brooks == 0) {
            r = rgbArray[drawcolor][0];
            g = rgbArray[drawcolor][1];
            b = rgbArray[drawcolor][2];
        }
        else {
            r = rgbArrays[y][drawcolor][0];
            g = rgbArrays[y][drawcolor][1];
            b = rgbArrays[y][drawcolor][2];
        }

        redDither[x]   = (int)r;
        greenDither[x] = (int)g;
        blueDither[x]  = (int)b;

        /* the error is linear in this implementation */
        /* - an integer is used so round-off of errors occurs
         - also clipping of the error occurs under some circumstances
         - no luminance consideration
         - no gamma correction
        */

        red_error   = red - r;
        green_error = green - g;
        blue_error  = blue - b;

        for (i=0;i<3;i++) {

            /* loop through all 3 RGB channels */
            switch(i) {
                case 0: colorptr = (sshort *)&redDither[0];
                        seedptr   = (sshort *)&redSeed[0];
                        seed2ptr  = (sshort *)&redSeed2[0];
                        color_error = red_error;
                        break;
                case 1: colorptr = (sshort *)&greenDither[0];
                        seedptr   = (sshort *)&greenSeed[0];
                        seed2ptr  = (sshort *)&greenSeed2[0];
                        color_error = green_error;
                        break;
                case 2: colorptr = (sshort *)&blueDither[0];
                        seedptr   = (sshort *)&blueSeed[0];
                        seed2ptr  = (sshort *)&blueSeed2[0];
                        color_error = blue_error;
                        break;
            }

            /* diffuse the error based on the dither */
            switch(dithertype)
            {

            case FLOYDSTEINBERG:
                /*
                    *   7
                3   5   1   (1/16)
                */

                /* if error summing is turned-on add the accumulated rounding error
                   to the next pixel */
                if (errorsum == 0) {
                    total_difference = 0;
                }
                else {
                    total_error = (color_error * 16) / bleed;
                    total_used =  (color_error * 3)/bleed;
                    total_used += (color_error * 5)/bleed;
                    total_used += (color_error * 1)/bleed;
                    total_used += (color_error * 7)/bleed;
                    total_difference = total_error - total_used;
                }


                /* finish this line */
                AdjustShortPixel(1,(sshort *)&colorptr[x+1],(sshort)((color_error * 7)/bleed)+total_difference);
                /* seed next line forward */
                if (x>0)AdjustShortPixel(0,(sshort *)&seedptr[x-1],(sshort)((color_error * 3)/bleed));
                AdjustShortPixel(0,(sshort *)&seedptr[x+1],(sshort)((color_error * 1)/bleed));
                AdjustShortPixel(0,(sshort *)&seedptr[x],(sshort)((color_error * 5)/bleed));
                break;

            case ATKINSON:
            case ATKINSON2:
                /*
                    *   1   1
                1   1   1
                    1           (1/8 - reduced bleed) or (1/6 full bleed even diffusion)

                */

                /* finish this line */
                AdjustShortPixel(1,(sshort *)&colorptr[x+1],(sshort)(color_error/bleed));
                AdjustShortPixel(1,(sshort *)&colorptr[x+2],(sshort)(color_error/bleed));

                /* seed next line forward */
                if (x>0)AdjustShortPixel(0,(sshort *)&seedptr[x-1],(sshort)(color_error/bleed));
                AdjustShortPixel(0,(sshort *)&seedptr[x],(sshort)(color_error/bleed));
                AdjustShortPixel(0,(sshort *)&seedptr[x+1],(sshort)(color_error/bleed));

                /* seed furthest line forward */
                AdjustShortPixel(0,(sshort *)&seed2ptr[x],(sshort)(color_error/bleed));
                break;

            default:

               /* buckels dither */
               /* uses the same weighting pattern as Atkinson but with full weighting */
               /*
                  * 2 1
                1 2 1
                  1          (1/8)

                */
                /* if error summing is turned-on add the accumulated rounding error
                   to the next pixel */
                /* random dither automatically turns error summing on in which case
                   rounding errors are placed at random within the atkinson pattern */
                errbuf[0] = errbuf[1] = errbuf[2] = errbuf[3] = errbuf[4] = errbuf[5] = 0;
                if (errorsum == 0) {
                    total_difference = 0;
                }
                else {
                    total_error = (color_error * 8) / bleed;
                    total_used =  (color_error * 2)/bleed;
                    total_used += (color_error * 2)/bleed;
                    total_used += (color_error /bleed);
                    total_used += (color_error /bleed);
                    total_used += (color_error /bleed);
                    total_used += (color_error /bleed);
                    total_difference = total_error - total_used;

                    if (randomdither == 0) {
                        /* next pixel */
                        errbuf[0] = total_difference;
                    }
                    else {
                        /* random pixel */
                        idx = RandomRange(6) - 1;
                        errbuf[idx] = total_difference;

                    }
                }

                /* finish this line */
                AdjustShortPixel(1,(sshort *)&colorptr[x+1],(sshort)((color_error*2)/bleed)+errbuf[0]);
                AdjustShortPixel(1,(sshort *)&colorptr[x+2],(sshort)(color_error/bleed)+errbuf[1]);
                /* seed next line forward */
                if (x>0)AdjustShortPixel(0,(sshort *)&seedptr[x-1],(sshort)(color_error/bleed)+errbuf[2]);
                AdjustShortPixel(0,(sshort *)&seedptr[x],(sshort)((color_error*2)/bleed)+errbuf[3]);
                AdjustShortPixel(0,(sshort *)&seedptr[x+1],(sshort)(color_error/bleed)+errbuf[4]);
                /* seed furthest line forward */
                AdjustShortPixel(0,(sshort *)&seed2ptr[x],(sshort)(color_error/bleed));
                break;
            }

        }
    }

   /* for DHGR */
   /* plot dithered scanline in DHGR buffer using selected conversion palette */
   /* this supports single-bit, double-bit and 4-bit per pixel dithering */
   /* effective nominal resolutions are 560 x 192, 280 x 192, and 140 x 192 */

   /* for LGR and DLGR, resolution goes-up to 320 x 200 providing an image
      that can be much larger than the screen and down to 1 x 1 for LGR and 2 x 1 for DLGR
      for sprites that can be much smaller than the screen */

   /* SHR output is also supported in 320 x 200 only */
   for (x=0,x1=0,x2=0;x<width;x++) {

        r = (uchar)redDither[x];
        g = (uchar)greenDither[x];
        b = (uchar)blueDither[x];

        idx = GetClosestColor(r,g,b);

        if (lores == 1) {
            /* LGR and DLGR use a 1:1 verbatim dithering only */
            /* SHR output is based on LGR and DLGR dithering so is verbatim also */
            setlopixel((uchar)idx,x,y);
            /* the code below is used only for DHGR dithering */
            continue;
        }

        if (outline == 1) {
            if (idx != 0) idx = 15;
        }

        for (i=0;i<pixels;i++) {
            drawcolor = dhrbits[idx][x1];
            if (x1 > 26) x1 = 0;
            else x1++;
            dhrmonoplot(x2,y,drawcolor);
            x2++;
        }

   }

}

void BrooksDither(int y, int width)
{

    sshort *colorptr, *seedptr, *seed2ptr, color_error, random_error, errbuf[6];
    sshort red, green, blue, red_error, green_error, blue_error, heavy_error;
    int i,x,x1,x2,y1,y2;
    int total_difference, total_error, total_used;
    uchar drawcolor,r,g,b,idx;
    double besttotal, thistotal;

   /* find the palette with the least total error */
   for (y1 = 0; y1 < 200; y1++) {
        thistotal = 0.0;
        InitDoubleLineArrays(y1);
        for (x=0;x<width;x++) {

            red   = redDither[x];
            green = greenDither[x];
            blue  = blueDither[x];

            drawcolor = GetClosestColor((uchar)red,(uchar)green,(uchar)blue);
            thistotal += globaldistance;
        }
        if (y1 == 0) {
            y2 = y1;
            besttotal = thistotal;
            continue;
        }

        if (thistotal < besttotal) {
            besttotal = thistotal;
            y2 = y1;
        }
    }

    /* set the palette with the least total error as the dithering palette */
    memcpy(&savepalettes[y][0][0],&rgbArrays[y2][0][0],48);
    InitDoubleLineArrays(y2);

    for (x=0;x<width;x++) {

        red   = redDither[x];
        green = greenDither[x];
        blue  = blueDither[x];

        drawcolor = GetClosestColor((uchar)red,(uchar)green,(uchar)blue);

        r = rgbArrays[y2][drawcolor][0];
        g = rgbArrays[y2][drawcolor][1];
        b = rgbArrays[y2][drawcolor][2];

        redDither[x]   = (int)r;
        greenDither[x] = (int)g;
        blueDither[x]  = (int)b;

        /* the error is linear in this implementation */
        /* - an integer is used so round-off of errors occurs
         - also clipping of the error occurs under some circumstances
         - no luminance consideration
         - no gamma correction
        */

        red_error   = red - r;
        green_error = green - g;
        blue_error  = blue - b;

        for (i=0;i<3;i++) {

            /* loop through all 3 RGB channels */
            switch(i) {
                case 0: colorptr = (sshort *)&redDither[0];
                        seedptr   = (sshort *)&redSeed[0];
                        seed2ptr  = (sshort *)&redSeed2[0];
                        color_error = red_error;
                        break;
                case 1: colorptr = (sshort *)&greenDither[0];
                        seedptr   = (sshort *)&greenSeed[0];
                        seed2ptr  = (sshort *)&greenSeed2[0];
                        color_error = green_error;
                        break;
                case 2: colorptr = (sshort *)&blueDither[0];
                        seedptr   = (sshort *)&blueSeed[0];
                        seed2ptr  = (sshort *)&blueSeed2[0];
                        color_error = blue_error;
                        break;
            }

            /* diffuse the error based on the dither */
            switch(dithertype)
            {

            case FLOYDSTEINBERG:
                /*
                    *   7
                3   5   1   (1/16)
                */

                /* if error summing is turned-on add the accumulated rounding error
                   to the next pixel */
                if (errorsum == 0) {
                    total_difference = 0;
                }
                else {
                    total_error = (color_error * 16) / bleed;
                    total_used =  (color_error * 3)/bleed;
                    total_used += (color_error * 5)/bleed;
                    total_used += (color_error * 1)/bleed;
                    total_used += (color_error * 7)/bleed;
                    total_difference = total_error - total_used;
                }


                /* finish this line */
                AdjustShortPixel(1,(sshort *)&colorptr[x+1],(sshort)((color_error * 7)/bleed)+total_difference);
                /* seed next line forward */
                if (x>0)AdjustShortPixel(0,(sshort *)&seedptr[x-1],(sshort)((color_error * 3)/bleed));
                AdjustShortPixel(0,(sshort *)&seedptr[x+1],(sshort)((color_error * 1)/bleed));
                AdjustShortPixel(0,(sshort *)&seedptr[x],(sshort)((color_error * 5)/bleed));
                break;

            case ATKINSON:
            case ATKINSON2:
                /*
                    *   1   1
                1   1   1
                    1           (1/8 - reduced bleed) or (1/6 full bleed even diffusion)

                */

                /* finish this line */
                AdjustShortPixel(1,(sshort *)&colorptr[x+1],(sshort)(color_error/bleed));
                AdjustShortPixel(1,(sshort *)&colorptr[x+2],(sshort)(color_error/bleed));

                /* seed next line forward */
                if (x>0)AdjustShortPixel(0,(sshort *)&seedptr[x-1],(sshort)(color_error/bleed));
                AdjustShortPixel(0,(sshort *)&seedptr[x],(sshort)(color_error/bleed));
                AdjustShortPixel(0,(sshort *)&seedptr[x+1],(sshort)(color_error/bleed));

                /* seed furthest line forward */
                AdjustShortPixel(0,(sshort *)&seed2ptr[x],(sshort)(color_error/bleed));
                break;

            default:

               /* buckels dither */
               /* uses the same weighting pattern as Atkinson but with full weighting */
               /*
                  * 2 1
                1 2 1
                  1          (1/8)

                */
                /* if error summing is turned-on add the accumulated rounding error
                   to the next pixel */
                /* random dither automatically turns error summing on in which case
                   rounding errors are placed at random within the atkinson pattern */
                errbuf[0] = errbuf[1] = errbuf[2] = errbuf[3] = errbuf[4] = errbuf[5] = 0;
                if (errorsum == 0) {
                    total_difference = 0;
                }
                else {
                    total_error = (color_error * 8) / bleed;
                    total_used =  (color_error * 2)/bleed;
                    total_used += (color_error * 2)/bleed;
                    total_used += (color_error /bleed);
                    total_used += (color_error /bleed);
                    total_used += (color_error /bleed);
                    total_used += (color_error /bleed);
                    total_difference = total_error - total_used;

                    if (randomdither == 0) {
                        /* next pixel */
                        errbuf[0] = total_difference;
                    }
                    else {
                        /* random pixel */
                        idx = RandomRange(6) - 1;
                        errbuf[idx] = total_difference;

                    }
                }

                /* finish this line */
                AdjustShortPixel(1,(sshort *)&colorptr[x+1],(sshort)((color_error*2)/bleed)+errbuf[0]);
                AdjustShortPixel(1,(sshort *)&colorptr[x+2],(sshort)(color_error/bleed)+errbuf[1]);
                /* seed next line forward */
                if (x>0)AdjustShortPixel(0,(sshort *)&seedptr[x-1],(sshort)(color_error/bleed)+errbuf[2]);
                AdjustShortPixel(0,(sshort *)&seedptr[x],(sshort)((color_error*2)/bleed)+errbuf[3]);
                AdjustShortPixel(0,(sshort *)&seedptr[x+1],(sshort)(color_error/bleed)+errbuf[4]);
                /* seed furthest line forward */
                AdjustShortPixel(0,(sshort *)&seed2ptr[x],(sshort)(color_error/bleed));

            }
        }
    }

   /* SHR output in 320 x 200 only */
   for (x=0,x1=0,x2=0;x<width;x++) {
        r = (uchar)redDither[x];
        g = (uchar)greenDither[x];
        b = (uchar)blueDither[x];
        idx = GetClosestColor(r,g,b);
        setlopixel((uchar)idx,x,y);
   }

}


void PicDither(int y, int width)
{

    sshort *colorptr, *seedptr, *seed2ptr, color_error, random_error, errbuf[6];
    sshort red, green, blue, red_error, green_error, blue_error;
    int i,x,x1,x2,y1,y2,saveline, maxpal = 16;
    int total_difference, total_error, total_used;
    uchar drawcolor,r,g,b,idx;
    double besttotal, thistotal;

    /* save original values */
    /* not used yet - may not be needed */
    /*
    memcpy(&redSave[0],&redDither[0],width * sizeof(sshort));
    memcpy(&greenSave[0],&greenDither[0],width * sizeof(sshort));
    memcpy(&blueSave[0],&blueDither[0],width * sizeof(sshort));
    */

   if (imnumpalettes == 8) maxpal = 8;
   else if (imnumpalettes == 1) maxpal = 1;

   /* find the palette with the least total error */
   for (y1 = 0; y1 < maxpal; y1++) {
        thistotal = 0.0;
        InitDoubleLine256Arrays(y1);
        for (x=0;x<width;x++) {

            red   = redDither[x];
            green = greenDither[x];
            blue  = blueDither[x];

            drawcolor = GetClosestColor((uchar)red,(uchar)green,(uchar)blue);
            thistotal += globaldistance;
        }
        if (y1 == 0) {
            y2 = y1;
            besttotal = thistotal;
            saveline = brooksline;
            continue;
        }

        if (thistotal < besttotal) {
            besttotal = thistotal;
            y2 = y1;
            saveline = brooksline;
        }
    }

    /* set the palette with the least total error as the dithering palette */
    memcpy(&savepalettes[y][0][0],&rgb256Arrays[y2][0][0],48);
    savescb[y] = y2;
    InitDoubleLineArrays(saveline);

    for (x=0;x<width;x++) {

        red   = redDither[x];
        green = greenDither[x];
        blue  = blueDither[x];

        drawcolor = GetClosestColor((uchar)red,(uchar)green,(uchar)blue);

        r = rgbArrays[saveline][drawcolor][0];
        g = rgbArrays[saveline][drawcolor][1];
        b = rgbArrays[saveline][drawcolor][2];

        redDither[x]   = (int)r;
        greenDither[x] = (int)g;
        blueDither[x]  = (int)b;

        /* the error is linear in this implementation */
        /* - an integer is used so round-off of errors occurs
         - also clipping of the error occurs under some circumstances
         - no luminance consideration
         - no gamma correction
        */

        red_error   = red - r;
        green_error = green - g;
        blue_error  = blue - b;

        for (i=0;i<3;i++) {

            /* loop through all 3 RGB channels */
            switch(i) {
                case 0: colorptr = (sshort *)&redDither[0];
                        seedptr   = (sshort *)&redSeed[0];
                        seed2ptr  = (sshort *)&redSeed2[0];
                        color_error = red_error;
                        break;
                case 1: colorptr = (sshort *)&greenDither[0];
                        seedptr   = (sshort *)&greenSeed[0];
                        seed2ptr  = (sshort *)&greenSeed2[0];
                        color_error = green_error;
                        break;
                case 2: colorptr = (sshort *)&blueDither[0];
                        seedptr   = (sshort *)&blueSeed[0];
                        seed2ptr  = (sshort *)&blueSeed2[0];
                        color_error = blue_error;
                        break;
            }


            /* diffuse the error based on the dither */
            switch(dithertype)
            {

            case FLOYDSTEINBERG:
                /*
                    *   7
                3   5   1   (1/16)
                */

                /* if error summing is turned-on add the accumulated rounding error
                   to the next pixel */
                if (errorsum == 0) {
                    total_difference = 0;
                }
                else {
                    total_error = (color_error * 16) / bleed;
                    total_used =  (color_error * 3)/bleed;
                    total_used += (color_error * 5)/bleed;
                    total_used += (color_error * 1)/bleed;
                    total_used += (color_error * 7)/bleed;
                    total_difference = total_error - total_used;
                }


                /* finish this line */
                AdjustShortPixel(1,(sshort *)&colorptr[x+1],(sshort)((color_error * 7)/bleed)+total_difference);
                /* seed next line forward */
                if (x>0)AdjustShortPixel(0,(sshort *)&seedptr[x-1],(sshort)((color_error * 3)/bleed));
                AdjustShortPixel(0,(sshort *)&seedptr[x+1],(sshort)((color_error * 1)/bleed));
                AdjustShortPixel(0,(sshort *)&seedptr[x],(sshort)((color_error * 5)/bleed));
                break;

            case ATKINSON:
            case ATKINSON2:
                /*
                    *   1   1
                1   1   1
                    1           (1/8 - reduced bleed) or (1/6 full bleed even diffusion)

                */

                /* finish this line */
                AdjustShortPixel(1,(sshort *)&colorptr[x+1],(sshort)(color_error/bleed));
                AdjustShortPixel(1,(sshort *)&colorptr[x+2],(sshort)(color_error/bleed));

                /* seed next line forward */
                if (x>0)AdjustShortPixel(0,(sshort *)&seedptr[x-1],(sshort)(color_error/bleed));
                AdjustShortPixel(0,(sshort *)&seedptr[x],(sshort)(color_error/bleed));
                AdjustShortPixel(0,(sshort *)&seedptr[x+1],(sshort)(color_error/bleed));

                /* seed furthest line forward */
                AdjustShortPixel(0,(sshort *)&seed2ptr[x],(sshort)(color_error/bleed));
                break;

            default:

               /* buckels dither */
               /* uses the same weighting pattern as Atkinson but with full weighting */
               /*
                  * 2 1
                1 2 1
                  1          (1/8)

                */
                /* if error summing is turned-on add the accumulated rounding error
                   to the next pixel */
                /* random dither automatically turns error summing on in which case
                   rounding errors are placed at random within the atkinson pattern */
                errbuf[0] = errbuf[1] = errbuf[2] = errbuf[3] = errbuf[4] = errbuf[5] = 0;
                if (errorsum == 0) {
                    total_difference = 0;
                }
                else {
                    total_error = (color_error * 8) / bleed;
                    total_used =  (color_error * 2)/bleed;
                    total_used += (color_error * 2)/bleed;
                    total_used += (color_error /bleed);
                    total_used += (color_error /bleed);
                    total_used += (color_error /bleed);
                    total_used += (color_error /bleed);
                    total_difference = total_error - total_used;

                    if (randomdither == 0) {
                        /* next pixel */
                        errbuf[0] = total_difference;
                    }
                    else {
                        /* random pixel */
                        idx = RandomRange(6) - 1;
                        errbuf[idx] = total_difference;

                    }
                }

                /* finish this line */
                AdjustShortPixel(1,(sshort *)&colorptr[x+1],(sshort)((color_error*2)/bleed)+errbuf[0]);
                AdjustShortPixel(1,(sshort *)&colorptr[x+2],(sshort)(color_error/bleed)+errbuf[1]);
                /* seed next line forward */
                if (x>0)AdjustShortPixel(0,(sshort *)&seedptr[x-1],(sshort)(color_error/bleed)+errbuf[2]);
                AdjustShortPixel(0,(sshort *)&seedptr[x],(sshort)((color_error*2)/bleed)+errbuf[3]);
                AdjustShortPixel(0,(sshort *)&seedptr[x+1],(sshort)(color_error/bleed)+errbuf[4]);
                /* seed furthest line forward */
                AdjustShortPixel(0,(sshort *)&seed2ptr[x],(sshort)(color_error/bleed));
            }
        }
    }

   /* SHR output in 320 x 200 only */
   for (x=0,x1=0,x2=0;x<width;x++) {
        r = (uchar)redDither[x];
        g = (uchar)greenDither[x];
        b = (uchar)blueDither[x];
        idx = GetClosestColor(r,g,b);
        setlopixel((uchar)idx,x,y);
   }

}


void BrooksPicDither(int y, int width)
{

    sshort *colorptr, *seedptr, *seed2ptr, color_error;
    sshort red, green, blue, red_error, green_error, blue_error;
    int i,x,x1,x2,y1,y2,savepalette,usepalettemethod;
    int total_difference, total_error, total_used;
    uchar drawcolor,r,g,b,idx;
    double besttotal, thistotal;
    double pictotal;

    /* save original values */
    /* not used yet - may not be needed */
    /*
    memcpy(&redSave[0],&redDither[0],width * sizeof(sshort));
    memcpy(&greenSave[0],&greenDither[0],width * sizeof(sshort));
    memcpy(&blueSave[0],&blueDither[0],width * sizeof(sshort));
    */

   /* find the palette with the least total error */
   for (y1 = 0; y1 < 16; y1++) {
        thistotal = 0.0;
        InitDoubleLine256Arrays(y1);
        for (x=0;x<width;x++) {

            red   = redDither[x];
            green = greenDither[x];
            blue  = blueDither[x];

            drawcolor = GetClosest256Color((uchar)red,(uchar)green,(uchar)blue,y1);
            thistotal += globaldistance;
        }
        if (y1 == 0) {
            savepalette = y2 = y1;
            pictotal = thistotal;
            continue;
        }

        if (thistotal < pictotal) {
            pictotal = thistotal;
            savepalette = y2 = y1;
        }
    }

    /* set the palette with the least total error as the initial dithering palette */
    memcpy(&savepalettes[y][0][0],&rgb256Arrays[y2][0][0],48);

   /* find the palette with the least total error */
   for (y1 = 0; y1 < 200; y1++) {
        thistotal = 0.0;
        InitDoubleLineArrays(y1);
        for (x=0;x<width;x++) {

            red   = redDither[x];
            green = greenDither[x];
            blue  = blueDither[x];

            drawcolor = GetClosestColor((uchar)red,(uchar)green,(uchar)blue);
            thistotal += globaldistance;
        }
        if (y1 == 0) {
            y2 = y1;
            besttotal = thistotal;
            continue;
        }

        if (thistotal < besttotal) {
            besttotal = thistotal;
            y2 = y1;
        }
    }

    /* set the palette with the least total error as the dithering palette */
    /* hopefully this decreases banding and doesn't increase banding */
    if (pictotal < besttotal) {
        /* if the palette for image sections is closer then use it */
        memcpy(&savepalettes[y][0][0],&rgb256Arrays[savepalette][0][0],48);
        InitDoubleLine256Arrays(savepalette);
        usepalettemethod = 1;
        if (quietmode == 0) printf("pictotal %f is greater than besttotal %f for line %d\n",pictotal,besttotal,y);
    }
    else {
        /* if the individual line palette is closer then use that instead */
        memcpy(&savepalettes[y][0][0],&rgbArrays[y2][0][0],48);
        InitDoubleLineArrays(y2);
        usepalettemethod = 0;
    }


    for (x=0;x<width;x++) {

        red   = redDither[x];
        green = greenDither[x];
        blue  = blueDither[x];

        if (usepalettemethod == 1) {
            drawcolor = GetClosest256Color((uchar)red,(uchar)green,(uchar)blue,savepalette);

            r = rgbArrays[savepalette][drawcolor][0];
            g = rgbArrays[savepalette][drawcolor][1];
            b = rgbArrays[savepalette][drawcolor][2];
        }
        else {
            drawcolor = GetClosestColor((uchar)red,(uchar)green,(uchar)blue);

            r = rgbArrays[y2][drawcolor][0];
            g = rgbArrays[y2][drawcolor][1];
            b = rgbArrays[y2][drawcolor][2];
        }

        redDither[x]   = (int)r;
        greenDither[x] = (int)g;
        blueDither[x]  = (int)b;

        /* the error is linear in this implementation */
        /* - an integer is used so round-off of errors occurs
         - also clipping of the error occurs under some circumstances
         - no luminance consideration
         - no gamma correction
        */

        red_error   = red - r;
        green_error = green - g;
        blue_error  = blue - b;

        for (i=0;i<3;i++) {

            /* loop through all 3 RGB channels */
            switch(i) {
                case 0: colorptr = (sshort *)&redDither[0];
                        seedptr   = (sshort *)&redSeed[0];
                        seed2ptr  = (sshort *)&redSeed2[0];
                        color_error = red_error;
                        break;
                case 1: colorptr = (sshort *)&greenDither[0];
                        seedptr   = (sshort *)&greenSeed[0];
                        seed2ptr  = (sshort *)&greenSeed2[0];
                        color_error = green_error;
                        break;
                case 2: colorptr = (sshort *)&blueDither[0];
                        seedptr   = (sshort *)&blueSeed[0];
                        seed2ptr  = (sshort *)&blueSeed2[0];
                        color_error = blue_error;
                        break;
            }

           /* buckels dither */
           /* uses the same weighting pattern as Atkinson but with full weighting */
           /*
              * 2 1
            1 2 1
              1          (1/8)

            */

            /* if error summing is turned-on add the accumulated rounding error
               to the next pixel */
            if (errorsum == 0) {
                total_difference = 0;
            }
            else {
                total_error = (color_error * 8) / bleed;
                total_used =  (color_error * 2)/bleed;
                total_used += (color_error * 2)/bleed;
                total_used += (color_error /bleed);
                total_used += (color_error /bleed);
                total_used += (color_error /bleed);
                total_used += (color_error /bleed);
                total_difference = total_error - total_used;
            }

            /* finish this line */
            AdjustShortPixel(1,(sshort *)&colorptr[x+1],(sshort)((color_error*2)/bleed)+total_difference);
            AdjustShortPixel(1,(sshort *)&colorptr[x+2],(sshort)(color_error/bleed));

            /* seed next line forward */
            if (x>0)AdjustShortPixel(0,(sshort *)&seedptr[x-1],(sshort)(color_error/bleed));
            AdjustShortPixel(0,(sshort *)&seedptr[x],(sshort)((color_error*2)/bleed));
            AdjustShortPixel(0,(sshort *)&seedptr[x+1],(sshort)(color_error/bleed));

            /* seed furthest line forward */
            AdjustShortPixel(0,(sshort *)&seed2ptr[x],(sshort)(color_error/bleed));

        }
    }

   /* SHR output in 320 x 200 only */
   for (x=0,x1=0,x2=0;x<width;x++) {
        r = (uchar)redDither[x];
        g = (uchar)greenDither[x];
        b = (uchar)blueDither[x];
        if (usepalettemethod == 1) {
            /* use the line palettes if they're better */
            idx = GetClosestColor(r,g,b);
        }
        else {
            /* use the section palettes if they're better */
            idx = GetClosest256Color(r,g,b,savepalette);
        }
        setlopixel((uchar)idx,x,y);
   }

}

/* save raster oriented DHGR image fragment

   file format is 5 byte header

   3 - upper case ID bytes 'D' 'H' 'R'

   1 byte - width in bytes (multiples of 4 bytes - 7 pixels)
   1 byte - height in rasters

   followed by interleaved raster data

   aux raster, main raster = (width in bytes)
   aux raster, main raster = (width in bytes)
   aux raster, main raster = (width in bytes)
   etc...

*/

/* this version of savesprite is different from Bmp2DHR's and this
   program's BMP image fragment output which both work at the pixel level for x origin.

   this version of savesprite takes its orgin from the beginning of a
   7 pixel (4 byte) block and pads with trailing pixels to the end of 4 pixel block.

   Bmp2DHR pads sprites with trailing space in a background color if the input
   BMP is not as wide as the dhgr 7 pixel (4 byte) block used by DHGR. This program
   outputs BMP image fragments more or less the same way. If the apple II input file
   is already a DHR fragment, whatever trailing color it has will be in the BMP. But if
   all we are doing is "ripping" a fragment out of a full-screen file, the BMP will
   be at the pixel level.


 */
int savesprite(char *spritefile)
{

    FILE *fp;
    int i, c, width, packet, x, y, x1, y1, xoff;
    ushort fl;
    uchar *ptraux, *ptrmain, ch;

    /* start at the beginning of a 7-pixel block (based on a color image fragment) */
    while (fragx%7 != 0) fragx--;

    /* color image fragments must finish at the end of a 7-pixel block */
    /* for monochrome image fragments this is a 28-pixel block */
    /* while there is no technical reason why a monochrome fragment can't be loaded
       on single byte boundaries, I didn't develop A2B to work this way and I also
       didn't give monochrome much thought when I developed the DHM masking file variation
       in Bmp2DHR. nor did I write my cc65 loader to handle a monochrome sprite
       so I am just sticking with the color sprite format for now */
    while (fragwidth%7 != 0) fragwidth++;
    x1 = fragx + fragwidth;
    y1 = fragy + fragheight;

    if (x1 > 140) {
        /* cursory check */
        printf("Width %d is out of range for %s!\n",fragwidth, spritefile);
        return INVALID;
    }

    if (y1 > 192) {
        /* cursory check */
        printf("Height %d is out of range for %s!\n",fragheight, spritefile);
        return INVALID;
    }

    width = (int)((fragwidth / 7) * 4); /* 4 bytes = 7 pixels */
    packet = (int)width / 2;

    fp = fopen(spritefile,"wb");
    if (NULL == fp) {
        printf("Error Opening %s for writing!\n",spritefile);
        return INVALID;
    }

    /* 5 byte header */
    /* some kind of identifier */
    fputc('D',fp);
    fputc('H',fp);
    fputc('R',fp);

    fputc((uchar)width,fp);      /* width in bytes */
    fputc((uchar)fragheight,fp); /* height in scanlines */

    width = (int)((fragx / 7) * 4);     /* 4 bytes = 7 pixels */
    x1 = (int)width / 2; /* starting byte in DHGR buffer */
    y1 = (int)fragy;     /* starting scanline in DHGR buffer */

    for (y = 0; y < fragheight; y++,y1++) {
        xoff = HB[y1] + x1;
        ptraux  = (uchar *) &dhrbuf[xoff-0x2000];
        ptrmain = (uchar *) &dhrbuf[xoff];
        /* aux raster */
        c = fwrite((char *)&ptraux[0],1,packet,fp);
        if (c!= packet) break;
        /* main raster */
        c = fwrite((char *)&ptrmain[0],1,packet,fp);
        if (c!= packet) break;

    }
    fclose(fp);

    y1 = fragy + fragheight -1;
    x1 = fragx + fragwidth;
    if (vbmp == 1) {
        /* for preview (aka VBMP output) black-out the area around the fragment */
        for (y = 0;y < 192; y++) {
            if (y < fragy || y > y1) {
                /* blank whole line above and below fragment */
                for (x = 0; x < 140; x++) dhrplot(x,y,0);
                continue;
            }
            /* blank left pillar before fragment */
            for (x = 0; x < fragx; x++) dhrplot(x,y,0);
            /* blank right pillar after fragment */
            for (x = x1; x < 140; x++) dhrplot(x,y,0);
        }

    }

    if (c!=packet) {
        remove(spritefile);
        printf("Error Writing %s!\n",spritefile);
        return INVALID;
    }

    printf("%s created!\n",spritefile);
    return SUCCESS;
}


/* from Sheldon Simms tohgr */
uchar gsColor (uchar c)
{
    /*
    c &= 0xF0;
    c |= (c >> 4);
    return c;
*/
    /**/
    if (c > 0xF6) return 0xFF;
    if (c > 0xE5) return 0xEE;
    if (c > 0xD4) return 0xDD;
    if (c > 0xC3) return 0xCC;
    if (c > 0xB2) return 0xBB;
    if (c > 0xA1) return 0xAA;
    if (c > 0x90) return 0x99;
    if (c > 0x7F) return 0x88;
    if (c > 0x6E) return 0x77;
    if (c > 0x5D) return 0x66;
    if (c > 0x4C) return 0x55;
    if (c > 0x3B) return 0x44;
    if (c > 0x2A) return 0x33;
    if (c > 0x19) return 0x22;
    if (c > 0x08) return 0x11;
    return 0;
    /**/
}

/* based on the above */
uchar LegalColor[16] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};

/* promote monochrome bmp lines to 24-bit bmp lines */
void ReformatMonoLine(ushort packet)
{
     int i,j,k;
     uchar b = 0, w = 255;

     memcpy(&bmpscanline2[0],&bmpscanline[0],packet);

     for(i=0,j=0;i<packet;i++)
     {
        for(k=0;k<8;k++)
        {
            if (bmpscanline2[i]&msk[k]) {
                bmpscanline[j] = bmpscanline[j+1] = bmpscanline[j+2] = w;
            }
            else {
                bmpscanline[j] = bmpscanline[j+1] = bmpscanline[j+2] = b;
            }
            j+=3;
        }
     }
}

/* promote 16 color and 256 color bmp lines to 24-bit bmp lines */
void ReformatVGALine()
{
    sshort i, j, packet;
    uchar ch;

    memset(&bmpscanline2[0],0,1680);
    if (bmi.biBitCount == 8) {
       memcpy(&bmpscanline2[0],&bmpscanline[0],bmi.biWidth);
    }
    else {
        packet = bmi.biWidth /2;
        if (bmi.biWidth%2 != 0) packet++;
        for (i=0,j=0;i<packet;i++) {
            ch = bmpscanline[i] >> 4;
            bmpscanline2[j] = ch; j++;
            ch = bmpscanline[i] & 0xf;
            bmpscanline2[j] = ch; j++;
        }
    }
    memset(&bmpscanline[0],0,1680);
    for (i=0,j=0;i<bmi.biWidth;i++) {
          ch = bmpscanline2[i];
          bmpscanline[j] = sbmp[ch].rgbBlue; j++;
          bmpscanline[j] = sbmp[ch].rgbGreen; j++;
          bmpscanline[j] = sbmp[ch].rgbRed; j++;
     }
}

uchar rgbColorCount[16][3];
sshort rgbColorCounter = 0;

void ClearPalette16()
{

    if (shr == 320) {
        /* clear the memory for the shr trailer buffer */
        memset((char *)&mypic.scb[0],0,sizeof(PICFILE));

        shrmode = 4096;
        if (bmi.biBitCount == 8) shrmode = 256;
        if (bmi.biBitCount == 1 || mono == 1) {
            shrmode = 2;
            /* disable brooks if set */
            brooks = 0;
        }
        if (bmi.biBitCount == 4 || shrgrey == 1) {
            shrmode = 16;
            /* disable brooks if set */
            brooks = 0;
        }
    }
    else {
        /* disable brooks if set */
        brooks = 0;
    }

    if (shr == 0 || mono == 1 || bmi.biBitCount == 4 || bmi.biBitCount == 1) {
        rgbColorCounter = -1;
    }
    else {
        rgbColorCounter = 0;
        memset(&rgbColorCount[0][0],0,48);
    }
}

int shr2=0;

void SetPalette16()
{
    sshort i, count = rgbColorCounter;
    /* if not setting a 16 color conversion palette for shr conversion return immediately */
    /* if the line has more than 16 colors return immediately */
    if (count == -1)  {
        return;
    }
    shrmode = 16;
    /* disable brooks if set */
    brooks = shr2 = 0;
    memset(&rgbArray[0][0],0,48);
    for (i = 0; i < count; i++) {
        rgbArray[i][0] = rgbColorCount[i][0];
        rgbArray[i][1] = rgbColorCount[i][1];
        rgbArray[i][2] = rgbColorCount[i][2];
    }
}

sshort BuildPalette16(uchar r, uchar g, uchar b)
{
    sshort i, count = rgbColorCounter;

    /* if not building a 16 color conversion palette for shr conversion return immediately */
    /* if the line has more than 16 colors return immediately */
    if (count == -1) return -1;

    /* if the color is already in the palette
       return the index */
    for (i = 0; i < count; i++) {
        if (r == rgbColorCount[i][0] &&
            g == rgbColorCount[i][1] &&
            b == rgbColorCount[i][2]) return i;
    }

    /* if we are out of colors then fail */
    if (count > 15) {
        rgbColorCounter = -1;
        return -1;
    }

     /* add the color to the palette
       since it wasn't already in there */
    rgbColorCount[count][0] = r;
    rgbColorCount[count][1] = g;
    rgbColorCount[count][2] = b;

    /* increment the palette count to the next
        available color */
    i = count;
    count++;
    rgbColorCounter = count;

    /* return the color index for the entry
       that was just added */
    return i;
}


/* convert 16 color and 256 color bmps to 24 bit bmps */
/* convert Monochrome bmps to 24 bit bmps */
/* convert 24 bit bmps using IIgs color thresholds from tohgr */
FILE *ReformatBMP(FILE *fp)
{

    FILE *fp2;
    ushort packet,outpacket,y,x,x1;
    sshort count = 0;

    /* 16 color palette for verbatim SHR conversion */
    ClearPalette16();

    /* seek past extraneous info in header if any */
    fseek(fp,BitMapFileHeader.bfOffBits,SEEK_SET);

    /* align on 4 byte boundaries */
    if (bmi.biBitCount == 1) {
        packet = bmi.biWidth / 8;
        if (bmi.biWidth%8 != 0)packet++;
    }
    else if (bmi.biBitCount == 4) {
        packet = bmi.biWidth / 2;
        if (bmi.biWidth%2 != 0)packet++;
    }
    else if (bmi.biBitCount == 8) {
        packet = bmi.biWidth;
    }
    else {
        packet = bmi.biWidth * 3;
    }
    while ((packet % 4)!=0)packet++;

    if((fp2=fopen("Reformat.bmp","wb"))==NULL) {
        printf("Error Opening %s for writing!\n","Reformat.bmp");
        fclose(fp);
        return fp;
    }

    outpacket = WriteDIBHeader(fp2,(ushort)bmi.biWidth,(ushort)bmi.biHeight);

    if (outpacket < 1) {
        fclose(fp2);
        remove("Reformat.bmp");
        printf("Error writing header to %s!\n","Reformat.bmp");
        return fp;
    }

    /* 16 color palette for verbatim SHR conversion of 8 bit and 24 bit BMPs if only 16 colors */
    count = rgbColorCounter;

    for (y=0;y<bmi.biHeight;y++) {
        fread((char *)&bmpscanline[0],1,packet,fp);

        if (bmi.biBitCount == 1) {
            ReformatMonoLine(packet);
        }
        else if (bmi.biBitCount == 4 || bmi.biBitCount == 8) {
            ReformatVGALine();
        }
        else if (usegscolors == 1) {
            /* apply IIgs threshold colors from tohgr to 24-bit scanline */
            /* this is not required for lower color depths because the palette colors
               have already been adjusted */
            for (x = 0; x < packet; x++) bmpscanline[x] = gsColor(bmpscanline[x]);
        }

        if (count != -1) {
            /* 16 color palette for verbatim SHR conversion of 8 bit and 24 bit BMPs if only 16 colors */
            /* line color count could be here as well */
            for (x = 0,x1 = 0; x < bmi.biWidth; x++,x1+=3) {
                count = BuildPalette16(bmpscanline[x1+2],bmpscanline[x1+1],bmpscanline[x1]);
                if (count == -1) break;
            }
        }
        fwrite((char *)&bmpscanline[0],1,outpacket,fp2);
    }
    fclose(fp2);
    fclose(fp);

    if((fp=fopen("Reformat.bmp","rb"))==NULL) {
        printf("Error Opening %s for reading!\n","Reformat.bmp");
        return fp;
    }
    /* read the header stuff into the appropriate structures */
    fread((char *)&BitMapFileHeader.bfType[0],
                 sizeof(BITMAPFILEHEADER),1,fp);
    fread((char *)&bmi.biSize,
                 sizeof(BITMAPINFOHEADER),1,fp);

    /* 16 color palette for verbatim SHR conversion of 8 bit and 24 bit BMPs if only 16 colors */
    SetPalette16();
    return fp;
}



int ReadHybrid(unsigned char *basename, unsigned char *newname)
{

    FILE *fp;
    int packet = INVALID, y,y1,x,x1,x2,j,k,width,height,pixels,bmpversion;
    int reformat = bmp3;
    float hue,saturation,luminance;

    char bmpfile[256], outfile[256];
    uchar r,g,b,drawcolor,idx;
    ushort temp;

    sprintf(bmpfile,"%s.bmp",basename);

    if((fp=fopen(bmpfile,"rb"))==NULL) return INVALID;

    /* read the header stuff into the appropriate structures,
       it's likely a bmp file */
    memset(&BitMapFileHeader.bfType,0,sizeof(BITMAPFILEHEADER));
    memset(&bmi.biSize,0,sizeof(BITMAPINFOHEADER));

    fread((char *)&BitMapFileHeader.bfType,
                 sizeof(BITMAPFILEHEADER),1,fp);
    fread((char *)&bmi.biSize,
                 sizeof(BITMAPINFOHEADER),1,fp);


    if (bmi.biSize != sizeof(BITMAPINFOHEADER)) {
		if (bmi.biSize == sizeof(BITMAPINFOHEADERV2)|| bmi.biSize == sizeof(BITMAPINFOHEADERV3)) {
            memset(&bmiV5.biSize,0,sizeof(BITMAPINFOHEADERV5));
            fseek(fp,sizeof(BITMAPFILEHEADER),SEEK_SET);
            fread((char *)&bmiV5.biSize,(unsigned)bmi.biSize,1,fp);
            bmpversion = 3;
            reformat = 1;
		}
		else if (bmi.biSize == sizeof(BITMAPINFOHEADERV4)) {
            memset(&bmiV5.biSize,0,sizeof(BITMAPINFOHEADERV5));
            fseek(fp,sizeof(BITMAPFILEHEADER),SEEK_SET);
            fread((char *)&bmiV5.biSize,sizeof(BITMAPINFOHEADERV4),1,fp);
            bmpversion = 4;
            reformat = 1;
        }
        else if (bmi.biSize == sizeof(BITMAPINFOHEADERV5)) {
            /* https://msdn.microsoft.com/en-us/library/windows/desktop/dd183386%28v=vs.85%29.aspx */
            memset(&bmiV5.biSize,0,sizeof(BITMAPINFOHEADERV5));
            fseek(fp,sizeof(BITMAPFILEHEADER),SEEK_SET);
            fread((char *)&bmiV5.biSize,sizeof(BITMAPINFOHEADERV5),1,fp);
            bmpversion = 5;
            /*
            Profile data refers to either the profile file name (linked profile)
            or the actual profile bits (embedded profile).
            The file format places the profile data at the end of the file.
            The profile data is placed just after the color table (if present).
            */
            reformat = 1;
        }
        else {
            /* perhaps some unknown version that I don't support */
            /* whether I support the other versions properly is also an interesting question */
            bmpversion = 0;
        }

    }
    else {
        bmpversion = 3;
    }

    if (bmpversion == 0) {
        fclose(fp);
        printf("BMP version of %s not recognized!\n",bmpfile);
        return SUCCESS;
    }

    if (bmi.biCompression==BI_RGB &&
        BitMapFileHeader.bfType[0] == 'B' && BitMapFileHeader.bfType[1] == 'M' &&
        bmi.biPlanes==1 && (bmi.biBitCount == 24 || bmi.biBitCount == 8 || bmi.biBitCount == 4 || bmi.biBitCount == 1)) {

            height = (int) bmi.biHeight;
            width = (int) bmi.biWidth;

            switch(width) {
                case 560: if (height == 192 || height == 384) {
                            packet = 1680;
                            pixels = 1;
                          }
                          break;
                case 280: if (height != 192) break;
                          packet = 840;
                          pixels = 2;
                          break;

                case 140: if (height != 192) break;
                          packet = 420;
                          pixels = 4;
                          break;

            }

            if (packet == INVALID) {
                fclose(fp);
                puts(szTextTitle);
                printf("%s is not in a supported size for DHGR file output.\nExiting!", bmpfile);
                return SUCCESS;
            }
    }

    if (packet == INVALID) {
        fclose(fp);
        printf("%s is not in a supported format for DHGR file output.\nExiting!", bmpfile);
        return SUCCESS;
    }

    if (bmi.biBitCount == 8 || bmi.biBitCount == 4 || bmi.biBitCount == 1) {

       memset((char *)&sbmp[0].rgbBlue,0,sizeof(RGBQUAD)*256);

       if (bmi.biBitCount == 1) fread((char *)&sbmp[0].rgbBlue, sizeof(RGBQUAD)*2,1,fp);
       else if (bmi.biBitCount == 4) fread((char *)&sbmp[0].rgbBlue, sizeof(RGBQUAD)*16,1,fp);
       else fread((char *)&sbmp[0].rgbBlue, sizeof(RGBQUAD)*256,1,fp);


       if (bmi.biBitCount == 8 && width == 560 && height == 384) {
            /* Legacy AppleWin screen-capture support for backward compatibility */
            if (sbmp[17].rgbRed == 255 && sbmp[17].rgbGreen == 255 && sbmp[17].rgbBlue == 254) {
                fclose(fp);
                /* pass-through to old routine */
                return INVALID;
            }
       }

        fp = ReformatBMP(fp);
        if (NULL == fp) return SUCCESS;
        reformat = 1;
    }

    if (mono == 1) {
        /* create a black and white palette */
        memset(&rgbArray[0][0],0,45);
        memset(&rgbArray[15][0],255,3);
        if (hsl == 1) {
            puts("achromatic hsl monochrome using hue channel de-saturation.");
        }
        else {
            puts("average rgb monochrome using color channel scaling.");
        }
    }

    /* initialize nearest color arrays */
    InitDoubleArrays();

    memset(&dhrbuf[0],0,16384); /* clear write buffer */

    if (dither != 0) {
        /* sizeof(sshort) * 640 */
        memset(&redDither[0],0,1280);
        memset(&greenDither[0],0,1280);
        memset(&blueDither[0],0,1280);
        memset(&redSeed[0],0,1280);
        memset(&greenSeed[0],0,1280);
        memset(&blueSeed[0],0,1280);
        memset(&redSeed2[0],0,1280);
        memset(&greenSeed2[0],0,1280);
        memset(&blueSeed2[0],0,1280);
    }

    fseek(fp,BitMapFileHeader.bfOffBits,SEEK_SET);

    if (dither == 0) puts("non-dithered output");
    else puts("dithered output");

    /* bmp's are upside-down so conversion of scanlines is in
       reverse order */
    for(y=0,y1=191;y<192;y++,y1--)
    {
          fread((char *)&bmpscanline[0],1,packet,fp);

          /* unconditional merging */
          if (height == 384) {
              fread((char *)&bmpscanline2[0],1,packet,fp);
              for (j = 0; j < packet; j++) {
                 temp = (ushort)bmpscanline[j];
                 temp += bmpscanline2[j];
                 if ((temp % 2)!=0) temp++;
                 bmpscanline[j] = (uchar) (temp/2);
              }

          }

          if (mono == 1) {
            /* work from a greyscale if mono */
            for (j = 0;j < width;j+=3) {
                if (hsl == 1) {
                    /* achromatic greyscale */
                    /* de-saturate using the hsl color model */
                    rgb2hsl(bmpscanline[j+2],bmpscanline[j+1],bmpscanline[j],&hue,&saturation,&luminance);
                    bmpscanline[j] = bmpscanline[j+1] = bmpscanline[j+2] =(uchar)(float)(luminance * 255);
                }
                else {
                    /* average RGB greyscale */
                    temp = (ushort)bmpscanline[j];
                    temp += bmpscanline[j+1];
                    temp += bmpscanline[j+2];
                    while ((temp % 3)!=0) temp++;
                    bmpscanline[j] = bmpscanline[j+1] = bmpscanline[j+2] = (uchar) (temp/3);
                }
            }
          }

          if (dither == 0) {
            /* if not dithering use direct pixel mapping */
            /* this is especially useful when a pixel graphics image has been hand-built and
               requires precise positioning */
            j = 0;
            for (x=0,x1=0,x2=0;x<width;x++) {

                b = bmpscanline[j]; j++;
                g = bmpscanline[j]; j++;
                r = bmpscanline[j]; j++;

                idx = GetClosestColor(r,g,b);

                if (outline == 1) {
                    if (idx != 0) idx = 15;
                }

                for (k=0;k<pixels;k++) {
                    drawcolor = dhrbits[idx][x1];
                    if (x1 > 26) x1 = 0;
                    else x1++;
                    dhrmonoplot(x2,y1,drawcolor);
                    x2++;
                }
            }
          }
          else
          {
              /* add the current line to the r,g,b line buffers used for dithering */
              for (x=0, x1 = 0; x1 < width; x1++) {
                    b = bmpscanline[x]; x++;
                    g = bmpscanline[x]; x++;
                    r = bmpscanline[x]; x++;

                    /* values are already seeded from previous 2 - line(s) */
                    AdjustShortPixel(1,(sshort *)&redDither[x1],(sshort)r);
                    AdjustShortPixel(1,(sshort *)&greenDither[x1],(sshort)g);
                    AdjustShortPixel(1,(sshort *)&blueDither[x1],(sshort)b);
              }

               /* dithering */
               BuckelsDither(y1,width,pixels);

               /* seed next line - promote nearest forward array to
                  current line */
               memcpy(&redDither[0],&redSeed[0],1280);
               memcpy(&greenDither[0],&greenSeed[0],1280);
               memcpy(&blueDither[0],&blueSeed[0],1280);

               /* seed first seed - promote furthest forward array
                  to nearest forward array */
               memcpy(&redSeed[0],&redSeed2[0],1280);
               memcpy(&greenSeed[0],&greenSeed2[0],1280);
               memcpy(&blueSeed[0],&blueSeed2[0],1280);

               /* clear last seed - furthest forward array */
               memset(&redSeed2[0],0,1280);
               memset(&greenSeed2[0],0,1280);
               memset(&blueSeed2[0],0,1280);
         }

    }
    fclose(fp);

    if (reformat == 1) {
		if (bmp3 == 0) {
			remove("Reformat.bmp");
		}
		else {
			sprintf(outfile,"%s.bm3", newname);
			remove(outfile);
			rename("Reformat.bmp",outfile);
		}
	}

    /* keep output files to a minimum */
    /* output Apple II files in one of 3 formats */
    if (frag == 1) {
        /* raster oriented top-down */
        /* for C programs */
        sprintf(outfile,"%s.DHR",newname);
        ucase((char *)&outfile[0]);
        savesprite(outfile);
    }
    else {
        if (applesoft == 0) {
            if (longnames == 0)sprintf(outfile,"%s.2FC", newname);
            else {
                if (mono == 1)
                    sprintf(outfile,"%s.A2FM", newname);
                else
                    sprintf(outfile,"%s.A2FC", newname);
            }
            ucase((char *)&outfile[0]);
            fp = fopen(outfile,"wb");
            if (NULL == fp) {
                puts(szTextTitle);
                printf("%s cannot be created.\n", outfile);
                return SUCCESS;
            }
            fwrite(dhrbuf,1,16384,fp);
            fclose(fp);
            printf("%s created.\n", outfile);
        }
        else {
            /* output AUX,BIN - default is A2FC */
            /* the bsaved images are split into two files
            the first file is loaded into aux mem */
            sprintf(outfile,"%s.AUX",newname);
            ucase((char *)&outfile[0]);
            fp = fopen(outfile,"wb");
            if (NULL == fp) {
                puts(szTextTitle);
                printf("%s cannot be created.\n", outfile);
                return SUCCESS;
            }
            fwrite(dhrbuf,1,8192,fp);
            fclose(fp);
            printf("%s created.\n", outfile);

            /* the second file is loaded into main mem */
            sprintf(outfile,"%s.BIN",newname);
            ucase((char *)&outfile[0]);
            fp = fopen(outfile,"wb");
            if (NULL == fp) {
                puts(szTextTitle);
                printf("%s cannot be created.\n", outfile);
                sprintf(outfile,"%s.AUX",newname);
                remove(outfile);
                printf("removed %s.\n", outfile);
                return SUCCESS;
            }
            fwrite(&dhrbuf[8192],1,8192,fp);
            fclose(fp);
            printf("%s created.\n", outfile);

        }
    }

    if (vbmp == 1) {
        if (longnames == 0)sprintf(outfile,"%s.vmp", newname);
        else sprintf(outfile,"%s_VBMP.bmp", newname);
        if (WriteVBMPFile(outfile) == SUCCESS) printf("%s created.\n", outfile);
    }

return SUCCESS;

}

/* Input File Helper Function - called from main() */
/* Reads an AppleWin Monochrome Screen Capture (256 color BMP) */
/* Writes an Apple II Compatible DHGR A2FC file (or optionally AUX, BIN file pair) */
/* Renames AppleWin Screen Capture from BMP to BM2 */
/* The Apple II Compatible Output is then processed normally. */
int ReadAppleWin(unsigned char *basename, unsigned char *newname)
{

    FILE *fp;
    int packet = INVALID, y,i,j;
    char bmpfile[256], outfile[256];

    if (bm2 == 1) sprintf(bmpfile,"%s.bm2",basename);
    else sprintf(bmpfile,"%s.bmp",basename);

    if((fp=fopen(bmpfile,"rb"))==NULL) {
        if (bm2 == 1) return INVALID;
        sprintf(bmpfile,"%s.bm2",basename);
        if((fp=fopen(bmpfile,"rb"))==NULL) {
            return INVALID;
        }
        bm2 = 1;
    }

    /* read the header stuff into the appropriate structures,
       it's likely a bmp file */
    memset(&BitMapFileHeader.bfType,0,sizeof(BITMAPFILEHEADER));
    memset(&bmi.biSize,0,sizeof(BITMAPINFOHEADER));

    fread((char *)&BitMapFileHeader.bfType,
                 sizeof(BITMAPFILEHEADER),1,fp);
    fread((char *)&bmi.biSize,
                 sizeof(BITMAPINFOHEADER),1,fp);

    /* check for an applewin bmp */
    if (bmi.biCompression==BI_RGB &&
        BitMapFileHeader.bfType[0] == 'B' && BitMapFileHeader.bfType[1] == 'M' &&
        bmi.biPlanes==1 && bmi.biBitCount == 8 &&
        bmi.biWidth == 560 &&  bmi.biHeight == 384)  {
        fread((char *)&sbmp[0].rgbBlue, sizeof(RGBQUAD)*256,1,fp);
        if (sbmp[17].rgbRed == 255 && sbmp[17].rgbGreen == 255 && sbmp[17].rgbBlue == 254) packet = 560;
    }

    if(packet == INVALID){
        puts(szTextTitle);
        printf("%s is not a Supported 560 x 384 AppleWin Screen Capture.\n", bmpfile);
        fclose(fp);
        free(dhrbuf);
        exit(1);
    }


    memset(&dhrbuf[0],0,16384); /* clear write buffer */


    i = 191; /* bmp's are upside-down
                so conversion of scanlines is in
                reverse order */
    for(y=192;y>0;y--)
    {
      fread((char *)&bmpscanline[0],1,560,fp);
      /* skip every second scanline  */
      fread((char *)&bmpscanline[0],1,560,fp);
      for (j=0;j<560;j++) {
        if (bmpscanline[j] != 0)bmpscanline[j] = 1;
      }
      applewinbits(i); /* encode 7 bit mono line */
      i--;
      if (i< 0) break;
    }
    fclose(fp);

    /* keep output files to a minimum */
    /* output Apple II files in one of 2 formats */
    if (applesoft == 0) {
        if (longnames == 0)sprintf(outfile,"%s.2FC", newname);
        else sprintf(outfile,"%s.A2FC", newname);
        fp = fopen(outfile,"wb");
        if (NULL == fp) {
            puts(szTextTitle);
            printf("%s cannot be created.\n", outfile);
            free(dhrbuf);
            exit(1);
        }
        fwrite(dhrbuf,1,16384,fp);
        fclose(fp);
    }
    else {
        /* output AUX,BIN - default is A2FC */
        /* the bsaved images are split into two files
        the first file is loaded into aux mem */
        sprintf(outfile,"%s.AUX",newname);
        fp = fopen(outfile,"wb");
        if (NULL == fp) {
            puts(szTextTitle);
            printf("%s cannot be created.\n", outfile);
            exit(1);
        }
        fwrite(dhrbuf,1,8192,fp);
        fclose(fp);

        /* the second file is loaded into main mem */
        sprintf(outfile,"%s.BIN",newname);
        fp = fopen(outfile,"wb");
        if (NULL == fp) {
            puts(szTextTitle);
            printf("%s cannot be created.\n", outfile);
            free(dhrbuf);
            exit(1);
        }
        fwrite(&dhrbuf[8192],1,8192,fp);
        fclose(fp);
    }

    /* save a back-up of the original AppleWin file */
    /* if we are not already processing the back-up */
    if (bm2 == 0) {
        sprintf(outfile,"%s.bm2",basename);
        remove(outfile);
        rename(bmpfile,outfile);
    }

return SUCCESS;

}


/* External Palette File - Color Output Helper functions */
/* External palette files must be in the Lo-Res Color Order.*/

/* These were put in place primarily to support:

1. The GIMP's gpl palette file format.
2. A simple CSV (comma separated value, comma delimited) file.

There is a small possibility that some people may use the JASC palette format
primarily because they may know it and it's easy.

Support for other types of palette files is speculative. Nobody might ever use
the others but the routines were already written so no harm to include the others.

I don't expect any of this to get much use anyway.

*/

/* ------------------------------------------------------------------------ */
/* START of palette reader and helper functions adapted from Clipshop       */
/* ------------------------------------------------------------------------ */

/*
squeeze redundant whitespace from lines read-in from a palette file
(leave only a single space character)
this is important if the user has created their own palette file
by hand... since they may accidentally type more than one whitespace
between RGB values...

Also, phototsyler version 2 palette file lines are fixed width,
right justified so we need to massage these for our reader...
*/
void SqueezeLine(char *ptr)
{
  int idx, jdx, len;
  char buf[128];

  idx = 0;
  while (ptr[idx] == ' ')idx++;  /* remove leading whitespace */
  strcpy(buf, &ptr[idx]);

  jdx = 0;
  ptr[jdx] = ASCIIZ;

  for (idx = 0; buf[idx] != ASCIIZ; idx++) {
    if (buf[idx] == 9) buf[idx] = ' ';         /* no tabs please */
    if (buf[idx] == ',') buf[idx] = ' ';       /* no commas please */
    if (buf[idx] == ' ' && buf[idx +1] == ' ')
      continue;
    /* truncate if any non-numeric characters */
    if ((buf[idx] < '0' || buf[idx] > '9') && buf[idx] != ' ')
      buf[idx] = ASCIIZ;
    ptr[jdx] = buf[idx]; jdx++;
    ptr[jdx] = ASCIIZ;
  }

  /* remove trailing whitespace...
    this occurrs during parsing of photostyler */
  len = strlen(ptr);
  while (len > 0) {
    len--;
    if (ptr[len] != ' ')
      break;
    ptr[len] = ASCIIZ;
  }
}

/* split the RGB triple from a text line read-in from an
   ascii palette file. */
int ReadPaletteLine(unsigned char *ptr, unsigned char *palptr, unsigned int colordepth)
{
  int red, green, blue, idx, spaces = 0;

  red = atoi(ptr);
  if (red < 0 || red > 255) return INVALID;

  /* there must be at least 3 fields */
  for (idx = 0; ptr[idx] != 0; idx++) {
    if (ptr[idx] == ' ' && ptr[idx+1] >= '0' && ptr[idx+1] <= '9') {
       spaces++;
       switch(spaces) {
         case 1:
           green = atoi(&ptr[idx+1]);
           if (green < 0 || green > 255) return INVALID;
           break;
         case 2:
           blue = atoi(&ptr[idx+1]);
           if (blue < 0 || blue > 255) return INVALID;
           break;
       }
    }
  }

  if (spaces<2)
    return INVALID;

  if (colordepth == 6) {
     palptr[0] = (uchar)red << 2;
     palptr[1] = (uchar)green << 2;
     palptr[2] = (uchar)blue << 2;
   }
   else {
     palptr[0] = (uchar)red;
     palptr[1] = (uchar)green;
     palptr[2] = (uchar)blue;
   }
   return SUCCESS;

}

/* check version if Paintshop palette since JASC may change someday */
/* also check Aldus version although that product is old... */

/* The Top Half of NeoPaint Windows Palettes are the same as their */
/* DOS palettes so we use the 6 bit color values and handle both   */
/* file types the same way... so no worry about neopaint versions. */

char *Gimp = "GIMP Palette"; /* followed by RGB values and comments */

/* NeoPaint and PaintShop Pro headers
   3 lines followed by RGB values */
char *NeoPaint  = "NeoPaint Palette File";
char *PaintShop = "JASC-PAL";
char *PaintShopVersion = "0100";

/* Aldus photostyler
   3 lines followed by RGB values */
char *AldusPal = "CWPAL";
char *AldusClr = "CWCLR";           /* partial palettes */
char *AldusVersion = "100";

#define GENERIC 1
#define GIMP 2
#define JASC 3
#define NEO 4
#define ALDUS 5

sshort GetUserPalette(char *name)
{
    FILE *fp;
    char buf[128];
    int cnt=16;
    sshort status = INVALID;
    unsigned colordepth=8,paltype=GENERIC;

    fp = fopen(name,"r");
    if (fp == NULL) return status;

    for (;;) {
        if (NULL == fgets(buf, 128, fp)) {
            fclose(fp);
            break;
        }
        nocr(buf);
        SqueezeLine(buf);

        /* check for some known palette types */
        if (strcmp(Gimp, buf)==0) paltype = GIMP;
        else if (strcmp(PaintShop, buf)==0) paltype = JASC;
        else if (strcmp(NeoPaint, buf)==0) {
            colordepth = 6;
            paltype = NEO;
        }
        else if (strcmp(AldusPal, buf) == 0 || strcmp(AldusClr, buf) == 0) {
            paltype = ALDUS;
        }
        /* if not a known type then assume it's just a simple csv */

        status = SUCCESS;
        switch(paltype)
        {
            case GENERIC: rewind(fp); break;

            case JASC:
            case NEO:
            case ALDUS:
                /* check 2 remaining header lines */
                status = INVALID;
                if (NULL == fgets(buf, 128, fp)) break;
                nocr(buf);
                SqueezeLine(buf);
                if (paltype == JASC && strcmp(PaintShopVersion, buf)!=0)break;
                if (paltype == ALDUS && strcmp(AldusVersion, buf) != 0)break;
                if (NULL == fgets(buf, 128, fp)) break;
                cnt = atoi(buf);
                if (cnt < 16) break;
                status = SUCCESS;
        }
        if (status == INVALID) break;

        memset(&rgbUser[0][0],0,48);
        cnt = 0;
        while (fgets(buf,128,fp) != NULL) {
            if (buf[0] == '#') continue;
            if (strlen(buf) < 5) continue;
            nocr(buf);
            SqueezeLine(buf);
            if (INVALID == ReadPaletteLine(buf,(uchar *)&rgbUser[cnt][0],colordepth)) continue;
            cnt++;
            if (cnt > 15)break;
        }
        break;
    }
    fclose(fp);

    if (cnt < 15) {
        printf("%s contains only %d colors!",name,cnt);
    }
    if (status == INVALID) {
        printf("%s is not a valid palette file!",name);
    }
    return status;
}

/* ------------------------------------------------------------------------ */
/* END of palette reader and helper functions adapted from Clipshop         */
/* ------------------------------------------------------------------------ */

/* Color Output Helper Function */
/* select conversion palette based on command-line options */
/* matches Bmp2DHR */
int GetBuiltinPalette(uchar cmd, uchar value, int palidx)
{
    int i,j, singlegrey = 0;
    uchar r,g,b;
    unsigned temp;

    if (cmd != 'P') return -1;
    if (value < 48 || value > 57) return -1;


    switch(palidx) {
        case 16:/* HGR conversion - optional palette from tohgr */
                for (i=0;i<16;i++) {
                    rgbArray[i][0] = hgrpal[i][0];
                    rgbArray[i][1] = hgrpal[i][1];
                    rgbArray[i][2] = hgrpal[i][2];
                }
                break;
        case 15: for (i=0;i<16;i++) {
                    rgbArray[i][0] = PseudoPalette[i][0];
                    rgbArray[i][1] = PseudoPalette[i][1];
                    rgbArray[i][2] = PseudoPalette[i][2];
                }
                break;
        /* Cybernesto */
        case 14: for (i=0;i<16;i++) {
                    rgbArray[i][0] = Cybernesto[i][0];
                    rgbArray[i][1] = Cybernesto[i][1];
                    rgbArray[i][2] = Cybernesto[i][2];
                }
                singlegrey = 1;
                break;
        /* Jace */
        case 13: for (i=0;i<16;i++) {
                    rgbArray[i][0] = Jace[i][0];
                    rgbArray[i][1] = Jace[i][1];
                    rgbArray[i][2] = Jace[i][2];
                }
                singlegrey = 1;
                break;
        /* Super Convert */
        case 12: for (i=0;i<16;i++) {
                    rgbArray[i][0] = SuperConvert[i][0];
                    rgbArray[i][1] = SuperConvert[i][1];
                    rgbArray[i][2] = SuperConvert[i][2];
                }
                break;
        /* 5 legacy palettes from BMPA2FC */
        case 11: for (i=0;i<16;i++) {
                    rgbArray[i][0] = rgbPcxArray[i][0];
                    rgbArray[i][1] = rgbPcxArray[i][1];
                    rgbArray[i][2] = rgbPcxArray[i][2];
                }
                break;
        case 10: for (i=0;i<16;i++) {
                    rgbArray[i][0] = rgbVgaArray[i][0];
                    rgbArray[i][1] = rgbVgaArray[i][1];
                    rgbArray[i][2] = rgbVgaArray[i][2];
                }
                break;
        case 9: for (i=0;i<16;i++) {
                    rgbArray[i][0] = rgbXmpArray[i][0];
                    rgbArray[i][1] = rgbXmpArray[i][1];
                    rgbArray[i][2] = rgbXmpArray[i][2];
                }
                break;
        case 8: for (i=0;i<16;i++) {
                    rgbArray[i][0] = rgbBmpArray[i][0];
                    rgbArray[i][1] = rgbBmpArray[i][1];
                    rgbArray[i][2] = rgbBmpArray[i][2];
                }
                break;
        case 7: for (i=0;i<16;i++) {
                    rgbArray[i][0] = rgbCanvasArray[i][0];
                    rgbArray[i][1] = rgbCanvasArray[i][1];
                    rgbArray[i][2] = rgbCanvasArray[i][2];
                }
                break;

        /* user definable palette file */
        case 6:
                for (i=0;i<16;i++) {
                    rgbArray[i][0] = rgbUser[i][0];
                    rgbArray[i][1] = rgbUser[i][1];
                    rgbArray[i][2] = rgbUser[i][2];
                }
                break;
        /* tohgr*/
        case 5: for (i=0;i<16;i++) {
                    rgbArray[i][0] = grpal[i][0];
                    rgbArray[i][1] = grpal[i][1];
                    rgbArray[i][2] = grpal[i][2];
                }
                singlegrey = 1;
                break;
        /* wikipedia */
        case 4: for (i=0;i<16;i++) {
                    rgbArray[i][0] = wikipedia[i][0];
                    rgbArray[i][1] = wikipedia[i][1];
                    rgbArray[i][2] = wikipedia[i][2];
                }
                singlegrey = 1;
                break;

        case 3: for (i=0;i<16;i++) {
                    rgbArray[i][0] = awinnewcolors[i][0];
                    rgbArray[i][1] = awinnewcolors[i][1];
                    rgbArray[i][2] = awinnewcolors[i][2];
                }
                break;
        case 2: for (i=0;i<16;i++) {
                    rgbArray[i][0] = awinoldcolors[i][0];
                    rgbArray[i][1] = awinoldcolors[i][1];
                    rgbArray[i][2] = awinoldcolors[i][2];
                }
                break;
        case 1: for (i=0;i<16;i++) {
                    rgbArray[i][0] = ciderpresscolors[i][0];
                    rgbArray[i][1] = ciderpresscolors[i][1];
                    rgbArray[i][2] = ciderpresscolors[i][2];
                }
                break;
        case 0: for (i=0;i<16;i++) {
                    rgbArray[i][0] = kegs32colors[i][0];
                    rgbArray[i][1] = kegs32colors[i][1];
                    rgbArray[i][2] = kegs32colors[i][2];
                }
                break;
        default: return -1;
        }

        /* grey conversion may be necessary if the Apple II Image
           uses light grey and dark grey. */
        if (doublegrey == 1) {
            if (singlegrey != 0 && rgbArray[5][0] != 0) {
               /* grey 1 = 5, grey2 = 10 */
               /* use the median between the existing grey and white */
               /* to approximate the color distance for this palette */
               temp = 255;
               temp += rgbArray[5][0];
               rgbArray[10][0] = rgbArray[10][1] = rgbArray[10][2] = (uchar) (temp/2);
            }
        }



        return palidx;

}


/* Monochrome Output Helper Function */
/* decodes apple II dhgr buffer into 8 bit monochrome scanline buffer */
int applebites(int y)
{
        int xoff,idx;
        unsigned char *ptraux, *ptrmain, ch;

        xoff = HB[y];
        ptraux  = (unsigned char *) &dhrbuf[xoff-0x2000];
        ptrmain = (unsigned char *) &dhrbuf[xoff];

        xoff = 0;
        for (idx = 0; idx < 40; idx++) {

            ch = ptraux[idx];

            buf560[xoff] = ((ch) &1); xoff++;
            buf560[xoff] = ((ch >> 1) &1); xoff++;
            buf560[xoff] = ((ch >> 2) &1); xoff++;
            buf560[xoff] = ((ch >> 3) &1); xoff++;
            buf560[xoff] = ((ch >> 4) &1); xoff++;
            buf560[xoff] = ((ch >> 5) &1); xoff++;
            buf560[xoff] = ((ch >> 6) &1); xoff++;

            ch = ptrmain[idx];

            buf560[xoff] = ((ch) &1); xoff++;
            buf560[xoff] = ((ch >> 1) &1); xoff++;
            buf560[xoff] = ((ch >> 2) &1); xoff++;
            buf560[xoff] = ((ch >> 3) &1); xoff++;
            buf560[xoff] = ((ch >> 4) &1); xoff++;
            buf560[xoff] = ((ch >> 5) &1); xoff++;
            buf560[xoff] = ((ch >> 6) &1); xoff++;

        }
        return SUCCESS;

}

/* Monochrome Output Helper Function */
/* encodes monochrome bmp scanline from 8 bit monochrome scanline buffer */
int ibmbites()
{
     int i,j,k;
     unsigned char bits[8];

     j=0;
     for(i=0;i<70;i++)
     {
        for(k=0;k<8;k++)
        {
          bits[k] = buf560[j]; j++;
        }
        bmpscanline[i] = (bits[0]<<7|bits[1]<<6|bits[2]<<5|bits[3]<<4|
                          bits[4]<<3|bits[5]<<2|bits[6]<<1|bits[7]);
     }
     return SUCCESS;

}

/* Monochrome Output Helper Function */
/* save a monochrome bmp file in either supported resolution */
int save_to_bmp(unsigned char *basename, int doublepixel)
{

    FILE *fp;
    unsigned char outfile[256];
    int y,y2;


    sprintf(outfile,"%s.bmp",basename);

    if (vbmp == 1) return WriteVBMPFile(outfile);

    fp = fopen(outfile,"wb");
    if (NULL == fp)return INVALID;

    /* write header */
    if (doublepixel == 1)
        fwrite(mono384,1,sizeof(mono384),fp);
    else
        fwrite(mono192,1,sizeof(mono192),fp);

    /* write scanlines */
    y2 = 191;
    bmpscanline[70] = bmpscanline[71] = 0xff; /* white padding */
    for (y = 0; y< 192; y++) {
       applebites(y2);
       ibmbites();
       fwrite(bmpscanline,1,72,fp);
       /* double-up */
       if (doublepixel == 1)
            fwrite(bmpscanline,1,72,fp);
       y2 -= 1;
    }

    return SUCCESS;

}


uchar greyoveride[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/* creates a IIgs mode320 PIC file with a single active palette */
/* also creates a IIgs mode320 PIC file with up to 16 active palettes */
/* also creates a IIgs mode3200 Brooks PIC file with 200 active palettes */
int SHR320_Output(char *outfile)
{

    FILE *fp;
    sshort i,j,k;
    uchar r,g,b,idx;
    int x, y;
    float hue, saturation,luminance;

    fp = fopen(outfile,"wb");
    if (NULL == fp) return INVALID;

    if (shrpalettes == 200 || shrpalettes == 16) {

        for (i = 0; i < 16; i++) {
            if (greyoveride[i] == 1 && quietmode == 0) printf("%s will be replaced with %d percent saturated equivalent!\n",colornames[i],(int)desaturate[i]);
        }


        /* palette is used for preview file for both 200 palette brooks and 16 palette pic */
        for (y = 0; y < 200; y++) {
            for (i = 0; i < 16; i++) {

                if (usegspalette == 1) {
                    rgbArrays[y][i][0] = gsColor(rgbArrays[y][i][0]);
                    rgbArrays[y][i][1] = gsColor(rgbArrays[y][i][1]);
                    rgbArrays[y][i][2] = gsColor(rgbArrays[y][i][2]);
                }


                if (greyoveride[i] != 1) continue;
                /* desaturate palette color if grey over-ride is in effect */
                rgb2hsl(rgbArrays[y][i][0],rgbArrays[y][i][1],rgbArrays[y][i][2],&hue,&saturation,&luminance);
                saturation = (saturation * desaturate[i]) / 100;
                hsl2rgb(hue,saturation,luminance,&rgbArrays[y][i][0],&rgbArrays[y][i][1],&rgbArrays[y][i][2]);
            }
        }
        if (shrpalettes == 16) {
            /* palette is used only for 16 palette pic */
            for (y = 0; y < 16; y++) {
                for (i = 0; i < 16; i++) {

                    if (usegspalette == 1) {
                        rgb256Arrays[y][i][0] = gsColor(rgb256Arrays[y][i][0]);
                        rgb256Arrays[y][i][1] = gsColor(rgb256Arrays[y][i][1]);
                        rgb256Arrays[y][i][2] = gsColor(rgb256Arrays[y][i][2]);
                    }


                    if (greyoveride[i] != 1) continue;
                    /* desaturate palette color if grey over-ride is in effect */
                    rgb2hsl(rgb256Arrays[y][i][0],rgbArrays[y][i][1],rgbArrays[y][i][2],&hue,&saturation,&luminance);
                    saturation = (saturation * desaturate[i]) / 100;
                    hsl2rgb(hue,saturation,luminance,&rgb256Arrays[y][i][0],&rgb256Arrays[y][i][1],&rgb256Arrays[y][i][2]);
                }
            }
        }

    }
    else {
        if (usegspalette == 1) {
            for (i=0;i<16;i++) {
                rgbArray[i][0] = gsColor(rgbArray[i][0]);
                rgbArray[i][1] = gsColor(rgbArray[i][1]);
                rgbArray[i][2] = gsColor(rgbArray[i][2]);
            }
        }
    }

    /* creates brooks files, and 16 palette and single-palette SHR PIC files */
    switch (shrpalettes) {
        case 200:
            /* brooks palettes - 1 for each line */
            /* in sequential order - no scb's */
            for (y=0;y<200;y++) {
                /* Brooks Palette Lines are in reverse order; the color value for color 15 is stored first.*/
                for (i = 0,j=30; i < 16;i++,j-=2) {

                    /* read BGR triples */
                    b = (uchar) (rgbArrays[y][i][2] >> 4);
                    g = (uchar) (rgbArrays[y][i][1] >> 4);
                    r = (uchar) (rgbArrays[y][i][0] >> 4);

                    /* encode $0RGB motorola unsigned short (LSB, MSB) for pic file into 2 bytes */
                    g = (uchar) (g << 4);
                    mypic.pal[y][j] = (uchar) (g | b);
                    mypic.pal[y][j+1] = r;
                }
            }
            break;

        case 16:
            /* scbs are set outside of here */
            for (j=0;j<16;j++) {
                /* palettes 0 - 16 */
                for (i=0;i< 16;i++) {
                    /* read BGR triples */
                    b = (uchar) (rgb256Arrays[j][i][2] >> 4);
                    g = (uchar) (rgb256Arrays[j][i][1] >> 4);
                    r = (uchar) (rgb256Arrays[j][i][0] >> 4);

                    /* offset from char to short */
                    k = i*2;

                    /* encode $0RGB motorola unsigned short (LSB, MSB) for pic file into 2 bytes */
                    g = (uchar) (g << 4);
                    mypic.pal[j][k] = (uchar) (g | b);
                    mypic.pal[j][k+1] = r;
                }
            }
            break;

        case 1:
        default:

            /* palette 0 - currently selected DHGR palette */
            /* all scbs default to zero */
            for (i=0;i< 16;i++) {

                /* read BGR triples */
                b = (uchar) (rgbArray[i][2] >> 4);
                g = (uchar) (rgbArray[i][1] >> 4);
                r = (uchar) (rgbArray[i][0] >> 4);

                /* offset from char to short */
                k = i*2;

                /* encode $0RGB motorola unsigned short (LSB, MSB) for pic file into 2 bytes */
                g = (uchar) (g << 4);
                mypic.pal[0][k] = (uchar) (g | b);
                mypic.pal[0][k+1] = r;
            }

    }

    /* 200 lines of image data */
    memset(&bmpscanline[0],0,160);
    for(y=0;y<200;y++) {
        /* build a packed pixel scanline */
        /* this is the same as for Windows 16 color BMPs */
        for (x = 0, i=0; x < 320; x++) {
            idx = getlopixel(x,y);
            /* range check */
            if (idx > 15)idx = 0; /* default black */
            if (x%2 == 0) {
                r = (uchar)idx << 4;
            }
            else {
                bmpscanline[i] = r | (uchar) idx; i++;
            }
        }
        fwrite((char *)&bmpscanline[0],1,160,fp);
    }
    if (shrpalettes == 200) {
        /* brooks */
        fwrite((char *)&mypic.pal[0],6400,1,fp);
    }
    else {
        /* PIC - scbs required  followed by palettes only */
        fwrite((char *)&mypic.scb[0],768,1,fp);
    }
    fclose(fp);

    return SUCCESS;
}

/* brooks output is experimental at this point */
int brooks2 = 0, brooks3 = 0, brooks4 = 0, brooks5 = 0;

int useoriginalcolors = 0;
int usesixteencolors = 0, usefourteencolors = 0, useegacolors = 0;

/* brooks4 and brooks5 arrays */
uchar linecolors[320][4];
ushort linecount[320];
double linedistance[320];

uchar linepalette[16][4];  /* r,g,b,used */
ushort linecolorindex[16]; /* index in linecolors array for color info */

uchar brooks5colors[] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};


char pcxheader[66], pcxfile[256];

/* this solves the problem of creating image specific conversion palettes especially for
    single palette output of SHR files - it takes too long to create a palette file so I just
    create a second image reduced to 16 colors and use that instead */

/* this gives me image level toning which generally works pretty well */
/* reads the palette from the BMP named on the option P - command line */
/* these can be created in the GIMP or any other editor that writes a compatible BMP file */
/* alternately, reads the palette from an 8-bit PCX file created by ImageMagick */
sshort GetBmpOrPcxPalette(char *name)
{
    FILE *fp;
    sshort status = INVALID, i, j, bmpversion;
    ulong flen;
    uchar temp;

    /* if we are using an imagemagick pcx for a palette, read the pcx file */
    /* ImageMagick also creates version 2 and 3 BMP files that use the standard header */
    /*

        C:\ImageMagick-6.9.1-1>convert blondeim.bmp -colors 16 -define format:bmp3 blondeim16.bmp
        C:\ImageMagick-6.9.1-1>convert blondeim.bmp -colors 16 -define format:bmp2 blondeim16.bmp
        C:\ImageMagick-6.9.1-1>convert blondeim.bmp -colors 16 -define format:pcx blonde.pcx

    */

    strcpy(pcxfile,name);

    j=999;
    for (i=0;pcxfile[i]!=(char)0;i++) {
       if (pcxfile[i] == '.') j = i;
    }
    if (j!=999) {
        pcxfile[j] = 0;
        strcat(pcxfile,".pcx");
        for (;;) {

            fp = fopen(pcxfile,"rb");

            if (fp == NULL) break;

            /* get the length of the file */
            fseek(fp, 0L, 2);
            flen = ftell(fp) - 769;

            /* avoid empty files - file must have a header and a palette */
            if (flen < (ulong)128) {
                fclose(fp);
                break;
            }

            /* read the PCX header for compatibility */
            rewind(fp);
            fread(pcxheader,66,1,fp);

            /* PCX version 5 with 8-bits per pixel, run-length encoded with one color plane */
            /* PCX version 5 with 1-bit  per pixel, run-length encoded with one color plane */
            if (pcxheader[0] != (char)10 || pcxheader[1] != (char)5 ||
                pcxheader[2] != (char)1 || pcxheader[65] != (char)1) {
                fclose(fp);
                break;
            }

            /* seek to end of file and read the first sixteen palette entries */
            fseek(fp,flen,SEEK_SET);
            /* first check the palette header (it must be a formfeed character) */
            fread(pcxheader, 1, 1, fp);
            if (pcxheader[0] != (char) 12) {
                fclose(fp);
                break;
            }
            /* read the palette */
            fread(&rgbUser[0][0],48,1,fp);
            fclose(fp);
            status = SUCCESS;
            break;
        }
    }

    if (status == SUCCESS) return SUCCESS;

    fp = fopen(name,"rb");
    if (fp == NULL) {
        /* try to open an m2s palette file */
        j=999;
        for (i=0;pcxfile[i]!=(char)0;i++) {
            if (pcxfile[i] == '.') j = i;
            if (pcxfile[i] == '_') {
                /* allow re-rendering of Simplifly output */
                if (cmpstr((char *)&pcxfile[i],"_proc.bmp") == SUCCESS) {
                    j = i;
                    break;
                }
            }
        }
        if (j==999) return status;
        pcxfile[j] = 0;
        strcat(pcxfile,"_palette.bmp");
        fp = fopen(pcxfile,"rb");
        /* if we have tried everything that makes sense, then give it up */
        if (fp == NULL)return status;
    }

    memset(&BitMapFileHeader.bfType,0,sizeof(BITMAPFILEHEADER));
    memset(&bmi.biSize,0,sizeof(BITMAPINFOHEADER));

    fread((char *)&BitMapFileHeader.bfType,
                 sizeof(BITMAPFILEHEADER),1,fp);
    fread((char *)&bmi.biSize,
                 sizeof(BITMAPINFOHEADER),1,fp);

    /* version 4 and version 5 BMP's use a different header than a version 3 bmp and may be incompatible */
    /* but initial tests show no sign of incompatibility */
    if (bmi.biSize != sizeof(BITMAPINFOHEADER)) {
		if (bmi.biSize == sizeof(BITMAPINFOHEADERV2)|| bmi.biSize == sizeof(BITMAPINFOHEADERV3)) {
            memset(&bmiV5.biSize,0,sizeof(BITMAPINFOHEADERV5));
            fseek(fp,sizeof(BITMAPFILEHEADER),SEEK_SET);
            fread((char *)&bmiV5.biSize,(unsigned)bmi.biSize,1,fp);
            bmpversion = 3;
		}
		else if (bmi.biSize == sizeof(BITMAPINFOHEADERV4)) {
            memset(&bmiV5.biSize,0,sizeof(BITMAPINFOHEADERV5));
            fseek(fp,sizeof(BITMAPFILEHEADER),SEEK_SET);
            fread((char *)&bmiV5.biSize,sizeof(BITMAPINFOHEADERV4),1,fp);
            bmpversion = 4;

        }
        else if (bmi.biSize == sizeof(BITMAPINFOHEADERV5)) {
            /* https://msdn.microsoft.com/en-us/library/windows/desktop/dd183386%28v=vs.85%29.aspx */
            memset(&bmiV5.biSize,0,sizeof(BITMAPINFOHEADERV5));
            fseek(fp,sizeof(BITMAPFILEHEADER),SEEK_SET);
            fread((char *)&bmiV5.biSize,sizeof(BITMAPINFOHEADERV5),1,fp);
            bmpversion = 5;
            /*
            Profile data refers to either the profile file name (linked profile)
            or the actual profile bits (embedded profile).
            The file format places the profile data at the end of the file.
            The profile data is placed just after the color table (if present).
            */
        }
        else {
            /* perhaps some unknown version that I don't support */
            /* whether I support the other versions properly is also an interesting question */
            bmpversion = 0;
        }

    }
    else {
        bmpversion = 3;
    }

    if (bmpversion != 0) {
        /* use any size at all - 16-color, 256-color, and 24-bit Version 3 BMP's are all supported here */
        if (bmi.biCompression==BI_RGB && BitMapFileHeader.bfType[0] == 'B' && BitMapFileHeader.bfType[1] == 'M') {

            if (bmi.biBitCount == 4 || bmi.biBitCount == 8) {
                status = SUCCESS;
                fread((char *)&sbmp[0].rgbBlue, sizeof(RGBQUAD)*16,1,fp);
                for (j=0;j<16;j++) {
                    rgbUser[j][0] = sbmp[j].rgbRed;
                    rgbUser[j][1] = sbmp[j].rgbGreen;
                    rgbUser[j][2] = sbmp[j].rgbBlue;
                }

           }
           else if (bmi.biBitCount == 24) {
                /* last of all, check for an m2s palette file */
                /* or something similar */
                status = SUCCESS;
                fread(&rgbUser[0][0],48,1,fp);
                /* reverse the Windows BGR triples to RGB triples */
                for (j=0;j<16;j++) {
                    temp = rgbUser[j][0];
                    rgbUser[j][0] = rgbUser[j][2];
                    rgbUser[j][2] = temp;
                }
           }

        }
    }

    fclose(fp);
    return status;
}


/* converts to lores and double lo-res image fragments and backgrounds (primarily targeted at game development) */
/* also converts to SHR mode320 full-screen PIC files - Single Palette, 16-Palette, and 200 Palette (Brooks) format*/
int ConvertLoResAndSHR(unsigned char *basename, unsigned char *newname)
{

    FILE *fp;
    int packet = INVALID, y,y1,y2,x,i,j,k,width,height,reformat = bmp3, bmpversion =0,lidx,didx,count;
    int outpacket, outputwidth, outputheight, offset;
    char bmpfile[256], outfile[256];
    uchar r,g,b,lr,lg,lb,red,green,blue,drawcolor,idx,toneindex;
    ushort temp, fl, darkest,lightest,found,unused;
    float hue,saturation,luminance;
    sshort jdx;

    sprintf(bmpfile,"%s.bmp",basename);

    if((fp=fopen(bmpfile,"rb"))==NULL) {
        printf("%s cannot be opened for input!\nIt probably does not exist or the filename was mis-spelled...\nExiting!\n");
        return INVALID;
    }

    /* read the header stuff into the appropriate structures,
       it's likely a bmp file */
    memset(&BitMapFileHeader.bfType,0,sizeof(BITMAPFILEHEADER));
    memset(&bmi.biSize,0,sizeof(BITMAPINFOHEADER));

    fread((char *)&BitMapFileHeader.bfType,
                 sizeof(BITMAPFILEHEADER),1,fp);
    fread((char *)&bmi.biSize,
                 sizeof(BITMAPINFOHEADER),1,fp);

    if (bmi.biSize != sizeof(BITMAPINFOHEADER)) {
		if (bmi.biSize == sizeof(BITMAPINFOHEADERV2)|| bmi.biSize == sizeof(BITMAPINFOHEADERV3)) {
            memset(&bmiV5.biSize,0,sizeof(BITMAPINFOHEADERV5));
            fseek(fp,sizeof(BITMAPFILEHEADER),SEEK_SET);
            fread((char *)&bmiV5.biSize,(unsigned)bmi.biSize,1,fp);
            bmpversion = 3;
            reformat = 1;
		}
		else if (bmi.biSize == sizeof(BITMAPINFOHEADERV4)) {
            memset(&bmiV5.biSize,0,sizeof(BITMAPINFOHEADERV5));
            fseek(fp,sizeof(BITMAPFILEHEADER),SEEK_SET);
            fread((char *)&bmiV5.biSize,sizeof(BITMAPINFOHEADERV4),1,fp);
            bmpversion = 4;
            reformat = 1;

        }
        else if (bmi.biSize == sizeof(BITMAPINFOHEADERV5)) {
            memset(&bmiV5.biSize,0,sizeof(BITMAPINFOHEADERV5));
            fseek(fp,sizeof(BITMAPFILEHEADER),SEEK_SET);
            fread((char *)&bmiV5.biSize,sizeof(BITMAPINFOHEADERV5),1,fp);
            bmpversion = 5;
            reformat = 1;
            /*
            Profile data refers to either the profile file name (linked profile)
            or the actual profile bits (embedded profile).
            The file format places the profile data at the end of the file.
            The profile data is placed just after the color table (if present).
            */
        }
        else {
            bmpversion = 0;
        }

    }
    else {
        bmpversion = 3;
    }

    if (bmpversion == 0) {
        fclose(fp);
        puts("BMP version not recognized!");
        printf("bmi.biSize = %d - BITMAPINFOHEADER = %d\n",bmi.biSize, sizeof(BITMAPINFOHEADER));
        return INVALID;
    }


    if (shr == 320) {
        lores = 1;
        doublelores = 0;
    }
    else {
        usegscolors = shrgrey = 0;
    }

    if (bmi.biCompression==BI_RGB &&
        BitMapFileHeader.bfType[0] == 'B' && BitMapFileHeader.bfType[1] == 'M' &&
        bmi.biPlanes==1 && (bmi.biBitCount == 24 || bmi.biBitCount == 8 || bmi.biBitCount == 4 || bmi.biBitCount == 1)) {

            if (shr == 320) {
                if (bmi.biWidth != 320 || bmi.biHeight != 200) {
                    fclose(fp);
                    puts(szTextTitle);
                    printf("%s is not in a supported size for SHR mode320 file output.\nExiting!\n", bmpfile);
                    return INVALID;
                }
            }

            if (bmi.biWidth > 320 || bmi.biHeight > 200) {
                fclose(fp);
                puts(szTextTitle);
                printf("%s is not in a supported size for LGR or DLGR file output.\nExiting!\n", bmpfile);
                return INVALID;
            }


            outputheight = height = (int) bmi.biHeight;
            outputwidth = width = (int) bmi.biWidth;

            if (outputheight%2 != 0) outputheight++;
            outputheight = outputheight/2;

            if (doublelores == 1) {
                /* double lo-res files are paired-pixels */
                /* they don't need to be and I may rethink this later */
                /* they must also be loaded on paired origins because auxiliary memory
                   uses different color mapping than main memory
                   */
                if (outputwidth%2 != 0) outputwidth++;
                outpacket = outputwidth / 2;
            }
            else outpacket = outputwidth;

            packet = width * 3;
            while (packet%4 != 0)packet++; /* BMP scanlines align on DWORD boundaries */
    }

    if (packet == INVALID) {
        fclose(fp);
        if (shr == 320) printf("%s is not in a supported format for SHR or BROOKS file output.\nExiting!\n", bmpfile);
        else printf("%s is not in a supported format for LGR or DLGR file output.\nExiting!\n", bmpfile);
        return INVALID;
    }

    if (shr == 320 || bmi.biBitCount == 8 || bmi.biBitCount == 4 || bmi.biBitCount == 1 || usegscolors == 1) {

       memset((char *)&sbmp[0].rgbBlue,0,sizeof(RGBQUAD)*256);

       if (bmi.biBitCount == 1) {
           fread((char *)&sbmp[0].rgbBlue, sizeof(RGBQUAD)*2,1,fp);
           if (shr == 320) dither = 0;
       }
       else if (bmi.biBitCount == 4) {
           fread((char *)&sbmp[0].rgbBlue, sizeof(RGBQUAD)*16,1,fp);
           if (shr == 320) {
               /* for SHR output, 16 color BMP's are assumed to be palette matched and are
                  converted using the colors they were created with */
               for (j=0;j<16;j++) {
                    if (usegscolors == 1) {
                        sbmp[j].rgbRed = gsColor(sbmp[j].rgbRed);
                        sbmp[j].rgbGreen = gsColor(sbmp[j].rgbGreen);
                        sbmp[j].rgbBlue = gsColor(sbmp[j].rgbBlue);
                    }
                    rgbArray[j][0] = sbmp[j].rgbRed;
                    rgbArray[j][1] = sbmp[j].rgbGreen;
                    rgbArray[j][2] = sbmp[j].rgbBlue;
               }
           }
       }
       else if (bmi.biBitCount == 8) {
           /* for SHR no special consideration is provided for 256 color BMP's in this utility.
              they are assumed to be converted GIF files or the equivalent and since we
              are providing only a single palette they get the same treatment as a 24-bit BMP.

              a color count routine is really required here to weed-out 16 color BMP's that
              have been promoted to 256 color BMP's and 24-bit BMP's.

              to do more for 256 color BMP's and 24-bit BMP's, palette routines that can be supported
              by the standard SHR format's 16 colors per line and 16 palettes per PIC would be needed. */
           fread((char *)&sbmp[0].rgbBlue, sizeof(RGBQUAD)*256,1,fp);
           if (usegscolors == 1) {
                for (j=0;j<256;j++) {
                    sbmp[j].rgbRed = gsColor(sbmp[j].rgbRed);
                    sbmp[j].rgbGreen = gsColor(sbmp[j].rgbGreen);
                    sbmp[j].rgbBlue = gsColor(sbmp[j].rgbBlue);
                }
           }
       }

       fp = ReformatBMP(fp);
       if (NULL == fp) return SUCCESS;
       if (shr == 1) {
           if (shrmode < 17) {
               puts("Fixed palette (16 color) mapping will be used for conversion.");
               if (mono == 0) puts("Dithering will have no effect.");
           }
       }
       reformat = 1;

    }

    if (mono == 1) {
        shr2 = 0;

        if (hsl == 1) {
            puts("achromatic hsl monochrome using hue channel de-saturation.");
        }
        else {
            puts("average rgb monochrome using color channel scaling.");
        }
        if (shrgrey == 1) {
            /* for SHR mode320 mono we use 16 levels of grey */
            for (j=0;j<16;j++) {
                rgbArray[j][0] = rgbArray[j][1] = rgbArray[j][2] = LegalColor[j];
            }
            puts("16 color greyscale conversion!");
        }
        else {
            /* for LGR and DLGR fixed palette output and SHR 320 true monochrome */
            /* create a black and white palette */
            memset(&rgbArray[0][0],0,45);
            memset(&rgbArray[15][0],255,3);
            if(shr == 320) puts("2 color monochrome conversion!");
        }
    }


    /* hard to say how this will work out */
    if (mono == 0 && shrmode > 16) {
        if (fourpal == 1) {
            /* group on GS colors */
            for (k=0;k<16;k++) {
                rgbArray[k][0] = gsColor(rgbArray[k][0]);
                rgbArray[k][1] = gsColor(rgbArray[k][1]);
                rgbArray[k][2] = gsColor(rgbArray[k][2]);
            }
        }

    }

    /* initialize nearest color arrays */
    /* for SHR this array will be used no matter what kind of output we eventually end-up with */
    /* for LGR and DLGR there is only one fixed palette of Lo-Res colors so there are no additional palettes */
    InitDoubleArrays();

    if (mono == 0 && shr == 320 && useimagetone == 1) {
        /* build initial palette of most used colors in each of our sixteen ranges */

        for (j=0;j<16;j++) {
            rgbArrays[0][j][0] = rgbArray[j][0];
            rgbArrays[0][j][1] = rgbArray[j][1];
            rgbArrays[0][j][2] = rgbArray[j][2];
            rgbUsed[0][j] = 0; /* not used yet */
            rgbDistance[0][j] = 0.0; /* initialize */
        }


        fseek(fp,BitMapFileHeader.bfOffBits,SEEK_SET);
        for(y=0,y1=height-1;y<height;y++,y1--)
        {
            fread((char *)&bmpscanline[0],1,packet,fp);
            if (fourplay == 1) {
                /* reduce palette */
                for (k=0;k<packet;k++) bmpscanline[k] = gsColor(bmpscanline[k]);
            }

            for (j=0, x = 0; x < width; x++) {
                b = bmpscanline[j]; j++;
                g = bmpscanline[j]; j++;
                r = bmpscanline[j]; j++;
                idx = GetClosestColor(r,g,b);
                for (toneindex = 0; toneindex < 16;toneindex++) {

                    idx = GetColorDistance(r,g,b,toneindex);
                    if (idx != toneindex) continue;

                    if (rgbUsed[0][toneindex] == 0) {
                        /* set initial value if this palette index has been used */
                        rgbUsed[0][toneindex] = 1;
                        rgbDistance[0][toneindex] = indexdistance;
                        rgbArrays[0][toneindex][0] = r;
                        rgbArrays[0][toneindex][1] = g;
                        rgbArrays[0][toneindex][2] = b;
                        continue;
                    }

                    /* if the new color is closer to the currently selected palette color
                       than the previously stored color, then replace the previously stored
                       color with the new color */
                    if (indexdistance < rgbDistance[0][toneindex]) {
                        rgbDistance[0][toneindex] = indexdistance;
                        rgbArrays[0][toneindex][0] = r;
                        rgbArrays[0][toneindex][1] = g;
                        rgbArrays[0][toneindex][2] = b;
                    }
                }
            }
        }

        for (j=0;j<16;j++) {
            rgbArray[j][0] = rgbArrays[0][j][0];
            rgbArray[j][1] = rgbArrays[0][j][1];
            rgbArray[j][2] = rgbArrays[0][j][2];
        }

        InitDoubleArrays();
        /* end of image tone */

    }

    if (shr2 != 0) {
        brooks = 0;
        /* build initial palette of most used colors in each of our sixteen ranges */

        for (j=0;j<16;j++) {
            rgbArrays[0][j][0] = rgbArray[j][0];
            rgbArrays[0][j][1] = rgbArray[j][1];
            rgbArrays[0][j][2] = rgbArray[j][2];
            rgbUsed[0][j] = 0; /* not used yet */
            rgbDistance[0][j] = 0.0; /* initialize */
        }


        fseek(fp,BitMapFileHeader.bfOffBits,SEEK_SET);
        for(y=0,y1=height-1;y<height;y++,y1--)
        {
            fread((char *)&bmpscanline[0],1,packet,fp);
            if (fourplay == 1) {
                /* reduce palette */
                for (k=0;k<packet;k++) bmpscanline[k] = gsColor(bmpscanline[k]);
            }

            for (j=0, x = 0; x < width; x++) {
                b = bmpscanline[j]; j++;
                g = bmpscanline[j]; j++;
                r = bmpscanline[j]; j++;
                idx = GetClosestColor(r,g,b);

                if (rgbUsed[0][idx] == 0) {
                    /* set initial value if this palette index has been used */
                    rgbUsed[0][idx] = 1;
                    rgbDistance[0][idx] = globaldistance;
                    rgbArrays[0][idx][0] = r;
                    rgbArrays[0][idx][1] = g;
                    rgbArrays[0][idx][2] = b;
                    continue;
                }

                /* if the new color is closer to the currently selected palette color
                   than the previously stored color, then replace the previously stored
                   color with the new color */
                if (globaldistance < rgbDistance[0][idx]) {
                    rgbDistance[0][idx] = globaldistance;
                    rgbArrays[0][idx][0] = r;
                    rgbArrays[0][idx][1] = g;
                    rgbArrays[0][idx][2] = b;
                }
            }
        }

        for (j=0;j<16;j++) {

            if (useimagetone == 0) {
                if (rgbArray[j][0] == rgbArrays[0][j][0] &&
                    rgbArray[j][1] == rgbArrays[0][j][1] &&
                    rgbArray[j][2] == rgbArrays[0][j][2]) greyoveride[j] = 1;
            }

            rgbArray[j][0] = rgbArrays[0][j][0];
            rgbArray[j][1] = rgbArrays[0][j][1];
            rgbArray[j][2] = rgbArrays[0][j][2];
        }

        InitDoubleArrays();
        /* end of shr2 */
    }

    /* if we are converting to SHR we now must decide on a conversion plan */
    /* for SHR since all input is now 24-bit we need to build some palettes */
    if (brooks != 0) {

        if (useoriginalcolors == 0 && quietmode == 0 && mix256 == 0) {
            puts("Initial Palette Color Values:");
            for (i=0;i<16;i++) {
                printf("Index %-2d: %-3d %-3d %-3d %s\n",i,rgbArray[i][0],rgbArray[i][1],rgbArray[i][2],colornames[i]);
            }
        }

        shrpalettes = 200;

        /* set initial values for conversion palettes */
        /* use color space of currently selected DHGR palette for color grouping of 16 colors */
        for (i=0;i<shrpalettes;i++) {
            for (j=0;j<16;j++) {
                if (useoriginalcolors == 0) {
                    rgbArrays[i][j][0] = rgbArray[j][0];
                    rgbArrays[i][j][1] = rgbArray[j][1];
                    rgbArrays[i][j][2] = rgbArray[j][2];
                    rgbUsed[i][j] = 0; /* not used */
                    rgbDistance[i][j] = 0.0; /* initialize */

                }
                else {
                    if (usefourteencolors == 1 && j==15) {
                        rgbArrays[i][j][0] = rgbArrays[i][j][1] = rgbArrays[i][j][2] = 255;
                    }
                    else {
                        rgbArrays[i][j][0] = rgbArrays[i][j][1] = rgbArrays[i][j][2] = 0;
                    }
                }
            }
        }

        fseek(fp,BitMapFileHeader.bfOffBits,SEEK_SET);
        for(y=0,y1=height-1;y<height;y++,y1--)
        {
            fread((char *)&bmpscanline[0],1,packet,fp);
            if (fourplay == 1) {
                /* reduce palette */
                for (k=0;k<packet;k++) bmpscanline[k] = gsColor(bmpscanline[k]);
            }

            if (brooks4 == 1 || brooks5 == 1) {
                /* this is a combination of population and original closest color */

                /* step 1 */
                /* first get closest color by population for 16 indexes */
                /* this is different than brooks, brooks2, and brooks3 (grey replacement) which do not consider population */
                count = 0;

                /* accumulate all original colors into line array */
                /* this a large working palette for the current line of all the original colors and their values */
                for (j=0, x = 0, count = 0; x < width; x++) {
                    b = bmpscanline[j]; j++;
                    g = bmpscanline[j]; j++;
                    r = bmpscanline[j]; j++;

                    found = 0;
                    for (i=0;i<count;i++) {
                        /* compare to original color value */
                        if (linecolors[i][0] == r && linecolors[i][1] == g && linecolors[i][2] == b) {
                            /* if color matches, update color count for this index entry */
                            linecount[i] += 1;
                            found = 1;
                            break;
                        }
                    }
                    if (found != 1) {
                        /* if no matching colors found in line colors add a new entry */
                        /* store r,g,b, color index and color distance */
                        linecolors[count][0] = r;
                        linecolors[count][1] = g;
                        linecolors[count][2] = b;
                        linecolors[count][3] = GetClosestColor(r,g,b);
                        linedistance[count] = globaldistance;
                        linecount[count] = 1;
                        count++;
                    }
                }

                /* step 2 */
                /* build initial palette of most used colors in each of our sixteen ranges */
                memset(&linepalette[0][0],0,64);
                memset(&linecolorindex[0],0,32);
                for (j = 0; j < count; j++) {
                    idx = linecolors[j][3];
                    if (linepalette[idx][3] == 1) {
                        /* index is already in use so update if count is higher */
                        i = linecolorindex[idx];
                        /* reset index used to zero to force update if count is higher */
                        if (linecount[j] > linecount[i]) linepalette[idx][3] = 0;
                    }

                    if (linepalette[idx][3] == 0) {
                        /* initial palette entry */
                        linepalette[idx][0] = linecolors[j][0];
                        linepalette[idx][1] = linecolors[j][1];
                        linepalette[idx][2] = linecolors[j][2];
                        linepalette[idx][3] = 1; /* mark palette index as used */
                        linecolorindex[idx] = j; /* store the linecolors index */
                        continue;
                    }
                }

                /* mark used colors in the linecolors array */
                for (idx=0,unused=0;idx<16;idx++) {
                    if (linepalette[idx][3] == 0) {
                        unused++;
                        continue;
                    }
                    brooks5colors[idx] = 1;
                    i = linecolorindex[idx];
                    linecount[i] = 0;
                }

                if (unused > 0) {
                    for (idx=0;idx<16;idx++) {
                        if (linepalette[idx][3] == 1) continue;
                        found = x = 0;
                        if (brooks5 == 1) x = 1;
                        for (j = 0; j < count; j++) {
                            if (linecount[j] > x) {
                                if (brooks5 == 1) {
                                    i = linecolors[j][3];
                                    /* restrict greys, blacks and whites to one entry */
                                    if (i == LOBLACK || i == LOWHITE ||
                                        i == LOGRAY || i == LOGREY) continue;
                                    if (brooks5colors[i] < 1) continue;
                                }
                                i = j;
                                x = linecount[j];
                                found = 1;
                            }
                        }
                        if (found == 1) {
                            j = linecolors[i][3];
                            brooks5colors[j] = 0;
                            linepalette[idx][0] = linecolors[i][0];
                            linepalette[idx][1] = linecolors[i][1];
                            linepalette[idx][2] = linecolors[i][2];
                            linepalette[idx][3] = 1; /* mark palette index as used */
                            linecolorindex[idx] = i; /* store the linecolors index */
                            linecount[i] = 0;        /* mark color used */
                            unused--;
                        }
                    }
                }

                /* if we still need colors just add them */
                if (brooks5 == 1 && unused > 0) {
                    for (idx=0;idx<16;idx++) {
                        if (linepalette[idx][3] == 1) continue;
                        found = x = 0;
                        for (j = 0; j < count; j++) {
                            if (linecount[j] > x) {
                                i = j;
                                x = linecount[j];
                                found = 1;
                            }
                        }
                        if (found == 1) {
                            j = linecolors[i][3];
                            brooks5colors[j] = 0;
                            linepalette[idx][0] = linecolors[i][0];
                            linepalette[idx][1] = linecolors[i][1];
                            linepalette[idx][2] = linecolors[i][2];
                            linepalette[idx][3] = 1; /* mark palette index as used */
                            linecolorindex[idx] = i; /* store the linecolors index */
                            linecount[i] = 0;        /* mark color used */
                            unused--;
                        }
                    }
                }


                for (idx=0;idx<16;idx++) {
                    rgbArrays[y1][idx][0] = linepalette[idx][0];
                    rgbArrays[y1][idx][1] = linepalette[idx][1];
                    rgbArrays[y1][idx][2] = linepalette[idx][2];

                }

                continue;
            }

            if (useoriginalcolors == 1) {
                count = 0;
                for (j=0, x = 0; x < width; x++) {
                    /* store original colors and working colors */
                    blue = b = bmpscanline[j]; j++;
                    green = g = bmpscanline[j]; j++;
                    red = r = bmpscanline[j]; j++;

                    /* if we are using a threshold color, assign it to our working colors */
                    if (useegacolors == 1) {
                        r = sethold(r,rhold);
                        g = sethold(g,rhold);
                        b = sethold(b,rhold);
                    }

                    found = 0;
                    if (usefourteencolors == 1) {
                        /* force black and white into palette */
                        idx = GetClosestColor(r,g,b);
                        if (idx == 15) r = g = b = red = blue = green = 255;
                        else if (idx == 0) r = g = b = red = green = blue = 0;
                    }
                    for (i=0;i<count;i++) {

                        if (useegacolors == 1) {
                            /* compare on threshold colors */
                            lr = sethold(linecolors[i][0],rhold);
                            lg = sethold(linecolors[i][1],ghold);
                            lb = sethold(linecolors[i][2],bhold);
                        }
                        else {
                            /* compare on full original color value */
                            lr = linecolors[i][0];
                            lg = linecolors[i][1];
                            lb = linecolors[i][2];
                        }

                        if (lr == r && lg == g && lb == b) {
                            if (useegacolors == 1) {
                                idx = GetClosestColor(red,green,blue);
                                if (globaldistance < linedistance[i]) {
                                    /* update rgb values if closer to current palette than first values added */
                                    linecolors[count][0] = red;
                                    linecolors[count][1] = green;
                                    linecolors[count][2] = blue;
                                    linecolors[count][3] = GetClosestColor(red,green,blue);
                                    linedistance[count] = globaldistance;
                                }
                            }
                            linecount[i] += 1;
                            found = 1;
                            break;
                        }
                    }
                    if (found != 1) {
                        linecolors[count][0] = red;
                        linecolors[count][1] = green;
                        linecolors[count][2] = blue;
                        linecolors[count][3] = GetClosestColor(red,green,blue);
                        linedistance[count] = globaldistance;
                        linecount[count] = 1;
                        count++;
                    }
                }

                if (usesixteencolors == 0 && usefourteencolors == 0) {
                    /* get darkest original color */
                    /* get lightest original color */
                    darkest = (ushort)linecolors[0][0];
                    darkest += linecolors[0][1];
                    darkest += linecolors[0][2];
                    lightest = darkest;
                    lidx = didx = 0;
                    for (i=1;i<count;i++) {
                        /* walk through the colors and find darkest and lightest colors */
                        temp = (ushort)linecolors[i][0];
                        temp += linecolors[i][1];
                        temp += linecolors[i][2];
                        if (temp < darkest) {
                            darkest = temp;
                            didx = i;
                        }
                        else if (temp > lightest) {
                            lightest = temp;
                            lidx = i;
                        }
                    }
                    for (i=0;i<15;i++) {
                        /* set the first 15 colors to the darkest color initially */
                        rgbArrays[y1][i][0] = linecolors[didx][0];
                        rgbArrays[y1][i][1] = linecolors[didx][1];
                        rgbArrays[y1][i][2] = linecolors[didx][2];
                    }
                    /* set the last color to the lightest color initially */
                    rgbArrays[y1][15][0] = linecolors[lidx][0];
                    rgbArrays[y1][15][1] = linecolors[lidx][1];
                    rgbArrays[y1][15][2] = linecolors[lidx][2];

                    linecount[lidx] = linecount[didx] = 0;
                }

                /* add up to 14 (or 16) most-used colors between the darkest and lightest to this palette */

                if (usesixteencolors == 1) {
                    j = 0;
                    lidx = 15;
                }
                else {
                    j = 1;
                    lidx = 14;
                }
                for (i = width;i > -1; i--) {
                    for (x=0;x<count;x++) {
                        if (linecount[x] == i) {
                            linecount[x] = 0;
                            if (usefourteencolors == 1) {
                                /* if palette is already preset for black and white no point in adding again */
                                if (linecolors[x][0] == 0 && linecolors[x][1] == 0 && linecolors[x][2] == 0) continue;
                                if (linecolors[x][0] == 255 && linecolors[x][1] == 255 && linecolors[x][2] == 255) continue;
                            }
                            rgbArrays[y1][j][0] = linecolors[x][0];
                            rgbArrays[y1][j][1] = linecolors[x][1];
                            rgbArrays[y1][j][2] = linecolors[x][2];
                            j++;
                            if (j>14) break;
                        }
                    }
                    if (j>14) break;
                }
                continue;
                /* original colors continues the loop */
            }


            for (j=0, x = 0; x < width; x++) {
                b = bmpscanline[j]; j++;
                g = bmpscanline[j]; j++;
                r = bmpscanline[j]; j++;

                idx = GetClosestColor(r,g,b);

                if (rgbUsed[y1][idx] == 0) {
                    /* set initial value if this palette index has been used */
                    rgbUsed[y1][idx] = 1;
                    rgbDistance[y1][idx] = globaldistance;
                    rgbArrays[y1][idx][0] = r;
                    rgbArrays[y1][idx][1] = g;
                    rgbArrays[y1][idx][2] = b;
                    continue;
                }

                /* if the new color is closer to the currently selected palette color
                   than the previously stored color, then replace the previously stored
                   color with the new color */
                if (globaldistance < rgbDistance[y1][idx]) {
                    rgbDistance[y1][idx] = globaldistance;
                    rgbArrays[y1][idx][0] = r;
                    rgbArrays[y1][idx][1] = g;
                    rgbArrays[y1][idx][2] = b;
                }
            }
        }

        /* fill-in actual image colors in the unused range */
        if (useoriginalcolors == 0 && (brooks2 == 1 || brooks3 == 1)) {
            /* go through each scanline in the image looking for palette entries that have not been set */
            /* the theory is that colors from the original image will match more closely than the colors
               from the currently selected DHGR palette */

            /* go through these in index order */
            for (i=0;i<16;i++) {
                /* found = 1; */
                for (y = 0; y < 200; y++) {
                    /* if no image color check the other lines */
                    if (rgbUsed[y][i] == 0) {
                        found = 1;
                        y1 = y2 = y;
                        for (;;)
                        {
                            /* expand upwards and downwards looking for an actual image color */
                            y1--;
                            y2++;
                            /* if we have expanded past the bottom and top of the image then give-up
                               for all the scanlines because this means that this color is not in
                               the image */
                            if (y1 < 0 && y2 > 200) {
                                found = 0;
                                break;

                            }
                            /* if one of the lines above has this palette entry then use it */
                            if (y1 > -1 && rgbUsed[y1][i] != 0) {
                                rgbUsed[y][i] = 1;
                                rgbArrays[y][i][0]=rgbArrays[y1][i][0];
                                rgbArrays[y][i][1]=rgbArrays[y1][i][1];
                                rgbArrays[y][i][2]=rgbArrays[y1][i][2];
                                break;
                            }
                            /* if one of the lines below has this palette entry then use it */
                            if (y2 < 200 && rgbUsed[y2][i] != 0) {
                                rgbUsed[y][i] = 1;
                                rgbArrays[y][i][0]=rgbArrays[y2][i][0];
                                rgbArrays[y][i][1]=rgbArrays[y2][i][1];
                                rgbArrays[y][i][2]=rgbArrays[y2][i][2];
                                break;
                            }

                        }
                    }
                    if (found == 0) {
                        if (brooks3 == 1 || brooks2 == 1) {
                            if (useimagetone == 1) break;
                            if (brooks3 == 1) greyoveride[i] = 1;
                            break;
                        }
                    }
                }
            }
        }

        if (shr256 == 1) {

            shrpalettes = 16;
            mix256 = 0;

            for (i=0;i<shrpalettes;i++) {
                for (j=0;j<16;j++) {
                    rgb256Arrays[i][j][0] = rgbArray[j][0];
                    rgb256Arrays[i][j][1] = rgbArray[j][1];
                    rgb256Arrays[i][j][2] = rgbArray[j][2];
                    rgbUsed[i][j] = 0; /* not used */
                    rgbDistance[i][j] = 0.0; /* initialize */
                }
            }


            /* first pass */
            for (i = 0, y = 0; i < 8; i++, y+=25) {
                y1 = y;
                y2 = y + 13;
                j = i * 2;
                /* temporary - hard-coded palette assignment */
                /* temporary ends */

                /* palette for 13 lines */
                for (x = y1; x < y2; x++) {
                    for (idx = 0; idx < 16; idx++) {
                        r = rgbArrays[x][idx][0];
                        g = rgbArrays[x][idx][1];
                        b = rgbArrays[x][idx][2];

                        drawcolor  = GetClosestColor(r,g,b);

                        if (rgbUsed[j][drawcolor] == 0) {
                            /* set initial value if this palette index has been used */
                            rgbUsed[j][drawcolor] = 1;
                            rgbDistance[j][drawcolor] = globaldistance;
                            rgb256Arrays[j][drawcolor][0] = r;
                            rgb256Arrays[j][drawcolor][1] = g;
                            rgb256Arrays[j][drawcolor][2] = b;
                            continue;
                        }

                        /* if the new color is closer to the currently selected palette color
                           than the previously stored color, then replace the previously stored
                           color with the new color */
                        if (globaldistance < rgbDistance[j][drawcolor]) {
                            rgbDistance[j][drawcolor] = globaldistance;
                            rgb256Arrays[j][drawcolor][0] = r;
                            rgb256Arrays[j][drawcolor][1] = g;
                            rgb256Arrays[j][drawcolor][2] = b;
                        }
                    }
                }

                for (idx = 0; idx < 16; idx++) {
                    /* put new palette back in old palette */
                    rgbArrays[y1][idx][0] = rgb256Arrays[j][idx][0];
                    rgbArrays[y1][idx][1] = rgb256Arrays[j][idx][1];
                    rgbArrays[y1][idx][2] = rgb256Arrays[j][idx][2];
                }

                /* palette for 12 lines */
                y1 = y + 13;
                y2 = y + 25;
                j = (i*2) + 1;

                for (x = y1; x < y2; x++) {
                    for (idx = 0; idx < 16; idx++) {
                        r = rgbArrays[x][idx][0];
                        g = rgbArrays[x][idx][1];
                        b = rgbArrays[x][idx][2];

                        drawcolor  = GetClosestColor(r,g,b);

                        if (rgbUsed[j][drawcolor] == 0) {
                            /* set initial value if this palette index has been used */
                            rgbUsed[j][drawcolor] = 1;
                            rgbDistance[j][drawcolor] = globaldistance;
                            rgb256Arrays[j][drawcolor][0] = r;
                            rgb256Arrays[j][drawcolor][1] = g;
                            rgb256Arrays[j][drawcolor][2] = b;
                            continue;
                        }

                        /* if the new color is closer to the currently selected palette color
                           than the previously stored color, then replace the previously stored
                           color with the new color */
                        if (globaldistance < rgbDistance[j][drawcolor]) {
                            rgbDistance[j][drawcolor] = globaldistance;
                            rgb256Arrays[j][drawcolor][0] = r;
                            rgb256Arrays[j][drawcolor][1] = g;
                            rgb256Arrays[j][drawcolor][2] = b;
                        }
                    }
                }

                for (idx = 0; idx < 16; idx++) {
                    /* put new palette back in old palette */
                    rgbArrays[y1][idx][0] = rgb256Arrays[j][idx][0];
                    rgbArrays[y1][idx][1] = rgb256Arrays[j][idx][1];
                    rgbArrays[y1][idx][2] = rgb256Arrays[j][idx][2];
                }

            }


            /* second pass */
            for (i = 0, y = 0; i < 8; i++, y+=25) {
                y1 = y;
                y2 = y + 13;
                j = i * 2;
                /* temporary - hard-coded palette assignment */
                for (idx = 0; idx < 16; idx++) {
                    /* palettes from 25 scanlines */
                    /* every 1st of 13 palettes */
                    rgb256Arrays[j][idx][0] = rgbArrays[y1][idx][0];
                    rgb256Arrays[j][idx][1] = rgbArrays[y1][idx][1];
                    rgb256Arrays[j][idx][2] = rgbArrays[y1][idx][2];
                    /* every 1st of 12 palettes */
                    rgb256Arrays[j+1][idx][0] = rgbArrays[y2][idx][0];
                    rgb256Arrays[j+1][idx][1] = rgbArrays[y2][idx][1];
                    rgb256Arrays[j+1][idx][2] = rgbArrays[y2][idx][2];
                }
                /* temporary ends */
                /* palette for 13 lines */
                for (x = y1; x < y2; x++) {
                    mypic.scb[x] = j;
                    for (idx = 0; idx < 16; idx++) {
                        rgbArrays[x][idx][0] = rgb256Arrays[j][idx][0];
                        rgbArrays[x][idx][1] = rgb256Arrays[j][idx][1];
                        rgbArrays[x][idx][2] = rgb256Arrays[j][idx][2];
                    }
                }
                /* palettte for 12 lines */
                y1 = y + 13;
                y2 = y + 25;
                j = (i*2) + 1;
                for (x = y1; x < y2; x++) {
                    mypic.scb[x] = j;
                    for (idx = 0; idx < 16; idx++) {
                        rgbArrays[x][idx][0] = rgb256Arrays[j][idx][0];
                        rgbArrays[x][idx][1] = rgb256Arrays[j][idx][1];
                        rgbArrays[x][idx][2] = rgb256Arrays[j][idx][2];
                    }
                }
            }
        } /* shr256 ends */
        else if (mix256 == 1) {

            if (quietmode == 0) puts("mixing colors");

            /* if we are also adding zone palettes to the mix then we build these here */
            /* this just gives us some additional palettes to work with */
            /* I have no clear thoughts on whether this will work better or worse */
            for (i=0;i<16;i++) {
                /* just initialize with the current palette values */
                for (j=0;j<16;j++) {
                    rgb256Arrays[i][j][0] = rgbArray[j][0];
                    rgb256Arrays[i][j][1] = rgbArray[j][1];
                    rgb256Arrays[i][j][2] = rgbArray[j][2];
                    rgbUsed[i][j] = 0; /* not used */
                    rgbDistance[i][j] = 0.0; /* initialize */
                }
            }


            /* only one pass is used - we don't use scbs for brooks */
            /* and we also need to keep the brooks palette intact */
            /* like the initial 16 - paletttes used by plain old SHR just divides the image into
               sections and builds composite palettes that more closely match our initial palettes */
            for (i = 0, y = 0; i < 8; i++, y+=25) {
                y1 = y;
                y2 = y + 13;
                j = i * 2;

                /* palette for 13 lines */
                for (x = y1; x < y2; x++) {
                    for (idx = 0; idx < 16; idx++) {
                        r = rgbArrays[x][idx][0];
                        g = rgbArrays[x][idx][1];
                        b = rgbArrays[x][idx][2];

                        drawcolor  = GetClosestColor(r,g,b);

                        if (rgbUsed[j][drawcolor] == 0) {
                            /* set initial value if this palette index has been used */
                            rgbUsed[j][drawcolor] = 1;
                            rgbDistance[j][drawcolor] = globaldistance;
                            rgb256Arrays[j][drawcolor][0] = r;
                            rgb256Arrays[j][drawcolor][1] = g;
                            rgb256Arrays[j][drawcolor][2] = b;
                            continue;
                        }

                        /* if the new color is closer to the currently selected palette color
                           than the previously stored color, then replace the previously stored
                           color with the new color */
                        if (globaldistance < rgbDistance[j][drawcolor]) {
                            rgbDistance[j][drawcolor] = globaldistance;
                            rgb256Arrays[j][drawcolor][0] = r;
                            rgb256Arrays[j][drawcolor][1] = g;
                            rgb256Arrays[j][drawcolor][2] = b;
                        }
                    }
                }


                /* palettte for 12 lines */
                y1 = y + 13;
                y2 = y + 25;
                j = (i*2) + 1;

                for (x = y1; x < y2; x++) {
                    for (idx = 0; idx < 16; idx++) {
                        r = rgbArrays[x][idx][0];
                        g = rgbArrays[x][idx][1];
                        b = rgbArrays[x][idx][2];

                        drawcolor  = GetClosestColor(r,g,b);

                        if (rgbUsed[j][drawcolor] == 0) {
                            /* set initial value if this palette index has been used */
                            rgbUsed[j][drawcolor] = 1;
                            rgbDistance[j][drawcolor] = globaldistance;
                            rgb256Arrays[j][drawcolor][0] = r;
                            rgb256Arrays[j][drawcolor][1] = g;
                            rgb256Arrays[j][drawcolor][2] = b;
                            continue;
                        }

                        /* if the new color is closer to the currently selected palette color
                           than the previously stored color, then replace the previously stored
                           color with the new color */
                        if (globaldistance < rgbDistance[j][drawcolor]) {
                            rgbDistance[j][drawcolor] = globaldistance;
                            rgb256Arrays[j][drawcolor][0] = r;
                            rgb256Arrays[j][drawcolor][1] = g;
                            rgb256Arrays[j][drawcolor][2] = b;
                        }
                    }
                }

            }

        } /* mix256 ends */

    }

    memset(&dhrbuf[0],0,32000); /* clear write buffer */

    if (dither != 0) {
        /* sizeof(sshort) * 640 */
        memset(&redDither[0],0,1280);
        memset(&greenDither[0],0,1280);
        memset(&blueDither[0],0,1280);
        memset(&redSeed[0],0,1280);
        memset(&greenSeed[0],0,1280);
        memset(&blueSeed[0],0,1280);
        memset(&redSeed2[0],0,1280);
        memset(&greenSeed2[0],0,1280);
        memset(&blueSeed2[0],0,1280);
    }
    else {
        usepalettedistance = 0;
    }

    /* seek to beginning of input file and process */
    fseek(fp,BitMapFileHeader.bfOffBits,SEEK_SET);

    if (dither == 0) puts("non-dithered output");
    else puts("dithered output");

    /* bmp's are upside-down so conversion of scanlines is in
       reverse order */
    for(y=0,y1=height-1;y<height;y++,y1--)
    {
          fread((char *)&bmpscanline[0],1,packet,fp);

          if (mono == 1) {
            /* work from a greyscale if mono - LGR and DLGR have no mono */
            /* SHR can have either mono or 16 levels of grey  in dithered or non-dithered output */
            for (x = 0,j=0;x<width;x++,j+=3) {
                if (hsl == 1) {
                    /* achromatic greyscale */
                    /* de-saturate using the hsl color model */
                    rgb2hsl(bmpscanline[j+2],bmpscanline[j+1],bmpscanline[j],&hue,&saturation,&luminance);
                    bmpscanline[j] = bmpscanline[j+1] = bmpscanline[j+2] =(uchar)(float)(luminance * 255);
                }
                else {
                    /* average RGB greyscale */
                    temp = (ushort)bmpscanline[j];
                    temp += bmpscanline[j+1];
                    temp += bmpscanline[j+2];
                    while ((temp % 3)!=0) temp++;
                    bmpscanline[j] = bmpscanline[j+1] = bmpscanline[j+2] = (uchar) (temp/3);
                }
            }
          }
          else if (shrpalettes > 1) {
             /* if doing brooks or 256 colors set the palette for this line */
             InitDoubleLineArrays(y1);
          }

          if (dither == 0) {
            /* if not dithering use direct pixel mapping */
            for (x=0,j=0;x<width;x++) {

                b = bmpscanline[j]; j++;
                g = bmpscanline[j]; j++;
                r = bmpscanline[j]; j++;

                idx = GetClosestColor(r,g,b);
                setlopixel((uchar)idx,x,y1);
            }
          }
          else
          {
              /* add the current line to the r,g,b line buffers used for dithering */
              for (j=0, x = 0; x < width; x++) {
                    b = bmpscanline[j]; j++;
                    g = bmpscanline[j]; j++;
                    r = bmpscanline[j]; j++;

                    /* values are already seeded from previous 2 - line(s) */
                    /* the idea here is to add a full value to whatever bleed values have been added */
                    AdjustShortPixel(1,(sshort *)&redDither[x],(sshort)r);
                    AdjustShortPixel(1,(sshort *)&greenDither[x],(sshort)g);
                    AdjustShortPixel(1,(sshort *)&blueDither[x],(sshort)b);
              }

              if (usepalettedistance == 1 && shrpalettes > 1) {
                  if (shrpalettes == 200) {
                    if (mix256 == 1) BrooksPicDither(y1,width);
                    else BrooksDither(y1,width);
                  }
                  else {
                    PicDither(y1,width);
                  }
              }
              else {
                /* dithering - "pixels" is just a "placeholder" */
                BuckelsDither(y1,width,1);
              }

               /* seed next line - promote nearest forward array to
                  current line */
               memcpy(&redDither[0],&redSeed[0],1280);
               memcpy(&greenDither[0],&greenSeed[0],1280);
               memcpy(&blueDither[0],&blueSeed[0],1280);

               /* seed first seed - promote furthest forward array
                  to nearest forward array */
               memcpy(&redSeed[0],&redSeed2[0],1280);
               memcpy(&greenSeed[0],&greenSeed2[0],1280);
               memcpy(&blueSeed[0],&blueSeed2[0],1280);

               /* clear last seed - furthest forward array */
               memset(&redSeed2[0],0,1280);
               memset(&greenSeed2[0],0,1280);
               memset(&blueSeed2[0],0,1280);
         }

    }
    fclose(fp);

	if (reformat == 1) {
		if (bmp3 == 0) {
			remove("Reformat.bmp");
		}
		else {
			sprintf(outfile,"%s.bm3", newname);
			remove(outfile);
			rename("Reformat.bmp",outfile);
		}
	}

    if (shr == 0) {
        /* create output files */
        /* stay with old full-screen naming convention for these */
        if (doublelores == 1) {
            if (outputwidth == 80 && outputheight == 20) sprintf(outfile,"%s.DTO", newname);
            else if (outputwidth == 80 && outputheight == 24) sprintf(outfile,"%s.DLO", newname);
            else sprintf(outfile,"%s.DLR", newname);
        }
        else {
            if (outputwidth == 40 && outputheight == 20) sprintf(outfile,"%s.STO", newname);
            else if (outputwidth == 40 && outputheight == 24) sprintf(outfile,"%s.SLO", newname);
            else sprintf(outfile,"%s.SLR", newname);
        }

        ucase((char *)&outfile[0]);

        if (longnames == 0) tags = 0;

        if (tags == 1) {
            strcat(outfile,"#060400");
            dosheader = 0;
        }

        fp = fopen(outfile,"wb");
        if (NULL == fp) {
            puts(szTextTitle);
            printf("%s cannot be created.\n", outfile);
            return INVALID;
        }

        if (dosheader == 1) {
            fl = (ushort) outputwidth;
            fl *= outputheight;
            if (outputwidth < 256) fl += 2;
            else fl += 7;
            WriteDosHeader(fp,fl,1024);
        }


        /* for most files a 2 byte header is all that is needed */
        /* these are exactly the same format as SLO,STO and DLO,DTO */
        if (outputwidth < 256) {
            fputc((uchar)outputwidth,fp);    /* width in bytes */
            fputc((uchar)outputheight,fp);   /* height in scanlines */
        }
        else {
            /* for files wider than 255 bytes */
            /* 7 byte header */
            /* 3 - bytes - some kind of identifier */
            if (doublelores == 1) fputc('D',fp);
            else fputc('S',fp);
            fputc('L',fp);
            fputc('R',fp);
            WriteDosHeader(fp,(ushort)outputwidth,(ushort)outputheight);
        }

        /* raster oriented top-down */
        for (y = 0; y < outputheight;y++) {
          offset = y * 320;
          fwrite(&dhrbuf[offset],1,outpacket,fp);
          if (doublelores == 1) fwrite(&dhrbuf[offset+160],1,outpacket,fp);
        }
        fclose(fp);
        printf("%s created.\n", outfile);

    }
    else if (shr == 320) {

        if (usepalettedistance == 1 && shrpalettes > 1) {
            /* if we shuffled the palettes based on the best total distance then we need
               to put our new palettes in place before writing our output file(s) */
            for (y= 0;y<200;y++) {
                memcpy(&rgbArrays[y][0][0],&savepalettes[y][0][0],48);
            }
            memcpy(&mypic.scb[0],&savescb[0],200);
        }


        /* SHR output file naming convention for the occasion */
        /* using different extensions to tell them apart */
        if (mono == 1) {
            if (shrgrey == 1) sprintf(outfile,"%s.SHG", newname);
            else sprintf(outfile,"%s.SHM", newname);
        }
        else {
            if (brooks == 0) sprintf(outfile,"%s.SHR", newname);
            else {
                if (shr256 == 1) sprintf(outfile,"%s.SH2", newname);
                else sprintf(outfile,"%s.SH3", newname);
            }
        }
        ucase((char *)&outfile[0]);
        if (longnames == 0) tags = 0;

        /* Ciderpress File Attribute Preservation Tags */
        if (tags == 1){
            if (brooks == 0) strcat(outfile,"#C10000");
            else {
                if (shr256 == 1) strcat(outfile,"#C10000");
                else strcat(outfile,"#C10002");
            }
        }

        if (SHR320_Output(outfile) != SUCCESS) {
            printf("%s cannot be created.\n", outfile);
            return INVALID;
        }
        printf("%s created.\n", outfile);
    }


    /* vbmp output is not available for LGR, DLGR, or SHR output because
       vbmp works only with DHGR color and monochrome and HGR monochrome.
       but I just use the same "option V" for preview output or M2S output here */
    if (vbmp == 1) {

        /* Magick2SHR palette output and Magick2SHR naming convention for round-trip editing */
        /* this applies only to 16 palette and brooks output */
        /* single palette files are output as 16-color BMPs and can be edited and reprocessed without using M2S */
        if (m2s == 1 && shrpalettes < 2) m2s = 0;

        if (m2s == 0) {
            /* for MS-DOS output we use short (and different) names */
            if (longnames == 0)sprintf(outfile,"%s.pmp", newname);
            else sprintf(outfile,"%s_Preview.bmp", newname);
        }
        else {
            if (longnames == 0)sprintf(outfile,"%s.dib", newname);
            else sprintf(outfile,"%s_palette.bmp", newname);
            /* open M2S palette file */
            fp = fopen(outfile,"wb");
            if (NULL == fp) {
                puts(szTextTitle);
                printf("%s cannot be created.\n", outfile);
                return INVALID;
            }

            memset((char *)&mybmp.bfi.bfType[0],0,sizeof(BMPHEADER));

            /* create the info header */
            mybmp.bmi.biSize = (ulong)40;
            mybmp.bmi.biWidth  = (ulong)16;
            mybmp.bmi.biHeight = (ulong)shrpalettes;
            mybmp.bmi.biPlanes = 1;
            mybmp.bmi.biBitCount = 24;
            mybmp.bmi.biCompression = (ulong) BI_RGB;

            outpacket = 48;
            mybmp.bmi.biSizeImage = (ulong)outpacket;
            mybmp.bmi.biSizeImage *= mybmp.bmi.biHeight;

            /* create the file header */
            mybmp.bfi.bfType[0] = 'B';
            mybmp.bfi.bfType[1] = 'M';
            mybmp.bfi.bfOffBits = (ulong) sizeof(BMPHEADER);
            mybmp.bfi.bfSize = mybmp.bmi.biSizeImage + mybmp.bfi.bfOffBits;

            /* write the header for the M2S palette BMP */
            fwrite((char *)&mybmp.bfi.bfType[0],sizeof(BMPHEADER),1,fp);

            memset(&bmpscanline[0],0,outpacket);

            /* write a BMP scanline for each SHR palette entry */
            for(y=0,y1=shrpalettes-1;y<shrpalettes;y++,y1--) {
                /* build a scanline of each palette entry */
                for (x = 0, j=0; x < 16; x++) {
                    if (shrpalettes == 200) {
                        /* brooks palettes */
                        bmpscanline[j] = rgbArrays[y1][x][2]; j++;
                        bmpscanline[j] = rgbArrays[y1][x][1]; j++;
                        bmpscanline[j] = rgbArrays[y1][x][0]; j++;
                    }
                    else {
                        /* pic palettes */
                        bmpscanline[j] = rgb256Arrays[y1][x][2]; j++;
                        bmpscanline[j] = rgb256Arrays[y1][x][1]; j++;
                        bmpscanline[j] = rgb256Arrays[y1][x][0]; j++;

                    }
                }
                /* write a scanline of each palette entry */
                fwrite((char *)&bmpscanline[0],1,outpacket,fp);
            }
            fclose(fp);
            printf("%s created.\n", outfile);
            /* now that the M2S palette file is written, the M2S proc file is exactly the same as the SHR preview file
               except that the naming convention follows M2S naming unless in MS-DOS */
            /* for MS-DOS output we use short names - MS-DOS users will need to rename the pmp file to a bmp file */

            if (longnames == 0)sprintf(outfile,"%s.pmp", newname);
            else sprintf(outfile,"%s_proc.bmp", newname);
        }

        fp = fopen(outfile,"wb");
        if (NULL == fp) {
            puts(szTextTitle);
            printf("%s cannot be created.\n", outfile);
            return INVALID;
        }

        /* monochrome preview is promoted to 16 color BMP format */
        if (shrpalettes == 1) {
            /* 16 color BMP preview */
            outpacket = width/2;
        }
        else {
            /* 256 color preview is promoted to */
            /* 24-bit BMP format */
            outpacket = width * 3;
        }

        if (width%2 != 0) outpacket++;
        while (outpacket%4 != 0)outpacket++;

        memset((char *)&mybmp.bfi.bfType[0],0,sizeof(BMPHEADER));

        /* create the info header */
        mybmp.bmi.biSize = (ulong)40;
        mybmp.bmi.biWidth  = (ulong)width;
        mybmp.bmi.biHeight = (ulong)height;
        mybmp.bmi.biPlanes = 1;

        if (shrpalettes == 1) mybmp.bmi.biBitCount = 4;
        else mybmp.bmi.biBitCount = 24;

        mybmp.bmi.biCompression = (ulong) BI_RGB;

        mybmp.bmi.biSizeImage = (ulong)outpacket;
        mybmp.bmi.biSizeImage *= mybmp.bmi.biHeight;

        /* create the file header */
        mybmp.bfi.bfType[0] = 'B';
        mybmp.bfi.bfType[1] = 'M';

        if (shrpalettes == 1) {
            mybmp.bfi.bfOffBits = (ulong) sizeof(BMPHEADER) + sizeof(RGBQUAD) * 16;
        }
        else {
            mybmp.bfi.bfOffBits = (ulong) sizeof(BMPHEADER);
        }

        mybmp.bfi.bfSize = mybmp.bmi.biSizeImage + mybmp.bfi.bfOffBits;

        /* write the header for the output BMP */
        fwrite((char *)&mybmp.bfi.bfType[0],sizeof(BMPHEADER),1,fp);

        if (shrpalettes == 1) {
            /* 4 bit bmp */
            for (j=0;j<16;j++) {
                sbmp[j].rgbRed   = rgbArray[j][0];
                sbmp[j].rgbGreen = rgbArray[j][1];
                sbmp[j].rgbBlue  = rgbArray[j][2];
                sbmp[j].rgbReserved = 0;
            }
            /* write the palette for the output bmp */
            fwrite((char *)&sbmp[0].rgbBlue, sizeof(RGBQUAD)*16,1,fp);
        }

        memset(&bmpscanline[0],0,outpacket);
        for(y=0,y1=height-1;y<height;y++,y1--) {
            /* build a packed scanline */
            /* this is the same as for Apple IIgs SHR mode
               providing of course we use the same palette */
            for (x = 0, j=0; x < width; x++) {
                idx = getlopixel(x,y1);
                /* range check */
                if (idx > 15)idx = 0; /* default black */

                if (shrpalettes != 1) {
                    /* brooks palettes are scanline oriented */
                    /* the 256 color palette has been copied to the brooks palette */
                    bmpscanline[j] = rgbArrays[y1][idx][2]; j++;
                    bmpscanline[j] = rgbArrays[y1][idx][1]; j++;
                    bmpscanline[j] = rgbArrays[y1][idx][0]; j++;
                }
                else {
                    if (x%2 == 0) {
                        r = (uchar)idx << 4;
                        bmpscanline[j] = r;
                    }
                    else {
                        bmpscanline[j] = r | (uchar) idx; j++;
                    }
                }
            }
            fwrite((char *)&bmpscanline[0],1,outpacket,fp);
        }
        fclose(fp);
        printf("%s created.\n", outfile);
    }


return SUCCESS;
}


/* PIM routines start here */

/* helper function for GetPCXPalettes */
sshort GetPIMPalette(int palnum, int numpalettes)
{
    FILE *fp;
    sshort status = INVALID, idx, i;
    ulong flen;


    for (;;) {
        /* open file - global filename is created in calling function */
        fp = fopen(pcxfile,"rb");
        if (fp == NULL) break;

        /* get the length of the file */
        fseek(fp, 0L, 2);
        flen = ftell(fp) - 769;

        /* avoid empty files - file must have a header and a palette */
        if (flen < (ulong)128) {
            fclose(fp);
            break;
        }

        /* read the PCX header for compatibility */
        rewind(fp);
        fread(pcxheader,66,1,fp);

        /* PCX version 5 with 8-bits per pixel, run-length encoded with one color plane */
        /* PCX version 5 with 1-bit  per pixel, run-length encoded with one color plane */
        if (pcxheader[0] != (char)10 || pcxheader[1] != (char)5) {
            /* if (pcxheader[0] != (char)10 || pcxheader[1] != (char)5 || pcxheader[2] != (char)1 || pcxheader[65] != (char)1) { */
            fclose(fp);
            puts("pcx file header problem");
            break;
        }

        /* seek to end of file and read the first sixteen palette entries */
        fseek(fp,flen,SEEK_SET);
        /* first check the palette header (it must be a formfeed character) */
        fread(pcxheader, 1, 1, fp);
        if (pcxheader[0] != (char) 12) {
            puts("pcx palette header problem");
            fclose(fp);
            break;
        }

        /* if PIM palette debugging is turned-on and 24-bit colors create duplicate 12-bit colors
           then print the details to the console */
        if (shrdupedebug == 1) clearcolorsused(); /* clear dupe count */

        /* read the palette */
        idx = (numpalettes - 1) - palnum;
        if (numpalettes == 200) {
            fread(&rgbArrays[idx][0][0],48,1,fp);
            if (shrdupedebug == 1) {
            	for (i = 0; i < 16; i++) shrcolorsused(rgbArrays[idx][i][0],rgbArrays[idx][i][1],rgbArrays[idx][i][2]);
			}
        }
        else {
            fread(&rgb256Arrays[idx][0][0],48,1,fp);
            if (shrdupedebug == 1) {
            	for (i = 0; i < 16; i++) shrcolorsused(rgb256Arrays[idx][i][0],rgb256Arrays[idx][i][1],rgb256Arrays[idx][i][2]);
			}
        }
        fclose(fp);
        status = SUCCESS;
        if (shrdupedebug == 1 && shrdupes == 1) {
			printf("24-bit palette %s has %d duplicate 12-bit color(s).\n",pcxfile,shrdupecount);
		}
        break;
    }

    return status;


}

/* iterates through SHR segments in PCX format produced by ImageMagick Convert */
/* each segment has a 16 color palette that is used for SHR conversion. The PCX Image data is ignored */
/* this is just a simple way to use the ImageMagick Convert utility to provide a palette in a format that is easy for
   me to read and use in this converter */

/* seedfile is the full pathname of one of the pcx images */
sshort GetPIMPalettes(char *seedfile)
{
    FILE *fp;
    sshort status = INVALID;
    char seedbase[256], ext[10];
    int i, j;

    imnumpalettes = 0;

    strcpy(seedbase,seedfile);

    j=999;
    for (i=0;seedbase[i]!=(char)0;i++) {
       /* support both forward and back slash for compatibility */
       if (seedbase[i] == (char)92 || seedbase[i] == (char)47) j = i+1;
    }
    if (j==999) {
        printf("%s is an invalid IM seed file name!\n",seedfile);
        return status;
    }
    seedbase[j] = 0;


    /* test for brooks mode first */
    strcpy(pcxfile,seedbase);
    strcat(pcxfile,"199.pcx");
    fp = fopen(pcxfile,"rb");
    if (NULL == fp) {
        /* if brooks test failed, test for multipalette */
        strcpy(pcxfile,seedbase);
        strcat(pcxfile,"15.pcx");
        fp = fopen(pcxfile,"rb");
        if (NULL == fp) {
            /* if multipalette test failed check for 8-palette  */
            strcpy(pcxfile,seedbase);
            strcat(pcxfile,"7.pcx");
            fp = fopen(pcxfile,"rb");
            if (NULL == fp) {
                strcpy(pcxfile,seedbase);
                strcat(pcxfile,"0.pcx");
                fp = fopen(pcxfile,"rb");
                if (NULL == fp) {
                    printf("%s is an invalid IM seed file!\n",seedfile);
                    return status;
                }
                imnumpalettes = 1;
            }
            else {
                imnumpalettes = 8;
            }
            /* black-out the palettes if less than 16 are active */
            memset(&rgb256Arrays[8][0][0],0,768);
        }
        else {
            imnumpalettes = 16;
        }
    }
    else {
        imnumpalettes = 200;
    }
    fclose(fp);
    if (quietmode == 0) shrdupedebug = 1;

    /* all the pcx files must be present and accounted for */
    for (i = 0; i < imnumpalettes; i++) {
        strcpy(pcxfile,seedbase);
        sprintf(ext,"%d.pcx",i);
        strcat(pcxfile,ext);
        status = GetPIMPalette(i,imnumpalettes);
        if (status == INVALID) {
            imnumpalettes = 0;
            printf("Unable to open IM palette %s!\n",pcxfile);
            return status;
        }
    }
    printf("%d IM Palettes successfully loaded!\n",imnumpalettes);

    return SUCCESS;
}


/* convert 256 color bmps to 24 bit bmps */
/* convert 24 bit bmps */
FILE *ReformatPIMBMP(FILE *fp)
{

    FILE *fp2;
    ushort packet,outpacket,y,x,x1;
    sshort count = 0;


    /* seek past extraneous info in header if any */
    fseek(fp,BitMapFileHeader.bfOffBits,SEEK_SET);

    if (bmi.biBitCount == 8) {
        packet = bmi.biWidth;
    }
    else {
        packet = bmi.biWidth * 3;
    }
    while ((packet % 4)!=0)packet++;


    if((fp2=fopen("Reformat.bmp","wb"))==NULL) {
        printf("Error Opening %s for writing!\n","Reformat.bmp");
        fclose(fp);
        return fp;
    }

    outpacket = WriteDIBHeader(fp2,(ushort)bmi.biWidth,(ushort)bmi.biHeight);

    if (outpacket < 1) {
        fclose(fp2);
        remove("Reformat.bmp");
        printf("Error writing header to %s!\n","Reformat.bmp");
        return fp;
    }

    for (y=0;y<bmi.biHeight;y++) {
        fread((char *)&bmpscanline[0],1,packet,fp);
        if (bmi.biBitCount == 8) {
            ReformatVGALine();
        }
        if (fourbit == 1) {
            for (x=0;x<outpacket;x++) bmpscanline[x] = gsColor(bmpscanline[x]);
        }
        fwrite((char *)&bmpscanline[0],1,outpacket,fp2);
    }
    fclose(fp2);
    fclose(fp);

    if((fp=fopen("Reformat.bmp","rb"))==NULL) {
        printf("Error Opening %s for reading!\n","Reformat.bmp");
        return fp;
    }
    /* read the header stuff into the appropriate structures */
    fread((char *)&BitMapFileHeader.bfType[0],
                 sizeof(BITMAPFILEHEADER),1,fp);
    fread((char *)&bmi.biSize,
                 sizeof(BITMAPINFOHEADER),1,fp);

    return fp;
}


/* uses ImageMagick to build the palettes */
/* todo - for raw output add functionality to use the image data from the palette pcx files */
/* also clean-up unused vars at some point */
int ConvertPIM(unsigned char *basename, unsigned char *newname)
{

    FILE *fp;
    int packet = INVALID, y,y1,y2,x,i,j,width,height,reformat = bmp3, bmpversion =0,lidx,didx,count;
    int outpacket, outputwidth, outputheight, offset;
    char bmpfile[256], outfile[256];
    uchar r,g,b,lr,lg,lb,red,green,blue,drawcolor,idx,toneindex;
    ushort temp, fl, darkest,lightest,found,unused;
    float hue,saturation,luminance;
    sshort jdx;

    sprintf(bmpfile,"%s.bmp",basename);

    if((fp=fopen(bmpfile,"rb"))==NULL) {
        puts(szTextTitle);
        printf("%s cannot be opened for input!\nIt probably does not exist or the filename was mis-spelled...\nExiting!\n");
        return INVALID;
    }

    /* read the header stuff into the appropriate structures,
       it's likely a bmp file */
    memset(&BitMapFileHeader.bfType,0,sizeof(BITMAPFILEHEADER));
    memset(&bmi.biSize,0,sizeof(BITMAPINFOHEADER));

    fread((char *)&BitMapFileHeader.bfType,
                 sizeof(BITMAPFILEHEADER),1,fp);
    fread((char *)&bmi.biSize,
                 sizeof(BITMAPINFOHEADER),1,fp);

    if (bmi.biSize != sizeof(BITMAPINFOHEADER)) {
		if (bmi.biSize == sizeof(BITMAPINFOHEADERV2)|| bmi.biSize == sizeof(BITMAPINFOHEADERV3)) {
            memset(&bmiV5.biSize,0,sizeof(BITMAPINFOHEADERV5));
            fseek(fp,sizeof(BITMAPFILEHEADER),SEEK_SET);
            fread((char *)&bmiV5.biSize,(unsigned)bmi.biSize,1,fp);
            bmpversion = 3;
            reformat = 1;
		}
		else if (bmi.biSize == sizeof(BITMAPINFOHEADERV4)) {
            memset(&bmiV5.biSize,0,sizeof(BITMAPINFOHEADERV5));
            fseek(fp,sizeof(BITMAPFILEHEADER),SEEK_SET);
            fread((char *)&bmiV5.biSize,sizeof(BITMAPINFOHEADERV4),1,fp);
            bmpversion = 4;
            reformat = 1;

        }
        else if (bmi.biSize == sizeof(BITMAPINFOHEADERV5)) {
            memset(&bmiV5.biSize,0,sizeof(BITMAPINFOHEADERV5));
            fseek(fp,sizeof(BITMAPFILEHEADER),SEEK_SET);
            fread((char *)&bmiV5.biSize,sizeof(BITMAPINFOHEADERV5),1,fp);
            bmpversion = 5;
            reformat = 1;
            /*
            Profile data refers to either the profile file name (linked profile)
            or the actual profile bits (embedded profile).
            The file format places the profile data at the end of the file.
            The profile data is placed just after the color table (if present).
            */
        }
        else {
            bmpversion = 0;
        }

    }
    else {
        bmpversion = 3;
    }

    if (bmpversion == 0) {
        fclose(fp);
        puts(szTextTitle);
        puts("BMP version not recognized!");
        return INVALID;
    }

    if (bmi.biCompression==BI_RGB &&
        BitMapFileHeader.bfType[0] == 'B' && BitMapFileHeader.bfType[1] == 'M' &&
        bmi.biPlanes==1 && (bmi.biBitCount == 24 || bmi.biBitCount == 8)) {

        if (bmi.biWidth != 320 || bmi.biHeight != 200) {
            fclose(fp);
            puts(szTextTitle);
            printf("%s is not in a supported size for SHR file output.\nExiting!\n", bmpfile);
            return INVALID;
        }

            outputheight = height = (int) bmi.biHeight;
            outputwidth = width = (int) bmi.biWidth;

            if (outputheight%2 != 0) outputheight++;
            outputheight = outputheight/2;

            outpacket = outputwidth;

            packet = width * 3;
            while (packet%4 != 0)packet++; /* BMP scanlines align on DWORD boundaries */
    }

    if (packet == INVALID) {
        fclose(fp);
        puts(szTextTitle);
        printf("%s is not in a supported format for SHR or BROOKS file output.\nExiting!\n", bmpfile);
        if (bmi.biCompression==BI_RGB) puts("compression ok");
        return INVALID;
    }

    if (bmi.biBitCount == 8) {
       fread((char *)&sbmp[0].rgbBlue, sizeof(RGBQUAD)*256,1,fp);
       reformat = 1;
    }

    if (reformat == 1) {
       fp = ReformatPIMBMP(fp);
       if (NULL == fp) return SUCCESS;
    }

    if (imnumpalettes != 200) shrpalettes = 16;
    else shrpalettes = 200;
    shr = 320;

    /* errorsum = 1; error summing is not forced but maybe it should be */

    /* initialize nearest color arrays */
    /* todo - probably not needed because setting nearest color arrays on a line by line basis */
    InitDoubleArrays();

    if (imnumpalettes == 16) {

        /* populate 200 palettes from 16-palettes */
        /* and set initial scbs */
        /* this allows most of the same line oriented routines to be re-used for either
           16-palette or 200-palette processing */
        shr256 = 1;
        for (i = 0, y = 0; i < 8; i++, y+=25) {
            y1 = y;
            y2 = y + 13;
            j = i * 2;
            /* palette for 13 lines */
            for (x = y1; x < y2; x++) {
                mypic.scb[x] = j;
                for (idx = 0; idx < 16; idx++) {
                    rgbArrays[x][idx][0] = rgb256Arrays[j][idx][0];
                    rgbArrays[x][idx][1] = rgb256Arrays[j][idx][1];
                    rgbArrays[x][idx][2] = rgb256Arrays[j][idx][2];
                }
            }
            /* palettte for 12 lines */
            y1 = y + 13;
            y2 = y + 25;
            j = (i*2) + 1;
            for (x = y1; x < y2; x++) {
                mypic.scb[x] = j;
                for (idx = 0; idx < 16; idx++) {
                    rgbArrays[x][idx][0] = rgb256Arrays[j][idx][0];
                    rgbArrays[x][idx][1] = rgb256Arrays[j][idx][1];
                    rgbArrays[x][idx][2] = rgb256Arrays[j][idx][2];
                }
            }
        }
    }
    else if (imnumpalettes == 8) {
        /* populate 200 palettes from 8-palettes */
        /* and set initial scbs */
        /* this allows most of the same line oriented routines to be re-used for either
           8-palette, 16-palette or 200-palette processing */
        shr256 = 1;
        for (i = 0, y = 0; i < 8; i++, y+=25) {
            y1 = y;
            y2 = y + 13;
            /* palette for 13 lines */
            for (x = y1; x < y2; x++) {
                mypic.scb[x] = i;
                for (idx = 0; idx < 16; idx++) {
                    rgbArrays[x][idx][0] = rgb256Arrays[i][idx][0];
                    rgbArrays[x][idx][1] = rgb256Arrays[i][idx][1];
                    rgbArrays[x][idx][2] = rgb256Arrays[i][idx][2];
                }
            }
            /* same palettte for 12 lines */
            y1 = y + 13;
            y2 = y + 25;
            for (x = y1; x < y2; x++) {
                mypic.scb[x] = i;
                for (idx = 0; idx < 16; idx++) {
                    rgbArrays[x][idx][0] = rgb256Arrays[i][idx][0];
                    rgbArrays[x][idx][1] = rgb256Arrays[i][idx][1];
                    rgbArrays[x][idx][2] = rgb256Arrays[i][idx][2];
                }
            }
        }
    }
    else if (imnumpalettes == 1) {
        for (x=0;x<200;x++) {
            shr256 = 1;
            mypic.scb[x] = 0;
            for (idx = 0; idx < 16; idx++) {
                rgbArrays[x][idx][0] = rgb256Arrays[0][idx][0];
                rgbArrays[x][idx][1] = rgb256Arrays[0][idx][1];
                rgbArrays[x][idx][2] = rgb256Arrays[0][idx][2];
            }
        }
    }
    else {
        shr256 = 0;
    }
    /* shr256 ends */


    memset(&dhrbuf[0],0,32000); /* clear write buffer */

    if (dither != 0) {
        usepalettedistance = 1;
        /* sizeof(sshort) * 640 */
        memset(&redDither[0],0,1280);
        memset(&greenDither[0],0,1280);
        memset(&blueDither[0],0,1280);
        memset(&redSeed[0],0,1280);
        memset(&greenSeed[0],0,1280);
        memset(&blueSeed[0],0,1280);
        memset(&redSeed2[0],0,1280);
        memset(&greenSeed2[0],0,1280);
        memset(&blueSeed2[0],0,1280);
    }
    else {
        usepalettedistance = 0;
    }

    /* seek to beginning of input file and process */
    fseek(fp,BitMapFileHeader.bfOffBits,SEEK_SET);

    if (dither == 0) puts("non-dithered output");
    else puts("dithered output");

    /* bmp's are upside-down so conversion of scanlines is in
       reverse order */
    for(y=0,y1=height-1;y<height;y++,y1--)
    {
          fread((char *)&bmpscanline[0],1,packet,fp);

          /* set the palette for this line */
          /* todo - probably not needed for dithered output */
          InitDoubleLineArrays(y1);

          if (dither == 0) {
            /* if not dithering use direct pixel mapping */
            for (x=0,j=0;x<width;x++) {

                b = bmpscanline[j]; j++;
                g = bmpscanline[j]; j++;
                r = bmpscanline[j]; j++;

                idx = GetClosestColor(r,g,b);
                setlopixel((uchar)idx,x,y1);
            }
          }
          else
          {
              /* add the current line to the r,g,b line buffers used for dithering */
              for (j=0, x = 0; x < width; x++) {
                    b = bmpscanline[j]; j++;
                    g = bmpscanline[j]; j++;
                    r = bmpscanline[j]; j++;

                    /* values are already seeded from previous 2 - line(s) */
                    /* the idea here is to add a full value to whatever bleed values have been added */
                    AdjustShortPixel(1,(sshort *)&redDither[x],(sshort)r);
                    AdjustShortPixel(1,(sshort *)&greenDither[x],(sshort)g);
                    AdjustShortPixel(1,(sshort *)&blueDither[x],(sshort)b);
              }


              if (shrpalettes == 200) BrooksDither(y1,width);
              else PicDither(y1,width);

               /* seed next line - promote nearest forward array to
                  current line */
               memcpy(&redDither[0],&redSeed[0],1280);
               memcpy(&greenDither[0],&greenSeed[0],1280);
               memcpy(&blueDither[0],&blueSeed[0],1280);

               /* seed first seed - promote furthest forward array
                  to nearest forward array */
               memcpy(&redSeed[0],&redSeed2[0],1280);
               memcpy(&greenSeed[0],&greenSeed2[0],1280);
               memcpy(&blueSeed[0],&blueSeed2[0],1280);

               /* clear last seed - furthest forward array */
               memset(&redSeed2[0],0,1280);
               memset(&greenSeed2[0],0,1280);
               memset(&blueSeed2[0],0,1280);
         }

    }
    fclose(fp);

    if (reformat == 1) {
		if (bmp3 == 0) {
			remove("Reformat.bmp");
		}
		else {
			sprintf(outfile,"%s.bm3", newname);
			remove(outfile);
			rename("Reformat.bmp",outfile);
		}
	}

    if (usepalettedistance == 1) {
        /* if we shuffled the palettes based on the best total distance then we need
           to put our new palettes in place before writing our output file(s) */
        for (y= 0;y<200;y++) {
            memcpy(&rgbArrays[y][0][0],&savepalettes[y][0][0],48);
        }
        memcpy(&mypic.scb[0],&savescb[0],200);
    }


    if (shr256 == 1) {
        /* for SHR motion video prototyping */
        /* uses only 8 palettes to facilitate slide transition */

        if (imnumpalettes == 1) sprintf(outfile,"%s.SHR", newname);
        else if (imnumpalettes == 8) sprintf(outfile,"%s.SMV", newname);
        else sprintf(outfile,"%s.SH2", newname);
    }
    else {
        sprintf(outfile,"%s.SH3", newname);
    }
    ucase((char *)&outfile[0]);
    if (longnames == 0) tags = 0;

    /* Ciderpress File Attribute Preservation Tags */
    if (tags == 1){
        if (shr256 == 1) strcat(outfile,"#C10000");
        else strcat(outfile,"#C10002");
    }

    if (SHR320_Output(outfile) != SUCCESS) {
        puts(szTextTitle);
        printf("%s cannot be created.\n", outfile);
        return INVALID;
    }
    printf("%s created.\n", outfile);

    /* "option V" for preview output or M2S output here */
    if (vbmp == 1) {

        if (m2s == 0) {
            /* for MS-DOS output we use short (and different) names */
            if (longnames == 0)sprintf(outfile,"%s.pmp", newname);
            else sprintf(outfile,"%s_Preview.bmp", newname);
        }
        else {
            /* Magick2SHR palette output and Magick2SHR naming convention for round-trip editing */
            if (longnames == 0)sprintf(outfile,"%s.dib", newname);
            else sprintf(outfile,"%s_palette.bmp", newname);
            /* open M2S palette file */
            fp = fopen(outfile,"wb");
            if (NULL == fp) {
                puts(szTextTitle);
                printf("%s cannot be created.\n", outfile);
                return INVALID;
            }

            memset((char *)&mybmp.bfi.bfType[0],0,sizeof(BMPHEADER));

            /* create the info header */
            mybmp.bmi.biSize = (ulong)40;
            mybmp.bmi.biWidth  = (ulong)16;
            mybmp.bmi.biHeight = (ulong)shrpalettes;
            mybmp.bmi.biPlanes = 1;
            mybmp.bmi.biBitCount = 24;
            mybmp.bmi.biCompression = (ulong) BI_RGB;

            outpacket = 48;
            mybmp.bmi.biSizeImage = (ulong)outpacket;
            mybmp.bmi.biSizeImage *= mybmp.bmi.biHeight;

            /* create the file header */
            mybmp.bfi.bfType[0] = 'B';
            mybmp.bfi.bfType[1] = 'M';
            mybmp.bfi.bfOffBits = (ulong) sizeof(BMPHEADER);
            mybmp.bfi.bfSize = mybmp.bmi.biSizeImage + mybmp.bfi.bfOffBits;

            /* write the header for the M2S palette BMP */
            fwrite((char *)&mybmp.bfi.bfType[0],sizeof(BMPHEADER),1,fp);

            memset(&bmpscanline[0],0,outpacket);

            /* write a BMP scanline for each SHR palette entry */
            for(y=0,y1=shrpalettes-1;y<shrpalettes;y++,y1--) {
                /* build a scanline of each palette entry */
                for (x = 0, j=0; x < 16; x++) {
                    if (shrpalettes == 200) {
                        /* brooks palettes */
                        bmpscanline[j] = rgbArrays[y1][x][2]; j++;
                        bmpscanline[j] = rgbArrays[y1][x][1]; j++;
                        bmpscanline[j] = rgbArrays[y1][x][0]; j++;
                    }
                    else {
                        /* pic palettes */
                        bmpscanline[j] = rgb256Arrays[y1][x][2]; j++;
                        bmpscanline[j] = rgb256Arrays[y1][x][1]; j++;
                        bmpscanline[j] = rgb256Arrays[y1][x][0]; j++;

                    }
                }
                /* write a scanline of each palette entry */
                fwrite((char *)&bmpscanline[0],1,outpacket,fp);
            }
            fclose(fp);
            printf("%s created.\n", outfile);
            /* now that the M2S palette file is written, the M2S proc file is exactly the same as the SHR preview file
               except that the naming convention follows M2S naming unless in MS-DOS */
            /* for MS-DOS output we use short names - MS-DOS users will need to rename the pmp file to a bmp file */

            if (longnames == 0)sprintf(outfile,"%s.pmp", newname);
            else sprintf(outfile,"%s_proc.bmp", newname);
        }

        fp = fopen(outfile,"wb");
        if (NULL == fp) {
            puts(szTextTitle);
            printf("%s cannot be created.\n", outfile);
            return INVALID;
        }

        /* 256 color preview is promoted to */
        /* 24-bit BMP format */
        outpacket = width * 3;

        if (width%2 != 0) outpacket++;
        while (outpacket%4 != 0)outpacket++;

        memset((char *)&mybmp.bfi.bfType[0],0,sizeof(BMPHEADER));

        /* create the info header */
        mybmp.bmi.biSize = (ulong)40;
        mybmp.bmi.biWidth  = (ulong)width;
        mybmp.bmi.biHeight = (ulong)height;
        mybmp.bmi.biPlanes = 1;
        mybmp.bmi.biBitCount = 24;
        mybmp.bmi.biCompression = (ulong) BI_RGB;
        mybmp.bmi.biSizeImage = (ulong)outpacket;
        mybmp.bmi.biSizeImage *= mybmp.bmi.biHeight;

        /* create the file header */
        mybmp.bfi.bfType[0] = 'B';
        mybmp.bfi.bfType[1] = 'M';
        mybmp.bfi.bfOffBits = (ulong) sizeof(BMPHEADER);
        mybmp.bfi.bfSize = mybmp.bmi.biSizeImage + mybmp.bfi.bfOffBits;

        /* write the header for the output BMP */
        fwrite((char *)&mybmp.bfi.bfType[0],sizeof(BMPHEADER),1,fp);

        memset(&bmpscanline[0],0,outpacket);
        for(y=0,y1=height-1;y<height;y++,y1--) {
            for (x = 0, j=0; x < width; x++) {
                idx = getlopixel(x,y1);
                /* range check */
                if (idx > 15)idx = 0; /* default black */
                /* brooks palettes are scanline oriented */
                /* the 256 color palette has been copied to the brooks palette */
                bmpscanline[j] = rgbArrays[y1][idx][2]; j++;
                bmpscanline[j] = rgbArrays[y1][idx][1]; j++;
                bmpscanline[j] = rgbArrays[y1][idx][0]; j++;
            }
            fwrite((char *)&bmpscanline[0],1,outpacket,fp);
        }
        fclose(fp);
        printf("%s created.\n", outfile);
    }


return SUCCESS;
}


/* uses ImageMagick to build the palettes */
/* clean-up unused vars at some point */
int Convertfourbit(unsigned char *basename, unsigned char *newname)
{

    FILE *fp;
    int packet = INVALID, bmpversion =0;
    char bmpfile[256], outfile[256];

    sprintf(bmpfile,"%s.bmp",basename);
    sprintf(outfile,"%s.4B",newname);

    if((fp=fopen(bmpfile,"rb"))==NULL) {
        puts(szTextTitle);
        printf("%s cannot be opened for input!\nIt probably does not exist or the filename was mis-spelled...\nExiting!\n");
        return INVALID;
    }

    /* read the header stuff into the appropriate structures,
       it's likely a bmp file */
    memset(&BitMapFileHeader.bfType,0,sizeof(BITMAPFILEHEADER));
    memset(&bmi.biSize,0,sizeof(BITMAPINFOHEADER));

    fread((char *)&BitMapFileHeader.bfType,
                 sizeof(BITMAPFILEHEADER),1,fp);
    fread((char *)&bmi.biSize,
                 sizeof(BITMAPINFOHEADER),1,fp);

    if (bmi.biSize != sizeof(BITMAPINFOHEADER)) {
		if (bmi.biSize == sizeof(BITMAPINFOHEADERV2)|| bmi.biSize == sizeof(BITMAPINFOHEADERV3)) {
            memset(&bmiV5.biSize,0,sizeof(BITMAPINFOHEADERV5));
            fseek(fp,sizeof(BITMAPFILEHEADER),SEEK_SET);
            fread((char *)&bmiV5.biSize,(unsigned)bmi.biSize,1,fp);
            bmpversion = 3;
		}
		else if (bmi.biSize == sizeof(BITMAPINFOHEADERV4)) {
            memset(&bmiV5.biSize,0,sizeof(BITMAPINFOHEADERV5));
            fseek(fp,sizeof(BITMAPFILEHEADER),SEEK_SET);
            fread((char *)&bmiV5.biSize,sizeof(BITMAPINFOHEADERV4),1,fp);
            bmpversion = 4;
        }
        else if (bmi.biSize == sizeof(BITMAPINFOHEADERV5)) {
            memset(&bmiV5.biSize,0,sizeof(BITMAPINFOHEADERV5));
            fseek(fp,sizeof(BITMAPFILEHEADER),SEEK_SET);
            fread((char *)&bmiV5.biSize,sizeof(BITMAPINFOHEADERV5),1,fp);
            bmpversion = 5;
            /*
            Profile data refers to either the profile file name (linked profile)
            or the actual profile bits (embedded profile).
            The file format places the profile data at the end of the file.
            The profile data is placed just after the color table (if present).
            */
        }
        else {
            bmpversion = 0;
        }

    }
    else {
        bmpversion = 3;
    }

    if (bmpversion == 0) {
        fclose(fp);
        puts(szTextTitle);
        puts("BMP version not recognized!");
        return INVALID;
    }

    if (bmi.biCompression==BI_RGB &&
        BitMapFileHeader.bfType[0] == 'B' && BitMapFileHeader.bfType[1] == 'M' &&
        bmi.biPlanes==1 && (bmi.biBitCount == 24 || bmi.biBitCount == 8)) {

        if (bmi.biWidth != 320 || bmi.biHeight != 200) {
            fclose(fp);
            puts(szTextTitle);
            printf("%s is not in a supported size for 4-bit file output.\nExiting!\n", bmpfile);
            return INVALID;
        }

        packet = SUCCESS;
    }

    if (packet == INVALID) {
        fclose(fp);
        puts(szTextTitle);
        printf("%s is not in a supported format for 4-bit file output.\nExiting!\n", bmpfile);
        return INVALID;
    }

    if (bmi.biBitCount == 8) {
       fread((char *)&sbmp[0].rgbBlue, sizeof(RGBQUAD)*256,1,fp);
    }

    fp = ReformatPIMBMP(fp);
    if (NULL == fp) return INVALID;
    fclose(fp);

    remove(outfile);
    rename("Reformat.bmp",outfile);
    printf("%s created.\n", outfile);

return SUCCESS;
}


/* Command Line Options Helper Function */
int cmpstr(char *str, char *cmp)
{
    int i;

    if (strlen(cmp) != strlen(str)) return INVALID;
    for (i=0;str[i] != 0;i++) {
        if (toupper(cmp[i]) != toupper(str[i])) return INVALID;
    }
    return SUCCESS;
}


/* raw SHR structures */
/* FileType $C1 AuxType $0000 - mode320 and mode640 */
#ifdef MINGW
typedef struct __attribute__((__packed__)) tagINPIC
#else
typedef struct tagINPIC
#endif
{
    uchar line[200][160]; /* 32000 bytes */
    uchar scb[200];
    uchar padding[56];
    uchar pal[16][32];
} INPIC;

/* FileType $C1 AuxType $0002 - mode3200 */
#ifdef MINGW
typedef struct __attribute__((__packed__)) tagINBROOKS
#else
typedef struct tagINBROOKS
#endif
{
    uchar line[200][160];
    uchar pal[200][32]; /* $0RGB table buffer palette entries 0-16 reversed */
} INBROOKS;

INPIC *p16 = NULL;
INBROOKS *p200 = NULL;
uchar *shrbuf = NULL;



sshort getshrindex(int x, int y)
{
     uchar *crt, color;
     int offset;

     offset = x / 2;

     /* each SHR scanline is offset by 160 bytes  */
     offset += (y * 160);
     crt = (unsigned char *)&shrbuf[offset];

     if (x%2 == 0) {
         /* even pixels */
         color = crt[0] >> 4;
     }
     else {
         /* odd pixels */
         color = crt[0] &0xf;
     }

     return (sshort)color;
}



/* the following provides some equivalent functionality for SHR as
   what A2B provides for producing BMP files from DHGR files.

   SHR Input files must follow one of our naming conventions.
   limited to mode320 and mode3200 PIC files.
*/

/* converts SHR images to imagedata and palette file pairs in BMP format */
/* provides a 320 x 200 x 24-bit BMP file for editing stray pixels etc.in converted output */
/* can also be used to convert SHR files produced by other applications to BMP format */

/* these have lost their original 24-bit precision and should not be re-transformed */
/* m2s should be used to convert edited files back to PIC or BROOKS files */
int shrtom2s(char *infile, char *basename)
{
    FILE *fp;
    char procfile[256], palfile[256];
    ulong flen;
    int brooks_format = 0;
    uchar buf[32],r,g,b;
    ushort *tmppal = (ushort *)&buf[0], *brookspal;
    sshort x,y,y1,i,j,k;

    /* use the file length to determine what kind of output we
       will provide */

    fp = fopen(infile,"rb");

    if (fp == NULL) {
        printf("Could not open SHR file %s\n",infile);
        return 1;
    }

    /* get the length of the file */
    fseek(fp, 0L, 2);
    flen = ftell(fp);

    if (flen == (ulong) sizeof(INPIC)) {
        /* 16 palettes = 32768 bytes */
        shrbuf = (uchar *) malloc(sizeof(INPIC));
    }
    else if (flen == (ulong) sizeof(INBROOKS)) {
        /* 200 palettes = 38400 bytes */
        brooks_format = 1;
        shrbuf = (uchar *) malloc(sizeof(INBROOKS));
    }
    else {
        fclose(fp);
        printf("%s is not a supported 320 x 200 SHR file\n",infile);
        return 1;
    }

    if (NULL == shrbuf) {
        puts("Not enough memory.");
        return 1;
    }

    rewind(fp);
    /* read scanlines */
    fread((uchar *)&shrbuf[0],sizeof(uchar),32000,fp);


    /* read palettes */
    if (brooks_format == 1) {
        fread((uchar *)&shrbuf[32000],sizeof(uchar),6400,fp);
        fclose(fp);
        /* convert 12-bit brooks palettes to 24-bit BMP palettes */
        p200 = (INBROOKS *)&shrbuf[0];
        for (y=0;y<200;y++) {
            brookspal = (ushort *)&p200[0].pal[y][0];
            /* flip the brooks palette end for end */
            for (i=0,k=15; i < 16; i++,k--) tmppal[i] = brookspal[k];
            /* place decoded 12 bit colors into 24-bit color array */
            /* the m2s BMP palette file for a BROOKS image contains all 200 palettes
               (or less)
            */
            for (i=0,k=0;i< 16;i++,k+=2) {
                /* decode $0RGB */

                g = b = buf[k];
                r = buf[k+1];

                r = (r &0xf);
                g = (g >> 4);
                b = (b &0xf);

                /* do not truncate the low nibble, use 24-bits */
                rgbArrays[y][i][0] = (uchar) (r << 4 | r);
                rgbArrays[y][i][1] = (uchar) (g << 4 | g);
                rgbArrays[y][i][2] = (uchar) (b << 4 | b);
            }
        }

    }
    else {
        fread((uchar *)&shrbuf[32000],sizeof(uchar),768,fp);
        fclose(fp);
        /* convert 12-bit pic palettes into 24-bit BMP palettes */
        p16 = (INPIC *)&shrbuf[0];

       /* encode into 16 palettes first
          this is needed for the m2s BMP palette file for a PIC */
        /* the m2s palette file for a PIC file contains 16 palettes
           (or less)
        */
        for (j=0;j<16;j++) {
            /* palette colors 0 - 16 */
            for (i=0,k=0;i< 16;i++,k+=2) {
                /* decode $0RGB */
                g = b = p16[0].pal[j][k];
                r = p16[0].pal[j][k+1];

                r = (r &0xf);
                g = (g >> 4);
                b = (b &0xf);

                /* place decoded 12 bit colors into 24-bit color array */
                rgb256Arrays[j][i][0] = (uchar) (r << 4 | r);
                rgb256Arrays[j][i][1] = (uchar) (g << 4 | g);
                rgb256Arrays[j][i][2] = (uchar) (b << 4 | b);
            }

        }

        /* copy the corresponding palette to the 200 line output array.
           this will be used to transform image data from the PIC file
           to 24-bit colors for the m2s BMP proc file */
        for (y=0;y<200;y++) {
            j = (sshort) p16[0].scb[y];
            /* palette colors 0 - 16 */
            for (i=0;i< 16;i++) {
                rgbArrays[y][i][0] = rgb256Arrays[j][i][0];
                rgbArrays[y][i][1] = rgb256Arrays[j][i][1];
                rgbArrays[y][i][2] = rgb256Arrays[j][i][2];
            }
        }


    }

    /* make output file names from basename */
    sprintf(procfile,"%s_proc.bmp",basename);
    sprintf(palfile,"%s_palette.bmp",basename);

    /* write image data in BMP format */
    fp = fopen(procfile,"wb");

    if (fp == NULL) {
        printf("Could not open %s for writing.\n",procfile);
        free(shrbuf);
        return 1;
    }

    clearcolorsused();
    memset((char *)&mybmp.bfi.bfType[0],0,sizeof(BMPHEADER));

    /* create the info header */
    mybmp.bmi.biSize = (ulong)40;
    mybmp.bmi.biWidth  = (ulong)320;
    mybmp.bmi.biHeight = (ulong)200;
    mybmp.bmi.biPlanes = 1;
    mybmp.bmi.biBitCount = 24;
    mybmp.bmi.biCompression = (ulong) BI_RGB;
    mybmp.bmi.biSizeImage = (ulong)960;
    mybmp.bmi.biSizeImage *= mybmp.bmi.biHeight;

    /* create the file header */
    mybmp.bfi.bfType[0] = 'B';
    mybmp.bfi.bfType[1] = 'M';
    mybmp.bfi.bfOffBits = (ulong) sizeof(BMPHEADER);
    mybmp.bfi.bfSize = mybmp.bmi.biSizeImage + mybmp.bfi.bfOffBits;

    /* write the header for the output BMP */
    fwrite((char *)&mybmp.bfi.bfType[0],sizeof(BMPHEADER),1,fp);

    memset(&bmpscanline[0],0,960);
    for(y=0,y1=199;y<200;y++,y1--) {
        for (x = 0, j=0; x < 320; x++) {
            k = getshrindex(x,y1);
            /* range check */
            if (k > 15)k = 0; /* default black */
            b = bmpscanline[j] = rgbArrays[y1][k][2]; j++;
            g = bmpscanline[j] = rgbArrays[y1][k][1]; j++;
            r = bmpscanline[j] = rgbArrays[y1][k][0]; j++;
            shrcolorsused(r,g,b);

        }
        fwrite((char *)&bmpscanline[0],1,960,fp);
    }
    fclose(fp);
    printf("%s created.\n",procfile);

    /* write palette data in BMP format */
    fp = fopen(palfile,"wb");

    if (fp == NULL) {
        printf("Could not open %s for writing.\n",palfile);
        free(shrbuf);
        return 1;
    }

    memset((char *)&mybmp.bfi.bfType[0],0,sizeof(BMPHEADER));

    /* create the info header */
    mybmp.bmi.biSize = (ulong)40;
    mybmp.bmi.biWidth  = (ulong)16;

    if (brooks_format == 0)
        mybmp.bmi.biHeight = (ulong)16;
    else
        mybmp.bmi.biHeight = (ulong)200;

    mybmp.bmi.biPlanes = 1;
    mybmp.bmi.biBitCount = 24;
    mybmp.bmi.biCompression = (ulong) BI_RGB;
    mybmp.bmi.biSizeImage = (ulong)48;
    mybmp.bmi.biSizeImage *= mybmp.bmi.biHeight;

    /* create the file header */
    mybmp.bfi.bfType[0] = 'B';
    mybmp.bfi.bfType[1] = 'M';
    mybmp.bfi.bfOffBits = (ulong) sizeof(BMPHEADER);
    mybmp.bfi.bfSize = mybmp.bmi.biSizeImage + mybmp.bfi.bfOffBits;

    /* write the header for the output BMP */
    fwrite((char *)&mybmp.bfi.bfType[0],sizeof(BMPHEADER),1,fp);

    memset(&bmpscanline[0],0,48);

    if (brooks_format == 0) {
        for(y=0,y1=15;y<16;y++,y1--) {
            for (x = 0, j=0; x < 16; x++) {
                bmpscanline[j] = rgb256Arrays[y1][x][2]; j++;
                bmpscanline[j] = rgb256Arrays[y1][x][1]; j++;
                bmpscanline[j] = rgb256Arrays[y1][x][0]; j++;
            }
            fwrite((char *)&bmpscanline[0],1,48,fp);
        }
    }
    else {
        for(y=0,y1=199;y<200;y++,y1--) {
            for (x = 0, j=0; x < 16; x++) {
                bmpscanline[j] = rgbArrays[y1][x][2]; j++;
                bmpscanline[j] = rgbArrays[y1][x][1]; j++;
                bmpscanline[j] = rgbArrays[y1][x][0]; j++;
            }
            fwrite((char *)&bmpscanline[0],1,48,fp);
        }
    }

    fclose(fp);
    printf("%s created.\n",palfile);


    free(shrbuf);
    puts("Done!");
    printf("%d unique SHR palette colors in image.\n",shrcolorcount);

    return 0;

}


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


int main(int argc, char **argv)
{

  int status = 0, idx, jdx, pset;
  char fname[256],sname[256],outfile[256], c, d, e, f;
  uchar *wordptr;
  FILE *fp;

  fname[0] = sname[0] = outfile[0] = ASCIIZ;

  for (idx = 0; idx < 16; idx++) {
      desaturate[idx] = 100.0;
  }

  setluma();

#ifdef MSDOS
  longnames = 0;
  system("cls");
#endif

  if(argc == 1) {
    puts(szTextTitle);
    puts("Command line Usage:     a2b filenames options");
    puts("                       \"a2b MyAppleWinDHiresMonoScreenCapture.bmp\"");
    puts("                   or  \"a2b MyDHires.2FC\" or \"a2b MyDHires.A2FC\"");
    puts("                   or  \"a2b MyDHires.dhr\" or \"a2b MyDHires.dhgr\"");
    puts("                   or  \"a2b MyDHires.BIN\" or \"a2b MyDHires.AUX\"");
    puts("If converting .BIN and .AUX file pairs, both must be present.");
    puts("Default is automatic naming. Optional different output filename.");
    puts("                       \"a2b MyDHires.2FC OutfileBaseName\"");
    puts("Option P - Palettes p0-p16");
    puts("Output: 280 x 192 x 24 Bit Windows .BMP File - Default");
    puts("        140 x 192 x 24 Bit Windows .BMP File - Option 140");
    puts("        560 x 384 x Monochrome Windows .BMP File - Option 384");
    puts("        560 x 192 x Monochrome Windows .BMP File - Option 192");
    puts("For additional options read the documentation and source code.");
    puts("Additional output includes Apple II DHGR, LGR and DLGR, and SHR files.");
    puts("Additional output also includes VBMP files (or Previews) and Image Fragments.");
    puts("Additional output also includes error-diffusion dithering.");
    printf("Enter Input FileName (Blank to Exit): ");
    gets(fname);
    if (fname[0] == ASCIIZ)
      return 1;
    printf("Enter Output FileBaseName (Blank for None) : ");
    gets(outfile);
  }
  else {
    strcpy(fname, argv[1]);
    /* getopts */
    if (argc > 2) {
        for (idx = 2; idx < argc; idx++) {
            /* switch character is optional */
            wordptr = (uchar *)&argv[idx][0];
            c = toupper(wordptr[0]);
            if (c == '-') {
                wordptr = (uchar *)&argv[idx][1];
                c = toupper(wordptr[0]);
            }

            d = e = f = toupper(wordptr[1]);
            if (d != (char)0) e = f = toupper(wordptr[2]);
            if (e != (char)0) f = toupper(wordptr[3]);

            /* a43b or a43s */
            if (c == 'A' && d == '4' && e == '3' && (f == 'B' || f == 'S')) {
				/* set ImageMagick geometry for gravity option */
				/* by default, cropping is centered */
				SetMagickAspectScript((char *)&wordptr[4]);

			   /* create cropping scripts for ImageMagick convert from ImageMagick identify output */
			   /* batch file */
			   if (f == 'B') {
				   return MakeMagickAspectScript(fname,0,0);
			   }
			   /* shell script */
			   if (f == 'S') {
				   return MakeMagickAspectScript(fname,1,0);
			   }
			}
			/* p43b or p43s */
			if (c == 'P' && d == '4') {
			   /* create padding scripts for ImageMagick convert from ImageMagick identify output */
			   /* batch file */
			   if (cmpstr(wordptr,"p43b") == SUCCESS) {
				   return MakeMagickAspectScript(fname,0,1);
			   }
			   /* shell script */
			   if (cmpstr(wordptr,"p43s") == SUCCESS) {
				   return MakeMagickAspectScript(fname,1,1);
			   }
			}

            if (c== 'B' && d == 'M') {
				/* produce a 24-bit Windows 3.1 compatible BMP file of the input file */
				/* this is primarily for compatibility with SuperConvert for running SHR comparisons */
				if (cmpstr(wordptr,"bmp3") == SUCCESS) {
					bmp3 = 1;
					continue;
				}
			}

            if (c == '4' && d != (char)0) {
                /* SHR output */
                /* settings to support reducing palettes to 4-bit depth */
                /* the threshold settings are currently fixed for this setting but some future version might
                   include changing these to mask out midtones or whatever - leave as-is for now */
                if (cmpstr(wordptr,"4bit") == SUCCESS) {
                    /* creates a copy of the 320 x 200 input file using 12-bit legal GS colors */
                    /* this file is then processed into segmented palettes using ImageMagick */
                    /* it can also be processed into a 16-color external palette for Image Tone rendering */
                    fourbit = 1;
                    continue;
                }
                if (cmpstr(wordptr,"4play") == SUCCESS) {
                    /* this setting affects only the colors that are in the orginal image and only for the
                       purposes of building palettes... */
                    fourplay = 1;
                    continue;
                }
                if (cmpstr(wordptr,"4pal") == SUCCESS) {
                    /* use this setting just before we start the building of the palettes */
                    /* this does not apply to segmented external palettes */
                    fourpal = 1;
                    continue;
                }
            }

            if (c == 'Q' && d == (char)0) {
                /* suppress additional output unless quiet mode is disabled - additional output may be useful to some including me */
                /* note also - windows and MS-DOS users should re-direct to a NUL device in batch (cmd) files
                   to avoid slowing-down processing with extraneous console output */
                quietmode = 0;
                continue;
            }

            if (c == 'S' && d != (char)0) {
                /* "lossless" dithering */
                if (cmpstr(wordptr,"sum") == SUCCESS) {
                    errorsum = 1;
                    continue;
                }
            }

            if (c == 'M' && d != (char)0) {
                /* Magick2SHR palette output in addition to SHR preview */
                if (cmpstr(wordptr,"m2s") == SUCCESS){
                    vbmp = m2s = 1;
                    continue;
                }
                if (cmpstr(wordptr,"mix") == SUCCESS){
                    /* use additional matching palettes for Brooks output */
                    mix256 = 1;
                    continue;
                }
            }

            /* there is no option in this utility to force reduced saturation of an image.
               the saturation control in this utility applies exclusively to SHR output

               by default for most SHR conversion options when a color in the conversion palette
               cannot be substituted with a real color in the same "spectrum" a fully saturated
               color from the conversion palette displays on the finished output.

               sometimes this looks like hell so the user has the option of replacing the
               fully saturated color with a de-saturated or grey pixel image-wide.

               but doing this changes the image tone and washes-it out so it seldom
               results in good-looking output */

            if ((c == 'S' && (d > (char)47 && d < (char)58)) || (c == 'S' && d == (char)0)) {
                if (d == (char) 0 || d == '0' && wordptr[2] == (char)0) {
                    /* greyscale final output if color not in image */
                    for (d = 0; d < 16; d++) {
                        if(desaturate[d] == 100.0)desaturate[d] = 0.0;
                    }
                    continue;
                }
                /* desaturate final output if color not in image */
                jdx = atoi((char *)&wordptr[1]);
                if (jdx > 0 && jdx < 201) {
                    for (d = 0; d < 16; d++) {
                        if(desaturate[d] == 100.0)desaturate[d] = (double)jdx;
                    }
                    continue;
                }
            }


            if (c == 'G' && d == 'O') {
                /* grey over-ride when making SHR file */
                jdx = atoi((char *)&wordptr[2]);
                if (jdx > 0 && jdx < 16) {
                    greyoveride[jdx] = 1;
                    desaturate[jdx] = 0.0;
                    continue;
                }
                if (cmpstr(wordptr,"go0") == SUCCESS) {
                    greyoveride[0] = 1;
                    desaturate[0] = 0.0;
                    continue;
                }
            }

            if ((c == 'I' && d == (char)ASCIIZ) || cmpstr(wordptr,"imagetone") == SUCCESS){
                useimagetone = 1;
                continue;
            }


            if ((c == 'L' && d == (char)ASCIIZ) || cmpstr(wordptr,"lgr") == SUCCESS){
                lores = 1;
                continue;
            }

            /* set different Luma coefficients for color distance */
            jdx = 0;
            if (cmpstr(wordptr,"GIMP") == SUCCESS) jdx = 411; /* color managed sRGB */
            else if (cmpstr(wordptr,"SRGB") == SUCCESS) jdx = 411;
            else if (cmpstr(wordptr,"MAGICK") == SUCCESS) jdx = 709;/* most modern images */
            else if (cmpstr(wordptr,"HDTV") == SUCCESS) jdx = 240;
            if (jdx != 0) {
               lumaREQ = jdx;
               printf("Using Luma Rec. %d\n", lumaREQ);
               setluma();
               continue;
            }

            if (c == 'L') {
              /* Luma */
              jdx = atoi((char *)&wordptr[1]);
              if (jdx == 601 || jdx == 709 || jdx == 240 || jdx == 911 || jdx == 411 || jdx == 2020) {
                  lumaREQ = jdx;
                  printf("Using Luma Rec. %d\n", lumaREQ);
                  setluma();
                  continue;
              }
            }

            if (cmpstr(wordptr,"dl") == SUCCESS || cmpstr(wordptr,"dlgr") == SUCCESS){
                lores = doublelores = 1;
                continue;
            }

            /* DOS 3.3 header will be appended to Apple II Output */
            if (cmpstr(wordptr,"dos") == SUCCESS) {
                dosheader = 1;
                continue;
            }

            if (cmpstr(wordptr,"shr") == SUCCESS || cmpstr(wordptr,"shr2") == SUCCESS) {
                shr = 320;
                if (cmpstr(wordptr,"shr2") == SUCCESS) {
                    shr2 = 1;
                    puts("shr2 is in effect");
                }
                if (shrpalette == 15){
                    palnumber = 15;
                    GetBuiltinPalette('P','1',palnumber);
                }
                continue;
            }

            if (cmpstr(wordptr,"drooks") == SUCCESS || cmpstr(wordptr,"drooks2") == SUCCESS ||
                cmpstr(wordptr,"drooks3") == SUCCESS || cmpstr(wordptr,"drooks4") == SUCCESS ||
                cmpstr(wordptr,"drooks5") == SUCCESS) {
                    wordptr[0] = 'b';
                    usepalettedistance = 1;
                    puts("Palette distance conversion is now on. This takes longer, be patient.");
            }

            if (cmpstr(wordptr,"brooks") == SUCCESS || cmpstr(wordptr,"brooks2") == SUCCESS ||
                cmpstr(wordptr,"brooks3") == SUCCESS || cmpstr(wordptr,"brooks4") == SUCCESS ||
                cmpstr(wordptr,"brooks5") == SUCCESS || cmpstr(wordptr,"ega") == SUCCESS){

                /* mutually exclusive options */
                brooks2 = brooks3 = brooks4 = brooks5 = useegacolors = 0;

                if (cmpstr(wordptr,"brooks2") == SUCCESS) brooks2 = 1;
                else if (cmpstr(wordptr,"brooks3") == SUCCESS) brooks3 = 1;
                else if (cmpstr(wordptr,"brooks4") == SUCCESS) {
                    brooks4 = 1;
                }
                else if (cmpstr(wordptr,"brooks5") == SUCCESS) {
                    brooks5 = 1;
                }
                else if (cmpstr(wordptr,"ega") == SUCCESS) {
                    useoriginalcolors = useegacolors = 1;
                }

                shr = 320;
                brooks = 1;
                shrgrey = mono = 0;
                if (shrpalette == 15){
                    palnumber = 15;
                    GetBuiltinPalette('P','1',palnumber);
                }
                continue;
            }


            if (cmpstr(wordptr,"dic") == SUCCESS || cmpstr(wordptr,"dic2") == SUCCESS ||
                cmpstr(wordptr,"dic3") == SUCCESS || cmpstr(wordptr,"dic4") == SUCCESS ||
                cmpstr(wordptr,"dic5") == SUCCESS){
                    wordptr[0] = 'p';
                    usepalettedistance = 1;
                    puts("Palette distance conversion is now on. This takes longer, be patient.");
            }

            if (cmpstr(wordptr,"pic") == SUCCESS || cmpstr(wordptr,"pic2") == SUCCESS ||
                cmpstr(wordptr,"pic3") == SUCCESS || cmpstr(wordptr,"pic4") == SUCCESS ||
                cmpstr(wordptr,"pic5") == SUCCESS){

                /* mutually exclusive options */
                brooks2 = brooks3 = brooks4 = brooks5 = useegacolors = 0;

                if (cmpstr(wordptr,"pic2") == SUCCESS) brooks2 = 1;
                else if (cmpstr(wordptr,"pic3") == SUCCESS) brooks3 = 1;
                else if (cmpstr(wordptr,"pic4") == SUCCESS) {
                    brooks4 = 1;
                }
                else if (cmpstr(wordptr,"pic5") == SUCCESS) {
                    brooks5 = 1;
                }

                shr256 = 1;
                shr = 320;
                brooks = 1;
                shrgrey = mono = 0;
                if (shrpalette == 15){
                    palnumber = 15;
                    GetBuiltinPalette('P','1',palnumber);
                }
                continue;
            }

            if (cmpstr(wordptr,"shrgrey") == SUCCESS || cmpstr(wordptr,"shrgray") == SUCCESS) {
                shr = 320;
                shrgrey = mono = 1;
                brooks = 0;
                continue;
            }

            if (cmpstr(wordptr,"rgb") == SUCCESS) {
                hsl = 0;
                continue;
            }


            if (cmpstr(wordptr,"gs") == SUCCESS) {
                usegscolors = 1;
                continue;
            }

            if (cmpstr(wordptr,"gs2") == SUCCESS) {
                usegspalette = 1;
                continue;
            }


            if (c == 'T' && d == (char)ASCIIZ) {
                tags = 1;
                continue;
            }

            if (c == 'D') {
                if (d == (char)ASCIIZ) {
                    dither = 1;
                    continue;
                }
                if (d == 'R' && wordptr[2] == (char) ASCIIZ) {
                    dither = randomdither = errorsum = 1;
                    continue;
                }
                if (d == 'F' && wordptr[2] == (char) ASCIIZ) {
                    dithertype = FLOYDSTEINBERG;
                    dither = errorsum = 1;
                    continue;
                }
                if (d == 'A') {
                    if (wordptr[2] == (char) ASCIIZ) {
                        dithertype == ATKINSON;
                        dither = errorsum = 1;
                        continue;
                    }
                    else if (wordptr[2] == '2' && wordptr[3] == (char)ASCIIZ) {
                        dithertype = ATKINSON2;
                        dither = errorsum = 1;
                        continue;
                    }
                }
            }

            if (c == 'O' && d == (char)ASCIIZ) {
                useoriginalcolors = usegscolors = 1;
                outline = 1;
                continue;
            }

            if (c == 'O' && d == '1') {
                if (wordptr[2] == '6' || wordptr[2] == '4') {
                   useoriginalcolors = usegscolors = 1;
                   if (wordptr[2] == '6')usesixteencolors = 1;
                   if (wordptr[2] == '4')usefourteencolors = 1;
                   continue;
                }
            }


            if (c == 'R') {
               if (d == (char)ASCIIZ) jdx = 25;
               else jdx = atoi((char *)&wordptr[1]);
               if (jdx != 0) {
                    if (jdx > 0 && jdx < 101) {
                        bleed = (8 * (100 + jdx))/ 100;
                        printf("%d percent reduced color bleed\n",jdx);
                    }
                    continue;
               }
            }

            /* applesoft-alternate output option for AppleWin Screen Capture Conversions */
            /* also applies to A2FC pixel-art (pixel level dithering) output from 24-bit BMP's */
            if ((c == 'A' && d == (char)ASCIIZ) || cmpstr(wordptr,"BIN") == SUCCESS) {
                /* output AUX,BIN - default is A2FC */
                applesoft = 1;
                continue;
            }

            /* VBMP output for DHGR or preview output for LGR/DLGR and SHR */
            if ((c == 'V' && d == (char)ASCIIZ) || cmpstr(wordptr,"VBMP") == SUCCESS) {
                vbmp = 1;
                continue;
            }

            /* for color output only */
            /* create BMP image fragment from Apple II full-screen or (larger) image fragment */
            if ((c == 'X' || c == 'Y' || c == 'W' || c == 'H') && (d > (char)47 && d < (char)58)) {
                jdx = atoi((char *)&wordptr[1]);
                /* range check */
                /* as long as there is something in the BMP output file, my range checking is relaxed */
                /* if width or height go out of bounds, the BMP output file will be padded with black-space */
                if (jdx < 0) continue;
                if (c == 'X') {
                    if (jdx > 139) continue;
                    fragx = (ushort) jdx;
                }
                else if (c == 'Y') {
                    if (jdx > 191) continue;
                    fragy = (ushort) jdx;
                }
                else if (c == 'W') {
                    if (jdx < 1 || jdx > 140) continue;
                    fragwidth = (ushort) jdx;
                }
                else if (c == 'H') {
                    if (jdx < 1 || jdx > 192) continue;
                    fragheight = (ushort) jdx;
                }
                frag = 1;
                continue;
            }

            jdx = atoi((char *)&wordptr[0]);
            if (jdx == 140) {
                /* 140 x 192 x 24-bit color BMP output option - default is 280 x 192 24-bit color BMP */
                doublepixel = 0;
                continue;
            }
            if ((c == 'M' && d == (char)ASCIIZ)|| jdx == 384 || jdx == 192 || cmpstr(wordptr,"mono") == SUCCESS) {
                /* 560 x 384 or 560 x 192 monochrome BMP output option - default is 280 x 192 24-bit color BMP */
                mono = 1;
                if (jdx == 192) {
                    doublepixel = 0;
                }
                continue;
            }
            if (c == 'P') {

                /* experimental ImageMagick segmentation for SHR output */
                /* literal "pim" followed by "seed" palette pathname - sets conversion mode if valid */
                if (toupper(wordptr[1]) == 'I') {
                    if (toupper(wordptr[2]) == 'M') {
                        if (GetPIMPalettes((char *)&wordptr[3]) == SUCCESS) continue;
                        puts("Exiting...");
                        return (1);

                    }
                }

                /* other palette routines */
                /* first check for a bmp or a pcxfile being used as a palette */
                jdx = GetBmpOrPcxPalette((char *)&wordptr[1]);
                if (jdx != SUCCESS) {
                    /* always check for a user definable palette file */
                    jdx = GetUserPalette((char *)&wordptr[1]);
                }

                if (jdx == SUCCESS) {
                    jdx = 6;
                    d = '6';
                }
                else {
                    jdx = atoi((char *)&wordptr[1]);
                }
                /* palette option - p0 to p16 */
                jdx = GetBuiltinPalette(c,d,jdx);
                if (jdx != -1) {
                    palnumber = jdx;
                    shrpalette = palnumber;
                    continue;
                }
            }
            if (cmpstr(wordptr,"full") == SUCCESS){
                doublegrey = 1;
                continue;
            }
            /* this is somewhat problematic */
            /* however some users may find it convenient to output to a different basename */
            /* I primarily left this in place to match the older MS-DOS version of this utility
               because a fair amount of documentation is already in place for that. */
            strcpy(outfile, argv[idx]);

        }
    }
  }


  /* do not allow extensions in output file base names */
  jdx = 999;
  for (idx = 0; outfile[idx] != ASCIIZ; idx++) {
      if (outfile[idx] == '.') jdx = idx;
  }
  if (jdx != 999) outfile[jdx] = ASCIIZ;

  jdx = 999;
  for (idx = 0; fname[idx] != ASCIIZ; idx++) {
      if (fname[idx] == '.') jdx = idx;
  }

  /* automatic bmp file extension for LGR, DLGR, and SHR output */
  /* we can't just assume that the input file is a bmp file for DHGR output
     because the input file extension drives the output type
     and we run the risk of over-writing the wrong file if the user forgets
     to put an extension on the input file name */
  if (jdx == 999 && (lores == 1 || shr == 320) && fname[0] != (char)ASCIIZ) {
      strcat(fname,".bmp");
      for (idx = 0; fname[idx] != ASCIIZ; idx++) {
        if (fname[idx] == '.') jdx = idx;
      }
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
        if (c == 'A' && d == '2' && e == 'F' && f == 'M') a2fc = mono = 1;
        /* support for Sheldon Simms tohgr */
        if (c == 'D' && d == 'H' && e == 'G' && f == 'R') a2fc = tohgr = 1;
      }

      /* support for our extensions only */
      /* full-screen formats only */
      if (c == 'B' && d == 'M' && e == 'P') bmp = 1;
      /* process back-up if requested */
      if (c == 'B' && d == 'M' && e == '2') bmp = bm2 = 1;

      /* full-screen formats */
      if (c == '2' && d == 'F' && e == 'C') a2fc = 1;
      if (c == 'B' && d == 'I' && e == 'N') auxbin = 1;
      if (c == 'A' && d == 'U' && e == 'X') auxbin = 1;
      /* image fragments */
      if (c == 'D' && d == 'H' && e == 'R') dhr = 1;


      if (c== 'S' && d == 'H') {
          /* native SHR to M2S format BMP conversion */
          if (e == 'R' || e == 'G' || e == '2' || e == '3') shrinput = 1;
      }

   }

   /* native SHR to M2S format BMP conversion
      all other options will be ignored */
   if (shrinput == 1) return shrtom2s(fname,outfile);

   dhrbuf = (uchar *)malloc(32000);

   if (dhrbuf == NULL) {
        puts("No memory...");
        return (1);
   }

   if (imnumpalettes == 0 && fourbit == 0) {
    if (mono == 1) {
        if (shrgrey == 1) puts("Palette : 16 level Grey Scale");
        else puts("Palette : Black and White Monochrome");

    }
    else {
        printf("Palette : %d - %s\n",palnumber,palname[palnumber]);
    }
  }

  /* color bleed is fixed for optional dither types for now */
  /* not all these are implemented for now */
   switch(dithertype) {
        case  FLOYDSTEINBERG:   bleed = 16; break;
        case  JARVIS:           bleed = 48; break;
        case  STUCKI:           bleed = 42; break;
        case  ATKINSON:         bleed = 8;  break;
        case  ATKINSON2:        bleed = 6;  break;
        case  BURKES:
        case  SIERRA:           bleed = 32; break;
        case  SIERRATWO:        bleed = 16; break;
        case  SIERRALITE:       bleed = 4;  break;
    }

  status = INVALID;

  if (bmp == 1) {

      if (fourbit != 0) {
          status = Convertfourbit(sname,outfile);
          free(dhrbuf);
          if (status == SUCCESS) return SUCCESS;
          return 1;
      }

      if (imnumpalettes != 0) {
          status = ConvertPIM(sname,outfile);
          free(dhrbuf);
          if (status == SUCCESS) return SUCCESS;
          return 1;
      }

      if (lores == 1 || shr == 320) {
          status = ConvertLoResAndSHR(sname,outfile);
          free(dhrbuf);
          if (status == SUCCESS) return SUCCESS;
          return 1;
      }
      status = ReadHybrid(sname,outfile);

      if (status == SUCCESS) {
          free(dhrbuf);
          return SUCCESS;
      }

      status = ReadAppleWin(sname,outfile);
      if (status == SUCCESS) {
          if (applesoft == 1) status = read_2fc(outfile);
          else status = read_binaux(outfile);
      }
  }
  if (auxbin == 1) status = read_binaux(sname);
  if (a2fc == 1) status = read_2fc(sname);
  if (dhr == 1) status = read_dhr(sname);

  if (status) {
    puts(szTextTitle);
    printf("%s is an Unsupported Format or cannot be opened.\n", fname);
    free(dhrbuf);
    return 1;
  }

  if (mono == 1) status = save_to_bmp(outfile, doublepixel);
  else status = save_to_bmp24(outfile);

    if (status == SUCCESS) {
        printf("%s.BMP Saved!\n",outfile);
    }
    else {
        printf("Error saving %s.BMP!\n",outfile);
        status = 1;
    }
    free(dhrbuf);
#ifdef MSDOS
    puts("Have a Nice Dos!");
#endif

    return status;
}
