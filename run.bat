@echo off
chcp 65001 > nul
echo ========================================
echo   BIEN DICH VA CHAY CHUONG TRINH C
echo ========================================
echo.

echo [1/2] Dang bien dich hovanphuoc.c...
gcc hovanphuoc.c -o hovanphuoc.exe 2>error.log

if %errorlevel% equ 0 (
    echo [OK] Bien dich thanh cong!
    echo.
    echo [2/2] Dang chay chuong trinh...
    echo ========================================
    echo.
    hovanphuoc.exe
    echo.
    echo ========================================
    echo Chuong trinh da ket thuc.
) else (
    echo [LOI] Bien dich that bai!
    echo.
    echo Chi tiet loi:
    type error.log
    echo.
    echo Vui long kiem tra lai code.
)

echo.
pause
