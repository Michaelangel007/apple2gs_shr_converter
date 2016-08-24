@echo off
REM By the time I was done
REM I had pretty much abandoned The GIMP for ImageMagick
REM For production development of an interface version
REM I would likely use the ImageMagick API to save some time

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


REM call ImageMagick to make a 16 color error-diffusion dithered version
REM use as an external palette for conventional color space toning
REM in windows use pcx format for imagemagick's indexed output (for compatibility)
for %%f in (*.bmp) do call C:\IM\convert %%f -dither FloydSteinberg -colors 16 -define format:PCX PCX\%%~nf.pcx

REM Make 7 variations to choose from
REM the first will usually be the worst and unusable
for %%f in (*.bmp) do call ..\a2b %%f  SHRTONE\%%f shr2 d v t i > NUL
for %%f in (*.bmp) do call ..\a2b %%f  SH32TONE\%%f dic3 d v t i > NUL
REM The TONE variations are always more heavily contrasted
for %%f in (*.bmp) do call ..\a2b %%f  SH33TONE\%%f drooks3 d v t i > NUL
for %%f in (*.bmp) do call ..\a2b %%f  SHRPCX\%%f shr2 d v t i pPCX\%%f > NUL
for %%f in (*.bmp) do call ..\a2b %%f  SH32PCX\%%f dic3 d v t i pPCX\%%f > NUL 
for %%f in (*.bmp) do call ..\a2b %%f  SH33PCX\%%f drooks3 d v t i pPCX\%%f > NUL
for %%f in (*.bmp) do call ..\a2b %%f  SHRGREY\%%f shrgrey d v t i> NUL

