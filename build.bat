@echo off

SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\Auxiliary\Build\vcvarsall.bat" amd64))

SET ARG=%1

IF "%ARG%"=="release" (
echo Release_Build
SET DEPLOY=-O2
SET NAME=release
SET DBGONLY=
)
IF "%ARG%"=="debug" (
echo Debug_Build
SET DEPLOY=-Od -D_DEBUG
SET NAME=debug
SET DBGONLY=Dbghelp.lib OneCoreUAP.lib
)

SET CODE_HOME=%cd%

REM SET TOOLDIR=F:\ProgrammingProjects\IncludeDir\Tools
REM SET PLATDIR=F:\ProgrammingProjects\IncludeDir\Platform

SET TOOLDIR=C:\Users\Lorenzo\Documents\Programming\Projects\IncludeDir\Tools
SET PLATDIR=C:\Users\Lorenzo\Documents\Programming\Projects\IncludeDir\Platform

SET OPTS= -favor:ATOM %DEPLOY% -I %TOOLDIR% -I %PLATDIR% /MT /GR- -FC -EHa- -nologo -diagnostics:column -Zi /std:c++latest
SET LIBS=kernel32.lib user32.lib gdi32.lib Msimg32.lib winmm.lib opengl32.lib %DBGONLY%

REM SET ASMDIR=F:\ProgrammingProjects\AddIncludes\PlatformAndTools\tools\asm
SET ASMDIR=%cd%\code\asm

SET ASMFILES=
SET OBJFILES=
REM SET ASMFILES=%ASMDIR%\lsBigInt.obj

IF [%ASMFILES%] == [] (

pushd out
cl %OPTS% %CODE_HOME%\code\win32_main.cpp -Fe%NAME% %LIBS%
del *.ilk
del *.obj
popd

)

IF NOT [%ASMFILES%] == [] (

pushd out\obj
ml64 /c %ASMFILES%
popd

pushd out
cl %OPTS% %CODE_HOME%\code\win32_main.cpp -Fe%NAME% %LIBS% %OBJFILES%
del *.ilk
del *.obj
popd

)
