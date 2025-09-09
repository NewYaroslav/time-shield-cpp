@echo off
mkdir build
cmake -S . -B build -DCMAKE_CXX_STANDARD=17 > log.txt 2>&1 || goto :error
cmake --build build >> log.txt 2>&1 || goto :error
goto :done

:error
echo Build failed. See log.txt for details.
pause
exit /b 1

:done
pause
