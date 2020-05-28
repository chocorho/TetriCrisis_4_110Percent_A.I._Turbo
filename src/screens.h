/*
  "TetriCrisis 4 110% A.I. Turbo" - Open-source cross-platform puzzle game.
  Copyright (C) 2020 - 16BitSoft Inc.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

  Email the author at: www.16BitSoft.com
*/
#ifndef SCREENS
#define SCREENS

class Screens
{
public:

	Screens(void);
	virtual ~Screens(void);

    Uint8 ScreenFadeTransparency;
    #define FadeIn          0
    #define FadeNone        1
    #define FadeOut         2
    #define FadeAll         3
    Uint8 ScreenTransitionStatus;
    Uint32 ScreenDisplayTimer;

    #define SixteenBitSoftScreen        0
    #define TitleScreen                 1
    #define OptionsScreen               2
    #define HowToPlayScreen             3
    #define HighScoresScreen            4
    #define AboutScreen                 5
    #define NewGameOptionsScreen        6
    #define PlayingGameScreen           7
    #define TestComputerSkillScreen     8
    #define NameInputKeyboardScreen     9
    #define NameInputJoystickScreen     10
    int ScreenToDisplay;
    int ScreenIsDirty;

    void ApplyScreenFadeTransition(void);

    void ProcessScreenToDisplay(void);
    void DisplaySixteenBitSoftScreen(void);
    void DisplayTitleScreen(void);

    void DisplayNewGameOptionsScreen(void);

    void DisplayOptionsScreen(void);

    void DisplayHowToPlayScreen(void);

    void DisplayHighScoresScreen(void);

    void DisplayAboutScreen(void);

    void DisplayPlayingGameScreen(void);

	struct NameInputButton
	{
		int ScreenX;
		int ScreenY;
		float Scale;
		int ScaleTimer;
	} NameInputButtons[73];

    void DisplayNameInputKeyboardScreen(void);
    void DisplayNameInputMouseKeyboardScreen(void);
    void DisplayNameInputJoystickScreen(void);

    void DisplayTestComputerSkillScreen(void);
};

#endif

