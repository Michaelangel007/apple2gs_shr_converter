#!/bin/sh
# cvt.sh - creates 6 variations of 320 x 200 SHR files using A2B and external segmented palettes
 
# set path to A2B converter below
A2B="F:/SHR/A2B2015/a2b"
# For most modern Linux distributions, consider locating A2B in: /usr/bin/ (see below)
# A2B="/usr/bin/a2b"
 
if [ ! -e SH30709 ]; then
     mkdir SH30709
fi  
if [ ! -e SH32709 ]; then
     mkdir SH32709
fi  
if [ ! -e SH33709 ]; then
     mkdir SH33709
fi  
if [ ! -e SH30709raw ]; then
     mkdir SH30709raw
fi  
if [ ! -e SH32709raw ]; then
     mkdir SH32709raw
fi  
if [ ! -e SH33709raw ]; then
     mkdir SH33709raw
fi  
 
if [ ! -e todo ]; then
     mkdir todo
fi  
if [ ! -e done ]; then
     mkdir done
fi
 
# make External Segmented Palettes using ImageMagick
./slicer.sh
 
# Call A2B to create SHR files
criteria="*.bmp"
if ls ./*.bmp 1> /dev/null 2>&1 ; then
for i in $( ls $criteria); do
   src=$i
   tgt=$(basename $i .bmp)
 
   $A2B $src SH30709/$src dr m2s t PIMsh0pcx/$tgt/foo sum l709 > /dev/null
   $A2B $src SH32709/$src dr m2s t PIMsh2pcx/$tgt/foo sum l709 > /dev/null
   $A2B $src SH33709/$src dr m2s t PIMsh3pcx/$tgt/foo sum l709 > /dev/null
 
   $A2B $src SH30709raw/$src m2s t PIMsh0pcx/$tgt/foo sum l709 > /dev/null
   $A2B $src SH32709raw/$src m2s t PIMsh2pcx/$tgt/foo sum l709 > /dev/null
   $A2B $src SH33709raw/$src m2s t PIMsh3pcx/$tgt/foo sum l709 > /dev/null
done
mv *.bmp ./done/
fi
 
# this is the end
