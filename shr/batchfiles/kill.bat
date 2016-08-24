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
