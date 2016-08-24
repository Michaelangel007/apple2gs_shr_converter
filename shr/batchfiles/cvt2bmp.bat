@echo off
REM cvt2bmp.bat - uses ImageMagick to produce 320 x 200 Input Files for A2B 

REM set path to ImageMagick below 
REM for ImageMagick version 6.9 use the following 
REM set MAGICK=c:\IM\convert
REM for ImageMagick version 7 use the following 
set MAGICK=C:\IM\magick

if not exist BMP320\*.* mkdir BMP320 >NUL
if not exist ORGBMP\*.* mkdir ORGBMP >NUL
if not exist jpg\*.* mkdir jpg >NUL

REM create GIF input files for native mode converters if needed. 
REM if not exist gif\*.* mkdir gif >NUL

REM assumes that all input files are 4:3 Aspect Ratio
REM accepts jpegs, pngs, gifs, or BMPs as input files
REM converts everything to truecolor BMP before scaling to avoid color loss
for %%f in (*.jpg) do call %MAGICK% %%f -type truecolor -units PixelsPerInch -density 72 -compress None -depth 24 -define format:BMP3 %%~nf.bmp
for %%f in (*.png) do call %MAGICK% %%f -type truecolor -units PixelsPerInch -density 72 -compress None -depth 24 -define format:BMP3 %%~nf.bmp
for %%f in (*.gif) do call %MAGICK% %%f -type truecolor -units PixelsPerInch -density 72 -compress None -depth 24 -define format:BMP3 %%~nf.bmp
cd jpg
if exist ..\*.jpg move ..\*.jpg . > NUL
if exist ..\*.png move ..\*.png . > NUL
if exist ..\*.gif move ..\*.gif . > NUL
cd ..

REM create A2B's input files - scaling is done here. 
for %%f in (*.bmp) do call %MAGICK% %%f -resize "320x200!" -type truecolor -units PixelsPerInch -density 72 -compress None -depth 24 -define format:BMP3 BMP320\%%~nf.bmp
REM create GIF input files for native mode converters if needed. 
REM for %%f in (BMP320\*.bmp) do call %MAGICK% %%f -colors 256 -define format:GIF GIF\%%~nfx.gif
cd orgbmp
move ..\*.bmp .
cd ..\bmp320 
move *.bmp ..\.
cd ..

REM this is the end


