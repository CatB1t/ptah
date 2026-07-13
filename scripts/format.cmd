@echo off
rem Format all C/C++ sources in include/ and src/ with clang-format.
setlocal
pushd "%~dp0.."

for /R include %%f in (*.h *.hpp *.inl *.c *.cc *.cpp *.cxx) do clang-format -i --style=file "%%f"
for /R src %%f in (*.h *.hpp *.inl *.c *.cc *.cpp *.cxx) do clang-format -i --style=file "%%f"

popd
echo Formatting done.
