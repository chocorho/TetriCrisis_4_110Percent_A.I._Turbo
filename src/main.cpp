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
_____________________________________________________________________________________________________________________________
                                                     SDL 2.0.12
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

                                                   Team 16BitSoft
_____________________________________________________________________________________________________________________________


***NOTE: Saved options and high scores will reset UNTIL: "Release Candidate"!!!


Release Candidate To Do List:


- Finalize music soundtrack.

- Finalize "Story+Mode".

- Test thoroughly for issues.


__________________________________________________________________________________________________

Beta3 Change List:


- Fixed "Story+Mode" clearing of playfield after playing for shortwhile.
(should only happen in "Crisis+Mode")

- Created "Story+Mode" game mode storyboard image display system.
(black and white sketches are temporary until artist finishes the work)

- Implemented display "Volume OFF" for music track/mode when music volume is set to OFF.
(not sure why you would mute the music - it rocks!)

- Implemented level up system in "Story+Mode".
(requires more line clears after each level)

- Fixed "Original Mode" ending at level 10, it's now never-ending.
(try some of the other game modes for more fun!)

- Fixed full screen desktop screen curruption.

- Implemented "hats" for joysticks.

- Fixed window resize(larger/maximize) screen corruption at top-left.

- Implemented first draft of animated intro and ending scenes in "Story+Mode".


__________________________________________________________________________________________________

Beta2 Change List:


- Lowered default high scores.
(in some modes the higher scores were impossible to beat)

- Added "on-the-fly" average lines/game display to A.I. testing screen.
([Shift]+[T] to activate on 16BitSoft screen)

- Changed game window title to: "T-Crisis 4 110% A.I. Turbo Remix".
(LOL - is the name long enough:)

- Modified staff screen texts a little and added Palm T-Crisis images/review.
(hope we did not forget anyone!)

- Optimized the staff screen.
(game runs 60FPS on an 11 year old 1.2GHz thin client)

- Fixed total completed lines counter in A.I. testing screen.
(game currently does about 100,000 completed lines per game)

- Optimized and Added F.P.S. to A.I. testing screen.
(pressing [T] key during test will toggle playfield display on/off)

- Introduced "Story+Mode" game mode.
(will feature story images from a paid graphic artist and a completely new music soundtrack!)

- Optimized [Options] configuration screen.
(thanks to "fogobogo" for spotting above on his RPi4!)

- Added "Danger!" voice to "Story+Mode" game mode.
(it's my voice - could not afford a voice actor)

- Fixed an infrequent crash when game is over and a new high score is acheived.
(was new high score name setting to NULL then trying to be displayed on screen: title/high scores)

- Implemented keep aspect ration for windowed and full screen mode.
(special thanks to "slvn_" for their help with above!)

- Fixed in-game mouse control.
(mouse input when playing is not ideal, but it's there as an option)

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
    else  printf("SDL2 initialized.\n");

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

    //-MAIN-LOOP------------------------------------------------------------------------
    printf("-Main loop started-----------------------------\n");
    while (visuals->CoreFailure != true && input->EXIT_Game != true)
    {
        input->GetAllUserInput();
        visuals->CalculateFramerate();
        screens->ProcessScreenToDisplay();
        visuals->ProcessFramerate();

        if (screens->ClearScreenWithBlackWhen1stRunInFullScreenMode == false)
        {
            screens->ClearScreenWithBlackWhen1stRunInFullScreenMode = true;
            visuals->ClearScreenBufferWithColor(0, 0, 0, 255);
            SDL_RenderPresent(visuals->Renderer);
            if (visuals->FullScreenMode == 1 || visuals->FullScreenMode == 3)  SDL_SetWindowFullscreen(visuals->Window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        }
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
