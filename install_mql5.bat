@echo off
setlocal enabledelayedexpansion

REM Set path to the library source folder
set "SRC_LIB=MQL5\Include"
set "SRC_EXAMPLES=MQL5\Scripts"

REM Check if source exists
if not exist "%SRC_LIB%" (
    echo [×] Library folder not found: %SRC_LIB%
    goto end
)

if not exist "%SRC_EXAMPLES%" (
    echo [×] Example folder not found: %SRC_EXAMPLES%
    goto end
)

echo [*] Scanning for installed MetaTrader terminals...

REM Set base directory where MT5 terminals are located
set "BASE=%APPDATA%\MetaQuotes\Terminal"

REM Iterate over subdirectories (terminals)
for /D %%T in ("%BASE%\*") do (
    set "DST_LIB=%%T\MQL5\Include"
    set "DST_EXAMPLES=%%T\MQL5\Scripts"

    REM Copy library files
    if exist "!DST_LIB!" (
        echo [→] Installing to: !DST_LIB!
        xcopy /e /i /y "%SRC_LIB%" "!DST_LIB!" >nul

        if errorlevel 1 (
            echo     [✓] Installed successfully to: !DST_LIB!
        ) else (
            echo     [×] Copy failed for: !DST_LIB!
        )
    ) else (
        echo [×] Skipping %%T — No MQL5\Include folder
    )

    REM Copy example scripts
    if exist "%%T\MQL5\Experts" (
        echo [→] Installing examples to: !DST_EXAMPLES!
        xcopy /e /i /y "%SRC_EXAMPLES%" "!DST_EXAMPLES!" >nul

        if errorlevel 1 (
            echo     [✓] Example installed to: !DST_EXAMPLES!
        ) else (
            echo     [×] Example copy failed for: !DST_EXAMPLES!
        )
    ) else (
        echo [×] Skipping examples for %%T — No MQL5\Experts folder
    )
)

:end
echo.
pause
endlocal
