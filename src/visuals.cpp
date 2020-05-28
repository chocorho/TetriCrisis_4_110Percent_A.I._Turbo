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

#include <fstream>
#include <sstream>

#include <iostream>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "visuals.h"

#include "screens.h"

extern Screens* screens;

//-------------------------------------------------------------------------------------------------
Visuals::Visuals(void)
{
    CoreFailure = false;

    ForceAspectRatio = false;

    FullScreenMode = false;

    FrameLock = 16;
    SystemTicks = SDL_GetTicks();
    NextFrameTicks = SystemTicks + FrameLock;
    NumberOfFrames = 0;

    CurrentFramePerSecond = 0;

    for (int index = 0; index < 10; index++)
    {
        FramesPerSecondArray[index] = 0;
    }

    NextSecondTick = 0;
    AverageFPS = 0;

    TotalNumberOfLoadedStaffTexts = 0;

    TextCacheCurrentIndex = 0;

	for (int index = 0; index < NumberOfTextsCached; index++)
	{
		TextTexture[index] = NULL;
		TextOutlineTexture[index] = NULL;
	}
    ClearTextCache();

    if (TTF_Init()==-1)
    {
        printf( "SDL2_TTF initialization failed: %s\n", TTF_GetError() );
        CoreFailure = true;
    }
}

//-------------------------------------------------------------------------------------------------
Visuals::~Visuals(void)
{
    ClearTextCache();

    UnloadFontsFromMemory();

    TTF_Quit();

    for (int index = 0; index < NumberOfSprites; index++)
    {
        if (Sprites[index].Texture != NULL)
        {
            SDL_DestroyTexture(Sprites[index].Texture);
        }
    }
    printf("Unloaded all sprite images from memory.\n");

    SDL_DestroyRenderer(Renderer);
    printf("SDL2 renderer destroyed.\n");

    SDL_DestroyWindow(Window);
    printf("SDL2 window destroyed.\n");
}

//-------------------------------------------------------------------------------------------------
void Visuals::CalculateFramerate(void)
{
    SystemTicks = SDL_GetTicks();
    NextFrameTicks = SystemTicks + FrameLock;

    NumberOfFrames++;

    if (SystemTicks > NextSecondTick)
    {
        NextSecondTick = SystemTicks + 1000;

        FramesPerSecondArray[CurrentFramePerSecond] = NumberOfFrames;
        NumberOfFrames = 0;

        if (CurrentFramePerSecond < 9)  CurrentFramePerSecond++;
        else  CurrentFramePerSecond = 0;

        Uint32 frameTotal = 0;
        for (int index = 0; index < 10; index++)
        {
            frameTotal += FramesPerSecondArray[index];
        }

        AverageFPS = frameTotal / 10;
    }
}

//-------------------------------------------------------------------------------------------------
void Visuals::ProcessFramerate(void)
{
    SystemTicks = SDL_GetTicks();
    if (NextFrameTicks > SystemTicks)  SDL_Delay(NextFrameTicks - SystemTicks);
}

//-------------------------------------------------------------------------------------------------
bool Visuals::InitializeWindow(void)
{
SDL_Surface* windowIcon = SDL_LoadBMP("data/visuals/icon.bmp");

    Window = NULL;
    Window = SDL_CreateWindow("''TetriCrisis 4 110% A.I. Turbo'' - (C)2020 By 16BitSoft Inc."
                                 , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_RESIZABLE);

    if (Window == NULL)
    {
        printf( "SDL2 create window failed: %s\n", SDL_GetError() );
        CoreFailure = true;
        return(false);
    }
    else  printf("SDL2 window created.\n");

    WindowWidthCurrent = 640;
    WindowHeightCurrent = 480;

    SDL_SetWindowIcon(Window, windowIcon);

    Renderer = NULL;
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);

    if (Renderer == NULL)
    {
        printf( "SDL2 create renderer failed: %s\n", SDL_GetError() );
        CoreFailure = true;
        return(false);
    }
    else  printf("SDL2 renderer created.\n");

    return(true);
}

//-------------------------------------------------------------------------------------------------
void Visuals::ClearScreenBufferWithColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
    SDL_SetRenderDrawColor(Renderer, red, green, blue, alpha);
    SDL_RenderFillRect(Renderer, NULL);
}

//-------------------------------------------------------------------------------------------------
bool Visuals::LoadSpritesAndInitialize(void)
{
char filePath[256];
SDL_Surface *tempSurface = NULL;
Uint32 textureFormat;
int textureAccess;
int textureWidth;
int textureHeight;

    for (int index = 0; index < NumberOfSprites; index++)
    {
        Sprites[index].Texture = NULL;
    }

    for (int index = 0; index < NumberOfSprites; index++)
    {
        strcpy(filePath, "~");

        switch(index)
        {
            case 0:
                strcpy(filePath, "data/visuals/Screen-Fade-Black-Box.png");
                break;

            case 1:
                strcpy(filePath, "data/visuals/16BitSoft-Logo.png");
                break;

            case 2:
                strcpy(filePath, "data/visuals/SaintBasilsCathedral-BG1.png");
                break;

            case 3:
                strcpy(filePath, "data/visuals/T-C-4-Logo.png");
                break;

            case 4:
                strcpy(filePath, "data/visuals/Keyboard-Controls.png");
                break;

            case 10:
                strcpy(filePath, "data/visuals/SDL2-Logo.png");
                break;


            case 80:
                strcpy(filePath, "data/visuals/Playfield.png");
                break;

            case 81:
                strcpy(filePath, "data/visuals/Playfield-Block-Attack.png");
                break;

            case 100:
                strcpy(filePath, "data/visuals/SaintBasilsCathedral-BG1.png");
                break;

            case 101:
                strcpy(filePath, "data/visuals/MIG31-BG.png");
                break;

            case 102:
                strcpy(filePath, "data/visuals/GT-R-BG.png");
                break;

            case 103:
                strcpy(filePath, "data/visuals/NY-BG.png");
                break;

            case 104:
                strcpy(filePath, "data/visuals/Van-Gogh-BG.png");
                break;

            case 105:
                strcpy(filePath, "data/visuals/Kittens-BG.png");
                break;

            case 106:
                strcpy(filePath, "data/visuals/Psycho-BG.png");
                break;

            case 155:
                strcpy(filePath, "data/visuals/Crack.png");
                break;


            default:
                break;
        }

        if (filePath[0] != '~')
        {
            tempSurface = IMG_Load(filePath);

            if (!tempSurface)
            {
                printf( "Image loading failed: %s\n", IMG_GetError() );
                CoreFailure = true;
                return(false);
            }
        }

        if (filePath[0] != '~')
        {
            Sprites[index].Texture = SDL_CreateTextureFromSurface(Renderer, tempSurface);

            Sprites[index].ScreenX = 320;
            Sprites[index].ScreenY = 240;
            Sprites[index].ScaleX = 1.0;
            Sprites[index].ScaleY = 1.0;
            Sprites[index].RotationDegree = 0;
            Sprites[index].RedHue = 255;
            Sprites[index].GreenHue = 255;
            Sprites[index].BlueHue = 255;
            Sprites[index].Transparency = 255;
            Sprites[index].Smooth = false;
            Sprites[index].FlipX = false;
            Sprites[index].FlipY = false;

            Sprites[index].AnimationTimer = 0;

            SDL_QueryTexture(Sprites[index].Texture, &textureFormat, &textureAccess, &textureWidth, &textureHeight);
            Sprites[index].TextureWidthOriginal = textureWidth;
            Sprites[index].TextureHeightOriginal = textureHeight;

            SDL_FreeSurface(tempSurface);
        }
    }

    if ( LoadBoxSpriteSheetIntoMemory() == false)
    {
        CoreFailure = true;
        return(false);
    }

    if ( LoadInterfaceIntoMemory() == false)
    {
        CoreFailure = true;
        return(false);
    }

    if ( PreloadStaffTextsIntoMemory() == false)
    {
        CoreFailure = true;
        return(false);
    }

    printf("Loaded all sprite images into memory.\n");
    return(true);
}

//-------------------------------------------------------------------------------------------------
bool Visuals::LoadBoxSpriteSheetIntoMemory(void)
{
Uint32 textureFormat;
int textureAccess;
int textureWidth;
int textureHeight;

    SDL_Surface *spriteSheet = NULL;
    SDL_Surface *boxTemp = NULL;
    SDL_Rect dstRect;
    SDL_Rect srcRect;
    int x = 1;
    for (int index = 200; index < 260; index++)
    {
        spriteSheet = IMG_Load("data/visuals/Boxes.png");
        if (!spriteSheet)
        {
            printf( "Image loading failed: %s\n", IMG_GetError() );
            CoreFailure = true;
            return(false);
        }

        boxTemp = IMG_Load("data/visuals/Box.png");
        if (!boxTemp)
        {
            printf( "Image loading failed: %s\n", IMG_GetError() );
            CoreFailure = true;
            return(false);
        }

        dstRect.x = 0;//8;
        dstRect.y = 0;//7;
        dstRect.w = 13;
        dstRect.h = 18;

        srcRect.w = 13;
        srcRect.h = 18;

        if (index > 199 && index < 210)
        {
            if (index == 200) x = 1;

            srcRect.x = x;
            srcRect.y = 1;
        }
        else if (index > 209 && index < 220)
        {
            if (index == 210) x = 1;

            srcRect.x = x;
            srcRect.y = 1+19*1;
        }
        else if (index > 219 && index < 230)
        {
            if (index == 220) x = 1;

            srcRect.x = x;
            srcRect.y = 1+19*2;
        }
        else if (index > 229 && index < 240)
        {
            if (index == 230) x = 1;

            srcRect.x = x;
            srcRect.y = 1+19*3;
        }
        else if (index > 239 && index < 250)
        {
            if (index == 240) x = 1;

            srcRect.x = x;
            srcRect.y = 1+19*4;
        }
        else if (index > 249 && index < 260)
        {
            if (index == 250) x = 1;

            srcRect.x = x;
            srcRect.y = 1+19*5;
        }

        SDL_BlitSurface(spriteSheet, &srcRect, boxTemp, &dstRect);
        x+=14;

        Sprites[index].Texture = SDL_CreateTextureFromSurface(Renderer, boxTemp);

        Sprites[index].ScreenX = 320;
        Sprites[index].ScreenY = 240;
        Sprites[index].ScaleX = 1.0;
        Sprites[index].ScaleY = 1.0;
        Sprites[index].RotationDegree = 0;
        Sprites[index].RedHue = 255;
        Sprites[index].GreenHue = 255;
        Sprites[index].BlueHue = 255;
        Sprites[index].Transparency = 255;
        Sprites[index].Smooth = false;
        Sprites[index].FlipX = false;
        Sprites[index].FlipY = false;

        Sprites[index].AnimationTimer = 0;

        SDL_QueryTexture(Sprites[index].Texture, &textureFormat, &textureAccess, &textureWidth, &textureHeight);
        Sprites[index].TextureWidthOriginal = textureWidth;
        Sprites[index].TextureHeightOriginal = textureHeight;

        SDL_FreeSurface(boxTemp);
    }

    SDL_FreeSurface(spriteSheet);

    return(true);
}

//-------------------------------------------------------------------------------------------------
bool Visuals::LoadInterfaceIntoMemory(void)
{
char filePath[256];
SDL_Surface *tempSurface = NULL;
Uint32 textureFormat;
int textureAccess;
int textureWidth;
int textureHeight;

    for (int index = 1000; index < 1010; index++)
    {
        Sprites[index].Texture = NULL;
    }

    for (int index = 1000; index < 1010; index++)
    {
        strcpy(filePath, "~");

        switch(index)
        {
            case 1000:
                strcpy(filePath, "data/visuals/Button-Selector-Left.png");
                break;

            case 1001:
                strcpy(filePath, "data/visuals/Button-Selector-Right.png");
                break;

            case 1002:
                strcpy(filePath, "data/visuals/Selector-Line.png");
                break;

            case 1003:
                strcpy(filePath, "data/visuals/Button.png");
                break;

            case 1004:
                strcpy(filePath, "data/visuals/Button.png");
                break;

            case 1005:
                strcpy(filePath, "data/visuals/Button.png");
                break;

            case 1006:
                strcpy(filePath, "data/visuals/Button.png");
                break;

            case 1007:
                strcpy(filePath, "data/visuals/Button.png");
                break;

            case 1008:
                strcpy(filePath, "data/visuals/Button.png");
                break;

            case 1009:
                strcpy(filePath, "data/visuals/Button.png");
                break;


            default:
                break;
        }

        if (filePath[0] != '~')
        {
            tempSurface = IMG_Load(filePath);

            if (!tempSurface)
            {
                printf( "Image loading failed: %s\n", IMG_GetError() );
                CoreFailure = true;
                return(false);
            }
        }

        if (index > 1002 && index < 1010)
        {
            char buttonText[64];

            strcpy(buttonText, " \0");

            if      (index == 1003)  strcpy(buttonText, "START!\0");
            else if (index == 1004)  strcpy(buttonText, "Options\0");
            else if (index == 1005)  strcpy(buttonText, "How To Play\0");
            else if (index == 1006)  strcpy(buttonText, "High Scores\0");
            else if (index == 1007)  strcpy(buttonText, "About\0");
            else if (index == 1008)  strcpy(buttonText, "Exit\0");
            else if (index == 1009)  strcpy(buttonText, "Back\0");

            SDL_Color textColor = { 0, 0, 0, 255 };//255, 255, 255, 255 };
            SDL_Color outlineColor = { 205, 205, 205, 255 };//80, 80, 80, 255 };
            SDL_Surface *text;
            SDL_Surface *textOutline;
            SDL_Rect destRect;

            text = TTF_RenderText_Blended(Font[0], buttonText, textColor);
            textOutline = TTF_RenderText_Solid(Font[0], buttonText, outlineColor);

            for (int posY = -2; posY < 3; posY++)
            {
                for (int posX = -2; posX < 3; posX++)
                {
                    destRect.x = (tempSurface->w / 2) - (text->w / 2) + posX;
                    destRect.y = (tempSurface->h / 2) - (text->h / 2) + posY;

                    SDL_BlitSurface(textOutline, NULL, tempSurface, &destRect);
                }
            }

            destRect.x = (tempSurface->w / 2) - (text->w / 2);
            destRect.y = (tempSurface->h / 2) - (text->h / 2);

            SDL_BlitSurface(text, NULL, tempSurface, &destRect);

            SDL_FreeSurface(text);
            SDL_FreeSurface(textOutline);
        }

        if (filePath[0] != '~')
        {
            Sprites[index].Texture = SDL_CreateTextureFromSurface(Renderer, tempSurface);

            Sprites[index].ScreenX = 320;
            Sprites[index].ScreenY = 240;
            Sprites[index].ScaleX = 1.0;
            Sprites[index].ScaleY = 1.0;
            Sprites[index].RotationDegree = 0;
            Sprites[index].RedHue = 255;
            Sprites[index].GreenHue = 255;
            Sprites[index].BlueHue = 255;
            Sprites[index].Transparency = 255;
            Sprites[index].Smooth = false;
            Sprites[index].FlipX = false;
            Sprites[index].FlipY = false;

            Sprites[index].AnimationTimer = 0;

            SDL_QueryTexture(Sprites[index].Texture, &textureFormat, &textureAccess, &textureWidth, &textureHeight);
            Sprites[index].TextureWidthOriginal = textureWidth;
            Sprites[index].TextureHeightOriginal = textureHeight;

            SDL_FreeSurface(tempSurface);
        }
    }

    if ( LoadCharacterTilesIntoMemory() == false)  return(false);

    return(true);
}

//-------------------------------------------------------------------------------------------------
bool Visuals::LoadCharacterTilesIntoMemory(void)
{
char filePath[256];
SDL_Surface *tempSurface = NULL;
Uint32 textureFormat;
int textureAccess;
int textureWidth;
int textureHeight;
std::string characterArray = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&0123456789+_<";
char *base_path = SDL_GetBasePath();

    for (int index = 300; index < 373; index++)
    {
        Sprites[index].Texture = NULL;
    }

    for (int index = 300; index < 373; index++)
    {
        strcpy(filePath, "~");
        strcpy(filePath, base_path);
        strcat(filePath, "data/visuals/Letter-Tile.png");

        if (filePath[0] != '~')
        {
            tempSurface = IMG_Load(filePath);

            if (!tempSurface)
            {
                printf( "Image loading failed: %s\n", IMG_GetError() );
                CoreFailure = true;
                return(false);
            }
        }

        if (index > 299 && index < 373)
        {
            std::string buttonText;
            if (index < 372)  buttonText = characterArray.substr(index-300, 1);
            else  buttonText = "End\0";

            SDL_Color textColor = { 0, 0, 0, 255 };//255, 255, 255, 255 };
            SDL_Color outlineColor = { 205, 205, 205, 255 };//80, 80, 80, 255 };
            SDL_Surface *text = NULL;
            SDL_Surface *textOutline = NULL;
            SDL_Rect destRect;

            char stringToCharArray[5];
            strcpy( stringToCharArray, buttonText.c_str() );

            if (index != 372)
            {
                text = TTF_RenderText_Blended(Font[0], stringToCharArray, textColor);
                textOutline = TTF_RenderText_Solid(Font[0], stringToCharArray, outlineColor);
            }
            else if (index == 372)
            {
                text = TTF_RenderText_Blended(Font[2], stringToCharArray, textColor);
                textOutline = TTF_RenderText_Solid(Font[2], stringToCharArray, outlineColor);
            }

            for (int posY = -2; posY < 3; posY++)
            {
                for (int posX = -2; posX < 3; posX++)
                {
                    destRect.x = (tempSurface->w / 2) - (text->w / 2) + posX;
                    destRect.y = (tempSurface->h / 2) - (text->h / 2) + posY;

                    SDL_BlitSurface(textOutline, NULL, tempSurface, &destRect);
                }
            }

            destRect.x = (tempSurface->w / 2) - (text->w / 2);
            destRect.y = (tempSurface->h / 2) - (text->h / 2);

            SDL_BlitSurface(text, NULL, tempSurface, &destRect);

            SDL_FreeSurface(text);
            SDL_FreeSurface(textOutline);
        }

        if (filePath[0] != '~')
        {
            Sprites[index].Texture = SDL_CreateTextureFromSurface(Renderer, tempSurface);

            Sprites[index].ScreenX = 320;
            Sprites[index].ScreenY = 240;
            Sprites[index].ScaleX = 1.0;
            Sprites[index].ScaleY = 1.0;
            Sprites[index].RotationDegree = 0;
            Sprites[index].RedHue = 255;
            Sprites[index].GreenHue = 255;
            Sprites[index].BlueHue = 255;
            Sprites[index].Transparency = 255;
            Sprites[index].Smooth = false;
            Sprites[index].FlipX = false;
            Sprites[index].FlipY = false;

            Sprites[index].AnimationTimer = 0;

            SDL_QueryTexture(Sprites[index].Texture, &textureFormat, &textureAccess, &textureWidth, &textureHeight);
            Sprites[index].TextureWidthOriginal = textureWidth;
            Sprites[index].TextureHeightOriginal = textureHeight;

            SDL_FreeSurface(tempSurface);
        }
    }

    return(true);
}

//-------------------------------------------------------------------------------------------------
bool Visuals::LoadStaffTextIntoMemory(const char *staffText, Uint8 textBlue)
{
Uint32 textureFormat;
int textureAccess;
int textureWidth;
int textureHeight;

    Sprites[1100+TotalNumberOfLoadedStaffTexts].Texture = NULL;

    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Color outlineColor = { 80, 80, 80, 255 };
    SDL_Surface *text;
    SDL_Surface *textOutline;
    SDL_Rect destRect;

    SDL_Surface *tempSurface = IMG_Load("data/visuals/Staff-Text-Image.png");

    if (!tempSurface)
    {
        printf( "Image loading failed: %s\n", IMG_GetError() );
        CoreFailure = true;
        return(false);
    }

    text = TTF_RenderText_Blended(Font[1], staffText, textColor);
    textOutline = TTF_RenderText_Solid(Font[1], staffText, outlineColor);

    for (int posY = -2; posY < 3; posY++)
    {
        for (int posX = -2; posX < 3; posX++)
        {
            destRect.x = (tempSurface->w / 2) - (text->w / 2) + posX;
            destRect.y = (tempSurface->h / 2) - (text->h / 2) + posY;

            SDL_BlitSurface(textOutline, NULL, tempSurface, &destRect);
        }
    }

    destRect.x = (tempSurface->w / 2) - (text->w / 2);
    destRect.y = (tempSurface->h / 2) - (text->h / 2);

    SDL_BlitSurface(text, NULL, tempSurface, &destRect);

    SDL_FreeSurface(text);
    SDL_FreeSurface(textOutline);

    Sprites[1100+TotalNumberOfLoadedStaffTexts].Texture = SDL_CreateTextureFromSurface(Renderer, tempSurface);

    Sprites[1100+TotalNumberOfLoadedStaffTexts].ScreenX = 320;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].ScreenY = 240;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].ScaleX = 1.0;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].ScaleY = 1.0;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].RotationDegree = 0;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].RedHue = 255;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].GreenHue = 255;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].BlueHue = textBlue;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].Transparency = 255;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].Smooth = false;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].FlipX = false;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].FlipY = false;

    Sprites[1100+TotalNumberOfLoadedStaffTexts].AnimationTimer = 0;

    SDL_QueryTexture(Sprites[1100+TotalNumberOfLoadedStaffTexts].Texture, &textureFormat, &textureAccess, &textureWidth, &textureHeight);
    Sprites[1100+TotalNumberOfLoadedStaffTexts].TextureWidthOriginal = textureWidth;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].TextureHeightOriginal = textureHeight;

    SDL_FreeSurface(tempSurface);

    TotalNumberOfLoadedStaffTexts++;
    return(true);
}

//-------------------------------------------------------------------------------------------------
bool Visuals::PreloadStaffTextsIntoMemory(void)
{
char textToDisplay[100];
char *copyright = new char[2];
char *reg = new char[2];
sprintf(copyright, "%c", 0xA9);
sprintf(reg, "%c", 0xAE);

    if ( LoadStaffTextIntoMemory("TM", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("TetriCrisis 4 110% A.I. Turbo", 0) == false)  return(false);
    strcpy(textToDisplay, copyright);
    strcat(textToDisplay, "2020, By 16BitSoft Inc.");
    if ( LoadStaffTextIntoMemory(textToDisplay, 255) == false)  return(false);

    strcpy(textToDisplay, "Original Tetris");
    strcat(textToDisplay, reg);
    strcat(textToDisplay, " Video Game Concept By:");
    if ( LoadStaffTextIntoMemory(textToDisplay, 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Alexey Pajitnov", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("''GT-R Twin TurboCharged'' Game Engine Programmer:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZ+Lee''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("''Gift Of Sight'' Artificial Intelligence Core Programmer:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZ+Lee''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Lead Game Designer:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZ+Lee''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Lead Game Programmer:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZ+Lee''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Lead Game Tester:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZ+Lee''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Lead Graphic Artist:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZ+Lee''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Lead Music Composer/Remixer / Sound Editor:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''D.J. Fading Twilight''", 255) == false)  return(false);

    strcpy(textToDisplay, "Microsoft");
    strcat(textToDisplay, reg);
    strcat(textToDisplay, " Windows");
    strcat(textToDisplay, reg);
    strcat(textToDisplay, " Technical Advisors:");
    if ( LoadStaffTextIntoMemory(textToDisplay, 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZ+Lee''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Daotheman''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Linux Technical Advisors:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZ+Lee''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''mattmatteh''", 255) == false)  return(false);

    strcpy(textToDisplay, "Apple");
    strcat(textToDisplay, reg);
    strcat(textToDisplay, " Mac");
    strcat(textToDisplay, reg);
    strcat(textToDisplay, " OS X");
    strcat(textToDisplay, reg);
    strcat(textToDisplay, " Technical Advisor:");
    if ( LoadStaffTextIntoMemory(textToDisplay, 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Someone?", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Email: ''Admin@16BitSoft.com'' To Help!", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Support Designers/Programmers/Testers:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Daotheman''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''mattmatteh''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Support Game Beta Testers:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''XaeL''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Blitz''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Blink''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''vipjun''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''farter''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''insatiate''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''clincher''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Integration''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''muf''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''simonlc''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''colour_thief''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Blockman''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Caithness''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Evi''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Zaphod77''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Sparks''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Technology Credits:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''SDL'' Version 2.0 - Simple DirectMedia Layer", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("(SDL2_Image / SDL2_Mixer / SDL2_TTF)", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("www.LibSDL.org", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("''A 110% By Team www.16BitSoft.com''", 0) == false)  return(false);

    return(true);
}

//-------------------------------------------------------------------------------------------------
void Visuals::DrawSpriteOntoScreenBuffer(Uint16 index)
{
SDL_Rect destinationRect;
int windowWidth;
int windowHeight;
Uint32 textureFormat;
int textureAccess;
int textureWidth;
int textureHeight;

    SDL_GetWindowSize(Window, &windowWidth, &windowHeight);

    SDL_QueryTexture(Sprites[index].Texture, &textureFormat, &textureAccess, &textureWidth, &textureHeight);

    float winWidthFixed;
    float winHeightFixed;
    if (ForceAspectRatio == false)
    {
        winWidthFixed = (float)windowWidth / 640;
        winHeightFixed = (float)windowHeight / 480;
    }
    else
    {
        winWidthFixed = 1;
        winHeightFixed = 1;
    }

    destinationRect.x = ( Sprites[index].ScreenX * (winWidthFixed) )
                        - (  ( (textureWidth * Sprites[index].ScaleX) * (winWidthFixed) ) / 2  );
    destinationRect.y = ( Sprites[index].ScreenY * (winHeightFixed) )
                        - (  ( (textureHeight * Sprites[index].ScaleY) * (winHeightFixed) ) / 2  );
    destinationRect.w = textureWidth * Sprites[index].ScaleX * (winWidthFixed);
    destinationRect.h = textureHeight * Sprites[index].ScaleY * (winHeightFixed);

    SDL_SetTextureColorMod(Sprites[index].Texture, Sprites[index].RedHue, Sprites[index].GreenHue, Sprites[index].BlueHue);
    SDL_SetTextureAlphaMod(Sprites[index].Texture, Sprites[index].Transparency);

    if (Sprites[index].FlipX == false && Sprites[index].FlipY == false)
    {
        SDL_RenderCopyEx(Renderer, Sprites[index].Texture, NULL, &destinationRect, Sprites[index].RotationDegree
                         , NULL, SDL_FLIP_NONE);
    }
    else if (Sprites[index].FlipX == true && Sprites[index].FlipY == false)
    {
        SDL_RenderCopyEx(Renderer, Sprites[index].Texture, NULL, &destinationRect, Sprites[index].RotationDegree
                         , NULL, SDL_FLIP_HORIZONTAL);
    }
    else if (Sprites[index].FlipX == false && Sprites[index].FlipY == true)
    {
        SDL_RenderCopyEx(Renderer, Sprites[index].Texture, NULL, &destinationRect, Sprites[index].RotationDegree
                         , NULL, SDL_FLIP_VERTICAL);
    }
    else if (Sprites[index].FlipX == true && Sprites[index].FlipY == true)
    {
        double flipHorizontallyAndVerticallyDegreeFix = Sprites[index].RotationDegree+180;

        SDL_RenderCopyEx(Renderer, Sprites[index].Texture, NULL, &destinationRect, flipHorizontallyAndVerticallyDegreeFix
                         , NULL, SDL_FLIP_NONE);
    }
}

//-------------------------------------------------------------------------------------------------
bool Visuals::LoadFontsIntoMemory(void)
{
    Font[0] = NULL;
    if ( !(Font[0] = TTF_OpenFont("data/fonts/Font-01.ttf", 23)) )
    {
        printf( "Error loading font: %s\n", TTF_GetError() );
        CoreFailure = true;
        return false;
    }

    Font[1] = NULL;
    if ( !(Font[1] = TTF_OpenFont("data/fonts/Font-01.ttf", 20)) )
    {
        printf( "Error loading font: %s\n", TTF_GetError() );
        CoreFailure = true;
        return false;
    }

    Font[2] = NULL;
    if ( !(Font[2] = TTF_OpenFont("data/fonts/Font-01.ttf", 14)) )
    {
        printf( "Error loading font: %s\n", TTF_GetError() );
        CoreFailure = true;
        return false;
    }

    Font[3] = NULL;
    if ( !(Font[3] = TTF_OpenFont("data/fonts/Line-01.ttf", 26)) )
    {
        printf( "Error loading font: %s\n", TTF_GetError() );
        CoreFailure = true;
        return false;
    }

    Font[4] = NULL;
    if ( !(Font[4] = TTF_OpenFont("data/fonts/Font-01.ttf", 55)) )
    {
        printf( "Error loading font: %s\n", TTF_GetError() );
        CoreFailure = true;
        return false;
    }

    printf("Loaded all TTF fonts into memory.\n");

    return true;
}

//-------------------------------------------------------------------------------------------------
void Visuals::UnloadFontsFromMemory(void)
{
    for (int index = 0; index < NumberOfFonts; index++)
    {
        if (Font[index] != NULL)  TTF_CloseFont(Font[index]);
    }
    printf("Unloaded all TTF fonts from memory.\n");
}

//-------------------------------------------------------------------------------------------------
void Visuals::ClearTextCache(void)
{
    for (int index = 0; index < NumberOfTextsCached; index++)
    {
        if (TextTexture[index] != NULL)  SDL_DestroyTexture(TextTexture[index]);
        if (TextOutlineTexture[index] != NULL)  SDL_DestroyTexture(TextOutlineTexture[index]);

        TextTexture[index] = NULL;
        TextOutlineTexture[index] = NULL;

        TextCachedText[index][0] = '\0';
        TextCachedScreenX[index] = 320;
        TextCachedScreenY[index] = 240;

        TextCachedWidth[index] = 0;
        TextCachedHeight[index] = 0;
    }

    TextCacheCurrentIndex = 0;

    printf("Cleared TTF text cache.\n");
}

//-------------------------------------------------------------------------------------------------
void Visuals::DrawTextOntoScreenBuffer(const char *textToDisplay, TTF_Font *font, int posX, int posY
                                       , Uint8 XJustification, Uint8 textRed, Uint8 textGreen, Uint8 textBlue
                                       , Uint8 outlineRed, Uint8 outlineGreen, Uint8 outlineBlue)
{
SDL_Color textColor = { textRed, textGreen, textBlue, 255 };
SDL_Color outlineColor = { outlineRed, outlineGreen, outlineBlue, 255 };
SDL_Surface *text = NULL;
SDL_Surface *textOutline = NULL;
SDL_Texture *textTexture = NULL;
SDL_Texture *textOutlineTexture = NULL;
SDL_Rect destinationRect;
SDL_Rect destinationOutlineRect;
int windowWidth;
int windowHeight;
int textCacheCheckIndex = NumberOfTextsCached;
bool sentenceMatches = true;
bool sentenceIsInCache = false;

    for (textCacheCheckIndex = 0; textCacheCheckIndex < NumberOfTextsCached; textCacheCheckIndex++)
    {
        sentenceMatches = true;
        if ( strlen(TextCachedText[textCacheCheckIndex]) == strlen(textToDisplay) )
        {
            for (Uint16 index = 0; index < strlen(textToDisplay); index++)
            {
                if (TextCachedText[textCacheCheckIndex][index] != textToDisplay[index])  sentenceMatches = false;
            }
        }
        else  sentenceMatches = false;

        if (sentenceMatches == true)
        {
            if (TextCachedScreenX[textCacheCheckIndex] == posX && TextCachedScreenY[textCacheCheckIndex] == posY)
            {
                sentenceIsInCache = true;
                break;
            }
        }
    }

    if (sentenceIsInCache == false)
    {
        if (TextTexture[TextCacheCurrentIndex] != NULL)  SDL_DestroyTexture(TextTexture[TextCacheCurrentIndex]);
        if (TextOutlineTexture[TextCacheCurrentIndex] != NULL)  SDL_DestroyTexture(TextOutlineTexture[TextCacheCurrentIndex]);
        TextTexture[TextCacheCurrentIndex] = NULL;
        TextOutlineTexture[TextCacheCurrentIndex] = NULL;
        TextCachedText[TextCacheCurrentIndex][0] = '\0';
        TextCachedScreenX[TextCacheCurrentIndex] = 320;
        TextCachedScreenY[TextCacheCurrentIndex] = 240;
        TextCachedWidth[TextCacheCurrentIndex] = 0;
        TextCachedHeight[TextCacheCurrentIndex] = 0;

        text = TTF_RenderText_Solid(font, textToDisplay, textColor);
        textOutline = TTF_RenderText_Solid(font, textToDisplay, outlineColor);

        strcpy(TextCachedText[TextCacheCurrentIndex], textToDisplay);

        TextCachedScreenX[TextCacheCurrentIndex] = posX;
        TextCachedScreenY[TextCacheCurrentIndex] = posY;

        TextCachedWidth[TextCacheCurrentIndex] = text->w;
        TextCachedHeight[TextCacheCurrentIndex] = text->h;

        textTexture = SDL_CreateTextureFromSurface(Renderer, text);
        TextTexture[TextCacheCurrentIndex] = SDL_CreateTextureFromSurface(Renderer, text);

        textOutlineTexture = SDL_CreateTextureFromSurface(Renderer, textOutline);
        TextOutlineTexture[TextCacheCurrentIndex] = SDL_CreateTextureFromSurface(Renderer, textOutline);

        if (XJustification == JustifyLeft)
        {
            posX = posX + (text->w / 2);
        }
        else if (XJustification == JustifyCenter)
        {
            posX = (640 / 2);
        }
        else if (XJustification == JustifyRight)
        {
            posX = (640 - posX) - (text->w / 2);
        }
        else if (XJustification == JustifyCenterOnPoint)
        {
            posX = posX;
        }

        if ( TextCacheCurrentIndex < (NumberOfTextsCached-1) )  TextCacheCurrentIndex++;
        else  TextCacheCurrentIndex = 0;
    }
    else
    {
        if (XJustification == JustifyLeft)
        {
            posX = posX + (TextCachedWidth[textCacheCheckIndex] / 2);
        }
        else if (XJustification == JustifyCenter)
        {
            posX = (640 / 2);
        }
        else if (XJustification == JustifyRight)
        {
            posX = (640 - posX) - (TextCachedWidth[textCacheCheckIndex] / 2);
        }
        else if (XJustification == JustifyCenterOnPoint)
        {
            posX = posX;
        }
    }

    SDL_GetWindowSize(Window, &windowWidth, &windowHeight);

    float winWidthFixed;
    float winHeightFixed;
    if (ForceAspectRatio == false)
    {
        winWidthFixed = (float)windowWidth / 640;
        winHeightFixed = (float)windowHeight / 480;
    }
    else
    {
        winWidthFixed = 1;
        winHeightFixed = 1;
    }

    if (sentenceIsInCache == false)
    {
        destinationRect.x = (posX * winWidthFixed) - ( (text->w * winWidthFixed) / 2 );
        destinationRect.y = (posY * winHeightFixed) - (winHeightFixed / 2) + 3;
        destinationRect.w = text->w * (winWidthFixed);
        destinationRect.h = text->h * (winHeightFixed);
    }
    else
    {
        destinationRect.x = (posX * winWidthFixed) - ( (TextCachedWidth[textCacheCheckIndex] * winWidthFixed) / 2 );
        destinationRect.y = (posY * winHeightFixed) - (winHeightFixed / 2) + 3;
        destinationRect.w = TextCachedWidth[textCacheCheckIndex] * (winWidthFixed);
        destinationRect.h = TextCachedHeight[textCacheCheckIndex] * (winHeightFixed);
    }

    destinationOutlineRect.x = destinationRect.x;
    destinationOutlineRect.y = destinationRect.y;
    destinationOutlineRect.w = destinationRect.w;
    destinationOutlineRect.h = destinationRect.h;

    for (Sint16 y = -3; y < 4; y+=1)
    {
        for (Sint16 x = -3; x < 4; x+=1)
        {
            destinationOutlineRect.x = destinationRect.x + x;
            destinationOutlineRect.y = destinationRect.y + y;

            if (sentenceIsInCache == false)
                SDL_RenderCopyEx(Renderer, textOutlineTexture, NULL, &destinationOutlineRect, 0, NULL, SDL_FLIP_NONE);
            else
            {
                SDL_RenderCopyEx(Renderer, TextOutlineTexture[textCacheCheckIndex], NULL, &destinationOutlineRect, 0, NULL, SDL_FLIP_NONE);
            }
        }
    }

    if (sentenceIsInCache == false)
        SDL_RenderCopyEx(Renderer, textTexture, NULL, &destinationRect, 0, NULL, SDL_FLIP_NONE);
    else
        SDL_RenderCopyEx(Renderer, TextTexture[textCacheCheckIndex], NULL, &destinationRect, 0, NULL, SDL_FLIP_NONE);

    SDL_DestroyTexture(textOutlineTexture);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(text);
    SDL_FreeSurface(textOutline);
}

//-------------------------------------------------------------------------------------------------
