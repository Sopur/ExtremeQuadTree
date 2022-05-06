const QuadTreeJS = require("./timohausmann-quadtree-js.js");
const ExtremeQT = require("../src/exports/wrapper.js");
const qt1 = new QuadTreeJS({ x: 0, y: 0, width: 100_000, height: 100_000 });
const qt2 = new ExtremeQT.Instance(100_000, 100_000);

console.log("Inserting bench mark");

console.time("timohausmann-quadtree-js");
for (let i = 1; i < 100_000; i++) {
    qt1.insert({
        x: Math.round(Math.random() * (100_000 - 1)),
        y: Math.round(Math.random() * (100_000 - 1)),
        width: 10,
        height: 10,
    });
}
console.timeEnd("timohausmann-quadtree-js");

console.time("ExtremeQT");
for (let i = 1; i < 100_000; i++) {
    qt2.create(i, Math.round(Math.random() * (100_000 - 1)), Math.round(Math.random() * (100_000 - 1)));
}
console.timeEnd("ExtremeQT");
