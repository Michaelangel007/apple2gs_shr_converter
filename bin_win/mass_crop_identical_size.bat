@echo off
REM crop1440.bat - center-crop jpeg 1920 x 1080 hd wallpaper to 4:3 aspect BMP
REM set path to ImageMagick below
REM for ImageMagick version 6.9 use the following
REM set MAGICK=c:\IM\convert
REM for ImageMagick version 7 use the following
set MAGICK=c:\IM\magick
if not exist jpg\*.* mkdir jpg >NUL
for %%f in (*.jpg) do call %MAGICK% %%f -gravity center -crop 1440x1080+0+0 -define format:BMP %%~nf.bmp
cd jpg
if exist ..\*.jpg move ..\*.jpg . > NUL
cd ..
