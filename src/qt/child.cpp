#pragma once
#include "def.h"
#include <cstdint>
#include <cstdlib>
#include <iostream>

#define findSector(nodeX, nodeY) \
    children[(nodeX > midX)][(nodeY > midY)]

inline void lock2d(ExtTreeVec2d& val, ExtTreeVec2d min, ExtTreeVec2d max) {
    if (val.x > max.x)
        val.x = max.x;
    else if (val.x < min.x)
        val.x = min.x;

    if (val.y > max.y)
        val.y = max.y;
    else if (val.y < min.y)
        val.y = min.y;
}

inline void lockSquare2d(ExtTreeSquare& square, ExtTreeVec2d min, ExtTreeVec2d max) {
    lock2d(square.topRight, min, max);
    lock2d(square.topLeft, min, max);
    lock2d(square.bottemRight, min, max);
    lock2d(square.bottomLeft, min, max);
}

class ExtTreeChild {
private:
public:
    bool isFull;
    bool hasSplit;

    uint8_t childIndex;
    ExtTreeChild* children[2][2];
    NodeData container[maxHold];
    containerIndex nextIndex;
    ExtTreeVal x;
    ExtTreeVal y;
    ExtTreeVal bottemX;
    ExtTreeVal bottemY;
    ExtTreeVal widthHalf;
    ExtTreeVal heightHalf;
    ExtTreeVal midX;
    ExtTreeVal midY;
    ExtTreeCache* cache;

    ExtTreeChild(ExtTreeVal _x, ExtTreeVal _y, ExtTreeVal _w, ExtTreeVal _h, uint8_t _ci, ExtTreeCache* _c) :
        isFull(false),
        hasSplit(false),
        childIndex(_ci),
        nextIndex(0),
        x(_x),
        y(_y),
        bottemX(x + _w),
        bottemY(y + _h),
        widthHalf(_w >> 1),
        heightHalf(_h >> 1),
        midX(x + widthHalf),
        midY(y + heightHalf),
        cache(_c) {
        for (int i = 0; i < maxHold; i++) {
            container[i] = {NULL_ID, {0, 0}};
        }
    }

    ~ExtTreeChild() {
        if (hasSplit == true) {
            delete children[0][0];
            delete children[1][0];
            delete children[0][1];
            delete children[1][1];
        }
    }

    ExtTreeRes split(void) {
        if ((widthHalf == 1) || (heightHalf == 1)) {
            isFull = true;
            // Return a warning to let the user know the quadtree is at its limit
            return ExtTreeRes::WARN_NO_MORE_ENTITIES_IN_SAME_SPACE_ALLOWED;
        }
        hasSplit = true;

        // |X|O|
        // |O|O|
        children[0][0] = new ExtTreeChild(x, y, widthHalf, heightHalf, 0b00, cache);

        // |O|X|
        // |O|O|
        children[1][0] = new ExtTreeChild(midX, y, widthHalf, heightHalf, 0b10, cache);

        // |O|O|
        // |X|O|
        children[0][1] = new ExtTreeChild(x, midY, widthHalf, heightHalf, 0b01, cache);

        // |O|O|
        // |O|X|
        children[1][1] = new ExtTreeChild(midX, midY, widthHalf, heightHalf, 0b11, cache);

        if (children[0][0] == nullptr) return ExtTreeRes::ERR_NO_MEMORY;
        if (children[1][0] == nullptr) return ExtTreeRes::ERR_NO_MEMORY;
        if (children[0][1] == nullptr) return ExtTreeRes::ERR_NO_MEMORY;
        if (children[1][1] == nullptr) return ExtTreeRes::ERR_NO_MEMORY;

        // Sort all the data in the new sectors
        // This child is now a pointer to other sectors
        for (int i = 0; i < maxHold; i++) {
            NodeData data = container[i];
            findSector(data.pos.x, data.pos.y)->add(data.id, data.pos);
        }

        return ExtTreeRes::SUCCESS;
    }

    ExtTreeRes add(ExtTreeID id, ExtTreeVec2d pos) {
        if (hasSplit == true) {
            return (findSector(pos.x, pos.y)->add(id, pos));
        }

        if (isFull == true) {
            // Reject the add
            return ExtTreeRes::ERR_TOO_MANY_ENTITIES_IN_SAME_SPACE;
        }

        ExtTreeCacheData& ref = cache->operator[](id);
        // Set the cache data to be easily found again
        ref.host = this;
        ref.localIndex = nextIndex;

        container[nextIndex] = {id, pos};

        for (int i = 0; i < maxHold; i++) {
            if (container[i].id == 0) {
                // If the tree is able to find the next available space, return a success
                nextIndex = i;
                return ExtTreeRes::SUCCESS;
            }
        }

        // Otherwise there are no spaces left, so split
        return (split());
    }

    ExtTreeRes destroy(ExtTreeID id) {
        ExtTreeCacheData& data = cache->operator[](id);
        nextIndex = data.localIndex; // Allow that space to be filled up again
        container[data.localIndex].id = 0;
        isFull = false;
        return ExtTreeRes::SUCCESS;
    }

    void getArea(ExtTreeCapture* capture, ExtTreeSquare square, ExtTreeCollisionMatrix& matrix) {
        // Check if the matrix provided already read a child from your id
        if (matrix[childIndex] == 1) return;

        // If not make it so
        matrix[childIndex] = 1;

        if (hasSplit == false) {
            // If it isn't split, no need to do any 2d math
            for (int i = 0; i < maxHold; i++) {
                if (container[i].id == NULL_ID) continue;
                capture->push_back(container[i].id);
            }
        } else {
            // Create a matrix to stop from reading from a child more than once
            ExtTreeCollisionMatrix collisionMaxtix = {0};

            // Now find what corners reach what children

            // Align the square with the child
            lockSquare2d(square, {x, y}, {bottemX, bottemY});

            // Test all the corners to see what children they hit
            findSector(square.topRight.x, square.topRight.y)->getArea(capture, square, collisionMaxtix);
            findSector(square.topLeft.x, square.topLeft.y)->getArea(capture, square, collisionMaxtix);
            findSector(square.bottemRight.x, square.bottemRight.y)->getArea(capture, square, collisionMaxtix);
            findSector(square.bottomLeft.x, square.bottomLeft.y)->getArea(capture, square, collisionMaxtix);
        }
    }
};