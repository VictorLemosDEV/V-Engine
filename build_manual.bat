@echo off
cd /d "C:\Users\victo\CLionProjects\Vector Learn"

echo Compiling with MSVC...
cl /I"libs\GLAD\include" /I"libs\GLFW\include" /EHsc /std:c++20 src\main.cpp src\glad.c /link /LIBPATH:"libs\GLFW\lib-vc2022" glfw3.lib opengl32.lib /OUT:Vector_Learn.exe

if %errorlevel% == 0 (
    echo Build successful!
    echo Copying DLL...
    copy "libs\GLFW\lib-vc2022\glfw3.dll" .
    echo.
    echo Running the program...
    Vector_Learn.exe
) else (
    echo Build failed!
)

pause
