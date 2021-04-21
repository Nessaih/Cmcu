::Comments:
::Written by vic, at data 2021/4/20.
::Please put this file and the program which named  "srec_cat.exe"  at the same folder as keil project. 
::And make sure the file has been added to the "user's commad after build" menu.


:: Generate hex and bin file for boot leader project.
:: @echo off
:: set src=%~dp0\output\debug
:: set dst=%~dp0
:: .\srec_cat.exe %src%\HC32F4A0SITB.hex -Intel -crop 0x00000 0x10000 -output_block_size=16 -o %dst%\HC32F4A0SITB.hex -Intel
:: .\srec_cat.exe .\HC32F4A0SITB.hex -Intel -o HC32F4A0SITB.bin -Binary  


:: :: Generate hex and bin file for Application project.
:: @echo off
:: set src=%~dp0\output\debug
:: set dst=%~dp0
:: .\srec_cat.exe %src%\HC32F4A0SITB.hex -Intel -crop 0x10000 0x70000 -output_block_size=16 -o %dst%\HC32F4A0SITB.hex -Intel
:: .\srec_cat.exe .\HC32F4A0SITB.hex -Intel -offset - 0x10000 -o HC32F4A0SITB.bin -Binary  


:: Merge two HEX files which have no address overlaps:
@echo off&setlocal enabledelayedexpansion 
set n=1
set dt=%date:~0,4%-%date:~5,2%-%date:~8,2%.%time:~0,2%%time:~3,2%%time:~6,2%
for /r %%i in (*.hex) do (
    REM %%~pnxi:  p:the file's path; n:the file's name; x: the file's expanded-name
    set "str=%%~i"
    if "!n!"=="1" (
        set file1=!str!
    ) else if "!n!"=="2" (
        set file2=!str!
    )
    set /a "n=n+1"
)
if exist "%~dp0\hc32f4a0\" (
    echo hc32f4a0 folder already exist.
) else (
    mkdir hc32f4a0
    echo create hc32f4a0 folder.
)
.\srec_cat.exe %file1% -Intel %file2% -Intel -output_block_size=16 -o .\hc32f4a0\hc32f4a0_%dt%.hex -Intel
.\srec_cat.exe  .\hc32f4a0\hc32f4a0_%dt%.hex -Intel -o  .\hc32f4a0\hc32f4a0_%dt%.bin -Binary
