// Author: Dennis Yakovlev

#pragma once
#include <prob_vars.h>
#include <v8.h>

#define namespace_link link

#define start_link namespace namespace_link {
#define end_link }

#define _v8 v8::

#define _link namespace_link::

#ifndef _prob
#define _prob namespace pathprob
#endif

#ifndef _std
#define _std namespace std
#endif

start_link

// All below are related to ThreadManager

static int __num = 0;
int* ptr_run_edge = &__num;
int* ptr_run_path = &__num;

int* ptr_done_edge = &__num;
int* ptr_done_path = &__num;

const _v8 FunctionCallbackInfo<_v8 Value>* ptr_args_edge;
const _v8 FunctionCallbackInfo<_v8 Value>* ptr_args_path;

_v8 Isolate* isolate_edge;

bool init_edge = true;
bool init_path = true;

end_link