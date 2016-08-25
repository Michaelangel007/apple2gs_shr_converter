#!/bin/sh
# slicer.sh - shell script to produce External Segment Palette Files for A2B 

# set paths to ImageMagick below 
# for ImageMagick version 6.9 use ImageMagick's convert utility
# MAGICK="C:/IM/convert"
# for ImageMagick version 7 use the following 
#MAGICK="C:/IM/magick"

# For most modern Linux distributions, all ImageMagick binaries are located in: /usr/bin/ (see below)
# for ImageMagick version 6.9
# MAGICK="/usr/bin/convert"
# for ImageMagick version 7
# MAGICK="/usr/bin/magick"
MAGICK=`which magick`
IDENTIFY=`which magick` identify

if [ ! -z "${MAGICK}" ]; then
   echo "ERROR: Image Magick isn't installed."
   exit 1
fi

# create directory structure for 16 color image segments
if [ ! -e sh0pcx ]; then
	mkdir sh0pcx
fi	
#if [ ! -e sh1pcx ]; then
#	mkdir sh1pcx
#fi	
if [ ! -e sh2pcx ]; then
	mkdir sh2pcx
fi	
if [ ! -e sh3pcx ]; then
	mkdir sh3pcx
fi	

# segment directories are of the same basename as the bmp being converted
# the segments themselves are 0.pcx 1.pcx, 2.pcx, etc.
criteria="*.bmp"
if ls ./*.bmp 1> /dev/null 2>&1 ; then
for i in $( ls $criteria); do
   tgt=$(basename $i .bmp)
   if [ ! -e ./sh0pcx/$tgt ]; then
   	mkdir sh0pcx/$tgt
   fi
#  if [ ! -e ./sh1pcx/$tgt ]; then
#   	mkdir sh1pcx/$tgt
#  fi	
   if [ ! -e ./sh2pcx/$tgt ]; then
   	mkdir sh2pcx/$tgt
   fi	
   if [ ! -e ./sh3pcx/$tgt ]; then
   	mkdir sh3pcx/$tgt
   fi	   
 done
fi

# 8-segments for multi-palette for motion video
# if ls ./*.bmp 1> /dev/null 2>&1 ; then
# for i in $( ls $criteria); do
#    src=$i
#    tgt=$(basename $i .bmp)
#    $MAGICK $src -crop 320x25+0+175 -dither FloydSteinberg -colors 16 -define format:PCX sh1pcx//$tgt//0.pcx
#    $MAGICK $src -crop 320x25+0+150 -dither FloydSteinberg -colors 16 -define format:PCX sh1pcx//$tgt//1.pcx
#    $MAGICK $src -crop 320x25+0+125 -dither FloydSteinberg -colors 16 -define format:PCX sh1pcx//$tgt//2.pcx
#    $MAGICK $src -crop 320x25+0+100 -dither FloydSteinberg -colors 16 -define format:PCX sh1pcx//$tgt//3.pcx
#    $MAGICK $src -crop 320x25+0+75 -dither FloydSteinberg -colors 16 -define format:PCX sh1pcx//$tgt//4.pcx
#    $MAGICK $src -crop 320x25+0+50 -dither FloydSteinberg -colors 16 -define format:PCX sh1pcx//$tgt//5.pcx
#    $MAGICK $src -crop 320x25+0+25 -dither FloydSteinberg -colors 16 -define format:PCX sh1pcx//$tgt//6.pcx
#    $MAGICK $src -crop 320x25+0+0 -dither FloydSteinberg -colors 16 -define format:PCX sh1pcx//$tgt//7.pcx
#  done
# fi

# use the same segments currently used in A2B

# 1-segment for single palette SHR 
if ls ./*.bmp 1> /dev/null 2>&1 ; then
for i in $( ls $criteria); do
   src=$i
   tgt=$(basename $i .bmp)
   $MAGICK $src -dither FloydSteinberg -colors 16 -define format:PCX sh0pcx//$tgt//0.pcx
 done
fi

# 16-segments for multi-palette
if ls ./*.bmp 1> /dev/null 2>&1 ; then
for i in $( ls $criteria); do
   src=$i
   tgt=$(basename $i .bmp)
   $MAGICK $src -crop 320x13+0+187 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx//$tgt//0.pcx
   $MAGICK $src -crop 320x12+0+175 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx//$tgt//1.pcx
   $MAGICK $src -crop 320x13+0+162 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx//$tgt//2.pcx
   $MAGICK $src -crop 320x12+0+150 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx//$tgt//3.pcx
   $MAGICK $src -crop 320x13+0+137 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx//$tgt//4.pcx
   $MAGICK $src -crop 320x12+0+125 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx//$tgt//5.pcx
   $MAGICK $src -crop 320x13+0+112 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx//$tgt//6.pcx
   $MAGICK $src -crop 320x12+0+100 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx//$tgt//7.pcx
   $MAGICK $src -crop 320x13+0+87 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx//$tgt//8.pcx
   $MAGICK $src -crop 320x12+0+75 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx//$tgt//9.pcx
   $MAGICK $src -crop 320x13+0+62 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx//$tgt//10.pcx
   $MAGICK $src -crop 320x12+0+50 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx//$tgt//11.pcx
   $MAGICK $src -crop 320x13+0+37 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx//$tgt//12.pcx
   $MAGICK $src -crop 320x12+0+25 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx//$tgt//13.pcx
   $MAGICK $src -crop 320x13+0+12 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx//$tgt//14.pcx
   $MAGICK $src -crop 320x12+0+0 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx//$tgt//15.pcx
 done
fi

# 200-segments for brooks
if ls ./*.bmp 1> /dev/null 2>&1 ; then
for i in $( ls $criteria); do
   src=$i
   tgt=$(basename $i .bmp)
   $MAGICK $src -crop 320x1+0+199 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//0.pcx
   $MAGICK $src -crop 320x1+0+198 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//1.pcx
   $MAGICK $src -crop 320x1+0+197 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//2.pcx
   $MAGICK $src -crop 320x1+0+196 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//3.pcx
   $MAGICK $src -crop 320x1+0+195 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//4.pcx
   $MAGICK $src -crop 320x1+0+194 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//5.pcx
   $MAGICK $src -crop 320x1+0+193 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//6.pcx
   $MAGICK $src -crop 320x1+0+192 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//7.pcx
   $MAGICK $src -crop 320x1+0+191 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//8.pcx
   $MAGICK $src -crop 320x1+0+190 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//9.pcx
   $MAGICK $src -crop 320x1+0+189 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//10.pcx
   $MAGICK $src -crop 320x1+0+188 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//11.pcx
   $MAGICK $src -crop 320x1+0+187 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//12.pcx
   $MAGICK $src -crop 320x1+0+186 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//13.pcx
   $MAGICK $src -crop 320x1+0+185 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//14.pcx
   $MAGICK $src -crop 320x1+0+184 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//15.pcx
   $MAGICK $src -crop 320x1+0+183 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//16.pcx
   $MAGICK $src -crop 320x1+0+182 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//17.pcx
   $MAGICK $src -crop 320x1+0+181 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//18.pcx
   $MAGICK $src -crop 320x1+0+180 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//19.pcx
   $MAGICK $src -crop 320x1+0+179 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//20.pcx
   $MAGICK $src -crop 320x1+0+178 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//21.pcx
   $MAGICK $src -crop 320x1+0+177 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//22.pcx
   $MAGICK $src -crop 320x1+0+176 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//23.pcx
   $MAGICK $src -crop 320x1+0+175 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//24.pcx
   $MAGICK $src -crop 320x1+0+174 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//25.pcx
   $MAGICK $src -crop 320x1+0+173 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//26.pcx
   $MAGICK $src -crop 320x1+0+172 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//27.pcx
   $MAGICK $src -crop 320x1+0+171 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//28.pcx
   $MAGICK $src -crop 320x1+0+170 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//29.pcx
   $MAGICK $src -crop 320x1+0+169 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//30.pcx
   $MAGICK $src -crop 320x1+0+168 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//31.pcx
   $MAGICK $src -crop 320x1+0+167 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//32.pcx
   $MAGICK $src -crop 320x1+0+166 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//33.pcx
   $MAGICK $src -crop 320x1+0+165 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//34.pcx
   $MAGICK $src -crop 320x1+0+164 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//35.pcx
   $MAGICK $src -crop 320x1+0+163 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//36.pcx
   $MAGICK $src -crop 320x1+0+162 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//37.pcx
   $MAGICK $src -crop 320x1+0+161 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//38.pcx
   $MAGICK $src -crop 320x1+0+160 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//39.pcx
   $MAGICK $src -crop 320x1+0+159 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//40.pcx
   $MAGICK $src -crop 320x1+0+158 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//41.pcx
   $MAGICK $src -crop 320x1+0+157 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//42.pcx
   $MAGICK $src -crop 320x1+0+156 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//43.pcx
   $MAGICK $src -crop 320x1+0+155 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//44.pcx
   $MAGICK $src -crop 320x1+0+154 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//45.pcx
   $MAGICK $src -crop 320x1+0+153 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//46.pcx
   $MAGICK $src -crop 320x1+0+152 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//47.pcx
   $MAGICK $src -crop 320x1+0+151 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//48.pcx
   $MAGICK $src -crop 320x1+0+150 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//49.pcx
   $MAGICK $src -crop 320x1+0+149 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//50.pcx
   $MAGICK $src -crop 320x1+0+148 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//51.pcx
   $MAGICK $src -crop 320x1+0+147 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//52.pcx
   $MAGICK $src -crop 320x1+0+146 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//53.pcx
   $MAGICK $src -crop 320x1+0+145 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//54.pcx
   $MAGICK $src -crop 320x1+0+144 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//55.pcx
   $MAGICK $src -crop 320x1+0+143 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//56.pcx
   $MAGICK $src -crop 320x1+0+142 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//57.pcx
   $MAGICK $src -crop 320x1+0+141 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//58.pcx
   $MAGICK $src -crop 320x1+0+140 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//59.pcx
   $MAGICK $src -crop 320x1+0+139 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//60.pcx
   $MAGICK $src -crop 320x1+0+138 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//61.pcx
   $MAGICK $src -crop 320x1+0+137 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//62.pcx
   $MAGICK $src -crop 320x1+0+136 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//63.pcx
   $MAGICK $src -crop 320x1+0+135 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//64.pcx
   $MAGICK $src -crop 320x1+0+134 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//65.pcx
   $MAGICK $src -crop 320x1+0+133 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//66.pcx
   $MAGICK $src -crop 320x1+0+132 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//67.pcx
   $MAGICK $src -crop 320x1+0+131 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//68.pcx
   $MAGICK $src -crop 320x1+0+130 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//69.pcx
   $MAGICK $src -crop 320x1+0+129 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//70.pcx
   $MAGICK $src -crop 320x1+0+128 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//71.pcx
   $MAGICK $src -crop 320x1+0+127 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//72.pcx
   $MAGICK $src -crop 320x1+0+126 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//73.pcx
   $MAGICK $src -crop 320x1+0+125 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//74.pcx
   $MAGICK $src -crop 320x1+0+124 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//75.pcx
   $MAGICK $src -crop 320x1+0+123 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//76.pcx
   $MAGICK $src -crop 320x1+0+122 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//77.pcx
   $MAGICK $src -crop 320x1+0+121 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//78.pcx
   $MAGICK $src -crop 320x1+0+120 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//79.pcx
   $MAGICK $src -crop 320x1+0+119 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//80.pcx
   $MAGICK $src -crop 320x1+0+118 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//81.pcx
   $MAGICK $src -crop 320x1+0+117 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//82.pcx
   $MAGICK $src -crop 320x1+0+116 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//83.pcx
   $MAGICK $src -crop 320x1+0+115 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//84.pcx
   $MAGICK $src -crop 320x1+0+114 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//85.pcx
   $MAGICK $src -crop 320x1+0+113 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//86.pcx
   $MAGICK $src -crop 320x1+0+112 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//87.pcx
   $MAGICK $src -crop 320x1+0+111 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//88.pcx
   $MAGICK $src -crop 320x1+0+110 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//89.pcx
   $MAGICK $src -crop 320x1+0+109 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//90.pcx
   $MAGICK $src -crop 320x1+0+108 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//91.pcx
   $MAGICK $src -crop 320x1+0+107 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//92.pcx
   $MAGICK $src -crop 320x1+0+106 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//93.pcx
   $MAGICK $src -crop 320x1+0+105 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//94.pcx
   $MAGICK $src -crop 320x1+0+104 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//95.pcx
   $MAGICK $src -crop 320x1+0+103 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//96.pcx
   $MAGICK $src -crop 320x1+0+102 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//97.pcx
   $MAGICK $src -crop 320x1+0+101 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//98.pcx
   $MAGICK $src -crop 320x1+0+100 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//99.pcx
   $MAGICK $src -crop 320x1+0+99 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//100.pcx
   $MAGICK $src -crop 320x1+0+98 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//101.pcx
   $MAGICK $src -crop 320x1+0+97 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//102.pcx
   $MAGICK $src -crop 320x1+0+96 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//103.pcx
   $MAGICK $src -crop 320x1+0+95 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//104.pcx
   $MAGICK $src -crop 320x1+0+94 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//105.pcx
   $MAGICK $src -crop 320x1+0+93 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//106.pcx
   $MAGICK $src -crop 320x1+0+92 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//107.pcx
   $MAGICK $src -crop 320x1+0+91 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//108.pcx
   $MAGICK $src -crop 320x1+0+90 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//109.pcx
   $MAGICK $src -crop 320x1+0+89 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//110.pcx
   $MAGICK $src -crop 320x1+0+88 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//111.pcx
   $MAGICK $src -crop 320x1+0+87 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//112.pcx
   $MAGICK $src -crop 320x1+0+86 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//113.pcx
   $MAGICK $src -crop 320x1+0+85 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//114.pcx
   $MAGICK $src -crop 320x1+0+84 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//115.pcx
   $MAGICK $src -crop 320x1+0+83 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//116.pcx
   $MAGICK $src -crop 320x1+0+82 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//117.pcx
   $MAGICK $src -crop 320x1+0+81 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//118.pcx
   $MAGICK $src -crop 320x1+0+80 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//119.pcx
   $MAGICK $src -crop 320x1+0+79 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//120.pcx
   $MAGICK $src -crop 320x1+0+78 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//121.pcx
   $MAGICK $src -crop 320x1+0+77 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//122.pcx
   $MAGICK $src -crop 320x1+0+76 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//123.pcx
   $MAGICK $src -crop 320x1+0+75 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//124.pcx
   $MAGICK $src -crop 320x1+0+74 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//125.pcx
   $MAGICK $src -crop 320x1+0+73 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//126.pcx
   $MAGICK $src -crop 320x1+0+72 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//127.pcx
   $MAGICK $src -crop 320x1+0+71 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//128.pcx
   $MAGICK $src -crop 320x1+0+70 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//129.pcx
   $MAGICK $src -crop 320x1+0+69 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//130.pcx
   $MAGICK $src -crop 320x1+0+68 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//131.pcx
   $MAGICK $src -crop 320x1+0+67 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//132.pcx
   $MAGICK $src -crop 320x1+0+66 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//133.pcx
   $MAGICK $src -crop 320x1+0+65 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//134.pcx
   $MAGICK $src -crop 320x1+0+64 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//135.pcx
   $MAGICK $src -crop 320x1+0+63 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//136.pcx
   $MAGICK $src -crop 320x1+0+62 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//137.pcx
   $MAGICK $src -crop 320x1+0+61 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//138.pcx
   $MAGICK $src -crop 320x1+0+60 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//139.pcx
   $MAGICK $src -crop 320x1+0+59 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//140.pcx
   $MAGICK $src -crop 320x1+0+58 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//141.pcx
   $MAGICK $src -crop 320x1+0+57 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//142.pcx
   $MAGICK $src -crop 320x1+0+56 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//143.pcx
   $MAGICK $src -crop 320x1+0+55 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//144.pcx
   $MAGICK $src -crop 320x1+0+54 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//145.pcx
   $MAGICK $src -crop 320x1+0+53 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//146.pcx
   $MAGICK $src -crop 320x1+0+52 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//147.pcx
   $MAGICK $src -crop 320x1+0+51 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//148.pcx
   $MAGICK $src -crop 320x1+0+50 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//149.pcx
   $MAGICK $src -crop 320x1+0+49 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//150.pcx
   $MAGICK $src -crop 320x1+0+48 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//151.pcx
   $MAGICK $src -crop 320x1+0+47 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//152.pcx
   $MAGICK $src -crop 320x1+0+46 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//153.pcx
   $MAGICK $src -crop 320x1+0+45 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//154.pcx
   $MAGICK $src -crop 320x1+0+44 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//155.pcx
   $MAGICK $src -crop 320x1+0+43 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//156.pcx
   $MAGICK $src -crop 320x1+0+42 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//157.pcx
   $MAGICK $src -crop 320x1+0+41 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//158.pcx
   $MAGICK $src -crop 320x1+0+40 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//159.pcx
   $MAGICK $src -crop 320x1+0+39 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//160.pcx
   $MAGICK $src -crop 320x1+0+38 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//161.pcx
   $MAGICK $src -crop 320x1+0+37 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//162.pcx
   $MAGICK $src -crop 320x1+0+36 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//163.pcx
   $MAGICK $src -crop 320x1+0+35 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//164.pcx
   $MAGICK $src -crop 320x1+0+34 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//165.pcx
   $MAGICK $src -crop 320x1+0+33 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//166.pcx
   $MAGICK $src -crop 320x1+0+32 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//167.pcx
   $MAGICK $src -crop 320x1+0+31 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//168.pcx
   $MAGICK $src -crop 320x1+0+30 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//169.pcx
   $MAGICK $src -crop 320x1+0+29 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//170.pcx
   $MAGICK $src -crop 320x1+0+28 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//171.pcx
   $MAGICK $src -crop 320x1+0+27 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//172.pcx
   $MAGICK $src -crop 320x1+0+26 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//173.pcx
   $MAGICK $src -crop 320x1+0+25 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//174.pcx
   $MAGICK $src -crop 320x1+0+24 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//175.pcx
   $MAGICK $src -crop 320x1+0+23 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//176.pcx
   $MAGICK $src -crop 320x1+0+22 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//177.pcx
   $MAGICK $src -crop 320x1+0+21 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//178.pcx
   $MAGICK $src -crop 320x1+0+20 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//179.pcx
   $MAGICK $src -crop 320x1+0+19 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//180.pcx
   $MAGICK $src -crop 320x1+0+18 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//181.pcx
   $MAGICK $src -crop 320x1+0+17 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//182.pcx
   $MAGICK $src -crop 320x1+0+16 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//183.pcx
   $MAGICK $src -crop 320x1+0+15 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//184.pcx
   $MAGICK $src -crop 320x1+0+14 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//185.pcx
   $MAGICK $src -crop 320x1+0+13 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//186.pcx
   $MAGICK $src -crop 320x1+0+12 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//187.pcx
   $MAGICK $src -crop 320x1+0+11 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//188.pcx
   $MAGICK $src -crop 320x1+0+10 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//189.pcx
   $MAGICK $src -crop 320x1+0+9 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//190.pcx
   $MAGICK $src -crop 320x1+0+8 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//191.pcx
   $MAGICK $src -crop 320x1+0+7 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//192.pcx
   $MAGICK $src -crop 320x1+0+6 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//193.pcx
   $MAGICK $src -crop 320x1+0+5 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//194.pcx
   $MAGICK $src -crop 320x1+0+4 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//195.pcx
   $MAGICK $src -crop 320x1+0+3 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//196.pcx
   $MAGICK $src -crop 320x1+0+2 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//197.pcx
   $MAGICK $src -crop 320x1+0+1 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//198.pcx
   $MAGICK $src -crop 320x1+0+0 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx//$tgt//199.pcx
 done
fi

# this is the end
