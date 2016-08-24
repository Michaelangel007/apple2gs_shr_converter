@echo off
REM magall.bat - post process the output from the A2B converter
REM use ImageMagick to make double scaled preview files for review in a separate subdirectory
REM these can be viewed as a slide-show under Windows
REM copy SHR output to a separate subdirectory (with no additional files) for preparing Apple IIgs slide-shows
REM CiderPress can then be used more easily to place SHR files on an Apple IIgs Disk Image
REM set path to ImageMagick below
REM for ImageMagick version 6.9 use ImageMagick's convert utility
REM set MAGICK=C:\IM\convert
REM for ImageMagick version 7 use the following
set MAGICK=C:\IM\magick
cd SH30709raw
if not exist png\*.* mkdir png >NUL
if not exist shr\*.* mkdir shr >NUL
for %%f in (*_proc.bmp) do if not exist png\%%~nf.png call %MAGICK% %%f -magnify -define format:PNG png\%%~nf.png
for %%f in (*.shr#C10000) do if not exist SHR\%%f copy %%f shr\%%f > NUL
cd ..
cd SH32709raw
if not exist png\*.* mkdir png >NUL
if not exist shr\*.* mkdir shr >NUL
for %%f in (*_proc.bmp) do if not exist png\%%~nf.png call %MAGICK% %%f -magnify -define format:PNG png\%%~nf.png
for %%f in (*.sh2#C10000) do if not exist SHR\%%f copy %%f shr\%%f > NUL
cd ..
cd SH33709raw
if not exist png\*.* mkdir png >NUL
if not exist shr\*.* mkdir shr >NUL
for %%f in (*_proc.bmp) do if not exist png\%%~nf.png call %MAGICK% %%f -magnify -define format:PNG png\%%~nf.png
for %%f in (*.sh3#C10002) do if not exist SHR\%%f copy %%f shr\%%f > NUL
cd ..
cd sh33709
if not exist png\*.* mkdir png >NUL
if not exist shr\*.* mkdir shr >NUL
for %%f in (*_proc.bmp) do if not exist png\%%~nf.png call %MAGICK% %%f -magnify -define format:PNG png\%%~nf.png
for %%f in (*.sh3#C10002) do if not exist SHR\%%f copy %%f shr\%%f > NUL
cd ..
cd sh32709
if not exist png\*.* mkdir png >NUL
if not exist shr\*.* mkdir shr >NUL
for %%f in (*_proc.bmp) do if not exist png\%%~nf.png call %MAGICK% %%f -magnify -define format:PNG png\%%~nf.png
for %%f in (*.sh2#C10000) do if not exist SHR\%%f copy %%f shr\%%f > NUL
cd ..
cd sh30709
if not exist png\*.* mkdir png >NUL
if not exist shr\*.* mkdir shr >NUL
for %%f in (*_proc.bmp) do if not exist png\%%~nf.png call %MAGICK% %%f -magnify -define format:PNG png\%%~nf.png
for %%f in (*.shr#C10000) do if not exist SHR\%%f copy %%f shr\%%f > NUL
cd ..
REM this is the end
