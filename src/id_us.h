/*
BStone: A Source port of
Blake Stone: Aliens of Gold and Blake Stone: Planet Strike

Copyright (c) 1992-2013 Apogee Entertainment, LLC
Copyright (c) 2013-2015 Boris I. Bendovsky (bibendovsky@hotmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the
Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/


//
//      ID Engine
//      ID_US.h - Header file for the User Manager
//      v1.0d1
//      By Jason Blochowiak
//

#ifndef BSTONE_ID_US_INCLUDED
#define BSTONE_ID_US_INCLUDED


#include <cstdint>


#ifdef  __DEBUG__
#define __DEBUG_UserMgr__
#endif

// #define      HELPTEXTLINKED

#define MaxHelpLines 500

#define MaxHighName 57
#define MaxScores 10

struct HighScore {
    char name[MaxHighName + 1];
    int32_t score;
    uint16_t completed;
    uint16_t episode;
    uint16_t ratio;
}; // HighScore

#define MaxGameName 32
#define MaxSaveGames 6

struct SaveGame {
    char signature[4];
    uint16_t* oldtest;
    bool present;
    char name[MaxGameName + 1];
}; // SaveGame

#define MaxString 128 // Maximum input string size

// Record used to save & restore screen windows
struct WindowRec {
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
    int16_t px;
    int16_t py;
}; // WindowRec;

enum GameDiff {
    gd_Continue,
    gd_Easy,
    gd_Normal,
    gd_Hard
}; // GameDiff

// Custom Cursor struct type for US_LineInput()

// JAM - Custom Cursor Support
struct US_CursorStruct {
    char cursor_char;
    char do_not_use; // Space holder for ASCZ string
    uint16_t cursor_color;
    uint16_t font_number;
}; // US_CursorStruct

struct Point {
    int16_t x;
    int16_t y;
}; // Point

struct Rect {
    Point ul;
    Point lr;
}; // Rect

//      Hack import for TED launch support
// extern       bool         tedlevel;
// extern       uint16_t          tedlevelnum;
extern void TEDDeath();

extern bool ingame; // Set by game code if a game is in progress
extern bool abortgame; // Set if a game load failed
extern bool loadedgame; // Set if the current game was loaded
extern bool NoWait;
extern bool HighScoresDirty;
extern char* abortprogram; // Set to error msg if program is dying
extern GameDiff restartgame; // Normally gd_Continue, else starts game
extern int16_t PrintX;
extern int16_t PrintY; // Current printing location in the window
extern int16_t WindowX;
extern int16_t WindowY; // Current location of window
extern int16_t WindowW;
extern int16_t WindowH; // Current size of window

extern bool Button0;
extern bool Button1;
extern bool CursorBad;
extern int16_t CursorX;
extern int16_t CursorY;

extern void (* USL_MeasureString)(const char*, int*, int*);
extern void (*USL_DrawString)(const char*);

extern bool (* USL_SaveGame)(int16_t);
extern bool (* USL_LoadGame)(int16_t);
extern void (* USL_ResetGame)();
extern SaveGame Games[MaxSaveGames];

using HighScores = std::vector<HighScore>;

extern HighScores Scores;

extern US_CursorStruct US_CustomCursor; // JAM
extern bool use_custom_cursor; // JAM

#define US_HomeWindow() { PrintX = WindowX; PrintY = WindowY; }

void US_Startup();
void US_Setup();
void US_Shutdown();
void US_InitRndT(bool randomize);
void US_SetLoadSaveHooks(bool (* load)(int16_t),
                    bool (* save)(int16_t),
                    void (* reset)());
void US_TextScreen();
void US_UpdateTextScreen();
void US_FinishTextScreen();
void US_DrawWindow(const int x, const int y, const int w, const int h);
void US_CenterWindow(int w, int h);
void US_SaveWindow(WindowRec * win);
void US_RestoreWindow(WindowRec * win);
void US_ClearWindow();
void US_SetPrintRoutines(void (* measure)(char*, uint16_t*, uint16_t*),
                    void (* print)(char*));
void US_PrintCentered(const char* s);
void US_CPrint(const char* s);
void US_CPrintLine(const char* s);
void US_Print(const char* s);
void US_PrintUnsigned(uint32_t n);
void US_PrintSigned(int32_t n);
void US_StartCursor();
void US_ShutCursor();
void US_CheckHighScore(int32_t score, uint16_t other);
void US_DisplayHighScores(int16_t which);

bool US_UpdateCursor();

bool US_LineInput(
    int16_t x,
    int16_t y,
    char* buf,
    char* def,
    bool escok,
    int16_t maxchars,
    int16_t maxwidth);

extern int16_t US_RndT();

void USL_PrintInCenter(
    const char* s,
    Rect r);
char* USL_GiveSaveName(
    uint16_t game);


#endif // BSTONE_ID_US_INCLUDED
