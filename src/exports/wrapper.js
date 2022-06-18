const addon = require("bindings")("ExtremeQT");

const NULL_ID = 0;
const bit32Limit = 4294967296;
const ResponseStatues = {
    0: "SUCCESS",
    1: "WARN_NO_MORE_ENTITIES_IN_SAME_SPACE_ALLOWED",
    2: "ERR_DOES_NOT_EXIST",
    3: "ERR_ALREADY_EXISTS",
    4: "ERR_NO_MEMORY",
    5: "ERR_TOO_MANY_ENTITIES_IN_SAME_SPACE",
    6: "ERR_INVALID_INPUT",
};

class ExtremeQTInstanceError extends Error {
    constructor(message) {
        super(`ExtremeQT instance raised an error: "${message}"`);
    }
}

class ExtremeQTInternalError extends Error {
    constructor(message) {
        super(`ExtremeQT internally raised an error: "${message}"`);
    }
}

function verifyUint32(number) {
    return typeof number === "number" && number >= 0 && number < bit32Limit;
}

function verifyUint32Array(array) {
    for (const value of array) {
        if (verifyUint32(value) === false) return false;
    }
    return true;
}

function dealResponse(status) {
    if (status > 1) {
        throw new ExtremeQTInternalError(ResponseStatues[status]);
    }
    return status;
}

class Instance {
    constructor(width, height) {
        if (!verifyUint32Array([width, height])) {
            throw new ExtremeQTInstanceError("Width and height parameters must be present and uint32 numbers");
        }
        if (width < 4 || height < 4) {
            throw new ExtremeQTInstanceError("The width and height parameters must be at least 4 in value");
        }

        // Floor width and height
        this.width = width << 0;
        this.height = height << 0;
        this.#ctx = addon.New(this.width, this.height); // Returns a pointer to that quadtree in memory
        if (this.#ctx === 0) {
            throw new ExtremeQTInstanceError("Could not allocate the QuadTree");
        }
    }

    create(id, x, y) {
        if (!verifyUint32Array([id, x, y])) {
            throw new ExtremeQTInstanceError("ID, x, and y parameters must be present and uint32 numbers");
        }
        if (x > this.width || y > this.height) {
            throw new ExtremeQTInstanceError("Cannot create an entity outside the QuadTree");
        }
        if (id === NULL_ID) {
            throw new ExtremeQTInstanceError(`The ID parameter can't be a null ID (${NULL_ID})`);
        }

        const res = addon.create(this.#ctx, id, x, y);
        return dealResponse(res);
    }

    destroy(id) {
        if (!verifyUint32Array([id])) {
            throw new ExtremeQTInstanceError("The ID parameter must be present and a uint32 number");
        }
        if (id === NULL_ID) {
            throw new ExtremeQTInstanceError(`The ID parameter can't be a null ID (${NULL_ID})`);
        }

        const res = addon.destroy(this.#ctx, id);
        return dealResponse(res);
    }

    move(id, x, y) {
        if (!verifyUint32Array([id, x, y])) {
            throw new ExtremeQTInstanceError("ID, x, and y parameters must be present and uint32 numbers");
        }
        if (x > this.width || y > this.height) {
            throw new ExtremeQTInstanceError("Cannot move an entity outside the QuadTree");
        }
        if (id === NULL_ID) {
            throw new ExtremeQTInstanceError(`The id parameter can't be a null ID (${NULL_ID})`);
        }

        const res = addon.move(this.#ctx, id, x, y);
        return dealResponse(res);
    }

    get(x, y, width, height) {
        if (!verifyUint32Array([x, y, width, height])) {
            throw new ExtremeQTInstanceError("x, y, width, and height parameters must be present and uint32 numbers");
        }

        if (width === 0 || height === 0) {
            return new Float32Array(0);
        }

        if (x + width > this.width || y + height > this.height) {
            throw new ExtremeQTInstanceError("Cannot get entities outside the QuadTree");
        }

        return addon.get(this.#ctx, x, y, width, height);
    }

    deallocate() {
        return addon.deallocate(this.#ctx);
    }
}

module.exports = {
    Instance,
    ResponseStatues,
};
