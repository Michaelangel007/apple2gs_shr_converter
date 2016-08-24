#!/bin/sh
# pad43.sh - pad all images to 4:3 aspect BMP (default usage)
# optional usage "pad43.sh CROP" - center-crop all images to 4:3 aspect BMP
 
# set path to ImageMagick binaries below
# for ImageMagick version 6.9 use the following
# MAGICK="c:/IM/convert"
# IDENTIFY="c:/IM/identify"
# for ImageMagick version 7 use the following
MAGICK="c:/IM/magick"
IDENTIFY="c:/IM/magick identify"
 
# For most modern Linux distributions, all ImageMagick binaries are located in: /usr/bin/ (see below)
# for ImageMagick version 6.9 use the following
# MAGICK="/usr/bin/convert"
# IDENTIFY="/usr/bin/identify"
# for ImageMagick version 7 use the following
# MAGICK="/usr/bin/magick"
# IDENTIFY="/usr/bin/magick identify"
 
# set path to A2B converter below
A2B="C:/SHR/A2B2016/a2b"
# For most modern Linux distributions, consider locating A2B in: /usr/bin/ (see below)
# A2B="/usr/bin/a2b"
 
# default usage
method="p43s"
# optional usage
if [ ! -z "$1" ]; then
  if [ "$1" == "CROP" ]; then
    method="a43s"
  fi 
  if [ "$1" == "crop" ]; then
    method="a43s"
  fi   
fi
 
criteria="*.jpg"
if ls ./*.jpg 1> /dev/null 2>&1 ; then
for i in $( ls $criteria);
do
   src=$i
   tgt=$(basename $i .jpg)
   $IDENTIFY $src > $tgt.id
   $A2B $src $method
   chmod 777 $tgt.sh
   ./$tgt.sh $MAGICK $src $tgt
   rm $tgt.id
   rm $tgt.sh
   mv $src ./jpg/
done
fi
criteria="*.png"
if ls ./*.png 1> /dev/null 2>&1 ; then
for i in $( ls $criteria);
do
   src=$i
   tgt=$(basename $i .png)
   $IDENTIFY $src > $tgt.id
   $A2B $src $method
   chmod 777 $tgt.sh
   ./$tgt.sh $MAGICK $src $tgt
   rm $tgt.id
   rm $tgt.sh
   mv $src ./jpg/
done
fi
criteria="*.gif"
if ls ./*.gif 1> /dev/null 2>&1 ; then
for i in $( ls $criteria);
do
   src=$i
   tgt=$(basename $i .gif)
   $IDENTIFY $src > $tgt.id
   $A2B $src $method
   chmod 777 $tgt.sh
   ./$tgt.sh $MAGICK $src $tgt
   rm $tgt.id
   rm $tgt.sh
   mv $src ./jpg/
done
fi
 
# this is the end
