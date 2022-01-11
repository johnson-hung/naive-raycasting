# Naive Raycasting
-[demo](https://youtu.be/Z8ZcTcYVCt0)
<br />

This is a game prototype that showcases the concept of 3D raycasting, integration of the [SDL2](https://github.com/libsdl-org/SDL) library, and ways to manage game states.

## Features
- Showcased the concept of 3D raycasting with an extensible game architecture
- Supported low-level access to keyboard and graphics hardware via SDL2 Library
- Applied object-oriented programming approach to create flexible and reusable software design

## Directory Structure
```
$ tree -d naive-raycasting

naive-raycasting
├── fonts		<- Font files 	  (.ttf)
├── include		<- Header files	  (.h)
├── obj			<- Object files   (.o)
├── src			<- Source files   (.cpp)
└── textures		<- Texture images (.bmp)
```

## Compile and Run
To compile the program, you may first clone this repo to your local workspace by executing the following command in the terminal:
<br />

```
git clone https://github.com/johnson-hung/naive-raycasting.git
```

After cloning the repo, make sure that you have [SDL2](https://libsdl.org/) and [SDL_ttf 2.0](https://www.libsdl.org/projects/SDL_ttf/) installed in your system:
<br />

- For Linux, execute: <br />
	```
	sudo apt-get install libsdl2-dev
	sudo apt-get install libsdl2-ttf-dev
	```
- For MacOS, execute: <br />
	```
	brew install sdl2
	brew install sdl2_ttf
	```

Then, you may compile the program by executing the following commands under the project root directory:
 ```
 make
 ``` 
 or
 ```
 make run
 ```
 
Now, you can run the program by executing:<br />
```
./game
```

## Resources
- The raycasting functionality was developed on the basis of [this tutorial](https://github.com/ssloy/tinyraycaster) with extra features and refined software design combined
- For game state management, [this](http://lazyfoo.net/articles/article06/index.php) and [this](http://gamedevgeek.com/tutorials/managing-game-states-in-c/) had provided non-trivial examples that could be put to good use.
