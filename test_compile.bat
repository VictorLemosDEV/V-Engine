@echo off
cd /d "C:\Users\victo\CLionProjects\Vector Learn"
"C:\Program Files\JetBrains\CLion 2025.1.3\bin\mingw\bin\g++.exe" -I"libs\GLAD\include" -I"libs\GLFW\include" -std=gnu++20 -c "src\main.cpp" -o test_main.obj 2>&1
echo.
echo === Compiling glad.c ===
"C:\Program Files\JetBrains\CLion 2025.1.3\bin\mingw\bin\gcc.exe" -I"libs\GLAD\include" -I"libs\GLFW\include" -c "src\glad.c" -o test_glad.obj 2>&1
pause
