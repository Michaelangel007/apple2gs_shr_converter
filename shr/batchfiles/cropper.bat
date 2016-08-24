@echo off
REM cropper.bat - center-crop or pad image to 4:3 aspect BMP 
REM called from aspect43.bat to center-crop each individual image 
REM called from pad43.bat to pad each individual image 
if "%1" == "" goto ENDER
REM set default method to crop
set METHOD=a43b
REM alternately for pillarboxed or letterboxed padding set method to pad 
if "%3" == "PAD" set METHOD=p43b 
if "%3" == "pad" set METHOD=p43b 
REM alternately set crop geometry to gravities other than center (default)
if "%3" == "N" set METHOD=a43bNorth
if "%3" == "n" set METHOD=a43bNorth
if "%3" == "S" set METHOD=a43bSouth
if "%3" == "s" set METHOD=a43bSouth
if "%3" == "E" set METHOD=a43bEast
if "%3" == "e" set METHOD=a43bEast
if "%3" == "W" set METHOD=a43bWest
if "%3" == "w" set METHOD=a43bWest
REM set paths below 
set A2B=..\a2b
rem for ImageMagick version 6.9 use the following 
rem set MAGICK=c:\IM\convert
rem set IDENTIFY=c:\IM\identify
rem for ImageMagick version 7 use the following 
set MAGICK=c:\IM\magick
set IDENTIFY=c:\IM\magick identify
REM redirect output from ImageMagick identify to a temporary id file for this image 
call %IDENTIFY% %1 > %2.id
REM call A2B to use the id file to write a temporary batch file for this image 
call %A2B% %1 %METHOD%
call %2.bat %MAGICK% %1 %2
REM clean-up the temporary files
del %2.bat
del %2.id
:ENDER
REM this is the end


