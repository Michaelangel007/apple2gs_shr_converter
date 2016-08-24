@echo off
REM slicer.bat - use ImageMagick to produce External Segment Palette Files for A2B
REM set path to ImageMagick below
REM for ImageMagick version 6.9 use ImageMagick's convert utility
REM set MAGICK=C:\IM\convert
REM for ImageMagick version 7 use the following
set MAGICK=C:\IM\magick
 
REM create directory structure for 16 color image segments
if not exist sh0pcx\*.* mkdir sh0pcx >NUL
REM if not exist sh1pcx\*.* mkdir sh1pcx >NUL
if not exist sh2pcx\*.* mkdir sh2pcx >NUL
if not exist sh3pcx\*.* mkdir sh3pcx >NUL
 
REM segment directories are of the same basename as the bmp being converted
REM the segments themselves are 0.pcx 1.pcx, 2.pcx, etc.
for %%f in (*.bmp) do if not exist sh0pcx\%%~nf mkdir sh0pcx\%%~nf >NUL 
REM for %%f in (*.bmp) do if not exist sh1pcx\%%~nf mkdir sh1pcx\%%~nf >NUL 
for %%f in (*.bmp) do if not exist sh2pcx\%%~nf mkdir sh2pcx\%%~nf >NUL 
for %%f in (*.bmp) do if not exist sh3pcx\%%~nf mkdir sh3pcx\%%~nf >NUL
 
goto BEGIN
 
REM 8-segments for multi-palette for motion video
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x25+0+175 -dither FloydSteinberg -colors 16 -define format:PCX sh1pcx\%%~nf\0.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x25+0+150 -dither FloydSteinberg -colors 16 -define format:PCX sh1pcx\%%~nf\1.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x25+0+125 -dither FloydSteinberg -colors 16 -define format:PCX sh1pcx\%%~nf\2.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x25+0+100 -dither FloydSteinberg -colors 16 -define format:PCX sh1pcx\%%~nf\3.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x25+0+75 -dither FloydSteinberg -colors 16 -define format:PCX sh1pcx\%%~nf\4.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x25+0+50 -dither FloydSteinberg -colors 16 -define format:PCX sh1pcx\%%~nf\5.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x25+0+25 -dither FloydSteinberg -colors 16 -define format:PCX sh1pcx\%%~nf\6.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x25+0+0 -dither FloydSteinberg -colors 16 -define format:PCX sh1pcx\%%~nf\7.pcx
 
:BEGIN
 
REM use the same segments currently used in A2B
 
REM 1-segment for single palette SHR
for %%f in (*.bmp) do call %MAGICK% %%f -dither FloydSteinberg -colors 16 -define format:PCX sh0pcx\%%~nf\0.pcx
 
REM 16-segments for multi-palette
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x13+0+187 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx\%%~nf\0.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x12+0+175 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx\%%~nf\1.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x13+0+162 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx\%%~nf\2.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x12+0+150 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx\%%~nf\3.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x13+0+137 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx\%%~nf\4.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x12+0+125 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx\%%~nf\5.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x13+0+112 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx\%%~nf\6.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x12+0+100 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx\%%~nf\7.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x13+0+87 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx\%%~nf\8.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x12+0+75 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx\%%~nf\9.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x13+0+62 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx\%%~nf\10.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x12+0+50 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx\%%~nf\11.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x13+0+37 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx\%%~nf\12.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x12+0+25 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx\%%~nf\13.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x13+0+12 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx\%%~nf\14.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x12+0+0 -dither FloydSteinberg -colors 16 -define format:PCX sh2pcx\%%~nf\15.pcx
 
REM 200-segments for brooks
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+199 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\0.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+198 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\1.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+197 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\2.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+196 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\3.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+195 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\4.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+194 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\5.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+193 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\6.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+192 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\7.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+191 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\8.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+190 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\9.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+189 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\10.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+188 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\11.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+187 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\12.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+186 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\13.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+185 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\14.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+184 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\15.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+183 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\16.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+182 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\17.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+181 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\18.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+180 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\19.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+179 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\20.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+178 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\21.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+177 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\22.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+176 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\23.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+175 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\24.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+174 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\25.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+173 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\26.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+172 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\27.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+171 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\28.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+170 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\29.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+169 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\30.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+168 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\31.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+167 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\32.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+166 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\33.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+165 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\34.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+164 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\35.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+163 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\36.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+162 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\37.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+161 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\38.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+160 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\39.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+159 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\40.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+158 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\41.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+157 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\42.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+156 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\43.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+155 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\44.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+154 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\45.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+153 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\46.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+152 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\47.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+151 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\48.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+150 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\49.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+149 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\50.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+148 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\51.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+147 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\52.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+146 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\53.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+145 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\54.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+144 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\55.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+143 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\56.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+142 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\57.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+141 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\58.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+140 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\59.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+139 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\60.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+138 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\61.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+137 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\62.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+136 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\63.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+135 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\64.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+134 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\65.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+133 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\66.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+132 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\67.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+131 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\68.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+130 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\69.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+129 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\70.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+128 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\71.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+127 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\72.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+126 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\73.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+125 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\74.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+124 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\75.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+123 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\76.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+122 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\77.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+121 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\78.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+120 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\79.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+119 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\80.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+118 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\81.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+117 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\82.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+116 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\83.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+115 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\84.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+114 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\85.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+113 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\86.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+112 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\87.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+111 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\88.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+110 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\89.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+109 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\90.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+108 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\91.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+107 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\92.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+106 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\93.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+105 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\94.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+104 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\95.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+103 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\96.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+102 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\97.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+101 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\98.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+100 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\99.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+99 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\100.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+98 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\101.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+97 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\102.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+96 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\103.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+95 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\104.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+94 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\105.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+93 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\106.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+92 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\107.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+91 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\108.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+90 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\109.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+89 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\110.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+88 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\111.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+87 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\112.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+86 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\113.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+85 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\114.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+84 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\115.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+83 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\116.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+82 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\117.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+81 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\118.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+80 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\119.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+79 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\120.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+78 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\121.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+77 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\122.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+76 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\123.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+75 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\124.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+74 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\125.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+73 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\126.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+72 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\127.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+71 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\128.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+70 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\129.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+69 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\130.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+68 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\131.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+67 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\132.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+66 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\133.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+65 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\134.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+64 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\135.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+63 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\136.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+62 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\137.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+61 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\138.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+60 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\139.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+59 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\140.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+58 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\141.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+57 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\142.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+56 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\143.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+55 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\144.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+54 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\145.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+53 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\146.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+52 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\147.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+51 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\148.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+50 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\149.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+49 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\150.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+48 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\151.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+47 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\152.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+46 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\153.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+45 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\154.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+44 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\155.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+43 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\156.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+42 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\157.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+41 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\158.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+40 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\159.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+39 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\160.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+38 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\161.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+37 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\162.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+36 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\163.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+35 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\164.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+34 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\165.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+33 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\166.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+32 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\167.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+31 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\168.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+30 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\169.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+29 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\170.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+28 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\171.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+27 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\172.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+26 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\173.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+25 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\174.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+24 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\175.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+23 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\176.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+22 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\177.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+21 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\178.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+20 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\179.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+19 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\180.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+18 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\181.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+17 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\182.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+16 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\183.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+15 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\184.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+14 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\185.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+13 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\186.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+12 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\187.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+11 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\188.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+10 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\189.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+9 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\190.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+8 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\191.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+7 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\192.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+6 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\193.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+5 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\194.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+4 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\195.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+3 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\196.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+2 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\197.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+1 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\198.pcx
for %%f in (*.bmp) do call %MAGICK% %%f -crop 320x1+0+0 -dither FloydSteinberg -colors 16 -define format:PCX sh3pcx\%%~nf\199.pcx
 
REM this is the end
