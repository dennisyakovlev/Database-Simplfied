// Author: Dennis Yakovlev

#pragma once
#include <chrono>
#include <iterator>
#include <link_utils.h>
#include <link_vars.h>
#include <node.h>
#include <v8.h>

start_link

void _get_edges_info(_v8 Isolate* isolate, _v8 Local<_v8 Context> context, const _prob coord_ty& coord, _v8 Local<_v8 Object> obj) {

    auto res = _link _read_map(coord);

    _v8 Local<_v8 String> edge_str = _v8 String::NewFromUtf8Literal(isolate, "edges");
    _v8 Local<_v8 Array> edge_arr = _v8 Array::New(isolate, res.second.size() - 1);
    auto iter_res = res.second.cbegin();
    ++iter_res;
    for (; iter_res != res.second.cend(); ++iter_res) {
        edge_arr->Set(context, _std distance(res.second.cbegin(), iter_res) - 1, _v8 Number::New(isolate, *iter_res));
    }

    obj.As<_v8 Object>()->Set(context, edge_str, edge_arr);

}

void get_edges_info(const _v8 FunctionCallbackInfo<_v8 Value>& args) {

    _v8 Isolate* isolate = args.GetIsolate();
    _v8 Local<_v8 Context> context = _v8 Context::New(isolate);

    _v8 Local<_v8 Object> obj_ret = _v8 Object::New(isolate); // object to return
    _v8 Local<_v8 Object> obj_in = args[0].As<_v8 Object>(); // object from args

    auto res = _link _get_obj_coords(isolate, context, obj_in, "x", "y");
    _link _get_edges_info(isolate, context, res, obj_ret);

    args.GetReturnValue().Set(obj_ret);

}

end_link