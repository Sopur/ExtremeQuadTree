#pragma once
#include "child.cpp"
#include "def.h"
#include "unordered_map"
#include <cstdint>

class ExtTree {
private:
public:
    ExtTreeVal width;
    ExtTreeVal height;
    ExtTreeCache* cache;
    ExtTreeChild child;

    ExtTree(ExtTreeVal w, ExtTreeVal h) :
        width(w),
        height(h),
        cache(new ExtTreeCache),
        child(0, 0, w, h, 0b00, cache) { }
    ~ExtTree() {
        // Auto deletes child
        delete cache;
    }

    // Creates a node
    ExtTreeRes create(ExtTreeID id, ExtTreeVec2d pos) {
        if (cache->count(id) != 0) {
            return ExtTreeRes::ERR_ALREADY_EXISTS;
        }
        cache->insert({id, {nullptr, 0}});
        return (child.add(id, pos));
    }

    // Removes a node
    ExtTreeRes destory(ExtTreeID id) {
        if (cache->count(id) == 0) {
            return ExtTreeRes::ERR_DOES_NOT_EXIST;
        }
        ExtTreeRes status = (((ExtTreeChild*) cache->operator[](id).host)->destroy(id));
        cache->erase(id);
        return status;
    }

    // Does the create and destroy functions natively
    ExtTreeRes move(ExtTreeID id, ExtTreeVec2d pos) {
        if (cache->count(id) == 0) {
            return ExtTreeRes::ERR_DOES_NOT_EXIST;
        }
        (((ExtTreeChild*) cache->operator[](id).host)->destroy(id)); // Delete the old one
        return (child.add(id, pos));                                 // Add it to a new place
    }

    // Finds all the entities in a square
    ExtTreeCapture* get(ExtTreeVal x, ExtTreeVal y, ExtTreeVal width, ExtTreeVal height) {
        ExtTreeCapture* captured = new ExtTreeCapture;
        ExtTreeCollisionMatrix matrix = {0};
        child.getArea(captured, {{x, y}, {x + width, y}, {x, y + height}, {x + width, y + height}}, matrix);
        return captured;
    }
};

#undef maxHold