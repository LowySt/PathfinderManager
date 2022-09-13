@echo off

SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\Auxiliary\Build\vcvarsall.bat" amd64))

SET ARG=%1

IF "%ARG%"=="release" (
echo Release_Build
SET DEPLOY=-O2
SET NAME=yoinkerRelease
SET DBGONLY=
)
IF "%ARG%"=="debug" (
echo Debug_Build
SET DEPLOY=-Od -D_DEBUG
SET NAME=yoinkerDebug
SET DBGONLY=Dbghelp.lib OneCoreUAP.lib
)

SET CODE_HOME=%cd%

SET TOOLDIR=F:\ProgrammingProjects\IncludeDir\Tools
SET PLATDIR=F:\ProgrammingProjects\IncludeDir\Platform

SET OPTS= -favor:ATOM %DEPLOY% -I %TOOLDIR% -I %PLATDIR% /MT /GR- -FC -EHa- -nologo -diagnostics:column -Zi
SET LIBS=kernel32.lib user32.lib gdi32.lib winmm.lib ws2_32.lib %DBGONLY%

pushd yoinkerOut
cl %OPTS% %CODE_HOME%\code\yoinker_main.cpp -Fe%NAME% %LIBS%
del *.ilk
del *.obj
popd
