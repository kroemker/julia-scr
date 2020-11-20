# julia-scr
This is a Windows screensaver visualizing Julia sets by plotting the complex plane. To use it as a screensaver, rename the compiled file extension to `.scr`, move it into the `Windows/system32` folder, and set it as screensaver in the Windows menu. The [SFML](https://www.sfml-dev.org) library is used to render the sets, so make sure to link against it when compiling.

## Features
- different color schemes (red, green, blue, HSV)
- multiple complex functions
- fps adjustment mechanism
- two modes:
  1. rendering parametrized Julia sets by moving the parameter randomly over the complex plane
  2. zoom in on an edge point of a static Julia set
