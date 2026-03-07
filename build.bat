@echo off
setlocal

REM ── C++ DSA Code Analyzer — build helper ───────────────────────────────────
set QT_DIR=C:\Qt\6.10.2\mingw_64
set MINGW_DIR=C:\Qt\Tools\mingw1310_64\bin
set CMAKE_EXE=C:\Qt\Tools\CMake_64\bin\cmake.exe
set NINJA_EXE=C:\Qt\Tools\Ninja\ninja.exe

echo [1/3] Configuring with CMake...
"%CMAKE_EXE%" -S . -B build ^
  -G "Ninja" ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_PREFIX_PATH="%QT_DIR%" ^
  -DCMAKE_MAKE_PROGRAM="%NINJA_EXE%" ^
  -DCMAKE_C_COMPILER="%MINGW_DIR%\gcc.exe" ^
  -DCMAKE_CXX_COMPILER="%MINGW_DIR%\g++.exe"

if errorlevel 1 (
    echo [ERROR] CMake configuration failed.
    pause
    exit /b 1
)

echo [2/3] Building...
"%CMAKE_EXE%" --build build --config Release --parallel

if errorlevel 1 (
    echo [ERROR] Build failed.
    pause
    exit /b 1
)

echo [3/3] Deploying Qt DLLs...
"%QT_DIR%\bin\windeployqt6.exe" --no-translations build\ComplexityAnalyzer.exe 2>nul
if errorlevel 1 (
    "%QT_DIR%\bin\windeployqt.exe" --no-translations build\ComplexityAnalyzer.exe 2>nul
)

echo.
echo ============================================
echo  Build successful!
echo  Run: build\ComplexityAnalyzer.exe
echo ============================================
pause
