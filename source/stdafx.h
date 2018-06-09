#pragma once

//#include "targetver.h"
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>

// enable or disable windows.h
//#define LIGHT_MODE

#ifndef LIGHT_MODE
#define USE_WIN
#define EXTRA_CONSOLE_STUFF
#endif

// header files referenced in main 

#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#ifdef USE_WIN
#include <windows.h>
#include <cwchar>
#include <wchar.h>
#endif

#ifndef USE_WIN
#ifdef EXTRA_CONSOLE_STUFF
#undef EXTRA_CONSOLE_STUFF
#endif
#endif
