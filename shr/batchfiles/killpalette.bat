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
