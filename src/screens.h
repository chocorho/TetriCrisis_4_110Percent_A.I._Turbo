/*
    Copyright 2020 Team www.16BitSoft.com

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or
    substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
    FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
    COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
    AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

    void DisplayNameInputKeyboardScreen(void);
    void DisplayNameInputJoystickScreen(void);

    void DisplayTestComputerSkillScreen(void);
};

#endif

