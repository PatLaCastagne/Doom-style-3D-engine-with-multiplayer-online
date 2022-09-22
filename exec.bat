@echo off
echo Compilation...
gcc src/main.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lws2_32 -lwsock32
gcc src/serveur.c -o bin/serv -I include -L lib -lmingw32 -lws2_32 -lwsock32
echo Finish !
pause
