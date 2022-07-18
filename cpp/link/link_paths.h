// Author: Dennis Yakovlev

#pragma once
#include <chrono>
#include <link_utils.h>
#include <link_vars.h>
#include <node.h>
#include <string>
#include <v8.h>

start_link

void _get_paths_info( _v8 Isolate* isolate, _v8 Local<_v8 Context> context, const _prob coord_ty& coord, _v8 Local<_v8 Object> obj) {

    auto res = _link _read_map(coord);

    _v8 Local<_v8 String> path_str = _v8 String::NewFromUtf8Literal(isolate, "paths");
    auto num_10 = BigUnsigned_10(res.first);
    _std string num_str_10;
    for (auto i : num_10) {
        num_str_10.push_back('0' + i);
    }
    _v8 Local<_v8 String> path_str_val;
    _v8 String::NewFromUtf8(isolate, num_str_10.c_str()).ToLocal(&path_str_val);

    obj.As<_v8 Object>()->Set(context, path_str, path_str_val);

}

void get_paths_info(const _v8 FunctionCallbackInfo<_v8 Value>& args) {

    _v8 Isolate* isolate = args.GetIsolate();
    _v8 Local<_v8 Context> context = _v8 Context::New(isolate);

    _v8 Local<_v8 Object> obj_ret = _v8 Object::New(isolate); // object to return
    _v8 Local<_v8 Object> obj_in = args[0].As<_v8 Object>(); // object from args

    auto res = _link _get_obj_coords(isolate, context, obj_in, "x", "y");
    _link _get_paths_info(isolate, context, res, obj_ret);

    args.GetReturnValue().Set(obj_ret);

}

end_link