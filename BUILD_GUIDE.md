# 3D Ship Project - Build and Usage Guide

## Quick Start

### Windows (Code::Blocks)
1. Install Code::Blocks with MinGW compiler
2. Install FreeGLUT library
3. Open `3DShip.cbp` in Code::Blocks
4. Press F9 to build and run

### Linux
```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install build-essential freeglut3-dev libglu1-mesa-dev

# Build and run
make
./3DShip
```

## Project Features

### 3D Ship Model Components
- **Hull**: Brown wooden main body with pointed bow
- **Deck**: Platform surface on top of hull
- **Cabin**: White/gray superstructure with blue windows
- **Masts**: Two vertical cylindrical masts
- **Sails**: White translucent sails

### Visual Effects
- **Lighting**: Ambient, diffuse, and specular lighting
- **Water**: Animated wave surface with transparency
- **Sky**: Gradient background
- **Animation**: Ship rotation and bobbing motion

### Interactive Controls
- **Mouse**: Click and drag to rotate camera view
- **W Key**: Zoom in (closer view)
- **S Key**: Zoom out (farther view)
- **ESC**: Exit application

## Technical Implementation

### Graphics Techniques Used
1. **3D Primitives**: Quads, triangles, cubes, cylinders
2. **Transformations**: Translation, rotation, scaling
3. **Lighting Model**: OpenGL fixed-function pipeline
4. **Transparency**: Alpha blending for water and sails
5. **Animation**: Timer-based updates (60 FPS)
6. **Camera**: Spherical coordinates for orbital view

### Code Structure
- **Drawing Functions**: Separate functions for each ship component
- **Input Handling**: Mouse and keyboard callbacks
- **Animation Loop**: GLUT timer function
- **Initialization**: OpenGL state setup

## Common Build Issues

### Error: "Cannot find -lfreeglut"
**Solution**: Install FreeGLUT development libraries
```bash
# Ubuntu/Debian
sudo apt-get install freeglut3-dev

# Fedora/RHEL
sudo dnf install freeglut-devel
```

### Error: "GL/glut.h: No such file or directory"
**Solution**: Install OpenGL and GLUT headers
```bash
sudo apt-get install libglu1-mesa-dev freeglut3-dev
```

### Windows: Linker errors
**Solution**: Ensure these libraries are linked in project settings:
- opengl32
- glu32
- freeglut (or glut32)

## Customization Ideas

1. **Colors**: Modify RGB values in draw functions
2. **Ship Size**: Adjust scale factors in transformations
3. **Animation Speed**: Change increment values in timer function
4. **Wave Pattern**: Modify sine wave parameters in drawWater()
5. **Camera Position**: Adjust initial camera angles and distance

## Performance Notes

- Target: 60 FPS on modern hardware
- Water grid: 40x40 quads (adjustable)
- Rendering: Double-buffered for smooth display
- Depth testing: Enabled for proper 3D rendering

## Educational Value

This project demonstrates:
- 3D computer graphics fundamentals
- OpenGL API usage
- Geometric transformations
- Lighting and shading
- User interaction
- Animation techniques
- Cross-platform C++ development

## License

Educational project - Free to use and modify for learning purposes.
