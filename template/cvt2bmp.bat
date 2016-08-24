@echo off

if not exist SHRTONE\*.* mkdir SHRTONE >NUL
if not exist SH33TONE\*.* mkdir SH33TONE >NUL
if not exist SH32TONE\*.* mkdir SH32TONE >NUL
if not exist SHRPCX\*.* mkdir SHRPCX >NUL
if not exist SH32PCX\*.* mkdir SH32PCX >NUL
if not exist SH33PCX\*.* mkdir SH33PCX >NUL
if not exist SHRGREY*.* mkdir SHRGREY >NUL
if not exist PCX\*.* mkdir PCX >NUL
if not exist BMP320\*.* mkdir BMP320 >NUL

if not exist todo\*.* mkdir todo >NUL
if not exist done\*.* mkdir done >NUL
if not exist gif\*.* mkdir gif >NUL

if "%1" == "" goto BEGIN
if "%1" == "box" goto BOX
if "%1" == "BOX" goto BOX
if "%1" == "BMP" goto BMP
if "%1" == "bmp" goto BMP
goto RESIZE

REM Notes from the ImageMagick Forums about using convert (in Windows?)

convert image -resize "275x275^" -gravity center -crop 275x275+0+0 +repage resultimage

see http://www.imagemagick.org/script/comma ... p#geometry regarding ^ and other character options.

This resizes according to the smaller of your input image dimensions keeping aspect ratio. Thus the smaller dimension will end
up as 275, but the larger dimension will be larger. Thus the crop following the resize.

On Windows you may have to escape the ^ with ^ so that it is ^^ (if the quotes don't help). See
http://www.imagemagick.org/Usage/windows/

Alternately, you can resize to the larger dimension and then pad the image

convert image -resize "275x275" -gravity center -background white -extent 275x275 resultimage

Alternately, you can resize and lose aspect ratio ( and accept some distortion)

convert image -resize "275x275!" resultimage

examples are at http://www.imagemagick.org/Usage/resize/#fill

I have had some trouble with BMP conversions too. I have typically specified BMP3:outfile.bmp and I normally get 24 bit BMP
files. sometimes I get an 8 bit BMP when converting JPEG files. I just tried the -type truecolor (having seen it here) and now
it looks like I get a 24 bit BMP in the cases where I used to get only 8 bits.

I also use these switches:

-units PixelsPerInch -density 72 -compress None -depth 24

the -depth 24 should do a 24 bit BMP (the depth -8 in your case may also be causing a problem) but as I said I sometimes get 8
bits even with the 24 specified.

the -units and -density cases corrected a problem where sometimes I would get a 0 for the biXPelsPerMeter and biYPelsPerMeter
fields in the BMP header (it did not seem to cause a problem though). by the way I have not found a good explanation of these 2
fields.

the -compress None was required because sometimes a conversion generated a compressed BMP - no logical reason for doing that was
found.


REM End of Notes

:RESIZE
C:\IM\convert %1 -resize "320x200!" -type truecolor -units PixelsPerInch -density 72 -compress None -depth 24 -define format:BMP3 Resized_%1
goto ENDER


:BMP 
for %%f in (*.bmp) do call C:\IM\convert %%f -resize "320x200!" -type truecolor -units PixelsPerInch -density 72 -compress None -depth 24 -define format:BMP3 BMP320\%%~nf.bmp
goto ENDER

:BOX
REM for %%f in (*.gif) do call C:\IM\convert %%f -resize "320x320" -gravity center -background black -extent 320x320 -crop 320x200+0+0 +repage -define format:BMP %%~nf.bmp
REM for %%f in (*.png) do call C:\IM\convert %%f -resize "320x320" -gravity center -background black -extent 320x320 -crop 320x200+0+0 +repage -define format:BMP %%~nf.bmp
for %%f in (*.jpg) do call C:\IM\convert %%f -resize "320x320" -gravity center -background black -extent 320x320 -crop 320x200+0+0 +repage -define format:BMP %%~nf.bmp
goto ENDER

:BEGIN
REM this next one is mostly for double dithering
for %%f in (*.pcx) do call C:\IM\convert %%f -resize "320x200!" -type truecolor -units PixelsPerInch -density 72 -compress None -depth 24 -define format:BMP3 %%~nf.bmp

for %%f in (*.gif) do call C:\IM\convert %%f -resize "320x200!" -type truecolor -units PixelsPerInch -density 72 -compress None -depth 24 -define format:BMP3 %%~nf.bmp
for %%f in (*.png) do call C:\IM\convert %%f -resize "320x200!" -type truecolor -units PixelsPerInch -density 72 -compress None -depth 24 -define format:BMP3 %%~nf.bmp
for %%f in (*.jpg) do call C:\IM\convert %%f -resize "320x200!" -define format:BMP %%~nf.bmp

:ENDER
REM this is the end


