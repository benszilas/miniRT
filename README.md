This minimalistic raytracing engine is a 42 school project which is still ongoing.
It is using the [Minilibx API](https://github.com/42Paris/minilibx-linux) made for 42 students.
Big thanks to my teammate [vman101](https://github.com/vman101) for implementing many fantastic creative ideas that go way beyond the scope of what is required by the school.
We are still working on this fun raytracing engine so there is more to come!

# Run in Docker:

```bash
docker build -t minirt .
docker run --rm -e DISPLAY="$DISPLAY" -v /tmp/.X11-unix:/tmp/.X11-unix minirt
```
you might need to enable X11 forwarding for local non-network users for docker run to work

```bash
xhost + local:
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

Parsing a scene file

• Your program must take as a first argument a scene description file with the .rt
extension.

◦ Each type of element can be separated by one or more line break(s).

◦ Each type of information from an element can be separated by one or more
space(s).

◦ Each type of element can be set in any order in the file.

◦ Elements which are defined by a capital letter can only be declared once in
the scene.
