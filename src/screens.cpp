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
#include <stdio.h>
#include <cstring>

#include <stdlib.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "screens.h"

#include "input.h"
#include "visuals.h"
#include "interfaces.h"
#include "data.h"
#include "logic.h"
#include "audio.h"

extern Input* input;
extern Visuals* visuals;
extern Interfaces* interfaces;
extern Data* data;
extern Logic* logic;
extern Audio* audio;

//-------------------------------------------------------------------------------------------------
Screens::Screens(void)
{
    ScreenIsDirty = true;
    ScreenToDisplay = SixteenBitSoftScreen;

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
        if (ScreenFadeTransparency > 24)
        {
            ScreenFadeTransparency -= 25;
            ScreenIsDirty = true;
        }
        else if (ScreenFadeTransparency != 0)
        {
            ScreenFadeTransparency = 0;
        }
        else
        {
            ScreenFadeTransparency = 0;
            ScreenTransitionStatus = FadeNone;
        }
    }
    else if (ScreenTransitionStatus == FadeOut)
    {
        if (ScreenFadeTransparency < 231)
        {
            ScreenFadeTransparency += 25;
            ScreenIsDirty = true;
        }
        else if (ScreenFadeTransparency != 255)
        {
            ScreenFadeTransparency = 255;
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

        ScreenIsDirty = true;
    }

    switch(ScreenToDisplay)
    {
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

        case TestComputerSkillScreen:
            DisplayTestComputerSkillScreen();
            break;

        case NameInputKeyboardScreen:
            DisplayNameInputMouseKeyboardScreen();
            break;

        case NameInputJoystickScreen:
            DisplayNameInputJoystickScreen();
            break;

        default:
            break;
    }

    ApplyScreenFadeTransition();

    if (input->DEBUG == true)
    {
        ScreenIsDirty = true;

        if (ScreenToDisplay != PlayingGameScreen)
        {
            char temp[256];
            strcpy(visuals->VariableText, "(");
            sprintf(temp, "%i", input->MouseX);
            strcat(visuals->VariableText, temp);
            strcat(visuals->VariableText, ",");
            sprintf(temp, "%i", input->MouseY);
            strcat(visuals->VariableText, temp);
            strcat(visuals->VariableText, ")");
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[2], 3, 442
                                              , JustifyLeft, 255, 255, 255, 0, 0, 0);

            sprintf(visuals->VariableText, "%d", visuals->AverageFPS);
            strcat(visuals->VariableText, "/60");
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[2], 3, 462
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
void Screens::DisplaySixteenBitSoftScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        ScreenDisplayTimer = 370;
        ScreenTransitionStatus = FadeIn;
    }

    if (input->KeyOnKeyboardPressedByUser == 'D')
    {
        input->DEBUG = !input->DEBUG;
        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(1, 0);
    }

    if (input->KeyOnKeyboardPressedByUser == 'T')
    {
        ScreenToDisplay = TestComputerSkillScreen;
        ScreenTransitionStatus = FadeAll;
        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(1, 0);

        logic->PlayersCanJoin = false;
    }

    if (input->MouseButtonPressed[0] == true
       || input->EnterKeyPressed == true
       || input->SpacebarKeyPressed == true)
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
        visuals->Sprites[1].ScaleX = 4.4;
        visuals->Sprites[1].ScaleY = 6.5;
        visuals->DrawSpriteOntoScreenBuffer(1);

        visuals->DrawTextOntoScreenBuffer("TM", visuals->Font[2]
                                          , 55, 295, JustifyRight, 1, 255, 1, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Bringing back old memories from the 16bit era!", visuals->Font[1]
                                          , 0, 307, JustifyCenter, 1, 255, 1, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("www.16BitSoft.com", visuals->Font[0]
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
        int buttonOffsetY = 39;
        interfaces->CreateButton( 1003, 0, buttonStartY );
        interfaces->CreateButton( 1004, 1, buttonStartY + (buttonOffsetY*1) );
        interfaces->CreateButton( 1005, 2, buttonStartY + (buttonOffsetY*2) );
        interfaces->CreateButton( 1006, 3, buttonStartY + (buttonOffsetY*3) );
        interfaces->CreateButton( 1007, 4, buttonStartY + (buttonOffsetY*4) );
        interfaces->CreateButton( 1008, 5, buttonStartY + (buttonOffsetY*5) );

        interfaces->CreateIcon(156, 0+64, 480-64);

        ScreenTransitionStatus = FadeIn;
    }

    interfaces->ProcessAllButtons();
    interfaces->ProcessAllIcons();

    if (interfaces->IconSelectedByPlayer == 0)
    {
        #ifdef __WIN32__
            system("start https://github.com/SLNTHERO/TetriCrisis_4_110Percent_A.I._Turbo");
        #endif

        #ifdef __linux__
            system("xdg-open https://github.com/SLNTHERO/TetriCrisis_4_110Percent_A.I._Turbo");
        #endif

        interfaces->IconSelectedByPlayer = -1;
    }

    if (ScreenIsDirty == true)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[2].ScreenX = 320;
        visuals->Sprites[2].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(2);

        visuals->DrawTextOntoScreenBuffer("JeZ+Lee's", visuals->Font[2]
                                          , 0, 1, JustifyCenter, 255, 255, 255, 90, 90, 90);

        visuals->Sprites[3].ScreenX = 320;
        visuals->Sprites[3].ScreenY = 100;
        visuals->Sprites[3].Smooth = true;
        visuals->DrawSpriteOntoScreenBuffer(3);

        visuals->DrawTextOntoScreenBuffer("TM", visuals->Font[2]
                                          , 0, -2, JustifyRight, 1, 255, 1, 1, 90, 1);

        visuals->DrawTextOntoScreenBuffer("A.I. 29,000+", visuals->Font[2]
                                          , 585, 80+6, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Line Average", visuals->Font[2]
                                          , 585, 96+6, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

        char temp[256];
        strcpy(visuals->VariableText, "''");
        strcat(visuals->VariableText, data->HighScoresName[logic->GameMode][0]);
        strcat(visuals->VariableText, "'' Scored: ");
		sprintf(temp, "%d", data->HighScoresScore[logic->GameMode][0]);
        strcat(visuals->VariableText, temp);

        visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], 0, 135+16
                                          , JustifyCenter, 255, 255, 0, 90, 90, 0);

        interfaces->DisplayAllButtonsOntoScreenBuffer();
        interfaces->DisplayAllIconsOntoScreenBuffer();

        visuals->Sprites[10].ScreenX = 565;
        visuals->Sprites[10].ScreenY = 418;
        visuals->Sprites[10].Transparency = 155;
        visuals->DrawSpriteOntoScreenBuffer(10);

        visuals->DrawTextOntoScreenBuffer("Retail Version 4.5.6", visuals->Font[2]
                                          , 2, 462, JustifyRight, 255, 255, 255, 90, 90, 90);

        char *copy = new char[2];
        char *reg = new char[2];
        sprintf(copy, "%c", 0xA9);
        sprintf(reg, "%c", 0xAE);
        strcpy(visuals->VariableText, copy);
        strcat(visuals->VariableText, "2020, By 16BitSoft Inc.");
        visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0]
                                          , 0, 420, JustifyCenter, 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("www.16BitSoft.com", visuals->Font[0]
                                          , 0, 447, JustifyCenter, 255, 255, 255, 90, 90, 90);
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;

        if (interfaces->ButtonSelectedByPlayer == 1)  ScreenToDisplay = OptionsScreen;
        else if (interfaces->ButtonSelectedByPlayer == 2)  ScreenToDisplay = HowToPlayScreen;
        else if (interfaces->ButtonSelectedByPlayer == 3)  ScreenToDisplay = HighScoresScreen;
        else if (interfaces->ButtonSelectedByPlayer == 4)  ScreenToDisplay = AboutScreen;
        else if (interfaces->ButtonSelectedByPlayer == 5)  input->EXIT_Game = true;
        else if (interfaces->ButtonSelectedByPlayer == 0)  ScreenToDisplay = NewGameOptionsScreen;

        interfaces->DestroyAllButtons();
        interfaces->DestroyAllIcons();
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayNewGameOptionsScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        interfaces->CreateButton(1003, 0, 454);

        interfaces->CreateArrowSet(0, 65);
        interfaces->CreateArrowSet(1, 105);
        interfaces->CreateArrowSet(2, 145);
        interfaces->CreateArrowSet(3, 185);
        interfaces->CreateArrowSet(4, 225);
        interfaces->CreateArrowSet(5, 265);
        interfaces->CreateArrowSet(6, 305);
        interfaces->CreateArrowSet(7, 345);

        audio->PlayMusic(1+logic->SelectedMusicTrack, -1);

        ScreenTransitionStatus = FadeIn;
    }

    interfaces->ProcessAllButtons();

    interfaces->ProcessAllArrowSets();

    if (interfaces->ArrowSetArrowSelectedByPlayer != -1)
    {
        if (interfaces->ArrowSetArrowSelectedByPlayer == 0)
        {
            if (logic->GameMode > 0)  logic->GameMode-=1;
            else  logic->GameMode = CrisisMode;
        }
        else if (interfaces->ArrowSetArrowSelectedByPlayer == 0.5)
        {
            if (logic->GameMode < CrisisMode)  logic->GameMode+=1;
            else  logic->GameMode = 0;
        }
        if (interfaces->ArrowSetArrowSelectedByPlayer == 1)
        {
            if (logic->SelectedMusicTrack > 0)  logic->SelectedMusicTrack-=1;
            else  logic->SelectedMusicTrack = 18;

            audio->PlayMusic(1+logic->SelectedMusicTrack, -1);
        }
        else if (interfaces->ArrowSetArrowSelectedByPlayer == 1.5)
        {
            if (logic->SelectedMusicTrack < 18)  logic->SelectedMusicTrack+=1;
            else  logic->SelectedMusicTrack = 0;

            audio->PlayMusic(1+logic->SelectedMusicTrack, -1);
        }
        if (interfaces->ArrowSetArrowSelectedByPlayer == 2)
        {
            if (logic->SelectedBackground > 0)  logic->SelectedBackground-=1;
            else  logic->SelectedBackground = 6;
        }
        else if (interfaces->ArrowSetArrowSelectedByPlayer == 2.5)
        {
            if (logic->SelectedBackground < 6)  logic->SelectedBackground+=1;
            else  logic->SelectedBackground = 0;
        }
        if (interfaces->ArrowSetArrowSelectedByPlayer == 3)
        {
            if (logic->NewGameGarbageHeight > 0)  logic->NewGameGarbageHeight-=1;
            else  logic->NewGameGarbageHeight = 14;
        }
        else if (interfaces->ArrowSetArrowSelectedByPlayer == 3.5)
        {
            if (logic->NewGameGarbageHeight < 14)  logic->NewGameGarbageHeight+=1;
            else  logic->NewGameGarbageHeight = 0;
        }

        if (interfaces->ArrowSetArrowSelectedByPlayer == 4)
        {
            if (logic->PressingUPAction > 0)  logic->PressingUPAction-=1;
            else  logic->PressingUPAction = 3;
        }
        else if (interfaces->ArrowSetArrowSelectedByPlayer == 4.5)
        {
            if (logic->PressingUPAction < 3)  logic->PressingUPAction+=1;
            else  logic->PressingUPAction = 0;
        }
        if (interfaces->ArrowSetArrowSelectedByPlayer == 5)
        {
            if (logic->DisplayNextPiece > 0)  logic->DisplayNextPiece-=1;
            else  logic->DisplayNextPiece = 1;
        }
        else if (interfaces->ArrowSetArrowSelectedByPlayer == 5.5)
        {
            if (logic->DisplayNextPiece < 1)  logic->DisplayNextPiece+=1;
            else  logic->DisplayNextPiece = 0;
        }

        if (interfaces->ArrowSetArrowSelectedByPlayer == 6)
        {
            if (logic->DisplayDropShadow > 0)  logic->DisplayDropShadow-=1;
            else  logic->DisplayDropShadow = 1;
        }
        else if (interfaces->ArrowSetArrowSelectedByPlayer == 6.5)
        {
            if (logic->DisplayDropShadow < 1)  logic->DisplayDropShadow+=1;
            else  logic->DisplayDropShadow = 0;
        }

        if (interfaces->ArrowSetArrowSelectedByPlayer == 7)
        {
            if (logic->TileSet > 0)  logic->TileSet-=1;
            else  logic->TileSet = 5;
        }
        else if (interfaces->ArrowSetArrowSelectedByPlayer == 7.5)
        {
            if (logic->TileSet < 5)  logic->TileSet+=1;
            else  logic->TileSet = 0;
        }

        interfaces->ArrowSetArrowSelectedByPlayer = -1;
    }

    if (ScreenIsDirty == true)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[100+logic->SelectedBackground].ScreenX = 320;
        visuals->Sprites[100+logic->SelectedBackground].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(100+logic->SelectedBackground);

        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = 200;
        visuals->DrawSpriteOntoScreenBuffer(0);

        visuals->DrawTextOntoScreenBuffer("N E W   G A M E   O P T I O N S:", visuals->Font[0]
                                          , 0, 1, JustifyCenter, 255, 255, 1, 90, 90, 1);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 7-6, JustifyCenter
                                          , 255, 255, 1, 90, 90, 1);

        visuals->DrawTextOntoScreenBuffer("Game Mode:", visuals->Font[0]
                                          , 50, 65-15, JustifyLeft
                                          , 255, 255, 255, 90, 90, 90);
        if (logic->GameMode == OriginalMode)
            visuals->DrawTextOntoScreenBuffer("Original Mode", visuals->Font[0]
                                              , 50, 65-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->GameMode == TimeAttack30Mode)
            visuals->DrawTextOntoScreenBuffer("Time Attack 30 Mode", visuals->Font[0]
                                              , 50, 65-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->GameMode == TimeAttack60Mode)
            visuals->DrawTextOntoScreenBuffer("Time Attack 60 Mode", visuals->Font[0]
                                              , 50, 65-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->GameMode == TimeAttack120Mode)
            visuals->DrawTextOntoScreenBuffer("Time Attack 120 Mode", visuals->Font[0]
                                              , 50, 65-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->GameMode == TwentyLineChallengeMode)
            visuals->DrawTextOntoScreenBuffer("20 Line Challenge Mode", visuals->Font[0]
                                              , 50, 65-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->GameMode == CrisisMode)
            visuals->DrawTextOntoScreenBuffer("Crisis+Mode", visuals->Font[0]
                                              , 50, 65-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("Music Track:", visuals->Font[0]
                                          , 50, 105-15, JustifyLeft
                                          , 255, 255, 255, 90, 90, 90);
        if (logic->SelectedMusicTrack == 0)
            visuals->DrawTextOntoScreenBuffer("InGame Track 00", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedMusicTrack == 1)
            visuals->DrawTextOntoScreenBuffer("InGame Track 01", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedMusicTrack == 2)
            visuals->DrawTextOntoScreenBuffer("InGame Track 02", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedMusicTrack == 3)
            visuals->DrawTextOntoScreenBuffer("InGame Track 03", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedMusicTrack == 4)
            visuals->DrawTextOntoScreenBuffer("InGame Track 04", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedMusicTrack == 5)
            visuals->DrawTextOntoScreenBuffer("InGame Track 05", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedMusicTrack == 6)
            visuals->DrawTextOntoScreenBuffer("InGame Track 06", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedMusicTrack == 7)
            visuals->DrawTextOntoScreenBuffer("InGame Track 07", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedMusicTrack == 8)
            visuals->DrawTextOntoScreenBuffer("InGame Track 08", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedMusicTrack == 9)
            visuals->DrawTextOntoScreenBuffer("InGame Track 09", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedMusicTrack == 10)
            visuals->DrawTextOntoScreenBuffer("InGame Track 10", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedMusicTrack == 11)
            visuals->DrawTextOntoScreenBuffer("InGame Track 11", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedMusicTrack == 12)
            visuals->DrawTextOntoScreenBuffer("InGame Track 12", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedMusicTrack == 13)
            visuals->DrawTextOntoScreenBuffer("InGame Track 13", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedMusicTrack == 14)
            visuals->DrawTextOntoScreenBuffer("InGame Track 14", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedMusicTrack == 15)
            visuals->DrawTextOntoScreenBuffer("InGame Track 15", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedMusicTrack == 16)
            visuals->DrawTextOntoScreenBuffer("InGame Track 16", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedMusicTrack == 17)
            visuals->DrawTextOntoScreenBuffer("InGame Track 17", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedMusicTrack == 18)
            visuals->DrawTextOntoScreenBuffer("InGame Track 18", visuals->Font[0]
                                              , 50, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("Background:", visuals->Font[0]
                                          , 50, 145-15, JustifyLeft
                                          , 255, 255, 255, 90, 90, 90);
        if (logic->SelectedBackground == 0)
            visuals->DrawTextOntoScreenBuffer("Saint Basils Cathedral", visuals->Font[0]
                                              , 50, 145-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedBackground == 1)
            visuals->DrawTextOntoScreenBuffer("Russian MIG-31 Firefox", visuals->Font[0]
                                              , 50, 145-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedBackground == 2)
        {
            char *reg = new char[2];
            sprintf(reg, "%c", 0xAE);
            strcpy(visuals->VariableText, "Nissan");
            strcat(visuals->VariableText, reg);
            strcat(visuals->VariableText, " GT-R(Blue)");
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0]
                                              , 50, 145-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        }
        else if (logic->SelectedBackground == 3)
            visuals->DrawTextOntoScreenBuffer("New York City", visuals->Font[0]
                                              , 50, 145-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedBackground == 4)
            visuals->DrawTextOntoScreenBuffer("Van Gogh Painting", visuals->Font[0]
                                              , 50, 145-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedBackground == 5)
            visuals->DrawTextOntoScreenBuffer("Kittens", visuals->Font[0]
                                              , 50, 145-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->SelectedBackground == 6)
            visuals->DrawTextOntoScreenBuffer("Psychedelic", visuals->Font[0]
                                              , 50, 145-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("Garbage Level Height:", visuals->Font[0]
                                          , 50, 185-15, JustifyLeft
                                          , 255, 255, 255, 90, 90, 90);

        char temp[256];
        sprintf(temp, "%d", logic->NewGameGarbageHeight);
        strcpy(visuals->VariableText, temp);

        visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0]
                                          , 50, 185-15, JustifyRight
                                          , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("Pressing [UP] Action:", visuals->Font[0]
                                          , 50, 225-15, JustifyLeft
                                          , 255, 255, 255, 90, 90, 90);
        if (logic->PressingUPAction == 0)
            visuals->DrawTextOntoScreenBuffer("None", visuals->Font[0]
                                              , 50, 225-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->PressingUPAction == 1)
            visuals->DrawTextOntoScreenBuffer("Quick Drop", visuals->Font[0]
                                              , 50, 225-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->PressingUPAction == 2)
            visuals->DrawTextOntoScreenBuffer("Smart Rotate", visuals->Font[0]
                                              , 50, 225-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->PressingUPAction == 3)
            visuals->DrawTextOntoScreenBuffer("Drop & Drag", visuals->Font[0]
                                              , 50, 225-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("Display Next Piece:", visuals->Font[0]
                                          , 50, 265-15, JustifyLeft
                                          , 255, 255, 255, 90, 90, 90);
        if (logic->DisplayNextPiece == 0)
            visuals->DrawTextOntoScreenBuffer("OFF", visuals->Font[0]
                                              , 50, 265-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->DisplayNextPiece == 1)
            visuals->DrawTextOntoScreenBuffer("ON", visuals->Font[0]
                                              , 50, 265-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("Display Drop Shadow:", visuals->Font[0]
                                          , 50, 305-15, JustifyLeft
                                          , 255, 255, 255, 90, 90, 90);
        if (logic->DisplayDropShadow == 0)
            visuals->DrawTextOntoScreenBuffer("OFF", visuals->Font[0]
                                              , 50, 305-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->DisplayDropShadow == 1)
            visuals->DrawTextOntoScreenBuffer("ON", visuals->Font[0]
                                              , 50, 305-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("Box Tileset:", visuals->Font[0]
                                          , 50, 345-15, JustifyLeft
                                          , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("Press [Enter] Or Click [Start!] To Begin!"
                                          , visuals->Font[0], 0, 380, JustifyCenter
                                          , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 404-6, JustifyCenter
                                          , 255, 255, 1, 90, 90, 1);

        interfaces->DisplayAllButtonsOntoScreenBuffer();

        interfaces->DisplayAllArrowSetsOntoScreenBuffer();

        float x = 401;
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

        if (interfaces->ButtonSelectedByPlayer == 0)
        {
            ScreenToDisplay = PlayingGameScreen;

            audio->PlayMusic(1+logic->SelectedMusicTrack, -1);
            if (logic->SelectedBackground == 1)  Mix_PauseMusic();
        }
        else
        {
            ScreenToDisplay = TitleScreen;
            audio->PlayMusic(0, -1);
        }

        interfaces->DestroyAllButtons();

        interfaces->DestroyAllArrowSets();
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayOptionsScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        interfaces->CreateButton(1009, 0, 454);

        interfaces->CreateArrowSet(0, 60);
        interfaces->CreateArrowSet(1, 105);

        interfaces->CreateArrowSet(2, 160);

        interfaces->CreateArrowSet(3, 215);

        interfaces->CreateArrowSet(4, 255);

        interfaces->CreateArrowSet(5, 270+25+10);

        interfaces->CreateArrowSet(6, 345);

        ScreenTransitionStatus = FadeIn;
    }

    if (input->F2KeyPressed == true && input->JoystickSetupProcess == JoySetupNotStarted)
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

    if (input->KeyboardSetupProcess == KeyboardSetupPressOne && input->F2KeyPressed != true
        && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyButtonOne = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupPressTwo;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = true;
    }
    else if (input->KeyboardSetupProcess == KeyboardSetupPressTwo && input->F2KeyPressed != true
             && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyButtonTwo = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupPressUP;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = true;
    }
    else if (input->KeyboardSetupProcess == KeyboardSetupPressUP && input->F2KeyPressed != true
             && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyUP = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupPressRIGHT;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = true;
    }
    else if (input->KeyboardSetupProcess == KeyboardSetupPressRIGHT && input->F2KeyPressed != true
             && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyRIGHT = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupPressDOWN;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = true;
    }
    else if (input->KeyboardSetupProcess == KeyboardSetupPressDOWN && input->F2KeyPressed != true
             && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyDOWN = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupPressLEFT;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = true;
    }
    else if (input->KeyboardSetupProcess == KeyboardSetupPressLEFT && input->F2KeyPressed != true
             && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyLEFT = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupPressPause;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = true;
    }
    else if (input->KeyboardSetupProcess == KeyboardSetupPressPause && input->F2KeyPressed != true
             && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyPause = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupNotStarted;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = true;
    }

    if ( input->F1KeyPressed  == true && input->JoystickDeviceOne != NULL
        && input->KeyboardSetupProcess == KeyboardSetupNotStarted )
    {
        ScreenIsDirty = true;
        audio->PlayDigitalSoundFX(1, 0);

        if (input->JoystickSetupProcess == JoySetupNotStarted)
        {
            input->JoystickSetupProcess = Joy1SetupPressUP;
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
        joyAction = input->QueryJoysticksForAction(2);
    else if (input->JoystickSetupProcess > Joy1SetupPressBUTTONTwo)
        joyAction = input->QueryJoysticksForAction(1);
    else if (input->JoystickSetupProcess > JoySetupNotStarted)
        joyAction = input->QueryJoysticksForAction(0);

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
        interfaces->ProcessAllButtons();
        interfaces->ProcessAllArrowSets();

        if (interfaces->ArrowSetArrowSelectedByPlayer != -1)
        {
            if (interfaces->ArrowSetArrowSelectedByPlayer == 0)
            {
                if (audio->MusicVolume > 0)  audio->MusicVolume-=32;
                else  audio->MusicVolume = 128;

                Mix_VolumeMusic(audio->MusicVolume);
            }
            else if (interfaces->ArrowSetArrowSelectedByPlayer == 0.5)
            {
                if (audio->MusicVolume < 128)  audio->MusicVolume+=32;
                else  audio->MusicVolume = 0;

                Mix_VolumeMusic(audio->MusicVolume);
            }
            else if (interfaces->ArrowSetArrowSelectedByPlayer == 1)
            {
                if (audio->SoundVolume > 0)  audio->SoundVolume-=32;
                else  audio->SoundVolume = 128;

                Mix_HaltChannel(-1);
                audio->PlayDigitalSoundFX(1, 0);
            }
            else if (interfaces->ArrowSetArrowSelectedByPlayer == 1.5)
            {
                if (audio->SoundVolume < 128)  audio->SoundVolume+=32;
                else  audio->SoundVolume = 0;

                Mix_HaltChannel(-1);
                audio->PlayDigitalSoundFX(1, 0);
            }
            else if (interfaces->ArrowSetArrowSelectedByPlayer == 2)
            {
                visuals->FullScreenMode = !visuals->FullScreenMode;

                if (visuals->FullScreenMode == false)  SDL_SetWindowFullscreen(visuals->Window, 0);
                else if (visuals->FullScreenMode == true)  SDL_SetWindowFullscreen(visuals->Window, SDL_WINDOW_FULLSCREEN_DESKTOP);

            }
            else if (interfaces->ArrowSetArrowSelectedByPlayer == 2.5)
            {
                visuals->FullScreenMode = !visuals->FullScreenMode;

                if (visuals->FullScreenMode == false)  SDL_SetWindowFullscreen(visuals->Window, 0);
                else if (visuals->FullScreenMode == true)  SDL_SetWindowFullscreen(visuals->Window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
            else if (interfaces->ArrowSetArrowSelectedByPlayer == 3)
            {
                if (logic->CPUPlayerEnabled > 0)  logic->CPUPlayerEnabled-=1;
                else  logic->CPUPlayerEnabled = 4;
            }
            else if (interfaces->ArrowSetArrowSelectedByPlayer == 3.5)
            {
                if (logic->CPUPlayerEnabled < 4)  logic->CPUPlayerEnabled+=1;
                else  logic->CPUPlayerEnabled = 0;
            }
            else if (interfaces->ArrowSetArrowSelectedByPlayer == 4)
            {
                if (logic->NaturalIntelligenceCore > 0)  logic->NaturalIntelligenceCore-=1;
                else  logic->NaturalIntelligenceCore = 1;

                logic->NaturalIntelligenceCore = 0;
            }
            else if (interfaces->ArrowSetArrowSelectedByPlayer == 4.5)
            {
                if (logic->NaturalIntelligenceCore < 1)  logic->NaturalIntelligenceCore+=1;
                else  logic->NaturalIntelligenceCore = 0;

                logic->NaturalIntelligenceCore = 0;
            }
            else if (interfaces->ArrowSetArrowSelectedByPlayer == 5)
            {
                if (logic->DelayAutoShift > 0)  logic->DelayAutoShift-=1;
                else  logic->DelayAutoShift = 2;
            }
            else if (interfaces->ArrowSetArrowSelectedByPlayer == 5.5)
            {
                if (logic->DelayAutoShift < 2)  logic->DelayAutoShift+=1;
                else  logic->DelayAutoShift = 0;
            }
            else if (interfaces->ArrowSetArrowSelectedByPlayer == 6)
            {
                if (logic->PlayingGameFrameLock == 33)  logic->PlayingGameFrameLock = 10;
                else if (logic->PlayingGameFrameLock == 10)  logic->PlayingGameFrameLock = 25;
                else if (logic->PlayingGameFrameLock == 25)  logic->PlayingGameFrameLock = 33;
            }
            else if (interfaces->ArrowSetArrowSelectedByPlayer == 6.5)
            {
                if (logic->PlayingGameFrameLock == 33)  logic->PlayingGameFrameLock = 25;
                else if (logic->PlayingGameFrameLock == 25)  logic->PlayingGameFrameLock = 10;
                else if (logic->PlayingGameFrameLock == 10)  logic->PlayingGameFrameLock = 33;
            }

            interfaces->ArrowSetArrowSelectedByPlayer = -1;
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

        visuals->DrawTextOntoScreenBuffer("O P T I O N S:", visuals->Font[0]
                                          , 0, 1, JustifyCenter, 255, 255, 1, 90, 90, 1);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 7-6, JustifyCenter
                                          , 255, 255, 1, 90, 90, 1);

        interfaces->DisplayAllArrowSetsOntoScreenBuffer();

        visuals->DrawTextOntoScreenBuffer("Music Volume:"
                                          , visuals->Font[0], 60, 60-15, JustifyLeft
                                          , 255, 255, 255, 90, 90, 90);
        if (audio->MusicVolume == 0)
            visuals->DrawTextOntoScreenBuffer("OFF", visuals->Font[0], 60, 60-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (audio->MusicVolume == 32)
            visuals->DrawTextOntoScreenBuffer("25%", visuals->Font[0], 60, 60-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (audio->MusicVolume == 64)
            visuals->DrawTextOntoScreenBuffer("50%", visuals->Font[0], 60, 60-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (audio->MusicVolume == 96)
            visuals->DrawTextOntoScreenBuffer("75%", visuals->Font[0], 60, 60-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (audio->MusicVolume == 128)
            visuals->DrawTextOntoScreenBuffer("100%", visuals->Font[0], 60, 60-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("Sound Effects Volume:"
                                          , visuals->Font[0], 60, 105-15, JustifyLeft
                                          , 255, 255, 255, 90, 90, 90);
        if (audio->SoundVolume == 0)
            visuals->DrawTextOntoScreenBuffer("OFF", visuals->Font[0], 60, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (audio->SoundVolume == 32)
            visuals->DrawTextOntoScreenBuffer("25%", visuals->Font[0], 60, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (audio->SoundVolume == 64)
            visuals->DrawTextOntoScreenBuffer("50%", visuals->Font[0], 60, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (audio->SoundVolume == 96)
            visuals->DrawTextOntoScreenBuffer("75%", visuals->Font[0], 60, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (audio->SoundVolume == 128)
            visuals->DrawTextOntoScreenBuffer("100%", visuals->Font[0], 60, 105-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 107-6, JustifyCenter
                                          , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("Full Screen Mode:"
                                          , visuals->Font[0], 60, 160-15, JustifyLeft
                                          , 255, 255, 255, 90, 90, 90);
        if (visuals->FullScreenMode == false)
            visuals->DrawTextOntoScreenBuffer("OFF", visuals->Font[0], 60, 160-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (visuals->FullScreenMode == true)
            visuals->DrawTextOntoScreenBuffer("ON", visuals->Font[0], 60, 160-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 162-6, JustifyCenter
                                          , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("C.P.U. Players:"
                                          , visuals->Font[0], 60, 215-15, JustifyLeft
                                          , 255, 255, 255, 90, 90, 90);
        if (logic->CPUPlayerEnabled == 0)
            visuals->DrawTextOntoScreenBuffer("OFF", visuals->Font[0], 60, 215-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->CPUPlayerEnabled == 1)
            visuals->DrawTextOntoScreenBuffer("Slow Speed", visuals->Font[0], 60, 215-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->CPUPlayerEnabled == 2)
            visuals->DrawTextOntoScreenBuffer("Medium Speed", visuals->Font[0], 60, 215-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->CPUPlayerEnabled == 3)
            visuals->DrawTextOntoScreenBuffer("Fast Speed", visuals->Font[0], 60, 215-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->CPUPlayerEnabled == 4)
            visuals->DrawTextOntoScreenBuffer("Very Fast Speed", visuals->Font[0], 60, 215-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("A.I. Core:"
                                          , visuals->Font[0], 60, 255-15, JustifyLeft
                                          , 255, 255, 255, 90, 90, 90);
        if (logic->NaturalIntelligenceCore == 0)
            visuals->DrawTextOntoScreenBuffer("Original JeZ+Lee A.I.", visuals->Font[0], 60, 255-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->NaturalIntelligenceCore == 1)
            visuals->DrawTextOntoScreenBuffer("New Perfect A.I.", visuals->Font[0], 60, 255-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 285-37, JustifyCenter
                                          , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("Delayed Auto Shift:"
                                          , visuals->Font[0], 60, 270-15+25+10, JustifyLeft
                                          , 255, 255, 255, 90, 90, 90);
        if (logic->DelayAutoShift == 0)
            visuals->DrawTextOntoScreenBuffer("Original", visuals->Font[0], 60, 270-15+25+10, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->DelayAutoShift == 1)
            visuals->DrawTextOntoScreenBuffer("Slow Shift", visuals->Font[0], 60, 270-15+25+10, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->DelayAutoShift == 2)
            visuals->DrawTextOntoScreenBuffer("Fast Shift", visuals->Font[0], 60, 270-15+25+10, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("Playing Game Speed:", visuals->Font[0]
                                          , 60, 345-15, JustifyLeft
                                          , 255, 255, 255, 90, 90, 90);
        if (logic->PlayingGameFrameLock == 33)
            visuals->DrawTextOntoScreenBuffer("Normal", visuals->Font[0], 60, 345-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->PlayingGameFrameLock == 25)
            visuals->DrawTextOntoScreenBuffer("Fast", visuals->Font[0], 60, 345-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->PlayingGameFrameLock == 10)
            visuals->DrawTextOntoScreenBuffer("Very Fast", visuals->Font[0], 60, 345-15, JustifyRight
                                              , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 344-6, JustifyCenter
                                          , 255, 255, 255, 90, 90, 90);

        if (input->JoystickDeviceOne != NULL)
        {
            visuals->DrawTextOntoScreenBuffer("Press [F1] On Keyboard To Setup Joystick(s)"
                                              , visuals->Font[1]
                                              , 0, 374, JustifyCenter, 255, 255, 255, 90, 90, 90);
        }

        visuals->DrawTextOntoScreenBuffer("Press [F2] On Keyboard To Setup Keyboard", visuals->Font[1]
                                          , 0, 399, JustifyCenter, 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 404-6, JustifyCenter
                                          , 255, 255, 1, 90, 90, 1);

        interfaces->DisplayAllButtonsOntoScreenBuffer();

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
        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = TitleScreen;

        interfaces->DestroyAllButtons();
        interfaces->DestroyAllArrowSets();
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayHowToPlayScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        interfaces->CreateButton(1009, 0, 454);

        ScreenTransitionStatus = FadeIn;
    }

    interfaces->ProcessAllButtons();

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
            visuals->Sprites[4].ScreenY = 160;

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

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 7-6, JustifyCenter
                                          , 255, 255, 1, 90, 90, 1);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 404-6, JustifyCenter
                                          , 255, 255, 1, 90, 90, 1);

        if (input->UserDefinedKeyButtonOne != -1 && input->UserDefinedKeyButtonTwo != -1)
        {

            visuals->DrawTextOntoScreenBuffer("CUSTOM CONTROL", visuals->Font[4]
                                              , 0, 110, JustifyCenter, 255, 255, 255, 90, 90, 1);

            sprintf(visuals->VariableText, "%c", input->UserDefinedKeyUP);
            strcat(visuals->VariableText, " = Special UP Action");
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[1]
                                              , 0, 260, JustifyCenter, 255, 255, 255, 0, 0, 0);

            sprintf(visuals->VariableText, "%c", input->UserDefinedKeyRIGHT);
            strcat(visuals->VariableText, " = Move Piece Right");
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[1]
                                              , 0, 280, JustifyCenter, 255, 255, 255, 0, 0, 0);

            sprintf(visuals->VariableText, "%c", input->UserDefinedKeyDOWN);
            strcat(visuals->VariableText, " = Move Piece Down");
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[1]
                                              , 0, 300, JustifyCenter, 255, 255, 255, 0, 0, 0);

            sprintf(visuals->VariableText, "%c", input->UserDefinedKeyLEFT);
            strcat(visuals->VariableText, " = Move Piece Left");
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[1]
                                              , 0, 320, JustifyCenter, 255, 255, 255, 0, 0, 0);

            sprintf(visuals->VariableText, "%c", input->UserDefinedKeyPause);
            strcat(visuals->VariableText, " = Pause");
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[1]
                                              , 0, 340, JustifyCenter, 255, 255, 255, 0, 0, 0);

            sprintf(visuals->VariableText, "%c", input->UserDefinedKeyButtonOne);
            strcat(visuals->VariableText, " = Rotate Clockwise");
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[1]
                                              , 0, 370, JustifyCenter, 255, 255, 255, 0, 0, 0);

            sprintf(visuals->VariableText, "%c", input->UserDefinedKeyButtonTwo);
            strcat(visuals->VariableText, " = Rotate Counter Clockwise");
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[1]
                                              , 0, 390, JustifyCenter, 255, 255, 255, 0, 0, 0);
        }

        interfaces->DisplayAllButtonsOntoScreenBuffer();
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = TitleScreen;

        interfaces->DestroyAllButtons();
    }
}


//-------------------------------------------------------------------------------------------------
void Screens::DisplayHighScoresScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        interfaces->CreateButton(1009, 0, 454);

        interfaces->CreateArrowSet(0, 65);

        ScreenTransitionStatus = FadeIn;
    }

    interfaces->ProcessAllButtons();

    interfaces->ProcessAllArrowSets();

    if (interfaces->ArrowSetArrowSelectedByPlayer != -1)
    {
        if (interfaces->ArrowSetArrowSelectedByPlayer == 0)
        {
            if (logic->GameMode > 0)  logic->GameMode-=1;
            else  logic->GameMode = CrisisMode;

            visuals->ClearTextCache();
        }
        else if (interfaces->ArrowSetArrowSelectedByPlayer == 0.5)
        {
            if (logic->GameMode < CrisisMode)  logic->GameMode+=1;
            else  logic->GameMode = 0;

            visuals->ClearTextCache();
        }

        interfaces->ArrowSetArrowSelectedByPlayer = -1;
    }

    if (input->KeyOnKeyboardPressedByUser == 'C')
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

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 7-6, JustifyCenter
                                          , 255, 255, 1, 90, 90, 1);

        interfaces->DisplayAllArrowSetsOntoScreenBuffer();

        if (logic->GameMode == OriginalMode)
            visuals->DrawTextOntoScreenBuffer("Original Mode", visuals->Font[0]
                                              , 0, 65-15, JustifyCenter
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->GameMode == TimeAttack30Mode)
            visuals->DrawTextOntoScreenBuffer("Time Attack 30 Mode", visuals->Font[0]
                                              , 0, 65-15, JustifyCenter
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->GameMode == TimeAttack60Mode)
            visuals->DrawTextOntoScreenBuffer("Time Attack 60 Mode", visuals->Font[0]
                                              , 0, 65-15, JustifyCenter
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->GameMode == TimeAttack120Mode)
            visuals->DrawTextOntoScreenBuffer("Time Attack 120 Mode", visuals->Font[0]
                                              , 0, 65-15, JustifyCenter
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->GameMode == TwentyLineChallengeMode)
            visuals->DrawTextOntoScreenBuffer("20 Line Challenge Mode", visuals->Font[0]
                                              , 0, 65-15, JustifyCenter
                                              , 255, 255, 255, 90, 90, 90);
        else if (logic->GameMode == CrisisMode)
            visuals->DrawTextOntoScreenBuffer("Crisis+Mode", visuals->Font[0]
                                              , 0, 65-15, JustifyCenter
                                              , 255, 255, 255, 90, 90, 90);

        visuals->DrawTextOntoScreenBuffer("Name:", visuals->Font[1],
                                          30, 88, JustifyLeft, 150, 150, 150, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Level:", visuals->Font[1],
                                          420, 88, JustifyLeft, 150, 150, 150, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Score:", visuals->Font[1],
                                          510, 88, JustifyLeft, 150, 150, 150, 1, 1, 1);

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

            if (logic->GameMode == CrisisMode && data->HighScoresLevel[logic->GameMode][index] == 10)
            {
                visuals->DrawTextOntoScreenBuffer("Won!", visuals->Font[0], 420, rankY
                                                  , JustifyLeft, 255, greenBlueColorValue, greenBlueColorValue, 1, 1, 1);
            }
            else
            {
                sprintf(visuals->VariableText, "%d", data->HighScoresLevel[logic->GameMode][index]);
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], 420, rankY
                                                  , JustifyLeft, 255, greenBlueColorValue, greenBlueColorValue, 1, 1, 1);
            }

            sprintf(visuals->VariableText, "%d", data->HighScoresScore[logic->GameMode][index]);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], 510, rankY, JustifyLeft
                                              , 255, greenBlueColorValue, greenBlueColorValue, 1, 1, 1);

            rankY += offsetY;
        }

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 404-6, JustifyCenter
                                          , 255, 255, 1, 90, 90, 1);

        interfaces->DisplayAllButtonsOntoScreenBuffer();
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = TitleScreen;

        interfaces->DestroyAllButtons();
        interfaces->DestroyAllArrowSets();
    }
}


//-------------------------------------------------------------------------------------------------
void Screens::DisplayAboutScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
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
                    visuals->Sprites[index].ScreenY = visuals->Sprites[index-1].ScreenY+120;
                }
                else
                {
                    visuals->Sprites[index].ScreenX = 320;
                    visuals->Sprites[index].ScreenY = visuals->Sprites[index-1].ScreenY+30;
                }
            }
            else if (index == 1100 + visuals->TotalNumberOfLoadedStaffTexts-1)
            {
                visuals->Sprites[index].ScreenX = 320;
                visuals->Sprites[index].ScreenY = visuals->Sprites[index-2].ScreenY+265;
            }
        }

        ScreenTransitionStatus = FadeIn;
    }

    if ( input->SpacebarKeyPressed == true
        || input->EnterKeyPressed == true
        || input->MouseButtonPressed[0] == true
        || input->JoystickButtonOne[Any] == ON)
    {
        ScreenTransitionStatus = FadeOut;
        input->DelayAllUserInput = 60;
        audio->PlayDigitalSoundFX(0, 0);
    }

    for (int index = 1100; index < (1100+visuals->TotalNumberOfLoadedStaffTexts); index++)
    {
        visuals->Sprites[index].ScreenY-=1;
    }

    if (visuals->Sprites[1100+visuals->TotalNumberOfLoadedStaffTexts-1].ScreenY < -20)
        ScreenTransitionStatus = FadeOut;

//    if (ScreenIsDirty == true)
    {
        visuals->Sprites[2].ScreenX = 320;
        visuals->Sprites[2].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(2);

        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = 200;
        visuals->DrawSpriteOntoScreenBuffer(0);

        for (  int index = 1100; index < ( (1100 + visuals->TotalNumberOfLoadedStaffTexts) ); index++  )
        {
            visuals->DrawSpriteOntoScreenBuffer(index);
        }

        ScreenIsDirty = true;
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = TitleScreen;

        if (logic->CrisisWon == true)
        {
            logic->CrisisWon = false;

            if (data->PlayerRankOnGameOver < 10)
            {
                if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == Keyboard)
                    ScreenToDisplay = NameInputKeyboardScreen;
                else if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickOne
                         || logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickTwo
                         || logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickThree)
                    ScreenToDisplay = NameInputJoystickScreen;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayPlayingGameScreen(void)
{
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

    for (int player = 0; player < NumberOfPlayers; player++)
    {
        if (input->JoystickButtonOne[logic->PlayerData[player].PlayerInput] == ON  && logic->PlayersCanJoin == true
           && logic->PlayerData[player].PlayerStatus == GameOver)  logic->PlayerData[player].PlayerStatus = NewPieceDropping;
    }

    if (logic->PlayersCanJoin == true && input->MouseButtonPressed[0] == true)
    {
        for (int p = 0; p < 4; p++)
        {
            if (logic->PlayerData[p].PlayerInput == Mouse)
            {
                logic->PlayerData[p].PlayerStatus = NewPieceDropping;
            }
        }
    }

//    if (ScreenIsDirty == true)
    {
        visuals->Sprites[100+logic->SelectedBackground].ScreenX = 320;
        visuals->Sprites[100+logic->SelectedBackground].ScreenY = 240;

        visuals->Sprites[106].ScaleX = 1.6;
        visuals->Sprites[106].ScaleY = 1.6;

        if (logic->PsychoBackgroundRotationOne < 360)  logic->PsychoBackgroundRotationOne++;
        else  logic->PsychoBackgroundRotationOne = 0;

        if (logic->PsychoBackgroundRotationTwo > 0)  logic->PsychoBackgroundRotationTwo--;
        else  logic->PsychoBackgroundRotationTwo = 360;

        if (logic->SelectedBackground != 6)
            visuals->DrawSpriteOntoScreenBuffer(100+logic->SelectedBackground);
        else
        {
            visuals->Sprites[106].Transparency = 255;
            visuals->Sprites[106].RotationDegree = logic->PsychoBackgroundRotationOne;
            visuals->DrawSpriteOntoScreenBuffer(106);

            visuals->Sprites[106].Transparency = 125;
            visuals->Sprites[106].RotationDegree = logic->PsychoBackgroundRotationTwo;
            visuals->DrawSpriteOntoScreenBuffer(106);
        }

        if (logic->PlayerData[0].PlayerStatus != GameOver)
        {
            visuals->Sprites[80].ScreenX = logic->PlayerData[0].PlayersPlayfieldScreenX;
            visuals->Sprites[80].ScreenY = logic->PlayerData[0].PlayersPlayfieldScreenY;
            visuals->Sprites[80].Transparency = 200;
            visuals->Sprites[80].RedHue = 255;
            visuals->Sprites[80].GreenHue = 0;
            visuals->Sprites[80].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(80);
        }

        if (logic->PlayerData[1].PlayerStatus != GameOver)
        {
            visuals->Sprites[80].ScreenX = logic->PlayerData[1].PlayersPlayfieldScreenX;
            visuals->Sprites[80].ScreenY = logic->PlayerData[1].PlayersPlayfieldScreenY;
            visuals->Sprites[80].Transparency = 200;
            visuals->Sprites[80].RedHue = 255;
            visuals->Sprites[80].GreenHue = 255;
            visuals->Sprites[80].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(80);
        }

        if (logic->PlayerData[2].PlayerStatus != GameOver)
        {
            visuals->Sprites[80].ScreenX = logic->PlayerData[2].PlayersPlayfieldScreenX;
            visuals->Sprites[80].ScreenY = logic->PlayerData[2].PlayersPlayfieldScreenY;
            visuals->Sprites[80].Transparency = 200;
            visuals->Sprites[80].RedHue = 0;
            visuals->Sprites[80].GreenHue = 255;
            visuals->Sprites[80].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(80);
        }

        if (logic->PlayerData[3].PlayerStatus != GameOver)
        {
            visuals->Sprites[80].ScreenX = logic->PlayerData[3].PlayersPlayfieldScreenX;
            visuals->Sprites[80].ScreenY = logic->PlayerData[3].PlayersPlayfieldScreenY;
            visuals->Sprites[80].Transparency = 200;
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
                    if (logic->PlayerData[player].Playfield[x][y] == 1 || logic->PlayerData[player].Playfield[x][y] == 1000)
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

                    boxScreenX+=13;
                }

                boxScreenX = logic->PlayerData[player].PlayersPlayfieldScreenX-57;
                boxScreenY+=18;
            }

            if (logic->PlayerData[player].PlayerInput == Mouse && logic->PlayerData[player].PlayerStatus != GameOver)
            {
                boxScreenX = logic->PlayerData[player].PlayersPlayfieldScreenX-57-13;
                boxScreenY = logic->PlayerData[player].PlayersPlayfieldScreenY-212;
                for (int y = 0; y < 26; y++)
                {
                    for (int x = 1; x < 12; x++)
                    {
                        if (logic->MouseMoveX == x && logic->MouseMoveY == y)
                        {
                            if (logic->MouseMoveOrRotate == MouseRotate)
                                visuals->DrawTextOntoScreenBuffer("ROTATE", visuals->Font[2], boxScreenX+13
                                                                  , boxScreenY, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
                            else if (logic->MouseMoveOrRotate == MouseMove)
                                visuals->DrawTextOntoScreenBuffer("MOVE", visuals->Font[2], boxScreenX+13
                                                                  , boxScreenY, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
                        }

                        boxScreenX+=13;
                    }

                    boxScreenX = logic->PlayerData[player].PlayersPlayfieldScreenX-57;
                    boxScreenY+=18;
                }
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

        for (int player = 0; player < NumberOfPlayers; player++)
        {
            if (logic->PlayersCanJoin == true)
            {

                if ( (logic->PlayerData[player].PlayerInput == JoystickOne && input->JoystickDeviceOne != NULL)
                    || (logic->PlayerData[player].PlayerInput == JoystickTwo && input->JoystickDeviceTwo != NULL)
                    || (logic->PlayerData[player].PlayerInput == JoystickThree && input->JoystickDeviceThree != NULL)
                    || (logic->PlayerData[player].PlayerInput == Keyboard)
                    || (logic->PlayerData[player].PlayerInput == Mouse) )
                {
                    if (logic->PlayerData[player].PlayerStatus == GameOver)
                        visuals->DrawTextOntoScreenBuffer("JOIN IN!", visuals->Font[1], logic->PlayerData[player].PlayersPlayfieldScreenX, 270,
                                                          JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
                }
            }
        }

        for (int player = 0; player < NumberOfPlayers; player++)
        {
            sprintf(visuals->VariableText, "%d", logic->PlayerData[player].Score);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[2]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX, 9
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

            sprintf(visuals->VariableText, "%d", logic->PlayerData[player].Lines);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[2]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX-59, 62
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

            sprintf(visuals->VariableText, "%d", logic->PlayerData[player].Level);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[2]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX+59, 62
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
                visuals->DrawTextOntoScreenBuffer("C.P.U."
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460, JustifyCenterOnPoint, 255, 255, 255, 90, 90, 90);
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
                    visuals->Sprites[155].Transparency = 30;
                    visuals->DrawSpriteOntoScreenBuffer(155);
                }
            }
        }

        bool humanStillAlive = false;
        if (logic->PlayerData[0].PlayerInput != CPU && logic->PlayerData[0].PlayerStatus != GameOver)  humanStillAlive = true;
        if (logic->PlayerData[1].PlayerInput != CPU && logic->PlayerData[1].PlayerStatus != GameOver)  humanStillAlive = true;
        if (logic->PlayerData[2].PlayerInput != CPU && logic->PlayerData[2].PlayerStatus != GameOver)  humanStillAlive = true;
        if (logic->PlayerData[3].PlayerInput != CPU && logic->PlayerData[3].PlayerStatus != GameOver)  humanStillAlive = true;

        if (humanStillAlive == false && input->DEBUG == 0)
            visuals->DrawTextOntoScreenBuffer("Continue Watching Or Press [Esc] On Keyboard To Exit!"
                                              , visuals->Font[1], 320, 260, JustifyCenter, 255, 255, 255, 1, 1, 1);

        if (logic->PAUSEgame == true && input->DEBUG != 1)
        {
            visuals->Sprites[0].ScreenX = 320;
            visuals->Sprites[0].ScreenY = 240;
            visuals->Sprites[0].Transparency = 200;
            visuals->DrawSpriteOntoScreenBuffer(0);

            visuals->DrawTextOntoScreenBuffer("G A M E   P A U S E D", visuals->Font[0]
                                              , 0, 225, JustifyCenter, 255, 255, 255, 90, 90, 90);

            if (input->UserDefinedKeyPause == -1)
                visuals->DrawTextOntoScreenBuffer("(Press [SpaceBar] On Keyboard To Continue!)", visuals->Font[1]
                                                  , 0, 265, JustifyCenter, 255, 255, 255, 90, 90, 90);
            else
            {
                strcpy(visuals->VariableText, "(Press [");
                char pauseKey[64];
                sprintf(pauseKey, "%c", input->UserDefinedKeyPause);
                strcat(visuals->VariableText, pauseKey);
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

        if (data->PlayerRankOnGameOver < 10)
        {
            if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == Keyboard)
                ScreenToDisplay = NameInputKeyboardScreen;
            else if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickOne
                     || logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickTwo
                     || logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickThree)
                ScreenToDisplay = NameInputJoystickScreen;
        }

        audio->PlayMusic(0, -1);

        if (logic->CrisisWon == true)
        {
            ScreenToDisplay = AboutScreen;
            audio->PlayMusic(25, -1);
        }

        visuals->FrameLock = 16;

        visuals->ClearTextCache();
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayNameInputKeyboardScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        interfaces->CreateButton(1008, 0, 454);

        ScreenTransitionStatus = FadeIn;

        data->NameInputArayIndex = 0;
    }

    interfaces->ProcessAllButtons();

    if (input->EnterKeyPressed == true)
    {
        if (data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] == '\0')
            data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] = ' ';

        audio->PlayDigitalSoundFX(0, 0);
    }
    else if (input->BackSpaceKeyPressed == true)
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
    else if (input->SpacebarKeyPressed == true)
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
            data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex]
                = input->KeyOnKeyboardPressedByUser;

        if (data->NameInputArayIndex < 19)  data->NameInputArayIndex++;

        input->DelayAllUserInput = 20;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = true;
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

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[0], 0, 7, JustifyCenter
                                          , 255, 255, 1, 90, 90, 1);

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

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[0], 0, 404, JustifyCenter
                                          , 255, 255, 1, 90, 90, 1);

        interfaces->DisplayAllButtonsOntoScreenBuffer();
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = HighScoresScreen;

        logic->CrisisWon = false;

        interfaces->DestroyAllButtons();
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayNameInputMouseKeyboardScreen(void)
{
	if (ScreenTransitionStatus == FadeAll)
	{
		interfaces->CreateButton(1008, 0, 454);

		input->DelayAllUserInput = 20;

		ScreenTransitionStatus = FadeIn;

		for (int index = 0; index < 73; index++)
		{
			NameInputButtons[index].Scale = 1;
			NameInputButtons[index].ScaleTimer = -1;
		}
	}

	interfaces->ProcessAllButtons();

	char characterArray[73] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
		, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
		, '!', '@', '#', '$', '%', '^', '&', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', ' ' };

    if ( input->BackSpaceKeyPressed == true )
    {
        NameInputButtons[71].Scale = .75;
        NameInputButtons[71].ScaleTimer = 10;

        input->DelayAllUserInput = 20;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = true;
    }

	if (input->KeyOnKeyboardPressedByUser != -1)
	{
		if (input->SpacebarKeyPressed == true )
		{
			NameInputButtons[70].Scale = .75;
			NameInputButtons[70].ScaleTimer = 10;

			input->DelayAllUserInput = 20;

			audio->PlayDigitalSoundFX(0, 0);
			ScreenIsDirty = true;
		}
		else
		{
			int charTyped = 70;
			for (int index = 0; index < 70; index++)
			{
				if (input->KeyOnKeyboardPressedByUser == characterArray[index])
				{
					charTyped = index;

					NameInputButtons[charTyped].Scale = .75;
					NameInputButtons[charTyped].ScaleTimer = 10;

					input->DelayAllUserInput = 20;

					audio->PlayDigitalSoundFX(0, 0);
					ScreenIsDirty = true;

					break;
				}
			}
		}
	}

	if (input->MouseButtonPressed[0] == true)
	{
		for (int index = 0; index < 73; index++)
		{
			if (input->MouseY >(NameInputButtons[index].ScreenY - 15)
				&& input->MouseY < (NameInputButtons[index].ScreenY + 15)
				&& input->MouseX >(NameInputButtons[index].ScreenX - 20)
				&& input->MouseX < (NameInputButtons[index].ScreenX + 20))
			{
				NameInputButtons[index].Scale = .75;
				NameInputButtons[index].ScaleTimer = 10;

				input->DelayAllUserInput = 10;

				ScreenIsDirty = true;

				audio->PlayDigitalSoundFX(0, 0);
			}
		}
	}

	for (int index = 0; index < 73; index++)
	{
		if (NameInputButtons[index].ScaleTimer > 0)  NameInputButtons[index].ScaleTimer--;
		else if (NameInputButtons[index].ScaleTimer == 0)
		{
			NameInputButtons[index].ScaleTimer = -1;
			NameInputButtons[index].Scale = 1;

			if (index < 71)
			{
				if (data->NameInputArayIndex < 18)
					data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex]
					= characterArray[index];

				if (data->NameInputArayIndex < 19)  data->NameInputArayIndex++;
			}
			else if (index == 71)
			{
				if (data->NameInputArayIndex == 19)
				{
					data->NameInputArayIndex--;
					data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
				}

				data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
				if (data->NameInputArayIndex > 0)  data->NameInputArayIndex--;
				data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';

				audio->PlayDigitalSoundFX(2, 0);
			}
			else if (index == 72)  ScreenTransitionStatus = FadeOut;

			ScreenIsDirty = true;
		}
	}

	if (ScreenIsDirty == 1)
	{
        visuals->Sprites[2].ScreenX = 320;
        visuals->Sprites[2].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(2);

        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = 200;
        visuals->DrawSpriteOntoScreenBuffer(0);

        visuals->DrawTextOntoScreenBuffer("H I G H   S C O R E   N A M E   I N P U T:", visuals->Font[0]
                                          , 0, 1, JustifyCenter, 255, 255, 1, 90, 90, 1);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 7, JustifyCenter
                                          , 255, 255, 1, 90, 90, 1);

        visuals->DrawTextOntoScreenBuffer("You have achieved a new high score!", visuals->Font[0],
                                          0, 80-30, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Enter your name using the keyboard or mouse:", visuals->Font[0],
                                          0, 125-40, JustifyCenter, 255, 255, 255, 1, 1, 1);

        if (data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] != (char)NULL)
            visuals->DrawTextOntoScreenBuffer(data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver],
                                              visuals->Font[0], 0, 207-60, JustifyCenter, 255, 255, 0, 1, 1, 1);

		int intScreenY = 216;
		int offsetY = 30;
		int screenX = 69;
		for (int index = 300; index < 313; index++)
		{
			visuals->Sprites[index].ScreenX = screenX;
			visuals->Sprites[index].ScreenY = intScreenY;
			visuals->Sprites[index].ScaleX = NameInputButtons[index - 300].Scale;
			visuals->Sprites[index].ScaleY = NameInputButtons[index - 300].Scale;
			visuals->DrawSpriteOntoScreenBuffer(index);

			NameInputButtons[index - 300].ScreenX = screenX;
			NameInputButtons[index - 300].ScreenY = visuals->Sprites[index].ScreenY;

			screenX += 41;
		}

		intScreenY += offsetY;
		screenX = 69;
		for (int index = 313; index < 326; index++)
		{
			visuals->Sprites[index].ScreenX = screenX;
			visuals->Sprites[index].ScreenY = intScreenY;
			visuals->Sprites[index].ScaleX = NameInputButtons[index - 300].Scale;
			visuals->Sprites[index].ScaleY = NameInputButtons[index - 300].Scale;
			visuals->DrawSpriteOntoScreenBuffer(index);

			NameInputButtons[index - 300].ScreenX = screenX;
			NameInputButtons[index - 300].ScreenY = visuals->Sprites[index].ScreenY;

			screenX += 41;
		}

		intScreenY += offsetY;
		screenX = 69;

		for (int index = 326; index < 339; index++)
		{
			visuals->Sprites[index].ScreenX = screenX;
			visuals->Sprites[index].ScreenY = intScreenY;
			visuals->Sprites[index].ScaleX = NameInputButtons[index - 300].Scale;
			visuals->Sprites[index].ScaleY = NameInputButtons[index - 300].Scale;
			visuals->DrawSpriteOntoScreenBuffer(index);

			NameInputButtons[index - 300].ScreenX = screenX;
			NameInputButtons[index - 300].ScreenY = visuals->Sprites[index].ScreenY;

			screenX += 41;
		}

		intScreenY += offsetY;
		screenX = 69;
		for (int index = 339; index < 352; index++)
		{
			visuals->Sprites[index].ScreenX = screenX;
			visuals->Sprites[index].ScreenY = intScreenY;
			visuals->Sprites[index].ScaleX = NameInputButtons[index - 300].Scale;
			visuals->Sprites[index].ScaleY = NameInputButtons[index - 300].Scale;
			visuals->DrawSpriteOntoScreenBuffer(index);

			NameInputButtons[index - 300].ScreenX = screenX;
			NameInputButtons[index - 300].ScreenY = visuals->Sprites[index].ScreenY;

			screenX += 41;
		}

		intScreenY += offsetY;
		screenX = 69 + (3 * 41);
		for (int index = 352; index < 359; index++)
		{
			visuals->Sprites[index].ScreenX = screenX;
			visuals->Sprites[index].ScreenY = intScreenY;
			visuals->Sprites[index].ScaleX = NameInputButtons[index - 300].Scale;
			visuals->Sprites[index].ScaleY = NameInputButtons[index - 300].Scale;
			visuals->DrawSpriteOntoScreenBuffer(index);

			NameInputButtons[index - 300].ScreenX = screenX;
			NameInputButtons[index - 300].ScreenY = visuals->Sprites[index].ScreenY;

			screenX += 41;
		}

		intScreenY += offsetY;
		screenX = 69;
		for (int index = 359; index < 372; index++)
		{
			visuals->Sprites[index].ScreenX = screenX;
			visuals->Sprites[index].ScreenY = intScreenY;
			visuals->Sprites[index].ScaleX = NameInputButtons[index - 300].Scale;
			visuals->Sprites[index].ScaleY = NameInputButtons[index - 300].Scale;
			visuals->DrawSpriteOntoScreenBuffer(index);

			NameInputButtons[index - 300].ScreenX = screenX;
			NameInputButtons[index - 300].ScreenY = visuals->Sprites[index].ScreenY;

			screenX += 41;
		}

		intScreenY += offsetY;

		visuals->Sprites[372].ScreenX = 69 + (6 * 41);
		visuals->Sprites[372].ScreenY = intScreenY;
		visuals->Sprites[372].ScaleX = NameInputButtons[72].Scale;
		visuals->Sprites[372].ScaleY = NameInputButtons[72].Scale;
		visuals->DrawSpriteOntoScreenBuffer(372);

		NameInputButtons[72].ScreenX = visuals->Sprites[372].ScreenX;
		NameInputButtons[72].ScreenY = visuals->Sprites[372].ScreenY;

		visuals->DrawTextOntoScreenBuffer("_____________________________________"
			, visuals->Font[3], 0, 404-6, JustifyCenter
			, 255, 255, 1, 90, 90, 1);

		interfaces->DisplayAllButtonsOntoScreenBuffer();
	}

	if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
	{
		ScreenTransitionStatus = FadeAll;
		ScreenToDisplay = HighScoresScreen;

		for (int index = 300; index < 347; index++)
		{
			visuals->Sprites[index].ScaleX = 1;
			visuals->Sprites[index].ScaleY = 1;
		}

		if (data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] == '\0')
			data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] = ' ';
	}
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayNameInputJoystickScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        interfaces->CreateButton(1008, 0, 454);

        ScreenTransitionStatus = FadeIn;

        data->NameInputJoyCharX = 0;
        data->NameInputJoyCharY = 0;
        data->NameInputJoyChar = 'A';
        data->NewHighScoreRegistered = false;

        data->NameInputArayIndex = 0;
    }

    interfaces->ProcessAllButtons();

    if (input->JoystickButtonOne[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == ON)
    {
        if (input->JoystickButton1Pressed[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == false)
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
                input->JoystickButton1Pressed[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] = true;
            }
        }
    }
    else if (input->JoystickButtonTwo[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == ON)
    {
        if (input->JoystickButton2Pressed[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == false)
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
            input->JoystickButton2Pressed[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] = true;
        }
    }
    else
    {
        input->JoystickButton1Pressed[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] = false;
        input->JoystickButton2Pressed[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] = false;
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
    else if (input->JoystickDirectionHorizonal[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == LEFT)
    {
        if (data->NameInputJoyCharX > 0)  data->NameInputJoyCharX--;
        else  data->NameInputJoyCharX = 12;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = true;
        input->DelayAllUserInput = 6;
    }
    else if (input->JoystickDirectionHorizonal[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == RIGHT)
    {
        if (data->NameInputJoyCharX < 12)  data->NameInputJoyCharX++;
        else  data->NameInputJoyCharX = 0;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = true;
        input->DelayAllUserInput = 6;
    }

    if (data->NameInputJoyCharY == 0)
    {
        data->NameInputJoyChar = (65+data->NameInputJoyCharX);
    }
    else if (data->NameInputJoyCharY == 1)
    {
        data->NameInputJoyChar = (78+data->NameInputJoyCharX);
    }
    else if (data->NameInputJoyCharY == 2)
    {
        data->NameInputJoyChar = (97+data->NameInputJoyCharX);
    }
    else if (data->NameInputJoyCharY == 3)
    {
        data->NameInputJoyChar = (110+data->NameInputJoyCharX);
    }
    else if (data->NameInputJoyCharY == 4 && data->NameInputJoyCharX < 10)
    {
        data->NameInputJoyChar = (48+data->NameInputJoyCharX);
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

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[0], 0, 7, JustifyCenter
                                          , 255, 255, 1, 90, 90, 1);

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
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 239
                                                  , JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
            else
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 239
                                                  , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            x+=45;
        }

        x = 45;
        for (char alphaNumeral = 'N'; alphaNumeral < '['; alphaNumeral++)
        {
            sprintf(visuals->VariableText, "%c", alphaNumeral);

            if (alphaNumeral == data->NameInputJoyChar)
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 269
                                                  , JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
            else
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 269
                                                  , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            x+=45;
        }

        x = 45;
        for (char alphaNumeral = 'a'; alphaNumeral < 'n'; alphaNumeral++)
        {
            sprintf(visuals->VariableText, "%c", alphaNumeral);

            if (alphaNumeral == data->NameInputJoyChar)
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 299
                                                  , JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
            else
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 299
                                                  , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            x+=45;
        }

        x = 45;
        for (char alphaNumeral = 'n'; alphaNumeral < '{'; alphaNumeral++)
        {
            sprintf(visuals->VariableText, "%c", alphaNumeral);

            if (alphaNumeral == data->NameInputJoyChar)
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 329
                                                  , JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
            else
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 329
                                                  , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            x+=45;
        }

        x = 45;
        for (char alphaNumeral = '0'; alphaNumeral < ':'; alphaNumeral++)
        {
            sprintf(visuals->VariableText, "%c", alphaNumeral);

            if (alphaNumeral == data->NameInputJoyChar)
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 359
                                                  , JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
            else
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 359
                                                  , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            x+=45;
        }

        if (data->NameInputJoyChar == '+')
            visuals->DrawTextOntoScreenBuffer("+", visuals->Font[0], x, 359, JustifyCenterOnPoint,
                                              255, 1, 1, 1, 1, 1);
        else
            visuals->DrawTextOntoScreenBuffer("+", visuals->Font[0], x, 359, JustifyCenterOnPoint,
                                              255, 255, 255, 1, 1, 1);
        x+=45;

        if (data->NameInputJoyChar == '_')
            visuals->DrawTextOntoScreenBuffer("_", visuals->Font[0], x, 359, JustifyCenterOnPoint,
                                              255, 1, 1, 1, 1, 1);
        else
            visuals->DrawTextOntoScreenBuffer("_", visuals->Font[0], x, 359, JustifyCenterOnPoint,
                                              255, 255, 255, 1, 1, 1);
        x+=45;

        if (data->NameInputJoyChar == '<')
            visuals->DrawTextOntoScreenBuffer("<", visuals->Font[0], x, 359, JustifyCenterOnPoint,
                                              255, 1, 1, 1, 1, 1);
        else
            visuals->DrawTextOntoScreenBuffer("<", visuals->Font[0], x, 359, JustifyCenterOnPoint,
                                              255, 255, 255, 1, 1, 1);
        x+=45;

        if (data->NameInputJoyCharY == 5)
            visuals->DrawTextOntoScreenBuffer("END", visuals->Font[0], 320, 389, JustifyCenterOnPoint,
                                              255, 1, 1, 1, 1, 1);
        else
            visuals->DrawTextOntoScreenBuffer("END", visuals->Font[0], 320, 389, JustifyCenterOnPoint,
                                              255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[0], 0, 404, JustifyCenter
                                          , 255, 255, 1, 90, 90, 1);

        interfaces->DisplayAllButtonsOntoScreenBuffer();
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = HighScoresScreen;

        logic->CrisisWon = false;

        input->DelayAllUserInput = 20;

        interfaces->DestroyAllButtons();
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayTestComputerSkillScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        logic->SetupForNewGame();

        for (int player = 0; player < 4; player++)
        {
            logic->PlayerData[player].PlayerInput = CPU;
            logic->PlayerData[player].PlayerStatus = NewPieceDropping;
        }

        visuals->FrameLock = 1;

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

    visuals->ClearScreenBufferWithColor(0, 0, 0, 0);

//    if (ScreenIsDirty == true)
    {
        if (logic->PlayerData[0].PlayerStatus != GameOver)
        {
            visuals->Sprites[80].ScreenX = logic->PlayerData[0].PlayersPlayfieldScreenX;
            visuals->Sprites[80].ScreenY = logic->PlayerData[0].PlayersPlayfieldScreenY;
            visuals->Sprites[80].Transparency = 200;
            visuals->Sprites[80].RedHue = 255;
            visuals->Sprites[80].GreenHue = 0;
            visuals->Sprites[80].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(80);
        }

        if (logic->PlayerData[1].PlayerStatus != GameOver)
        {
            visuals->Sprites[80].ScreenX = logic->PlayerData[1].PlayersPlayfieldScreenX;
            visuals->Sprites[80].ScreenY = logic->PlayerData[1].PlayersPlayfieldScreenY;
            visuals->Sprites[80].Transparency = 200;
            visuals->Sprites[80].RedHue = 255;
            visuals->Sprites[80].GreenHue = 255;
            visuals->Sprites[80].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(80);
        }

        if (logic->PlayerData[2].PlayerStatus != GameOver)
        {
            visuals->Sprites[80].ScreenX = logic->PlayerData[2].PlayersPlayfieldScreenX;
            visuals->Sprites[80].ScreenY = logic->PlayerData[2].PlayersPlayfieldScreenY;
            visuals->Sprites[80].Transparency = 200;
            visuals->Sprites[80].RedHue = 0;
            visuals->Sprites[80].GreenHue = 255;
            visuals->Sprites[80].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(80);
        }

        if (logic->PlayerData[3].PlayerStatus != GameOver)
        {
            visuals->Sprites[80].ScreenX = logic->PlayerData[3].PlayersPlayfieldScreenX;
            visuals->Sprites[80].ScreenY = logic->PlayerData[3].PlayersPlayfieldScreenY;
            visuals->Sprites[80].Transparency = 200;
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
            sprintf(visuals->VariableText, "%d", logic->PlayerData[player].Score);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[2]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX, 7
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

            sprintf(visuals->VariableText, "%d", logic->PlayerData[player].Lines);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[2]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX-56, 62
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

            sprintf(visuals->VariableText, "%d", logic->PlayerData[player].Level);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[2]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX+56, 62
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
                visuals->DrawTextOntoScreenBuffer("C.P.U."
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

    visuals->DrawTextOntoScreenBuffer("A.I. TEST", visuals->Font[0]
                                      , 0, 110, JustifyCenter, 255, 255, 255, 0, 0, 0);

    char temp[256];
    strcpy(visuals->VariableText, "Number Of Games: ");
    sprintf(temp, "%d", logic->NumberofCPUGames);
    strcat(visuals->VariableText, temp);
    visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[1], 0, 140
                                      , JustifyCenter, 255, 255, 255, 0, 0, 0);

    strcpy(visuals->VariableText, "Number Of Lines: ");
    sprintf(temp, "%d", logic->TotalCPUPlayerLines);
    strcat(visuals->VariableText, temp);
    visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[1], 0, 160
                                      , JustifyCenter, 255, 255, 255, 0, 0, 0);

    int averageLinesPerGame = 0;
    if (logic->NumberofCPUGames > 0)  averageLinesPerGame = logic->TotalCPUPlayerLines / logic->NumberofCPUGames;
    strcpy(visuals->VariableText, "Average Lines/Game: ");
    sprintf(temp, "%d", averageLinesPerGame);
    strcat(visuals->VariableText, temp);
    visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], 0, 180
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
    visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[1], 0, 205
                                      , JustifyCenter, 255, 255, 255, 0, 0, 0);

    ScreenIsDirty = true;

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

    for (logic->Player = 0; logic->Player < NumberOfPlayers; logic->Player++)
    {
        if (logic->PlayerData[logic->Player].PlayerStatus == GameOver)
        {
            logic->NumberofCPUGames++;
            logic->TotalCPUPlayerLines+=logic->PlayerData[logic->Player].Lines;


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
