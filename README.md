# HTTP-Server
This package provides a configurable HTTP 1.0 implemented server written in C.

Created by:
| Username | Name | Student # |
|-|-|-|
|[imqt](https://github.com/imqt)|Bui, Quoc Trung|A01071612|
|[vvnwu](https://github.com/vvnwu)|Wu, Vivian|A01076716|
|[hanssy130](https://github.com/hanssy130)hanssy130|Sy, Hans|A00892259|

## Features
- Supports HTTP 1.0
- Handles multiple clients using threads or processes (configurable)
- Configurable 404 page
- Configurable html root
- supports HEAD, GET, and POST
- ncurses GUI configuration program
- Runs on Mac, Ubuntu, and FreeBSD
- Snake game!!!

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
This command runs the server
### `./edit-config`
This opens `edit-config.txt` for editing.
