# Minesweeper
A text based minesweeper game for the windows command prompt.

## Screenshots

![Screenshot 1](/screenshots/v1.0%20Demo%201.png?raw=true "Screenshot")
![Screenshot 2](/screenshots/v1.0%20Demo%202.png?raw=true "Screenshot")
![Screenshot 3](/screenshots/v1.0%20Demo%203.png?raw=true "Screenshot")

## Instructions
See [HELP.md](HELP.md).

## Build Instructions

In [CMakeLists.txt](source/CMakeLists.txt), `PLATFORM` needs to be changed
to the desired option (on line 4).
```python
set(platform 0)
# Select colour implementation: 
# 0: automatic
# 1: none (use 0 or any non-number)
# 2: windows api
# 3: option 1 with window resizing
# 4: ansi escape codes
# 5: option 3 with windows virtual terminal sequences
```
Options 2, 3, and 5 are exclusively for windows, while option 4 does not work on windows (but should do
on other platforms). Option 1 has no colour but shouldn't be platform specific.
