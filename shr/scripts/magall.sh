#!/bin/sh
# magall.sh - post process the output from the A2B converter 
# use ImageMagick to make double scaled preview files for review in a separate subdirectory
# these can be viewed as a slide-show under Windows
# copy SHR output to a separate subdirectory (with no additional files) for preparing Apple IIgs slide-shows
# CiderPress can then be used more easily to place SHR files on an Apple IIgs Disk Image

# set paths to ImageMagick below 
# for ImageMagick version 6.9 use ImageMagick's convert utility
# MAGICK="C:/IM/convert"
# for ImageMagick version 7 use the following 
MAGICK="C:/IM/magick"
# For most modern Linux distributions, all ImageMagick binaries are located in: /usr/bin/ (see below)
# for ImageMagick version 6.9
# MAGICK="/usr/bin/convert"
# for ImageMagick version 7
# MAGICK="/usr/bin/magick"

cd SH30709raw
if [ ! -e png ]; then
	mkdir png
fi	
if [ ! -e shr ]; then
	mkdir shr
fi	
if ls ./*_proc.bmp 1> /dev/null 2>&1 ; then
criteria="*_proc.bmp"
for i in $( ls $criteria); do
   src=$i
   tgt=$(basename $i .bmp)
   if [ ! -e ./png/$src ]; then
   	$MAGICK $src -magnify -define format:PNG png//$tgt.png
   fi
 done
criteria="*.SH*"
for i in $( ls $criteria); do
   src=$i
   if [ ! -e ./shr/$src ]; then
   	cp $src shr/$src
   fi
 done
fi
cd ..
cd SH32709raw
if [ ! -e png ]; then
	mkdir png
fi	
if [ ! -e shr ]; then
	mkdir shr
fi	
if ls ./*_proc.bmp 1> /dev/null 2>&1 ; then
criteria="*_proc.bmp"
for i in $( ls $criteria); do
   src=$i
   tgt=$(basename $i .bmp)
   if [ ! -e ./png/$src ]; then
   	$MAGICK $src -magnify -define format:PNG png//$tgt.png
   fi
 done
criteria="*.SH*"
for i in $( ls $criteria); do
   src=$i
   if [ ! -e ./shr/$src ]; then
   	cp $src shr/$src
   fi
 done
fi
cd ..
cd SH33709raw
if [ ! -e png ]; then
	mkdir png
fi	
if [ ! -e shr ]; then
	mkdir shr
fi	
if ls ./*_proc.bmp 1> /dev/null 2>&1 ; then
criteria="*_proc.bmp"
for i in $( ls $criteria); do
   src=$i
   tgt=$(basename $i .bmp)
   if [ ! -e ./png/$src ]; then
   	$MAGICK $src -magnify -define format:PNG png//$tgt.png
   fi
 done
criteria="*.SH*"
for i in $( ls $criteria); do
   src=$i
   if [ ! -e ./shr/$src ]; then
   	cp $src shr/$src
   fi
 done
fi
cd ..
cd sh33709
if [ ! -e png ]; then
	mkdir png
fi	
if [ ! -e shr ]; then
	mkdir shr
fi	
if ls ./*_proc.bmp 1> /dev/null 2>&1 ; then
criteria="*_proc.bmp"
for i in $( ls $criteria); do
   src=$i
   tgt=$(basename $i .bmp)
   if [ ! -e ./png/$src ]; then
   	$MAGICK $src -magnify -define format:PNG png//$tgt.png
   fi
 done
criteria="*.SH*"
for i in $( ls $criteria); do
   src=$i
   if [ ! -e ./shr/$src ]; then
   	cp $src shr/$src
   fi
 done
fi
cd ..
cd sh32709
if [ ! -e png ]; then
	mkdir png
fi	
if [ ! -e shr ]; then
	mkdir shr
fi	
if ls ./*_proc.bmp 1> /dev/null 2>&1 ; then
criteria="*_proc.bmp"
for i in $( ls $criteria); do
   src=$i
   tgt=$(basename $i .bmp)
   if [ ! -e ./png/$src ]; then
   	$MAGICK $src -magnify -define format:PNG png//$tgt.png
   fi
 done
criteria="*.SH*"
for i in $( ls $criteria); do
   src=$i
   if [ ! -e ./shr/$src ]; then
   	cp $src shr/$src
   fi
 done
fi
cd ..
cd sh30709
if [ ! -e png ]; then
	mkdir png
fi	
if [ ! -e shr ]; then
	mkdir shr
fi	
if ls ./*_proc.bmp 1> /dev/null 2>&1 ; then
criteria="*_proc.bmp"
for i in $( ls $criteria); do
   src=$i
   tgt=$(basename $i .bmp)
   if [ ! -e ./png/$src ]; then
   	$MAGICK $src -magnify -define format:PNG png//$tgt.png
   fi
 done
criteria="*.SH*"
for i in $( ls $criteria); do
   src=$i
   if [ ! -e ./shr/$src ]; then
   	cp $src shr/$src
   fi
 done
fi
cd ..
# this is the end

