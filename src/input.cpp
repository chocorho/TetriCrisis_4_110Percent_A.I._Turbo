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

    KeyOnKeyboardPressedByUser = -1;

    MouseButtonPressed[0] = false;
    MouseButtonPressed[1] = false;

    JoystickDeviceOne = NULL;
    JoystickDeviceTwo = NULL;
    JoystickDeviceThree = NULL;

    JoystickDisabled[0] = 1;
    JoystickDisabled[1] = 1;
    JoystickDisabled[2] = 1;

    if (SDL_NumJoysticks() == 0)
    {
        printf("No USB joysticks are plugged in.\n");
    }
    else
    {
        if (SDL_NumJoysticks()>0)
        {
            JoystickDeviceOne = SDL_JoystickOpen(0);
            NumberOfJoyButtons[0] = SDL_JoystickNumButtons(JoystickDeviceOne);
            NumberOfJoyAxises[0] = SDL_JoystickNumAxes(JoystickDeviceOne);
            printf("SDL2 Joystick #0 ''%s'' opened.\n", SDL_JoystickName(JoystickDeviceOne));
            JoystickDisabled[0] = 0;
        }

        if (SDL_NumJoysticks()>1)
        {
            JoystickDeviceTwo = SDL_JoystickOpen(1);
            NumberOfJoyButtons[1] = SDL_JoystickNumButtons(JoystickDeviceTwo);
            NumberOfJoyAxises[1] = SDL_JoystickNumAxes(JoystickDeviceTwo);
            printf("SDL2 Joystick #1 ''%s'' opened.\n", SDL_JoystickName(JoystickDeviceTwo));
            JoystickDisabled[1] = 0;
        }

        if (SDL_NumJoysticks()>2)
        {
            JoystickDeviceThree = SDL_JoystickOpen(2);
            NumberOfJoyButtons[2] = SDL_JoystickNumButtons(JoystickDeviceThree);
            NumberOfJoyAxises[2] = SDL_JoystickNumAxes(JoystickDeviceThree);
            printf("SDL2 Joystick #2 ''%s'' opened.\n", SDL_JoystickName(JoystickDeviceThree));
            JoystickDisabled[2] = 0;
        }
    }

    for (int joy = 0; joy < 3; joy++)
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
    if (JoystickDeviceOne != NULL)  SDL_JoystickClose(JoystickDeviceOne);
    if (JoystickDeviceTwo != NULL)  SDL_JoystickClose(JoystickDeviceTwo);
    if (JoystickDeviceThree != NULL)  SDL_JoystickClose(JoystickDeviceThree);
}

//-------------------------------------------------------------------------------------------------
int Input::QueryJoysticksForAction(Uint8 joy, bool justButtons)
{
int returnValue = -1;

	if (DelayAllUserInput > 0)  return(-1);

	if (joy == 0 && JoystickDeviceOne != NULL)
	{
		SDL_JoystickUpdate();

        if (justButtons == false)
        {
            for (int index = 0; index < NumberOfJoyAxises[joy]; index++)
            {
                Sint16 joyAxis = 0;
                joyAxis = SDL_JoystickGetAxis(JoystickDeviceOne, index);
                if (joyAxis < -16383 || joyAxis > 16383)
                {
                    returnValue = Axis0+index;
                    return(returnValue);
                }
            }
        }

        for (int index = 0; index < NumberOfJoyButtons[joy]; index++)
        {
            if ( SDL_JoystickGetButton(JoystickDeviceOne, index) )
            {
                returnValue = Button0+index;
                return(returnValue);
            }
        }
    }

	if (joy == 1 && JoystickDeviceTwo != NULL)
	{
		SDL_JoystickUpdate();

        if (justButtons == false)
        {
            for (int index = 0; index < NumberOfJoyAxises[joy]; index++)
            {
                Sint16 joyAxis = 0;
                joyAxis = SDL_JoystickGetAxis(JoystickDeviceTwo, index);
                if (joyAxis < -16383 || joyAxis > 16383)
                {
                    returnValue = Axis0+index;
                    return(returnValue);
                }
            }
        }

        for (int index = 0; index < NumberOfJoyButtons[joy]; index++)
        {
            if ( SDL_JoystickGetButton(JoystickDeviceTwo, index) )
            {
                returnValue = Button0+index;
                return(returnValue);
            }
        }
    }

	if (joy == 2 && JoystickDeviceThree != NULL)
	{
		SDL_JoystickUpdate();

        if (justButtons == false)
        {
            for (int index = 0; index < NumberOfJoyAxises[joy]; index++)
            {
                Sint16 joyAxis = 0;
                joyAxis = SDL_JoystickGetAxis(JoystickDeviceThree, index);
                if (joyAxis < -16383 || joyAxis > 16383)
                {
                    returnValue = Axis0+index;
                    return(returnValue);
                }
            }
        }

        for (int index = 0; index < NumberOfJoyButtons[joy]; index++)
        {
            if ( SDL_JoystickGetButton(JoystickDeviceThree, index) )
            {
                returnValue = Button0+index;
                return(returnValue);
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
                    screens->ScreenIsDirty = true;
                    logic->GameDisplayChanged = true;
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

        if (screens->ScreenToDisplay == PlayingGameScreen)
        {
            if (logic->HumanStillAlive == true)
            {
                logic->GameForfeit = true;

                for (int index = 0; index < 4; index++)

                if (logic->PlayerData[index].PlayerInput != CPU)
                {
                    if (logic->PlayerData[index].PlayerStatus != GameOver)
                    {
                        logic->PlayerData[index].Score = 0;
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
    if (JoystickDeviceOne != NULL && JoystickDisabled[0] == 0)
	{
		SDL_JoystickUpdate();

		Sint16 joystickXmovement = 0;
        if (JoyLEFT[0] > Axis7)
        {
            if ( SDL_JoystickGetButton(JoystickDeviceOne, JoyLEFT[0]-8) )  JoystickDirectionHorizontal[JoystickOne] = LEFT;
            else  if ( SDL_JoystickGetButton(JoystickDeviceOne, JoyRIGHT[0]-8) )  JoystickDirectionHorizontal[JoystickOne] = RIGHT;
        }
        else joystickXmovement = SDL_JoystickGetAxis(JoystickDeviceOne, JoyLEFT[0]);

		Sint16 joystickYmovement = 0;
        if (JoyUP[0] > Axis7)
        {
            if ( SDL_JoystickGetButton(JoystickDeviceOne, JoyUP[0]-8) )  JoystickDirectionVertical[JoystickOne] = UP;
            else  if ( SDL_JoystickGetButton(JoystickDeviceOne, JoyDOWN[0]-8) )  JoystickDirectionVertical[JoystickOne] = DOWN;
        }
        else joystickYmovement = SDL_JoystickGetAxis(JoystickDeviceOne, JoyUP[0]);

		if (joystickYmovement < -16383)
		{
            JoystickDirectionVertical[JoystickOne] = UP;
		}
		else if (joystickYmovement > 16383)
		{
            JoystickDirectionVertical[JoystickOne] = DOWN;
		}

		if (joystickXmovement < -16383)
		{
            JoystickDirectionHorizontal[JoystickOne] = LEFT;
		}
		else if (joystickXmovement > 16383)
		{
            JoystickDirectionHorizontal[JoystickOne] = RIGHT;
		}

        if (JoyButton1[0] > Axis7)
        {
	    	if ( SDL_JoystickGetButton(JoystickDeviceOne, JoyButton1[0]-8) )  JoystickButtonOne[JoystickOne] = ON;
        }
        else
        {
            Sint16 padAsButton = SDL_JoystickGetAxis(JoystickDeviceOne, JoyButton1[0]);
            if (padAsButton < -16383)  JoystickButtonOne[JoystickOne] = ON;
        }

        if (JoyButton2[0] > Axis7)
        {
    		if ( SDL_JoystickGetButton(JoystickDeviceOne, JoyButton2[0]-8) )  JoystickButtonTwo[JoystickOne] = ON;
        }
        else
        {
            Sint16 padAsButton = SDL_JoystickGetAxis(JoystickDeviceOne, JoyButton2[0]);
            if (padAsButton < -16383)  JoystickButtonTwo[JoystickOne] = ON;
        }
    }

    if (JoystickDeviceTwo != NULL && JoystickDisabled[1] == 0)
	{
		SDL_JoystickUpdate();

		Sint16 joystickXmovement = 0;
        if (JoyLEFT[1] > Axis7)
        {
            if ( SDL_JoystickGetButton(JoystickDeviceTwo, JoyLEFT[1]-8) )  JoystickDirectionHorizontal[JoystickTwo] = LEFT;
            else  if ( SDL_JoystickGetButton(JoystickDeviceTwo, JoyRIGHT[1]-8) )  JoystickDirectionHorizontal[JoystickTwo] = RIGHT;
        }
        else joystickXmovement = SDL_JoystickGetAxis(JoystickDeviceTwo, JoyLEFT[1]);

		Sint16 joystickYmovement = 0;
        if (JoyUP[1] > Axis7)
        {
            if ( SDL_JoystickGetButton(JoystickDeviceTwo, JoyUP[1]-8) )  JoystickDirectionVertical[JoystickTwo] = UP;
            else  if ( SDL_JoystickGetButton(JoystickDeviceTwo, JoyDOWN[1]-8) )  JoystickDirectionVertical[JoystickTwo] = DOWN;
        }
        else joystickYmovement = SDL_JoystickGetAxis(JoystickDeviceTwo, JoyUP[1]);

		if (joystickYmovement < -16383)
		{
            JoystickDirectionVertical[JoystickTwo] = UP;
		}
		else if (joystickYmovement > 16383)
		{
            JoystickDirectionVertical[JoystickTwo] = DOWN;
		}

		if (joystickXmovement < -16383)
		{
            JoystickDirectionHorizontal[JoystickTwo] = LEFT;
		}
		else if (joystickXmovement > 16383)
		{
            JoystickDirectionHorizontal[JoystickTwo] = RIGHT;
		}

        if (JoyButton1[1] > Axis7)
        {
	    	if ( SDL_JoystickGetButton(JoystickDeviceTwo, JoyButton1[1]-8) )  JoystickButtonOne[JoystickTwo] = ON;
        }
        else
        {
            Sint16 padAsButton = SDL_JoystickGetAxis(JoystickDeviceTwo, JoyButton1[1]);
            if (padAsButton < -16383)  JoystickButtonOne[JoystickTwo] = ON;
        }

        if (JoyButton2[1] > Axis7)
        {
    		if ( SDL_JoystickGetButton(JoystickDeviceTwo, JoyButton2[1]-8) )  JoystickButtonTwo[JoystickTwo] = ON;
        }
        else
        {
            Sint16 padAsButton = SDL_JoystickGetAxis(JoystickDeviceTwo, JoyButton2[1]);
            if (padAsButton < -16383)  JoystickButtonTwo[JoystickTwo] = ON;
        }
    }

    if (JoystickDeviceThree != NULL && JoystickDisabled[2] == 0)
	{
		SDL_JoystickUpdate();

		Sint16 joystickXmovement = 0;
        if (JoyLEFT[2] > Axis7)
        {
            if ( SDL_JoystickGetButton(JoystickDeviceThree, JoyLEFT[2]-8) )  JoystickDirectionHorizontal[JoystickThree] = LEFT;
            else  if ( SDL_JoystickGetButton(JoystickDeviceThree, JoyRIGHT[2]-8) )  JoystickDirectionHorizontal[JoystickThree] = RIGHT;
        }
        else joystickXmovement = SDL_JoystickGetAxis(JoystickDeviceThree, JoyLEFT[2]);

		Sint16 joystickYmovement = 0;
        if (JoyUP[2] > Axis7)
        {
            if ( SDL_JoystickGetButton(JoystickDeviceThree, JoyUP[2]-8) )  JoystickDirectionVertical[JoystickThree] = UP;
            else  if ( SDL_JoystickGetButton(JoystickDeviceThree, JoyDOWN[2]-8) )  JoystickDirectionVertical[JoystickThree] = DOWN;
        }
        else joystickYmovement = SDL_JoystickGetAxis(JoystickDeviceThree, JoyUP[2]);

		if (joystickYmovement < -16383)
		{
            JoystickDirectionVertical[JoystickThree] = UP;
		}
		else if (joystickYmovement > 16383)
		{
            JoystickDirectionVertical[JoystickThree] = DOWN;
		}

		if (joystickXmovement < -16383)
		{
            JoystickDirectionHorizontal[JoystickThree] = LEFT;
		}
		else if (joystickXmovement > 16383)
		{
            JoystickDirectionHorizontal[JoystickThree] = RIGHT;
		}

        if (JoyButton1[2] > Axis7)
        {
	    	if ( SDL_JoystickGetButton(JoystickDeviceThree, JoyButton1[2]-8) )  JoystickButtonOne[JoystickThree] = ON;
        }
        else
        {
            Sint16 padAsButton = SDL_JoystickGetAxis(JoystickDeviceThree, JoyButton1[2]);
            if (padAsButton < -16383)  JoystickButtonOne[JoystickThree] = ON;
        }

        if (JoyButton2[2] > Axis7)
        {
    		if ( SDL_JoystickGetButton(JoystickDeviceThree, JoyButton2[2]-8) )  JoystickButtonTwo[JoystickThree] = ON;
        }
        else
        {
            Sint16 padAsButton = SDL_JoystickGetAxis(JoystickDeviceThree, JoyButton2[2]);
            if (padAsButton < -16383)  JoystickButtonTwo[JoystickThree] = ON;
        }
    }
//------------------------------------------------------------------------

    for (int index = 0; index < 4; index++)
    {
        if (JoystickDirectionHorizontal[Any] == CENTER)  JoystickDirectionHorizontal[Any] = JoystickDirectionHorizontal[index];
        if (JoystickDirectionVertical[Any] == CENTER)  JoystickDirectionVertical[Any] = JoystickDirectionVertical[index];
        if (JoystickButtonOne[Any] == OFF && JoystickButtonOnePressed[index] == false)  JoystickButtonOne[Any] = JoystickButtonOne[index];
        if (JoystickButtonTwo[Any] == OFF)  JoystickButtonTwo[Any] = JoystickButtonTwo[index];
    }

    if (screens->ScreenToDisplay != NameInputJoystickScreen)
    {
        for (int index = 0; index < 4; index++)
        {
            if (JoystickButtonOne[index] == ON)  JoystickButtonOnePressed[index] = true;
            else  JoystickButtonOnePressed[index] = false;
        }
    }
//------------------------------------------------------------------------
}
