REM Build script for the vulkan engine.
@echo off
SetLocal EnableDelayedExpansion

REM Get a list of all the .c files.
SET cFilenames=
FOR /R %%f in (*.c) do (
    SET cFilenames=!cFilenames! %%f
)

REM echo "Files:" %cFilenames%
SET assembly=VulkanEngine
SET compilerFlags=-g -shared -Wvarargs -Wall -Werror
REM -Wall -Werror
SET includeFlags=-Isrc -I%VULKAN_SDK%/Include
SET linkerFlags=-luser32 -lvulkan-1 -L%VULKAN_SDK%/Lib
SET defines=-D_DEBUG -DVKCEXPORT -D_CRT_SECURE_NO_WARNINGS

ECHO [BUILD]: Building %assembly%%...
clang %cFilenames% %compilerFlags% -o ../bin/Win64/%assembly%.dll %defines% %includeFlags% %linkerFlags%
