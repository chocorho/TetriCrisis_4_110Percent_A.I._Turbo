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

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "audio.h"

#include "screens.h"
#include "visuals.h"

extern Screens* screens;
extern Visuals* visuals;

//-------------------------------------------------------------------------------------------------
Audio::Audio(void)
{
    MusicVolume = 64;
    SoundVolume = 64;
    CurrentlySelectedMusicTrack = 0;
    MusicJukeboxMode = 0;

    for (int soundIndex = 0; soundIndex < SoundTotal; soundIndex++)  SoundFX[soundIndex] = NULL;
    for (int musicIndex = 0; musicIndex < MusicTotal; musicIndex++)  MusicTrack[musicIndex] = NULL;

    AudioWorking = false;
}

//-------------------------------------------------------------------------------------------------
Audio::~Audio(void)
{
    if (AudioWorking == false)  return;

    Mix_HaltChannel(-1);
    for (int soundIndex = 0; soundIndex < SoundTotal; soundIndex++)
    {
        if (SoundFX[soundIndex] != NULL)  Mix_FreeChunk(SoundFX[soundIndex]);
        SoundFX[soundIndex] = NULL;
    }
    Mix_AllocateChannels(0);
    printf("Unloaded all music from memory.\n");

    Mix_HaltMusic();
    for (int musicIndex = 0; musicIndex < MusicTotal; musicIndex++)
    {
        if (MusicTrack[musicIndex] != NULL)  Mix_FreeMusic(MusicTrack[musicIndex]);
        MusicTrack[musicIndex] = NULL;
    }
    printf("Unloaded all sounds from memory.\n");

    Mix_CloseAudio();
    Mix_Quit();
    printf("SDL_mixer closed.\n");
}

//-------------------------------------------------------------------------------------------------
void Audio::SetupAudio(void)
{
    strcpy(Mix_Init_Error, "SDL2_Mixer: OK");

    int flags = MIX_INIT_OGG|MIX_INIT_MOD;

    int initted = Mix_Init(flags);
    if (initted&flags && !flags)
    {
        printf("Mix_Init: Failed to init required ogg and mod support!\n");
        printf("Mix_Init: %s\n", Mix_GetError());

        strcpy(Mix_Init_Error, "SDL2_Mixer ERROR: ");
        strcat( Mix_Init_Error, Mix_GetError() );

        AudioWorking = false;
        return;
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 2048)==-1)
    {
        printf("ERROR: SDL2_Mixer init failed: %s\n", Mix_GetError());

        strcpy(Mix_Init_Error, "SDL2_Mixer ERROR: ");
        strcat( Mix_Init_Error, Mix_GetError() );

        AudioWorking = false;
        return;
    }
    else printf("SDL2_Mixer initialized.\n");

    Mix_AllocateChannels(16);
    for (int channel = 0; channel < 16; channel++)
    {
        Mix_Volume(channel, SoundVolume);
    }

    for (int index = 0; index < MusicTotal; index++)
    {
        switch(index)
        {
            case 0:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Title.mod");
                break;

            case 1:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-1.mod");
                break;

            case 2:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-2.mod");
                break;

            case 3:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-3.mod");
                break;

            case 4:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-4.mod");
                break;

            case 5:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-5.mod");
                break;

            case 6:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-6.mod");
                break;

            case 7:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-7.mod");
                break;

            case 8:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-8.mod");
                break;

            case 9:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-9.mod");
                break;

            case 10:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-10.mod");
                break;

            case 11:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-11.mod");
                break;

            case 12:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-12.mod");
                break;

            case 13:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-13.mod");
                break;

            case 14:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-14.mod");
                break;

            case 15:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-15.mod");
                break;

            case 16:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-16.mod");
                break;

            case 17:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-17.mod");
                break;

            case 18:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-18.mod");
                break;

            case 19:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-19.mod");
                break;

            case 20:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-19.mod");
                break;

            case 21:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-19.mod");
                break;

            case 22:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-19.mod");
                break;

            case 23:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Track-19.mod");
                break;

            case 24:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Level7.mod");
                break;

            case 25:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Ending.ogg");
                break;

            case 26:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Story0-2.ogg");
                break;

            case 27:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Story3-5.ogg");
                break;

            case 28:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Story6-8.ogg");
                break;

            case 29:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-Story9.ogg");
                break;

            case 30:
                MusicTrack[index] = Mix_LoadMUS("data/music/BGM-StoryEnd.ogg");
                break;

            default:
                break;
        }

        if(!MusicTrack[index] && MusicTotal != 0)
        {
            printf("ERROR: Couldn't load music %s\n", Mix_GetError());
        }
        else  Mix_VolumeMusic(MusicVolume);
    }
    printf("Loaded all music into memory.\n");

    for (int index = 0; index < SoundTotal; index++)
    {
        switch(index)
        {
            case 0:
                SoundFX[index] = Mix_LoadWAV("data/effects/SFX-Menu-Move.ogg");
                break;

            case 1:
                SoundFX[index] = Mix_LoadWAV("data/effects/SFX-Menu-Click.ogg");
                break;

            case 2:
                SoundFX[index] = Mix_LoadWAV("data/effects/SFX-Move-Piece.ogg");
                break;

            case 3:
                SoundFX[index] = Mix_LoadWAV("data/effects/SFX-Piece-Collision.ogg");
                break;

            case 4:
                SoundFX[index] = Mix_LoadWAV("data/effects/SFX-Piece-Drop.ogg");
                break;

            case 5:
                SoundFX[index] = Mix_LoadWAV("data/effects/SFX-Piece-Rotate.ogg");
                break;

            case 6:
                SoundFX[index] = Mix_LoadWAV("data/effects/SFX-Line-Cleared.ogg");
                break;

            case 7:
                SoundFX[index] = Mix_LoadWAV("data/effects/SFX-Tetri-Cleared.ogg");
                break;

            case 8:
                SoundFX[index] = Mix_LoadWAV("data/effects/SFX-Level-Up.ogg");
                break;

            case 9:
                SoundFX[index] = Mix_LoadWAV("data/effects/SFX-Must-Think-In-Russian.ogg");
                break;

            case 10:
                SoundFX[index] = Mix_LoadWAV("data/effects/SFX-Incoming-Line.ogg");
                break;

            case 11:
                SoundFX[index] = Mix_LoadWAV("data/effects/SFX-Game-Over.ogg");
                break;

            case 12:
                SoundFX[index] = Mix_LoadWAV("data/effects/SFX-Crack.ogg");
                break;

            case 13:
                SoundFX[index] = Mix_LoadWAV("data/effects/SFX-Shall-We-Play-A-Game.ogg");
                break;

            case 14:
                SoundFX[index] = Mix_LoadWAV("data/effects/SFX-Sword.ogg");
                break;

            case 15:
                SoundFX[index] = Mix_LoadWAV("data/effects/SFX-Danger.ogg");
                break;

            case 16:
                SoundFX[index] = Mix_LoadWAV("data/effects/SFX-Thrust.ogg");
                break;

            case 17:
                SoundFX[index] = Mix_LoadWAV("data/effects/SFX-Explosion.ogg");
                break;


            default:
                break;
        }

        if(!SoundFX[index])
        {
            printf("ERROR: Couldn't load sound effect %s\n", Mix_GetError());
        }
        else  Mix_VolumeChunk(SoundFX[index], MIX_MAX_VOLUME);
    }
    printf("Loaded all sounds into memory.\n");

    AudioWorking = true;
}

//-------------------------------------------------------------------------------------------------
void Audio::PlayMusic(Uint8 musicIndex, int loop)
{
    if (AudioWorking == false)  return;

    if (MusicTrack[musicIndex] == NULL)  return;

    if (musicIndex >= MusicTotal)  return;


    if (Mix_PlayingMusic() == 1)  Mix_HaltMusic();

    Mix_VolumeMusic(MusicVolume);

    CurrentlySelectedMusicTrack = musicIndex;

    CurrentMusicTrackPlaying = musicIndex;

    if (MusicVolume == 0)  return;

    if(Mix_PlayMusic(MusicTrack[musicIndex], loop)==-1)
    {
        printf( "Mix_PlayMusic: %s\n", Mix_GetError() );
    }
}

//-------------------------------------------------------------------------------------------------
void Audio::PlayDigitalSoundFX(Uint8 soundIndex, int loops)
{
    if (AudioWorking == false)  return;

    if (SoundFX[soundIndex] == NULL)  return;

    if (soundIndex >= SoundTotal)  return;

    if (SoundVolume == 0)  return;

    for (int index = 1; index < 16; index++)
        Mix_Volume( index, SoundVolume );

    for (int indexTwo = 0; indexTwo < SoundTotal; indexTwo++)
        Mix_VolumeChunk(SoundFX[indexTwo], SoundVolume);

    if(Mix_PlayChannel(-1, SoundFX[soundIndex], loops)==-1)
    {
        printf( "Mix_PlayChannel: %s\n",Mix_GetError() );
    }
}
//-------------------------------------------------------------------------------------------------
