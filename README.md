# HTTP-Server
This package provides a configurable HTTP 1.0 implemented server written in C.

[Watch the video here!](https://youtu.be/5z02WyU6e2s)

Created by:
| Username |
|-|
|[imqt](https://github.com/imqt)|
|[vvnwu](https://github.com/vvnwu)|
|[hanssy130](https://github.com/hanssy130)|

## Required Features
Requirement breakdown for BCIT CST COMP 4981 Advanced Networking
- Supports HTTP 1.0
- HTTP GET  	                     10%
- HTTP HEAD 	                     5%
- Threads 	                       10%
- Sub-Process                      10%
- An IPC mechanism                 10%
- Config file 	                   10%
- ncurses GUI                      10% 
- Runs on Mac, Ubuntu, and FreeBSD 20%
- 10+ concurrent connections 	     10%
- html root                        2.5%
- 404 	                           2.5%

Server makes use of the file command to get content type of the requested file and sends the file in uint8_t.

## Bonus Features:
- ***Home page*, *404 page*, and *root directory*** can be changed while the server is running
  by entering HOME_PAGE/404_PAGE/ROOT_FOLDER:new_value and hitting enter.
  The server info will then be reloaded and the changes will take effect without having to rerun the server.
- Made use of pipe, shared memory, message queues, and domain sockets.
- HTML files you should check out because they are cute:
    /game/snake.html
    /game/chatnoir.html
    /404.html
    and maybe the /animation folder as well if you are have time and want to see some cute stuff.
    





    

## Installation
### Requirements
- CMake
### Steps
1. Clone the repository to your computer.
2. Navigate to the `src` folder.
3. Use `mkdir build` to create a directory called build.
4. Navigate inside the `build` folder.
5. Run `cmake ..`.
6. Run `cmake --build .`.

## Usage
### `./server`
This command runs the server.
### `./edit-config`
This opens `edit-config.txt` for editing.
