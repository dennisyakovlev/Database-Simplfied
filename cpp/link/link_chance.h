// Author: Dennis Yakovlev

#pragma once
#include <link_utils.h>
#include <link_vars.h>
#include <node.h>
#include <v8.h>

start_link

void calc_chance(const _v8 FunctionCallbackInfo<_v8 Value>& args) {

    _v8 Isolate* isolate = args.GetIsolate();
    _v8 Local<_v8 Context> context = _v8 Context::New(isolate);

    _v8 Local<_v8 Object> obj_ret = _v8 Object::New(isolate); // object to return
    _v8 Local<_v8 Object> obj_in = args[0].As<_v8 Object>(); // object from args

    auto coord_1 = _link _get_obj_coords(isolate, context, obj_in, "x1", "y1");
    auto coord_2 = _link _get_obj_coords(isolate, context, obj_in, "x2", "y2");

    auto res = _prob chance_path(coord_1, coord_2);
    _link _set_obj_arg_num(isolate, context, obj_ret, "chance", res);

    args.GetReturnValue().Set(obj_ret);

}

end_link