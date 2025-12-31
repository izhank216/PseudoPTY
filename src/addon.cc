#include <napi.h>
extern "C" {
#include "PseudoPTY_NodeWrapper.c"
}

using namespace Napi;

Value PseudoPTY_CreateWrapped(const CallbackInfo& info) {
    Env env = info.Env();
    std::string command = info[0].As<String>().Utf8Value();
    int cols = info[1].As<Number>().Int32Value();
    int rows = info[2].As<Number>().Int32Value();
    WindowsPty* pty = PseudoPTY_Create(command.c_str(), cols, rows);
    if (!pty) {
        Error::New(env, "Terminal does not exist").ThrowAsJavaScriptException();
        return env.Null();
    }
    return External<WindowsPty>::New(env, pty);
}

Value PseudoPTY_ReadWrapped(const CallbackInfo& info) {
    Env env = info.Env();
    WindowsPty* pty = info[0].As<External<WindowsPty>>().Data();
    if (!pty) {
        Error::New(env, "PseudoPTY not created").ThrowAsJavaScriptException();
        return env.Null();
    }
    int len = info[1].As<Number>().Int32Value();
    char* buffer = new char[len];
    int n = PseudoPTY_Read(pty, buffer, len);
    std::string result(buffer, n);
    delete[] buffer;
    return String::New(env, result);
}

Value PseudoPTY_WriteWrapped(const CallbackInfo& info) {
    Env env = info.Env();
    WindowsPty* pty = info[0].As<External<WindowsPty>>().Data();
    if (!pty) {
        Error::New(env, "PseudoPTY not created").ThrowAsJavaScriptException();
        return env.Null();
    }
    std::string data = info[1].As<String>().Utf8Value();
    int written = PseudoPTY_Write(pty, data.c_str(), data.size());
    return Number::New(env, written);
}

void PseudoPTY_DestroyWrapped(const CallbackInfo& info) {
    WindowsPty* pty = info[0].As<External<WindowsPty>>().Data();
    if (!pty) {
        Error::New(info.Env(), "PseudoPTY not created").ThrowAsJavaScriptException();
        return;
    }
    PseudoPTY_Destroy(pty);
}

void PseudoPTY_FlushWrapped(const CallbackInfo& info) {
    WindowsPty* pty = info[0].As<External<WindowsPty>>().Data();
    if (!pty) {
        Error::New(info.Env(), "PseudoPTY not created").ThrowAsJavaScriptException();
        return;
    }
    PseudoPTY_Flush(pty);
}

void PseudoPTY_InteractiveWrapped(const CallbackInfo& info) {
    WindowsPty* pty = info[0].As<External<WindowsPty>>().Data();
    if (!pty) {
        Error::New(info.Env(), "PseudoPTY not created").ThrowAsJavaScriptException();
        return;
    }
    PseudoPTY_Interactive(pty);
}

Object Init(Env env, Object exports) {
    exports.Set("PseudoPTY_Create", Function::New(env, PseudoPTY_CreateWrapped));
    exports.Set("PseudoPTY_Read", Function::New(env, PseudoPTY_ReadWrapped));
    exports.Set("PseudoPTY_Write", Function::New(env, PseudoPTY_WriteWrapped));
    exports.Set("PseudoPTY_Destroy", Function::New(env, PseudoPTY_DestroyWrapped));
    exports.Set("PseudoPTY_Flush", Function::New(env, PseudoPTY_FlushWrapped));
    exports.Set("PseudoPTY_Interactive", Function::New(env, PseudoPTY_InteractiveWrapped));
    return exports;
}

NODE_API_MODULE(PseudoPTY, Init)
