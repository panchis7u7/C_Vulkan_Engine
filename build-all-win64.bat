@echo off
REM Build All.

ECHO [BUILD]: Building all.

PUSHD engine
CALL build-lib-win64.bat
IF %ERRORLEVEL% NEQ 0 (echo [ERROR]: %ERRORLEVEL%)
POPD

PUSHD test
CALL build-exec-win64.bat
IF %ERRORLEVEL% NEQ 0 (echo [ERROR]: %ERRORLEVEL%)
POPD

ECHO [BUILD]: All assemblies built successfully!