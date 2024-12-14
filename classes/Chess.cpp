#include "Chess.h"
#include <string>

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

void Chess::placePiece(const ChessPiece piece, const int x, const int y, const int player){
    Bit* bit = PieceForPlayer(player, piece);
    bit->setPosition(_grid[y][x].getPosition());
    bit->setParent(&_grid[y][x]);
    bit->setGameTag(piece);
    _grid[y][x].setBit(bit);
}

void Chess::FENtoBoard(const std::string &s){
    int x = 0;
    int y = 7;
    int player = 0;
    char c;
    Bit* bit = PieceForPlayer(player, Rook);
    int i;
    for(i = 0; i < s.length(); i++){
        c = s.at(i);
        if(y >= 0){
            switch(c){
                case 'r':
                    placePiece(Rook, x, y, 1);
                    break;
                case 'n':
                    placePiece(Knight, x, y, 1);
                    break;
                case 'b':
                    placePiece(Bishop, x, y, 1);
                    break;
                case 'q':
                    placePiece(Queen, x, y, 1);
                    break;
                case 'k':
                    placePiece(King, x, y, 1);
                    break;
                case 'p':
                    placePiece(Pawn, x, y, 1);
                    break;
                case 'R':
                    placePiece(Rook, x, y, 0);
                    break;
                case 'N':
                    placePiece(Knight, x, y, 0);
                    break;
                case 'B':
                    placePiece(Bishop, x, y, 0);
                    break;
                case 'Q':
                    placePiece(Queen, x, y, 0);
                    break;
                case 'K':
                    placePiece(King, x, y, 0);
                    break;
                case 'P':
                    placePiece(Pawn, x, y, 0);
                    break;
                case '1':
                    x+=0;
                    break;
                case '2':
                    x+=1;
                    break;
                case '3':
                    x+=2;
                    break;
                case '4':
                    x+=3;
                    break;
                case '5':
                    x+=4;
                    break;
                case '6':
                    x+=5;
                    break;
                case '7':
                    x+=6;
                    break;
                case '8':
                    x+=7;
                    break;
                case '/':
                    x=-1;
                    y--;
                    break;
                case ' ':
                    y=-1;
                    break;
            }
            x++;
        }
        else{
            break;
        }
    }
    int count = 0;
    std::string token = "";
    while(i < s.length()){
        if(count == 0){
            if(s.at(i) == 'b'){
                endTurn();
            }
            count++;
            i++;
        }
        else if(count == 1){
            if(s.at(i) == '-'){
                count++;
                i++;
            }
            else if(s.at(i) == 'k'){
                bkCastle = true;
            }
            else if(s.at(i) == 'q'){
                bqCastle = true;
            }
            else if(s.at(i) == 'K'){
                wkCastle = true;
            }
            else if(s.at(i) == 'Q'){
                wqCastle = true;
            }
            else if(s.at(i) == ' '){
                count++;
            }
            else{
                std::cout << "invalid castling";
            }
        }
        else if(count == 2){
            if(s.at(i) == '-'){
                count++;
                i++;
            }
            else if(s.at(i) >= 'a' && s.at(i) <= 'g'){
                enPassantTarget = &_grid[s.at(i) - 'a'][s.at(i+1)];
                count++;
                i+=2;
            }
            else{
                std::cout << "invalid en passant";
            }
        }
        else if(count == 3){
            if(s.at(i) == ' '){
                count++;
                halfMoveCount = stoi(token);
                token = "";
            }
            else{
                token += s.at(i);
            }
        }
        else if(count == 4){
            token += s.at(i);
            if(i == s.length()-1){
                count++;
                fullMoveCount = stoi(token);
                token = "";
            }
        }
        i++;
    }
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
    bkCastle = false;
    bqCastle = false;
    wkCastle = false;
    wqCastle = false;
    enPassantTarget = NULL;
    halfMoveCount = 0;
    fullMoveCount = 1;  

    FENtoBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    setAIPlayer(1);
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
    if(bit.gameTag() == Pawn){
        int player = bit.getOwner()->playerNumber();
        if(player == 0){
            // double move
            if(src.getRow() == 1 && src.getColumn() == dst.getColumn() && src.getRow() == dst.getRow()-2 && dst.empty() && _grid[src.getRow()+1][src.getColumn()].empty()){
                return true;
            }
            // captures
            if((src.getColumn() == dst.getColumn()-1 || src.getColumn() == dst.getColumn()+1) && src.getRow() == dst.getRow()-1 && dst.bit()){
                return true;
            }
            // google en passant
            if(enPassantTarget != NULL && dst.getColumn() == enPassantTarget->getColumn() && dst.getRow() == enPassantTarget->getRow()+1){
                if(src.getRow() == enPassantTarget->getRow() && (src.getColumn() == dst.getColumn()-1 || src.getColumn() == dst.getColumn()+1)){
                    return true;
                }
            }
            // pawn push
            return dst.empty() && src.getColumn() == dst.getColumn() && src.getRow() == dst.getRow()-1;
        }
        if(player == 1){
            // double move
            if(src.getRow() == 6 && src.getColumn() == dst.getColumn() && src.getRow() == dst.getRow()+2 && dst.empty() && _grid[src.getRow()-1][src.getColumn()].empty()){
                return true;
            }
            // captures
            if((src.getColumn() == dst.getColumn()-1 || src.getColumn() == dst.getColumn()+1) && src.getRow() == dst.getRow()+1 && dst.bit()){
                return true;
            }
            // google en passant
            if(enPassantTarget != NULL && dst.getColumn() == enPassantTarget->getColumn() && dst.getRow() == enPassantTarget->getRow()-1){
                if(src.getRow() == enPassantTarget->getRow() && (src.getColumn() == dst.getColumn()-1 || src.getColumn() == dst.getColumn()+1)){
                    return true;
                }
            }
            // pawn push
            return dst.empty() && src.getColumn() == dst.getColumn() && src.getRow() == dst.getRow()+1;
        }
        return false;
    }
    if(bit.gameTag() == Knight){
        int additions[8][2] = {
            {2, 1},
            {2, -1},
            {-2, -1},
            {-2, 1},
            {1, 2},
            {1, -2},
            {-1, 2},
            {-1, -2}
        };
        for(int i = 0; i < 8; i++){
            if(src.getColumn()+additions[i][0] == dst.getColumn() && src.getRow()+additions[i][1] == dst.getRow()){
                if(dst.bit() && dst.bit()->getOwner()->playerNumber() == bit.getOwner()->playerNumber()){
                    return false;
                }
                return true;
            }
        }
        return false;
    }
    if(bit.gameTag() == Bishop){
        int x;
        int y;
        if(src.getColumn() < dst.getColumn()){
            x = 1;
        }
        else{
            x = -1;
        }
        if(src.getRow() < dst.getRow()){
            y = 1;
        }
        else{
            y = -1;
        }
        int srcCol = src.getColumn();
        int srcRow = src.getRow();
        while(srcRow >= 0 && srcRow < 8){
            srcCol += x;
            srcRow += y;
            if(srcCol == dst.getColumn() && srcRow == dst.getRow()){
                if(dst.bit() && dst.bit()->getOwner()->playerNumber() == bit.getOwner()->playerNumber()){
                    return false;
                }
                return true;
            }
            if(_grid[srcRow][srcCol].bit() != nullptr){
                return false;
            }
            
        }
        return false;
    }
    if(bit.gameTag() == Rook){
        // must be on same file or same rank
        if((src.getColumn() == dst.getColumn()) != (src.getRow() == dst.getRow())){
            // same file
            if(src.getColumn() == dst.getColumn()){
                int y;
                if(src.getRow() < dst.getRow()){
                    y = 1;
                }
                else{
                    y = -1;
                }
                int srcRow = src.getRow();
                while(srcRow >= 0 && srcRow < 8){
                    srcRow += y;
                    if(srcRow >= 8 || srcRow < 0){
                        return false;
                    }
                    if(srcRow == dst.getRow() && dst.bit() && dst.bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber()){
                        return true;
                    }
                    if(_grid[srcRow][src.getColumn()].bit() != nullptr){
                        return false;
                    }
                }
                return false;
            }
            // same rank
            else{
                int x;
                if(src.getColumn() < dst.getColumn()){
                    x = 1;
                }
                else{
                    x = -1;
                }
                int srcCol = src.getColumn();
                while(srcCol >= 0 && srcCol< 8){
                    srcCol += x;
                    if(srcCol >= 8 || srcCol < 0){
                        return false;
                    }
                    if(srcCol == dst.getColumn() && dst.bit() && dst.bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber()){
                        return true;
                    }
                    if(_grid[src.getRow()][srcCol].bit() != nullptr){
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
            if(src.getColumn()+additions[i][0] == dst.getColumn() && src.getRow()+additions[i][1] == dst.getRow()){
                if(dst.bit() && dst.bit()->getOwner()->playerNumber() == bit.getOwner()->playerNumber()){
                    return false;
                }
                return true;
            }
        }
        int player = getCurrentPlayer()->playerNumber();
        if(player == 0){
            // is king home
            if(src.getColumn() == 4 && src.getRow() == 0){
                // is choosing correct tile
                if(dst.getColumn() == 6 && dst.getRow() == 0 && wkCastle){
                    // is rook there
                    if(_grid[0][7].bit()->gameTag() == Rook){
                        // nothing blocking
                        if(_grid[0][5].empty() && _grid[0][6].empty()){
                            return true;
                        }
                    }
                }
                // is other tile
                if(dst.getColumn() == 2 && dst.getRow() == 0 && wqCastle){
                    // is rook there
                    if(_grid[0][0].bit()->gameTag() == Rook){
                        // nothing blocking
                        if(_grid[0][3].empty() && _grid[0][2].empty() && _grid[0][1].empty()){
                            return true;
                        }
                    }
                }
            }
        }
        if(player == 1){
            // is king home
            if(src.getColumn() == 4 && src.getRow() == 7){
                // is choosing correct tile
                if(dst.getColumn() == 6 && dst.getRow() == 7 && bkCastle){
                    // is rook there
                    if(_grid[7][7].bit()->gameTag() == Rook){
                        if(_grid[7][6].empty() && _grid[0][5].empty()){
                            return true;
                        }
                    }
                }
                // is other tile
                if(dst.getColumn() == 2 && dst.getRow() == 7 && bqCastle){
                    // is rook there
                    if(_grid[7][0].bit()->gameTag() == Rook){
                        // nothing blocking
                        if(_grid[7][3].empty() && _grid[0][2].empty() && _grid[0][1].empty()){
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }
    return false;
}


void Chess::bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst) {
    int player = getCurrentPlayer()->playerNumber();
    if(bit.gameTag() == King){
        if(player == 0){
            // is king home
            if(src.getColumn() == 4 && src.getRow() == 0){
                // is choosing correct tile
                if(dst.getColumn() == 6 && dst.getRow() == 0){
                    // is rook there
                    if(_grid[0][7].bit()->gameTag() == Rook){
                        actionForEmptyHolder(_grid[0][5], Rook);
                        _grid[0][7].destroyBit();
                    }
                }
                // is other tile
                if(dst.getColumn() == 2 && dst.getRow() == 0){
                    // is rook there
                    if(_grid[0][0].bit()->gameTag() == Rook){
                        actionForEmptyHolder(_grid[0][3], Rook);
                        _grid[0][0].destroyBit();
                    }
                }
            }
        }
        if(player == 1){
            // is king home
            if(src.getColumn() == 4 && src.getRow() == 7){
                // is choosing correct tile
                if(dst.getColumn() == 6 && dst.getRow() == 7){
                    // is rook there
                    if(_grid[7][7].bit()->gameTag() == Rook){
                        actionForEmptyHolder(_grid[7][5], Rook);
                        _grid[7][7].destroyBit();
                    }
                }
                // is other tile
                if(dst.getColumn() == 2 && dst.getRow() == 7){
                    // is rook there
                    if(_grid[7][0].bit()->gameTag() == Rook){
                        actionForEmptyHolder(_grid[7][3], Rook);
                        _grid[7][0].destroyBit();
                    }
                }
            }
        }
    }
    
    else if(bit.gameTag() == Pawn){
        if(player == 0){
            if(src.getColumn() == dst.getColumn() && src.getRow() == dst.getRow()-2){
                enPassantTarget = &dst;
            }
            else if(enPassantTarget != NULL && dst.getColumn() == enPassantTarget->getColumn() && dst.getRow() == enPassantTarget->getRow()+1){
                if(src.getRow() == enPassantTarget->getRow() && (src.getColumn() == dst.getColumn()-1 || src.getColumn() == dst.getColumn()+1)){
                    _grid[enPassantTarget->getRow()][enPassantTarget->getColumn()].destroyBit();
                    enPassantTarget = NULL;
                }
            }
            else{
                enPassantTarget = NULL;
            }
            if(dst.getRow() == 7){
                _grid[dst.getRow()][dst.getColumn()].destroyBit();
                placePiece(Queen, dst.getColumn(), dst.getRow(), 0);
            }
        }
        if(player == 1){
            if(src.getColumn() == dst.getColumn() && src.getRow() == dst.getRow()+2){
                enPassantTarget = &dst;
            }
            else if(enPassantTarget != NULL && dst.getColumn() == enPassantTarget->getColumn() && dst.getRow() == enPassantTarget->getRow()-1){
                if(src.getRow() == enPassantTarget->getRow() && (src.getColumn() == dst.getColumn()-1 || src.getColumn() == dst.getColumn()+1)){
                    _grid[enPassantTarget->getRow()][enPassantTarget->getColumn()].destroyBit();
                    enPassantTarget = NULL;
                }
            }
            else{
                enPassantTarget = NULL;
            }
            if(dst.getRow() == 0){
                _grid[dst.getRow()][dst.getColumn()].destroyBit();
                placePiece(Queen, dst.getColumn(), dst.getRow(), 1);
            }
        }
    }   
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
        notation = bit->getOwner()->playerNumber() == 0 ? wpieces[bit->gameTag()] : bpieces[bit->gameTag() & 127];
    } else {
        notation = '0';
    }
    return notation;
}

const char Chess::copyBitToPieceNotation(int row, int column) const {
    if (row < 0 || row >= 8 || column < 0 || column >= 8) {
        return '0';
    }

    const char* wpieces = { "?PNBRQK" };
    const char* bpieces = { "?pnbrqk" };
    unsigned char notation = '0';
    Bit* bit = _copyGrid[row][column].bit();
    if (bit) {
        notation = bit->getOwner()->playerNumber() == 0 ? wpieces[bit->gameTag()] : bpieces[bit->gameTag() & 127];
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

bool Chess::copyCanBitMoveFromTo(Bit& bit, BitHolder& src, BitHolder& dst)
{
    if(bit.gameTag() == Pawn){
        int player = bit.getOwner()->playerNumber();
        if(player == 0){
            // double move
            if(src.getRow() == 1 && src.getColumn() == dst.getColumn() && src.getRow() == dst.getRow()-2 && dst.empty() && _grid[src.getRow()+1][src.getColumn()].empty()){
                return true;
            }
            // captures
            if((src.getColumn() == dst.getColumn()-1 || src.getColumn() == dst.getColumn()+1) && src.getRow() == dst.getRow()-1 && dst.bit()){
                return true;
            }
            // google en passant
            if(enPassantTarget != NULL && dst.getColumn() == enPassantTarget->getColumn() && dst.getRow() == enPassantTarget->getRow()+1){
                if(src.getRow() == enPassantTarget->getRow() && (src.getColumn() == dst.getColumn()-1 || src.getColumn() == dst.getColumn()+1)){
                    return true;
                }
            }
            // pawn push
            return dst.empty() && src.getColumn() == dst.getColumn() && src.getRow() == dst.getRow()-1;
        }
        if(player == 1){
            // double move
            if(src.getRow() == 6 && src.getColumn() == dst.getColumn() && src.getRow() == dst.getRow()+2 && dst.empty() && _grid[src.getRow()-1][src.getColumn()].empty()){
                return true;
            }
            // captures
            if((src.getColumn() == dst.getColumn()-1 || src.getColumn() == dst.getColumn()+1) && src.getRow() == dst.getRow()+1 && dst.bit()){
                return true;
            }
            // google en passant
            if(enPassantTarget != NULL && dst.getColumn() == enPassantTarget->getColumn() && dst.getRow() == enPassantTarget->getRow()-1){
                if(src.getRow() == enPassantTarget->getRow() && (src.getColumn() == dst.getColumn()-1 || src.getColumn() == dst.getColumn()+1)){
                    return true;
                }
            }
            // pawn push
            return dst.empty() && src.getColumn() == dst.getColumn() && src.getRow() == dst.getRow()+1;
        }
        return false;
    }
    if(bit.gameTag() == Knight){
        int additions[8][2] = {
            {2, 1},
            {2, -1},
            {-2, -1},
            {-2, 1},
            {1, 2},
            {1, -2},
            {-1, 2},
            {-1, -2}
        };
        for(int i = 0; i < 8; i++){
            if(src.getColumn()+additions[i][0] == dst.getColumn() && src.getRow()+additions[i][1] == dst.getRow()){
                if(dst.bit() && dst.bit()->getOwner()->playerNumber() == bit.getOwner()->playerNumber()){
                    return false;
                }
                return true;
            }
        }
        return false;
    }
    if(bit.gameTag() == Bishop){
        int x;
        int y;
        if(src.getColumn() < dst.getColumn()){
            x = 1;
        }
        else{
            x = -1;
        }
        if(src.getRow() < dst.getRow()){
            y = 1;
        }
        else{
            y = -1;
        }
        int srcCol = src.getColumn();
        int srcRow = src.getRow();
        while(srcRow >= 0 && srcRow < 8){
            srcCol += x;
            srcRow += y;
            if(srcCol == dst.getColumn() && srcRow == dst.getRow()){
                if(dst.bit() && dst.bit()->getOwner()->playerNumber() == bit.getOwner()->playerNumber()){
                    return false;
                }
                return true;
            }
            if(_copyGrid[srcRow][srcCol].bit() != nullptr){
                return false;
            }
            
        }
        return false;
    }
    if(bit.gameTag() == Rook){
        // must be on same file or same rank
        if((src.getColumn() == dst.getColumn()) != (src.getRow() == dst.getRow())){
            // same file
            if(src.getColumn() == dst.getColumn()){
                int y;
                if(src.getRow() < dst.getRow()){
                    y = 1;
                }
                else{
                    y = -1;
                }
                int srcRow = src.getRow();
                while(srcRow >= 0 && srcRow < 8){
                    srcRow += y;
                    if(srcRow >= 8 || srcRow < 0){
                        return false;
                    }
                    if(srcRow == dst.getRow() && dst.bit() && dst.bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber()){
                        return true;
                    }
                    if(_copyGrid[srcRow][src.getColumn()].bit() != nullptr){
                        return false;
                    }
                }
                return false;
            }
            // same rank
            else{
                int x;
                if(src.getColumn() < dst.getColumn()){
                    x = 1;
                }
                else{
                    x = -1;
                }
                int srcCol = src.getColumn();
                while(srcCol >= 0 && srcCol< 8){
                    srcCol += x;
                    if(srcCol >= 8 || srcCol < 0){
                        return false;
                    }
                    if(srcCol == dst.getColumn() && dst.bit() && dst.bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber()){
                        return true;
                    }
                    if(_copyGrid[src.getRow()][srcCol].bit() != nullptr){
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
        if(copyCanBitMoveFromTo(bit, src, dst)){
            status = true;
        }
        bit.setGameTag(Rook);
        if(copyCanBitMoveFromTo(bit, src, dst)){
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
            if(src.getColumn()+additions[i][0] == dst.getColumn() && src.getRow()+additions[i][1] == dst.getRow()){
                if(dst.bit() && dst.bit()->getOwner()->playerNumber() == bit.getOwner()->playerNumber()){
                    return false;
                }
                return true;
            }
        }
        int player = getCurrentPlayer()->playerNumber();
        if(player == 0){
            // is king home
            if(src.getColumn() == 4 && src.getRow() == 0){
                // is choosing correct tile
                if(dst.getColumn() == 6 && dst.getRow() == 0 && wkCastle){
                    // is rook there
                    if(_copyGrid[0][7].bit()->gameTag() == Rook){
                        // nothing blocking
                        if(_copyGrid[0][5].empty() && _copyGrid[0][6].empty()){
                            return true;
                        }
                    }
                }
                // is other tile
                if(dst.getColumn() == 2 && dst.getRow() == 0 && wqCastle){
                    // is rook there
                    if(_copyGrid[0][0].bit()->gameTag() == Rook){
                        // nothing blocking
                        if(_copyGrid[0][3].empty() && _copyGrid[0][2].empty() && _copyGrid[0][1].empty()){
                            return true;
                        }
                    }
                }
            }
        }
        if(player == 1){
            // is king home
            if(src.getColumn() == 4 && src.getRow() == 7){
                // is choosing correct tile
                if(dst.getColumn() == 6 && dst.getRow() == 7 && bkCastle){
                    // is rook there
                    if(_copyGrid[7][7].bit()->gameTag() == Rook){
                        if(_copyGrid[7][6].empty() && _copyGrid[0][5].empty()){
                            return true;
                        }
                    }
                }
                // is other tile
                if(dst.getColumn() == 2 && dst.getRow() == 7 && bqCastle){
                    // is rook there
                    if(_copyGrid[7][0].bit()->gameTag() == Rook){
                        // nothing blocking
                        if(_copyGrid[7][3].empty() && _copyGrid[0][2].empty() && _copyGrid[0][1].empty()){
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }
    return false;
}


std::vector<move> Chess::generateMoves(int player) {
    std::vector<move> moves;
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            Bit *src = _copyGrid[y][x].bit();
            if(src == nullptr){
                continue;
            }
            if(src->getOwner()->playerNumber() == player){
                for(int y2 = 0; y2 < 8; y2++){
                    for(int x2 = 0; x2 < 8; x2++){
                        if(copyCanBitMoveFromTo(*src, _copyGrid[y][x], _copyGrid[y2][x2])){
                            move m;
                            m.x = x;
                            m.y = y;
                            m.x2 = x2;
                            m.y2 = y2;
                            if(_copyGrid[y2][x2].bit()){
                                m.capture = (ChessPiece)_copyGrid[y2][x2].bit()->gameTag();
                            }
                            m.capture = NoPiece;
                            moves.push_back(m);
                        }
                    }
                }
            }
        }
    }
    return moves;
}

int Chess::evaluatePosition(){
    std::map<char, int> pieceValues = {
        {'P', 100}, {'p', -100},
        {'N', 300}, {'n', -300},
        {'B', 330}, {'b', -330},
        {'R', 500}, {'r', -500},
        {'Q', 900}, {'q', -900},
        {'K', 20000}, {'k', -20000}
    };
    int score = 0;
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            Bit *bit = _copyGrid[y][x].bit();
            if(bit){
                score += pieceValues[copyBitToPieceNotation(y, x)];
            }
        }
    }
    return score;
}

void Chess::copyGrid(){
    char piece[2];
    piece[1] = 0;
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for (int x = 0; x < _gameOptions.rowX; x++) {
            ImVec2 position((float)(pieceSize * x + pieceSize), (float)(pieceSize * (_gameOptions.rowY - y) + pieceSize));
            _copyGrid[y][x].initHolder(position, "boardsquare.png", x, y);
            _copyGrid[y][x].setGameTag(0);
            piece[0] = bitToPieceNotation(y,x);
            _copyGrid[y][x].setNotation(piece);
        }
    }
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            if(_grid[y][x].bit()){
                Bit* bit = PieceForPlayer(_grid[y][x].bit()->getOwner()->playerNumber(), (ChessPiece)_grid[y][x].bit()->gameTag());
                bit->setPosition(_copyGrid[y][x].getPosition());
                bit->setParent(&_copyGrid[y][x]);
                bit->setGameTag(_grid[y][x].bit()->gameTag());
                bit->setOwner(_grid[y][x].bit()->getOwner());
                _copyGrid[y][x].setBit(bit);
            }
        }
    }
}

void Chess::makeMove(move m){
    ChessSquare src = _copyGrid[m.y][m.x];
    ChessSquare dst = _copyGrid[m.y2][m.x2];
    /*
    dst.setBit(src.bit());
    src.bit()->setPosition(dst.getPosition());
    src.bit()->setParent(&dst);
    src.destroyBit();*/
}

void Chess::unmakeMove(move m){
    BitHolder src = _copyGrid[m.y][m.x];
    BitHolder dst = _copyGrid[m.y2][m.x2];
    /*
    src.setBit(dst.bit());
    Bit* bit = PieceForPlayer(1-src.bit()->getOwner()->playerNumber(), m.capture);
    bit->setPosition(dst.getPosition());
    bit->setParent(&dst);
    bit->setGameTag(m.capture);
    dst.setBit(bit);*/
}

int Chess::negamax(int depth, int alpha, int beta, int color){
    if(depth <= 0){
        return color * evaluatePosition();
    }

    int maxEval = -100000;
    std::vector<move> moves;
    if(color == -1){
        moves = generateMoves(0);
    }
    else{
        moves = generateMoves(1);
    }
    for(move m : moves){
        makeMove(m);
        std::cout << evaluatePosition() << std::endl;
        int eval = -1*(negamax(depth-1, -beta, -alpha, -color));
        unmakeMove(m);

        
        maxEval = std::max(maxEval, eval);
        alpha = std::max(alpha, maxEval);

        if(alpha >= beta){
            break;
        }
    }
    return maxEval;
}

//
// this is the function that will be called by the AI
//
void Chess::updateAI() 
{
    copyGrid();
    int bestVal = -1000;
    move bestMove;
    int depth = 2;
    
    std::vector<move> moves = generateMoves(getCurrentPlayer()->playerNumber());
    /*std::cout << "moves: " << moves.size() << std::endl;
    for(int i = 0; i < moves.size(); i++){
        std::cout << _copyGrid[moves[i].y][moves[i].x].bit()->gameTag() << moves[i].y << moves[i].x << moves[i].y2 << moves[i].x2 << std::endl;
    }*/
    for(move m : moves){
        makeMove(m);
        int moveEval = -1*(negamax(depth-1, -100000, 100000, -1));
        unmakeMove(m);

        std::cout << moveEval << m.y << m.x << m.y2 << m.x2 << std::endl;

        if(moveEval > bestVal){
            bestVal = moveEval;
            bestMove = m;
        }
    }
    std::cout << "----------" << std::endl;
    //std::cout << bestVal << bestMove.y << bestMove.x << bestMove.y2 << bestMove.x2 << std::endl;
    BitHolder &src = _grid[bestMove.y][bestMove.x];
    BitHolder &dst = _grid[bestMove.y2][bestMove.x2];

    Bit bit = *src.bit();
    int player = 1;
    bool state = false;
    if(bit.gameTag() == King){
        if(player == 0){
            // is king home
            if(src.getColumn() == 4 && src.getRow() == 0){
                // is choosing correct tile
                if(dst.getColumn() == 6 && dst.getRow() == 0){
                    // is rook there
                    if(_grid[0][7].bit()->gameTag() == Rook){
                        state = true;
                        actionForEmptyHolder(_grid[0][5], Rook);
                        _grid[0][7].destroyBit();
                    }
                }
                // is other tile
                if(dst.getColumn() == 2 && dst.getRow() == 0){
                    // is rook there
                    if(_grid[0][0].bit()->gameTag() == Rook){
                        state = true;
                        actionForEmptyHolder(_grid[0][3], Rook);
                        _grid[0][0].destroyBit();
                    }
                }
            }
        }
        if(player == 1){
            // is king home
            if(src.getColumn() == 4 && src.getRow() == 7){
                // is choosing correct tile
                if(dst.getColumn() == 6 && dst.getRow() == 7){
                    // is rook there
                    if(_grid[7][7].bit()->gameTag() == Rook){
                        state = true;
                        actionForEmptyHolder(_grid[7][5], Rook);
                        _grid[7][7].destroyBit();
                    }
                }
                // is other tile
                if(dst.getColumn() == 2 && dst.getRow() == 7){
                    // is rook there
                    if(_grid[7][0].bit()->gameTag() == Rook){
                        state = true;
                        actionForEmptyHolder(_grid[7][3], Rook);
                        _grid[7][0].destroyBit();
                    }
                }
            }
        }
    }
    
    else if(bit.gameTag() == Pawn){
        if(player == 0){
            if(src.getColumn() == dst.getColumn() && src.getRow() == dst.getRow()-2){
                enPassantTarget = &dst;
            }
            else if(enPassantTarget != NULL && dst.getColumn() == enPassantTarget->getColumn() && dst.getRow() == enPassantTarget->getRow()+1){
                if(src.getRow() == enPassantTarget->getRow() && (src.getColumn() == dst.getColumn()-1 || src.getColumn() == dst.getColumn()+1)){
                    state = true;
                    _grid[enPassantTarget->getRow()][enPassantTarget->getColumn()].destroyBit();
                    enPassantTarget = NULL;
                }
            }
            else if(dst.getRow() == 7){
                state = true;
                _grid[dst.getRow()][dst.getColumn()].destroyBit();
                placePiece(Queen, dst.getColumn(), dst.getRow(), 0);
            }
            else{
                enPassantTarget = NULL;
            }
            
        }
        if(player == 1){
            if(src.getColumn() == dst.getColumn() && src.getRow() == dst.getRow()+2){
                enPassantTarget = &dst;
            }
            else if(enPassantTarget != NULL && dst.getColumn() == enPassantTarget->getColumn() && dst.getRow() == enPassantTarget->getRow()-1){
                if(src.getRow() == enPassantTarget->getRow() && (src.getColumn() == dst.getColumn()-1 || src.getColumn() == dst.getColumn()+1)){
                    state = true;
                    _grid[enPassantTarget->getRow()][enPassantTarget->getColumn()].destroyBit();
                    enPassantTarget = NULL;
                }
            }
            else if(dst.getRow() == 0){
                state = true;
                _grid[dst.getRow()][dst.getColumn()].destroyBit();
                placePiece(Queen, dst.getColumn(), dst.getRow(), 1);
            }
            else{
                enPassantTarget = NULL;
            }
            
        }
    }
    if(!state){
        enPassantTarget = NULL;
        dst.destroyBit();
        placePiece((ChessPiece)bit.gameTag(), dst.getColumn(), dst.getRow(), player);
        src.destroyBit();
    }
    endTurn();
}