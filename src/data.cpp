/*
    Copyright 2017 Team www.16BitSoft.com

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

using namespace std;
#include <fstream>
#include <string.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "data.h"

#include "audio.h"
#include "input.h"
#include "logic.h"
#include "visuals.h"

extern Audio* audio;
extern Input* input;
extern Logic* logic;
extern Visuals* visuals;

//-------------------------------------------------------------------------------------------------
Data::Data(void)
{
    ClearHighScores();

    PlayerWithHighestScore = 255;
    PlayerRankOnGameOver = 10;
    NameInputJoyCharX = 0;
    NameInputJoyCharY = 0;
    NameInputJoyChar = (char)NULL;
    NewHighScoreRegistered = false;
}

//-------------------------------------------------------------------------------------------------
Data::~Data(void)
{

}

//-------------------------------------------------------------------------------------------------
void Data::CheckForNewHighScore(void)
{
int playerRank = 10;

    if (logic->PlayerData[0].Score >= logic->PlayerData[1].Score
        && logic->PlayerData[0].Score >= logic->PlayerData[2].Score
        && logic->PlayerData[0].Score >= logic->PlayerData[3].Score)  PlayerWithHighestScore = 0;

    if (logic->PlayerData[1].Score >= logic->PlayerData[0].Score
        && logic->PlayerData[1].Score >= logic->PlayerData[2].Score
        && logic->PlayerData[1].Score >= logic->PlayerData[3].Score)  PlayerWithHighestScore = 1;

    if (logic->PlayerData[2].Score >= logic->PlayerData[0].Score
        && logic->PlayerData[2].Score >= logic->PlayerData[1].Score
        && logic->PlayerData[2].Score >= logic->PlayerData[3].Score)  PlayerWithHighestScore = 2;

    if (logic->PlayerData[3].Score >= logic->PlayerData[0].Score
        && logic->PlayerData[3].Score >= logic->PlayerData[1].Score
        && logic->PlayerData[3].Score >= logic->PlayerData[2].Score)  PlayerWithHighestScore = 3;


    if (logic->PlayerData[PlayerWithHighestScore].PlayerInput == CPU)  return;

    for (int rank = 0; rank < 10; rank++)
    {
        if (logic->PlayerData[PlayerWithHighestScore].Score >= HighScoresScore[logic->GameMode][rank])
        {
            playerRank = rank;

            for (int index = 9; index > rank; index--)
            {
                strcpy(HighScoresName[logic->GameMode][index], HighScoresName[logic->GameMode][index-1]);
                HighScoresLevel[logic->GameMode][index] = HighScoresLevel[logic->GameMode][index-1];
                HighScoresScore[logic->GameMode][index] = HighScoresScore[logic->GameMode][index-1];
            }

            char erase[20];
            for (int letter = 0; letter < 20; letter++)  erase[letter] = '\0';
            for (int letterTwo = 0; letterTwo < 20; letterTwo++)  HighScoresName[logic->GameMode][rank][letterTwo] = '\0';
            strcpy(HighScoresName[logic->GameMode][rank], erase);

            HighScoresLevel[logic->GameMode][rank] = logic->PlayerData[PlayerWithHighestScore].Level;
            HighScoresScore[logic->GameMode][rank] = logic->PlayerData[PlayerWithHighestScore].Score;

            rank = 10;
        }
    }

    HighScoresName[logic->GameMode][playerRank][0] = ' ';

    PlayerRankOnGameOver = playerRank;
    if (PlayerRankOnGameOver == 10)  PlayerWithHighestScore = 255;

    NameInputJoyCharX = 0;
    NameInputJoyCharY = 0;
    NameInputJoyChar = 'A';
}

//-------------------------------------------------------------------------------------------------
void Data::ClearHighScores(void)
{
char name1st[19]  = { 'J', 'e', 'Z', 'x', 'L', 'e', 'e', '\0' };
char name2nd[19]  = { 'D', 'a', 'o', 't', 'h', 'e', 'm', 'a', 'n', '\0' };
char name3rd[19]  = { 'm', 'a', 't', 't', 'm', 'a', 't', 't', 'e', 'h', '\0' };
char name4th[19]  = { 'Y', 'o', 'u', '!', '\0' };
char name5th[19]  = { 'Y', 'o', 'u', '!', '\0' };
char name6th[19]  = { 'Y', 'o', 'u', '!', '\0' };
char name7th[19]  = { 'Y', 'o', 'u', '!', '\0' };
char name8th[19]  = { 'Y', 'o', 'u', '!', '\0' };
char name9th[19]  = { 'Y', 'o', 'u', '!', '\0' };
char name10th[19] = { 'Y', 'o', 'u', '!', '\0' };

    for (int gameMode = 0; gameMode < 6; gameMode++)
        for (int index = 0; index < 10; index++)
            for (int indexTwo = 0; indexTwo < 20; indexTwo++)
                HighScoresName[gameMode][index][indexTwo] = (char)NULL;

    for (int gameMode = 0; gameMode < 6; gameMode++)
    {
        for (int loop = 0; loop < 10; loop++)
        {
            switch (loop)
            {
                case 0:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name1st[index];

                    HighScoresLevel[gameMode][loop] = 10;

                    if (gameMode != CrisisMode)  HighScoresScore[gameMode][loop] = 10000;
                    else  HighScoresScore[gameMode][loop] = 45396;
                    break;

                case 1:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name2nd[index];

                    HighScoresLevel[gameMode][loop] = 9;
                    HighScoresScore[gameMode][loop] = 9000;
                    break;

                case 2:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name3rd[index];

                    HighScoresLevel[gameMode][loop] = 8;
                    HighScoresScore[gameMode][loop] = 8000;
                    break;

                case 3:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name4th[index];

                    HighScoresLevel[gameMode][loop] = 7;
                    HighScoresScore[gameMode][loop] = 7000;
                    break;

                case 4:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name5th[index];

                    HighScoresLevel[gameMode][loop] = 6;
                    HighScoresScore[gameMode][loop] = 6000;
                    break;

                case 5:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name6th[index];

                    HighScoresLevel[gameMode][loop] = 5;
                    HighScoresScore[gameMode][loop] = 5000;
                    break;

                case 6:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name7th[index];

                    HighScoresLevel[gameMode][loop] = 4;
                    HighScoresScore[gameMode][loop] = 4000;
                    break;

                case 7:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name8th[index];

                    HighScoresLevel[gameMode][loop] = 3;
                    HighScoresScore[gameMode][loop] = 3000;
                    break;

                case 8:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name9th[index];

                    HighScoresLevel[gameMode][loop] = 2;
                    HighScoresScore[gameMode][loop] = 2000;
                    break;

                case 9:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name10th[index];

                    HighScoresLevel[gameMode][loop] = 1;
                    HighScoresScore[gameMode][loop] = 1000;
                    break;

                default:
                    break;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Data::LoadHighScoresAndOptions(void)
{
char filename[256];
fstream fileStream;
char textBuffer[50];
char *base_path = SDL_GetPrefPath("16BitSoftInc", "TetriCrisis4-SDL2");
char *pref_path = NULL;

    if (base_path)
    {
        pref_path = SDL_strdup(base_path);
        SDL_free(base_path);
    }
    else  return;

	strcpy(filename, pref_path);
	strcat(filename, "TetriCrisis4-Data-Retail4_5_6");

	fileStream.open (filename, fstream::in);
	if (fileStream.is_open())
	{
		fileStream.getline (textBuffer, 30);
		audio->MusicVolume = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		audio->SoundVolume = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		visuals->FullScreenMode = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->CPUPlayerEnabled = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->PlayingGameFrameLock = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->DelayAutoShift = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->GameMode = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->SelectedMusicTrack = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->SelectedBackground = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->NewGameGarbageHeight = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->PressingUPAction = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->DisplayNextPiece = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->DisplayDropShadow = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->TileSet = (int)atoi(textBuffer);

        fileStream.getline (textBuffer, 30);
        input->UserDefinedKeyButtonOne = (int)atoi(textBuffer);

        fileStream.getline (textBuffer, 30);
        input->UserDefinedKeyButtonTwo = (int)atoi(textBuffer);

        fileStream.getline (textBuffer, 30);
        input->UserDefinedKeyUP = (int)atoi(textBuffer);

        fileStream.getline (textBuffer, 30);
        input->UserDefinedKeyRIGHT = (int)atoi(textBuffer);

        fileStream.getline (textBuffer, 30);
        input->UserDefinedKeyDOWN = (int)atoi(textBuffer);

        fileStream.getline (textBuffer, 30);
        input->UserDefinedKeyLEFT = (int)atoi(textBuffer);

        fileStream.getline (textBuffer, 30);
        input->UserDefinedKeyPause = (int)atoi(textBuffer);

        for (int index = 0; index < 3; index++)
        {
            fileStream.getline (textBuffer, 30);
            input->JoyUP[index] = (int)atoi(textBuffer);

            fileStream.getline (textBuffer, 30);
            input->JoyDOWN[index] = (int)atoi(textBuffer);

            fileStream.getline (textBuffer, 30);
            input->JoyLEFT[index] = (int)atoi(textBuffer);

            fileStream.getline (textBuffer, 30);
            input->JoyRIGHT[index] = (int)atoi(textBuffer);

            fileStream.getline (textBuffer, 30);
            input->JoyButton1[index] = (int)atoi(textBuffer);

            fileStream.getline (textBuffer, 30);
            input->JoyButton2[index] = (int)atoi(textBuffer);
        }

        for (int gameMode = 0; gameMode < 6; gameMode++)
        {
            for (Uint8 rank = 0; rank < 10; rank++)
            {
                for (int index = 0; index < 18; index++)
                {
                    HighScoresName[gameMode][rank][index] = char(NULL);
                }

                fileStream.getline (textBuffer, 30);

                for (Uint8 index = 0; index < 18; index++)
                {
                    HighScoresName[gameMode][rank][index] = textBuffer[index];
                }

                fileStream.getline (textBuffer, 30);
                HighScoresLevel[gameMode][rank] = atoi(textBuffer);

                fileStream.getline (textBuffer, 30);
                HighScoresScore[gameMode][rank] = atoll(textBuffer);
            }
        }

        fileStream.close();
	}
}

//-------------------------------------------------------------------------------------------------
void Data::SaveHighScoresAndOptions(void)
{
char filename[256];
fstream fileStream;
char textBuffer[50];
char *base_path = SDL_GetPrefPath("16BitSoftInc", "TetriCrisis4-SDL2");
char *pref_path = NULL;

    if (base_path)
    {
        pref_path = SDL_strdup(base_path);
        SDL_free(base_path);
    }
    else  return;

	strcpy(filename, pref_path);
	strcat(filename, "TetriCrisis4-Data-Retail4_5_6");

	fileStream.open (filename, fstream::out);
	if (fileStream.is_open())
	{
		sprintf(textBuffer, "%d", audio->MusicVolume);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", audio->SoundVolume);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", visuals->FullScreenMode);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", logic->CPUPlayerEnabled);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", logic->PlayingGameFrameLock);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", logic->DelayAutoShift);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", logic->GameMode);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", logic->SelectedMusicTrack);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", logic->SelectedBackground);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", logic->NewGameGarbageHeight);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", logic->PressingUPAction);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", logic->DisplayNextPiece);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", logic->DisplayDropShadow);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", logic->TileSet);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", input->UserDefinedKeyButtonOne);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", input->UserDefinedKeyButtonTwo);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", input->UserDefinedKeyUP);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", input->UserDefinedKeyRIGHT);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", input->UserDefinedKeyDOWN);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", input->UserDefinedKeyLEFT);
		fileStream<<textBuffer;
		fileStream<<"\n";

		sprintf(textBuffer, "%d", input->UserDefinedKeyPause);
		fileStream<<textBuffer;
		fileStream<<"\n";

        for (int index = 0; index < 3; index++)
        {
            sprintf(textBuffer, "%d", input->JoyUP[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";

            sprintf(textBuffer, "%d", input->JoyDOWN[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";

            sprintf(textBuffer, "%d", input->JoyLEFT[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";

            sprintf(textBuffer, "%d", input->JoyRIGHT[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";

            sprintf(textBuffer, "%d", input->JoyButton1[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";

            sprintf(textBuffer, "%d", input->JoyButton2[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";
        }

        for (int gameMode = 0; gameMode < 6; gameMode++)
        {
            for (Uint8 rank = 0; rank < 10; rank++)
            {
                strcpy(textBuffer, HighScoresName[gameMode][rank]);
                fileStream<<textBuffer;
                fileStream<<"\n";

                sprintf(textBuffer, "%d", HighScoresLevel[gameMode][rank]);
                fileStream<<textBuffer;
                fileStream<<"\n";

                #ifdef _WIN32
                    sprintf(textBuffer, "%I64u", HighScoresScore[gameMode][rank]);
                #else
                    sprintf(textBuffer, "%lu", HighScoresScore[gameMode][rank]);
                #endif

                fileStream<<textBuffer;
                fileStream<<"\n";
            }
        }

        fileStream.close();
	}
}
//-------------------------------------------------------------------------------------------------
