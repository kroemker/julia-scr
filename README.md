# julia-scr
This is a Julia set visualizer that can be used as a screensaver. The [SFML](https://www.sfml-dev.org) library is used for visualization, so make sure to link against it when compiling.

## Features
- different color schemes (red, green, blue, HSV)
- multiple complex functions
- fps adjustment mechanism
- two screensaver modes:
  1. rendering parametrized Julia sets by moving the parameter randomly over the complex plane
  2. zoom in on an edge point of a static Julia set
- interactive mode

## Usage
- as a screensaver:
  1. Compile and rename the extension to `.scr`
  2. Move the executable to your `Windows/system32` folder
  3. Set it as the screensaver in the Windows menu
- interactive mode:
  1. Compile, open a terminal, and start the program by typing:
  ```julia-scr \i```
  2. Use arrow keys to adjust the complex parameter `c`, use R to reset the parameter
  3. Use PageUp/PageDown keys to adjust the number of iterations
  4. Use F1-F4 keys to adjust the complex function
  5. Use T to enable/disable the HUD
  6. Use W/A/S/D to move the focal point
  7. Use Q/E to zoom in/out