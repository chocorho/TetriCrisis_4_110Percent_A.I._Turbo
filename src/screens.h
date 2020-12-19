/*
    Copyright 2020 Team 16BitSoft

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

    bool ClearScreenWithBlackWhen1stRunInFullScreenMode = false;

    Uint8 ScreenFadeTransparency;
    #define FadeIn          0
    #define FadeNone        1
    #define FadeOut         2
    #define FadeAll         3
    Uint8 ScreenTransitionStatus;
    Uint32 ScreenDisplayTimer;

    float JoystickScreenDisplayTimer;

    #define JoystickScreen              0
    #define SDLLogoScreen               1
    #define SixteenBitSoftScreen        2
    #define TitleScreen                 3
    #define OptionsScreen               4
    #define HowToPlayScreen             5
    #define HighScoresScreen            6
    #define AboutScreen                 7
    #define NewGameOptionsScreen        8
    #define PlayingGameScreen           9
    #define PlayingStoryGameScreen      10
    #define ShowStoryScreen             11
    #define TestComputerSkillScreen     12
    #define NameInputKeyboardScreen     13
    #define NameInputJoystickScreen     14
    #define NameInputMouseScreen        15
    int ScreenToDisplay;
    int ScreenIsDirty;

    void ApplyScreenFadeTransition(void);

    void ProcessScreenToDisplay(void);

    void DisplayJoystickScreen(void);

    void DisplaySDLLogoScreen(void);

    void DisplaySixteenBitSoftScreen(void);

    void DisplayTitleScreen(void);

    void DisplayNewGameOptionsScreen(void);

    int JoystickFlash;
    void DisplayOptionsScreen(void);

    void DisplayHowToPlayScreen(void);

    void DisplayHighScoresScreen(void);

    int ReviewShowDelay;
    float ReviewScale;
    void DisplayAboutScreen(void);

    void DisplayPlayingGameScreen(void);

    void DisplayShowStoryScreen(void);
    void DisplayPlayingStoryGameScreen(void);

    void DisplayNameInputKeyboardScreen(void);
    void DisplayNameInputMouseScreen(void);
    void DisplayNameInputJoystickScreen(void);

    void DisplayTestComputerSkillScreen(void);
};

#endif

