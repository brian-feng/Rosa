#include "Chess.h"

const int AI_PLAYER = 1;
const int HUMAN_PLAYER = -1;

Chess::Chess()
{
}

Chess::~Chess()
{
}

//
// make a chess piece for the player
//
Bit* Chess::PieceForPlayer(const int playerNumber, ChessPiece piece)
{
    const char* pieces[] = { "pawn.png", "knight.png", "bishop.png", "rook.png", "queen.png", "king.png" };

    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit* bit = new Bit();
    // should possibly be cached from player class?
    const char* pieceName = pieces[piece - 1];
    std::string spritePath = std::string("chess/") + (playerNumber == 0 ? "w_" : "b_") + pieceName;
    bit->LoadTextureFromFile(spritePath.c_str());
    bit->setOwner(getPlayerAt(playerNumber));
    bit->setSize(pieceSize, pieceSize);

    return bit;
}

void Chess::setUpBoard()
{
    setNumberOfPlayers(2);
    _gameOptions.rowX = 8;
    _gameOptions.rowY = 8;
    //
    // we want white to be at the bottom of the screen so we need to reverse the board
    //
    char piece[2];
    piece[1] = 0;
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for (int x = 0; x < _gameOptions.rowX; x++) {
            ImVec2 position((float)(pieceSize * x + pieceSize), (float)(pieceSize * (_gameOptions.rowY - y) + pieceSize));
            _grid[y][x].initHolder(position, "boardsquare.png", x, y);
            _grid[y][x].setGameTag(0);
            piece[0] = bitToPieceNotation(y,x);
            _grid[y][x].setNotation(piece);
        }
    }

    // white pieces
    int player = 0;
    Bit* bit = PieceForPlayer(player, Rook);
    bit->setPosition(_grid[0][0].getPosition());
    bit->setParent(&_grid[0][0]);
    bit->setGameTag(Rook);
    _grid[0][0].setBit(bit);
    bit = PieceForPlayer(player, Knight);
    bit->setPosition(_grid[0][1].getPosition());
    bit->setParent(&_grid[0][1]);
    bit->setGameTag(Knight);
    _grid[0][1].setBit(bit);
    bit = PieceForPlayer(player, Bishop);
    bit->setPosition(_grid[0][2].getPosition());
    bit->setParent(&_grid[0][2]);
    bit->setGameTag(Bishop);
    _grid[0][2].setBit(bit);
    bit = PieceForPlayer(player, Queen);
    bit->setPosition(_grid[0][3].getPosition());
    bit->setParent(&_grid[0][3]);
    bit->setGameTag(Queen);
    _grid[0][3].setBit(bit);
    bit = PieceForPlayer(player, King);
    bit->setPosition(_grid[0][4].getPosition());
    bit->setParent(&_grid[0][4]);
    bit->setGameTag(King);
    _grid[0][4].setBit(bit);
    bit = PieceForPlayer(player, Bishop);
    bit->setPosition(_grid[0][5].getPosition());
    bit->setParent(&_grid[0][5]);
    bit->setGameTag(Bishop);
    _grid[0][5].setBit(bit);
    bit = PieceForPlayer(player, Knight);
    bit->setPosition(_grid[0][6].getPosition());
    bit->setParent(&_grid[0][6]);
    bit->setGameTag(Knight);
    _grid[0][6].setBit(bit);
    bit = PieceForPlayer(player, Rook);
    bit->setPosition(_grid[0][7].getPosition());
    bit->setParent(&_grid[0][7]);
    bit->setGameTag(Rook);
    _grid[0][7].setBit(bit);
    for(int i = 0; i < 8; i++){
        bit = PieceForPlayer(player, Pawn);
        bit->setPosition(_grid[1][i].getPosition());
        bit->setParent(&_grid[1][i]);
        bit->setGameTag(Pawn);
        _grid[1][i].setBit(bit);
    }

    // black pieces
    player = 1;
    bit = PieceForPlayer(player, Rook);
    bit->setPosition(_grid[7][0].getPosition());
    bit->setParent(&_grid[7][0]);
    bit->setGameTag(Rook);
    _grid[7][0].setBit(bit);
    bit = PieceForPlayer(player, Knight);
    bit->setPosition(_grid[7][1].getPosition());
    bit->setParent(&_grid[7][1]);
    bit->setGameTag(Knight);
    _grid[7][1].setBit(bit);
    bit = PieceForPlayer(player, Bishop);
    bit->setPosition(_grid[7][2].getPosition());
    bit->setParent(&_grid[8][2]);
    bit->setGameTag(Bishop);
    _grid[7][2].setBit(bit);
    bit = PieceForPlayer(player, Queen);
    bit->setPosition(_grid[7][3].getPosition());
    bit->setParent(&_grid[7][3]);
    bit->setGameTag(Queen);
    _grid[7][3].setBit(bit);
    bit = PieceForPlayer(player, King);
    bit->setPosition(_grid[7][4].getPosition());
    bit->setParent(&_grid[7][4]);
    bit->setGameTag(King);
    _grid[7][4].setBit(bit);
    bit = PieceForPlayer(player, Bishop);
    bit->setPosition(_grid[7][5].getPosition());
    bit->setParent(&_grid[7][5]);
    bit->setGameTag(Bishop);
    _grid[7][5].setBit(bit);
    bit = PieceForPlayer(player, Knight);
    bit->setPosition(_grid[7][6].getPosition());
    bit->setParent(&_grid[7][6]);
    bit->setGameTag(Knight);
    _grid[7][6].setBit(bit);
    bit = PieceForPlayer(player, Rook);
    bit->setPosition(_grid[7][7].getPosition());
    bit->setParent(&_grid[7][7]);
    bit->setGameTag(Rook);
    _grid[7][7].setBit(bit);
    for(int i = 0; i < 8; i++){
        bit = PieceForPlayer(player, Pawn);
        bit->setPosition(_grid[6][i].getPosition());
        bit->setParent(&_grid[6][i]);
        bit->setGameTag(Pawn);
        _grid[6][i].setBit(bit);
    }
}

bool Chess::actionForEmptyHolder(BitHolder &holder, ChessPiece piece)
{
    if(holder.bit()){
        return false;
    }

    Bit *bit = PieceForPlayer(getCurrentPlayer()->playerNumber(), piece);
    bit->setPosition(holder.getPosition());
    bit->setParent(&holder);
    bit->setGameTag(piece);
    holder.setBit(bit);
    return true;
}

position Chess::getPosition(BitHolder &holder){
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for (int x = 0; x < _gameOptions.rowX; x++) {
            if (_grid[y][x].getPosition().x == holder.getPosition().x && _grid[y][x].getPosition().y == holder.getPosition().y){
                position ret;
                ret.x = x;
                ret.y = y;
                return ret;
            }
        }
    }
    position ret;
    ret.x = -1;
    ret.y = -1;
    return ret;
}

bool Chess::canBitMoveFrom(Bit &bit, BitHolder &src)
{
    return bit.getOwner()->playerNumber() == getCurrentPlayer()->playerNumber();
}

bool Chess::canBitMoveFromTo(Bit& bit, BitHolder& src, BitHolder& dst)
{
    position srcPos = getPosition(src);
    position dstPos = getPosition(dst);
    if(bit.gameTag() == Pawn){
        int player = bit.getOwner()->playerNumber();
        if(player == 0){
            if(srcPos.y == 1 && srcPos.x == dstPos.x && srcPos.y == dstPos.y-2){
                return true;
            }
            if((srcPos.x == dstPos.x-1 || srcPos.x == dstPos.x+1) && srcPos.y == dstPos.y-1){
                return true;
            }
            return srcPos.x == dstPos.x && srcPos.y == dstPos.y-1;
        }
        if(player == 1){
            if(srcPos.y == 6 && srcPos.x == dstPos.x && srcPos.y == dstPos.y+2){
                return true;
            }
            if((srcPos.x == dstPos.x-1 || srcPos.x == dstPos.x+1) && srcPos.y == dstPos.y+1){
                return true;
            }
            return srcPos.x == dstPos.x && srcPos.y == dstPos.y+1;
        }
        return false;
    }
    if(bit.gameTag() == Knight){
        int additions[8][2] = {
            {2, 1},
            {2, -1},
            {-2, -1},
            {-2, -2},
            {1, 2},
            {1, -2},
            {-1, 2},
            {-1, -2}
        };
        for(int i = 0; i < 8; i++){
            if(srcPos.x+additions[i][0] == dstPos.x && srcPos.y+additions[i][1] == dstPos.y){
                return true;
            }
        }
        return false;
    }
    if(bit.gameTag() == Bishop){
        int x;
        int y;
        if(srcPos.x < dstPos.x){
            x = 1;
        }
        else{
            x = -1;
        }
        if(srcPos.y < dstPos.y){
            y = 1;
        }
        else{
            y = -1;
        }
        while(srcPos.x >= 0 && srcPos.y < 8){
            srcPos.x += x;
            srcPos.y += y;
            if(srcPos.x == dstPos.x && srcPos.y == dstPos.y){
                return true;
            }
            if(_grid[srcPos.y][srcPos.x].bit() != nullptr){
                return false;
            }
        }
        return false;
    }
    if(bit.gameTag() == Rook){
        // must be on same file or same rank
        if((srcPos.x == dstPos.x) != (srcPos.y == dstPos.y)){
            // same file
            if(srcPos.x == dstPos.x){
                int y;
                if(srcPos.y < dstPos.y){
                    y = 1;
                }
                else{
                    y = -1;
                }
                while(srcPos.y >= 0 && srcPos.y < 8){
                    srcPos.y += y;
                    if(srcPos.y == dstPos.y){
                        return true;
                    }
                    if(_grid[srcPos.y][srcPos.x].bit() != nullptr){
                        return false;
                    }
                }
                return false;
            }
            // same rank
            else{
                int x;
                if(srcPos.x < dstPos.x){
                    x = 1;
                }
                else{
                    x = -1;
                }
                while(srcPos.x >= 0 && srcPos.x < 8){
                    srcPos.x += x;
                    if(srcPos.x == dstPos.x){
                        return true;
                    }
                    if(_grid[srcPos.y][srcPos.x].bit() != nullptr){
                        return false;
                    }
                }
                return false;
            }
        }
        return false;
    }
    if(bit.gameTag() == Queen){
        bool status = false;
        bit.setGameTag(Bishop);
        if(canBitMoveFromTo(bit, src, dst)){
            status = true;
        }
        bit.setGameTag(Rook);
        if(canBitMoveFromTo(bit, src, dst)){
            status = true;
        }
        bit.setGameTag(Queen);
        return status;
    }
    if(bit.gameTag() == King){
        int additions[8][2] = {
            {1, 1},
            {1, 0},
            {1, -1},
            {0, 1},
            {0, -1},
            {-1, 1},
            {-1, 0},
            {-1, -1}
        };
        for(int i = 0; i < 8; i++){
            if(srcPos.x+additions[i][0] == dstPos.x && srcPos.y+additions[i][1] == dstPos.y){
                return true;
            }
        }
        return false;
    }
    return false;
}

void Chess::bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst) {
    endTurn();
}

//
// free all the memory used by the game on the heap
//
void Chess::stopGame()
{
}

Player* Chess::checkForWinner()
{
    // check to see if either player has won
    return nullptr;
}

bool Chess::checkForDraw()
{
    // check to see if the board is full
    return false;
}

//
// add a helper to Square so it returns out FEN chess notation in the form p for white pawn, K for black king, etc.
// this version is used from the top level board to record moves
//
const char Chess::bitToPieceNotation(int row, int column) const {
    if (row < 0 || row >= 8 || column < 0 || column >= 8) {
        return '0';
    }

    const char* wpieces = { "?PNBRQK" };
    const char* bpieces = { "?pnbrqk" };
    unsigned char notation = '0';
    Bit* bit = _grid[row][column].bit();
    if (bit) {
        notation = bit->gameTag() < 128 ? wpieces[bit->gameTag()] : bpieces[bit->gameTag() & 127];
    } else {
        notation = '0';
    }
    return notation;
}

//
// state strings
//
std::string Chess::initialStateString()
{
    return stateString();
}

//
// this still needs to be tied into imguis init and shutdown
// we will read the state string and store it in each turn object
//
std::string Chess::stateString()
{
    std::string s;
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for (int x = 0; x < _gameOptions.rowX; x++) {
            s += bitToPieceNotation(y, x);
        }
    }
    return s;
}

//
// this still needs to be tied into imguis init and shutdown
// when the program starts it will load the current game from the imgui ini file and set the game state to the last saved state
//
void Chess::setStateString(const std::string &s)
{
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for (int x = 0; x < _gameOptions.rowX; x++) {
            int index = y * _gameOptions.rowX + x;
            int playerNumber = s[index] - '0';
            if (playerNumber) {
                _grid[y][x].setBit(PieceForPlayer(playerNumber - 1, Pawn));
            } else {
                _grid[y][x].setBit(nullptr);
            }
        }
    }
}


//
// this is the function that will be called by the AI
//
void Chess::updateAI() 
{
}

