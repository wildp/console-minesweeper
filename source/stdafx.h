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
#include <tchar.h>
#include <windows.h> ///win
#include <cwchar>
#include <wchar.h>
#endif

#ifndef USE_WIN
#ifdef EXTRA_CONSOLE_STUFF
#undef EXTRA_CONSOLE_STUFF
#endif
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
