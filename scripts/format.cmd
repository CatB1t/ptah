@echo off
rem Format all C/C++ sources in include/ and src/ with clang-format.
rem Prints each file that was changed and a total count.
setlocal EnableDelayedExpansion
pushd "%~dp0.."
set "root=%CD%\"
set /a changed=0

for /R include %%f in (*.h *.hpp *.inl *.c *.cc *.cpp *.cxx) do call :format "%%f"
for /R src %%f in (*.h *.hpp *.inl *.c *.cc *.cpp *.cxx) do call :format "%%f"

popd
echo Formatting done: !changed! file(s) changed.
exit /b 0

:format
clang-format --dry-run -Werror --style=file %1 >nul 2>&1
if errorlevel 1 (
    clang-format -i --style=file %1
    set "rel=%~1"
    echo   changed: !rel:%root%=!
    set /a changed+=1
)
exit /b 0
