@echo off 
REM kill.bat - house-cleaning batch file
REM cleans unwanted input and output from working directory
REM must be run from the command line
REM killpalette.bat must also be run after kill.bat to remove unused external palette directories
if "%1" == "" goto ENDER
dir %1*.* /s /b > work.txt
for /F %%j in (work.txt) do del %%j
del work.txt
:ENDER
REM this is the end
 
@echo off 
REM killpalette.bat - house-cleaning batch file
REM removes unused palette directories left behind by kill.bat
dir sh0pcx\*.* /b /A:D > work.txt
for /F %%j in (work.txt) do if not exist sh0pcx\%%j\0.pcx rd sh0pcx\%%j
dir sh2pcx\*.* /b /A:D > work.txt
for /F %%j in (work.txt) do if not exist sh2pcx\%%j\0.pcx rd sh2pcx\%%j
dir sh3pcx\*.* /b /A:D > work.txt
for /F %%j in (work.txt) do if not exist sh3pcx\%%j\0.pcx rd sh3pcx\%%j
del work.txt
REM this is the end
