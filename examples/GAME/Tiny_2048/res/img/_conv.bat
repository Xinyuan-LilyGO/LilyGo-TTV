@echo off
set PATH=c:\python27;%PATH%

python _conv.py
move /Y img.h ..\
pause
