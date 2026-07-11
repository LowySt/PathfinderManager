@echo off

if not defined VSCMD_VER (
    call "%VCVARSALLPATH%" amd64
)

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

SET INCLUDEDIR=F:\ProgrammingProjects\IncludeDir
SET TOOLDIR=%INCLUDEDIR%\Tools
SET PLATDIR=%INCLUDEDIR%\Platform

SET OPTS=/utf-8 -favor:ATOM %DEPLOY% -I %TOOLDIR% -I %PLATDIR% /MT /GR- -FC -EHa- -nologo -diagnostics:column -Zi /std:c++latest
SET LIBS=kernel32.lib user32.lib gdi32.lib Msimg32.lib winmm.lib opengl32.lib %DBGONLY%

pushd out
cl %OPTS% %CODE_HOME%\code\win32_main.cpp -Fe%NAME% %LIBS%
del *.ilk
del *.obj
popd
