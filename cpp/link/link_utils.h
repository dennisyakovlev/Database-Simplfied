// Author: Dennis Yakovlev

#pragma once
#include <link_vars.h>
#include <node.h>
#include <prob_file.h>
#include <prob_vars.h>
#include <string>
#include <utility>
#include <v8.h>

start_link

template<typename T>
_v8 Array _create_array_num(_v8 Isolate* isolate, _v8 Local<_v8 Context> context, T start, T end) {

    _v8 Local<_v8 Array> v8_arr = _v8 Array::New(isolate, _std distance(start, end));
    auto start_original = start;
    for (; start != end; ++start) {
        v8_arr->Set(context, _std distance(start_original, start), _v8 Number::New(isolate, *start));
    }

    return **v8_arr;

}

_v8 Local<_v8 Value> _get_obj_arg(_v8 Isolate* isolate, _v8 Local<_v8 Context> context, _v8 Local<_v8 Object> obj, _std string key_val) {
    // get value from object using <key_val>

    _v8 Local<_v8 String> key;
    _v8 String::NewFromUtf8(isolate, key_val.c_str()).ToLocal(&key);
    _v8 Local<_v8 Value> obj_get_res;
    obj->Get(context, key).ToLocal(&obj_get_res);

    return obj_get_res;

}

pathprob::coord_ty _get_obj_coords(_v8 Isolate* isolate, _v8 Local<_v8 Context> context, _v8 Local<_v8 Object> obj, _std string key_val_x, _std string key_val_y) {
    // get coordinate pair from object

    auto x_res = _get_obj_arg(isolate, context, obj, key_val_x);
    auto y_res = _get_obj_arg(isolate, context, obj, key_val_y);


    pathprob::coord_ty res;
    res.first = static_cast<int_least64_t>(x_res.As<_v8 Integer>()->Value());
    res.second = static_cast<int_least64_t>(y_res.As<_v8 Integer>()->Value());

    return res;

}

// template<typename T>
// void _set_obj_arg_arr(_v8 Isolate* isolate, _v8 Local<_v8 Context> context, _v8 Local<_v8 Object> obj, _std string key_val, T start, T end) {

//     _v8 Local<_v8 String> key;
//     _v8 String::NewFromUtf8(isolate, key_val.c_str()).Tov8::Local(&key);

//     // _v8 Local<_v8 Array> v8_arr = Array::New(isolate, _std distance(start, end));
//     // auto start_original = start;
//     // for (; start != end; ++start) {
//     //     v8_arr->Set(context, _std distance(start_original, start), _v8 Number::New(isolate, *start));
//     // }

//     auto v8_arr = _link _create_array_num(isolate, context, start, end);

//     obj.As<_v8 Object>()->Set(context, key, v8_arr);

// }

void _set_obj_arg_str(_v8 Isolate* isolate, _v8 Local<_v8 Context> context, _v8 Local<_v8 Object> obj, _std string key_val, const _std string& str) {

    _v8 Local<_v8 String> key;
    _v8 String::NewFromUtf8(isolate, key_val.c_str()).ToLocal(&key);

    _v8 Local<_v8 String> val;
    _v8 String::NewFromUtf8(isolate, str.c_str()).ToLocal(&val);

    obj.As<_v8 Object>()->Set(context, key, val);

}

template<typename T>
void _set_obj_arg_num(_v8 Isolate* isolate, _v8 Local<_v8 Context> context, _v8 Local<_v8 Object> obj, _std string key_val, T num) {

    _v8 Local<_v8 String> key;
    _v8 String::NewFromUtf8(isolate, key_val.c_str()).ToLocal(&key);

    _v8 Local<_v8 Number> val = _v8 Number::New(isolate, num);

    obj.As<_v8 Object>()->Set(context, key, val);

}

auto _read_map(const pathprob::coord_ty& coord) {

    return pathprob::read_map("map.bin", "info.bin", coord, pathprob::DOUBLE{});

}

end_link