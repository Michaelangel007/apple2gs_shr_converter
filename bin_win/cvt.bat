@echo off
REM cvt.bat - creates 6 variations of 320 x 200 SHR files using A2B and external segmented palettes
 
REM set path to A2B converter below
set A2B=..\a2b
 
if not exist SH30709\*.* mkdir SH30709 >NUL
if not exist SH32709\*.* mkdir SH32709 >NUL
if not exist SH33709\*.* mkdir SH33709 >NUL
if not exist SH30709raw\*.* mkdir SH30709raw >NUL
if not exist SH32709raw\*.* mkdir SH32709raw >NUL
if not exist SH33709raw\*.* mkdir SH33709raw >NUL
 
if not exist todo\*.* mkdir todo >NUL
if not exist done\*.* mkdir done >NUL
 
REM make External Segmented Palettes using ImageMagick's Convert Utility
call slicer.bat
 
REM Call A2B to create SHR files
for %%f in (*.bmp) do call %A2B% %%f SH30709\%%f dr m2s t PIMsh0pcx\%%~nf\foo sum l709 >NUL
for %%f in (*.bmp) do call %A2B% %%f SH32709\%%f dr m2s t PIMsh2pcx\%%~nf\foo sum l709 >NUL
for %%f in (*.bmp) do call %A2B% %%f SH33709\%%f dr m2s t PIMsh3pcx\%%~nf\foo sum l709 >NUL
 
for %%f in (*.bmp) do call %A2B% %%f SH30709raw\%%f m2s t PIMsh0pcx\%%~nf\foo sum l709 >NUL
for %%f in (*.bmp) do call %A2B% %%f SH32709raw\%%f m2s t PIMsh2pcx\%%~nf\foo sum l709 >NUL
for %%f in (*.bmp) do call %A2B% %%f SH33709raw\%%f m2s t PIMsh3pcx\%%~nf\foo sum l709 >NUL
 
cd done
move ..\*.bmp .
cd ..
 
REM this is the end
