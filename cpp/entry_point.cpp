// Author: Dennis Yakovlev

// HEY UNDEFINE ME WHEN COMPILING WITH MSVS
// #define _WIN32_WINNT 0x0501

#include <node.h>
#include <v8.h>
#include <link_chance.h>
#include <link_edges.h>
#include <link_paths.h>
#include <link_vars.h>
#include <link_info.h>

void Initialize(_v8 Local<_v8 Object> exports) {
    
    NODE_SET_METHOD(exports, "request_edges", _link get_edges_info);
    NODE_SET_METHOD(exports, "request_paths", _link get_paths_info);
    NODE_SET_METHOD(exports, "calculate_chance", _link calc_chance);
    NODE_SET_METHOD(exports, "request_info", _link get_complete_info);

}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)