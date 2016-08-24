#!/bin/sh
# crop1440.sh - center-crop jpeg 1920 x 1080 hd wallpaper to 4:3 aspect BMP
# set path to ImageMagick below
# for ImageMagick version 6.9 use the following
# MAGICK="c:/IM/convert"
# for ImageMagick version 7 use the following
MAGICK="c:/IM/magick"
# For most modern Linux distributions, all ImageMagick binaries are located in: /usr/bin/ (see below)
# for ImageMagick version 6.9 use the following
# MAGICK="/usr/bin/convert"
# for ImageMagick version 7 use the following
# MAGICK="/usr/bin/magick"
criteria="*.jpg"
if ls ./*.jpg 1> /dev/null 2>&1 ; then
  if [ ! -e jpg ]; then
    mkdir jpg
  fi
for i in $( ls $criteria);
do
   src=$i
   tgt=$(basename $i .jpg)
   $MAGICK $src -gravity center -crop 1440x1080+0+0 -define format:BMP $tgt.bmp
   mv $src ./jpg/
done
fi
