/* ---------------------------------------------------------------------

m2s MagickToShr (C) Copyright Jonas Grönhagen and Bill Buckels 2014.
All Rights Reserved.

Licence Agreement
-----------------

Use and Distribute at your own risk!

This code and the programs it produces are under development. Neither
Jonas Grönhagen or Bill Buckels have warranty obligations or liability
resulting from its use in any way whatsoever. If you don't agree,
remove this source code, the programs it produces, and related files
from your computer now.

Description
-----------

MagickToShr (C) Copyright Jonas Grönhagen and Bill Buckels 2014.
All Rights Reserved.
Usage is: m2s BaseName -Options
BaseName: _proc.bmp and _palette.bmp (file pairs)
          "m2s Woz" opens "Woz_proc.bmp" and "Woz_palette.bmp"...
          For MS-DOS, "M2S16 WOZ" opens "WOZ.BMP" and "WOZ.DIB!"
          16 palettes for mode320 output and 200 for mode3200!
Options:  -A = Alternate PNT file output (run length encoded).
               Default: raw SHR (mode320) or SH3 (mode3200).
          -T = Use CiderPress Attribute Preservation Tags.
               Default: No Tags! (unadorned file extensions)
               Does not apply to M2S16.EXE (MS-DOS binary).
          Options may be combined: "-ta" or "-at"
          Options are Case Insensitive - Switchar "-" is Optional.

Designed by:   Jonas Grönhagen and Bill Buckels
Programmed by: Bill Buckels
Email:         bbuckels@mts.net

Synopsis:

BMP Input
Apple II SHR Output

Revision     : April, 2014 - 0.0 - None - Under Development
               April 2015 - fixed palette count - watch for problems
               April 2015 - added support for BMP 4 and BMP 5
                            initilly only BMP 3 was supported.

-----------------------------------------------------------------------
M2S16.EXE - MS-DOS

Built under Large Model 16 bit Microsoft C (MSC) Version 8.00c
Note: Run in an MS-DOS emulator like DOSBox if you can't run it raw.

M2S32.EXE - WIN32

Built under Visual Studio 2005
Microsoft (R) 32-bit C/C++ Optimizing Compiler Version 14.00 for 80x86

M2S.EXE - WIN32

Built under MinGW 5.1.4 (gcc)
----------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

/* ***************************************************************** */
/* ========================== defines ============================== */
/* Note: define DEBUG to get additional info.                        */
/* ***************************************************************** */

/* status codes */
#define SUCCESS  0
#define VALID    SUCCESS
#define FAILURE  -1
#define INVALID  FAILURE

/* error codes */
#define ERR_IN     -1
#define ERR_OUT    -2
#define ERR_FMT    -3
#define ERR_SIZE   -4
#define ERR_ABORT  -5

/* the various 3 dimensional color arrays used in here expect this order for RGB values */
#define RED 0
#define GREEN 1
#define BLUE 2

/* constants for the biCompression field in a BMP header */
#define BI_RGB      0L
#define BI_RLE8     1L
#define BI_RLE4     2L

#define PIC_FMT    1
#define BROOKS_FMT 2

/* RLE constants - including APF and PackBytes constants */

/* maximum number of list nodes for packbytes encoder */
/* must be equal or greater than the raw line length of the input BMP */
#define RAW_MAX 160
/* maximum line length for encoded packbytes buffer */
/* must be double the raw line length of the input bmp */
#define PAK_MAX (RAW_MAX * 2)

/* ***************************************************************** */
/* ========================== typedefs ============================= */
/* ***************************************************************** */

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef short sshort;

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


/* April 2015 - added support for BMP 4 and BMP 5 headers */
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


/* picfile trailer structure (sort of) - extended for brooks output
   this is really just a buffer when it comes to brooks */
#ifdef MINGW
typedef struct __attribute__((__packed__)) tagPICFILE
#else
typedef struct tagPICFILE
#endif
{
    uchar scb[200];
    uchar padding[56];
	uchar pal[200][32]; /* note the PIC file actually has only 16 palette entries */
} PICFILE;


/* APF structures */
/* FileType - $C0 AuxType $0002 */
/* integers are in big endian (Motorola) Format */

#ifdef MINGW
typedef struct __attribute__((__packed__)) tagPNTPIC
#else
typedef struct tagPNTPIC
#endif
{
	ulong  Length;                   /* Block Length (also image length) */
	uchar  Kind[5];                  /* 4 'M' 'A' 'I' 'N' */
	ushort MasterMode;               /* 0 for mode320 and 0x80 for mode640 */
	ushort PixelsPerScanline;        /* 320 or 640 */
	ushort NumColorTables;           /* 16 */
    uchar  ColorTable[16][32];       /* $0RGB table buffer */
    ushort NumScanLines;             /* vertical resolution */
    ushort ScanLineDirectory[200][2];
} PNTPIC;

#ifdef MINGW
typedef struct __attribute__((__packed__)) tagPNTBROOKS
#else
typedef struct tagPNTBROOKS
#endif
{
	ulong  Length;
	uchar  Kind[5];                  /* 4 'M' 'A' 'I' 'N' */
	ushort MasterMode;
	ushort PixelsPerScanline;        /* 320 */
	ushort NumColorTables;           /* 1 */
    uchar  ColorTable[1][32];        /* $0RGB table buffer - All Black */
    ushort NumScanLines;             /* vertical resolution */
    ushort ScanLineDirectory[200][2];/* ModeWord is used for sequential scbs */
} PNTBROOKS;

#ifdef MINGW
typedef struct __attribute__((__packed__)) tagMULTIPAL
#else
typedef struct tagMULTIPAL
#endif
{
	ulong  Length;
	uchar  Kind[9];                   /* 8 'M' 'U' 'L' 'T' 'I' 'P' 'A' 'L' */
	ushort NumColorTables;            /* 200 */
    uchar  ColorTableArray[200][32];  /* $0RGB table buffer */
 } MULTIPAL;

/* end of Big Endian Motorola Format Structures */


/* PackBytes Line Encoder List Structure */
/* FileType - $C0 AuxType $0002 */
/* Double layers - both sequential

   Layer 1 - Count, Value pairs for an shr scanline
   Layer 2 - FIFO Stack of Singleton Values

   This List Structure must always provide 1 node for each byte of a raw shr scanline.

*/
#ifdef MINGW
typedef struct __attribute__((__packed__)) tagRAWLIST
#else
typedef struct tagRAWLIST
#endif
{
	uchar  CNT;
	ushort VAL;
	uchar  Singleton; /* this is a FIFO stack for processing singletons */
} RAWLIST;


/* ***************************************************************** */
/* ========================== globals ============================== */
/* ***************************************************************** */

/* static structures for processing */
BITMAPFILEHEADER bfi;
BITMAPINFOHEADER bmi;
/* April 2015 - support for BMP version 4 but not sure about other versions */
BITMAPINFOHEADERV5 bmiV5;
PICFILE shr;

uchar shrline[200][160];

/* colormap array */
uchar cmap[200][16][3];

/* line index for shr palette */
uchar svgaline[320];
/* rgb triples from bmpline */
uchar bmpline[960];

/* filenames */
char bmpfile[256], cmapfile[256], shrfile[256], brooksfile[256], pntfile[256];

/* default */
sshort output_format = PIC_FMT;
sshort numpalettes = 16;

/* ***************************************************************** */
/* ========================== RLE specific globals ================= */
/* ***************************************************************** */

sshort output_pnt = 0, suppress_pnt = 1, no_tags = 1, suppress_pic = 0;
ulong MainLength = 0L;

/* an SHR file is always 160 bytes x 200 scanlines */
/* but a PNT file can be any length x any width */
/* the packbytes implementation in this program is limited to 160 bytes */
/* it is therefore limited to packing screensize PNT files */

ushort RawCount = 0, SingleCount = 0;
RAWLIST *RawBuf;
MULTIPAL *MultiPal;

PNTPIC *picMain;
PNTBROOKS *brooksMain;

ushort PackedCount = 0;
/* output buffer for the Packed Line */
uchar PackedBuf[PAK_MAX];


/* ***************************************************************** */
/* ========================== RLE output routines ================== */
/* ***************************************************************** */

/* The Apple Preferred Format (APF) file is also called a PNT file.
   It stores graphics images for display in SHR mode320, mode640 and
   mode3200.

   Images stored in APF can be wider and longer than the screen.

   Since I am processing mode320 and mode3200 screens only, I am
   limiting my processing and output routines to these two subsets of
   the Apple Preferrred Format (APF).

   For the purposes of general SHR file interchange, there are only 3
   Native SHR FileTypes of interest (marked with arrows below):


   $Cx Types: Graphics

   $C0 PNT Apple IIgs Packed Super HiRes
      $0000 - Paintworks Packed Super Hi-Res
      $0001 - Packed Super HiRes
      $0002 - Apple Preferred Format     <--- Run Length Encoded
      $0003 - Packed QuickDraw II PICT

   $C1 PIC Apple IIgs Super HiRes
      $0000 - Super Hi-Res Screen Image  <--- Raw - Easiest
      $0001 - QuickDraw PICT
      $0002 - Super HiRes 3200           <--- Raw - Not Easy to load.


   Output File Extension - SHR#C10000 - Pic
   ==================================

   For the average user, the PIC File Aux Type 0000 is easiest and
   quickest to load. It is a RAW BSAVED image of SHR memory, which
   includes 3200 bytes of screen memory of 200 x 160 byte scanlines,
   followed by scanline control bytes padded to 256 bytes, followed by
   512 bytes of 16 palettes of 16 colors.

   Because the standard SHR Screen Memory is only in two resolutions,
   mode320 and mode640, this file format supports both resolutions,
   but does not support images with higher resolutions than the SHR
   screen

   Even though this file is the lowest common denominator for SHR and
   easiest to load, a specific load sequence including control over
   main and auxiliary memory is still required, so intermediate
   programming skills are required for this file to be of use by a
   programmer.

   Output File Extension - SH3#C10002 - Brooks
   ==================================

   The PIC File Aux Type 0002 (Brooks Format) is quick to load but not
   easy for a programmer to display. Displaying this type of file in a
   program requires that the program code is constantly synching with
   the screen update routines for each scanline to move palettes in
   real time from a buffer of 200 palettes into the 16 palettes
   supported by SHR. This leaves little time for anything else, and
   less time on a slower Apple IIgs, and even less time on an Apple
   IIe with a VOC but no accelerator.

   While I see Todd Whitesel has written a Brooks Loader for the VOC
   on a GS, I have never seen a VOC Brooks loader for the Apple IIe.

   I don't know whether an 8 bit program like SHRVIEW would work for
   Brooks on a IIe equipped with a VOC since I don't have a VOC.

   Todd's file format for the VOC's standard interlace mode is also not
   as robust as my own, since it only allows half the palettes.

   I am speculating that he likely based his format on what was easiest
   for him to create in whatever IIgs Paint program he used at the time.
   I also have no evidence of any Brooks format files that follow his
   HT and HB formats.

   While my BMP2SHR utility supports the creation of Brooks Files for
   the VOC's interlace mode, in practice these may be quite useless.

   This utility does not support those, nor any provision for the
   VOC's interlace mode400 at all.

   Output File Extension - PNT#C00002 - APF
   ========================================

   APF is the most complicated of the 3 SHR output formats supported
   by this program, and also the most extensible since APF includes
   Run-Length Encoded (RLE) versions of both PIC and BROOKS files, and
   also offers support for sizes other than 320 x 200 and 320 x 400.
   An APF file can be used to store Image Fragments (Sprites) or SHR
   images larger than the screen. All the SHR scanlines stored in an
   APF are in PackBytes RLE format, but the header information is not.

   While APF's PackBytes RLE is not as efficient as DreamGraphx LZW
   compression, APF files are widely supported. (DreamGraphx files are
   not noted above, and are also not supported by this program.)

   Some APF Considerations:

   1. The APF consists of CHUNKS called BLOCKS. The MAIN Block in
   	  an APF contains the entire mode320 and mode640 subset and
   	  in the case of a mode3200, everything except for the 200
   	  palettes is stored.

   2. One additional consideration is the requirement to store
      the position for the start of each scanline in the APF
      file so that each scanline can be unpacked separately.

      Doing so allows an APF viewer or Editor to Unpack scanlines one
	  a time to support scrolling through an SHR image larger than the
	  screen. Memory is scarce on the Apple II, but because the APF
	  stores the start of each scanline, the programmer can store the
	  Packed scanline in a small buffer using very little memory in
	  the process; by seeking through a file one line at a time to the
	  start of a packed line, then reading and unpacking (in whole or
	  in part) directly to the SHR screen.

   Unsupported SHR File Formats
   ============================

   Unsupported Compressed Formats
   ==============================

   As noted above DreamGraphx file output is not supported. If this
   program is extended to support converting to a format that uses
   file compression, only the most efficient compressed native IIgs
   SHR format will be supported.

   For some notes on the work Brutal Deluxe has done to develop this
   format see the following link:

   http://www.brutaldeluxe.fr/products/crossdevtools/lz4/

   Non-native formats abound, both compressed and uncompressed,
   but these can commonly be converted to a simple 24-bit
   uncompressed BMP file for conversion to the 3 Apple II formats
   supported by this program.

   Compuserve GIF files saw wide use at the time the IIgs had wide
   use. GIF files were largely produced on the IBM-PC for display
   in SVGA unrestricted 256 color mode. They were often highly
   optimized for a single image palette, and not for 16 segmented
   palettes like SHR, or 200 16 color line palette like BROOKS.
   Therefore reorganizing the colors in a GIF to match the SHR palette
   can produce poor results. Complex GIFs like those created from
   photos can end-up dithered and the color balance can also be
   optimized for the SVGA display, so methods like error diffusion to
   smooth a GIF may not work so well. Despite the fact that a GIF is
   not lossy, and a JPEG is, the color balance in a JPEG combined with
   its lack of dithering can result in better palettizing and
   dithering to an SHR file.

   GIF and JPEG are neither supported input formats (only 24-bit BMPs
   are input formats) nor are they supported for output: this program
   converts to SHR formats, not to IBM-PC style formats. PNG is not
   supported for input or output either.

   Unsupported Uncompressed Formats - Raw or Packed
   ================================

   4 of the file formats noted above are not supported by this
   program, They include two variants of the QuickDraw PICT file
   format.

   QuickDraw II PICT files store pictures as a script; a series of
   command instructions; opcodes which are used to record the
   QuickDraw II commands that created the picture. Modeled after PICT2
   on the Macintosh, they can be used to exchange pictures between the
   IIgs and the Mac. They can be played-back by a IIGs GUI program
   with the QuickDraw II Toolbox Routine DrawPicture:

   http://apple2.boldt.ca/?page=til/tn.iigs.046

   *BUT* they are not bitmapped graphics files.

   Of the 2 remaining unsupported packed SHR files in the list above,
   the least elegant is the $C0,$0001 Packed Super HiRes. It is just
   an RLE PIC file, encoded from start to finish. It is not extensible
   to BROOKS, and it is not extensible to sizes smaller or larger than
   the SHR screen. Any programmer who can load these to the screen can
   also load an APF. Any programmer who cannot write the code for
   UnPackBytes will have no use for these but may still be able to
   load the raw PIC format equivalent. So a Packed Super Hi Res file
   offers no advantage over the APF.

   The remaining file format of the unsupported formats from the list
   above is the Paintworks Packed Super Hi-Res Picture File. This file
   is targetted to users of Activision's PaintWorks Program, and
   contains irrelevent header baggage in the form of QuickDraw II
   Patterns in addition to Bitmapped Graphics Information.

   It is also not as robust as the APF when it comes to sizes; only
   screen width is supported, and even though images longer than the
   screen are supported (which may suit editing SHR images of 400
   lines targetted at the VOC's interlaced mode400), Paintworks
   Packed files can support full-paged documents but do not specify
   exactly how many scanlines are in a page.

   So besides its reliance on QuickDraw II, this also relies on the
   PaintWorks Application. And besides being primarily supported only
   by their creator (Activision PaintWorks), these files use the
   PackBytes RLE and not something better, so offer no advantage
   over APF in image size.

   The Apple II FileType notes can be referred to for additional
   details not included here or elsewhere in this program's source
   code comments or additional documentation.

     */



/*
 * Apple PackBytes RLE format
 *
 * Format is:
 *	<mask | count>, <value>
 *
 * Mask is the 2 hi-bits | Count is the 6 low bits, value is the raw byte

 *  Encoding: Description

 *	00xxxxxx: (0-63) 1 to 64 bytes follow, all different - mask = 0x00
 *	01xxxxxx: (0-63) 1 to 64 repeats of next byte        - mask = 0x40
 *	11xxxxxx: (0-63) 1 to 64 repeats of next byte taken as 4 bytes - mask - 0xc0

 The above 3 encodings are used in this implementation. The following is not:

 *	10xxxxxx: (0-63) 1 to 64 repeats of next 4 bytes (quads) - mask = 0x80


 Rationale
 =========

 My rationale of PackBytes, including for selecting only 3 of the 4
 encodings is as follows:

 Run-length encoding depends on repeats. If there are no repeats, or a
 small number of short repeats, the packed line will be larger than
 than the raw line.

 Since the SHR display is a packed pixel format of 2 pixels per byte,
 2 pixels must repeat. Packed pixels already result in smaller lines
 in raw format, so the savings between raw and packed scanlines can be much
 less they might be with 8 bit indices.

 In any run length encoding scheme the likehood of pixels repeating
 decreases with more colors. The likelihood of the SHR's packed pairs
 of pixels repeating decreases even more with more colors per line .

 Mask x00 - Singletons
 =====================

 If all 160 bytes in a screen width image are different, the line will
 expand when encoded as follows:

 Raw (64,64,32) = Encoded (65,65,33) = 163 Bytes.

 But some encoded lines can be even larger, making it hardly worth the
 effort to encode the line in the first place. To begin with
 additional disk space may needed to store the encoded file. and
 loading the file requires decoding, which not only takes longer to
 load from disk because of the larger file size, but because the file
 can't be read directly into screen memory, it must be buffered and
 decoded. This might be worth the effort to save disk space if a file
 encodes to a much smaller size, but not when it expands when encoded.

 Since the SHR display is in linear auxiliary memory, and organized
 from the top of the screen to the bottom of the screen, the only
 advantage of using run length encoding on an SHR file that gets
 larger and not smaller when encoded, is when the file is larger than
 the display resolution and must be scrolled through to view. For
 screen images a larger file is pointless especially since IIgs
 programs that work with SHR images will likely work with raw PIC files
 anyway.

 If a small number of short repeats occur throughout the line,
 encoding can expand the line size further.

 For example if a line has 2 repeats of 2 bytes that divide it into 5
 segments, it can look like this when encoded:

 Repeated bytes = 4, Encoded Bytes = 4

 Equal Segments - Packed Length = 159 + 4 = 163
 Individual bytes = 160 - 4 = 156 / 3 = (52,52,52), Encoded Bytes = (53+53+53) = 159

 Unequal Segments - Packed Length = 161 + 4 = 165
 Individual bytes = 160 - 4 = 156 / 3 = (154,1,1),  Encoded Bytes = (65+65+27)+(2+2) = 161

 A second example for a line with 3 repeats of 2 bytes that divide it into 7 segments can
 look like this when encoded:

 Repeated bytes = 6, Encoded Bytes = 6

 Equal Segments - Packed Length = 158 + 6 = 164
 Individual bytes = 160 - 6 = 154 / 4 = (38,39,38,39), Encoded Bytes = (39+40+39+40) = 158

 Unequal Segments - Packed Length = 160 + 6 = 166
 Individual bytes = 160 - 6 = 154 / 4 = (151,1,1,1),   Encoded Bytes = (65+65+24)+(2+2+2) = 160

 A final example for a line with 4 repeats of 2 bytes that divide it into 9 segments can
 look like this when encoded:

 Repeated bytes = 8, Encoded Bytes = 8

 Equal Segments - Packed Length = 158 + 8 = 166
 Individual bytes = 160 - 8 = 152 / 5 = (30,30,32,30,30), Encoded Bytes = (31+31+33+31+31) = 158

 Unequal Segments - Packed Length = 156 + 8 = 164
 Individual bytes = 160 - 8 = 152 / 5 = (148,1,1,1,1),    Encoded Bytes = (65+65+21)+(2+2+2+2) = 156

 The 3 examples above are ordered examples, but in practice SHR images
 follow no rhyme or reason, so the combinations resulting from encoding
 will be as random and unpredicatble as the image itself.

 The PackBytes implementation in this program attempts to overcome
 unecessarily complicated encoded lines. If a line does not encode
 below the 163 byte threshold of a 3 segmented line of individual
 bytes, it will be encoded into a 163 byte 3 segmented line of 163
 bytes. PackBytes provides for no smaller alternative.

 When compared to other run length encoding of the same era, like
 Z-Soft PCX, PackBytes is less-efficient when it comes to a line of
 all different single bytes, unless single bytes of PCX's 0xc0 mask byte
 are encountered. Otherwise such a line in a PCX file be the same as
 the raw version of the line... but this 0xc0 constraint is the only
 condition that will expand a PCX line.

 Since PCX encoding offers a maximum encoded length of 63 and not 64
 bytes as in PackBytes Mask 0x40, and also does not allow a whole one
 color line to be encoded into a single byte pair (PCX has no
 equivalent to PackBytes Mask 0xc0), sometimes PackBytes can outperform
 PCX on simple images with repeats over 63 bytes in length. Other
 schemes for RLE similar to PCX that changed the mask byte incurred
 additional overhead to do so. It is difficult to generally compare
 one RLE scheme to another; that needs to be done image by image.

 File formats that came after RLE, like GIF, applied non-lossy
 compression and achieved better results than RLE alone, but were more
 complicated to display, despite the fact that they incurred less
 storage and loading overhead. These were followed by equally
 complicated but lossy formats like jpeg which are not well suited to
 the limited 4 bit color depth of the SHR display despite far smaller
 files.

 Since PackBytes is the standard for the APF file and the SHR display,
 and works reasonably quickly on 8 bit Apple //e's equipped with a
 VOC, discussion of other RLE's and other compressed formats is moot,
 and out of the scope of what I have done here.

 Mask 0x40 and Mask 0xc0 - Repeats
 =================================

 Repeats were discussed above. There are 3 types of repeats in
 PackBytes but only two of them are used in this PackBytes
 implementation.

 If a repeat is 64 bytes or less, Mask 0x40 is used to encode the
 line. For repeat of over 64 bytes, Mask 0xc0 is used for the first
 encoded byte pair. But how the subsequent encoded byte pairs are done
 depends on how many repeats are done.

 Mask 0xc0 works the same way as Mask 0x40, and both encodings are
 pairs of bytes. But the count for Mask 0x40 is from 1-64, and the
 count for Mask 0xc0 is from 1-64 in repeats of 4. Therefore to be
 encoded entirely using Mask 0xc0, repeats that do not evenly divide
 by 4 will have 1-3 stragglers at the end of a run. These stragglers
 are encoded using Mask 0x40 at the end of the Mask 0xc0 encoded
 pairs, adding an additional 2 bytes to the encoded line.

 Mask 0xc0 lines are obviously the only way to go on a line with more
 than 64 repeats. On a line of one color the savings between a raw
 line and an encoded line are as follows:

 Best Case Scenario - Mask 0xc0

 10 Values
 Repeated Bytes = 160 = (40 x 4), Encoded Bytes = 2
 Repeated Bytes = 124,128,132,136,140,144,148,152,156 Encoded Bytes = 2

 10 Values
 Repeated Bytes = 120 = (30 x 4), Encoded Bytes = 2
 Repeated Bytes = 84,88,92,96,100,104,108,112,116 Encoded Bytes = 2;

 4 Values
 Repeated Bytes =  80 = (20 x 4), Encoded Bytes = 2
 Repeated Bytes =  68,72,76 Encoded Bytes = 2

 Total Values = 24

 Best Case Scenario - Mask 0x40

 Repeated Bytes =  64 = (64 x 1), Encoded Bytes = 2
 Repeated Bytes =  2-63 Encoded Bytes = 2

 Total Values = 63

 On an image with many colors on one line, this scenario is most
 likely, if repeats exist at all.

 Worst Case Scenario - Mask 0xc0 = 2 Encoded Bytes, Mask 0x40 = 2 Encoded Bytes

 The Best Case Scenarios above need only 2 Bytes to Encode up to an
 entire line of repeats. Repeated bytes in the following ranges need
 three encoded bytes:

 30 Values
 Repeated Bytes = 121-123,125-127,129-131,133-135,137-139,141-143,145-147,149-151,153-155,157-159 = 3
 30 Values
 Repeated Bytes = 81-83,85-87,89-91,93-95,97-99,101-103,105-107,109-111,113-115,117-119 = 3
 12 Values
 Repeated Bytes = 65-67,69-71,73-75,77-79 = 3

 Total Values = 72

 Conclusions:
 ============

 Repeats of 2-64 Are 100% likely to encode in 2 bytes.
 Repeats of 65-160 Are 25% likely to encode in 2 bytes.
 Repeats of 2-160 Are 60% likely to encode in 2 bytes

 PCX Comparison
 ==============

 34 Values  127-160 Encoded Bytes = 6
 63 Values  64-126 Encoded Bytes = 4
 62 Values  2-63 Encoded Bytes = 2

 0xc0 Mask Byte - Must Always Be Encoded - Not Included Above

 Comparitive Conclusions
 =======================

 Repeats of 2-64 Are 98% likely to encode in 2 bytes.
 Repeats of 65-160 Are 0% likely to encode in 2 bytes
 Repeats of 2-160 Are 39% likely to encode in 2 bytes

 Full Range Compression Ratio - 160 values
 =========================================

 PackBytes (88*2) + (72*3) =  392
 PCX (62*2) + (63*4) + (34*6) = 580

 On lines with over 63 repeats PackBytes is far more efficient than PCX.

 On lines of single colors PCX does better than Packbytes.

 PCX Could learn from PackBytes, and allow 2 MaskBytes like 0xc0 and
 0x40. Both formats could learn from Pictor which allows MaskBytes to
 be set inline. One feature that PCX has that PackBytes does not is
 the ability to encode in planar format. But encoding a single bit
 plane offers debatable efficiency when it comes to encoding bit
 mapped images. Both are designed in a device dependent manner so
 suffer trade-offs for reasons of ease of display rather than encoding
 efficiency.

 Mask 0x80 - Quad Repeats of the same 4 Bytes - Unused
 =====================================================

 Mask 0x80 encoding expects a byte pattern of 4 bytes to repeat in an
 image.

 Using Mask 0xc0 encoding the Packed Repeat for 160 Bytes is (count,data) =  (40,value) = 2 bytes;
 Using Mask 0x80 the same repeat is (40,value,value,value,value) = 5 bytes.

 No advantage there. No advantage for mode320 files at all, as far as
 I can see.

 I haven't been able to establish a logical case for the efficiency of
 Mask 80 encoding for mode640 files either.

 To expect a pattern of 01234567 to repeat in a mode320 image seems
 rather remote.

 In a dithered mode640 image, to expect a pixel pattern of
 0123456789012345 to repeat also seems rather remote.

 Overall Conclusion
 ==================

 If lines expand to more than 103 bytes when packed, then pack the
 line as a 103 byte run in 3 segments, and hope the next line saves
 some space.

 For repeats of multiples, use Mask 0xc0 where possible for runs of
 over 64 bytes, and encode the stragglers as a Mask 0x40 repeat of 2
 or 3 pixels, and single pixels if less than 2.

 I will likely expand a test version that tries Mask 0x80 at some
 point if I have some epiphany about size reduction advantages.It
 could well be for all that I know, that some encoder somewhere uses
 the 0x80 mask for something or other, but based on what I see,it
 wasn't very well thought out. Or perhaps it is so well thought out
 that I can't figure it out.

 */



/*

This PackBytes encoder is really a simple list processor.

Although the list processor for this function was written for the
occasion, it is a gutted version of the PCX encoder from the Z-Soft
technical reference for the PCX file format. It is just the standard
RLE algorithm without encoding. and can be easily adapted to most
RLE's including for debugging an RLE, because of its modular nature.

Inputs - Shr Scanline, Length
Outputs - PackBytes Scanline, Length

Pre Processing

This PackBytes Encoder uses a double buffering approach; It initially
reads a raw Super Hi-Res scanline into a sequential list of
Count,Value pairs, effectively splitting the line into a queue of 2 types of
nodes; Singletons and Repeats.

After the initial pass, the raw scanline is set aside, and the list is
encoded into an output buffer.

Passive State - Queue Priority 0

During this second pass, Singletons are
pushed onto a stack, until a Repeat node is encountered.

Active State - Singletons Priority 1, Repeats Priority 0

Then all the Singletons (if any) are popped off the stack and encoded,
before encoding the Repeat node.

Active State - Repeats Priority 1, Singletons Priority 0

Repeats are always immediate, but always yield to Singletons.
Singletons always line-up in the Queue, waiting for a repeat to
trigger an encoding event.

Meta State

This second process iterates through the list using this same
strategy, until all the nodes are encoded.

Quit State

If straggler Singletons are still on the stack when the end of the list
is reached, they are popped off the stack and encoded.

Post Processing

An encoded line with many colors and few repeats can be longer than
the raw line. Packbytes does poorly with when runs of Singletons are
split with short repeats, fragmenting the line.

Condition

To avoid expanding the line unnecesarily, this PackBytes encoder
calculates the optimum encoded line length without repeats. This is a
simple calculation which involves splitting the line up into 65 byte
segments, with 1 count byte for every 64 data bytes. If the encoded
line has exceeded the calculated optimal Singleton length, the raw
line is recoded as a line of Singletons without repeats.

Return Value

Length of Encoded Line

*/

int PackBytes(uchar *inbuff, sshort inlen)
{
    uchar this,last,msk;
    ushort runcount, repeats, singlerun, singlereps, maxpack, len;
    sshort idx,jdx,i,j;


	/* ********************************************* */
	/* ========== Build the List for this line ===== */
	/* ********************************************* */

	/* Build a list of count,value pairs */

    RawCount = 0;
    last = inbuff[0];
    len = 0;
    runcount=1;
	for(idx=1;idx<inlen;idx++){
		this=inbuff[idx];
		if(this==last){
			runcount++;
    	}
    	else{
        	if(runcount > 0){
				RawBuf[RawCount].CNT = runcount;
				RawBuf[RawCount].VAL = last;
				RawCount++;
				len += runcount;
        	}
        	last=this;
        	runcount=1;
      	}
	}

    /* stragglers */
   	if(runcount > 0) {
		RawBuf[RawCount].CNT = runcount;
		RawBuf[RawCount].VAL = last;
		RawCount++;
		len+= runcount;
	}

#ifdef DEBUG
    if (len != inlen) printf("Wrong length %d\n",len);
#endif


  	/* ********************************************* */
	/* ====== Encode the List for this line ======== */
	/* ********************************************* */

	/* Process a list of count,value pairs */

	PackedCount = SingleCount = 0;

	for (idx=0;idx<RawCount;) {
		runcount = RawBuf[idx].CNT;
		if (runcount == 0) {
			/* list nodes should never have a zero count */
			idx++;
			continue;
		}

		if (runcount == 1) {
		   /* push singleton nodes onto the stack until we hit a repeat node */
		   RawBuf[SingleCount].Singleton = RawBuf[idx].VAL;
		   SingleCount++;
		   idx++;
		   continue;
		}

		/* if we have hit a repeat list node... */
		/* before encoding the repeat, pop singleton nodes (if any) off the stack and encode 'em first */
		singlerun = 0;
		while (SingleCount > 0) {

			  if (SingleCount < 65) {
				  msk = (uchar)(SingleCount - 1);
				  PackedBuf[PackedCount] = msk;
				  PackedCount++;
				  for (i=0;i<SingleCount;i++) {
					 PackedBuf[PackedCount] = RawBuf[singlerun].Singleton;
					 singlerun++;
					 PackedCount++;
				  }
				  SingleCount = 0;
			      break;
			   }

			   PackedBuf[PackedCount] = (uchar)63;
			   PackedCount++;
               for (i=0;i<64;i++) {
					 PackedBuf[PackedCount] = RawBuf[singlerun].Singleton;
					 singlerun++;
					 PackedCount++;
			   }
			   SingleCount -= 64;
		 }

#ifdef DEBUG
		 if (SingleCount != 0) printf("SingleCount error = %d\n", SingleCount);
#endif

		/* Mask 0xc0 - use full quads to reduce repeats */
		while (runcount > 256) {
		   PackedBuf[PackedCount] = 0xff; /* 63 | 0xc0 */
		   PackedCount++;
		   PackedBuf[PackedCount] = RawBuf[idx].VAL;
		   PackedCount++;
		   runcount-= 256; /* decrement runcount until 256 or below */
		}

		if (runcount < 1) {
			idx++;
			continue;
		}

		/* Mask 0x40 for repeats of 1 to 64 */
		if (runcount < 65) {
			msk = (uchar)(runcount - 1);
			PackedBuf[PackedCount] = (uchar) (msk | 0x40);
			PackedCount++;
			PackedBuf[PackedCount] = RawBuf[idx].VAL;
			PackedCount++;
			idx++;
			continue;
		}

		/* Mask 0xc0 - use quads for repeats of 65 to 256 */
		repeats = runcount / 4;

		msk = (uchar) (repeats  - 1);
		PackedBuf[PackedCount] = (uchar) (msk | 0xc0);
		PackedCount++;
		PackedBuf[PackedCount] = RawBuf[idx].VAL;
		PackedCount++;
		runcount -= (repeats * 4);

		if (runcount < 1) {
			idx++;
			continue;
		}

		/* Mask 0x40 for stragglers - repeats of 1 to 3 */
		msk = (uchar)(runcount - 1);
		PackedBuf[PackedCount] = (uchar) (msk | 0x40);
		PackedCount++;
		PackedBuf[PackedCount] = RawBuf[idx].VAL;
		PackedCount++;
		/* on to the next list member */
		idx++;

	}

	/* clear stragglers */
	singlerun = 0;
	while (SingleCount > 0) {
          /* pop any remaining singleton nodes off the stack and finish-up */
		  if (SingleCount < 65) {
			  msk = (uchar)(SingleCount - 1);
			  PackedBuf[PackedCount] = msk;
			  PackedCount++;
			  for (i=0;i<SingleCount;i++) {
				 PackedBuf[PackedCount] = RawBuf[singlerun].Singleton;
				 singlerun++;
				 PackedCount++;
			  }
			  SingleCount = 0;
			  break;
		   }

		   PackedBuf[PackedCount] = (uchar)63;
		   PackedCount++;
		   for (i=0;i<64;i++) {
				 PackedBuf[PackedCount] = RawBuf[singlerun].Singleton;
				 singlerun++;
				 PackedCount++;
		   }
		   SingleCount -= 64;
	 }

#ifdef DEBUG
		 if (SingleCount != 0) printf("SingleCount error = %d\n", SingleCount);
		 goto SKIP;
#endif

	/* a raw line of singletons incurs 1 count byte for every 64 data bytes */
	/* a line of 160 singletons is expanded by 3 count bytes to 163 bytes */
	/* if the packed line expands beyond this length then simply encode it as singletons */


    /* the following calculates the most efficient encoding for a line of singletons */
    maxpack = (ushort)(inlen/64);
    if ((inlen % 64) != 0) maxpack++;
    maxpack+=inlen;

    /* if the length of a packed line goes beyond this, run individual bytes */
    if (PackedCount > maxpack) {

		PackedCount = RawCount = 0;
		maxpack = (ushort) (inlen / 64);
		for (idx = 0;idx < maxpack; idx++) {
			/* runs of 64 individual bytes */
		    PackedBuf[PackedCount] = 63; PackedCount++;
		    for (jdx = 0; jdx < 64; jdx++, RawCount++, PackedCount++) {
			    PackedBuf[PackedCount] = inbuff[RawCount];
			}

		}

		inlen -= (maxpack*64);

		if (inlen > 0) {
			/* stragglers if any */
			PackedBuf[PackedCount] = (uchar)(inlen-1); PackedCount++;
		    for (jdx = 0; jdx < inlen; jdx++, RawCount++, PackedCount++) {
			    PackedBuf[PackedCount] = inbuff[RawCount];
			}
		}
	}

SKIP:;

	/* return length of packed buffer */
	return PackedCount;

}


/* intel uses little endian */
/* motorola uses big endian */

/* for debugging */
ulong Intel32(ulong val)
{
	uchar buf[4];
	ulong *ptr;

  	buf[0] = (uchar) val &0xff; val >>=8;
	buf[1] = (uchar) val &0xff; val >>=8;
	buf[2] = (uchar) val &0xff; val >>=8;
	buf[3] = (uchar) val &0xff;

    ptr = (ulong *)&buf[0];
    val = ptr[0];

    return val;


}

ushort Intel16(ushort val)
{
	uchar buf[12];
	ushort *ptr;

	/* msb in largest address */
	buf[0] = (uchar) val &0xff; val >>=8;
	buf[1] = (uchar) val &0xff;

    ptr = (ushort *)&buf[0];
    val = ptr[0];

    return val;
}

/* for conversion */
ulong Motorola32(ulong val)
{
	uchar buf[4];
	ulong *ptr;

	/*

	Base Address+0   Byte3
	Base Address+1   Byte2
	Base Address+2   Byte1
	Base Address+3   Byte0

	*/

	buf[0] = (uchar) (val % 256); val = val/256;
	buf[1] = (uchar) (val % 256); val = val/256;
	buf[2] = (uchar) (val % 256); val = val/256;
	buf[3] = (uchar) (val % 256);

    /* cast back to unsigned long data type */
    ptr = (ulong *)&buf[0];
    val = ptr[0];

    return val;


}

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

/* frees memory allocated by PntAlloc (below) */
void PntFree()
{
	if (output_pnt == 0) return;

	free(RawBuf); /* packbytes list */

	switch(output_format) {
		case BROOKS_FMT:

					free(MultiPal);
					free(brooksMain);
					break;

		case PIC_FMT:
		default:
					free(picMain);
					break;
	}
}



/* allocates memory and sets-up defaults for the type of
   PNT file output that will be produced */
sshort PntAlloc()
{
	sshort status = INVALID;
	uchar buf[10];

    output_pnt = 0;

    if(suppress_pnt == 1) return INVALID;

   	if (NULL == (RawBuf = (RAWLIST *) malloc(sizeof(RAWLIST)*RAW_MAX))) {
		puts("Not Enough Memory for PackBytes... PNT Output Disabled.");
		return status;
	}

	switch(output_format) {
		case BROOKS_FMT:

					MultiPal = (MULTIPAL *) malloc(sizeof(MULTIPAL));

					if (NULL == MultiPal)
					{
						free(RawBuf);
						puts("Not Enough Memory for Multipalette... PNT Output Disabled.");
						break;
					}

					brooksMain = (PNTBROOKS *) malloc(sizeof(PNTBROOKS));

					if (NULL == brooksMain) {
						puts("Not Enough Memory for Main... PNT Output Disabled.");
						free(MultiPal);
						free(RawBuf);
						break;
					}

					/* set defaults and invariants */
					memset(&MultiPal[0].Length,0,sizeof(MULTIPAL));
					MultiPal[0].Length = Motorola32((ulong)sizeof(MULTIPAL));
					strcpy(&buf[1],"MULTIPAL"); buf[0] = 8;
					memcpy(&MultiPal[0].Kind[0],&buf[0],9);
					MultiPal[0].NumColorTables = Motorola16((ushort)200);

                    /* The 200 Palettes will be assigned when the Brooks Palettes are built
                       so nothing further to do except write to disk when done processing */

					memset(&brooksMain[0].Length,0,sizeof(PNTBROOKS));
					strcpy(&buf[1],"MAIN"); buf[0] = 4;
					memcpy(&brooksMain[0].Kind[0],&buf[0],5);
					brooksMain[0].PixelsPerScanline = Motorola16((ushort)320);
					/* as dumb as it may sound, the CiderPress file viewer needs one color table
					   to be included in the MAIN block even when a Brooks MULTIPAL is used. */

					brooksMain[0].NumColorTables = Motorola16((ushort)1);
					brooksMain[0].NumScanLines = Motorola16((ushort)200);

					/* the scbs will be setup in the scan line directory
					   with the packed lengths when the scanlines are packed... */

                    output_pnt = 1;
					status = SUCCESS;
					break;

		case PIC_FMT:
		default:
					picMain = (PNTPIC *) malloc(sizeof(PNTPIC));

					if (NULL == picMain) {
						puts("Not Enough Memory for Main... PNT Output Disabled.");
						free(RawBuf);
						break;
					}

					/* set defaults and invariants */
					memset(&picMain[0].Length,0,sizeof(PNTPIC));
					strcpy(&buf[1],"MAIN"); buf[0] = 4;
					memcpy(&picMain[0].Kind[0],&buf[0],5);
					picMain[0].PixelsPerScanline = Motorola16((ushort)320);
					picMain[0].NumColorTables = Motorola16((ushort)16);
					picMain[0].NumScanLines = Motorola16((ushort)200);

                    output_pnt = 1;
               		status = SUCCESS;
					break;
	}

	return status;
}



sshort WritePnt(FILE *fp)
{
	sshort status = SUCCESS;
	ulong blocklen = 0L;
    ushort len,y;

    if (output_pnt == 0) return status;


    /* write the header to disk as-is, then rewind, write it again
       after updating it when we are done packing scanlines etc. */
    if (output_format == BROOKS_FMT) {

		fwrite((char*)&brooksMain[0].Length,sizeof(PNTBROOKS),1,fp);
		blocklen = (ulong)sizeof(PNTBROOKS);

		for (y=0;y<200;y++) {

           len = PackBytes((char *)&shrline[y][0],160);
           fwrite((char *)&PackedBuf[0],sizeof(uchar) * len,1,fp);

           brooksMain[0].ScanLineDirectory[y][0] = Motorola16(len);
           brooksMain[0].ScanLineDirectory[y][1] = Motorola16(y);
           blocklen += len;

    	}

    	fwrite((char*)&MultiPal[0].Length,sizeof(MULTIPAL),1,fp);

    	rewind(fp);
    	brooksMain[0].Length = Motorola32(blocklen);
    	fwrite(&brooksMain[0].Length,sizeof(PNTBROOKS),1,fp);

#ifdef DEBUG
	printf("APF File Length = %ld\n",(ulong)Intel32(brooksMain[0].Length) + sizeof(MULTIPAL));
	printf("MasterMode = %d\n",Intel16(brooksMain[0].MasterMode));
	printf("Pixels per Line = %d\n",Intel16(brooksMain[0].PixelsPerScanline));
	printf("Color Tables = %d\n",Intel16(brooksMain[0].NumColorTables));
	printf("Number of Lines = %d\n",Intel16(brooksMain[0].NumScanLines));
	printf("MAIN Information Block Length = %ld\n", blocklen);
    printf("MULTIPALETTE Information Block Length = %ld\n", (ulong) sizeof(MULTIPAL));
#endif

	}
	else {

    	/* copy palette from picfile */
		memcpy(&picMain[0].ColorTable[0][0],&shr.pal[0][0],512);

		fwrite((char*)&picMain[0].Length,sizeof(PNTPIC),1,fp);
		blocklen = (ulong)sizeof(PNTPIC);

		for (y=0;y<200;y++) {

           len = PackBytes((char *)&shrline[y][0],160);
           fwrite((char *)&PackedBuf[0],sizeof(uchar) * len,1,fp);

           picMain[0].ScanLineDirectory[y][0] = Motorola16(len);
           picMain[0].ScanLineDirectory[y][1] = Motorola16((ushort)shr.scb[y]);
           blocklen += len;

    	}

    	rewind(fp);
    	picMain[0].Length = Motorola32(blocklen);
    	fwrite(&picMain[0].Length,sizeof(PNTPIC),1,fp);

#ifdef DEBUG
	printf("APF File Length = %ld\n",Intel32(picMain[0].Length ));
	printf("MasterMode = %d\n",Intel16(picMain[0].MasterMode));
	printf("Pixels per Line = %d\n",Intel16(picMain[0].PixelsPerScanline));
	printf("Color Tables = %d\n",Intel16(picMain[0].NumColorTables));
	printf("Number of Lines = %d\n",Intel16(picMain[0].NumScanLines));
	printf("MAIN Information Block Length = %ld\n", blocklen);
#endif


	}

	return status;
}



/* ***************************************************************** */
/* ========================== raw output routines ================== */
/* ***************************************************************** */


/* Brooks Palettes are stored sequentially. There are no scb's */
void BuildBrooksPaletteLine(short y, sshort midx)
{
	uchar g, *pal;
	sshort i,j;

	/* Brooks Palette Lines are in reverse order
	   the color value for color 15 is stored first.*/
	for (i = 0,j=30; i < 16;i++,j-=2) {
		g = cmap[midx][i][GREEN] << 4;
		shr.pal[y][j] = g | cmap[midx][i][BLUE];
		shr.pal[y][j+1] = cmap[midx][i][RED];
	}

	if (output_pnt == 0) return;

    /* Build the Palette for the APF when we build the Brooks Palette */
    pal = (uchar *)&MultiPal[0].ColorTableArray[y][0];

	/* According to Apple's Filetype Notes mode3200 palettes
	   are in the same order as any other palette: 0..16 */
	for (i = 0,j=0; i < 16;i++,j+=2) {
		g = cmap[midx][i][GREEN] << 4;
		pal[j] = g | cmap[midx][i][BLUE];
		pal[j+1] = cmap[midx][i][RED];
	}


}


/* this builds the shr PIC file palette as well as
   setting-up the colormap */
void SetColorIndex(sshort midx)
{
	sshort i,j,k;
	uchar r,g,b;

	for (i=0,j=0; i< 16; i++) {

	    /* read BGR triples - 48 bytes */
        b = (uchar) (bmpline[j] >> 4); j++;
        g = (uchar) (bmpline[j] >> 4); j++;
        r = (uchar) (bmpline[j] >> 4); j++;

        /* encode RGB 4-bit color into cmap */
        cmap[midx][i][RED]   = r;
        cmap[midx][i][GREEN] = g;
        cmap[midx][i][BLUE]  = b;

        /* Brooks palettes are built later */
		if (output_format == BROOKS_FMT) continue;

        /* offset from char to short */
		k = i*2;

        /* encode $0RGB motorola unsigned short (LSB, MSB) for pic file into 2 bytes */
        g = (uchar) (g << 4);
	    shr.pal[midx][k] = (uchar) (g | b);
		shr.pal[midx][k+1] = r;
	}
}


/* returns -1 if color not found in 16 color palette */
/* unfortunely for us a sequential search is required */
sshort GetColorIndex(uchar r, uchar g, uchar b, sshort midx)
{
	sshort i;

	for (i=0;i<16;i++) {
		if (cmap[midx][i][RED] == r && cmap[midx][i][GREEN] == g && cmap[midx][i][BLUE] == b) return i;
	}
	return INVALID;
}



/* this sets the scb for the shrfile */
/* it also converts the bmp line from 8 bit to 4 bit */
/* and puts it into the shr file buffer */
sshort ConvertLine(sshort y)
{

	sshort x, i, j, midx = -1;
	uchar r,g,b;

    /* convert RGB triples to 4 bit depth */
    for (i = 0; i < 960; i++) {
	     bmpline[i] = (uchar) (bmpline[i] >> 4);
    }

    /* go through every palette until we hit one that works for the whole line */
    /* try to build the line as we go */
	for (i = 0; i < numpalettes; i++) {
		/* convert to a line index in the range of 0-16 */
	    memset(svgaline,0,320);
		for (x = 0, j=0; x < 320; x++) {
		   b = bmpline[j]; j++;
		   g = bmpline[j]; j++;
		   r = bmpline[j]; j++;
		   /* just bail immediately if the palette doesn't match
		      and try the next palette */
		   midx = GetColorIndex(r,g,b,i);
		   if (midx == INVALID) break;
		   svgaline[x] = (uchar)midx;
		}
		if (midx == INVALID) continue;
		midx = i;
		break;
	}

    /* we only use the scb array for the PIC structure but.
       when we add support for PNT files we will also use
       something similar... the output format logic will be
       extended at that time.
    */
	if (midx == -1) {
		if (output_format == BROOKS_FMT) return INVALID;
	    shr.scb[y] = 0;
	    return INVALID;
	}
	if (output_format == BROOKS_FMT) BuildBrooksPaletteLine(y,midx);
	else shr.scb[y] = midx;


	/* create 4 bit line - pixel order nibbles */
	for (i = 0,j=1,x = 0; x < 160; x++,i+=2,j+=2) {
        shrline[y][x] = (uchar) ((svgaline[i] << 4) | svgaline[j]);
    }

return SUCCESS;

}

sshort Convert()
{

    FILE *fp, *fpshr, *fpapf;
    sshort status = INVALID, i, y, bmpversion;
    uchar ch;

    if((fp=fopen(bmpfile,"rb"))==NULL) {
		printf("Error Opening %s!\n",bmpfile);
		return status;
	}

    /* read the header stuff into the appropriate structures */
    fread((char *)&bfi.bfType,
	             sizeof(BITMAPFILEHEADER),1,fp);
    fread((char *)&bmi.biSize,
                 sizeof(BITMAPINFOHEADER),1,fp);

	if (bmi.biSize != sizeof(BITMAPINFOHEADER)) {
		if (bmi.biSize == sizeof(BITMAPINFOHEADERV4)) {
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

	if (bmpversion == 0) {
		fclose(fp);
		printf("BMP version of %s not recognized!\n",bmpfile);
		return status;
	}

    if (bmi.biCompression==BI_RGB &&
        bfi.bfType[0] == 'B' && bfi.bfType[1] == 'M' &&
        bmi.biPlanes==1 && bmi.biBitCount == 24) {
			if (bmi.biWidth == 320 && bmi.biHeight == 200) status = SUCCESS;
	}

	if (status == INVALID) {
		printf("%s is in the wrong format!\n",bmpfile);
		fclose(fp);
		return status;
	}

    /* seek past extraneous info in header if any */
	fseek(fp,bfi.bfOffBits,SEEK_SET);

	for (i = 0,y=199; i< 200; i++, y--) {
		fread((char *)&bmpline[0],1,960,fp);
		if (ConvertLine(y) == INVALID) {
		   printf("No palette for line %d\n",y);
		   status = INVALID;
	    }
	}
	fclose(fp);

	/* insert RLE routines here */
    /* the buffers are ready to be written by the time it gets to this point */

    if((fpshr=fopen(shrfile,"wb"))==NULL) {
		printf("Error Opening %s!\n",shrfile);
		return INVALID;
	}

    if (output_pnt != 0) {
    	if(NULL == (fpapf=fopen(pntfile,"wb+"))) {
			printf("Error Opening %s!\n",pntfile);
			PntFree();
			output_pnt = 0;
		}
	}



    switch(output_format) {

		case BROOKS_FMT:
		         fwrite((char *)&shrline[0][0],1,32000,fpshr);
		         fwrite((char *)&shr.pal[0][0],1,sizeof(uchar) * 6400,fpshr);
		         break;

		case PIC_FMT:
		default:
    			fwrite((char *)&shrline[0][0],1,32000,fpshr);
    			fwrite((char *)&shr.scb[0],sizeof(uchar) * 768,1,fpshr);
    			break;
	}

    fclose(fpshr);

    if (suppress_pic == 1) remove(shrfile);
    else printf("Created %s!\n",shrfile);

    if (output_pnt != 0) {
    	if (WritePnt(fpapf) == SUCCESS) {
			printf("Created %s!\n",pntfile);
		}
		fclose(fpapf);
	}

	return SUCCESS;

}

sshort ReadColorMap()
{
    FILE *fp;
    sshort status = INVALID,i,x,y,localpalettes, bmpversion;

    /* open the colormap */
    if((fp=fopen(cmapfile,"rb"))==NULL) {
		printf("Error Opening %s!\n",cmapfile);
		return status;
	}

    /* read the header stuff into the appropriate structures */
    fread((char *)&bfi.bfType,
	             sizeof(BITMAPFILEHEADER),1,fp);
    fread((char *)&bmi.biSize,
                 sizeof(BITMAPINFOHEADER),1,fp);

	if (bmi.biSize != sizeof(BITMAPINFOHEADER)) {
		if (bmi.biSize == sizeof(BITMAPINFOHEADERV4)) {
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

	if (bmpversion == 0) {
		fclose(fp);
		printf("BMP version of %s not recognized!\n",bmpfile);
		return status;
	}

    if (bmi.biCompression==BI_RGB &&
        bfi.bfType[0] == 'B' && bfi.bfType[1] == 'M' &&
        bmi.biPlanes==1 && bmi.biBitCount == 24 && bmi.biWidth == 16) {
			if (bmi.biHeight == 1) {
				output_format = PIC_FMT;
				numpalettes = 16;
				localpalettes = 1;
				status = SUCCESS;
			}

			if (bmi.biHeight == 16) {
				output_format = PIC_FMT;
				numpalettes = localpalettes = 16;
				status = SUCCESS;
			}
			else if (bmi.biHeight == 200) {
				output_format = BROOKS_FMT;
				/* this will likely be adjusted down
				   when the palette is read */
				numpalettes = 200;
				strcpy(shrfile,brooksfile);
				status = SUCCESS;
			}
	}

	if (status == INVALID) {
		printf("%s is in the wrong format!\n",cmapfile);
		fclose(fp);
		return status;
	}

	PntAlloc();

    /* clear the memory for the shr file buffers */
    memset((char *)&shrline[0][0],0,32000);
    memset((char *)&shr.scb[0],0,sizeof(PICFILE));

    /* seek past extraneous info in header if any */
	fseek(fp,bfi.bfOffBits,SEEK_SET);

    /* set palettes for the SHR file */
    switch(output_format) {
		case BROOKS_FMT:
		       numpalettes = i = 0;
		       for (y=0;y<200;y++) {
				   fread((char *)&bmpline[0],1,48,fp);
				   /* skip blank lines at the bottom of the image */
				   /* a black line with an all black palette will
				      end-up as a black line with an all black palette
				      anwyay... obviously */
				   for (x = 0; x < 48; x++) {
					   if (bmpline[x] != 0) {
								SetColorIndex(i); i++;
								numpalettes++;
								break;
					   }
				   }
			   }
			   /* printf("Number of palettes is %d\n",numpalettes); */
			   break;


		case PIC_FMT:
		default:
		    for (i=0;i<16;i++) {
				if (i < localpalettes) fread((char *)&bmpline[0],1,48,fp);
				SetColorIndex(i);
			}


	}
	fclose(fp);

return status;
}


int main(int argc, char **argv)
{
	sshort idx, jdx=999, status = 0;
	uchar fname[256], ch, ch2, *wordptr;

    /* defaults */
	suppress_pnt = 1;
	suppress_pic = 0;
	no_tags = 1;

    /* getopts */
    if (argc > 2) {
    	for (idx = 2; idx < argc; idx++) {
	   		wordptr = (uchar *)&argv[idx][0];
	   		ch2 = ch = toupper(wordptr[0]);
	   		if (ch == '-') {
		   		wordptr = (uchar *)&argv[idx][1];
       			ch2 = ch  = toupper(wordptr[0]);
			}
			if (ch != 0) ch2 = toupper(wordptr[1]);

			if (ch == 'A' || ch2 == 'A') {
			   suppress_pnt = 0; suppress_pic = 1;
			}
			if (ch == 'T' || ch2 == 'T') {
			   no_tags = 0;
			}

		}
	   	if (suppress_pnt == 0 || no_tags == 0) argc = 2;
	}


    if (argc == 2) {
		strcpy(fname, argv[1]);
		/* check fullname _proc.bmp */
		for (idx = 0; fname[idx] != (uchar)0; idx++) {
			ch = fname[idx];
			if (ch != '_') continue;
			ch = toupper(fname[idx+1]);
			if (ch != 'P') continue;
			ch = toupper(fname[idx+2]);
			if (ch != 'R') continue;
			ch = toupper(fname[idx+3]);
			if (ch != 'O') continue;
			ch = toupper(fname[idx+4]);
			if (ch != 'C') continue;
			ch = toupper(fname[idx+5]);
			if (ch != '.') continue;
			ch = toupper(fname[idx+6]);
			if (ch != 'B') continue;
			ch = toupper(fname[idx+7]);
			if (ch != 'M') continue;
			ch = toupper(fname[idx+8]);
			if (ch != 'P') continue;
			fname[idx] = (uchar)0;
			break;
		}


		if (jdx == 999) {
			/* check basename */
			for (idx = 0; fname[idx] != (uchar)0; idx++) {
				ch = fname[idx];
				if (ch != '.') continue;
				ch = toupper(fname[idx+1]);
				if (ch != 'B' && ch != 'D') continue;
				ch = toupper(fname[idx+2]);
				if (ch != 'M' && ch != 'I') continue;
				ch = toupper(fname[idx+3]);
				if (ch != 'P' && ch != 'B') continue;
				jdx = idx;
			}
		}
	 }

#ifdef MSDOS
	    if (jdx != 999) fname[jdx] = (uchar)0;
#else
		if (jdx !=999) argc = 1;
#endif

	if(argc != 2) {
		puts("MagickToShr (C) Copyright Jonas Grönhagen and Bill Buckels 2014.");
        puts("All Rights Reserved.");
		puts("Usage is: m2s BaseName -Options");
		puts("BaseName: _proc.bmp and _palette.bmp (file pairs)");
		puts("          \"m2s Woz\" opens \"Woz_proc.bmp\" and \"Woz_palette.bmp\"...");
		puts("          For MS-DOS, \"M2S16 WOZ\" opens \"WOZ.BMP\" and \"WOZ.DIB!\"");
		puts("          16 palettes for mode320 output and 200 for mode3200!");
		puts("Options:  -A = Alternate PNT file output (run length encoded).");
		puts("               Default: raw SHR (mode320) or SH3 (mode3200).");
		puts("          -T = Use CiderPress Attribute Preservation Tags.");
		puts("               Default: No Tags! (unadorned file extensions)");
		puts("               Does not apply to M2S16.EXE (MS-DOS binary).");
		puts("          Options may be combined: \"-ta\" or \"-at\"");
		puts("          Options are Case Insensitive - Switchar \"-\" is Optional.");
		return(1);
	}

#ifdef MSDOS
    /* MS-DOS uses short file names - 8.3 */
    sprintf(bmpfile,"%s.BMP",fname);
    sprintf(cmapfile,"%s.DIB",fname);
    sprintf(shrfile,"%s.SHR",fname);
    sprintf(brooksfile,"%s.SH3",fname);
    sprintf(pntfile,"%s.PNT",fname);
 #else
    /* if using long file names add
       ciderpress file attribute preservation tags
       to output files */
    sprintf(bmpfile,"%s_proc.bmp",fname);
    sprintf(cmapfile,"%s_palette.bmp",fname);

if (no_tags == 1) {
    sprintf(shrfile,"%s.SHR",fname);
    sprintf(brooksfile,"%s.SH3",fname);
    sprintf(pntfile,"%s.PNT",fname);
}
else {
    sprintf(shrfile,"%s.SHR#C10000",fname);
    sprintf(brooksfile,"%s.SH3#C10002",fname);
    sprintf(pntfile,"%s.PNT#C00002",fname);
}

#endif

    for (;;) {
    	if ((status = ReadColorMap()) == INVALID) break;

    	status = Convert();
    	break;
	}

	PntFree();
    if (status == INVALID) return (1);

return SUCCESS;
}

