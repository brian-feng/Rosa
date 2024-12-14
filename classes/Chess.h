#pragma once
#include "Game.h"
#include "ChessSquare.h"

const int pieceSize = 64;

typedef struct {
    int x;
    int y;
} position;

enum ChessPiece {
    NoPiece = 0,
    Pawn = 1,
    Knight = 2,
    Bishop = 3,
    Rook = 4,
    Queen = 5,
    King = 6
};

typedef struct {
    int x;
    int y;
    int x2;
    int y2;
    ChessPiece capture;
} move;

//
// the main game class
//
class Chess : public Game
{
public:
    Chess();
    ~Chess();

    // set up the board
    void        placePiece(const ChessPiece piece, const int x, const int y, const int player);
    std::vector<std::string>    split(const std::string& s, const std::string& delimiter);
    void        FENtoBoard(const std::string &s);
    void        setUpBoard() override;

    Player*     checkForWinner() override;
    bool        checkForDraw() override;
    std::string initialStateString() override;
    std::string stateString() override;
    void        setStateString(const std::string &s) override;
    bool        actionForEmptyHolder(BitHolder& holder, ChessPiece piece);
    position    getPosition(BitHolder& holder);
    bool        canBitMoveFrom(Bit& bit, BitHolder& src) override;
    bool        canBitMoveFromTo(Bit& bit, BitHolder& src, BitHolder& dst) override;
    void        bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;

    void        stopGame() override;
    BitHolder& getHolderAt(const int x, const int y) override { return _grid[y][x]; }

    std::vector<move>   generateMoves(int player);
	void        updateAI() override;
    bool        gameHasAI() override { return true; }
private:
    Bit *       PieceForPlayer(const int playerNumber, ChessPiece piece);
    const char  bitToPieceNotation(int row, int column) const;

    ChessSquare      _grid[8][8];
    ChessSquare      _copyGrid[8][8];
    bool            bkCastle;
    bool            bqCastle;
    bool            wkCastle;
    bool            wqCastle;
    BitHolder     *enPassantTarget;
    int             halfMoveCount;
    int             fullMoveCount;
};

