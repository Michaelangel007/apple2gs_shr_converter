@echo off
REM pad43.bat - pad all images to 4:3 aspect BMP
REM calls cropper.bat to do so for each individual image
if not exist jpg\*.* mkdir jpg >NUL
for %%f in (*.jpg) do call cropper.bat %%f %%~nf PAD
for %%f in (*.png) do call cropper.bat %%f %%~nf PAD
for %%f in (*.gif) do call cropper.bat %%f %%~nf PAD
cd jpg
if exist ..\*.jpg move ..\*.jpg . > NUL
if exist ..\*.png move ..\*.png . > NUL
if exist ..\*.gif move ..\*.gif . > NUL
cd ..
REM this is the end  
