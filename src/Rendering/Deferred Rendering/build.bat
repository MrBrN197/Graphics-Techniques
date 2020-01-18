@echo off

set IncludePaths=/I ..\..\..\Dependencies\GLFW\include /I ..\..\..\Dependencies\Glad\include
set LibraryPaths=/LIBPATH:..\..\..\Dependencies\GLFW\lib\ /LIBPATH:..\..\..\Dependencies\Glad\lib\
cl %IncludePaths% /EHcs /MTd /Zi /Od /Gr /fp:fast /fp:except- /WX /W3 /Fe:Main.exe main.cpp /link %LibraryPaths% glfw3.lib glad.lib user32.lib gdi32.lib shell32.lib


if %ERRORLEVEL% EQU 0 (
    main.exe
)
