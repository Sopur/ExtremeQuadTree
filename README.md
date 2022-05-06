# ExtremeQT

Extreme QuadTree is an insanely fast QuadTree library made in C++. <br>
This QuadTree includes moving entities and deleting them making it great for game engines/collision systems. <br>
It uses the Node Addon API to run native C++ code in NodeJS. <br>
If you want to use this in raw C++, just include src/qt/tree.cpp.

# Installing

`npm i extremeqt`

# Bench marking

This bench mark is against the extremely popular JS QuadTree, timohausmann-quadtree-js. <br>
All benchmarks can be seen in the `benchmarks/` folder

## Inserting

-   timohausmann: 120ms on average
-   ExtremeQT: 110ms on average

## Retrieving

### Note: timohausmann's QuadTree would return around 400 things on average, while ExtremeQT would return around 15

-   timohausmann: 14 seconds on average
-   ExtremeQT: 18ms on average

# Examples

```js
const ExtQT = require("ExtremeQT");
const width = 100;
const height = 100;

const qt = new ExtQT.Instance(width, height);

qt.create(3, 50, 50); // Create an entity on the QuadTree with an ID of 3, at position 50, 50.
console.log(qt.get(20, 20, 60, 60)); // Get all entities in a square with the width and height of 60, at position 20,20
qt.move(3, 70, 70); // Moves an entity with an ID of 3 into position 70,70
qt.destroy(3); // Deletes an entity with an ID of 3

qt.deallocate(); // Do this when you will not be using the quadtree anymore. Otherwise it will cause a memory leak.
```

# Documentation

## **Constructor**

`new Instance(width, height)`

-   width - The width of the QuadTree
-   height - The height of the QuadTree

## **Instance.create**

### Places an entity on the QuadTree

`create(id, x, y)`

-   id - The identifier of the entity
-   x - Its position on the X-axis
-   y - Its position on the Y-axis

## **Instance.destroy**

### Deletes an entity on the QuadTree

`destroy(id)`

-   id - The identifier of the entity

## **Instance.move**

### Moves an entity to a different position

`destroy(id, x, y)`

-   id - The identifier of the entity
-   x - The new position on the X-axis
-   y - The new position on the Y-axis

## **Instance.get**

### Gets all the entities in the provided square

`get(x, y, width, height)`

-   x - The start position on the X-axis
-   y - The start position on the Y-axis
-   width - The width of the square
-   height - The height of the square

## **Instance.deallocate**

### Deallocates the QuadTree

`deallocate()`

# Limitations

-   All positions, IDs, and the QuadTree dimensions all have to be unsigned 32bit numbers, limiting their values to under 4294967296.
-   All positions, IDs, and the QuadTree dimensions cannot be negative
-   You cannot create, move, or get entities outside the QuadTree statically set dimensions.
-   There cannot be more than 10 entities in a 2x2 space
