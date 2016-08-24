@echo off
rem for ImageMagick version 6.9 use the following 
rem set MAGICK=c:\IM\convert
rem for ImageMagick version 7 use the following 
set MAGICK=c:\IM\magick
for %%f in (*.bmp) do call %MAGICK% %%f -define format:PNG %%~nf.png
