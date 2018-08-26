// Copyright 2018 Peter Wild
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef STDAFX_H
#define STDAFX_H

#if (PLATFORM == 0)
	// select colour implementation: 
	// 1 none
	// 2 windows api
	// 3 option 1 with window resizing 
	// 4 ansi escape codes
	// 5 option 3 with windows virtual terminal sequences
	#ifdef _WIN32
		#define PLATFORM 3
	#else 
		#define PLATFORM 4
	#endif
#endif

#if (PLATFORM == 1)
	#define LIGHT_MODE
	#define DEFAULT_COLOUR 0
#elif (PLATFORM == 2)
	#define USE_WIN
	#define DEFAULT_COLOUR 7
#elif (PLATFORM == 3)
	#define USE_WIN
	#define EXTRA_CONSOLE_STUFF
	#define DEFAULT_COLOUR 7
#elif (PLATFORM == 4)
	#define USE_ESC
	#define ESC "\x1b"
	#define CSI ESC"["
	#define OSC ESC"]"
	#define BEL "\x07"
	#define ST  ESC"\\"
	#define DEFAULT_COLOUR 37
#elif (PLATFORM == 5)
	#define USE_ESC
	#define USE_VT
	#define ESC "\x1b"
	#define CSI ESC"["
	#define OSC ESC"]"
	#define BEL "\x07" 
	#define ST  ESC"\\"
	#define DEFAULT_COLOUR 37
#else
	#error PLATFORM must be between 0 and 5
#endif

// header files referenced in main 

#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#ifdef USE_VT
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h> 
#endif

#ifdef USE_WIN
	#define WIN32_LEAN_AND_MEAN
	#include <tchar.h>
	#include <windows.h>
	#include <cwchar>
	#include <wchar.h>
#endif

#ifndef USE_WIN
	#ifdef EXTRA_CONSOLE_STUFF
		#undef EXTRA_CONSOLE_STUFF
	#endif
#endif

#ifndef DEFAULT_COLOUR
	#define DEFAULT_COLOUR 0
#endif

#ifdef USE_ESC
constexpr bool escape{ true };
#else 
constexpr bool escape{ false };
#endif

/** Notes:
*
* BEGINNER
* 9 x 9 Grid (Size 88)
* 10 Mines
* Density = 0.123
* NOVICE
* 8 x 8 Grid (Size 77)
* 10 Mines
* Density = 0.156
* INTERMEDIATE
* 16 x 16 Grid (Size FF)
* 40 Mines
* Density = 0.156
* ADVANCED
* 24 x 24 Grid (Size MM)
* 99 Mines
* Density = 0.172
* EXPERT
* 30 x 16 Grid (Size TF)
* 99 Mines
* Density = 0.206
* MASTER
* 30 x 24 Grid (Size TM)
* 200 Mines
* Density = 0.278
* GRANDMASTER
* 36 x 36 Grid (Size ZZ)
* 648 Mines
* Density = 0.500
* CUSTOM
*
*/

#endif // !STDAFX_H
