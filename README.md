# Raylib Particle Toy
Based on [Codemaker4 Raylib Particle Toy](https://github.com/codemaker4/raylib-particle-toy) but with performance improvements.
## Description
This is a little project playground to mess with particles.

In Codemaker4 implementation, every particle is drawn with the ``DrawPixelV()`` function, which causes 1 draw call per particle. So I decided to experiment and improve it with the method of drawing pixels on the texture, resulting in only 1 draw call and a nice performance improvement. It doesn't call a texture draw on particles that are outside the screen. The texture in which particles are drawn can be of lower or higher resolution than the screen. 

You can move the camera with WASD and zoom using the scroll wheel. Pressing the scroll wheel will reset the zoom.
It is easy to change and play with parameters. So go for it!

## Compile the project
The entire repository is a Visual Studio (Community Edition 2022) project, so you can open and compile with it. 

[Raylib 5.0 library](https://github.com/raysan5/raylib) is included in the project.

## Contribution
Feel free to contribute to this project and suggest some ideas for it. 
## License
Under the MIT license. Feel free to use it. Never lose your smile :)
