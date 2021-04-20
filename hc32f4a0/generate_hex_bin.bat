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
for /r %%i in (*.hex) do (
    set "str= %%~i"
    if "!n!"=="1" (
        set file1=!str!
    ) else if "!n!"=="2" (
        set file2=!str!
    )
    set /a "n=n+1"
)
.\srec_cat.exe %file1% -Intel %file2% -Intel -output_block_size=16 -o merged_hex_file.hex -Intel

