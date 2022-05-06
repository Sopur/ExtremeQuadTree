#include "def.h"
#include "tree.cpp"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <napi.h>
#include <string.h>

#define getCtxFromParamter() ExtTree* ctx = (ExtTree*) info[0].As<Napi::Number>().Int64Value();

Napi::Number FUNCTION_NEW(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    uint32_t width = info[0].As<Napi::Number>().Uint32Value();
    uint32_t height = info[1].As<Napi::Number>().Uint32Value();

    ExtTree* tree = new ExtTree(width, height);

    return Napi::Number::New(env, uintptr_t(tree));
}

Napi::Number FUNCTION_CREATE(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    getCtxFromParamter();
    uint32_t id = info[1].As<Napi::Number>().Uint32Value();
    uint32_t x = info[2].As<Napi::Number>().Uint32Value();
    uint32_t y = info[3].As<Napi::Number>().Uint32Value();

    return Napi::Number::New(env, int(ctx->create(id, {x, y})));
}

Napi::Number FUNCTION_DESTROY(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    getCtxFromParamter();
    uint32_t id = info[1].As<Napi::Number>().Uint32Value();

    return Napi::Number::New(env, int(ctx->destory(id)));
}

Napi::Number FUNCTION_MOVE(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    getCtxFromParamter();
    uint32_t id = info[1].As<Napi::Number>().Uint32Value();
    uint32_t x = info[2].As<Napi::Number>().Uint32Value();
    uint32_t y = info[3].As<Napi::Number>().Uint32Value();

    return Napi::Number::New(env, int(ctx->move(id, {x, y})));
}

Napi::Uint32Array FUNCTION_GET(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    getCtxFromParamter();
    uint32_t x = info[1].As<Napi::Number>().Uint32Value();
    uint32_t y = info[2].As<Napi::Number>().Uint32Value();
    uint32_t width = info[3].As<Napi::Number>().Uint32Value();
    uint32_t height = info[4].As<Napi::Number>().Uint32Value();
    ExtTreeCapture* captured = ctx->get(x, y, width, height);
    size_t size = captured->size();

    auto array = Napi::Uint32Array::New(env, size);
    memcpy(&array[0], &captured->operator[](0), size * sizeof(ExtTreeID));
    delete captured;

    return array;
}

Napi::Number FUNCTION_DEALLOCATE(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    getCtxFromParamter();
    delete ctx;
    return Napi::Number::New(env, int(ExtTreeRes::SUCCESS));
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(
        Napi::String::New(env, "New"),
        Napi::Function::New(env, FUNCTION_NEW));

    exports.Set(
        Napi::String::New(env, "create"),
        Napi::Function::New(env, FUNCTION_CREATE));

    exports.Set(
        Napi::String::New(env, "destroy"),
        Napi::Function::New(env, FUNCTION_DESTROY));

    exports.Set(
        Napi::String::New(env, "move"),
        Napi::Function::New(env, FUNCTION_MOVE));

    exports.Set(
        Napi::String::New(env, "get"),
        Napi::Function::New(env, FUNCTION_GET));

    exports.Set(
        Napi::String::New(env, "deallocate"),
        Napi::Function::New(env, FUNCTION_DEALLOCATE));
    return exports;
}

NODE_API_MODULE(NULL, Init)