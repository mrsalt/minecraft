The goal of the code in this directory is to parse a 3D model and rasterize it into an array (or list) of blocks.
I'm using Visual Studio Code, and I've installed extensions for C++ and CMake, on Windows.
If CMake and a C++ compiler are installed, this should work on Linux too.  TODO: verify

# Step 1: Rasterize 3D model
Input: A perfect 3D model
Output: 3D rasterized cubes

Rasterize that 3D model according to the resolution (scale) that is specified.  (Minecraft blocks are 1 meter cubes, so depending on how large the item will be rendered in Minecraft, and the scale of the original model, the scale value used will vary).
If I store the information for each block separately, I end up duplicating information, and that gets confusing.
So I'm thinking I'll store the data as line segments.

Coordinate order here is x,y,z.

     (0,1,1)  *---------* (1,1,1)
             /|        /|
            / |       / |
           /  |      /  |
  (0,1,0  *---------*---* (1,0,1)
          |  /      |  /
          | /       | /
          |/        |/
  (0,0,0) *---------* (1,0,0)

Two coordinates are not shown: (0,0,1) and (1,1,0).
This is a Left Hand Coordinate system.

I'm only showing one cube here, but the rasterized model would be made up of many cubes.

Think of these line segments as scaffolding being placed around an inside a 3D model.  The places where these lines intersect with the 3D model are filled regions.  In theory, there could be many starts and stops, especially for bumpy objects, if they intersect just the right way with the right scale.  Minecraft blocks are VERY rough however, so we're going to represent each line segment with two data points:
1. Start
2. Stop

If both values are 0, that line segment represents empty space.  If start is 0 and stop is 1, that line segment is completely filled.  Values inbetween (usually either start will be 0 and stop will be 1 and the other value will be somewhere in the middle).

Objects smaller than a cube, or smaller than the cube in 1 or 2 dimensions, are tricky to represent.  A flagpole for example would be tall but not wide or deep.

start_stop[x][y][z]
Thus the amount of memory to rasterize is width (in number of cubes) x height (in number of cubes) x depth (in number of cubes) x 4 bytes.
The start and stop do not need to be precise; x start and stop get 8 bits, y start and stop get 8 bits, z start and stop gets 8 bits.  8 bits are unused.  4 bits for each value means a value from 1 - 16.  Minecraft currently subdivides each block in a given axis a maximum of twice.

I'm going to write this program in C++ to make it easier to deal with values using only 4 bits.

How do we interpret the 3D model to begin with?  The 3D model will be read as a series of polygons.

Besides the polygon's vertices, each polygon will be assigned a min and max x, y, and z value.
Rasterization will take place along x, y, and z planes, incrementing by the size of a block each time.
All polygons will be sorted 3 ways, by their min x, y, and z values.
Thus as we rasterize in the x dimension (x is constant and y and z vary) we will look for all polygons with an x min value less than or equal to the current x position, with an x max value greater than the current position.  Thus as we advance the rasterization plane, we can easily determine which of the set of 'active' polygons is dropping out of the rasterization plane and which new polygons are entering the rasterization plane.

Next we need to intersect each 'active' polygon with the rasterization plane.  This intersection will be a line segment, unless the polygon is exactly parallel to and at the same position as the plane.  In that case we will fill a polygon directly onto the rasterization surface.

# Step 2: Mapping rasterized data to the closest Minecraft block.

