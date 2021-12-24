REM Build script for the vulkan engine.
@echo off
SetLocal EnableDelayedExpansion

REM Get a list of all the .c files.
SET cFilenames=
FOR /R %%f in (*.c) do (
    SET cFilenames=!cFilenames! %%f
)

REM echo "Files:" %cFilenames%
SET assembly=test
SET compilerFlags=-g
REM -Wall -Werror
SET includeFlags=-Isrc -I../engine/src/
SET linkerFlags=-L../bin/Win64/ -lVulkanEngine.lib
SET defines=-D_DEBUG -DVKCIMPORT

ECHO [BUILD]: Building %assembly%%...
clang %cFilenames% %compilerFlags% -o ../bin/Win64/%assembly%.exe %defines% %includeFlags% %linkerFlags%
