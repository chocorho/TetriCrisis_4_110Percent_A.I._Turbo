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
#ifndef LOGIC
#define LOGIC

class Logic
{
public:

    #define OriginalMode                0
    #define TimeAttack30Mode            1
    #define TimeAttack60Mode            2
    #define TimeAttack120Mode           3
    #define TwentyLineChallengeMode     4
    #define CrisisMode                  5
    Uint8 GameMode;

    Uint8 MaxRotationArray[8];
    Uint8 PieceDropStartHeight[8];

    Uint8 CPUPlayerEnabled;

    Uint8 SelectedBackground;
    Uint8 SelectedMusicTrack;
    Uint8 NewGameGarbageHeight;

    #define OriginalJeZxLeeAI           0
    #define NewPerfectAI                1
    Uint8 NaturalIntelligenceCore;

	Uint8 PieceData[8][5][17];

	Uint8 Player;
    #define NumberOfPlayers     4
    struct PlayData
    {
        int Playfield[15][26];
        int PlayfieldBackup[15][26];

        int PlayfieldStartX;
        int PlayfieldEndX;

        Uint8 Piece;
        Uint8 PieceHistory[4];

        Sint8 PieceMovementDelay;
        Uint8 PieceRotation;
        Uint8 PiecePlayfieldX;
        Uint8 PiecePlayfieldY;
        Uint8 NextPiece;

        #define GameOver					-1
        #define NewPieceDropping			0
        #define PieceFalling				1
        #define FlashingCompletedLines		2
        #define ClearingCompletedLines		3
        #define ClearingPlayfield           4
        Sint16 PlayerStatus;

        float PlayersPlayfieldScreenX;
        float PlayersPlayfieldScreenY;
        int PieceDropTimer;
        int TimeToDropPiece;
        bool PieceRotated1;
        bool PieceRotated2;

        int MoveOneBlockCavernHoles[15][5];
        int MoveCompletedLines[15][5];
        int MovePieceHeight[15][5];
        int MovePlayfieldBoxEdges[15][5];
        int MoveTrappedHoles[15][5];

        int BestMoveX;
        int BestRotation;
        bool MovedToBestMove;
        bool BestMoveCalculated;

        bool UPActionTaken;
        Uint8 RotateDirection;

        int PlayerInput;

        Uint8 FlashCompletedLinesTimer;
        Uint8 ClearCompletedLinesTimer;

    	Uint8 AttackLines[10][12];

        Uint8 TwentyLineCounter;

        int SkipAFrameForCPUPieceDrop;
        int CPUFrame;

        Uint32 Score;
        Uint32 DropBonus;
        Uint16 Level;
        Uint16 Lines;

    } PlayerData[NumberOfPlayers];

    float PsychoBackgroundRotationOne;
    float PsychoBackgroundRotationTwo;

    bool CrisisModeOnePlayerLeftPlayfieldCleared;

    int TotalCPUPlayerLines;
    int NumberofCPUGames;
    int TotalOneLines;
    int TotalTwoLines;
    int TotalThreeLines;
    int TotalFourLines;

    Uint32 PlayingGameFrameLock;

    bool GameDisplayChanged;

    bool DisplayNextPiece;
    bool DisplayDropShadow;
    #define None            0
    #define Fall            1
    #define Rotate          2
    #define DropAndDrag     3
    Uint8 PressingUPAction;

    Uint8 DelayAutoShift;

    bool PlayersCanJoin;

    int MouseMoveX;
    int MouseMoveY;

    #define MouseMove       0
    #define MouseRotate     1
    int MouseMoveOrRotate;

    Uint32 TimeAttackTimer;

    Uint16 ThinkRussianTimer;

    Uint16 CrisisModeTimer;

    Uint8 BlockAttackTransparency[NumberOfPlayers];

    bool Crisis7BGMPlayed;
    bool CrisisWon;

    bool GameWasJustPlayed;

	bool PAUSEgame;

    Uint8 TileSet;

	Logic(void);
	virtual ~Logic(void);

	void InitializePieceData(void);
	void ClearPlayfieldsWithCollisionDetection(void);
	Uint8 GetRandomPiece(void);

    #define CollisionNotTrue            0
    #define CollisionWithPlayfield      1
	int PieceCollision(void);
	int PieceCollisionDown(void);

	#define Current		0
	#define Next		1
	#define DropShadow	2
	void AddPieceToPlayfieldMemory(int TempOrCurrentOrNextOrDropShadow);
	void DeletePieceFromPlayfieldMemory(int CurrentOrDropShadow);

    void SetupNewPiece(void);

	void CheckForCompletedLines(void);

	void MovePieceDown(bool Force);
    void MovePieceDownFast(void);
    void MovePieceDownFastDropAndDrag(void);
	bool RotatePieceCounterClockwise(void);
	bool RotatePieceClockwise(void);
	void MovePieceLeft(void);
	void MovePieceRight(void);

	void SetupForNewGame(void);

	void DisplayTetriScreenOntoScreenBuffer(void);

	void FlashCompletedLines(void);
	void ClearCompletedLines(void);

	void AddAnAttackLineToEnemiesPlayfield(void);

    bool AddAnIncompleteLineToPlayfieldCrisisMode(void);

	void RunTetriGameEngine(void);

    bool CrisisModeClearPlayfield(void);

    void ComputeComputerPlayerMove(void);
};

#endif
