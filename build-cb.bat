@echo off
mkdir build-cb
cmake -G "CodeBlocks - MinGW Makefiles" -S . -B build-cb || goto :error
goto :done

:error
echo CMake generation failed.
pause
exit /b 1

:done
pause
