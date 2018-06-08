# Minesweeper Help
The following text has been copied verbatim from the help option of the menu.

    Minesweeper Help

    Minesweeper is a single player video game which has its origins in the mainframe
    games of the 1960s and 1970s. The aim of minesweeper is to clear a rectangular
    board containing mines without detonating any of them, with the help of squares
    adjacent to mines, which denote the number of neighbouring mines in a 3 by 3
    arrangement. Squares are uncovered one at a time, with the coordinates of the
    square being entered in base-36 (numbers 0-9, followed by letters A-Z) with the
    format (X,Y). If an uncovered square has zero adjacent mines, the square will be
    blank and all adjacent squares will be recursively revealed. The player loses if
    they uncover a square with a mine. When the game set up the board, a mine will
    never be placed on the first square uncovered. Flags can also be used to help
    with gameplay; they can be used by adding a symbol before the coordinate. Flags
    cannot be used on the first turn. The current flags in the game are listed as
    follows:
    
        !   Mark an uncovered square as containing a mine. The square can no longer
            be uncovered and the mines remaining counter will be decreased by 1.
            If at the end a marked square is revealed to have been incorrectly
            placed, it is replaced on the grid with the incorrect mark marker (:).

        ?   Mark an uncovered square with a question mark. This serves as an aid to
            logical deduction, but does not prevent the square from being uncovered.

        #   Return a previously marked square with an exclamation mark or question
            mark to its unmarked state.

        %   Uncover all unmarked tiles. This is a quick shortcut to use instead of
            manually uncovering all of the remaining tiles. This flag should be used
            only when you are certain that you have marked all squares with mines,
            otherwise it may result in a loss. The coordinate given is redundant.

       @   Surrender and reveal all squares. The coordinate given is redundant.



    At the start of the game, the player is presented with presets for mine boards.
    Here is an explaination of the options:

        b    Beginner: a 9 by 9 grid with 10 mines. This is the 'beginner' mode from
             Microsoft Minesweeper from Windows 2000 onwards.

        n    Novice: an 8 by 8 grid with 10 mines. This was the original 'beginner'
             mode from Microsoft Win Mine, when it was first made in July 1990.

        i    Intermediate: a 16 by 16 grid with 40 mines, which has been in the game
             as 'intermediate' since Microsoft Minesweeper was first introduced.

        a    Advanced: a 24 by 24 grid with 99 mines. This was the original 'expert'
             mode in Win Mine, which was replaced by a 16 by 30 grid when Microsoft
             published Minesweeper as part of the Windows Entertainment Pack (1990).

        e    Expert: a 16 by 30 grid with 99 mines. This mode appears in all of the
             published versions of Minesweeper (not including Win Mine).

        m    Master: a 24 by 30 grid with 200 mines. 24 by 30 is the maximum size of
             a custom board in Microsoft Minesweeper.

        g    Grandmaster: a 36 by 36 grid with 50% mine density (648 mines). This is
             the maximum grid size possible in this implementation of Minesweeper
             due to limitations in the current input method and the number of
             characters in base-36. This is mode is extremely difficult. Good Luck.

        d    Debug: a 5 by 5 grid with 1 mine for testing purposes.

        c    Custom: see below:

    Custom mode allows for a maximum board size of 36 by 36, with a minimum side
    length of 5. The size of board is entered in base-36 coordinate (0-Z). The
    number of mines can also be determined by the user, with the minimum number of
    mines being 1 and the maximum being the number of squares in the board - 1.
    
    
    Here is a good website for minesweeper strategy:
    http://www.minesweeper.info/wiki/Strategy
 
