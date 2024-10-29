This minimalistic raytracing engine is a school project which is still ongoing.
It is using the [Minilibx API](https://github.com/42Paris/minilibx-linux).
All code besides the directory minilibx-linux is our work.
Big thanks to my teammate [vman101](https://github.com/vman101) for implementing many fantastic creative ideas that go way beyond the scope of what is required by the school.
Credits go to him for his libft and his garbage collector in the memory directory.
We are still working on this fun raytracing engine so there is more to come!

# Run using Docker:

```bash
git clone git@github.com:Fapad/miniRT.git
cd miniRT
docker build -t minirt .
docker run --rm -e DISPLAY="$DISPLAY" -v /tmp/.X11-unix:/tmp/.X11-unix minirt
```
If you get the error "Authorization required, but no authorization protocol specified", you might need to enable X11 forwarding locally

```bash
xhost + local:
docker run --rm -e DISPLAY="$DISPLAY" -v /tmp/.X11-unix:/tmp/.X11-unix minirt
```

# Or compile with make
## 1. Required Dependencies

```bash
sudo apt-get update -y
sudo apt-get install -y build-essential xorg libx11-dev libxext-dev libbsd-dev
```
## 2. clone the repo and compile

```bash
git clone git@github.com:Fapad/miniRT.git
cd miniRT
make
```

## 3. run the executable 
either without arguments,
```bash
./miniRT
```
or with the path to a specific scene file as argument
```bash
./miniRT scenes/eclipse.rt
```

# Navigation:

change resolution (for faster movement): numpad - or numpad +

select object: right click

move camera or object: WASDQE

rotate camera or object: arrow keys

undo object selection: left click on empty space

change fov: mouse wheel

move object: lmb + drag

add object: click menu item

# School requirements:

Allowed functions
• open, close, read, write,
printf, malloc, free, perror,
strerror, exit
• All functions of the math
library (-lm man man 3 math)
• All functions of the MinilibX

The constraints are as follows:
• You must use the miniLibX. Either the version that is available on the operating
system, or from its sources. If you choose to work with the sources, you will
need to apply the same rules for your libft as those written above in Common
Instructions part.

• The management of your window must remain fluid: switching to another window,
minimization, etc..

• When you change the resolution of the window, the content of the window must
remain unchanged and be adjusted accordingly.

• You need at least these 3 simple geometric objects: plane, sphere, cylinder.

My first RayTracer with miniLibX
• If applicable, all possible intersections and the inside of the object must be handled
correctly.

• Your program must be able to resize the object’s unique properties: diameter for a
sphere and the width and height for a cylinder.

• Your program must be able to apply translation and rotation transformation to
objects, lights and cameras (except for spheres and lights that cannot be rotated).

• Light management: spot brightness, hard shadows, ambiance lighting (objects are
never completely in the dark). You must implement Ambient and diffuse lighting.

the program displays the image in a window and respect the following rules:
◦ Pressing ESC must close the window and quit the program cleanly.

◦ Clicking on the red cross on the window’s frame must close the window and
quit the program cleanly.

◦ The use of images of the minilibX is strongly recommended.

# the .rt file format describes a 3d scene

• Your program must take as a first argument a scene description file with the .rt
extension.

◦ Each type of element can be separated by one or more line break(s).

◦ Each type of information from an element can be separated by one or more
space(s).

◦ Each type of element can be set in any order in the file.

◦ Elements which are defined by a capital letter can only be declared once in
the scene.

Each element first’s information is the type identifier (composed by one or two character(s)), followed by all specific information for each object in a strict order such as:

◦ Ambient lightning:
∗ identifier: A
∗ ambient lighting ratio in range [0.0,1.0]: 0.2 ∗ R,G,B colors in range [0-255]: 255, 255, 255
    
  ◦ Camera:
∗ identifier: C
     ∗ x,y,z coordinates of the view point: -50.0,0,20
∗ 3d normalized orientation vector. In range [-1,1] for each x,y,z axis:
0.0,0.0,1.0
∗ FOV : Horizontal field of view in degrees in range [0,180]: 70

◦ Light:
∗ identifier: L
∗ x,y,z coordinates of the light point: -40.0,50.0,0.0
∗ the light brightness ratio in range [0.0,1.0]: 0.6
∗ (unused in mandatory part)R,G,B colors in range [0-255]: 10, 0, 255

◦ Sphere:
∗ identifier: sp
∗ x,y,z coordinates of the sphere center: 0.0,0.0,20.6 ∗ the sphere diameter: 12.6
∗ R,G,B colors in range [0-255]: 10, 0, 255

◦ Plane:
∗ identifier: pl
∗ x,y,z coordinates of a point in the plane: 0.0,0.0,-10.0
∗ 3d normalized normal vector. In range [-1,1] for each x,y,z axis: 0.0,1.0,0.0 ∗ R,G,B colors in range [0-255]: 0,0,225

  ◦ Cylinder:
∗ identifier: cy
     ∗ x,y,z coordinates of the center of the cylinder: 50.0,0.0,20.6
∗ 3d normalized vector of axis of cylinder. In range [-1,1] for each x,y,z axis:
∗ the cylinder diameter: 14.2
∗ the cylinder height: 21.42
∗ R,G,B colors in range [0,255]: 10, 0, 255
