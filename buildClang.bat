@echo off

SET CODE_HOME=%cd%
SET CLANGDIR="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\x64\bin"

SET TOOLDIR=F:\ProgrammingProjects\IncludeDir\Tools
SET PLATDIR=F:\ProgrammingProjects\IncludeDir\Platform

SET OPTS= -o release.exe -O3 -I %TOOLDIR% -I %PLATDIR% -std=c++11 -mfpmath=sse -mmmx -msse -msse2 -msse4.1 -mwindows
SET LINKER= -static -lkernel32 -luser32 -lgdi32 -lwinmm

pushd clangout
%CLANGDIR%\clang++.exe %OPTS% %CODE_HOME%\code\win32_main.cpp %LINKER%
popd



