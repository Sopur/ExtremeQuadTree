#pragma once
#include <cstdint>
#include <unordered_map>
#include <vector>

#define maxHold 10
#define NULL_ID 0
typedef uint32_t ExtTreeVal;
typedef uint32_t ExtTreeID;
typedef uint16_t containerIndex;

struct ExtTreeVec2d {
    ExtTreeVal x, y;
};

enum class ExtTreeRes : char {
    SUCCESS,
    WARN_NO_MORE_ENTITIES_IN_SAME_SPACE_ALLOWED,
    ERR_DOES_NOT_EXIST,
    ERR_ALREADY_EXISTS,
    ERR_NO_MEMORY,
    ERR_TOO_MANY_ENTITIES_IN_SAME_SPACE,
    ERR_INVALID_INPUT,
};

struct ExtTreeCacheData {
    void* host;
    containerIndex localIndex;
};

struct NodeData {
    ExtTreeID id;
    ExtTreeVec2d pos;
};

struct ExtTreeSquare {
    ExtTreeVec2d topLeft;
    ExtTreeVec2d topRight;
    ExtTreeVec2d bottomLeft;
    ExtTreeVec2d bottemRight;
};

typedef std::unordered_map<ExtTreeID, ExtTreeCacheData> ExtTreeCache;
typedef std::vector<ExtTreeID> ExtTreeCapture;
typedef uint8_t ExtTreeCollisionMatrix[0b11 + 1];
