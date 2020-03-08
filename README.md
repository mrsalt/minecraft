# minecraft
Let's write some code to automate Minecraft.

Minecraft has published APIs (I've used a python API on Raspberry Pi) that allow you to get state information on the game and to change the game state (place/change a block at specified coordinates).  These APIs open the door to automation.

## Construction
The first goal of this project is to automate construction.  If you've spent much time playing minecraft, you know that to build something really cool looking takes a long time.  In a way, that's part of the fun.  What can you accomplish with limited time and tools?  How good are you at selecting the right blocks, counting the blocks as you place them, picking the right terrain to build your structure at, etc.  At the same time, as a software developer, I know there are ways we could make the construction of builds *way* faster.

Ideas for automating construction
1. 3D Models
3D models are made by artists and 3D scanning software, or a combination of the two.  There are multiple file formats, but at least one of them has been around for a long time [^1]!
2. Procedurally Generated Structures
Some structures have simple geometry and are well suited for creation without a tremendous amount of code.  For example, with about 20 lines of python I was able to generate a pretty decent skyscraper.
```
TODO: copy that python code right here.
```
3. Procedurally assisted structures
With this idea, I imagine placing the outline of a building, like a castle, in the ground, using a particular block.  Or perhaps just the location of towers, and the type of block would correspond to the height and/or width of the tower (and shape - circular vs. square.  Perhaps blocks could be stacked on top as additional 'parameters' for the automated construction of the tower).  Walls would be formed automatically between towers.

I'm most excited about ideas 1 & 3.  #1 just for the shear fun of figuring out how 3D rendering works again, and because my daughter just did a report on the Colosseum in ancient Rome and wants to build it and I'm telling her we can do it with code way faster and better.  #3 because it feels more true to the spirit of minecraft, just a way to augment your creativity.  Like replacing a hand shovel with a diesel powered excavator.

[^1]: I took CS 5400, Computer Graphics, at Utah State University, I believe Spring Semester 2001.  Basically after learning some basic 2D line and circle rasterization methods, we jumped into building our own 3D rendering engine using Matrices.  We started to add support for texture rendering (shaders) into our engines by the end of the class.  It was a lot of fun.  Anyway, I found a 3D model of a dragon; my engine displayed the dragon quite nicely.  The same file can be opened in Paint 3D today, so it must be a pretty standard format.