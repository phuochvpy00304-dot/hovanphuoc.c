@echo off
chcp 65001 > nul
echo Bien dich hovanphuoc.c...
gcc -Wall hovanphuoc.c -o hovanphuoc.exe
if %errorlevel% equ 0 (
    echo Thanh cong! File hovanphuoc.exe da duoc tao.
) else (
    echo Loi bien dich!
)
pause
