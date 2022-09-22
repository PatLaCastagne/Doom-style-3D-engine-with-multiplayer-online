# Simple 3D engine testn with multiplayer online

I created a 3D rendering system similar to Doom’s where the character you control can look sideways but not up or down. I also added a multiplayer system where two players can join a single world using hamachi. All of this was written in C and using the SDL2 library.


## How to run it Online

1. You will need to be connected with your friends to the same network on hamachi.
2. To avoid any kind of connection problems, don’t forget to disable Windows defender in Control Panel
3. To connect the server to your ip, (local or hammachi ip). You can put it in the txt file here
```bash
./IP.txt
```
4. One of you will have to start the server:
```bash
./serv.bat
```
5. You two can then start the engine once on each of your computers. (To play without multiplayer just start the engine without server.)
```bash
./cdm.bat
```
![Capture d’écran (175)](https://user-images.githubusercontent.com/85646339/191774041-1ad8e118-e139-4b1f-9c1d-5ed353dad705.png)

### Controls
* ZQSD to move around
* Mouse to look around
* Mouse button to shoot

