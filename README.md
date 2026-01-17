# 3D Ship - Computer Graphics Project

A 3D ship visualization project built with C++, OpenGL, and GLUT. This project demonstrates 3D modeling, lighting, animation, and interactive camera controls.

## Features

- **3D Ship Model**: Detailed ship with hull, deck, cabin, masts, and sails
- **Animated Water**: Wave-based ocean surface with transparency
- **Lighting System**: Dynamic lighting with ambient, diffuse, and specular components
- **Interactive Camera**: Mouse-based rotation and keyboard zoom controls
- **Smooth Animation**: Ship bobbing motion and rotating display
- **Sky Background**: Gradient sky for enhanced visual appeal

## Controls

- **Left Mouse Button + Drag**: Rotate the camera around the ship
- **W Key**: Zoom in (move camera closer)
- **S Key**: Zoom out (move camera farther)
- **ESC Key**: Exit the application

## Prerequisites

### Windows (Code::Blocks)
- Code::Blocks IDE (version 17.12 or later recommended)
- MinGW compiler
- FreeGLUT library

### Linux
- GCC/G++ compiler
- OpenGL development libraries
- GLUT/FreeGLUT development libraries

Install dependencies on Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install freeglut3-dev
sudo apt-get install libglu1-mesa-dev
```

Install dependencies on Fedora/RHEL:
```bash
sudo dnf install gcc-c++
sudo dnf install freeglut-devel
sudo dnf install mesa-libGLU-devel
```

## Building and Running

### Using Code::Blocks (Windows)

1. Open `3DShip.cbp` in Code::Blocks
2. Go to **Build → Build** (or press F9)
3. Run the project with **Build → Run** (or press Ctrl+F10)

### Using Makefile (Linux)

```bash
# Build the project
make

# Run the application
make run

# Clean build files
make clean
```

### Manual Compilation

```bash
# Linux
g++ -o 3DShip main.cpp -lGL -lGLU -lglut -lm

# Windows (MinGW)
g++ -o 3DShip.exe main.cpp -lopengl32 -lglu32 -lfreeglut -lm
```

## Project Structure

```
3D---Ship/
├── main.cpp          # Main source code with OpenGL rendering
├── 3DShip.cbp        # Code::Blocks project file
├── Makefile          # Linux build configuration
└── README.md         # This file
```

## Code Overview

### Main Components

1. **Ship Model**: Composed of multiple geometric primitives
   - Hull: Main body using quads
   - Bow: Triangular front section
   - Deck: Platform on top of hull
   - Cabin: Superstructure with windows
   - Masts: Cylindrical structures
   - Sails: Quad-based translucent surfaces

2. **Water Surface**: Grid-based animated surface using sine waves

3. **Lighting**: OpenGL lighting with positioned light source

4. **Camera System**: Spherical coordinate-based camera with mouse control

5. **Animation**: Timer-based updates for rotation and wave motion

## Technical Details

- **Graphics API**: OpenGL with GLUT
- **Language**: C++
- **Rendering**: Double-buffered with depth testing
- **Shading**: Smooth shading with lighting enabled
- **Transparency**: Alpha blending for water and sails

## Screenshots

The application displays a 3D ship floating on animated water with:
- Brown wooden hull and deck
- White/gray cabin structure
- White sails on dark masts
- Blue translucent water with wave animation
- Light blue sky gradient background

## Troubleshooting

### Common Issues

**Issue**: "freeglut.h: No such file or directory"
- **Solution**: Install FreeGLUT development libraries (see Prerequisites)

**Issue**: "undefined reference to glutInit"
- **Solution**: Link against GLUT library (-lglut or -lfreeglut)

**Issue**: Black screen or no display
- **Solution**: Ensure graphics drivers are up to date and OpenGL is supported

## Credits

This is a computer graphics project assignment demonstrating 3D modeling and OpenGL programming concepts.

## License

Educational project - free to use and modify for learning purposes.
