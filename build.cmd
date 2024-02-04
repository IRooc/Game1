REM cl /Zi /I libs\  main.c /link libs/raylib.lib opengl32.lib user32.lib gdi32.lib winmm.lib kernel32.lib msvcrt.lib shell32.lib
clang -o main.exe main.c --debug --no-standard-libraries -L./libs -lraylib -lopengl32 -luser32 -lmsvcrt -lgdi32 -lshell32 -lwinmm

REM webassembly
clang --target=wasm32 -o main.wasm main.c --no-standard-libraries -mbulk-memory -Wl,--no-entry -Wl,--export=game_init -Wl,--export=game_frame -Wl,--export=game_over -Wl,--allow-undefined -DPLATFORM_WEB