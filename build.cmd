REM cl /Zi /I libs\  main.c /link libs/raylib.lib opengl32.lib user32.lib gdi32.lib winmm.lib kernel32.lib msvcrt.lib shell32.lib
clang -o main.exe main.c --no-standard-libraries -L./libs -lraylib -lopengl32 -luser32 -lmsvcrt -lgdi32 -lshell32 -lwinmm

REM webassembly
clang --target=wasm32 -o main.wasm main.c --no-standard-libraries -Wl,--no-entry -Wl,--export=main -Wl,--allow-undefined