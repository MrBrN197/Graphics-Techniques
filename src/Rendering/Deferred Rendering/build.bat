@echo off

set IncludePaths=/I ..\..\..\Dependencies\GLFW\include /I ..\..\..\Dependencies\Glad\include /I ..\..\common
set Defines=/D_CRT_SECURE_NO_WARNINGS
set LibraryPaths=/LIBPATH:..\..\..\Dependencies\GLFW\lib\ /LIBPATH:..\..\..\Dependencies\Glad\lib\
cl %IncludePaths% %Defines% /EHcs /MTd /Zi /Od /Gr /fp:fast /fp:except- /WX /W3 /Fe:Main.exe main.cpp /link %LibraryPaths% glfw3.lib glad.lib user32.lib gdi32.lib shell32.lib
REM cl %IncludePaths% /P main.cpp
