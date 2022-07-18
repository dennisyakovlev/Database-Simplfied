// Author: Dennis Yakovlev

#pragma once
#include <iterator>
#include <link_utils.h>
#include <link_vars.h>
#include <node.h>
#include <v8.h>

start_link

void get_complete_info(const _v8 FunctionCallbackInfo<_v8 Value>& args) {

    _v8 Isolate* isolate = args.GetIsolate();
    _v8 Local<_v8 Context> context = _v8 Context::New(isolate);

    _v8 Local<_v8 Object> obj_ret = _v8 Object::New(isolate); // object to return
    _v8 Local<_v8 Object> obj_in = args[0].As<_v8 Object>(); // object from args

    auto coord = _link _get_obj_coords(isolate, context, obj_in, "x", "y"); // cordinate from input
    
    auto res = _link _read_map(coord); // result from database

    // create chance array
    _v8 Local<_v8 String> edge_str = _v8 String::NewFromUtf8Literal(isolate, "edges");
    _v8 Local<_v8 Array> edge_arr = _v8 Array::New(isolate, res.second.size() - 1);
    auto iter_res = res.second.cbegin();
    ++iter_res;
    for (; iter_res != res.second.cend(); ++iter_res) {
        edge_arr->Set(context, _std distance(res.second.cbegin(), iter_res) - 1, _v8 Number::New(isolate, *iter_res));
    }

    // create chance through string
    _v8 Local<_v8 String> path_str = _v8 String::NewFromUtf8Literal(isolate, "paths");
    auto num_10 = BigUnsigned_10(res.first);
    _std string num_str_10;
    for (auto i : num_10) {
        num_str_10.push_back('0' + i);
    }
    _v8 Local<_v8 String> path_str_val;
    _v8 String::NewFromUtf8(isolate, num_str_10.c_str()).ToLocal(&path_str_val);

    obj_ret.As<_v8 Object>()->Set(context, edge_str, edge_arr); // set edges to return
    obj_ret.As<_v8 Object>()->Set(context, path_str, path_str_val); // set paths to return

    args.GetReturnValue().Set(obj_ret); // return

}

end_link