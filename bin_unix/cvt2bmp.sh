#!/bin/sh
# cvt2bmp.sh - uses ImageMagick to produce 320 x 200 Input Files for A2B
 
# set path to ImageMagick binaries below
# for ImageMagick version 6.9 use the following
# MAGICK="c:/IM/convert"
# for ImageMagick version 7 use the following
MAGICK="c:/IM/magick"
 
# For most modern Linux distributions, all ImageMagick binaries are located in: /usr/bin/ (see below)
# for ImageMagick version 6.9 use the following
# MAGICK="/usr/bin/convert"
# for ImageMagick version 7 use the following
# MAGICK="/usr/bin/magick"
 
if [ ! -e BMP320 ]; then
     mkdir BMP320
fi  
if [ ! -e ORGBMP ]; then
     mkdir ORGBMP
fi  
if [ ! -e jpg ]; then
     mkdir jpg
fi  
 
# create GIF input files for native mode converters if needed.
# if [ ! -e gif ]; then
#    mkdir gif
# fi
 
# assumes that all input files are 4:3 Aspect Ratio
# accepts jpegs, pngs, gifs, or BMPs as input files
# converts everything to truecolor BMP before scaling to avoid color loss
 
criteria="*.jpg"
if ls ./*.jpg 1> /dev/null 2>&1 ; then
for i in $( ls $criteria);
do
   src=$i
   tgt=$(basename $i .jpg)
   $MAGICK $src -type truecolor -units PixelsPerInch -density 72 -compress None -depth 24 -define format:BMP3 $tgt.bmp
   mv $src ./jpg/
done
fi
criteria="*.png"
if ls ./*.png 1> /dev/null 2>&1 ; then
for i in $( ls $criteria);
do
   src=$i
   tgt=$(basename $i .png)
   $MAGICK $src -type truecolor -units PixelsPerInch -density 72 -compress None -depth 24 -define format:BMP3 $tgt.bmp
   mv $src ./jpg/
done
fi
criteria="*.gif"
if ls ./*.gif 1> /dev/null 2>&1 ; then
for i in $( ls $criteria);
do
   src=$i
   tgt=$(basename $i .gif)
   $MAGICK $src -type truecolor -units PixelsPerInch -density 72 -compress None -depth 24 -define format:BMP3 $tgt.bmp
   mv $src ./jpg/
done
fi
 
# create A2B's input files - scaling is done here.
criteria="*.bmp"
if ls ./*.bmp 1> /dev/null 2>&1 ; then
for i in $( ls $criteria);
do
   src=$i
   tgt=$(basename $i .bmp)
   $MAGICK $src -resize "320x200!" -type truecolor -units PixelsPerInch -density 72 -compress None -depth 24 -define format:BMP3 BMP320//$tgt.bmp
   # create GIF input files for native mode converters if needed.
   # $MAGICK $src -colors 256 -define format:GIF gif//"$tgt"x.gif
done
mv *.bmp ./ORGBMP/
mv BMP320/*.bmp ./
fi
 
# this is the end
