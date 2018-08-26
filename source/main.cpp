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

#include "stdafx.h"
#include "docs.h"

enum Gamemode
{
	MODE_TEST_INVERT       = -5,
	MODE_TEST_COLOURS      = -4,
	MODE_SHOW_LICENCE      = -3,
	MODE_SHOW_HELP         = -2,
	MODE_SHOW_CREDITS      = -1,
	MODE_BEGINNER          =  0,
	MODE_NOVICE            =  1,
	MODE_INTERMEDIATE      =  2,
	MODE_ADVANCED          =  3,
	MODE_EXPERT            =  4,
	MODE_MASTER            =  5,
	MODE_GRANDMASTER       =  6,
	MODE_DEBUG             =  7,
	MODE_CUSTOM            =  8,
	MODE_ERROR             =  9,	
	MODE_EXIT              = 10,
};


struct Coordinate
{
	int col = -1;
	int row = -1;
};


struct Flag
{
	Coordinate c = { -1, -1 };
	char f = ' ';
};


namespace Directions
{
	extern const Coordinate dir[]{ {  0, -1 } , {  1, -1 } ,  // N, NE
								   {  1,  0 } , {  1,  1 } ,  // E, SE
								   {  0,  1 } , { -1,  1 } ,  // S, SW
								   { -1,  0 } , { -1, -1 } }; // W, NW
}


namespace GamemodePresets
{
	extern const Coordinate size[]{ {9,9}, {8,8}, {16,16}, {24,24}, {30,16}, {30,24}, {36,36}, {5,5} , {-1,-1} };
	extern const int mines[]{ 10, 10, 40, 99, 99, 200, 648, 1, 0 };
	}


namespace TextOutput
{
#ifdef USE_WIN
	extern const HANDLE handleConsole{ GetStdHandle(STD_OUTPUT_HANDLE) };
#endif
#ifdef USE_ESC
	extern bool negative{ false };
#endif

	void setup(short fontSize = 10) 
	{
#ifdef USE_WIN
		SetConsoleTitle(_T("Console Minesweeper")); 

	#ifdef EXTRA_CONSOLE_STUFF
		CONSOLE_FONT_INFOEX font =
		{
			font.cbSize = sizeof(font),
			font.nFont = 0,
			font.dwFontSize.X = 0,
			font.dwFontSize.Y = fontSize,
			font.FontFamily = FF_DONTCARE,
			font.FontWeight = FW_NORMAL,
		};

		wcscpy_s(font.FaceName, L"Consolas");

		const COORD c{ 120, 48 };

		bool fail{ true };

		while (fail)
		{
			SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &font);

			COORD maxSize = GetLargestConsoleWindowSize(handleConsole);
			if ((c.X > maxSize.X) || (c.Y > maxSize.Y))
			{
				fail = false;
				font.dwFontSize.Y--;
				SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &font);
			}
			else
				font.dwFontSize.Y++;
		}

		CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
		GetConsoleScreenBufferInfo(handleConsole, &bufferInfo);

		SMALL_RECT &winInfo = bufferInfo.srWindow;
		COORD windowSize = { winInfo.Right - winInfo.Left + 1, winInfo.Bottom - winInfo.Top + 1 };

		SetConsoleScreenBufferSize(handleConsole, { c.X, 9001 });

		SMALL_RECT info = { 0, 0, c.X - 1, c.Y - 1 };
		SetConsoleWindowInfo(handleConsole, TRUE, &info);
	#endif // EXTRA_CONSOLE_STUFF
#endif // USE_WIN

#ifdef USE_ESC
	#ifdef USE_VT
		// Set output mode to handle virtual terminal sequences
		HANDLE Out = GetStdHandle(STD_OUTPUT_HANDLE);
		if (Out == INVALID_HANDLE_VALUE)
			exit(1);

		DWORD OriginalOutMode = 0;

		if (!GetConsoleMode(Out, &OriginalOutMode))
			exit(1);

		DWORD RequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;

		DWORD OutMode = OriginalOutMode | RequestedOutModes;
		if (!SetConsoleMode(Out, OutMode))
		{
			RequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			OutMode = OriginalOutMode | RequestedOutModes;
			if (!SetConsoleMode(Out, OutMode))
				exit(1);
		}
	#endif // USE_VT
		std::cout << OSC "0;Console Minesweeper" ST;
#endif // USE_ESC
		return;
	}

	void setColour(int option = -1)
	{	// colour 7 is default on windows console, 37 with ansi colour codes
#ifdef USE_WIN
		if (option == -1)
			option = DEFAULT_COLOUR;

		SetConsoleTextAttribute(handleConsole, option);
#endif // USE_WIN
#ifdef USE_ESC

		if (negative && option != -1)
			std::cout << CSI"7m";
		else
			std::cout << CSI"27m";

		if (option == -1)
			option = DEFAULT_COLOUR;

		std::string ostring{ CSI };
		ostring.append(std::to_string(option)).append("m");
		std::cout << ostring;
		
		
#endif // USE_ESC
	}

	void setColour(char c)
	{	// picks colour according to symbol
		int code{ 15 };
#ifdef USE_WIN
		switch (c)
		{
		case '1': code = 9;  break;
		case '2': code = 2;  break;
		case '3': code = 4;  break;
		case '4': code = 1;  break;
		case '5': code = 5;  break;
		case '6': code = 3;  break;
		case '7': code = 6;  break;
		case '8': code = 8;  break;
		case '#': code = 7;  break;
		case '!': code = 12; break;
		case '?': code = 15; break;
		case 'X': code = 12; break;
		case ':': code = 11; break;
		default:  code = DEFAULT_COLOUR;  break;
		}
#endif // USE_WIN
#ifdef USE_ESC
		switch (c)
		{
		case '1': code = 94;  break;
		case '2': code = 32;  break;
		case '3': code = 31;  break;
		case '4': code = 34;  break;
		case '5': code = 35;  break;
		case '6': code = 36;  break;
		case '7': code = 33;  break;
		case '8': code = 90;  break;
		case '#': code = 37;  break;
		case '!': code = 91; break;
		case '?': code = 97; break;
		case 'X': code = 91; break;
		case ':': code = 96; break;
		default:  code = DEFAULT_COLOUR;  break;
		}
#endif // USE_ESC
		setColour(code);
	}

#ifdef USE_ESC
	void toggleState()
	{
		negative = (negative) ? false : true;
	}
#endif

	void testColour()
	{
		char chars[]{ ' ','1','2','3','4','5','6','7','8','#','!','?','X' };
		for (auto &c : chars)
		{
			setColour(c);
			std::cout << c << ' ';
		}
		setColour();
	}
}

namespace TextDisplay
{
	void startup()
	{
		std::cout << "Console based Minesweeper" << "\nVersion " << Docs::version << '\n';
	}

	void lastupdate()
	{
		TextOutput::setColour((escape) ? 90 : 8);
		std::cout << "Last updated " << Docs::lastupdate << '\n';
		TextOutput::setColour();
	}

	void compiledate()
	{
		TextOutput::setColour((escape) ? 90 : 8);
		std::cout << "Compiled " << Docs::compiledate << '\n';
		TextOutput::setColour();
	}

	void showfill()
	{
		TextOutput::setColour((escape) ? 90 : 8);
		std::cout << '\n' << Docs::fill << '\n';
		TextOutput::setColour((escape) ? 37 : 7);
	}

	void showoptions()
	{
		TextOutput::setColour();
		std::cout << '\n' << Docs::helptext << '\n' << Docs::modes;
	}

	void showhelp()
	{
		
		showfill();
		std::cout << '\n' << Docs::helptitle << "\n\n";
		for (auto &line : Docs::help)
			std::cout << line << '\n';
		showfill();
		std::cout << '\n' << std::endl;
		lastupdate();
	}

	void showlicence()
	{
		showfill();
#ifndef SHORT_DOCS
		std::cout << '\n' << Docs::licencetitle << '\n';
		showfill();
#endif
		std::cout << '\n';
		for (auto &line : Docs::licence)
			std::cout << line << '\n';
		showfill();
		std::cout << '\n' << std::endl;
	}

	void showcredits()
	{
		showfill();
		std::cout << '\n';
		for (auto &line : Docs::credits)
			std::cout << line << '\n';
		showfill();
		std::cout << '\n' << std::endl;
		compiledate();
	}

}

namespace TextInput
{
	int decode(char input)
	{
		switch (input)
		{
		case '0':           return  0;
		case '1':           return  1;
		case '2':           return  2;
		case '3':           return  3;
		case '4':           return  4;
		case '5':           return  5;
		case '6':           return  6;
		case '7':           return  7;
		case '8':           return  8;
		case '9':           return  9;
		case 'A': case 'a': return 10;
		case 'B': case 'b': return 11;
		case 'C': case 'c': return 12;
		case 'D': case 'd': return 13;
		case 'E': case 'e': return 14;
		case 'F': case 'f': return 15;
		case 'G': case 'g': return 16;
		case 'H': case 'h': return 17;
		case 'I': case 'i': return 18;
		case 'J': case 'j': return 19;
		case 'K': case 'k': return 20;
		case 'L': case 'l': return 21;
		case 'M': case 'm': return 22;
		case 'N': case 'n': return 23;
		case 'O': case 'o': return 24;
		case 'P': case 'p': return 25;
		case 'Q': case 'q': return 26;
		case 'R': case 'r': return 27;
		case 'S': case 's': return 28;
		case 'T': case 't': return 29;
		case 'U': case 'u': return 30;
		case 'V': case 'v': return 31;
		case 'W': case 'w': return 32;
		case 'X': case 'x': return 33;
		case 'Y': case 'y': return 34;
		case 'Z': case 'z': return 35;
		default:  return -1;
		}
	}

	char encode(int input)
	{
		switch (input)
		{
		case  0: return '0';
		case  1: return '1';
		case  2: return '2';
		case  3: return '3';
		case  4: return '4';
		case  5: return '5';
		case  6: return '6';
		case  7: return '7';
		case  8: return '8';
		case  9: return '9';
		case 10: return 'A';
		case 11: return 'B';
		case 12: return 'C';
		case 13: return 'D';
		case 14: return 'E';
		case 15: return 'F';
		case 16: return 'G';
		case 17: return 'H';
		case 18: return 'I';
		case 19: return 'J';
		case 20: return 'K';
		case 21: return 'L';
		case 22: return 'M';
		case 23: return 'N';
		case 24: return 'O';
		case 25: return 'P';
		case 26: return 'Q';
		case 27: return 'R';
		case 28: return 'S';
		case 29: return 'T';
		case 30: return 'U';
		case 31: return 'V';
		case 32: return 'W';
		case 33: return 'X';
		case 34: return 'Y';
		case 35: return 'Z';
		default: return ' ';
		}
	}

	Flag grab(Coordinate size = { 10,10 }) //convert into flag
	{
		char input1{};
		char input2{};
		char input3{};
		Flag coord{ { -1,-1 }, ' ' };

		char col{};
		char row{};

		while (true)
		{
			coord.f = ' ';

			std::cout << "\nEnter coordinates of the tile: ";

			std::cin >> input1;
			std::cin >> input2;

			if (((static_cast<short>(input1) < 48) || (static_cast<short>(input1) > 57)) &&
				((static_cast<short>(input1) < 65) || (static_cast<short>(input1) > 90)) &&  
				((static_cast<short>(input1) < 94) || (static_cast<short>(input1) > 122)))
			{	// conversion in to ascii codes: numbers are 48 to 57, uppercase are 65 to 90, lowercase are 94 to 122
				std::cin >> input3;
				col = input2;
				row = input3;
				coord.f = input1;
			}
			else
			{
				col = input1;
				row = input2;
			}

			std::cin.ignore(32767, '\n');

			coord.c = { decode(col) ,decode(row) };

			if ((coord.f != '!') && (coord.f != '?') && (coord.f != '#') && (coord.f != '@') && (coord.f != '%') && (coord.f != ' '))
				std::cout << coord.f << " is an invalid flag\n";
			else if ((coord.c.row < 0) || (coord.c.row >= size.row) || (coord.c.col < 0) || (coord.c.col >= size.col))
				std::cout << col << row << " is an invalid coordinate\n";
			else break;
		}
		return coord;
	}

	Flag grab(int dimension) { return grab({ dimension, dimension }); }

	Coordinate setupWorld(int maxDimension = 35, int minDimension = 4)
	{
		char col{};
		char row{};
		Coordinate coord{ -1,-1 };
		while (true)
		{
			std::cout << "\nEnter size of grid: ";
			std::cin >> col;
			std::cin >> row;
			std::cin.ignore(32767, '\n');

			coord = { decode(col) ,decode(row) };

			if ((coord.row > maxDimension) || (coord.col > maxDimension))
				std::cout << "Grid " << col << row << " is too large (Maximum side length: " << encode(maxDimension) << ")\n";
			else if ((coord.row < minDimension) || (coord.col < minDimension))
				std::cout << "Grid " << col << row << " is too small (Minimum side length: " << encode(minDimension) << ")\n";
			else break;
		}
		return { coord.col + 1, coord.row + 1 };
	}

	int setupMines(Coordinate grid)
	{
		int maxMines{ ((grid.col * grid.row) -1)};
		int mines{};
		while (true)
		{
			std::cout << "\nEnter number of mines: ";
			std::cin >> mines;

			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(32767, '\n');
			}
			if ((mines <= 0) || (mines > maxMines))
				std::cout << "Invalid number of mines\n";
			else break;
		}
		return mines;
	}

	Gamemode modeDecode(char c)
	{
		switch (c)
		{
		case 'A': case 'a': case '3':
			return MODE_ADVANCED;
		case 'B': case 'b': case '0':
			return MODE_BEGINNER;
		case 'C': case 'c': case '7':
			return MODE_CUSTOM;
		case 'D': case 'd':
			return MODE_DEBUG;
		case 'E': case 'e': case '4':
			return MODE_EXPERT;
		case 'G': case 'g': case '6':
			return MODE_GRANDMASTER;
		case 'H': case 'h':
			return MODE_SHOW_HELP;
		case 'I': case 'i': case '2':
			return MODE_INTERMEDIATE;
		case 'L': case 'l':
			return MODE_SHOW_LICENCE;
		case 'M': case 'm': case '5':
			return MODE_MASTER;
		case 'N': case 'n': case '1':
			return MODE_NOVICE;
		case 'Z': case 'z':
			return MODE_SHOW_CREDITS;
		case 'X': case 'x':
			return MODE_EXIT;
		case 'T': case 't':
			return MODE_TEST_COLOURS;
		case 'V': case 'v':
			return MODE_TEST_INVERT;
		default:
			return MODE_ERROR;
		}
	}

	Gamemode modePreset()
	{
		char c{};
		Gamemode g{ MODE_ERROR };
		TextOutput::setColour();
		TextDisplay::showoptions();
		while (true)
		{
			std::cout << "\nSelect Option: ";
			std::cin >> c;
			std::cin.ignore(32767, '\n');
			g = modeDecode(c);
			if (g == MODE_ERROR)
				std::cout << "Invalid option\n";
			else break;
		}
		return g;
	}

	Coordinate determineSize(Gamemode mode = MODE_CUSTOM)
	{
		if (mode != MODE_CUSTOM)
			return GamemodePresets::size[mode];
		else return setupWorld();
	}

	int determineMines(Gamemode mode, Coordinate size)
	{
		if (mode != MODE_CUSTOM)
			return GamemodePresets::mines[mode];
		else return setupMines(size);
	}
}


class Tile
{
private:
	bool m_mine;
	int m_number;
	char m_symbol;

public:
	Tile(bool ingnoreThisBool = true) :
		m_mine   { false },
		m_number { 0 },
		m_symbol { '#' }
	{
	}

	void determineMine(bool mine) { m_mine = mine; }

	void determineNumbers(int number) { if (!m_mine) m_number = number; }

	void determineflag(char flag) 
	{
		if (((flag == '!') || (flag == '?') || (flag == '#')) && 
			((m_symbol == '!') || (m_symbol == '?') || (m_symbol == '#'))) 
			m_symbol = flag;
	}

	const bool uncover()
	{
		if (m_symbol != '!')
		{
			if (m_mine)
			{
				m_symbol = 'X';
				return true;
			}
			else
			{
				if (m_number > 0)
					m_symbol = static_cast<char>(m_number) + 48;
				else
					m_symbol = ' ';
				return false;
			}
		}
		return false;
	}

	void reveal()
	{
		if ((!m_mine) && (m_symbol == '!'))
			m_symbol = ':';
		else if ((m_mine) && (m_symbol != '!'))
				m_symbol = 'X';
		else if ((m_mine) && (m_symbol == '!'))
			m_symbol = '!';
	}

	const bool getMine()   { return m_mine;   }
	const char getSymbol() { return m_symbol; }
	const int  getNumber() { return m_number; }
};

class World		
{
private:
	Coordinate m_size;
	int m_length;
	int m_mines;
	std::vector<Tile> m_world;


	const int getIndex(Coordinate c)
	{	// returns the index of a coordinate
		if ((c.col < 0) || (c.col >= m_size.col) || (c.row < 0) || (c.row >= m_size.row))
			return -1;
		return (c.row * m_size.col) + c.col;
	}

	const int getIndex(int col, int row) { return getIndex({ col,row }); }

	const bool getMine(Coordinate c)
	{	// returns m_mine from
		if (coordVerifyBool(c))
			return m_world[getIndex(c)].getMine();
		else return false;
	}


	const int adjustIndex(int value)
	{	// modifies the index if the index is out of range
		if (value >= m_length) value -= m_length;
		else if (value < 0) value += m_length;
		if ((value >= m_length)||(value < 0)) value = adjustIndex(value);
		return value;
	}


	const Coordinate coordVerify(Coordinate c)
	{   // returns the coordinate if valid, else {-1,-1}
		if ((c.col < 0) || (c.col >= m_size.col) || (c.row < 0) || (c.row >= m_size.row))
			return { -1,-1 };
		else return c;
	}

	const Coordinate coordTranslate(Coordinate pos, Coordinate trans)
	{	// returns a coordinate if translated coordinate is valid, else {-1,-1}
		return coordVerify({ pos.col + trans.col, pos.row + trans.row });
	}

	const bool coordVerifyBool(Coordinate c)
	{	// returns true if the coordinate is valid, else false
		Coordinate d{ coordVerify(c) };
		if ((c.col == d.col) && (c.row == d.row) && (c.col != -1) && (c.row != -1))
			return true;
		else return false;
	}


	enum genTypes
	{
		GEN_RANDOM_INTERVAL_UNIFORM,

		MAX_GENTYPES, // Maximum number of genTypes
		GEN_NONE,    // No genType
	};

	void gen(Coordinate seed, genTypes type = GEN_NONE)
	{
		switch (type)
		{
		case GEN_RANDOM_INTERVAL_UNIFORM: return genRandomIntervalUniform(seed);
		}
	}

	void genRandomIntervalUniform(Coordinate seed)
	{   // generation code for random intervals
		// probably not the most effective method

		// set up PRNG, and distribution
		static std::random_device rd;
		static std::mt19937_64 mt{ rd() };
		std::uniform_int_distribution<int> dist(0, m_length);

		// mark seed as a mine (unmark later) 
		m_world[getIndex(seed)].determineMine(true);
		int position = 0;
		for (int count{ 0 }; count < m_mines; ++count)
		{	// increases count per iteration
			position = adjustIndex(position + dist(mt));
			if (m_world[position].getMine() == true)
				--count; 
				// reject that position and decrease count
			else
				m_world[position].determineMine(true);
				// mark that position as a mine
		}
		// unmark seed as a mine
		m_world[getIndex(seed)].determineMine(false);
	}


	void setNumbers()
	{	// sets the counter for number of adjacent mines per tile
		for (int row{ 0 }; row < m_size.row; ++row)
			for (int col{ 0 }; col < m_size.col; ++col)
			{
				Coordinate c{ col, row };
				int mineCount{ 0 };

				for (int direction{ 0 }; direction < 8; direction++)
					mineCount += getMine(coordTranslate(c, Directions::dir[direction]));

				m_world[getIndex(c)].determineNumbers(mineCount);
			}
	}


	void chainuncover(Coordinate seed)
	{   // uncovers tiles from a starting coordinate
		std::vector<Coordinate> stack{seed};
		static Coordinate currentCoord;
		static int currentIndex;
		static char currentSymbl;
		stack.reserve(100);
		while (stack.size() != 0)
		{
			if (coordVerifyBool(stack.back()))
			{
				currentCoord = stack.back();
				currentIndex = getIndex(currentCoord);
				currentSymbl = m_world[currentIndex].getSymbol();
				stack.pop_back();

				if ((currentSymbl  == '#') || (currentSymbl == '?'))
				{	
					if (m_world[currentIndex].uncover()) return;

					if (m_world[currentIndex].getNumber() == 0)
					{
						Coordinate currentNewCoord{ -1,-1 };
						for (int direction{ 0 }; direction < 8; direction++)
						{	// adds all adjacent coordinates to the stack (if valid)
							currentNewCoord = coordTranslate(currentCoord, Directions::dir[direction]);
							if (coordVerifyBool(currentNewCoord))
								stack.push_back(currentNewCoord);
						}
					}
				}
			}
			else stack.pop_back();
		}
	}


	void displayBasic()
	{	// displays grid of mines
		for (int row{ 0 }; row < m_size.row; ++row)
		{
			for (int col{ 0 }; col < m_size.col; ++col)
				std::cout << ' ' << m_world[getIndex(col, row)].getSymbol();
			std::cout << '\n';
		}
	}

	void displayGuides()
	{	// displays grid of mines with axes
		std::cout << "\n #  ";
		for (int header{ 0 }; header < m_size.col; ++header)
			std::cout << ' ' << TextInput::encode(header);
		std::cout << "\n\n";
		for (int row{ 0 }; row < m_size.row; ++row)
		{
			std::cout << ' ' << TextInput::encode(row) << "  ";
			for (int col{ 0 }; col < m_size.col; ++col)
				std::cout << ' ' << m_world[getIndex(col, row)].getSymbol();
			std::cout << '\n';
		}
	}

	void displayGuidesColour()
	{	// displays grid of mines with axes in colour
		TextOutput::setColour();
		std::cout << "\n #  ";
		for (int header{ 0 }; header < m_size.col; ++header)
			std::cout << ' ' << TextInput::encode(header);
		std::cout << "\n\n";
		for (int row{ 0 }; row < m_size.row; ++row)
		{
			TextOutput::setColour();
			std::cout << ' ' << TextInput::encode(row) << "  ";
			for (int col{ 0 }; col < m_size.col; ++col)
			{
				char symbol = m_world[getIndex(col, row)].getSymbol();
				TextOutput::setColour(symbol);
				std::cout << ' ' << symbol;
			}
			std::cout << '\n';
		}
		TextOutput::setColour();
	}

	void displayMine()
	{	// displays grid with a bool if a mine is present
		for (int row{ 0 }; row < m_size.row; ++row)
		{
			for (int col{ 0 }; col < m_size.col; ++col)
				std::cout << ' ' << m_world[getIndex(col, row)].getMine();
			std::cout << '\n';
		}
	}


	void showMineCount()
	{
		int count{ m_mines };
		for (auto &mine : m_world)
			if (mine.getSymbol() == '!')
				count--;
		if (count < 0) count = 0;
		TextOutput::setColour((escape) ? 90 : 8);
		std::cout << "\nMines left: " << count << '\n';
		TextOutput::setColour();
	}


public:
	World(Coordinate size = { 10,10 }, int maxMines = 10) : //default
		m_size   { size.col, size.row },
		m_length { size.col * size.row },
		m_mines  { maxMines }
	{
		m_world.reserve(m_length);
		populate();
	}

	World(int dimension, int mines = 10) { World({ dimension, dimension }, mines); }

	void populate()
	{
		for (int count{ 0 }; count < (m_length); ++count)
			m_world.push_back(new Tile);
	}

	void generate(Coordinate seed)
	{
		gen(seed, GEN_RANDOM_INTERVAL_UNIFORM);
		setNumbers();
	}
	
	void display(bool showCounter=true) 
	{
		if (showCounter) showMineCount();
		displayGuidesColour();
	}

	bool uncover(Coordinate c)
	{
		chainuncover(c);
		if (m_world[getIndex(c)].getSymbol() == '!')
			return false;
		else
			return m_world[getIndex(c)].getMine();
	}

	void uncover()
	{
		for (auto &mine : m_world)
			if ((mine.getMine()) || (mine.getSymbol() == '!'))
				mine.reveal();
	}

	bool uncovermost()
	{
		for (auto &mine : m_world)
			if (mine.getSymbol() == '#')
				if (mine.uncover()) return true;
		return false;
	}

	void uncoverall()
	{
		for (auto &mine : m_world)
			mine.uncover();
	}


	bool flag(Flag coord)
	{
		if (coord.f == '%')
			return uncovermost();
		else if (coord.f == '@')
		{
			uncoverall();
			return true;
		}
		else
			m_world[getIndex(coord.c)].determineflag(coord.f);
		return false;
	}

	bool checkwin()
	{
		for (int row{ 0 }; row < m_size.row; ++row)
			for (int col{ 0 }; col < m_size.col; ++col)
			{
				char symbol{ m_world[getIndex(col, row)].getSymbol() };
				if ((symbol == '#') || (symbol == '?') || (symbol == '!'))
				{
					if (m_world[getIndex(col, row)].getMine() == 0)
						return false;
				}
				else if (symbol == 'X') return false; // hard coded exit
			}
		return true;
	}

	~World()
	{
		for (int count{ 0 }; count < (m_length); ++count)
			m_world.pop_back();
	}

};


bool replay(bool autoreplay=false)
{
	if (autoreplay) return true;

	char r{};
	while (true)
	{
		std::cout << "\nWould you like to start a new game (y/n)? ";
		std::cin >> r;
		std::cout << '\n';

		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(32767, '\n');
		}
		else if (r == 'y')
		{
			std::cout << "\nNew Game:\n";
			return true;
		}
		else if (r == 'n')
			return false;
		else
			std::cin.ignore(32767, '\n');
	}
}

bool win(World &w)
{
	if (w.checkwin())
	{
		TextOutput::setColour((escape) ? 92 : 10);
		std::cout << "\nYou have won\n";
		TextOutput::setColour();
		return true;
	}
	return false;
}

int gameText(Gamemode preset)
{
	// setup
	Coordinate worldSize{ TextInput::determineSize(preset) };
	World world{ worldSize, TextInput::determineMines(preset, worldSize) };
	world.display();
	
	bool lose{ false };
	Flag selection{};
	
	// takes an input coordinate and lays mines accordingly 
	selection = TextInput::grab(worldSize);
	world.generate(selection.c);
	world.uncover(selection.c);
	world.display();
	if (!(win(world)))
	{
		do
		{
			selection = TextInput::grab(worldSize);

			if (selection.f != ' ')
				lose = world.flag(selection);
			else
				lose = world.uncover(selection.c);

			world.display();
			if (win(world)) break;
		} while (!lose);
	}

	if (lose)
	{
		TextOutput::setColour((escape) ? 91 : 12);
		std::cout << "\nYou have died\n";
		TextOutput::setColour();
	}

	world.uncover();
	world.display(false);
	return 0;
}


int main()
{
	TextOutput::setup();
	TextDisplay::startup();
	bool autoreplay{ false };
	do
	{
		static Gamemode p;
		p = TextInput::modePreset();

		switch (p)
		{
		case MODE_ERROR:
			autoreplay = true;
			break;
		case MODE_EXIT:
			exit(0);
		case MODE_SHOW_HELP:
			TextDisplay::showhelp();
			autoreplay = true;
			break;
		case MODE_SHOW_LICENCE:
			TextDisplay::showlicence();
			autoreplay = true;
			break;
		case MODE_SHOW_CREDITS:
			TextDisplay::showcredits();
			autoreplay = true;
			break;
		case MODE_TEST_COLOURS:
			std::cout << '\n';
			TextOutput::testColour();
			autoreplay = true;
			break;
		case MODE_TEST_INVERT:
#ifdef USE_ESC
			TextOutput::toggleState();
#endif
			autoreplay = true;
			break;
		default:
			gameText(p);
			autoreplay = false;
			break;
		}

		std::cout << '\n';
	} while (replay(autoreplay));
	return 0;
}
