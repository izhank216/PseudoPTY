#include <node.h>
#include <v8.h>

extern "C" {
    #include "PseudoPTY_NodeWrapper.c"
}

using namespace v8;

class PtyWrapper {
public:
    WindowsPty* pty;

    PtyWrapper() : pty(nullptr) {}

    static void New(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        if (!args.IsConstructCall()) {
            isolate->ThrowException(String::NewFromUtf8Literal(isolate, "Must use new"));
            return;
        }

        PtyWrapper* obj = new PtyWrapper();
        if (args.Length() >= 3) {
            v8::String::Utf8Value cmd(isolate, args[0]);
            int cols = args[1]->Int32Value(isolate->GetCurrentContext()).FromMaybe(80);
            int rows = args[2]->Int32Value(isolate->GetCurrentContext()).FromMaybe(25);
            obj->pty = PseudoPTY_Create(*cmd, cols, rows);
        }

        args.This()->SetAlignedPointerInInternalField(0, obj);
        args.GetReturnValue().Set(args.This());
    }

    static void Write(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        PtyWrapper* obj = static_cast<PtyWrapper*>(args.This()->GetAlignedPointerFromInternalField(0));
        if (!obj->pty) return;

        v8::String::Utf8Value data(isolate, args[0]);
        int len = args[1]->Int32Value(isolate->GetCurrentContext()).FromMaybe((int)strlen(*data));
        int written = PseudoPTY_Write(obj->pty, *data, len);

        args.GetReturnValue().Set(written);
    }

    static void Read(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        PtyWrapper* obj = static_cast<PtyWrapper*>(args.This()->GetAlignedPointerFromInternalField(0));
        if (!obj->pty) return;

        int len = args[0]->Int32Value(isolate->GetCurrentContext()).FromMaybe(512);
        char* buffer = new char[len];
        int n = PseudoPTY_Read(obj->pty, buffer, len);
        v8::Local<v8::String> result = String::NewFromUtf8(isolate, buffer, v8::NewStringType::kNormal, n).ToLocalChecked();
        delete[] buffer;
        args.GetReturnValue().Set(result);
    }

    static void Flush(const FunctionCallbackInfo<Value>& args) {
        PtyWrapper* obj = static_cast<PtyWrapper*>(args.This()->GetAlignedPointerFromInternalField(0));
        if (!obj->pty) return;
        PseudoPTY_Flush(obj->pty);
    }

    static void Interactive(const FunctionCallbackInfo<Value>& args) {
        PtyWrapper* obj = static_cast<PtyWrapper*>(args.This()->GetAlignedPointerFromInternalField(0));
        if (!obj->pty) return;
        PseudoPTY_Interactive(obj->pty);
    }

    static void Destroy(const FunctionCallbackInfo<Value>& args) {
        PtyWrapper* obj = static_cast<PtyWrapper*>(args.This()->GetAlignedPointerFromInternalField(0));
        if (!obj->pty) return;
        PseudoPTY_Destroy(obj->pty);
        obj->pty = nullptr;
    }

    static void Init(Local<Object> exports) {
        Isolate* isolate = exports->GetIsolate();
        Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
        tpl->SetClassName(String::NewFromUtf8Literal(isolate, "PseudoPTY"));
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        NODE_SET_PROTOTYPE_METHOD(tpl, "write", Write);
        NODE_SET_PROTOTYPE_METHOD(tpl, "read", Read);
        NODE_SET_PROTOTYPE_METHOD(tpl, "flush", Flush);
        NODE_SET_PROTOTYPE_METHOD(tpl, "interactive", Interactive);
        NODE_SET_PROTOTYPE_METHOD(tpl, "destroy", Destroy);

        exports->Set(isolate->GetCurrentContext(), String::NewFromUtf8Literal(isolate, "PseudoPTY"), tpl->GetFunction(isolate->GetCurrentContext()).ToLocalChecked()).Check();
    }
};

NODE_MODULE(PseudoPTY, PtyWrapper::Init)
