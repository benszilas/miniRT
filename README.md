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
sudo apt-get install -y build-essential libpthread-stubs0-dev xorg libx11-dev libxext-dev libbsd-dev
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

move camera or object: WASDQE

rotate camera or object: arrow keys

select object: right click

change object properties: click on object specific menu item

undo object selection: click on empty space

move object on xy axis: lmb + drag

add object: click menu item

change fov: mouse wheel

# the .rt file describes a 3d scene

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
∗ orientation vector for each x,y,z axis:
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
∗ 3d vector perpendicular to the plane. for each x,y,z axis: 0.0,1.0,0.0 ∗ R,G,B colors in range [0-255]: 0,0,225

  ◦ Cylinder:
∗ identifier: cy
     ∗ x,y,z coordinates of the center of the cylinder: 50.0,0.0,20.6
∗ 3D vector x,y,z of axis of cylinder
∗ the cylinder diameter: 14.2
∗ the cylinder height: 21.42
∗ R,G,B colors in range [0,255]: 10, 0, 255

Cone:
∗ identifier: cn
     ∗ x,y,z coordinates of the vertex of the cone: 50.0,0.0,20.6
∗ 3D vector x,y,z of axis of cone
∗ the diameter of the bottom disk of the cone: 14.2
∗ the cone height: 21.42
∗ R,G,B colors in range [0,255]: 10, 0, 255
