/*
    Copyright 2021 Team 16BitSoft

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
_____________________________________________________________________________________________________________________________
                                                    SDL 2.0.14
                                           (SDL2_Image/SDL2_Mixer/SDL2_TTF)
                                         Cross-Platform / M.I.T. Open-Source
  _______ _______     ______   _______    _       _______          _           _______ _                                _
 (_______|_______)   (_____ \ (_______)  (_)     (_______)        | |         (_______) |   Nissan(R) NISMO Powered!   | |
  _   ___    _  _____ _____) )    _ _ _ _ _ ____     _ _   _  ____| |__   ___  _      | |__  _____  ____ ____ _____  __| |
 | | (_  |  | |(_____)  __  /    | | | | | |  _ \   | | | | |/ ___)  _ \ / _ \| |     |  _ \(____ |/ ___) _  | ___ |/ _  |
 | |___) |  | |      | |  \ \    | | | | | | | | |  | | |_| | |   | |_) ) |_| | |_____| | | / ___ | |  ( (_| | ____( (_| |
  \_____/   |_|      |_|   |_|   |_|\___/|_|_| |_|  |_|____/|_|   |____/ \___/ \______)_| |_\_____|_|   \___ |_____)\____|
                                                                                                       (_____|Version 3.0
                                           2-Dimensional Video Game Engine
.............................................................................................................................

                                         "T-Crisis 4 110% A.I. Turbo Remix"
                                               Retail3 Version 4.5.6

                                      Post Release Candidate / NOT Retail Yet!

                                                   Team 16BitSoft
_____________________________________________________________________________________________________________________________
*/
#include <stdio.h>
#include <cstring>
#include <time.h>
#include <cstdlib>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "visuals.h"
#include "input.h"
#include "screens.h"
#include "interface.h"
#include "audio.h"
#include "data.h"
#include "logic.h"

Visuals *visuals;
Input *input;
Screens *screens;
Interface *interface;
Audio *audio;
Data *data;
Logic *logic;

//-------------------------------------------------------------------------------------------------
int main( int argc, char* args[] )
{
    printf("''GT-R Twin TurboCharged'' game engine started!\n");
    argc = argc; args = args;

    if ( SDL_Init(SDL_INIT_EVERYTHING) != 0 )
    {
        printf( "Unable to initialize SDL2: %s\n", SDL_GetError() );
        return(1);
    }
    else
    {
        SDL_version compiled;
        SDL_VERSION(&compiled);
        printf("SDL2 version %d.%d.%d initialized.\n", compiled.major, compiled.minor, compiled.patch);
    }

    visuals = new Visuals();

    if ( visuals->InitializeWindow() != true ) visuals->CoreFailure = true;
    if ( visuals->LoadFontsIntoMemory() != true )  visuals->CoreFailure = true;
    if ( visuals->LoadSpritesAndInitialize() != true )  visuals->CoreFailure = true;

    input = new Input();

    screens = new Screens();

    interface = new Interface();

    data = new Data();

    srand( (unsigned)time(NULL) ); // Place unique time seed into random number generator.

    logic = new Logic();

    audio = new Audio();
    audio->SetupAudio();

    data->LoadHighScoresAndOptions();

    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0" );

    if (visuals->FullScreenMode == 1 || visuals->FullScreenMode == 3)  SDL_SetWindowFullscreen(visuals->Window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    //-MAIN-LOOP------------------------------------------------------------------------
    printf("-Main loop started-----------------------------\n");
    while (visuals->CoreFailure != true && input->EXIT_Game != true)
    {
        input->GetAllUserInput();
        visuals->CalculateFramerate();
        screens->ProcessScreenToDisplay();
        visuals->ProcessFramerate();
    }

    if (visuals->CoreFailure == true)  printf("*****ERROR: Game crashed in Main loop*****\n");
    else  printf("-----------------------------Main loop ended-\n");
    //------------------------------------------------------------------------MAIN-LOOP-

    data->SaveHighScoresAndOptions();

    delete logic;
    delete data;
    delete audio;
    delete interface;
    delete screens;
    delete input;
    delete visuals;
    SDL_Quit();
    printf("SDL2 closed.\n");
    printf("''GT-R Twin TurboCharged'' game engine ended!\n");
    return(0);
}

// "A 110% By Team 16BitSoft!"
