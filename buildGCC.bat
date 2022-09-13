@echo off

SET CODE_HOME=%cd%

SET TOOLDIR=F:\ProgrammingProjects\IncludeDir\Tools
SET PLATDIR=F:\ProgrammingProjects\IncludeDir\Platform

SET OPTS= -o release.exe -O2 -I %TOOLDIR% -I %PLATDIR% -std=c++11 -mfpmath=sse -mmmx -msse -msse2 -msse4.1 -mwindows
SET LINKER= -static -static-libgcc -static-libstdc++ -lgdi32 -lwinmm

pushd gccout
E:\ProgrammingUtility\mingw64\bin\g++.exe %OPTS% %CODE_HOME%\code\win32_main.cpp %LINKER%
popd
