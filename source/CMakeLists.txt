cmake_minimum_required (VERSION 3.7)
project (Console_Minesweeper_v1.1)

set(platform 0)
# Select colour implementation: 
# 0: automatic
# 1: none (use 0 or any non-number)
# 2: windows api
# 3: option 1 with window resizing
# 4: ansi escape codes
# 5: option 3 with windows virtual terminal sequences

# Options 2,3, and 5 are exclusively for windows, while
#   option 4 does not work on windows. Option 1 has no
#   colour but shouldn't be platform specific.

set(shortdocs 0)
#if defined as 1 will reduce the size of docs 

add_executable(${PROJECT_NAME} main.cpp)

if ((platform GREATER 5) OR (platform LESS 1))
	target_compile_definitions(${PROJECT_NAME} PRIVATE SHORT_DOCS=${shortdocs})
else ((platform GREATER 5) OR (platform LESS 1))
	target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM=${platform} PRIVATE SHORT_DOCS=${shortdocs})
endif ((platform GREATER 5) OR (platform LESS 1))





