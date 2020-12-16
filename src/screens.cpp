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

#include <stdio.h>
#include <cstring>
#include <iostream>
#include <cmath>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "screens.h"

#include "input.h"
#include "visuals.h"
#include "interface.h"
#include "data.h"
#include "logic.h"
#include "audio.h"

extern Input* input;
extern Visuals* visuals;
extern Interface* interface;
extern Data* data;
extern Logic* logic;
extern Audio* audio;

//-------------------------------------------------------------------------------------------------
Screens::Screens(void)
{
    ScreenIsDirty = true;

    if (input->JoystickDeviceOne != NULL)
        ScreenToDisplay = JoystickScreen;
    else
        ScreenToDisplay = SDLLogoScreen;

    ScreenFadeTransparency = 255;
    ScreenTransitionStatus = FadeAll;

}

//-------------------------------------------------------------------------------------------------
Screens::~Screens(void)
{

}

//-------------------------------------------------------------------------------------------------
void Screens::ApplyScreenFadeTransition(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        visuals->ClearTextCache();
    }
    else if (ScreenTransitionStatus == FadeIn)
    {
        if ( ScreenFadeTransparency > (255/3) )
        {
            ScreenFadeTransparency -= (255/3);
            ScreenIsDirty = true;
        }
        else
        {
            ScreenFadeTransparency = 0;
            ScreenTransitionStatus = FadeNone;
        }
    }
    else if (ScreenTransitionStatus == FadeOut)
    {
        if ( ScreenFadeTransparency < (255/3) )
        {
            ScreenFadeTransparency += (255/3);
            ScreenIsDirty = true;
        }
        else
        {
            ScreenFadeTransparency = 255;

            interface->DestroyAllButtons();
            interface->DestroyAllArrowSets();
            interface->DestroyAllIcons();
        }
    }

    if (ScreenTransitionStatus != FadeNone)
    {
        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = ScreenFadeTransparency;
        visuals->DrawSpriteOntoScreenBuffer(0);
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::ProcessScreenToDisplay(void)
{
int windowWidth;
int windowHeight;

    if (input->DEBUG == true)  ScreenIsDirty = true;

    SDL_GetWindowSize(visuals->Window, &windowWidth, &windowHeight);
    if (windowWidth != visuals->WindowWidthCurrent || windowHeight != visuals->WindowHeightCurrent)
    {
        visuals->WindowWidthCurrent = windowWidth;
        visuals->WindowHeightCurrent = windowHeight;

        visuals->ClearTextCache();

        ScreenIsDirty = true;
    }

    if (input->JoystickSetupProcess == JoySetupNotStarted
     && input->KeyboardSetupProcess == KeyboardSetupNotStarted)
    {
        interface->ProcessAllButtons();
        interface->ProcessAllArrowSets();
        interface->ProcessAllIcons();
    }

    switch(ScreenToDisplay)
    {
        case JoystickScreen:
            DisplayJoystickScreen();
            break;

        case SDLLogoScreen:
            DisplaySDLLogoScreen();
            break;

        case SixteenBitSoftScreen:
            DisplaySixteenBitSoftScreen();
            break;

        case TitleScreen:
            DisplayTitleScreen();
            break;

        case OptionsScreen:
            DisplayOptionsScreen();
            break;

        case HowToPlayScreen:
            DisplayHowToPlayScreen();
            break;

        case HighScoresScreen:
            DisplayHighScoresScreen();
            break;

        case AboutScreen:
            DisplayAboutScreen();
            break;

        case NewGameOptionsScreen:
            DisplayNewGameOptionsScreen();
            break;

        case PlayingGameScreen:
            DisplayPlayingGameScreen();
            break;

        case PlayingStoryGameScreen:
            DisplayPlayingStoryGameScreen();
            break;

        case TestComputerSkillScreen:
            DisplayTestComputerSkillScreen();
            break;

        case NameInputKeyboardScreen:
            DisplayNameInputKeyboardScreen();
            break;

        case NameInputJoystickScreen:
            DisplayNameInputJoystickScreen();
            break;

        case NameInputMouseScreen:
            DisplayNameInputMouseScreen();
            break;

        default:
            break;
    }

    interface->DisplayAllButtonsOntoScreenBuffer();
    interface->DisplayAllIconsOntoScreenBuffer();

    ApplyScreenFadeTransition();

    if (input->DEBUG == true || ScreenToDisplay == TestComputerSkillScreen)
    {
        ScreenIsDirty = true;

        if (ScreenToDisplay != PlayingGameScreen)
        {
            if (input->DEBUG == true)
            {
                char temp[256];
                strcpy(visuals->VariableText, "(");
                sprintf(temp, "%i", input->MouseX);
                strcat(visuals->VariableText, temp);
                strcat(visuals->VariableText, ",");
                sprintf(temp, "%i", input->MouseY);
                strcat(visuals->VariableText, temp);
                strcat(visuals->VariableText, ")");
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[7], 3, 440
                                                  , JustifyLeft, 255, 255, 255, 0, 0, 0);
            }

            sprintf(visuals->VariableText, "%d", visuals->AverageFPS);
            strcat(visuals->VariableText, "/60");
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[7], 3, 460
                                              , JustifyLeft, 255, 255, 255, 0, 0, 0);
        }
    }

    if (ScreenIsDirty > false)
    {
        if (visuals->ForceAspectRatio == true)  SDL_RenderSetLogicalSize(visuals->Renderer, 640, 480);

        SDL_RenderPresent(visuals->Renderer);
        if (ScreenTransitionStatus != FadeIn && ScreenTransitionStatus != FadeOut)  ScreenIsDirty--;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayJoystickScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        input->JoystickDisabled[0] = 1;
        input->JoystickDisabled[1] = 1;
        input->JoystickDisabled[2] = 1;

        JoystickScreenDisplayTimer = 1000;
        ScreenTransitionStatus = FadeIn;
    }

    if (JoystickScreenDisplayTimer > 0)  JoystickScreenDisplayTimer--;
    else if (ScreenTransitionStatus != FadeIn)  ScreenTransitionStatus = FadeOut;

    int joyAction;

    joyAction = input->QueryJoysticksForAction(0, true);
    if (joyAction > 0)
        input->JoystickDisabled[0] = 0;

    joyAction = input->QueryJoysticksForAction(1, true);
    if (joyAction > 0)
        input->JoystickDisabled[1] = 0;

    joyAction = input->QueryJoysticksForAction(2, true);
    if (joyAction > 0)
        input->JoystickDisabled[2] = 0;

    ScreenIsDirty = true;
    if (ScreenIsDirty == true)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 7-6+30+10;
        visuals->Sprites[7].RedHue = 0;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        if (input->JoystickDeviceThree != NULL)
        {
            visuals->DrawTextOntoScreenBuffer("3 Joysticks Found!", visuals->Font[0]
                                              , 0, 10, JustifyCenter, 0, 255, 0, 0, 100, 0);

            visuals->DrawTextOntoScreenBuffer("Press Joystick's Button To Activate!", visuals->Font[0]
                                              , 0, 6+50, JustifyCenter, 0, 255, 0, 0, 100, 0);

            if (input->JoystickDisabled[0] == 1)
            {
                visuals->DrawTextOntoScreenBuffer("Joystick #1", visuals->Font[0]
                                  , (640/2)-200, (480/2)-20, JustifyCenterOnPoint, 255, 0, 0, 100, 0, 0);
                visuals->DrawTextOntoScreenBuffer("NOT ACTIVE", visuals->Font[0]
                                  , (640/2)-200, (480/2)+20, JustifyCenterOnPoint, 255, 0, 0, 100, 0, 0);
            }
            else
            {
                visuals->DrawTextOntoScreenBuffer("Joystick #1", visuals->Font[0]
                                  , (640/2)-200, (480/2)-20, JustifyCenterOnPoint, 0, 255, 0, 0, 100, 0);

                visuals->DrawTextOntoScreenBuffer("ACTIVE", visuals->Font[0]
                                  , (640/2)-200, (480/2)+20, JustifyCenterOnPoint, 0, 255, 0, 0, 100, 0);
            }

            if (input->JoystickDisabled[1] == 1)
            {
                visuals->DrawTextOntoScreenBuffer("Joystick #2", visuals->Font[0]
                                  , (640/2), (480/2)-20, JustifyCenterOnPoint, 255, 0, 0, 100, 0, 0);
                visuals->DrawTextOntoScreenBuffer("NOT ACTIVE", visuals->Font[0]
                                  , (640/2), (480/2)+20, JustifyCenterOnPoint, 255, 0, 0, 100, 0, 0);
            }
            else
            {
                visuals->DrawTextOntoScreenBuffer("Joystick #2", visuals->Font[0]
                                  , (640/2), (480/2)-20, JustifyCenterOnPoint, 0, 255, 0, 0, 100, 0);

                visuals->DrawTextOntoScreenBuffer("ACTIVE", visuals->Font[0]
                                  , (640/2), (480/2)+20, JustifyCenterOnPoint, 0, 255, 0, 0, 100, 0);
            }

            if (input->JoystickDisabled[2] == 1)
            {
                visuals->DrawTextOntoScreenBuffer("Joystick #3", visuals->Font[0]
                                  , (640/2)+200, (480/2)-20, JustifyCenterOnPoint, 255, 0, 0, 100, 0, 0);
                visuals->DrawTextOntoScreenBuffer("NOT ACTIVE", visuals->Font[0]
                                  , (640/2)+200, (480/2)+20, JustifyCenterOnPoint, 255, 0, 0, 100, 0, 0);
            }
            else
            {
                visuals->DrawTextOntoScreenBuffer("Joystick #3", visuals->Font[0]
                                  , (640/2)+200, (480/2)-20, JustifyCenterOnPoint, 0, 255, 0, 0, 100, 0);

                visuals->DrawTextOntoScreenBuffer("ACTIVE", visuals->Font[0]
                                  , (640/2)+200, (480/2)+20, JustifyCenterOnPoint, 0, 255, 0, 0, 100, 0);
            }
        }
        else if (input->JoystickDeviceTwo != NULL)
        {
            input->JoystickDisabled[2] = -1;

            visuals->DrawTextOntoScreenBuffer("2 Joysticks Found!", visuals->Font[0]
                                              , 0, 10, JustifyCenter, 0, 255, 0, 0, 100, 0);

            visuals->DrawTextOntoScreenBuffer("Press Joystick's Button To Activate!", visuals->Font[0]
                                              , 0, 6+50, JustifyCenter, 0, 255, 0, 0, 100, 0);

            if (input->JoystickDisabled[0] == 1)
            {
                visuals->DrawTextOntoScreenBuffer("Joystick #1", visuals->Font[0]
                                  , (640*.30), (480/2)-20, JustifyCenterOnPoint, 255, 0, 0, 100, 0, 0);
                visuals->DrawTextOntoScreenBuffer("NOT ACTIVE", visuals->Font[0]
                                  , (640*.30), (480/2)+20, JustifyCenterOnPoint, 255, 0, 0, 100, 0, 0);
            }
            else
            {
                visuals->DrawTextOntoScreenBuffer("Joystick #1", visuals->Font[0]
                                  , (640*.30), (480/2)-20, JustifyCenterOnPoint, 0, 255, 0, 0, 100, 0);

                visuals->DrawTextOntoScreenBuffer("ACTIVE", visuals->Font[0]
                                  , (640*.30), (480/2)+20, JustifyCenterOnPoint, 0, 255, 0, 0, 100, 0);
            }

            if (input->JoystickDisabled[1] == 1)
            {
                visuals->DrawTextOntoScreenBuffer("Joystick #2", visuals->Font[0]
                                  , 640-(640*.30), (480/2)-20, JustifyCenterOnPoint, 255, 0, 0, 100, 0, 0);
                visuals->DrawTextOntoScreenBuffer("NOT ACTIVE", visuals->Font[0]
                                  , 640-(640*.30), (480/2)+20, JustifyCenterOnPoint, 255, 0, 0, 100, 0, 0);
            }
            else
            {
                visuals->DrawTextOntoScreenBuffer("Joystick #2", visuals->Font[0]
                                  , 640-(640*.30), (480/2)-20, JustifyCenterOnPoint, 0, 255, 0, 0, 100, 0);

                visuals->DrawTextOntoScreenBuffer("ACTIVE", visuals->Font[0]
                                  , 640-(640*.30), (480/2)+20, JustifyCenterOnPoint, 0, 255, 0, 0, 100, 0);
            }
        }
        else if (input->JoystickDeviceOne != NULL)
        {
            input->JoystickDisabled[1] = -1;
            input->JoystickDisabled[2] = -1;

            visuals->DrawTextOntoScreenBuffer("1 Joystick Found!", visuals->Font[0]
                                              , 0, 6, JustifyCenter, 0, 255, 0, 0, 100, 0);

            visuals->DrawTextOntoScreenBuffer("Press Joystick Button To Activate!", visuals->Font[0]
                                              , 0, 6+50, JustifyCenter, 0, 255, 0, 0, 100, 0);

            if (input->JoystickDisabled[0] == 1)
            {
                visuals->DrawTextOntoScreenBuffer("Joystick #1", visuals->Font[0]
                                  , (640/2), (480/2)-20, JustifyCenterOnPoint, 255, 0, 0, 100, 0, 0);
                visuals->DrawTextOntoScreenBuffer("NOT ACTIVE", visuals->Font[0]
                                  , (640/2), (480/2)+20, JustifyCenterOnPoint, 255, 0, 0, 100, 0, 0);
            }
            else
            {
                visuals->DrawTextOntoScreenBuffer("Joystick #1", visuals->Font[0]
                                  , (640/2), (480/2)-20, JustifyCenterOnPoint, 0, 255, 0, 0, 100, 0);

                visuals->DrawTextOntoScreenBuffer("ACTIVE", visuals->Font[0]
                                  , (640/2), (480/2)+20, JustifyCenterOnPoint, 0, 255, 0, 0, 100, 0);
            }
        }
        else
        {
            visuals->DrawTextOntoScreenBuffer("No Joysticks Found?", visuals->Font[0]
                                              , 0, 10, JustifyCenter, 0, 255, 0, 0, 100, 0);

            JoystickScreenDisplayTimer = 0;
        }

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 404-6+30+10;
        visuals->Sprites[7].RedHue = 0;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        int percent = floor( (JoystickScreenDisplayTimer/1000)*10 );
        sprintf(visuals->VariableText, "%d", percent);
        visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], 0, 450
                                          , JustifyCenter, 255, 255, 0, 100, 100, 0);
    }

    if (input->JoystickDisabled[0] == 0 && input->JoystickDisabled[1] == -1 && input->JoystickDisabled[2] == -1)
    {
        if (JoystickScreenDisplayTimer > 100)  JoystickScreenDisplayTimer = 100;
    }
    else if (input->JoystickDisabled[0] == 0 && input->JoystickDisabled[1] == 0 && input->JoystickDisabled[2] == -1)
    {
        if (JoystickScreenDisplayTimer > 100)  JoystickScreenDisplayTimer = 100;
    }
    else if (input->JoystickDisabled[0] == 0 && input->JoystickDisabled[1] == 0 && input->JoystickDisabled[2] == 0)
    {
        if (JoystickScreenDisplayTimer > 100)  JoystickScreenDisplayTimer = 100;
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;

        visuals->Sprites[7].RedHue = 255;
        visuals->Sprites[7].BlueHue = 255;

        if (input->JoystickDisabled[0] == 1)
        {
            if (input->JoystickDeviceOne != NULL)
            {
                printf("SDL2 Joystick #0 ''%s'' disabled.\n", SDL_JoystickName(input->JoystickDeviceOne));
            }
        }

        if (input->JoystickDisabled[1] == 1)
        {
            if (input->JoystickDeviceTwo != NULL)
            {
                printf("SDL2 Joystick #1 ''%s'' disabled.\n", SDL_JoystickName(input->JoystickDeviceTwo));
            }
        }

        if (input->JoystickDisabled[2] == 1)
        {
            if (input->JoystickDeviceThree != NULL)
            {
                printf("SDL2 Joystick #2 ''%s'' disabled.\n", SDL_JoystickName(input->JoystickDeviceThree));
            }
        }

        ScreenToDisplay = SDLLogoScreen;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplaySDLLogoScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        ScreenDisplayTimer = 610/2;
        ScreenTransitionStatus = FadeIn;

        audio->PlayMusic(0, -1);
    }

    if (input->MouseButtonPressed[0] == true
       || input->KeyOnKeyboardPressedByUser == SDLK_SPACE
       || input->KeyOnKeyboardPressedByUser == SDLK_RETURN
       || input->JoystickButtonOne[Any] == ON)
    {
        ScreenDisplayTimer = 0;
        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(0, 0);
    }

    if (ScreenDisplayTimer > 0)  ScreenDisplayTimer--;
    else if (ScreenTransitionStatus != FadeIn)  ScreenTransitionStatus = FadeOut;

    if (ScreenIsDirty == true)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[9].ScreenX = 320;
        visuals->Sprites[9].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(9);
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;

        ScreenToDisplay = SixteenBitSoftScreen;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplaySixteenBitSoftScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        ScreenDisplayTimer = 610/2;
        ScreenTransitionStatus = FadeIn;
    }

    if (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_d)
    {
        input->DEBUG = !input->DEBUG;
        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(1, 0);
    }

    if (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_t)
    {
        ScreenToDisplay = TestComputerSkillScreen;
        ScreenTransitionStatus = FadeAll;
        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(1, 0);

        audio->SoundVolume = 0;
        audio->MusicVolume = 0;

        audio->PlayMusic(0, -1);

        logic->PlayersCanJoin = false;
    }

    if (input->MouseButtonPressed[0] == true
       || input->KeyOnKeyboardPressedByUser == SDLK_SPACE
       || input->KeyOnKeyboardPressedByUser == SDLK_RETURN
       || input->JoystickButtonOne[Any] == ON)
    {
        ScreenDisplayTimer = 0;
        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(0, 0);
    }

    if (ScreenDisplayTimer > 0)  ScreenDisplayTimer--;
    else if (ScreenTransitionStatus != FadeIn)  ScreenTransitionStatus = FadeOut;

    if (ScreenIsDirty == true)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[1].ScreenX = 320;
        visuals->Sprites[1].ScreenY = 240;
        visuals->Sprites[1].ScaleX = 6;
        visuals->Sprites[1].ScaleY = 8;
        visuals->DrawSpriteOntoScreenBuffer(1);

        visuals->DrawTextOntoScreenBuffer("Bringing back old memories from the 16bit era!", visuals->Font[1]
                                          , 0, 307, JustifyCenter, 1, 255, 1, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Team 16BitSoft", visuals->Font[0]
                                          , 0, 447, JustifyCenter, 1, 255, 1, 1, 90, 1);
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );

        ScreenTransitionStatus = FadeAll;

        ScreenToDisplay = TitleScreen;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayTitleScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        int buttonStartY = 204;
        int buttonOffsetY = 43;
        interface->CreateButton( 1003, 0, buttonStartY );
        interface->CreateButton( 1004, 1, buttonStartY + (buttonOffsetY*1) );
        interface->CreateButton( 1005, 2, buttonStartY + (buttonOffsetY*2) );
        interface->CreateButton( 1006, 3, buttonStartY + (buttonOffsetY*3) );
        interface->CreateButton( 1007, 4, buttonStartY + (buttonOffsetY*4) );
        interface->CreateButton( 1008, 5, buttonStartY + (buttonOffsetY*5) );

        ScreenTransitionStatus = FadeIn;
    }

    if (ScreenIsDirty == true)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[2].ScreenX = 320;
        visuals->Sprites[2].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(2);

        visuals->DrawTextOntoScreenBuffer("JeZxLee's", visuals->Font[7]
                                          , 360, 3, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->Sprites[3].ScreenX = 320;
        visuals->Sprites[3].ScreenY = 100;
        visuals->Sprites[3].Smooth = true;
        visuals->DrawSpriteOntoScreenBuffer(3);

        visuals->DrawTextOntoScreenBuffer("A.I. 100,000+", visuals->Font[2]
                                          , 585+2, 80+6+4, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Line Average", visuals->Font[2]
                                          , 585, 96+6+4, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

        char temp[256];
        strcpy(visuals->VariableText, "''");
        strcat(visuals->VariableText, data->HighScoresName[logic->GameMode][0]);
        strcat(visuals->VariableText, "'' Scored: ");

        #ifdef _WIN32
            sprintf(temp, "%I64u", data->HighScoresScore[logic->GameMode][0]);
        #else
            sprintf(temp, "%lu", data->HighScoresScore[logic->GameMode][0]);
        #endif

        strcat(visuals->VariableText, temp);

        visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], 0, 135+16
                                          , JustifyCenter, 255, 255, 0, 1, 1, 1);

        visuals->Sprites[10].ScreenX = 565;
        visuals->Sprites[10].ScreenY = 418-5;
        visuals->DrawSpriteOntoScreenBuffer(10);

        visuals->DrawTextOntoScreenBuffer("Retail3 4.5.6 Remix", visuals->Font[2]
                                          , 2+3, 462-2, JustifyRight, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Team 16BitSoft", visuals->Font[0]
                                          , 0, 447, JustifyCenter, 255, 255, 255, 1, 1, 1);
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;

        if (interface->ButtonSelectedByPlayer == 1)  ScreenToDisplay = OptionsScreen;
        else if (interface->ButtonSelectedByPlayer == 2)  ScreenToDisplay = HowToPlayScreen;
        else if (interface->ButtonSelectedByPlayer == 3)  ScreenToDisplay = HighScoresScreen;
        else if (interface->ButtonSelectedByPlayer == 4)  ScreenToDisplay = AboutScreen;
        else if (interface->ButtonSelectedByPlayer == 5)  input->EXIT_Game = true;
        else if (interface->ButtonSelectedByPlayer == 0)
        {
            if (logic->GameMode < StoryMode)
                ScreenToDisplay = NewGameOptionsScreen;
            else
                ScreenToDisplay = NewGameOptionsScreen;
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayNewGameOptionsScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        interface->CreateButton(1003, 0, 454);

        interface->CreateArrowSet(0, 65);
        interface->CreateArrowSet(1, 105);
        interface->CreateArrowSet(2, 145);
        interface->CreateArrowSet(3, 185);
        interface->CreateArrowSet(4, 225);
        interface->CreateArrowSet(5, 265);
        interface->CreateArrowSet(6, 305);
        interface->CreateArrowSet(7, 345);

        if (logic->GameMode != StoryMode)
            audio->PlayMusic(1+logic->SelectedMusicTrack, -1);
        else
            if (Mix_PlayingMusic() == 1)  Mix_HaltMusic();

        ScreenTransitionStatus = FadeIn;
    }

    if (interface->ArrowSetArrowSelectedByPlayer != -1)
    {
        if (interface->ArrowSetArrowSelectedByPlayer == 0)
        {
            if (logic->GameMode > 0)  logic->GameMode-=1;
            else  logic->GameMode = StoryMode;

            if (logic->GameMode != StoryMode)
                audio->PlayMusic(1+logic->SelectedMusicTrack, -1);
            else
                if (Mix_PlayingMusic() == 1)  Mix_HaltMusic();
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 0.5)
        {
            if (logic->GameMode < StoryMode)  logic->GameMode+=1;
            else  logic->GameMode = 0;

            if (logic->GameMode != StoryMode)
                audio->PlayMusic(1+logic->SelectedMusicTrack, -1);
            else
                if (Mix_PlayingMusic() == 1)  Mix_HaltMusic();
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 1 && audio->MusicJukeboxMode == 0 && logic->GameMode != StoryMode)
        {
            if (logic->SelectedMusicTrack > 0)  logic->SelectedMusicTrack-=1;
            else  logic->SelectedMusicTrack = 18;

            audio->PlayMusic(1+logic->SelectedMusicTrack, -1);
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 1.5 && audio->MusicJukeboxMode == 0 && logic->GameMode != StoryMode)
        {
            if (logic->SelectedMusicTrack < 18)  logic->SelectedMusicTrack+=1;
            else  logic->SelectedMusicTrack = 0;

            audio->PlayMusic(1+logic->SelectedMusicTrack, -1);
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 2 && logic->GameMode != StoryMode)
        {
            if (audio->MusicJukeboxMode > 0)  audio->MusicJukeboxMode-=1;
            else  audio->MusicJukeboxMode = 1;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 2.5 && logic->GameMode != StoryMode)
        {
            if (audio->MusicJukeboxMode < 1)  audio->MusicJukeboxMode+=1;
            else  audio->MusicJukeboxMode = 0;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 3)
        {
            if (logic->GameMode != StoryMode)
            {
                if (logic->SelectedBackground > 0)  logic->SelectedBackground-=1;
                else  logic->SelectedBackground = 6;
            }
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 3.5)
        {
            if (logic->GameMode != StoryMode)
            {
                if (logic->SelectedBackground < 6)  logic->SelectedBackground+=1;
                else  logic->SelectedBackground = 0;
            }
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 4 && logic->GameMode != StoryMode)
        {
            if (logic->NewGameGarbageHeight > 0)  logic->NewGameGarbageHeight-=1;
            else  logic->NewGameGarbageHeight = 14;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 4.5 && logic->GameMode != StoryMode)
        {
            if (logic->NewGameGarbageHeight < 14)  logic->NewGameGarbageHeight+=1;
            else  logic->NewGameGarbageHeight = 0;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 5)
        {
            if (logic->DisplayNextPiece > 0)  logic->DisplayNextPiece-=1;
            else  logic->DisplayNextPiece = 1;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 5.5)
        {
            if (logic->DisplayNextPiece < 1)  logic->DisplayNextPiece+=1;
            else  logic->DisplayNextPiece = 0;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 6)
        {
            if (logic->DisplayDropShadow > 0)  logic->DisplayDropShadow-=1;
            else  logic->DisplayDropShadow = 1;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 6.5)
        {
            if (logic->DisplayDropShadow < 1)  logic->DisplayDropShadow+=1;
            else  logic->DisplayDropShadow = 0;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 7 && logic->GameMode != StoryMode)
        {
            if (logic->TileSet > 0)  logic->TileSet-=1;
            else  logic->TileSet = 5;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 7.5 && logic->GameMode != StoryMode)
        {
            if (logic->TileSet < 5)  logic->TileSet+=1;
            else  logic->TileSet = 0;
        }

        interface->ArrowSetArrowSelectedByPlayer = -1;
    }

    if (ScreenIsDirty == true)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        if (logic->GameMode != StoryMode)
        {
        visuals->Sprites[100+logic->SelectedBackground].ScreenX = 320;
        visuals->Sprites[100+logic->SelectedBackground].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(100+logic->SelectedBackground);
        }
        else
        {
            visuals->Sprites[108].ScreenX = 320;
            visuals->Sprites[108].ScreenY = 240;
            visuals->DrawSpriteOntoScreenBuffer(108);
        }

        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = 120;
        visuals->DrawSpriteOntoScreenBuffer(0);

        visuals->DrawTextOntoScreenBuffer("N E W   G A M E   O P T I O N S:", visuals->Font[0]
                                          , 0, 1, JustifyCenter, 255, 255, 1, 90, 90, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 7-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        interface->DisplayAllArrowSetsOntoScreenBuffer();

        visuals->DrawTextOntoScreenBuffer("Game Mode:", visuals->Font[0]
                                          , 60, 65-15+3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (logic->GameMode == OriginalMode)
            visuals->DrawTextOntoScreenBuffer("Original", visuals->Font[0]
                                              , 60, 65-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == TimeAttack30Mode)
            visuals->DrawTextOntoScreenBuffer("Time Attack 30", visuals->Font[0]
                                              , 60, 65-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == TimeAttack60Mode)
            visuals->DrawTextOntoScreenBuffer("Time Attack 60", visuals->Font[0]
                                              , 60, 65-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == TimeAttack120Mode)
            visuals->DrawTextOntoScreenBuffer("Time Attack 120", visuals->Font[0]
                                              , 60, 65-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == TwentyLineChallengeMode)
            visuals->DrawTextOntoScreenBuffer("20 Line Challenge", visuals->Font[0]
                                              , 60, 65-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == CrisisMode)
            visuals->DrawTextOntoScreenBuffer("Crisis+Mode", visuals->Font[0]
                                              , 60, 65-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == StoryMode)
            visuals->DrawTextOntoScreenBuffer("1 Player Story+Mode", visuals->Font[0]
                                              , 60, 65-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Music Track:", visuals->Font[0]
                                          , 60, 105-15+3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);

        if (logic->GameMode == StoryMode)
            visuals->DrawTextOntoScreenBuffer("Story BGM's", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->MusicJukeboxMode == 1)
            visuals->DrawTextOntoScreenBuffer("Random", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 0)
            visuals->DrawTextOntoScreenBuffer("Track 00", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 1)
            visuals->DrawTextOntoScreenBuffer("Track 01", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 2)
            visuals->DrawTextOntoScreenBuffer("Track 02", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 3)
            visuals->DrawTextOntoScreenBuffer("Track 03", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 4)
            visuals->DrawTextOntoScreenBuffer("Track 04", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 5)
            visuals->DrawTextOntoScreenBuffer("Track 05", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 6)
            visuals->DrawTextOntoScreenBuffer("Track 06", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 7)
            visuals->DrawTextOntoScreenBuffer("Track 07", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 8)
            visuals->DrawTextOntoScreenBuffer("Track 08", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 9)
            visuals->DrawTextOntoScreenBuffer("Track 09", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 10)
            visuals->DrawTextOntoScreenBuffer("Track 10", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 11)
            visuals->DrawTextOntoScreenBuffer("Track 11", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 12)
            visuals->DrawTextOntoScreenBuffer("Track 12", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 13)
            visuals->DrawTextOntoScreenBuffer("Track 13", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 14)
            visuals->DrawTextOntoScreenBuffer("Track 14", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 15)
            visuals->DrawTextOntoScreenBuffer("Track 15", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 16)
            visuals->DrawTextOntoScreenBuffer("Track 16", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 17)
            visuals->DrawTextOntoScreenBuffer("Track 17", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 18)
            visuals->DrawTextOntoScreenBuffer("Track 18", visuals->Font[0]
                                              , 60, 105-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Music Mode:", visuals->Font[0]
                                          , 60, 145-15+3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);

        if (logic->GameMode == StoryMode)
            visuals->DrawTextOntoScreenBuffer("Story", visuals->Font[0]
                                              , 60, 145-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->MusicJukeboxMode == 0)
            visuals->DrawTextOntoScreenBuffer("Repeat", visuals->Font[0]
                                              , 60, 145-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->MusicJukeboxMode == 1)
            visuals->DrawTextOntoScreenBuffer("Random", visuals->Font[0]
                                              , 60, 145-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Background:", visuals->Font[0]
                                          , 60, 185-15+3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);

        if (logic->GameMode == StoryMode)
            visuals->DrawTextOntoScreenBuffer("Story BG's", visuals->Font[0]
                                              , 60, 185-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedBackground == 0)
            visuals->DrawTextOntoScreenBuffer("Cathedral", visuals->Font[0]
                                              , 60, 185-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedBackground == 1)
            visuals->DrawTextOntoScreenBuffer("Firefox", visuals->Font[0]
                                              , 60, 185-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedBackground == 2)
            visuals->DrawTextOntoScreenBuffer("GT-R", visuals->Font[0]
                                              , 60, 185-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedBackground == 3)
            visuals->DrawTextOntoScreenBuffer("New York", visuals->Font[0]
                                              , 60, 185-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedBackground == 4)
            visuals->DrawTextOntoScreenBuffer("Painting", visuals->Font[0]
                                              , 60, 185-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedBackground == 5)
            visuals->DrawTextOntoScreenBuffer("Kittens", visuals->Font[0]
                                              , 60, 185-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedBackground == 6)
            visuals->DrawTextOntoScreenBuffer("Psychedelic", visuals->Font[0]
                                              , 60, 185-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Garbage Height:", visuals->Font[0]
                                          , 60, 225-15+3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);

        char temp[256];
        sprintf(temp, "%d", logic->NewGameGarbageHeight);
        strcpy(visuals->VariableText, temp);

        visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0]
                                          , 60, 225-15+3, JustifyRight
                                          , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Display Next:", visuals->Font[0]
                                          , 60, 265-15+3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (logic->DisplayNextPiece == 0)
            visuals->DrawTextOntoScreenBuffer("OFF", visuals->Font[0]
                                              , 60, 265-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->DisplayNextPiece == 1)
            visuals->DrawTextOntoScreenBuffer("ON", visuals->Font[0]
                                              , 60, 265-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Display Shadow:", visuals->Font[0]
                                          , 60, 305-15+3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (logic->DisplayDropShadow == 0)
            visuals->DrawTextOntoScreenBuffer("OFF", visuals->Font[0]
                                              , 60, 305-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->DisplayDropShadow == 1)
            visuals->DrawTextOntoScreenBuffer("ON", visuals->Font[0]
                                              , 60, 305-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Box Tileset:", visuals->Font[0]
                                          , 60, 345-15+3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Press [Enter] Or Click [Start!] To Begin!"
                                          , visuals->Font[0], 0, 380, JustifyCenter
                                          , 255, 255, 255, 1, 1, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 404-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        float x = 401-10;
        for (int boxIndex = 0; boxIndex < 7; boxIndex++)
        {
            visuals->Sprites[202+boxIndex+(logic->TileSet*10)].ScreenX = x;
            visuals->Sprites[202+boxIndex+(logic->TileSet*10)].ScreenY = 345;
            visuals->Sprites[202+boxIndex+(logic->TileSet*10)].ScaleX = 1.8;
            visuals->Sprites[202+boxIndex+(logic->TileSet*10)].ScaleY = 1.8;
            visuals->DrawSpriteOntoScreenBuffer( 202+boxIndex+(logic->TileSet*10) );
            visuals->Sprites[202+boxIndex+(logic->TileSet*10)].ScaleX = 1;
            visuals->Sprites[202+boxIndex+(logic->TileSet*10)].ScaleY = 1;

            x+=30;
        }
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;

        if (interface->ButtonSelectedByPlayer == 0)
        {
            SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0" );

            if (logic->GameMode < StoryMode)
                ScreenToDisplay = PlayingGameScreen;
            else
                ScreenToDisplay = PlayingStoryGameScreen;

            if (audio->MusicJukeboxMode == 0)
                audio->PlayMusic(1+logic->SelectedMusicTrack, -1);
            else
                audio->PlayMusic( (rand()%22 + 1 ), 0 );

            if (logic->SelectedBackground == 1)  Mix_PauseMusic();
        }
        else
        {
            ScreenToDisplay = TitleScreen;
            audio->PlayMusic(0, -1);
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayOptionsScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        JoystickFlash = 0;

        interface->CreateButton(1009, 0, 454);

        interface->CreateArrowSet(0, 60);
        interface->CreateArrowSet(1, 105-4);

        interface->CreateArrowSet(2, 160-7);

        interface->CreateArrowSet(3, 215-10);

        interface->CreateArrowSet(4, 215+45-10);

        interface->CreateArrowSet(5, 215+45-10+45);

        interface->CreateArrowSet(6, 325+16);

        ScreenTransitionStatus = FadeIn;
    }

    if (input->KeyOnKeyboardPressedByUser == SDLK_F2 && input->JoystickSetupProcess == JoySetupNotStarted)
    {
        ScreenIsDirty = true;
        audio->PlayDigitalSoundFX(1, 0);

        if (input->KeyboardSetupProcess == KeyboardSetupNotStarted)
        {
            input->KeyboardSetupProcess = KeyboardSetupPressOne;
        }
        else
        {
            input->KeyboardSetupProcess = KeyboardSetupNotStarted;

            input->UserDefinedKeyButtonOne = -1;
            input->UserDefinedKeyButtonTwo = -1;
            input->UserDefinedKeyUP = -1;
            input->UserDefinedKeyRIGHT = -1;
            input->UserDefinedKeyDOWN = -1;
            input->UserDefinedKeyLEFT = -1;
            input->UserDefinedKeyPause = -1;
        }
    }

    if (input->KeyboardSetupProcess == KeyboardSetupPressOne && input->KeyOnKeyboardPressedByUser != SDLK_F2 && input->KeyOnKeyboardPressedByUser != SDLK_RETURN
        && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyButtonOne = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupPressTwo;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = true;
    }
    else if (input->KeyboardSetupProcess == KeyboardSetupPressTwo && input->KeyOnKeyboardPressedByUser != SDLK_F2 && input->KeyOnKeyboardPressedByUser != SDLK_RETURN
             && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyButtonTwo = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupPressUP;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = true;
    }
    else if (input->KeyboardSetupProcess == KeyboardSetupPressUP && input->KeyOnKeyboardPressedByUser != SDLK_F2 && input->KeyOnKeyboardPressedByUser != SDLK_RETURN
             && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyUP = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupPressRIGHT;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = true;
    }
    else if (input->KeyboardSetupProcess == KeyboardSetupPressRIGHT && input->KeyOnKeyboardPressedByUser != SDLK_F2 && input->KeyOnKeyboardPressedByUser != SDLK_RETURN
             && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyRIGHT = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupPressDOWN;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = true;
    }
    else if (input->KeyboardSetupProcess == KeyboardSetupPressDOWN && input->KeyOnKeyboardPressedByUser != SDLK_F2 && input->KeyOnKeyboardPressedByUser != SDLK_RETURN
             && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyDOWN = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupPressLEFT;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = true;
    }
    else if (input->KeyboardSetupProcess == KeyboardSetupPressLEFT && input->KeyOnKeyboardPressedByUser != SDLK_F2 && input->KeyOnKeyboardPressedByUser != SDLK_RETURN
             && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyLEFT = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupPressPause;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = true;
    }
    else if (input->KeyboardSetupProcess == KeyboardSetupPressPause && input->KeyOnKeyboardPressedByUser != SDLK_F2 && input->KeyOnKeyboardPressedByUser != SDLK_RETURN
             && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyPause = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupNotStarted;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = true;
    }

    if ( input->KeyOnKeyboardPressedByUser == SDLK_F1
        && input->KeyboardSetupProcess == KeyboardSetupNotStarted )
    {
        ScreenIsDirty = true;
        audio->PlayDigitalSoundFX(1, 0);

        if (input->JoystickSetupProcess == JoySetupNotStarted)
        {
            if (input->JoystickDeviceOne != NULL)
                input->JoystickSetupProcess = Joy1SetupPressUP;
            else if (input->JoystickDeviceTwo != NULL)
                input->JoystickSetupProcess = Joy2SetupPressUP;
            else if (input->JoystickDeviceThree != NULL)
                input->JoystickSetupProcess = Joy3SetupPressUP;
            else
                input->JoystickSetupProcess = JoySetupNotStarted;
        }
        else
        {
            input->JoystickSetupProcess = JoySetupNotStarted;

            for (int joy = 0; joy < 2; joy++)
            {
                input->JoyUP[joy] = Axis1;
                input->JoyDOWN[joy] = Axis1;
                input->JoyLEFT[joy] = Axis0;
                input->JoyRIGHT[joy] = Axis0;
                input->JoyButton1[joy] = Button0;
                input->JoyButton2[joy] = Button1;
            }
        }
    }

    int joyAction = -1;
    if (input->JoystickSetupProcess > Joy2SetupPressBUTTONTwo)
        joyAction = input->QueryJoysticksForAction(2, false);
    else if (input->JoystickSetupProcess > Joy1SetupPressBUTTONTwo)
        joyAction = input->QueryJoysticksForAction(1, false);
    else if (input->JoystickSetupProcess > JoySetupNotStarted)
        joyAction = input->QueryJoysticksForAction(0, false);

    if (joyAction > -1)
    {
        if (input->JoystickSetupProcess == Joy1SetupPressUP)
        {
            input->JoyUP[0] = joyAction;
            input->JoystickSetupProcess = Joy1SetupPressDOWN;
            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 20;
            ScreenIsDirty = true;
        }
        else if (input->JoystickSetupProcess == Joy1SetupPressDOWN)
        {
            input->JoyDOWN[0] = joyAction;
            input->JoystickSetupProcess = Joy1SetupPressLEFT;
            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 20;
            ScreenIsDirty = true;
        }
        else if (input->JoystickSetupProcess == Joy1SetupPressLEFT)
        {
            input->JoyLEFT[0] = joyAction;
            input->JoystickSetupProcess = Joy1SetupPressRIGHT;
            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 20;
            ScreenIsDirty = true;
        }
        else if (input->JoystickSetupProcess == Joy1SetupPressRIGHT)
        {
            input->JoyRIGHT[0] = joyAction;
            input->JoystickSetupProcess = Joy1SetupPressBUTTONOne;
            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 20;
            ScreenIsDirty = true;
        }
        else if (input->JoystickSetupProcess == Joy1SetupPressBUTTONOne)
        {
            input->JoyButton1[0] = joyAction;
            input->JoystickSetupProcess = Joy1SetupPressBUTTONTwo;
            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 20;
            ScreenIsDirty = true;
        }
        else if (input->JoystickSetupProcess == Joy1SetupPressBUTTONTwo)
        {
            input->JoyButton2[0] = joyAction;

            if (input->JoystickDeviceTwo)
                input->JoystickSetupProcess = Joy2SetupPressUP;
            else
                input->JoystickSetupProcess = JoySetupNotStarted;

            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 20;
            ScreenIsDirty = true;
        }
        else if (input->JoystickSetupProcess == Joy2SetupPressUP)
        {
            input->JoyUP[1] = joyAction;
            input->JoystickSetupProcess = Joy2SetupPressDOWN;
            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 20;
            ScreenIsDirty = true;
        }
        else if (input->JoystickSetupProcess == Joy2SetupPressDOWN)
        {
            input->JoyDOWN[1] = joyAction;
            input->JoystickSetupProcess = Joy2SetupPressLEFT;
            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 20;
            ScreenIsDirty = true;
        }
        else if (input->JoystickSetupProcess == Joy2SetupPressLEFT)
        {
            input->JoyLEFT[1] = joyAction;
            input->JoystickSetupProcess = Joy2SetupPressRIGHT;
            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 20;
            ScreenIsDirty = true;
        }
        else if (input->JoystickSetupProcess == Joy2SetupPressRIGHT)
        {
            input->JoyRIGHT[1] = joyAction;
            input->JoystickSetupProcess = Joy2SetupPressBUTTONOne;
            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 20;
            ScreenIsDirty = true;
        }
        else if (input->JoystickSetupProcess == Joy2SetupPressBUTTONOne)
        {
            input->JoyButton1[1] = joyAction;
            input->JoystickSetupProcess = Joy2SetupPressBUTTONTwo;
            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 20;
            ScreenIsDirty = true;
        }
        else if (input->JoystickSetupProcess == Joy2SetupPressBUTTONTwo)
        {
            input->JoyButton2[1] = joyAction;

            if (input->JoystickDeviceThree)
                input->JoystickSetupProcess = Joy3SetupPressUP;
            else
                input->JoystickSetupProcess = JoySetupNotStarted;

            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 20;
            ScreenIsDirty = true;
        }
        else if (input->JoystickSetupProcess == Joy3SetupPressUP)
        {
            input->JoyUP[2] = joyAction;
            input->JoystickSetupProcess = Joy3SetupPressDOWN;
            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 20;
            ScreenIsDirty = true;
        }
        else if (input->JoystickSetupProcess == Joy3SetupPressDOWN)
        {
            input->JoyDOWN[2] = joyAction;
            input->JoystickSetupProcess = Joy3SetupPressLEFT;
            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 20;
            ScreenIsDirty = true;
        }
        else if (input->JoystickSetupProcess == Joy3SetupPressLEFT)
        {
            input->JoyLEFT[2] = joyAction;
            input->JoystickSetupProcess = Joy3SetupPressRIGHT;
            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 20;
            ScreenIsDirty = true;
        }
        else if (input->JoystickSetupProcess == Joy3SetupPressRIGHT)
        {
            input->JoyRIGHT[2] = joyAction;
            input->JoystickSetupProcess = Joy3SetupPressBUTTONOne;
            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 20;
            ScreenIsDirty = true;
        }
        else if (input->JoystickSetupProcess == Joy3SetupPressBUTTONOne)
        {
            input->JoyButton1[2] = joyAction;
            input->JoystickSetupProcess = Joy3SetupPressBUTTONTwo;
            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 20;
            ScreenIsDirty = true;
        }
        else if (input->JoystickSetupProcess == Joy3SetupPressBUTTONTwo)
        {
            input->JoyButton2[2] = joyAction;
            input->JoystickSetupProcess = JoySetupNotStarted;
            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 20;
            ScreenIsDirty = true;
        }
    }

    if (input->JoystickSetupProcess == JoySetupNotStarted
     && input->KeyboardSetupProcess == KeyboardSetupNotStarted)
    {
        if (interface->ArrowSetArrowSelectedByPlayer != -1)
        {
            if (interface->ArrowSetArrowSelectedByPlayer == 0)
            {
                if (audio->MusicVolume > 0)  audio->MusicVolume-=32;
                else  audio->MusicVolume = 128;

                Mix_VolumeMusic(audio->MusicVolume);

                if (audio->MusicVolume == 0)
                {
                    if (Mix_PlayingMusic() == 1)  Mix_HaltMusic();
                }
                else
                {
                    audio->PlayMusic(audio->CurrentMusicTrackPlaying, -1);
                }
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 0.5)
            {
                if (audio->MusicVolume < 128)  audio->MusicVolume+=32;
                else  audio->MusicVolume = 0;

                Mix_VolumeMusic(audio->MusicVolume);

                if (audio->MusicVolume == 0)
                {
                    if (Mix_PlayingMusic() == 1)  Mix_HaltMusic();
                }
                else
                {
                    audio->PlayMusic(audio->CurrentMusicTrackPlaying, -1);
                }
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 1)
            {
                if (audio->SoundVolume > 0)  audio->SoundVolume-=32;
                else  audio->SoundVolume = 128;

                Mix_HaltChannel(-1);
                audio->PlayDigitalSoundFX(1, 0);
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 1.5)
            {
                if (audio->SoundVolume < 128)  audio->SoundVolume+=32;
                else  audio->SoundVolume = 0;

                Mix_HaltChannel(-1);
                audio->PlayDigitalSoundFX(1, 0);
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 2)
            {
                visuals->FullScreenMode = !visuals->FullScreenMode;

                if (visuals->FullScreenMode == false)  SDL_SetWindowFullscreen(visuals->Window, 0);
                else if (visuals->FullScreenMode == true)  SDL_SetWindowFullscreen(visuals->Window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 2.5)
            {
                visuals->FullScreenMode = !visuals->FullScreenMode;

                if (visuals->FullScreenMode == false)  SDL_SetWindowFullscreen(visuals->Window, 0);
                else if (visuals->FullScreenMode == true)  SDL_SetWindowFullscreen(visuals->Window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 3)
            {
                if (logic->UseOldAI == 1)
                {
                    if (logic->CPUPlayerEnabled > 0)  logic->CPUPlayerEnabled-=1;
                    else
                    {
                        logic->CPUPlayerEnabled = 4;
                        logic->UseOldAI = 0;
                    }
                }
                else if (logic->UseOldAI == 0)
                {
                    if (logic->CPUPlayerEnabled > 0)  logic->CPUPlayerEnabled-=1;
                    else
                    {
                        logic->CPUPlayerEnabled = 4;
                        logic->UseOldAI = 1;
                    }
                }
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 3.5)
            {
                if (logic->UseOldAI == 1)
                {
                    if (logic->CPUPlayerEnabled < 4)  logic->CPUPlayerEnabled+=1;
                    else
                    {
                        logic->CPUPlayerEnabled = 0;
                        logic->UseOldAI = 0;
                    }
                }
                else if (logic->UseOldAI == 0)
                {
                    if (logic->CPUPlayerEnabled < 4)  logic->CPUPlayerEnabled+=1;
                    else
                    {
                        logic->CPUPlayerEnabled = 0;
                        logic->UseOldAI = 1;
                    }
                }
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 4)
            {
                if (logic->DelayAutoShift > 0)  logic->DelayAutoShift-=1;
                else  logic->DelayAutoShift = 2;
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 4.5)
            {
                if (logic->DelayAutoShift < 2)  logic->DelayAutoShift+=1;
                else  logic->DelayAutoShift = 0;
            }

            if (interface->ArrowSetArrowSelectedByPlayer == 5)
            {
                if (logic->PressingUPAction > 0)  logic->PressingUPAction-=1;
                else  logic->PressingUPAction = 3;
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 5.5)
            {
                if (logic->PressingUPAction < 3)  logic->PressingUPAction+=1;
                else  logic->PressingUPAction = 0;
            }

            else if (interface->ArrowSetArrowSelectedByPlayer == 6)
            {
                if (logic->PlayingGameFrameLock == 33)  logic->PlayingGameFrameLock = 10;
                else if (logic->PlayingGameFrameLock == 10)  logic->PlayingGameFrameLock = 25;
                else if (logic->PlayingGameFrameLock == 25)  logic->PlayingGameFrameLock = 33;
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 6.5)
            {
                if (logic->PlayingGameFrameLock == 33)  logic->PlayingGameFrameLock = 25;
                else if (logic->PlayingGameFrameLock == 25)  logic->PlayingGameFrameLock = 10;
                else if (logic->PlayingGameFrameLock == 10)  logic->PlayingGameFrameLock = 33;
            }

            interface->ArrowSetArrowSelectedByPlayer = -1;
        }
    }

    if (JoystickFlash < 320)
        JoystickFlash+=1;
    else
        JoystickFlash = 0;

    if (JoystickFlash == 0)
        ScreenIsDirty = true;
    else if (JoystickFlash == 161)
        ScreenIsDirty = true;

//    if (ScreenIsDirty == true)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[2].ScreenX = 320;
        visuals->Sprites[2].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(2);

        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = 200;
        visuals->DrawSpriteOntoScreenBuffer(0);

        visuals->DrawTextOntoScreenBuffer("O P T I O N S:", visuals->Font[0]
                                          , 0, 1, JustifyCenter, 255, 255, 1, 90, 90, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 7-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        interface->DisplayAllArrowSetsOntoScreenBuffer();

        visuals->DrawTextOntoScreenBuffer("Music Volume:"
                                          , visuals->Font[0], 60, 60-15+3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (audio->MusicVolume == 0)
            visuals->DrawTextOntoScreenBuffer("OFF", visuals->Font[0], 60, 60-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->MusicVolume == 32)
            visuals->DrawTextOntoScreenBuffer("25%", visuals->Font[0], 60, 60-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->MusicVolume == 64)
            visuals->DrawTextOntoScreenBuffer("50%", visuals->Font[0], 60, 60-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->MusicVolume == 96)
            visuals->DrawTextOntoScreenBuffer("75%", visuals->Font[0], 60, 60-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->MusicVolume == 128)
            visuals->DrawTextOntoScreenBuffer("100%", visuals->Font[0], 60, 60-15+3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Sound Effects Volume:"
                                          , visuals->Font[0], 60, 105-15+3-4, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (audio->SoundVolume == 0)
            visuals->DrawTextOntoScreenBuffer("OFF", visuals->Font[0], 60, 105-15+3-4, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->SoundVolume == 32)
            visuals->DrawTextOntoScreenBuffer("25%", visuals->Font[0], 60, 105-15+3-4, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->SoundVolume == 64)
            visuals->DrawTextOntoScreenBuffer("50%", visuals->Font[0], 60, 105-15+3-4, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->SoundVolume == 96)
            visuals->DrawTextOntoScreenBuffer("75%", visuals->Font[0], 60, 105-15+3-4, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->SoundVolume == 128)
            visuals->DrawTextOntoScreenBuffer("100%", visuals->Font[0], 60, 105-15+3-4, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 107-6-4, JustifyCenter
                                          , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Full Screen Mode:"
                                          , visuals->Font[0], 60, 160-15+3-7, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (visuals->FullScreenMode == false)
            visuals->DrawTextOntoScreenBuffer("OFF", visuals->Font[0], 60, 160-15+3-7, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (visuals->FullScreenMode == true)
            visuals->DrawTextOntoScreenBuffer("ON", visuals->Font[0], 60, 160-15+3-7, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 162-6-7, JustifyCenter
                                          , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("C.P.U. Players:"
                                          , visuals->Font[0], 60, 215-15+3-10, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (logic->CPUPlayerEnabled == 0)
            visuals->DrawTextOntoScreenBuffer("OFF", visuals->Font[0], 60, 215-15+3-10, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        if (logic->UseOldAI == 1)
        {
            if (logic->CPUPlayerEnabled == 1)
                visuals->DrawTextOntoScreenBuffer("Slow(Old)", visuals->Font[0], 60, 215-15+3-10, JustifyRight
                                                  , 255, 255, 255, 1, 1, 1);
            else if (logic->CPUPlayerEnabled == 2)
                visuals->DrawTextOntoScreenBuffer("Medium(old)", visuals->Font[0], 60, 215-15+3-10, JustifyRight
                                                  , 255, 255, 255, 1, 1, 1);
            else if (logic->CPUPlayerEnabled == 3)
                visuals->DrawTextOntoScreenBuffer("Fast(old)", visuals->Font[0], 60, 215-15+3-10, JustifyRight
                                                  , 255, 255, 255, 1, 1, 1);
            else if (logic->CPUPlayerEnabled == 4)
                visuals->DrawTextOntoScreenBuffer("Turbo!(old)", visuals->Font[0], 60, 215-15+3-10, JustifyRight
                                                  , 255, 255, 255, 1, 1, 1);
        }
        else if (logic->UseOldAI == 0)
        {
            if (logic->CPUPlayerEnabled == 1)
                visuals->DrawTextOntoScreenBuffer("Slow(New)", visuals->Font[0], 60, 215-15+3-10, JustifyRight
                                                  , 255, 255, 255, 1, 1, 1);
            else if (logic->CPUPlayerEnabled == 2)
                visuals->DrawTextOntoScreenBuffer("Medium(New)", visuals->Font[0], 60, 215-15+3-10, JustifyRight
                                                  , 255, 255, 255, 1, 1, 1);
            else if (logic->CPUPlayerEnabled == 3)
                visuals->DrawTextOntoScreenBuffer("Fast(New)", visuals->Font[0], 60, 215-15+3-10, JustifyRight
                                                  , 255, 255, 255, 1, 1, 1);
            else if (logic->CPUPlayerEnabled == 4)
                visuals->DrawTextOntoScreenBuffer("Turbo!(New)", visuals->Font[0], 60, 215-15+3-10, JustifyRight
                                                  , 255, 255, 255, 1, 1, 1);
        }

        visuals->DrawTextOntoScreenBuffer("Delayed Auto Shift:"
                                          , visuals->Font[0], 60, 215+45-15+3-10, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (logic->DelayAutoShift == 0)
            visuals->DrawTextOntoScreenBuffer("Original", visuals->Font[0], 60, 215+45-15+3-10, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->DelayAutoShift == 1)
            visuals->DrawTextOntoScreenBuffer("Slow Shift", visuals->Font[0], 60, 215+45-15+3-10, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->DelayAutoShift == 2)
            visuals->DrawTextOntoScreenBuffer("Fast Shift", visuals->Font[0], 60, 215+45-15+3-10, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("[UP] Action:", visuals->Font[0]
                                          , 60, 215+45-10+45-13, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (logic->PressingUPAction == 0)
            visuals->DrawTextOntoScreenBuffer("None", visuals->Font[0]
                                              , 60, 215+45-10+45-13, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->PressingUPAction == 1)
            visuals->DrawTextOntoScreenBuffer("Quick Drop", visuals->Font[0]
                                              , 60, 215+45-10+45-13, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->PressingUPAction == 2)
            visuals->DrawTextOntoScreenBuffer("Smart Rotate", visuals->Font[0]
                                              , 60, 215+45-10+45-13, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->PressingUPAction == 3)
            visuals->DrawTextOntoScreenBuffer("Drop & Drag", visuals->Font[0]
                                              , 60, 215+45-10+45-13, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 256-10+42, JustifyCenter
                                          , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Playing Game Speed:", visuals->Font[0]
                                          , 60, 325-15+3+16, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (logic->PlayingGameFrameLock == 33)
            visuals->DrawTextOntoScreenBuffer("Normal", visuals->Font[0], 60, 325-15+3+16, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->PlayingGameFrameLock == 25)
            visuals->DrawTextOntoScreenBuffer("Fast", visuals->Font[0], 60, 325-15+3+16, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->PlayingGameFrameLock == 10)
            visuals->DrawTextOntoScreenBuffer("Turbo!", visuals->Font[0], 60, 325-15+3+16, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 344-6, JustifyCenter
                                          , 255, 255, 255, 90, 90, 90);

        if (input->JoystickDeviceOne != NULL || input->JoystickDeviceTwo != NULL || input->JoystickDeviceThree != NULL)
        {
            visuals->DrawTextOntoScreenBuffer("Press [F1] On Keyboard To Setup Joystick(s)"
                                              , visuals->Font[1]
                                              , 0, 374, JustifyCenter, 255, 255, 255, 1, 1, 1);
        }
        else
        {
            if (JoystickFlash < 130)
            {
                visuals->DrawTextOntoScreenBuffer("* Game Supports USB Joysticks - Go Grab Some & Party! *"
                                                  , visuals->Font[1]
                                                  , 0, 374, JustifyCenter, 255, 255, 255, 1, 1, 1);
            }
            else if ( JoystickFlash > 160 && JoystickFlash < (160+130) )
            {
                visuals->DrawTextOntoScreenBuffer("* Requires Restart Of Game After Plugged In *"
                                                  , visuals->Font[1]
                                                  , 0, 374, JustifyCenter, 255, 255, 255, 1, 1, 1);
            }
        }

        visuals->DrawTextOntoScreenBuffer("Press [F2] On Keyboard To Setup Keyboard", visuals->Font[1]
                                          , 0, 399, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 404-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        if (input->KeyboardSetupProcess > KeyboardSetupNotStarted)
        {
            visuals->ClearScreenBufferWithColor(0.0f, 0.0f, 0.0f, 1.0f);

            if (input->KeyboardSetupProcess == KeyboardSetupPressOne)
                visuals->DrawTextOntoScreenBuffer("Press key on keyboard for [ROTATE1] now!"
                                                  ,visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);

            else if (input->KeyboardSetupProcess == KeyboardSetupPressTwo)
                visuals->DrawTextOntoScreenBuffer("Press key on keyboard for [ROTATE2] now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);

            else if (input->KeyboardSetupProcess == KeyboardSetupPressUP)
                visuals->DrawTextOntoScreenBuffer("Press key on keyboard for [UP] now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);

            else if (input->KeyboardSetupProcess == KeyboardSetupPressRIGHT)
                visuals->DrawTextOntoScreenBuffer("Press key on keyboard for [RIGHT] now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);

            else if (input->KeyboardSetupProcess == KeyboardSetupPressDOWN)
                visuals->DrawTextOntoScreenBuffer("Press key on keyboard for [DOWN] now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);

            else if (input->KeyboardSetupProcess == KeyboardSetupPressLEFT)
                visuals->DrawTextOntoScreenBuffer("Press key on keyboard for [LEFT] now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);

            else if (input->KeyboardSetupProcess == KeyboardSetupPressPause)
                visuals->DrawTextOntoScreenBuffer("Press key on keyboard for [Pause] now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);

            visuals->DrawTextOntoScreenBuffer("Press [F2] key on keyboard to quit setup."
                                              , visuals->Font[0]
                                              , 0, 380, JustifyCenter, 200, 200, 200, 1, 1, 1);

            visuals->DrawTextOntoScreenBuffer("(Will setup keyboard with default mappings.)"
                                              , visuals->Font[1]
                                              , 0, 410, JustifyCenter, 200, 200, 200, 1, 1, 1);
        }

        if (input->JoystickSetupProcess > JoySetupNotStarted)
        {
            visuals->ClearScreenBufferWithColor(0.0f, 0.0f, 0.0f, 1.0f);

            if (input->JoystickSetupProcess == Joy1SetupPressUP)
                visuals->DrawTextOntoScreenBuffer("Press [UP] on joystick #1 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy1SetupPressDOWN)
                visuals->DrawTextOntoScreenBuffer("Press [DOWN] on joystick #1 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy1SetupPressLEFT)
                visuals->DrawTextOntoScreenBuffer("Press [LEFT] on joystick #1 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy1SetupPressRIGHT)
                visuals->DrawTextOntoScreenBuffer("Press [RIGHT] on joystick #1 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy1SetupPressBUTTONOne)
                visuals->DrawTextOntoScreenBuffer("Press [BUTTON1] on joystick #1 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy1SetupPressBUTTONTwo)
                visuals->DrawTextOntoScreenBuffer("Press [BUTTON2] on joystick #1 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy2SetupPressUP)
                visuals->DrawTextOntoScreenBuffer("Press [UP] on joystick #2 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy2SetupPressDOWN)
                visuals->DrawTextOntoScreenBuffer("Press [DOWN] on joystick #2 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy2SetupPressLEFT)
                visuals->DrawTextOntoScreenBuffer("Press [LEFT] on joystick #2 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy2SetupPressRIGHT)
                visuals->DrawTextOntoScreenBuffer("Press [RIGHT] on joystick #2 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy2SetupPressBUTTONOne)
                visuals->DrawTextOntoScreenBuffer("Press [BUTTON1] on joystick #2 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy2SetupPressBUTTONTwo)
                visuals->DrawTextOntoScreenBuffer("Press [BUTTON2] on joystick #2 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy3SetupPressUP)
                visuals->DrawTextOntoScreenBuffer("Press [UP] on joystick #3 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy3SetupPressDOWN)
                visuals->DrawTextOntoScreenBuffer("Press [DOWN] on joystick #3 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy3SetupPressLEFT)
                visuals->DrawTextOntoScreenBuffer("Press [LEFT] on joystick #3 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy3SetupPressRIGHT)
                visuals->DrawTextOntoScreenBuffer("Press [RIGHT] on joystick #3 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy3SetupPressBUTTONOne)
                visuals->DrawTextOntoScreenBuffer("Press [BUTTON1] on joystick #3 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy3SetupPressBUTTONTwo)
                visuals->DrawTextOntoScreenBuffer("Press [BUTTON2] on joystick #3 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);

            visuals->DrawTextOntoScreenBuffer("Press [F1] key on keyboard to quit setup.", visuals->Font[0]
                                              , 0, 380, JustifyCenter, 200, 200, 200, 1, 1, 1);

            visuals->DrawTextOntoScreenBuffer("(Will setup joystick(s) with default mappings.)", visuals->Font[1]
                                              , 0, 410, JustifyCenter, 200, 200, 200, 1, 1, 1);
        }
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        if (logic->UseOldAI == 0)  logic->UseOldAI = 1;

        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = TitleScreen;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayHowToPlayScreen(void)
{
const char* keyName;

    if (ScreenTransitionStatus == FadeAll)
    {
        interface->CreateButton(1009, 0, 454);

        ScreenTransitionStatus = FadeIn;
    }

    if (ScreenIsDirty == true)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[2].ScreenX = 320;
        visuals->Sprites[2].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(2);

        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = 200;
        visuals->DrawSpriteOntoScreenBuffer(0);

        visuals->Sprites[4].ScreenX = 320;

        if (input->UserDefinedKeyButtonOne == -1 && input->UserDefinedKeyButtonTwo == -1)
            visuals->Sprites[4].ScreenY = 240;
        else
            visuals->Sprites[4].ScreenY = 160-1000;

        visuals->DrawSpriteOntoScreenBuffer(4);

        if (input->UserDefinedKeyButtonOne != -1 && input->UserDefinedKeyButtonTwo != -1)
        {
            visuals->Sprites[0].ScreenX = 320;
            visuals->Sprites[0].ScreenY = 240;
            visuals->Sprites[0].Transparency = 100;
            visuals->DrawSpriteOntoScreenBuffer(0);
        }

        visuals->DrawTextOntoScreenBuffer("H O W   T O   P L A Y:", visuals->Font[0]
                                          , 0, 1, JustifyCenter, 255, 255, 1, 90, 90, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 7-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 404-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        if (input->UserDefinedKeyButtonOne != -1 && input->UserDefinedKeyButtonTwo != -1)
        {
            visuals->DrawTextOntoScreenBuffer("CUSTOM CONTROL", visuals->Font[4]
                                              , 0, 50, JustifyCenter, 255, 255, 255, 90, 90, 1);

            keyName = SDL_GetKeyName(input->UserDefinedKeyUP);
            strcpy(visuals->VariableText, "[ ");
            strcat(visuals->VariableText, keyName);
            strcat(visuals->VariableText, " ] = Special UP Action");
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[3]
                                              , 0, 50+70, JustifyCenter, 255, 255, 255, 0, 0, 0);

            keyName = SDL_GetKeyName(input->UserDefinedKeyRIGHT);
            strcpy(visuals->VariableText, "[ ");
            strcat(visuals->VariableText, keyName);
            strcat(visuals->VariableText, " ] = Move Piece Right");
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[3]
                                              , 0, 50+70+(1*40), JustifyCenter, 255, 255, 255, 0, 0, 0);

            keyName = SDL_GetKeyName(input->UserDefinedKeyDOWN);
            strcpy(visuals->VariableText, "[ ");
            strcat(visuals->VariableText, keyName);
            strcat(visuals->VariableText, " ] = Move Piece Down");
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[3]
                                              , 0, 50+70+(2*40), JustifyCenter, 255, 255, 255, 0, 0, 0);

            keyName = SDL_GetKeyName(input->UserDefinedKeyLEFT);
            strcpy(visuals->VariableText, "[ ");
            strcat(visuals->VariableText, keyName);
            strcat(visuals->VariableText, " ] = Move Piece Left");
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[3]
                                              , 0, 50+70+(3*40), JustifyCenter, 255, 255, 255, 0, 0, 0);

            keyName = SDL_GetKeyName(input->UserDefinedKeyPause);
            strcpy(visuals->VariableText, "[ ");
            strcat(visuals->VariableText, keyName);
            strcat(visuals->VariableText, " ] = Pause");
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[3]
                                              , 0, 50+70+(4*40), JustifyCenter, 255, 255, 255, 0, 0, 0);

            keyName = SDL_GetKeyName(input->UserDefinedKeyButtonOne);
            strcpy(visuals->VariableText, "[ ");
            strcat(visuals->VariableText, keyName);
            strcat(visuals->VariableText, " ] = Rotate Clockwise");
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[3]
                                              , 0, 50+70+(5*40), JustifyCenter, 255, 255, 255, 0, 0, 0);

            keyName = SDL_GetKeyName(input->UserDefinedKeyButtonTwo);
            strcpy(visuals->VariableText, "[ ");
            strcat(visuals->VariableText, keyName);
            strcat(visuals->VariableText, " ] = Rotate Counter Clockwise");
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[3]
                                              , 0, 50+70+(6*40), JustifyCenter, 255, 255, 255, 0, 0, 0);
        }
        else
        {
            visuals->DrawTextOntoScreenBuffer("You can change the keyboard controls in [Options]", visuals->Font[1]
                                             , 0, 360, JustifyCenter, 255, 255, 255, 0, 0, 0);

        }
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = TitleScreen;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayHighScoresScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        interface->CreateButton(1009, 0, 454);

        interface->CreateArrowSet(0, 65);

        ScreenTransitionStatus = FadeIn;
    }

    if (interface->ArrowSetArrowSelectedByPlayer != -1)
    {
        if (interface->ArrowSetArrowSelectedByPlayer == 0)
        {
            if (logic->GameMode > 0)  logic->GameMode-=1;
            else  logic->GameMode = StoryMode;

            visuals->ClearTextCache();
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 0.5)
        {
            if (logic->GameMode < StoryMode)  logic->GameMode+=1;
            else  logic->GameMode = 0;

            visuals->ClearTextCache();
        }

        interface->ArrowSetArrowSelectedByPlayer = -1;
    }

    if (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_c)
    {
        data->ClearHighScores();
        ScreenIsDirty = true;
        audio->PlayDigitalSoundFX(1, 0);
    }

    if (ScreenIsDirty == true)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[2].ScreenX = 320;
        visuals->Sprites[2].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(2);

        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = 200;
        visuals->DrawSpriteOntoScreenBuffer(0);

        visuals->DrawTextOntoScreenBuffer("H I G H   S C O R E S:", visuals->Font[0]
                                          , 0, 1, JustifyCenter, 255, 255, 1, 90, 90, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 7-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        interface->DisplayAllArrowSetsOntoScreenBuffer();

        if (logic->GameMode == OriginalMode)
            visuals->DrawTextOntoScreenBuffer("Original Mode", visuals->Font[0]
                                              , 0, 65-15+3, JustifyCenter
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == TimeAttack30Mode)
            visuals->DrawTextOntoScreenBuffer("Time Attack 30 Mode", visuals->Font[0]
                                              , 0, 65-15+3, JustifyCenter
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == TimeAttack60Mode)
            visuals->DrawTextOntoScreenBuffer("Time Attack 60 Mode", visuals->Font[0]
                                              , 0, 65-15+3, JustifyCenter
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == TimeAttack120Mode)
            visuals->DrawTextOntoScreenBuffer("Time Attack 120 Mode", visuals->Font[0]
                                              , 0, 65-15+3, JustifyCenter
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == TwentyLineChallengeMode)
            visuals->DrawTextOntoScreenBuffer("20 Line Challenge Mode", visuals->Font[0]
                                              , 0, 65-15+3, JustifyCenter
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == CrisisMode)
            visuals->DrawTextOntoScreenBuffer("Crisis+Mode", visuals->Font[0]
                                              , 0, 65-15+3, JustifyCenter
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == StoryMode)
            visuals->DrawTextOntoScreenBuffer("Story+Mode", visuals->Font[0]
                                              , 0, 65-15+3, JustifyCenter
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Name:", visuals->Font[1],
                                          30, 88, JustifyLeft, 150, 150, 150, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Level:", visuals->Font[1],
                                          420-30, 88, JustifyLeft, 150, 150, 150, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Score:", visuals->Font[1],
                                          510-50, 88, JustifyLeft, 150, 150, 150, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("1.", visuals->Font[1], 1, 112+4, JustifyLeft, 150, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("2.", visuals->Font[1], 1, 142+4, JustifyLeft, 150, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("3.", visuals->Font[1], 1, 172+4, JustifyLeft, 150, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("4.", visuals->Font[1], 1, 202+4, JustifyLeft, 150, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("5.", visuals->Font[1], 1, 232+4, JustifyLeft, 150, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("6.", visuals->Font[1], 1, 263+4, JustifyLeft, 150, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("7.", visuals->Font[1], 1, 292+4, JustifyLeft, 150, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("8.", visuals->Font[1], 1, 322+4, JustifyLeft, 150, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("9.", visuals->Font[1], 1, 352+4, JustifyLeft, 150, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("10.", visuals->Font[1], 1, 382+4, JustifyLeft, 150, 150, 150, 1, 1, 1);

        int rankY = 112;
        int offsetY = 30;

        for (Uint8 index = 0; index < 10; index++)
        {
            Uint8 greenBlueColorValue = 255;

            for (int playerIndex = 0; playerIndex < NumberOfPlayers; playerIndex++)
            {
                if (data->PlayerWithHighestScore == playerIndex
                    && data->HighScoresScore[logic->GameMode][index] == logic->PlayerData[playerIndex].Score
                    && data->HighScoresLevel[logic->GameMode][index] == logic->PlayerData[playerIndex].Level)
                {
                    greenBlueColorValue = 0;
                }
            }

            visuals->DrawTextOntoScreenBuffer(data->HighScoresName[logic->GameMode][index], visuals->Font[0]
                                              , 30, rankY, JustifyLeft, 255, greenBlueColorValue, greenBlueColorValue, 1, 1, 1);

            if ( (logic->GameMode == CrisisMode || logic->GameMode == StoryMode) && data->HighScoresLevel[logic->GameMode][index] == 10 )
            {
                visuals->DrawTextOntoScreenBuffer("Won!", visuals->Font[0], 420-30, rankY
                                                  , JustifyLeft, 255, greenBlueColorValue, greenBlueColorValue, 1, 1, 1);
            }
            else
            {
                sprintf(visuals->VariableText, "%d", data->HighScoresLevel[logic->GameMode][index]);
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], 420-30, rankY
                                                  , JustifyLeft, 255, greenBlueColorValue, greenBlueColorValue, 1, 1, 1);
            }

            #ifdef _WIN32
                sprintf(visuals->VariableText, "%I64u", data->HighScoresScore[logic->GameMode][index]);
            #else
                sprintf(visuals->VariableText, "%lu", data->HighScoresScore[logic->GameMode][index]);
            #endif

            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], 510-50, rankY, JustifyLeft
                                              , 255, greenBlueColorValue, greenBlueColorValue, 1, 1, 1);

            rankY += offsetY;
        }

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 404-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = TitleScreen;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayAboutScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        ReviewScale = 1.0f;
        ReviewShowDelay = 500;

        for (int index = 1100; index < (1100+visuals->TotalNumberOfLoadedStaffTexts); index++)
        {
            if (index == 1100)
            {
                visuals->Sprites[1100].ScreenX = 446+40;
                visuals->Sprites[1100].ScreenY = 491;
            }
            else if (index == 1101)
            {
                visuals->Sprites[1101].ScreenX = 320;
                visuals->Sprites[1101].ScreenY = 510;
            }
            else if (index == 1102)
            {
                visuals->Sprites[1102].ScreenX = 320;
                visuals->Sprites[1102].ScreenY = 540;
            }
            else if ( index < (1100 + visuals->TotalNumberOfLoadedStaffTexts-1) )
            {
                if (visuals->Sprites[index-1].BlueHue == 255
                 && visuals->Sprites[index].BlueHue == 0)
                {
                    visuals->Sprites[index].ScreenX = 320;
                    visuals->Sprites[index].ScreenY = visuals->Sprites[index-1].ScreenY+110;
                }
                else
                {
                    visuals->Sprites[index].ScreenX = 320;
                    visuals->Sprites[index].ScreenY = visuals->Sprites[index-1].ScreenY+35;
                }
            }
            else if (index == 1100 + visuals->TotalNumberOfLoadedStaffTexts-1)
            {
                visuals->Sprites[index].ScreenX = 320;
                visuals->Sprites[index].ScreenY = visuals->Sprites[index-2].ScreenY+250+30;
            }
        }

        visuals->Sprites[13].ScreenY = (visuals->Sprites[1100+visuals->TotalNumberOfLoadedStaffTexts-1].ScreenY + 240 + 50);
        visuals->Sprites[14].ScreenY = (visuals->Sprites[1100+visuals->TotalNumberOfLoadedStaffTexts-1].ScreenY + 240 + 50 + 320 + 50);
        visuals->Sprites[15].ScreenY = (visuals->Sprites[1100+visuals->TotalNumberOfLoadedStaffTexts-1].ScreenY + 240 + 50 + 320 + 50 + 385);

        ScreenTransitionStatus = FadeIn;
    }

    if ( input->KeyOnKeyboardPressedByUser == SDLK_SPACE
        || input->KeyOnKeyboardPressedByUser == SDLK_RETURN
        || input->MouseButtonPressed[0] == true
        || input->JoystickButtonOne[Any] == ON)
    {
        ScreenTransitionStatus = FadeOut;
        input->DelayAllUserInput = 60;
        audio->PlayDigitalSoundFX(0, 0);
    }

    int skip = 1;
    if (input->JoystickDirectionVertical[Keyboard] == UP)  skip = 13;

    for (int index = 1100; index < (1100+visuals->TotalNumberOfLoadedStaffTexts); index++)
    {
        visuals->Sprites[index].ScreenY-=skip;
    }

    visuals->Sprites[13].ScreenY-=skip;
    visuals->Sprites[14].ScreenY-=skip;

    if (visuals->Sprites[15].ScreenY > 240)
    {
        visuals->Sprites[15].ScreenY-=skip;
    }
    else if (ReviewShowDelay > 0)
    {
        ReviewShowDelay-=1;
    }
    else
    {
        ReviewScale-=0.01;
    }

    if (ReviewScale < 0.0)  ScreenTransitionStatus = FadeOut;

//    if (ScreenIsDirty == true)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 0);

        visuals->Sprites[2].ScreenX = 320;
        visuals->Sprites[2].ScreenY = 240;
        visuals->Sprites[2].Transparency = 100;
        visuals->DrawSpriteOntoScreenBuffer(2);

        visuals->Sprites[13].ScreenX = 320;
        visuals->DrawSpriteOntoScreenBuffer(13);

        visuals->Sprites[14].ScreenX = 320;
        visuals->DrawSpriteOntoScreenBuffer(14);

        visuals->Sprites[15].ScreenX = 320;
        visuals->Sprites[15].ScaleX = ReviewScale;
        visuals->Sprites[15].ScaleY = ReviewScale;
        visuals->DrawSpriteOntoScreenBuffer(15);

        for (  int index = 1100; index < ( (1100 + visuals->TotalNumberOfLoadedStaffTexts) ); index++  )
        {
            if (visuals->Sprites[index].ScreenY > -40 && visuals->Sprites[index].ScreenY < 640+40)  visuals->DrawSpriteOntoScreenBuffer(index);
        }

        ScreenIsDirty = true;
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        visuals->Sprites[2].Transparency = 255;

        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = TitleScreen;

        if (logic->Won == true)
        {
            if (data->PlayerRankOnGameOver < 10)
            {
                if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickOne
                         || logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickTwo
                         || logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickThree)
                    ScreenToDisplay = NameInputJoystickScreen;
                else if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == Keyboard)
                    ScreenToDisplay = NameInputKeyboardScreen;
                else if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == Mouse)
                    ScreenToDisplay = NameInputMouseScreen;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayPlayingGameScreen(void)
{
const char* keyName;

    if (ScreenTransitionStatus == FadeAll)
    {
        logic->SetupForNewGame();

        visuals->FrameLock = logic->PlayingGameFrameLock;

        ScreenTransitionStatus = FadeIn;
    }

    logic->RunTetriGameEngine();

    for (logic->Player = 0; logic->Player < NumberOfPlayers; logic->Player++)
    {
        if (logic->PlayerData[logic->Player].PlayerStatus != FlashingCompletedLines
            && logic->PlayerData[logic->Player].PlayerStatus != ClearingCompletedLines)
        {
            logic->DeletePieceFromPlayfieldMemory(Current);
            logic->AddPieceToPlayfieldMemory(DropShadow);
            logic->AddPieceToPlayfieldMemory(Current);
        }
    }

    if (logic->PlayersCanJoin == true)
    {
        for (int player = 0; player < NumberOfPlayers; player++)
        {
            if (input->JoystickButtonOne[logic->PlayerData[player].PlayerInput] == ON
                && logic->PlayerData[player].PlayerStatus == GameOver)  logic->PlayerData[player].PlayerStatus = NewPieceDropping;
        }

        for (int player = 0; player < NumberOfPlayers; player++)
        {
            if (logic->PlayerData[player].PlayerInput == Mouse && logic->PlayerData[player].PlayerStatus == GameOver)
            {
                if (   input->MouseButtonPressed[0] == true
                && (  input->MouseX > ( logic->PlayerData[player].PlayersPlayfieldScreenX-(156/2) )  )
                && (  input->MouseX < ( logic->PlayerData[player].PlayersPlayfieldScreenX+(156/2) )  )
                && (  input->MouseY > ( logic->PlayerData[player].PlayersPlayfieldScreenY-(458/2) )  )
                && (  input->MouseY < ( logic->PlayerData[player].PlayersPlayfieldScreenY+(458/2) )  )   )
                {
                    logic->PlayerData[player].PlayerStatus = NewPieceDropping;
                }
            }
        }
    }

//    if (ScreenIsDirty == true)
    {
        visuals->Sprites[100+logic->SelectedBackground].ScreenX = 320;
        visuals->Sprites[100+logic->SelectedBackground].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(100+logic->SelectedBackground);

        if (logic->PlayerData[0].PlayerStatus != GameOver)
        {
            visuals->Sprites[80].ScreenX = logic->PlayerData[0].PlayersPlayfieldScreenX;
            visuals->Sprites[80].ScreenY = logic->PlayerData[0].PlayersPlayfieldScreenY;
            visuals->Sprites[80].Transparency = 150;
            visuals->Sprites[80].RedHue = 255;
            visuals->Sprites[80].GreenHue = 0;
            visuals->Sprites[80].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(80);
        }

        if (logic->PlayerData[1].PlayerStatus != GameOver)
        {
            visuals->Sprites[80].ScreenX = logic->PlayerData[1].PlayersPlayfieldScreenX;
            visuals->Sprites[80].ScreenY = logic->PlayerData[1].PlayersPlayfieldScreenY;
            visuals->Sprites[80].Transparency = 150;
            visuals->Sprites[80].RedHue = 255;
            visuals->Sprites[80].GreenHue = 255;
            visuals->Sprites[80].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(80);
        }

        if (logic->PlayerData[2].PlayerStatus != GameOver)
        {
            visuals->Sprites[80].ScreenX = logic->PlayerData[2].PlayersPlayfieldScreenX;
            visuals->Sprites[80].ScreenY = logic->PlayerData[2].PlayersPlayfieldScreenY;
            visuals->Sprites[80].Transparency = 150;
            visuals->Sprites[80].RedHue = 0;
            visuals->Sprites[80].GreenHue = 255;
            visuals->Sprites[80].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(80);
        }

        if (logic->PlayerData[3].PlayerStatus != GameOver)
        {
            visuals->Sprites[80].ScreenX = logic->PlayerData[3].PlayersPlayfieldScreenX;
            visuals->Sprites[80].ScreenY = logic->PlayerData[3].PlayersPlayfieldScreenY;
            visuals->Sprites[80].Transparency = 150;
            visuals->Sprites[80].RedHue = 0;
            visuals->Sprites[80].GreenHue = 0;
            visuals->Sprites[80].BlueHue = 255;
            visuals->DrawSpriteOntoScreenBuffer(80);
        }

        float mouseScreenX = -999;
        float mouseScreenY = -999;
        int mousePlayfieldY = -999;
        for (int player = 0; player < NumberOfPlayers; player++)
        {
            float boxScreenX = logic->PlayerData[player].PlayersPlayfieldScreenX-57-(2*13);
            float boxScreenY = logic->PlayerData[player].PlayersPlayfieldScreenY-212;

            for (int y = 0; y < 26; y++)
            {
                for (int x = 0; x < 12; x++)
                {
                    if (logic->PlayerData[player].Playfield[x][y] == 1)
                    {
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenX = boxScreenX;
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenY = boxScreenY;
                        visuals->Sprites[201 + (10*logic->TileSet)].Transparency = 70;
                        visuals->DrawSpriteOntoScreenBuffer(201 + (10*logic->TileSet));
                    }
                    else if (logic->PlayerData[player].Playfield[x][y] > 10
                             && logic->PlayerData[player].Playfield[x][y] < 20)
                    {
                        int spriteIndex = 200 + (10*logic->TileSet);

                        visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].ScreenX = boxScreenX;
                        visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].ScreenY = boxScreenY;
                        visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].Transparency = 255;
                        visuals->DrawSpriteOntoScreenBuffer(spriteIndex-9+logic->PlayerData[player].Playfield[x][y]);

                    }
                    else if (logic->PlayerData[player].Playfield[x][y] > 20
                             && logic->PlayerData[player].Playfield[x][y] < 30)
                    {
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenX = boxScreenX;
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenY = boxScreenY;
                        visuals->Sprites[201 + (10*logic->TileSet)].Transparency = 255;
                        visuals->DrawSpriteOntoScreenBuffer(201 + (10*logic->TileSet));
                    }

                    if (logic->PlayerData[player].PlayerInput == Mouse && logic->PlayerData[player].PlayerStatus == PieceFalling)
                    {
                        if (  input->MouseX >= ( boxScreenX-(13/2) ) && input->MouseX <= ( boxScreenX+(13/2) )
                        && input->MouseY >= ( boxScreenY-(18/2) ) && input->MouseY <= ( boxScreenY+(18/2) )  )
                        {
                            mouseScreenX = boxScreenX-13;
                            mouseScreenY = boxScreenY;
                            mousePlayfieldY = y;
                        }
                    }

                    boxScreenX+=13;
                }

                boxScreenX = logic->PlayerData[player].PlayersPlayfieldScreenX-57-(2*13);
                boxScreenY+=18;
            }

            if (logic->PlayerData[player].PlayerStatus != GameOver)
            {
                if (logic->BlockAttackTransparency[player] > 0)
                {
                    visuals->Sprites[81].ScreenX = logic->PlayerData[player].PlayersPlayfieldScreenX;
                    visuals->Sprites[81].ScreenY = logic->PlayerData[player].PlayersPlayfieldScreenY;
                    visuals->Sprites[81].Transparency = logic->BlockAttackTransparency[player];
                    visuals->DrawSpriteOntoScreenBuffer(81);
                }
            }

            if (logic->GameMode == TimeAttack30Mode || logic->GameMode == TimeAttack60Mode || logic->GameMode == TimeAttack120Mode)
            {
                Uint32 taTimer = logic->TimeAttackTimer / 200;
                sprintf(visuals->VariableText, "%d", taTimer);
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[6], logic->PlayerData[player].PlayersPlayfieldScreenX
                                                  , 440+5, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            }
            else if (logic->GameMode == TwentyLineChallengeMode)
            {
                sprintf(visuals->VariableText, "%d", logic->PlayerData[player].TwentyLineCounter);
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[6], logic->PlayerData[player].PlayersPlayfieldScreenX
                                                  , 440+5, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            }
        }

        for (int player = 0; player < 4; player++)
        {
            if (logic->PlayerData[player].PlayerInput == Mouse && logic->PlayerData[player].PlayerStatus == PieceFalling)
            {
                int box = 0;
                for (int y = 0; y < 4; y++)
                {
                    for (int x = 0; x < 4; x++)
                    {
                        box++;
                        if (logic->PieceData[logic->PlayerData[player].Piece][logic->PlayerData[player].PieceRotation] [box] == 1)
                        {
                            visuals->Sprites[201 + (10*logic->TileSet)+logic->PlayerData[player].Piece].ScreenX = mouseScreenX;
                            visuals->Sprites[201 + (10*logic->TileSet)+logic->PlayerData[player].Piece].ScreenY = mouseScreenY;
                            visuals->Sprites[201 + (10*logic->TileSet)+logic->PlayerData[player].Piece].Transparency = 128;
                            visuals->DrawSpriteOntoScreenBuffer(201 + (10*logic->TileSet)+logic->PlayerData[player].Piece);

                            visuals->Sprites[201 + (10*logic->TileSet)+logic->PlayerData[player].Piece].Transparency = 255;
                        }

                        mouseScreenX+=13;
                    }

                    mouseScreenX-=(4*13);
                    mouseScreenY+=18;
                }

                if (mousePlayfieldY >= logic->PlayerData[player].PiecePlayfieldY)
                {
                    visuals->DrawTextOntoScreenBuffer("MOVE", visuals->Font[2], mouseScreenX+(13*1), mouseScreenY-(18*2), JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
                }
                else  visuals->DrawTextOntoScreenBuffer("ROTATE", visuals->Font[2], mouseScreenX+(13*1), mouseScreenY-(18*2), JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            }
        }

        if ( logic->GameMode == CrisisMode)
        {
            if (logic->PlayerData[0].Level > 6)
            {
                visuals->Sprites[155].ScreenX = logic->PlayerData[0].PlayersPlayfieldScreenX;
                visuals->Sprites[155].ScreenY = logic->PlayerData[0].PlayersPlayfieldScreenY;
                visuals->Sprites[155].Transparency = 155;
                visuals->DrawSpriteOntoScreenBuffer(155);
            }

            if (logic->PlayerData[1].Level > 6)
            {
                visuals->Sprites[155].ScreenX = logic->PlayerData[1].PlayersPlayfieldScreenX;
                visuals->Sprites[155].ScreenY = logic->PlayerData[1].PlayersPlayfieldScreenY;
                visuals->Sprites[155].Transparency = 155;
                visuals->DrawSpriteOntoScreenBuffer(155);
            }

            if (logic->PlayerData[2].Level > 6)
            {
                visuals->Sprites[155].ScreenX = logic->PlayerData[2].PlayersPlayfieldScreenX;
                visuals->Sprites[155].ScreenY = logic->PlayerData[2].PlayersPlayfieldScreenY;
                visuals->Sprites[155].Transparency = 155;
                visuals->DrawSpriteOntoScreenBuffer(155);
            }

            if (logic->PlayerData[3].Level > 6)
            {
                visuals->Sprites[155].ScreenX = logic->PlayerData[3].PlayersPlayfieldScreenX;
                visuals->Sprites[155].ScreenY = logic->PlayerData[3].PlayersPlayfieldScreenY;
                visuals->Sprites[155].Transparency = 155;
                visuals->DrawSpriteOntoScreenBuffer(155);
            }
        }

        if (logic->PlayerData[0].PlayerStatus == GameOver)
        {
            visuals->Sprites[80].ScreenX = logic->PlayerData[0].PlayersPlayfieldScreenX;
            visuals->Sprites[80].ScreenY = logic->PlayerData[0].PlayersPlayfieldScreenY;
            visuals->Sprites[80].Transparency = 200;
            visuals->Sprites[80].RedHue = 255;
            visuals->Sprites[80].GreenHue = 0;
            visuals->Sprites[80].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(80);

            visuals->DrawTextOntoScreenBuffer("GAME OVER"
                                      , visuals->Font[1], logic->PlayerData[0].PlayersPlayfieldScreenX
                                      , 240, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
        }

        if (logic->PlayerData[1].PlayerStatus == GameOver)
        {
            visuals->Sprites[80].ScreenX = logic->PlayerData[1].PlayersPlayfieldScreenX;
            visuals->Sprites[80].ScreenY = logic->PlayerData[1].PlayersPlayfieldScreenY;
            visuals->Sprites[80].Transparency = 200;
            visuals->Sprites[80].RedHue = 255;
            visuals->Sprites[80].GreenHue = 255;
            visuals->Sprites[80].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(80);

            visuals->DrawTextOntoScreenBuffer("GAME OVER"
                                      , visuals->Font[1], logic->PlayerData[1].PlayersPlayfieldScreenX
                                      , 240, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
        }

        if (logic->PlayerData[2].PlayerStatus == GameOver)
        {
            visuals->Sprites[80].ScreenX = logic->PlayerData[2].PlayersPlayfieldScreenX;
            visuals->Sprites[80].ScreenY = logic->PlayerData[2].PlayersPlayfieldScreenY;
            visuals->Sprites[80].Transparency = 200;
            visuals->Sprites[80].RedHue = 0;
            visuals->Sprites[80].GreenHue = 255;
            visuals->Sprites[80].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(80);

            visuals->DrawTextOntoScreenBuffer("GAME OVER"
                                      , visuals->Font[1], logic->PlayerData[2].PlayersPlayfieldScreenX
                                      , 240, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
        }

        if (logic->PlayerData[3].PlayerStatus == GameOver)
        {
            visuals->Sprites[80].ScreenX = logic->PlayerData[3].PlayersPlayfieldScreenX;
            visuals->Sprites[80].ScreenY = logic->PlayerData[3].PlayersPlayfieldScreenY;
            visuals->Sprites[80].Transparency = 200;
            visuals->Sprites[80].RedHue = 0;
            visuals->Sprites[80].GreenHue = 0;
            visuals->Sprites[80].BlueHue = 255;
            visuals->DrawSpriteOntoScreenBuffer(80);

            visuals->DrawTextOntoScreenBuffer("GAME OVER"
                                      , visuals->Font[1], logic->PlayerData[3].PlayersPlayfieldScreenX
                                      , 240, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
        }

        for (int player = 0; player < NumberOfPlayers; player++)
        {
            if (logic->PlayersCanJoin == true)
            {

                if ( (logic->PlayerData[player].PlayerInput == JoystickOne && input->JoystickDeviceOne != NULL)
                    || (logic->PlayerData[player].PlayerInput == JoystickTwo && input->JoystickDeviceTwo != NULL)
                    || (logic->PlayerData[player].PlayerInput == JoystickThree && input->JoystickDeviceThree != NULL)
                    || (logic->PlayerData[player].PlayerInput == Keyboard) || (logic->PlayerData[player].PlayerInput == Mouse) )
                {
                    logic->JoinInTimer++;
                    if (logic->JoinInTimer > 30)
                        logic->JoinInTimer = 0;

                    if (logic->JoinInTimer < 15)
                        if (logic->PlayerData[player].PlayerStatus == GameOver)
                            visuals->DrawTextOntoScreenBuffer("JOIN IN!", visuals->Font[1], logic->PlayerData[player].PlayersPlayfieldScreenX, 270,
                                                              JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
                }
            }
        }

        for (int player = 0; player < NumberOfPlayers; player++)
        {
            #ifdef _WIN32
                sprintf(visuals->VariableText, "%I64u", logic->PlayerData[player].Score);
            #else
                sprintf(visuals->VariableText, "%lu", logic->PlayerData[player].Score);
            #endif

            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[5]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX, 9-2+1
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

            sprintf(visuals->VariableText, "%d", logic->PlayerData[player].Lines);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[6]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX-59, 62-1+6
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

            sprintf(visuals->VariableText, "%d", logic->PlayerData[player].Level);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[6]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX+59, 62-1+6
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
        }

        for (int player = 0; player < NumberOfPlayers; player++)
        {
            if (logic->PlayerData[player].PlayerInput == Keyboard)
                visuals->DrawTextOntoScreenBuffer("Keyboard"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            else if (logic->PlayerData[player].PlayerInput == JoystickOne)
                visuals->DrawTextOntoScreenBuffer("Joystick #1"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            else if (logic->PlayerData[player].PlayerInput == JoystickTwo)
                visuals->DrawTextOntoScreenBuffer("Joystick #2"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            else if (logic->PlayerData[player].PlayerInput == JoystickThree)
                visuals->DrawTextOntoScreenBuffer("Joystick #3"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            else if (logic->PlayerData[player].PlayerInput == CPU)
            {
                visuals->DrawTextOntoScreenBuffer("C.P.U."
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            }
            else if (logic->PlayerData[player].PlayerInput == Mouse)
                visuals->DrawTextOntoScreenBuffer("Mouse"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);

            if (logic->GameMode == CrisisMode && logic->Crisis7BGMPlayed == true)
            {
                for (int player = 0; player < NumberOfPlayers; player++)
                {
                    visuals->Sprites[155].ScreenX = logic->PlayerData[player].PlayersPlayfieldScreenX;
                    visuals->Sprites[155].ScreenY = 240;
                    visuals->Sprites[155].Transparency = 0.10f;
                    visuals->DrawSpriteOntoScreenBuffer(155);
                }
            }
        }

        logic->HumanStillAlive = false;
        if (logic->PlayerData[0].PlayerInput != CPU && logic->PlayerData[0].PlayerStatus != GameOver)  logic->HumanStillAlive = true;
        if (logic->PlayerData[1].PlayerInput != CPU && logic->PlayerData[1].PlayerStatus != GameOver)  logic->HumanStillAlive = true;
        if (logic->PlayerData[2].PlayerInput != CPU && logic->PlayerData[2].PlayerStatus != GameOver)  logic->HumanStillAlive = true;
        if (logic->PlayerData[3].PlayerInput != CPU && logic->PlayerData[3].PlayerStatus != GameOver)  logic->HumanStillAlive = true;

        if (logic->HumanStillAlive == false && input->DEBUG == 0)
        {
            logic->ContinueWatchingTimer++;
            if (logic->ContinueWatchingTimer > 20)
                logic->ContinueWatchingTimer = 0;

            if (logic->ContinueWatchingTimer < 15)
                visuals->DrawTextOntoScreenBuffer("Continue Watching Or Press [Esc] On Keyboard To Exit!"
                                                , visuals->Font[1], 320, 260, JustifyCenter, 255, 255, 255, 1, 1, 1);
        }

        if (logic->PAUSEgame == true && input->DEBUG != 1)
        {
            visuals->Sprites[0].ScreenX = 320;
            visuals->Sprites[0].ScreenY = 240;
            visuals->Sprites[0].Transparency = 150;
            visuals->DrawSpriteOntoScreenBuffer(0);

            visuals->DrawTextOntoScreenBuffer("G A M E   P A U S E D", visuals->Font[0]
                                              , 0, 225, JustifyCenter, 255, 255, 255, 90, 90, 90);

            if (input->UserDefinedKeyPause == -1)
                visuals->DrawTextOntoScreenBuffer("(Press [SpaceBar] On Keyboard To Continue!)", visuals->Font[1]
                                                  , 0, 265, JustifyCenter, 255, 255, 255, 90, 90, 90);
            else
            {
                strcpy(visuals->VariableText, "(Press [");
                keyName = SDL_GetKeyName(input->UserDefinedKeyPause);
                strcat(visuals->VariableText, keyName);
                strcat(visuals->VariableText, "] On Keyboard To Continue!)");
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[1]
                                                  , 0, 265, JustifyCenter, 255, 255, 255, 90, 90, 90);
            }
        }

        ScreenIsDirty = true;
    }

    for (logic->Player = 0; logic->Player < NumberOfPlayers; logic->Player++)
    {
        if (logic->PlayerData[logic->Player].PlayerStatus != FlashingCompletedLines
            && logic->PlayerData[logic->Player].PlayerStatus != ClearingCompletedLines)
        {
            logic->DeletePieceFromPlayfieldMemory(Current);
            logic->DeletePieceFromPlayfieldMemory(DropShadow);
        }
    }

    if (logic->PlayerData[0].PlayerStatus == GameOver && logic->PlayerData[1].PlayerStatus == GameOver
        && logic->PlayerData[2].PlayerStatus == GameOver && logic->PlayerData[3].PlayerStatus == GameOver)
        ScreenTransitionStatus = FadeOut;

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;

        ScreenToDisplay = HighScoresScreen;

        data->CheckForNewHighScore();

        if (data->PlayerRankOnGameOver < 10 && logic->GameForfeit == false)
        {
            if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickOne
                     || logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickTwo
                     || logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickThree)
                ScreenToDisplay = NameInputJoystickScreen;
            else if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == Keyboard)
                ScreenToDisplay = NameInputKeyboardScreen;
            else if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == Mouse)
                ScreenToDisplay = NameInputMouseScreen;
        }

        audio->PlayMusic(0, -1);

        if (logic->Won == true)
        {
            ScreenToDisplay = AboutScreen;
            audio->PlayMusic(25, -1);
        }

        SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );

        visuals->FrameLock = 16;

        visuals->ClearTextCache();
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayPlayingStoryGameScreen(void) // NOT FINISHED...
{
const char* keyName;

    if (ScreenTransitionStatus == FadeAll)
    {
        logic->SetupForNewGame();

        visuals->FrameLock = logic->PlayingGameFrameLock;

        audio->PlayMusic(26 , -1);

        ScreenTransitionStatus = FadeIn;
    }

    logic->RunTetriGameEngine();

    for (logic->Player = 0; logic->Player < NumberOfPlayers; logic->Player++)
    {
        if (logic->PlayerData[logic->Player].PlayerStatus != FlashingCompletedLines
            && logic->PlayerData[logic->Player].PlayerStatus != ClearingCompletedLines)
        {
            logic->DeletePieceFromPlayfieldMemory(Current);
            logic->AddPieceToPlayfieldMemory(DropShadow);
            logic->AddPieceToPlayfieldMemory(Current);
        }
    }

//    if (ScreenIsDirty == true)
    {
        visuals->Sprites[108].ScreenX = 320;
        visuals->Sprites[108].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(108);

        if (logic->PlayerData[1].PlayerStatus != GameOver)
        {
            visuals->Sprites[79].ScreenX = logic->PlayerData[1].PlayersPlayfieldScreenX;
            visuals->Sprites[79].ScreenY = logic->PlayerData[1].PlayersPlayfieldScreenY;
            visuals->Sprites[79].Transparency = 150;
            visuals->Sprites[79].RedHue = 255;
            visuals->Sprites[79].GreenHue = 255;
            visuals->Sprites[79].BlueHue = 255;
            visuals->DrawSpriteOntoScreenBuffer(79);
        }

        float mouseScreenX = -999;
        float mouseScreenY = -999;
        int mousePlayfieldY = -999;
        for (int player = 1; player < 2; player++)
        {
            float boxScreenX = ( logic->PlayerData[player].PlayersPlayfieldScreenX-57-(2*13) - 86 );
            float boxScreenY = logic->PlayerData[player].PlayersPlayfieldScreenY-212;

            for (int y = 0; y < 26; y++)
            {
                for (int x = 0; x < 12; x++)
                {
                    if (logic->PlayerData[player].Playfield[x][y] == 1)
                    {
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenX = boxScreenX;
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenY = boxScreenY;
                        visuals->Sprites[201 + (10*logic->TileSet)].ScaleX = 2;
                        visuals->Sprites[201 + (10*logic->TileSet)].Transparency = 70;
                        visuals->DrawSpriteOntoScreenBuffer(201 + (10*logic->TileSet));
                    }
                    else if (logic->PlayerData[player].Playfield[x][y] > 10
                             && logic->PlayerData[player].Playfield[x][y] < 20)
                    {
                        int spriteIndex = 200 + (10*logic->TileSet);

                        visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].ScreenX = boxScreenX;
                        visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].ScreenY = boxScreenY;
                        visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].ScaleX = 2;
                        visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].Transparency = 255;
                        visuals->DrawSpriteOntoScreenBuffer(spriteIndex-9+logic->PlayerData[player].Playfield[x][y]);
                    }
                    else if (logic->PlayerData[player].Playfield[x][y] > 20
                             && logic->PlayerData[player].Playfield[x][y] < 30)
                    {
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenX = boxScreenX;
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenY = boxScreenY;
                        visuals->Sprites[201 + (10*logic->TileSet)].ScaleX = 2;
                        visuals->Sprites[201 + (10*logic->TileSet)].Transparency = 255;
                        visuals->DrawSpriteOntoScreenBuffer(201 + (10*logic->TileSet));
                    }

                    if (logic->PlayerData[player].PlayerInput == Mouse && logic->PlayerData[player].PlayerStatus == PieceFalling)
                    {
                        if (  input->MouseX >= ( boxScreenX-(13/2) ) && input->MouseX <= ( boxScreenX+(13/2) )
                        && input->MouseY >= ( boxScreenY-(18/2) ) && input->MouseY <= ( boxScreenY+(18/2) )  )
                        {
                            mouseScreenX = boxScreenX-13;
                            mouseScreenY = boxScreenY;
                            mousePlayfieldY = y;
                        }
                    }

                    boxScreenX+=(13*2);
                }

                boxScreenX = ( logic->PlayerData[player].PlayersPlayfieldScreenX-57-(2*13) - 86 );
                boxScreenY+=18;
            }

            if (logic->PlayerData[player].PlayerStatus != GameOver)
            {
                if (logic->BlockAttackTransparency[player] > 0)
                {
                    visuals->Sprites[81].ScreenX = logic->PlayerData[player].PlayersPlayfieldScreenX;
                    visuals->Sprites[81].ScreenY = logic->PlayerData[player].PlayersPlayfieldScreenY;
                    visuals->Sprites[81].Transparency = logic->BlockAttackTransparency[player];
                    visuals->DrawSpriteOntoScreenBuffer(81);
                }
            }
        }

        for (int player = 1; player < 2; player++)
        {
            if (logic->PlayerData[player].PlayerInput == Mouse && logic->PlayerData[player].PlayerStatus == PieceFalling)
            {
                int box = 0;
                for (int y = 0; y < 4; y++)
                {
                    for (int x = 0; x < 4; x++)
                    {
                        box++;
                        if (logic->PieceData[logic->PlayerData[player].Piece][logic->PlayerData[player].PieceRotation] [box] == 1)
                        {
                            visuals->Sprites[201 + (10*logic->TileSet)+logic->PlayerData[player].Piece].ScreenX = mouseScreenX;
                            visuals->Sprites[201 + (10*logic->TileSet)+logic->PlayerData[player].Piece].ScreenY = mouseScreenY;
                            visuals->Sprites[201 + (10*logic->TileSet)+logic->PlayerData[player].Piece].Transparency = 128;
                            visuals->DrawSpriteOntoScreenBuffer(201 + (10*logic->TileSet)+logic->PlayerData[player].Piece);

                            visuals->Sprites[201 + (10*logic->TileSet)+logic->PlayerData[player].Piece].Transparency = 255;
                        }

                        mouseScreenX+=13;
                    }

                    mouseScreenX-=(4*13);
                    mouseScreenY+=18;
                }

                if (mousePlayfieldY >= logic->PlayerData[player].PiecePlayfieldY)
                {
                    visuals->DrawTextOntoScreenBuffer("MOVE", visuals->Font[2], mouseScreenX+(13*1), mouseScreenY-(18*2), JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
                }
                else  visuals->DrawTextOntoScreenBuffer("ROTATE", visuals->Font[2], mouseScreenX+(13*1), mouseScreenY-(18*2), JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            }
        }

        if (logic->PlayerData[1].PlayerStatus == GameOver)
        {
            visuals->Sprites[79].ScreenX = logic->PlayerData[1].PlayersPlayfieldScreenX;
            visuals->Sprites[79].ScreenY = logic->PlayerData[1].PlayersPlayfieldScreenY;
            visuals->Sprites[79].Transparency = 200;
            visuals->Sprites[79].RedHue = 255;
            visuals->Sprites[79].GreenHue = 255;
            visuals->Sprites[79].BlueHue = 255;
            visuals->DrawSpriteOntoScreenBuffer(79);

            visuals->DrawTextOntoScreenBuffer("GAME OVER"
                                      , visuals->Font[1], logic->PlayerData[1].PlayersPlayfieldScreenX
                                      , 240, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
        }

        for (int player = 1; player < 2; player++)
        {
            #ifdef _WIN32
                sprintf(visuals->VariableText, "%I64u", logic->PlayerData[player].Score);
            #else
                sprintf(visuals->VariableText, "%lu", logic->PlayerData[player].Score);
            #endif

            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[5]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX, 9-2+1
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

            sprintf(visuals->VariableText, "%d", logic->PlayerData[player].Lines);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[6]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX-59-58, 62-1+6
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

            sprintf(visuals->VariableText, "%d", logic->PlayerData[player].Level);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[6]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX+59+58, 62-1+6
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
        }

        for (int player = 1; player < 2; player++)
        {
            if (logic->PlayerData[player].PlayerInput == Keyboard)
                visuals->DrawTextOntoScreenBuffer("Keyboard"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            else if (logic->PlayerData[player].PlayerInput == JoystickOne)
                visuals->DrawTextOntoScreenBuffer("Joystick #1"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            else if (logic->PlayerData[player].PlayerInput == JoystickTwo)
                visuals->DrawTextOntoScreenBuffer("Joystick #2"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            else if (logic->PlayerData[player].PlayerInput == JoystickThree)
                visuals->DrawTextOntoScreenBuffer("Joystick #3"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            else if (logic->PlayerData[player].PlayerInput == CPU)
            {
                visuals->DrawTextOntoScreenBuffer("C.P.U."
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            }
            else if (logic->PlayerData[player].PlayerInput == Mouse)
                visuals->DrawTextOntoScreenBuffer("Mouse"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
/*
            if (logic->GameMode == CrisisMode && logic->Crisis7BGMPlayed == true)
            {
                for (int player = 0; player < NumberOfPlayers; player++)
                {
                    visuals->Sprites[155].ScreenX = logic->PlayerData[player].PlayersPlayfieldScreenX;
                    visuals->Sprites[155].ScreenY = 240;
                    visuals->Sprites[155].Transparency = 0.10f;
                    visuals->DrawSpriteOntoScreenBuffer(155);
                }
            }*/
        }

        if (logic->PAUSEgame == true && input->DEBUG != 1)
        {
            visuals->Sprites[0].ScreenX = 320;
            visuals->Sprites[0].ScreenY = 240;
            visuals->Sprites[0].Transparency = 150;
            visuals->DrawSpriteOntoScreenBuffer(0);

            visuals->DrawTextOntoScreenBuffer("G A M E   P A U S E D", visuals->Font[0]
                                              , 0, 225, JustifyCenter, 255, 255, 255, 90, 90, 90);

            if (input->UserDefinedKeyPause == -1)
                visuals->DrawTextOntoScreenBuffer("(Press [SpaceBar] On Keyboard To Continue!)", visuals->Font[1]
                                                  , 0, 265, JustifyCenter, 255, 255, 255, 90, 90, 90);
            else
            {
                strcpy(visuals->VariableText, "(Press [");
                keyName = SDL_GetKeyName(input->UserDefinedKeyPause);
                strcat(visuals->VariableText, keyName);
                strcat(visuals->VariableText, "] On Keyboard To Continue!)");
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[1]
                                                  , 0, 265, JustifyCenter, 255, 255, 255, 90, 90, 90);
            }
        }

        ScreenIsDirty = true;
    }

    for (logic->Player = 1; logic->Player < 2; logic->Player++)
    {
        if (logic->PlayerData[logic->Player].PlayerStatus != FlashingCompletedLines
            && logic->PlayerData[logic->Player].PlayerStatus != ClearingCompletedLines)
        {
            logic->DeletePieceFromPlayfieldMemory(Current);
            logic->DeletePieceFromPlayfieldMemory(DropShadow);
        }
    }

    if (logic->PlayerData[1].PlayerStatus == GameOver)  ScreenTransitionStatus = FadeOut;

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        visuals->Sprites[201 + (10*logic->TileSet)].ScaleX = 1;

        int spriteIndex;
        for (  spriteIndex = ( 200 + (10*logic->TileSet) ); spriteIndex < ( 200 + (10*logic->TileSet) + 7 ); spriteIndex++  )
        {
            visuals->Sprites[spriteIndex].ScaleX = 1;
        }

        ScreenTransitionStatus = FadeAll;

        ScreenToDisplay = HighScoresScreen;

        data->CheckForNewHighScore();

        if (data->PlayerRankOnGameOver < 10 && logic->GameForfeit == false)
        {
            if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickOne
                     || logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickTwo
                     || logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickThree)
                ScreenToDisplay = NameInputJoystickScreen;
            else if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == Keyboard)
                ScreenToDisplay = NameInputKeyboardScreen;
            else if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == Mouse)
                ScreenToDisplay = NameInputMouseScreen;
        }

        audio->PlayMusic(0, -1);

        if (logic->Won == true)
        {
            ScreenToDisplay = AboutScreen;
            audio->PlayMusic(28, -1);
        }

        SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );

        visuals->FrameLock = 16;

        visuals->ClearTextCache();
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayNameInputKeyboardScreen(void)
{
bool lastKeyWasNotAcceptable = false;

    if (ScreenTransitionStatus == FadeAll)
    {
        interface->CreateButton(1008, 0, 454);

        ScreenTransitionStatus = FadeIn;

        data->NameInputArayIndex = 0;
    }

    if (input->KeyOnKeyboardPressedByUser == SDLK_RETURN)
    {
        if (data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] == '\0')
            data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] = ' ';

        audio->PlayDigitalSoundFX(0, 0);
    }
    else if (input->KeyOnKeyboardPressedByUser == SDLK_BACKSPACE)
    {
        if (data->NameInputArayIndex == 19)
        {
            data->NameInputArayIndex--;
            data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
        }

        data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
        if (data->NameInputArayIndex > 0)  data->NameInputArayIndex--;
        data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';

        input->DelayAllUserInput = 20;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = true;
    }
    else if (input->KeyOnKeyboardPressedByUser == SDLK_SPACE)
    {
        if (data->NameInputArayIndex < 18)
            data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex]
                = ' ';

        if (data->NameInputArayIndex < 19)  data->NameInputArayIndex++;

        input->DelayAllUserInput = 20;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = true;
    }
    else if (input->KeyOnKeyboardPressedByUser > -1)
    {
        if (data->NameInputArayIndex < 18)
        {
            if (input->KeyOnKeyboardPressedByUser >= SDLK_a && input->KeyOnKeyboardPressedByUser <= SDLK_z)
            {
                if (input->ShiftKeyPressed == false)  data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = input->KeyOnKeyboardPressedByUser;
                else  data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = ( char(input->KeyOnKeyboardPressedByUser) - 32 );
            }
            else if (input->KeyOnKeyboardPressedByUser >= SDLK_0 && input->KeyOnKeyboardPressedByUser <= SDLK_9)
            {
                if (input->ShiftKeyPressed == false)  data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = input->KeyOnKeyboardPressedByUser;
            }
            else if (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_EQUALS)
                data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '+';
            else
                lastKeyWasNotAcceptable = true;
        }

        if (lastKeyWasNotAcceptable == false && data->NameInputArayIndex < 19)
        {
            data->NameInputArayIndex++;

            input->DelayAllUserInput = 20;

            audio->PlayDigitalSoundFX(0, 0);
            ScreenIsDirty = true;
        }
    }

    if (ScreenIsDirty == true)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[2].ScreenX = 320;
        visuals->Sprites[2].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(2);

        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = 200;
        visuals->DrawSpriteOntoScreenBuffer(0);

        visuals->DrawTextOntoScreenBuffer("H I G H   S C O R E   N A M E   I N P U T:", visuals->Font[0]
                                          , 0, 1, JustifyCenter, 255, 255, 1, 90, 90, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 7-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        visuals->DrawTextOntoScreenBuffer("You have achieved a new high score!", visuals->Font[0],
                                          0, 80, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Enter your name using the keyboard:", visuals->Font[0],
                                          0, 125, JustifyCenter, 255, 255, 255, 1, 1, 1);

        if (data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] != (char)NULL)
            visuals->DrawTextOntoScreenBuffer(data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver],
                                              visuals->Font[0], 0, 207, JustifyCenter, 255, 255, 0, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Press [Backspace] key to erase.", visuals->Font[0],
                                          0, 305, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Press [Enter] key when finished.", visuals->Font[0],
                                          0, 345, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 404-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = HighScoresScreen;

        if (data->NameInputArayIndex == 0)  data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = ' ';

        logic->Won = false;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayNameInputMouseScreen(void)
{
char temp[256];
char charToAccept[256];
bool lastKeyWasNotAcceptable = false;

    if (ScreenTransitionStatus == FadeAll)
    {
        interface->CreateButton(1008, 0, 454);

        ScreenTransitionStatus = FadeIn;

        data->NameInputArayIndex = 0;

        int screenY = 250;
        int screenX = 63;
        int index = -1;
        for (int y = 0; y < 5; y++)
        {
            for (int x = 0; x < 13; x++)
            {
                index++;

                interface->CreateIcon(1020+index, screenX, screenY);

                screenX+=(26+17);
            }

            screenX = 63;
            screenY+=(36+2);
        }
    }

    if (interface->IconSelectedByPlayer == 64)
    {
        if (data->NameInputArayIndex == 19)
        {
            data->NameInputArayIndex--;
            data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
        }

        data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
        if (data->NameInputArayIndex > 0)  data->NameInputArayIndex--;
        data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';

        input->DelayAllUserInput = 20;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = true;
    }
    else if (interface->IconSelectedByPlayer == 63)
    {
        if (data->NameInputArayIndex < 18)
            data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = ' ';

        if (data->NameInputArayIndex < 19)  data->NameInputArayIndex++;

        input->DelayAllUserInput = 20;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = true;
    }
    else if (interface->IconSelectedByPlayer > -1 && interface->IconSelectedByPlayer < 63)
    {
        if (data->NameInputArayIndex < 18)
        {

            if (interface->IconSelectedByPlayer == 62)
            {
                data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '+';
                data->NameInputArayIndex++;
                lastKeyWasNotAcceptable = true;
            }
            else if (interface->IconSelectedByPlayer > -1 && interface->IconSelectedByPlayer < 13+13)
            {
                sprintf( temp, "%c", (65+interface->IconSelectedByPlayer) );
                strcpy(charToAccept, temp);
                data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = charToAccept[0];
                data->NameInputArayIndex++;
                lastKeyWasNotAcceptable = true;
            }
            else if (interface->IconSelectedByPlayer > 13+13-1 && interface->IconSelectedByPlayer < 13+13+13+13)
            {
                sprintf( temp, "%c", (97+(interface->IconSelectedByPlayer-26)) );
                strcpy(charToAccept, temp);
                data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = charToAccept[0];
                data->NameInputArayIndex++;
                lastKeyWasNotAcceptable = true;
            }
            else if (interface->IconSelectedByPlayer > 13+13+13+13-1 && interface->IconSelectedByPlayer < 13+13+13+13+11)
            {
                sprintf( temp, "%c", (48+(interface->IconSelectedByPlayer-26-26)) );
                strcpy(charToAccept, temp);
                data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = charToAccept[0];
                data->NameInputArayIndex++;
                lastKeyWasNotAcceptable = true;
            }
        }

        if (lastKeyWasNotAcceptable == false && data->NameInputArayIndex < 19)
        {
            data->NameInputArayIndex++;

            input->DelayAllUserInput = 20;

            audio->PlayDigitalSoundFX(0, 0);
            ScreenIsDirty = true;
        }
    }

    if (ScreenIsDirty == true)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[2].ScreenX = 320;
        visuals->Sprites[2].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(2);

        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = 200;
        visuals->DrawSpriteOntoScreenBuffer(0);

        visuals->DrawTextOntoScreenBuffer("H I G H   S C O R E   N A M E   I N P U T:", visuals->Font[0]
                                          , 0, 1, JustifyCenter, 255, 255, 1, 90, 90, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 7-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        visuals->DrawTextOntoScreenBuffer("You have achieved a new high score!", visuals->Font[0],
                                          0, 80, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Enter your name using the mouse:", visuals->Font[0],
                                          0, 125, JustifyCenter, 255, 255, 255, 1, 1, 1);



        if (data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] != (char)NULL)
            visuals->DrawTextOntoScreenBuffer(data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver],
                                              visuals->Font[0], 0, 207-25, JustifyCenter, 255, 255, 0, 1, 1, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 404-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = HighScoresScreen;

        if (data->NameInputArayIndex == 0)  data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = ' ';

        logic->Won = false;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayNameInputJoystickScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        interface->CreateButton(1008, 0, 454);

        ScreenTransitionStatus = FadeIn;

        data->NameInputJoyCharX = 0;
        data->NameInputJoyCharY = 0;
        data->NameInputJoyChar = 'A';
        data->NewHighScoreRegistered = false;

        data->NameInputArayIndex = 0;
    }

    if (input->JoystickButtonOne[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == ON)
    {
        if (input->JoystickButtonOnePressed[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == false)
        {
            if (data->NameInputJoyChar != (char)NULL)
            {
                if (data->NameInputJoyCharY == 5)
                {
                    if (data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] == '\0')
                        data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] = ' ';

                    ScreenTransitionStatus = FadeOut;
                }
                else if (data->NameInputJoyChar == '<')
                {
                    if (data->NameInputArayIndex == 19)
                    {
                        data->NameInputArayIndex--;
                        data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
                    }

                    data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
                    if (data->NameInputArayIndex > 0)  data->NameInputArayIndex--;
                    data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
                }
                else if (data->NameInputJoyChar == '_')
                {
                    if (data->NameInputArayIndex < 18)
                        data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex]
                            = ' ';

                    if (data->NameInputArayIndex < 19)  data->NameInputArayIndex++;
                }
                else if (data->NameInputJoyCharY != 5)
                {
                    if (data->NameInputArayIndex < 18)
                        data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex]
                            = data->NameInputJoyChar;

                    if (data->NameInputArayIndex < 19)  data->NameInputArayIndex++;
                }

                audio->PlayDigitalSoundFX(0, 0);
                ScreenIsDirty = true;
                input->JoystickButtonOnePressed[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] = true;
            }
        }
    }
    else if (input->JoystickButtonTwo[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == ON)
    {
        if (input->JoystickButtonTwoPressed[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == false)
        {
            if (data->NameInputArayIndex == 19)
            {
                data->NameInputArayIndex--;
                data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
            }

            data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
            if (data->NameInputArayIndex > 0)  data->NameInputArayIndex--;
            data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';

            audio->PlayDigitalSoundFX(0, 0);
            ScreenIsDirty = true;
            input->JoystickButtonTwoPressed[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] = true;
        }
    }
    else
    {
        input->JoystickButtonOnePressed[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] = false;
        input->JoystickButtonTwoPressed[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] = false;
    }

    if (input->JoystickDirectionVertical[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == UP)
    {
        if (data->NameInputJoyCharY > 0)  data->NameInputJoyCharY--;
        else  data->NameInputJoyCharY = 5;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = true;
        input->DelayAllUserInput = 6;
    }
    else if (input->JoystickDirectionVertical[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == DOWN)
    {
        if (data->NameInputJoyCharY < 5)  data->NameInputJoyCharY++;
        else  data->NameInputJoyCharY = 0;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = true;
        input->DelayAllUserInput = 6;
    }
    else if (input->JoystickDirectionHorizontal[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == LEFT)
    {
        if (data->NameInputJoyCharX > 0)  data->NameInputJoyCharX--;
        else  data->NameInputJoyCharX = 12;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = true;
        input->DelayAllUserInput = 6;
    }
    else if (input->JoystickDirectionHorizontal[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == RIGHT)
    {
        if (data->NameInputJoyCharX < 12)  data->NameInputJoyCharX++;
        else  data->NameInputJoyCharX = 0;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = true;
        input->DelayAllUserInput = 6;
    }

    if (data->NameInputJoyCharY == 0)
    {
        data->NameInputJoyChar = char('A'+data->NameInputJoyCharX);
    }
    else if (data->NameInputJoyCharY == 1)
    {
        data->NameInputJoyChar = char('N'+data->NameInputJoyCharX);
    }
    else if (data->NameInputJoyCharY == 2)
    {
        data->NameInputJoyChar = char('a'+data->NameInputJoyCharX);
    }
    else if (data->NameInputJoyCharY == 3)
    {
        data->NameInputJoyChar = char('n'+data->NameInputJoyCharX);
    }
    else if (data->NameInputJoyCharY == 4 && data->NameInputJoyCharX < 10)
    {
        data->NameInputJoyChar = char('0'+data->NameInputJoyCharX);
    }
    else if (data->NameInputJoyCharY == 4 && data->NameInputJoyCharX == 10)
    {
        data->NameInputJoyChar = '+';
    }
    else if (data->NameInputJoyCharY == 4 && data->NameInputJoyCharX == 11)
    {
        data->NameInputJoyChar = '_';
    }
    else if (data->NameInputJoyCharY == 4 && data->NameInputJoyCharX == 12)
    {
        data->NameInputJoyChar = '<';
    }
    else if (data->NameInputJoyCharY == 5)
    {
        data->NameInputJoyChar = '/';
    }

    if (ScreenIsDirty == true)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[2].ScreenX = 320;
        visuals->Sprites[2].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(2);

        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = 200;
        visuals->DrawSpriteOntoScreenBuffer(0);

        visuals->DrawTextOntoScreenBuffer("H I G H   S C O R E   N A M E   I N P U T:", visuals->Font[0]
                                          , 0, 1, JustifyCenter, 255, 255, 1, 90, 90, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 7-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        visuals->DrawTextOntoScreenBuffer("You have achieved a new high score!", visuals->Font[0],
                                          0, 80, JustifyCenter, 255, 255, 255, 1, 1, 1);

        if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickOne)
            visuals->DrawTextOntoScreenBuffer("Enter your name using Joystick #1:", visuals->Font[0],
                                              0, 125, JustifyCenter, 255, 255, 255, 1, 1, 1);
        if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickTwo)
            visuals->DrawTextOntoScreenBuffer("Enter your name using Joystick #2:", visuals->Font[0],
                                              0, 125, JustifyCenter, 255, 255, 255, 1, 1, 1);
        if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickThree)
            visuals->DrawTextOntoScreenBuffer("Enter your name using Joystick #3:", visuals->Font[0],
                                              0, 125, JustifyCenter, 255, 255, 255, 1, 1, 1);

        if (data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] != (char)NULL)
            visuals->DrawTextOntoScreenBuffer(data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver],
                                              visuals->Font[0], 0, 190, JustifyCenter, 255, 255, 0, 1, 1, 1);

        int x = 45;
        for (char alphaNumeral = 'A'; alphaNumeral < 'N'; alphaNumeral++)
        {
            sprintf(visuals->VariableText, "%c", alphaNumeral);

            if (alphaNumeral == data->NameInputJoyChar)
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 239, JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
            else
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 239, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            x+=45;
        }

        x = 45;
        for (char alphaNumeral = 'N'; alphaNumeral < '['; alphaNumeral++)
        {
            sprintf(visuals->VariableText, "%c", alphaNumeral);

            if (alphaNumeral == data->NameInputJoyChar)
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 269, JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
            else
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 269, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            x+=45;
        }

        x = 45;
        for (char alphaNumeral = 'a'; alphaNumeral < 'n'; alphaNumeral++)
        {
            sprintf(visuals->VariableText, "%c", alphaNumeral);

            if (alphaNumeral == data->NameInputJoyChar)
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 299, JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
            else
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 299, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            x+=45;
        }

        x = 45;
        for (char alphaNumeral = 'n'; alphaNumeral < '{'; alphaNumeral++)
        {
            sprintf(visuals->VariableText, "%c", alphaNumeral);

            if (alphaNumeral == data->NameInputJoyChar)
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 329, JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
            else
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 329, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            x+=45;
        }

        x = 45;
        for (char alphaNumeral = '0'; alphaNumeral < ':'; alphaNumeral++)
        {
            sprintf(visuals->VariableText, "%c", alphaNumeral);

            if (alphaNumeral == data->NameInputJoyChar)
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 359, JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
            else
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 359, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            x+=45;
        }

        if (data->NameInputJoyChar == '+')
            visuals->DrawTextOntoScreenBuffer("+", visuals->Font[0], x, 359, JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
        else
            visuals->DrawTextOntoScreenBuffer("+", visuals->Font[0], x, 359, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
        x+=45;

        if (data->NameInputJoyChar == '_')
            visuals->DrawTextOntoScreenBuffer("_", visuals->Font[0], x, 359, JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
        else
            visuals->DrawTextOntoScreenBuffer("_", visuals->Font[0], x, 359, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
        x+=45;

        if (data->NameInputJoyChar == '<')
            visuals->DrawTextOntoScreenBuffer("<", visuals->Font[0], x, 359, JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
        else
            visuals->DrawTextOntoScreenBuffer("<", visuals->Font[0], x, 359, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
        x+=45;

        if (data->NameInputJoyCharY == 5)
            visuals->DrawTextOntoScreenBuffer("END", visuals->Font[0], 320, 389, JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
        else
            visuals->DrawTextOntoScreenBuffer("END", visuals->Font[0], 320, 389, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 404-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = HighScoresScreen;

        if (data->NameInputArayIndex == 0)  data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = ' ';

        logic->Won = false;

        input->DelayAllUserInput = 20;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayTestComputerSkillScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        logic->SetupForNewGame();

        visuals->FrameLock = (16*4);

        input->DelayAllUserInput = 20;

        ScreenTransitionStatus = FadeIn;
    }

    logic->RunTetriGameEngine();

    if (input->KeyOnKeyboardPressedByUser == SDLK_t)
    {
        input->DelayAllUserInput = 20;

        visuals->ClearScreenBufferWithColor(0, 0, 0, 0);

        if (logic->DontDisplayTestImages == false)
        {
            logic->DontDisplayTestImages = true;
            visuals->FrameLock = 0;
        }
        else
        {
            logic->DontDisplayTestImages = false;
            visuals->FrameLock = 1;
        }
    }
    else if (input->KeyOnKeyboardPressedByUser == SDLK_MINUS)
    {
        if (logic->Multiplier > -100)  logic->Multiplier-=0.1;

        printf("Trapped X: %f\n", logic->Multiplier);

        logic->SetupForNewGame();

        logic->TotalCPUPlayerLines = 0;
        logic->TotalCPUPlayerLinesLast = 0;
        logic->NumberofCPUGames = 4;
        logic->TotalOneLines = 0;
        logic->TotalTwoLines = 0;
        logic->TotalThreeLines = 0;
        logic->TotalFourLines = 0;

        input->DelayAllUserInput = 5;
    }
    else if (input->KeyOnKeyboardPressedByUser == SDLK_EQUALS)
    {
        if (logic->Multiplier < 100)  logic->Multiplier+=0.1;

        printf("Trapped X: %f\n", logic->Multiplier);

        logic->SetupForNewGame();

        logic->TotalCPUPlayerLines = 0;
        logic->TotalCPUPlayerLinesLast = 0;
        logic->NumberofCPUGames = 4;
        logic->TotalOneLines = 0;
        logic->TotalTwoLines = 0;
        logic->TotalThreeLines = 0;
        logic->TotalFourLines = 0;

        input->DelayAllUserInput = 5;
    }

    for (logic->Player = 0; logic->Player < NumberOfPlayers; logic->Player++)
    {
        if (logic->PlayerData[logic->Player].PlayerStatus != FlashingCompletedLines
            && logic->PlayerData[logic->Player].PlayerStatus != ClearingCompletedLines)
        {
            logic->DeletePieceFromPlayfieldMemory(Current);
            logic->AddPieceToPlayfieldMemory(DropShadow);
            logic->AddPieceToPlayfieldMemory(Current);
        }
    }

//    if (ScreenIsDirty == true)
    {

        if (logic->DontDisplayTestImages == false)
        {
            visuals->ClearScreenBufferWithColor(0, 0, 0, 0);

            if (logic->PlayerData[0].PlayerStatus != GameOver)
            {
                visuals->Sprites[80].ScreenX = logic->PlayerData[0].PlayersPlayfieldScreenX;
                visuals->Sprites[80].ScreenY = logic->PlayerData[0].PlayersPlayfieldScreenY;
                visuals->Sprites[80].RedHue = 255;
                visuals->Sprites[80].GreenHue = 0;
                visuals->Sprites[80].BlueHue = 0;
                visuals->DrawSpriteOntoScreenBuffer(80);
            }

            if (logic->PlayerData[1].PlayerStatus != GameOver)
            {
                visuals->Sprites[80].ScreenX = logic->PlayerData[1].PlayersPlayfieldScreenX;
                visuals->Sprites[80].ScreenY = logic->PlayerData[1].PlayersPlayfieldScreenY;
                visuals->Sprites[80].RedHue = 255;
                visuals->Sprites[80].GreenHue = 255;
                visuals->Sprites[80].BlueHue = 0;
                visuals->DrawSpriteOntoScreenBuffer(80);
            }

            if (logic->PlayerData[2].PlayerStatus != GameOver)
            {
                visuals->Sprites[80].ScreenX = logic->PlayerData[2].PlayersPlayfieldScreenX;
                visuals->Sprites[80].ScreenY = logic->PlayerData[2].PlayersPlayfieldScreenY;
                visuals->Sprites[80].RedHue = 0;
                visuals->Sprites[80].GreenHue = 255;
                visuals->Sprites[80].BlueHue = 0;
                visuals->DrawSpriteOntoScreenBuffer(80);
            }

            if (logic->PlayerData[3].PlayerStatus != GameOver)
            {
                visuals->Sprites[80].ScreenX = logic->PlayerData[3].PlayersPlayfieldScreenX;
                visuals->Sprites[80].ScreenY = logic->PlayerData[3].PlayersPlayfieldScreenY;
                visuals->Sprites[80].RedHue = 0;
                visuals->Sprites[80].GreenHue = 0;
                visuals->Sprites[80].BlueHue = 255;
                visuals->DrawSpriteOntoScreenBuffer(80);
            }

            for (int player = 0; player < NumberOfPlayers; player++)
            {
                float boxScreenX = logic->PlayerData[player].PlayersPlayfieldScreenX-57;
                float boxScreenY = logic->PlayerData[player].PlayersPlayfieldScreenY-212;

                for (int y = 0; y < 26; y++)
                {
                    for (int x = 2; x < 12; x++)
                    {
                        if (logic->PlayerData[player].Playfield[x][y] == 1)
                        {
                            visuals->Sprites[201 + (10*logic->TileSet)].ScreenX = boxScreenX;
                            visuals->Sprites[201 + (10*logic->TileSet)].ScreenY = boxScreenY;
                            visuals->Sprites[201 + (10*logic->TileSet)].Transparency = 70;
                            visuals->DrawSpriteOntoScreenBuffer(201 + (10*logic->TileSet));
                        }
                        else if (logic->PlayerData[player].Playfield[x][y] > 10
                                 && logic->PlayerData[player].Playfield[x][y] < 20)
                        {
                            int spriteIndex = 200 + (10*logic->TileSet);

                            visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].ScreenX = boxScreenX;
                            visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].ScreenY = boxScreenY;
                            visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].Transparency = 255;
                            visuals->DrawSpriteOntoScreenBuffer(spriteIndex-9+logic->PlayerData[player].Playfield[x][y]);

                        }
                        else if (logic->PlayerData[player].Playfield[x][y] > 20
                                 && logic->PlayerData[player].Playfield[x][y] < 30)
                        {
                            visuals->Sprites[201 + (10*logic->TileSet)].ScreenX = boxScreenX;
                            visuals->Sprites[201 + (10*logic->TileSet)].ScreenY = boxScreenY;
                            visuals->Sprites[201 + (10*logic->TileSet)].Transparency = 255;
                            visuals->DrawSpriteOntoScreenBuffer(201 + (10*logic->TileSet));
                        }

                        if (logic->PlayerData[player].PlayfieldAI[x][y] == 999)
                        {
                            visuals->Sprites[200 + (10*logic->TileSet)].ScreenX = boxScreenX;
                            visuals->Sprites[200 + (10*logic->TileSet)].ScreenY = boxScreenY;
                            visuals->DrawSpriteOntoScreenBuffer(200 + (10*logic->TileSet));
                        }

                        boxScreenX+=13;
                    }

                    boxScreenX = logic->PlayerData[player].PlayersPlayfieldScreenX-57;
                    boxScreenY+=18;
                }

                if (logic->GameMode == TimeAttack30Mode || logic->GameMode == TimeAttack60Mode || logic->GameMode == TimeAttack120Mode)
                {
                    Uint32 taTimer = logic->TimeAttackTimer / 200;
                    sprintf(visuals->VariableText, "%d", taTimer);
                    visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                                      , 440, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
                }
                else if (logic->GameMode == TwentyLineChallengeMode)
                {
                    sprintf(visuals->VariableText, "%d", logic->PlayerData[player].TwentyLineCounter);
                    visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                                      , 440, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
                }
            }
/*
            if (logic->PlayerData[0].PlayerStatus == GameOver)
            {
                visuals->Sprites[80].ScreenX = logic->PlayerData[0].PlayersPlayfieldScreenX;
                visuals->Sprites[80].ScreenY = logic->PlayerData[0].PlayersPlayfieldScreenY;
                visuals->Sprites[80].Transparency = 200;
                visuals->Sprites[80].RedHue = 255;
                visuals->Sprites[80].GreenHue = 0;
                visuals->Sprites[80].BlueHue = 0;
                visuals->DrawSpriteOntoScreenBuffer(80);

                visuals->DrawTextOntoScreenBuffer("GAME OVER"
                                          , visuals->Font[1], logic->PlayerData[0].PlayersPlayfieldScreenX
                                          , 240, JustifyCenterOnPoint, 255, 255, 255, 90, 90, 90);
            }

            if (logic->PlayerData[1].PlayerStatus == GameOver)
            {
                visuals->Sprites[80].ScreenX = logic->PlayerData[1].PlayersPlayfieldScreenX;
                visuals->Sprites[80].ScreenY = logic->PlayerData[1].PlayersPlayfieldScreenY;
                visuals->Sprites[80].Transparency = 200;
                visuals->Sprites[80].RedHue = 255;
                visuals->Sprites[80].GreenHue = 255;
                visuals->Sprites[80].BlueHue = 0;
                visuals->DrawSpriteOntoScreenBuffer(80);

                visuals->DrawTextOntoScreenBuffer("GAME OVER"
                                          , visuals->Font[1], logic->PlayerData[1].PlayersPlayfieldScreenX
                                          , 240, JustifyCenterOnPoint, 255, 255, 255, 90, 90, 90);
            }

            if (logic->PlayerData[2].PlayerStatus == GameOver)
            {
                visuals->Sprites[80].ScreenX = logic->PlayerData[2].PlayersPlayfieldScreenX;
                visuals->Sprites[80].ScreenY = logic->PlayerData[2].PlayersPlayfieldScreenY;
                visuals->Sprites[80].Transparency = 200;
                visuals->Sprites[80].RedHue = 0;
                visuals->Sprites[80].GreenHue = 255;
                visuals->Sprites[80].BlueHue = 0;
                visuals->DrawSpriteOntoScreenBuffer(80);

                visuals->DrawTextOntoScreenBuffer("GAME OVER"
                                          , visuals->Font[1], logic->PlayerData[2].PlayersPlayfieldScreenX
                                          , 240, JustifyCenterOnPoint, 255, 255, 255, 90, 90, 90);
            }

            if (logic->PlayerData[3].PlayerStatus == GameOver)
            {
                visuals->Sprites[80].ScreenX = logic->PlayerData[3].PlayersPlayfieldScreenX;
                visuals->Sprites[80].ScreenY = logic->PlayerData[3].PlayersPlayfieldScreenY;
                visuals->Sprites[80].Transparency = 200;
                visuals->Sprites[80].RedHue = 0;
                visuals->Sprites[80].GreenHue = 0;
                visuals->Sprites[80].BlueHue = 255;
                visuals->DrawSpriteOntoScreenBuffer(80);

                visuals->DrawTextOntoScreenBuffer("GAME OVER"
                                          , visuals->Font[1], logic->PlayerData[3].PlayersPlayfieldScreenX
                                          , 240, JustifyCenterOnPoint, 255, 255, 255, 90, 90, 90);
            }
*/
            for (int player = 0; player < NumberOfPlayers; player++)
            {
                if (logic->PlayersCanJoin == true)
                {

                    if ( (logic->PlayerData[player].PlayerInput == JoystickOne && input->JoystickDeviceOne != NULL)
                        || (logic->PlayerData[player].PlayerInput == JoystickTwo && input->JoystickDeviceTwo != NULL)
                        || (logic->PlayerData[player].PlayerInput == JoystickThree && input->JoystickDeviceThree != NULL)
                        || (logic->PlayerData[player].PlayerInput == Keyboard) )
                    {
                        if (logic->PlayerData[player].PlayerStatus == GameOver)
                            visuals->DrawTextOntoScreenBuffer("JOIN IN!", visuals->Font[1], logic->PlayerData[player].PlayersPlayfieldScreenX, 270,
                                                              JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
                    }
                }
            }

            for (int player = 0; player < NumberOfPlayers; player++)
            {
                #ifdef _WIN32
                    sprintf(visuals->VariableText, "%I64u", logic->PlayerData[player].Score);
                #else
                    sprintf(visuals->VariableText, "%lu", logic->PlayerData[player].Score);
                #endif

                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[5]
                                                  , logic->PlayerData[player].PlayersPlayfieldScreenX, 9-2
                                                  , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

                sprintf(visuals->VariableText, "%d", logic->PlayerData[player].Lines);
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[6]
                                                  , logic->PlayerData[player].PlayersPlayfieldScreenX-59, 62-1+6
                                                  , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

                sprintf(visuals->VariableText, "%d", logic->PlayerData[player].Level);
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[6]
                                                  , logic->PlayerData[player].PlayersPlayfieldScreenX+59, 62-1+6
                                                  , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            }

            for (int player = 0; player < NumberOfPlayers; player++)
            {
                if (logic->PlayerData[player].PlayerInput == Keyboard)
                    visuals->DrawTextOntoScreenBuffer("Keyboard"
                                              , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                              , 460, JustifyCenterOnPoint, 255, 255, 255, 90, 90, 90);
                else if (logic->PlayerData[player].PlayerInput == JoystickOne)
                    visuals->DrawTextOntoScreenBuffer("Joystick #1"
                                              , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                              , 460, JustifyCenterOnPoint, 255, 255, 255, 90, 90, 90);
                else if (logic->PlayerData[player].PlayerInput == JoystickTwo)
                    visuals->DrawTextOntoScreenBuffer("Joystick #2"
                                              , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                              , 460, JustifyCenterOnPoint, 255, 255, 255, 90, 90, 90);
                else if (logic->PlayerData[player].PlayerInput == JoystickThree)
                    visuals->DrawTextOntoScreenBuffer("Joystick #3"
                                              , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                              , 460, JustifyCenterOnPoint, 255, 255, 255, 90, 90, 90);
                else if (logic->PlayerData[player].PlayerInput == CPU)
                {
                        visuals->DrawTextOntoScreenBuffer("C.P.U."
                                                  , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                                  , 460, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
                }
                else if (logic->PlayerData[player].PlayerInput == Mouse)
                    visuals->DrawTextOntoScreenBuffer("Mouse"
                                              , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                              , 460, JustifyCenterOnPoint, 255, 255, 255, 90, 90, 90);

                if (logic->GameMode == CrisisMode && logic->Crisis7BGMPlayed == true)
                {
                    for (int player = 0; player < NumberOfPlayers; player++)
                    {
                        visuals->Sprites[155].ScreenX = logic->PlayerData[player].PlayersPlayfieldScreenX;
                        visuals->Sprites[155].ScreenY = 240;
                        visuals->Sprites[155].Transparency = 0.20f;
                        visuals->DrawSpriteOntoScreenBuffer(155);
                    }
                }
            }

            if (logic->PAUSEgame == true && input->DEBUG != 1)
            {
                visuals->Sprites[0].ScreenX = 320;
                visuals->Sprites[0].ScreenY = 240;
                visuals->Sprites[0].Transparency = 0.85;
                visuals->DrawSpriteOntoScreenBuffer(0);

                visuals->DrawTextOntoScreenBuffer("G A M E   P A U S E D", visuals->Font[0]
                                                  , 0, 225, JustifyCenter, 255, 255, 255, 90, 90, 90);

                visuals->DrawTextOntoScreenBuffer("(Press [SpaceBar] On Keyboard To Continue!)", visuals->Font[1]
                                                  , 0, 265, JustifyCenter, 255, 255, 255, 90, 90, 90);
            }

            ScreenIsDirty = true;
        }
    }

    char temp[256];
    if (logic->DontDisplayTestImages == false)
    {
        visuals->DrawTextOntoScreenBuffer("A.I. TEST", visuals->Font[8]
                                          , 0, 90, JustifyCenter, 255, 255, 255, 0, 0, 0);

        strcpy(visuals->VariableText, "Number Of Games: ");
        sprintf(temp, "%d", logic->NumberofCPUGames);
        strcat(visuals->VariableText, temp);
        visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[8], 0, 130
                                          , JustifyCenter, 255, 255, 255, 0, 0, 0);

        strcpy(visuals->VariableText, "Number Of Lines: ");
        logic->TotalCPUPlayerLines = ( logic->TotalOneLines+(2*logic->TotalTwoLines)+(3*logic->TotalThreeLines)+(4*logic->TotalFourLines) );
        sprintf(temp, "%d", logic->TotalCPUPlayerLines);
        strcat(visuals->VariableText, temp);
        visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[8], 0, 150
                                          , JustifyCenter, 255, 255, 255, 0, 0, 0);

        strcpy(visuals->VariableText, "Completed Lines: 1=");
        sprintf(temp, "%d", logic->TotalOneLines);
        strcat(visuals->VariableText, temp);
        strcat(visuals->VariableText, " 2=");
        sprintf(temp, "%d", logic->TotalTwoLines);
        strcat(visuals->VariableText, temp);
        strcat(visuals->VariableText, " 3=");
        sprintf(temp, "%d", logic->TotalThreeLines);
        strcat(visuals->VariableText, temp);
        strcat(visuals->VariableText, " 4=");
        sprintf(temp, "%d", logic->TotalFourLines);
        strcat(visuals->VariableText, temp);
        visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[8], 0, 195
                                          , JustifyCenter, 255, 255, 255, 0, 0, 0);
    }

    logic->TotalCPUPlayerLines = ( logic->TotalOneLines+(2*logic->TotalTwoLines)+(3*logic->TotalThreeLines)+(4*logic->TotalFourLines) );

    if (logic->TotalCPUPlayerLinesLast < logic->TotalCPUPlayerLines || logic->DontDisplayTestImages == false)
    {
        logic->TotalCPUPlayerLinesLast = logic->TotalCPUPlayerLines;

        int averageLinesPerGame = logic->TotalCPUPlayerLines / logic->NumberofCPUGames;
        strcpy(visuals->VariableText, "Average Lines/Game: ");
        sprintf(temp, "%d", averageLinesPerGame);
        strcat(visuals->VariableText, temp);
        visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[8], 0, 170
                                          , JustifyCenter, 255, 255, 255, 0, 0, 0);
    }

    ScreenIsDirty = true;

//    if (logic->GameDisplayChanged == true)
    {
        for (logic->Player = 0; logic->Player < NumberOfPlayers; logic->Player++)
        {
            if (logic->PlayerData[logic->Player].PlayerStatus != FlashingCompletedLines
                && logic->PlayerData[logic->Player].PlayerStatus != ClearingCompletedLines)
            {
                logic->DeletePieceFromPlayfieldMemory(Current);
                logic->DeletePieceFromPlayfieldMemory(DropShadow);
            }
        }

        logic->GameDisplayChanged = false;
    }

    for (logic->Player = 0; logic->Player < NumberOfPlayers; logic->Player++)
    {
        if (logic->PlayerData[logic->Player].PlayerStatus == GameOver)
        {
            logic->NumberofCPUGames++;

            for (int y = 0; y < 26; y++)
                for (int x = 0; x < 15; x++)
                    logic->PlayerData[logic->Player].Playfield[x][y] = 255; // Collision detection value

            for (int y = 2; y < 5; y++)
                for (int x = 5; x < 9; x++)
                    logic->PlayerData[logic->Player].Playfield[x][y] = 0;

            for (int y = 5; y < 24; y++)
                for (int x = 2; x < 12; x++)
                    logic->PlayerData[logic->Player].Playfield[x][y] = 0;

            logic->PlayerData[logic->Player].PiecePlayfieldX = 5;
            logic->PlayerData[logic->Player].PiecePlayfieldY = 0;

            logic->PlayerData[logic->Player].Piece = logic->GetRandomPiece();
            logic->PlayerData[logic->Player].PieceMovementDelay = 0;
            logic->PlayerData[logic->Player].PieceRotation = 1;

            logic->PlayerData[logic->Player].NextPiece = logic->GetRandomPiece();

            logic->PlayerData[logic->Player].PlayerStatus = NewPieceDropping;

            logic->PlayerData[logic->Player].PieceDropTimer = 0;
            logic->PlayerData[logic->Player].TimeToDropPiece = 47;

            logic->PlayerData[logic->Player].PieceRotated1 = false;
            logic->PlayerData[logic->Player].PieceRotated2 = false;

            logic->PlayerData[logic->Player].FlashCompletedLinesTimer = 0;
            logic->PlayerData[logic->Player].ClearCompletedLinesTimer = 0;

            logic->PlayerData[logic->Player].Score = 0;
            logic->PlayerData[logic->Player].DropBonus = 0;
            logic->PlayerData[logic->Player].Level = 0;
            logic->PlayerData[logic->Player].Lines = 0;
        }
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;
    }
}
