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
_____________________________________________________________________________________________________________________________
                                                      JeZxLee's
                                                      SDL 2.0.12
                                            Cross-Platform / Open-Source
  _______ _______     ______   _______    _       _______          _           _______ _                                _ TM
 (_______|_______)   (_____ \ (_______)  (_)     (_______)        | |         (_______) |      "Retro Blast Tech"      | |
  _   ___    _  _____ _____) )    _ _ _ _ _ ____     _ _   _  ____| |__   ___  _      | |__  _____  ____ ____ _____  __| |
 | | (_  |  | |(_____)  __  /    | | | | | |  _ \   | | | | |/ ___)  _ \ / _ \| |     |  _ \(____ |/ ___) _  | ___ |/ _  |
 | |___) |  | |      | |  \ \    | | | | | | | | |  | | |_| | |   | |_) ) |_| | |_____| | | / ___ | |  ( (_| | ____( (_| |
  \_____/   |_|      |_|   |_|   |_|\___/|_|_| |_|  |_|____/|_|   |____/ \___/ \______)_| |_\_____|_|   \___ |_____)\____|
                                                                                                       (_____|
                                           2-Dimensional Video Game Engine

                                       (C)opyright 2020 By Team 16BitSoft Inc.
.............................................................................................................................
                                                                         TM
                                           "TetriCrisis 4 110% A.I. Turbo"
                                                    (REMASTERED)
                                      (W.I.P.) Retail2 Version 4.5.6 (W.I.P.)

                                         (C)opyright 2020 By 16BitSoft Inc.
                                                  www.16BitSoft.com
_____________________________________________________________________________________________________________________________
*/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "visuals.h"
#include "input.h"
#include "screens.h"
#include "interfaces.h"
#include "audio.h"
#include "data.h"
#include "logic.h"

Visuals *visuals;
Input *input;
Screens *screens;
Interfaces *interfaces;
Audio *audio;
Data *data;
Logic *logic;

//-------------------------------------------------------------------------------------------------
int main( int argc, char* args[] )
{
    printf("JeZxLee's ''GT-R Twin TurboCharged'' game engine started!\n");
    argc = argc; args = args;

    if ( SDL_Init(SDL_INIT_EVERYTHING) != 0 )
    {
        printf( "Unable to initialize SDL2: %s\n", SDL_GetError() );
        return(1);
    }
    else  printf("SDL2 initialized.\n");

    visuals = new Visuals();

    if ( visuals->InitializeWindow() != true ) visuals->CoreFailure = true;
    if ( visuals->LoadFontsIntoMemory() != true )  visuals->CoreFailure = true;
    if ( visuals->LoadSpritesAndInitialize() != true )  visuals->CoreFailure = true;

    input = new Input();

    screens = new Screens();

    interfaces = new Interfaces();

    data = new Data();

    srand( (unsigned)time(NULL) ); // Place unique time seed into random number generator.
    logic = new Logic();

    audio = new Audio();
    audio->SetupAudio();

    data->LoadHighScoresAndOptions();

    if (visuals->FullScreenMode == true)  SDL_SetWindowFullscreen(visuals->Window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    audio->PlayMusic(0, -1);

    //-MAIN-LOOP------------------------------------------------------------------------
    printf("Main loop started...\n");
    while (visuals->CoreFailure != true && input->EXIT_Game != true)
    {
        input->GetAllUserInput();
        visuals->CalculateFramerate();
        screens->ProcessScreenToDisplay();
        visuals->ProcessFramerate();
    }
    printf("...Main loop exited\n");
    //------------------------------------------------------------------------MAIN-LOOP-

    data->SaveHighScoresAndOptions();

    delete logic;
    delete data;
    delete audio;
    delete interfaces;
    delete screens;
    delete input;
    delete visuals;
    SDL_Quit();
    printf("SDL2 closed.\n");
    printf("JeZxLee's ''GT-R Twin TurboCharged'' game engine ended!\n");
    return(0);
}

//                                   TM
// "A 110% By Team www.16BitSoft.com!"
