@echo off
REM aspect43.bat - center-crop all images to 4:3 aspect BMP
REM calls cropper.bat to do so for each individual image
set CROP=CROP
REM optionally pad all images to 4:3 aspect BMP
if "%1" == "pad" set CROP=%1
if "%1" == "PAD" set CROP=%1
if "%1" == "" goto BEGIN
REM optionally set crop geometry to other gravities (N,S,E,W)
set CROP=%1
:BEGIN
if not exist jpg\*.* mkdir jpg >NUL
for %%f in (*.jpg) do call cropper.bat %%f %%~nf %CROP%
for %%f in (*.png) do call cropper.bat %%f %%~nf %CROP%
for %%f in (*.gif) do call cropper.bat %%f %%~nf %CROP%
cd jpg
if exist ..\*.jpg move ..\*.jpg . > NUL
if exist ..\*.png move ..\*.png . > NUL
if exist ..\*.gif move ..\*.gif . > NUL
cd ..
REM this is the end
