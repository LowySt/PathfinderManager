@echo off

SET VC_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\Auxiliary\Build\vcvarsall.bat" amd64))

SET ARG=%1

SET NAME=assetBuilder
SET CODE_HOME=%cd%

SET TOOLDIR=F:\ProgrammingProjects\IncludeDir\Tools
SET PLATDIR=F:\ProgrammingProjects\IncludeDir\Platform

SET OPTS= -Od -D_DEBUG -I %TOOLDIR% -I %PLATDIR% -MT -FC -GR- -EHa- -nologo -diagnostics:column -Zi /std:c++latest
SET LIBS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib winmm.lib Dbghelp.lib OneCoreUAP.lib

pushd out
cl %OPTS% %CODE_HOME%\code\asset_main.cpp -Fe%NAME% %LIBS%
del *.ilk
del *.obj
popd

