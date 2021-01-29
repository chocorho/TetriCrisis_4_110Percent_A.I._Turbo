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
*/

#include <stdio.h>
#include <cstring>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "input.h"

#include "visuals.h"
#include "screens.h"
#include "audio.h"
#include "logic.h"

extern Visuals* visuals;
extern Screens* screens;
extern Audio* audio;
extern Logic* logic;

//-------------------------------------------------------------------------------------------------
Input::Input(void)
{
    DEBUG = false;

    EXIT_Game = false;

    DelayAllUserInput = 0;

    LastEventWasWindowResize = 0;

    KeyOnKeyboardPressedByUser = -1;

    MouseButtonPressed[0] = false;
    MouseButtonPressed[1] = false;

    for (int index = 0; index < 4; index++)
    {
        JoystickDevices[index] = NULL;
        NumberOfJoyButtons[index] = 0;
        NumberOfJoyAxises[index] = 0;
        JoystickDisabled[index] = 1;

        for (int indexTwo = 0; indexTwo < 8; indexTwo++)
        {
            JoystickAxisDisable[index][indexTwo] = true;
        }
    }

    if (SDL_NumJoysticks() == 0)
    {
        printf("No USB joysticks are plugged in.\n");
    }
    else
    {
        Sint16 joyAxis = -1;
        if (SDL_NumJoysticks()>0)
        {
            JoystickDevices[0] = SDL_JoystickOpen(0);
            NumberOfJoyButtons[0] = SDL_JoystickNumButtons(JoystickDevices[0]);

            NumberOfJoyAxises[0] = SDL_JoystickNumAxes(JoystickDevices[0]);
            for (int index = 0; index < NumberOfJoyAxises[0]; index++)
            {
                JoystickAxisDisable[0][index] = false;
            }

            for (int index = 0; index < NumberOfJoyAxises[0]; index++)
            {
                SDL_JoystickUpdate();
                joyAxis = SDL_JoystickGetAxis(JoystickDevices[0], index);

                if ( joyAxis < (-32768*0.75) || joyAxis > (32767*0.75) )
                    JoystickAxisDisable[0][index] = true;
            }

            printf("SDL2 Joystick #0 ''%s'' opened.\n", SDL_JoystickName(JoystickDevices[0]));
            strcpy( JoystickName[0], SDL_JoystickName(JoystickDevices[0]) );
            JoystickDisabled[0] = 0;
        }

        joyAxis = -1;
        if (SDL_NumJoysticks()>1)
        {
            JoystickDevices[1] = SDL_JoystickOpen(1);
            NumberOfJoyButtons[1] = SDL_JoystickNumButtons(JoystickDevices[1]);

            NumberOfJoyAxises[1] = SDL_JoystickNumAxes(JoystickDevices[1]);
            for (int index = 0; index < NumberOfJoyAxises[1]; index++)
            {
                JoystickAxisDisable[1][index] = false;
            }

            for (int index = 0; index < NumberOfJoyAxises[1]; index++)
            {
                SDL_JoystickUpdate();
                joyAxis = SDL_JoystickGetAxis(JoystickDevices[1], index);

                if ( joyAxis < (-32768*0.75) || joyAxis > (32767*0.75) )
                    JoystickAxisDisable[1][index] = true;
            }

            printf("SDL2 Joystick #1 ''%s'' opened.\n", SDL_JoystickName(JoystickDevices[1]));
            strcpy( JoystickName[1], SDL_JoystickName(JoystickDevices[1]) );
            JoystickDisabled[1] = 0;
        }

        joyAxis = -1;
        if (SDL_NumJoysticks()>2)
        {
            JoystickDevices[2] = SDL_JoystickOpen(2);
            NumberOfJoyButtons[2] = SDL_JoystickNumButtons(JoystickDevices[2]);

            NumberOfJoyAxises[2] = SDL_JoystickNumAxes(JoystickDevices[2]);
            for (int index = 0; index < NumberOfJoyAxises[2]; index++)
            {
                JoystickAxisDisable[2][index] = false;
            }

            for (int index = 0; index < NumberOfJoyAxises[2]; index++)
            {
                SDL_JoystickUpdate();
                joyAxis = SDL_JoystickGetAxis(JoystickDevices[2], index);

                if ( joyAxis < (-32768*0.75) || joyAxis > (32767*0.75) )
                    JoystickAxisDisable[2][index] = true;
            }

            printf("SDL2 Joystick #2 ''%s'' opened.\n", SDL_JoystickName(JoystickDevices[2]));
            strcpy( JoystickName[2], SDL_JoystickName(JoystickDevices[2]) );
            JoystickDisabled[2] = 0;
        }

        joyAxis = -1;
        if (SDL_NumJoysticks()>3)
        {
            JoystickDevices[3] = SDL_JoystickOpen(3);
            NumberOfJoyButtons[3] = SDL_JoystickNumButtons(JoystickDevices[3]);

            NumberOfJoyAxises[3] = SDL_JoystickNumAxes(JoystickDevices[3]);
            for (int index = 0; index < NumberOfJoyAxises[3]; index++)
            {
                JoystickAxisDisable[3][index] = false;
            }

            for (int index = 0; index < NumberOfJoyAxises[3]; index++)
            {
                SDL_JoystickUpdate();
                joyAxis = SDL_JoystickGetAxis(JoystickDevices[3], index);

                if ( joyAxis < (-32768*0.75) || joyAxis > (32767*0.75) )
                    JoystickAxisDisable[3][index] = true;
            }

            printf("SDL2 Joystick #3 ''%s'' opened.\n", SDL_JoystickName(JoystickDevices[3]));
            strcpy( JoystickName[3], SDL_JoystickName(JoystickDevices[3]) );
            JoystickDisabled[3] = 0;
        }
    }

    for (int joy = 0; joy < 4; joy++)
    {
        JoyUP[joy] = Axis1;
        JoyDOWN[joy] = Axis1;
        JoyLEFT[joy] = Axis0;
        JoyRIGHT[joy] = Axis0;
        JoyButton1[joy] = Button0;
        JoyButton2[joy] = Button1;
    }

    JoystickSetupProcess = JoySetupNotStarted;

    for (int index = 0; index < 5; index++)
    {
        JoystickDirectionHorizontal[index] = CENTER;
        JoystickDirectionVertical[index] = CENTER;
        JoystickButtonOne[index] = OFF;
        JoystickButtonTwo[index] = OFF;
    }

    KeyboardSetupProcess = KeyboardSetupNotStarted;
    UserDefinedKeyButtonOne = -1;
    UserDefinedKeyButtonTwo = -1;
    UserDefinedKeyUP = -1;
    UserDefinedKeyRIGHT = -1;
    UserDefinedKeyDOWN = -1;
    UserDefinedKeyLEFT = -1;
    UserDefinedKeyPause = -1;

    DelayAllUserInput = -1;
}

//-------------------------------------------------------------------------------------------------
Input::~Input(void)
{
    for (int index = 0; index < 4; index++)
    {
        if (JoystickDevices[index] != NULL)  SDL_JoystickClose(JoystickDevices[index]);
    }
}

//-------------------------------------------------------------------------------------------------
int Input::QueryJoysticksForAction(Uint8 joy, int buttonsOrAxis)
{
int returnValue = -1;

	if (DelayAllUserInput > 0)  return(-1);

	for (int joyIndex = 0; joyIndex < 4; joyIndex++)
    {
        if (joy == joyIndex && JoystickDevices[joy] != NULL)
        {
            if (buttonsOrAxis == JustJoystickAxises || buttonsOrAxis == BothJoystickAxisesAndButtons)
            {
                for (int index = 0; index < NumberOfJoyAxises[joyIndex]; index++)
                {
                    SDL_JoystickUpdate();
                    Sint16 joyAxis = 0;
                    joyAxis = SDL_JoystickGetAxis(JoystickDevices[joyIndex], index);

                    if (JoystickAxisDisable[joyIndex][index] == false)
                    {
                        if ( joyAxis < (-32768*0.75) || joyAxis > (32767*0.75) )
                        {
                            DelayAllUserInput = 50;
                            returnValue = Axis0+index;

                            return(returnValue);
                        }
                    }
                }

                if (JoystickHat[joyIndex] == SDL_HAT_LEFT)
                {
                    returnValue = Hat0;
                }
                else if (JoystickHat[joyIndex] == SDL_HAT_RIGHT)
                {
                    returnValue = Hat0;
                }
                else if (JoystickHat[joyIndex] == SDL_HAT_UP)
                {
                    returnValue = Hat0;
                }
                else if (JoystickHat[joyIndex] == SDL_HAT_DOWN)
                {
                    returnValue = Hat0;
                }
            }

            if (buttonsOrAxis == JustJoystickButtons || buttonsOrAxis == BothJoystickAxisesAndButtons)
            {
                for (int index = 0; index < NumberOfJoyButtons[joyIndex]; index++)
                {
                    SDL_JoystickUpdate();
                    if ( SDL_JoystickGetButton(JoystickDevices[joyIndex], index) )
                    {
                        returnValue = Button0+index;
                    }
                }
            }
        }
    }

    return(returnValue);
}

//-------------------------------------------------------------------------------------------------
void Input::GetAllUserInput(void)
{
    KeyOnKeyboardPressedByUser = -1;

    MouseButtonPressed[0] = false;
    MouseButtonPressed[1] = false;

    for (int index = 0; index < 5; index++)
    {
        JoystickDirectionHorizontal[index] = CENTER;
        JoystickDirectionVertical[index] = CENTER;
        JoystickButtonOne[index] = OFF;
        JoystickButtonTwo[index] = OFF;
    }
    JoystickDirectionHorizontal[Any] = CENTER;
    JoystickDirectionVertical[Any] = CENTER;
    JoystickButtonOne[Any] = OFF;
    JoystickButtonTwo[Any] = OFF;

    for (int index = 0; index < 4; index++)  JoystickHat[0] = -1;

    if (DelayAllUserInput > 0)
    {
        DelayAllUserInput--;
        return;
    }

    KeyModState = SDL_GetModState();
    if ( (KeyModState & KMOD_LSHIFT) || (KeyModState & KMOD_RSHIFT) )  ShiftKeyPressed = true;
    else  ShiftKeyPressed = false;

    while ( SDL_PollEvent(&Event) )
    {
        switch (Event.type)
        {
            case SDL_QUIT:
                EXIT_Game = true;
                return;
                break;

            case SDL_WINDOWEVENT:
                if (Event.window.event == SDL_WINDOWEVENT_RESIZED || Event.window.event == SDL_WINDOWEVENT_SHOWN)
                {
                    screens->ScreenIsDirty = 2;
                    LastEventWasWindowResize = 2;
                }
                break;

            case SDL_KEYDOWN:
                KeyOnKeyboardPressedByUser = SDL_GetKeyFromScancode(Event.key.keysym.scancode);

                if (KeyOnKeyboardPressedByUser == SDLK_LSHIFT || KeyOnKeyboardPressedByUser == SDLK_RSHIFT)
                    KeyOnKeyboardPressedByUser = -1;
                break;

            case SDL_MOUSEBUTTONDOWN:
                MouseButtonClicked = Event.button.clicks;

                MouseX = Event.button.x;
                MouseY = Event.button.y;
                break;

            case SDL_MOUSEMOTION:
                if (screens->ScreenToDisplay == PlayingGameScreen || screens->ScreenToDisplay == PlayingStoryGameScreen)
                {
                    MouseX = Event.button.x;
                    MouseY = Event.button.y;
                }
                break;


            default:
                break;
        }
    }

    bool usingCustomKeys = true;
    if (UserDefinedKeyButtonOne == -1
     || UserDefinedKeyButtonTwo == -1
     || UserDefinedKeyUP == -1
     || UserDefinedKeyRIGHT == -1
     || UserDefinedKeyDOWN == -1
     || UserDefinedKeyLEFT == -1
     || UserDefinedKeyPause == -1)  usingCustomKeys = false;

    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    if ( (screens->ScreenToDisplay != PlayingGameScreen)
     || (screens->ScreenToDisplay == PlayingGameScreen && usingCustomKeys == false) )
    {
        if      (keyboardState[    SDL_SCANCODE_Z] && screens->ScreenToDisplay != NameInputKeyboardScreen)  JoystickButtonOne[Keyboard] = ON;
        else if (keyboardState[    SDL_SCANCODE_X] && screens->ScreenToDisplay != NameInputKeyboardScreen)  JoystickButtonTwo[Keyboard] = ON;

        if      (keyboardState[   SDL_SCANCODE_UP])  JoystickDirectionVertical[Keyboard] = UP;
        else if (keyboardState[ SDL_SCANCODE_DOWN])  JoystickDirectionVertical[Keyboard] = DOWN;

        if      (keyboardState[ SDL_SCANCODE_LEFT])  JoystickDirectionHorizontal[Keyboard] = LEFT;
        else if (keyboardState[SDL_SCANCODE_RIGHT])  JoystickDirectionHorizontal[Keyboard] = RIGHT;
    }
    else if (screens->ScreenToDisplay == PlayingGameScreen && usingCustomKeys == true)
    {
        if      (keyboardState[ SDL_GetScancodeFromKey(UserDefinedKeyButtonOne) ] && screens->ScreenToDisplay != NameInputKeyboardScreen)  JoystickButtonOne[Keyboard] = ON;
        else if (keyboardState[ SDL_GetScancodeFromKey(UserDefinedKeyButtonTwo) ] && screens->ScreenToDisplay != NameInputKeyboardScreen)  JoystickButtonTwo[Keyboard] = ON;

        if      (keyboardState[ SDL_GetScancodeFromKey(UserDefinedKeyUP) ])  JoystickDirectionVertical[Keyboard] = UP;
        else if (keyboardState[ SDL_GetScancodeFromKey(UserDefinedKeyDOWN) ])  JoystickDirectionVertical[Keyboard] = DOWN;

        if      (keyboardState[ SDL_GetScancodeFromKey(UserDefinedKeyLEFT) ])  JoystickDirectionHorizontal[Keyboard] = LEFT;
        else if (keyboardState[ SDL_GetScancodeFromKey(UserDefinedKeyRIGHT) ])  JoystickDirectionHorizontal[Keyboard] = RIGHT;
    }

    if (KeyOnKeyboardPressedByUser == SDLK_ESCAPE)
    {
        screens->ScreenTransitionStatus = FadeOut;

        DelayAllUserInput = 30;

        if (logic->GameMode == StoryMode)  logic->GameForfeit = true;

        if (screens->ScreenToDisplay == PlayingGameScreen || screens->ScreenToDisplay == PlayingStoryGameScreen)
        {
            if (logic->HumanStillAlive == true)
            {
                for (int index = 0; index < 4; index++)

                if (logic->PlayerData[index].PlayerInput != CPU)
                {
                    if (logic->PlayerData[index].PlayerStatus != GameOver)
                    {
                        logic->PlayerData[index].Score = 0;
                        logic->PlayerData[index].PlayerStatus = GameOver;
                    }
                }
            }
        }
    }

//------------------------------------------------------------------------

    SDL_PumpEvents();

    for (int index = 0; index < 2; index++)
    {
        if ( SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(index+1) )
        {
            if (MouseButtonWasClicked[index] == false)
            {
                MouseButtonPressed[index] = true;
                MouseButtonWasClicked[index] = true;
            }
        }
        else  MouseButtonWasClicked[index] = false;
    }

//------------------------------------------------------------------------

    SDL_JoystickUpdate();
    for (int index = 0; index < 4; index++)
    {
        if (JoystickDisabled[index] == 0)
        {
            JoystickHat[index] = SDL_JoystickGetHat(JoystickDevices[index], 0);
            if (JoyLEFT[index] == Hat0 && JoystickHat[index] == SDL_HAT_LEFT)
            {
                JoystickDirectionHorizontal[JoystickOne+index] = LEFT;
            }
            if (JoyRIGHT[index] == Hat0 && JoystickHat[index] == SDL_HAT_RIGHT)
            {
                JoystickDirectionHorizontal[JoystickOne+index] = RIGHT;
            }
            if (JoyUP[index] == Hat0 && JoystickHat[index] == SDL_HAT_UP)
            {
                JoystickDirectionVertical[JoystickOne+index] = UP;
            }
            if (JoyDOWN[index] == Hat0 && JoystickHat[index] == SDL_HAT_DOWN)
            {
                JoystickDirectionVertical[JoystickOne+index] = DOWN;
            }

            Sint16 joystickXmovement = 0;
            if (JoyLEFT[index] > Hat0)
            {
                if ( SDL_JoystickGetButton(JoystickDevices[index], JoyLEFT[index]-9) )  JoystickDirectionHorizontal[JoystickOne+index] = LEFT;
                else  if ( SDL_JoystickGetButton(JoystickDevices[index], JoyRIGHT[index]-9) )  JoystickDirectionHorizontal[JoystickOne+index] = RIGHT;
            }
            else joystickXmovement = SDL_JoystickGetAxis(JoystickDevices[index], JoyLEFT[index]);

            Sint16 joystickYmovement = 0;
            if (JoyUP[index] > Hat0)
            {
                if ( SDL_JoystickGetButton(JoystickDevices[index], JoyUP[index]-9) )  JoystickDirectionVertical[JoystickOne+index] = UP;
                else  if ( SDL_JoystickGetButton(JoystickDevices[index], JoyDOWN[index]-9) )  JoystickDirectionVertical[JoystickOne+index] = DOWN;
            }
            else joystickYmovement = SDL_JoystickGetAxis(JoystickDevices[index], JoyUP[index]);

            if ( joystickYmovement < (-32768*0.75) )
            {
                JoystickDirectionVertical[JoystickOne+index] = UP;
            }
            else if ( joystickYmovement > (32767*0.75) )
            {
                JoystickDirectionVertical[JoystickOne+index] = DOWN;
            }

            if ( joystickXmovement < (-32768*0.75) )
            {
                JoystickDirectionHorizontal[JoystickOne+index] = LEFT;
            }
            else if ( joystickXmovement > (32767*0.75) )
            {
                JoystickDirectionHorizontal[JoystickOne+index] = RIGHT;
            }

            if (JoyButton1[index] > Hat0)
            {
                if ( SDL_JoystickGetButton(JoystickDevices[index], JoyButton1[index]-9) )  JoystickButtonOne[JoystickOne+index] = ON;
            }
            else
            {
                Sint16 padAsButton = SDL_JoystickGetAxis(JoystickDevices[index], JoyButton1[index]);
                if (padAsButton < (-32768*0.75))  JoystickButtonOne[JoystickOne+index] = ON;
            }

            if (JoyButton2[index] > Hat0)
            {
                if ( SDL_JoystickGetButton(JoystickDevices[index], JoyButton2[index]-9) )  JoystickButtonTwo[JoystickOne+index] = ON;
            }
            else
            {
                Sint16 padAsButton = SDL_JoystickGetAxis(JoystickDevices[index], JoyButton2[index]);
                if (padAsButton < (-32768*0.75))  JoystickButtonTwo[JoystickOne+index] = ON;
            }
        }
    }

//------------------------------------------------------------------------

    for (int index = 0; index < 5; index++)
    {
        if (JoystickDirectionHorizontal[Any] == CENTER)  JoystickDirectionHorizontal[Any] = JoystickDirectionHorizontal[index];
        if (JoystickDirectionVertical[Any] == CENTER)  JoystickDirectionVertical[Any] = JoystickDirectionVertical[index];
        if (JoystickButtonOne[Any] == OFF && JoystickButtonOnePressed[index] == false)  JoystickButtonOne[Any] = JoystickButtonOne[index];
        if (JoystickButtonTwo[Any] == OFF)  JoystickButtonTwo[Any] = JoystickButtonTwo[index];
    }

    if (screens->ScreenToDisplay != NameInputJoystickScreen)
    {
        for (int index = 0; index < 5; index++)
        {
            if (JoystickButtonOne[index] == ON)  JoystickButtonOnePressed[index] = true;
            else  JoystickButtonOnePressed[index] = false;
        }
    }
//------------------------------------------------------------------------
}
