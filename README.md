# Rosa - Chess AI for CMPM 170

## Description

### Part 1: Basic Movement
This is Rosa, a chess engined named after the late Rosa at the Monterey Bay Aquarium. May she dine on infinite shrimp in the afterlife. As of right now the project only allowed two human players to play a simple game of chess. Key features are yet to be implemented, like check and en passant. However, all the pieces move like they should and captures work great.

There have been a few key changes made to the infrastructure of the provided skeleton. First, the function to check if a move is legal no longer believes that piece color is stored within game tags. I have refactored game.cpp to exclude that possibility. Outside of that, all the work is done in chess.cpp. In fact, a majority of the calculation is being done in the function canBitMoveFromTo(), which returns whether a move is legal or not, considering captures, blocking, pawn rules, and castling. It does not however consider check, which I plan on implementing further in the future. In retrospect, there are a lot of design decisions in the skeleton that I consider to be suboptimal, like game tags not including color, and the different classes Square and ChessPiece having a column and row value while BitHolder does not, requiring precious cpu cycles to static cast BitHolders into ChessPieces. I would have also liked to use a more bit manipulation in my approach towards move legality, but that proved insanely hard. I have decided that if I ever get the free time after this quarter, I will try to implement a quicker move search. Anyways, here's a demo.

### Part 2: FEN, promotion, and en passant. 
To add FEN support, I originally tried to tokenize the input string using delimeters, but I couldn't make that work so I used a sort of brutish while loop wihle keeping track of how many spaces I've seen so far, and doing something different depending on the current token. I generally do not account for invalid FEN strings, assuming that I would never be provided an invalid one. 

To add promotion, I simply check if a pawn is on the back row of the opposite side of the board. If it is, then simply destroy the pawn and put a queen where it was. Support for underpromotion will be available later. 

En passant was the actual challenge here. First, it must be noted that there is only one valid en passant target in any given game, as en passant can only be used against pawns that have double pushed in the previous turn, and there is only one previous turn. Then, one can store the BitHolder of the most recent en passant target. We can then check if the target of the desired move is one behind the position of the recent en passant target, and if so it is a valid move. When actually executing the most, I remembered to remove the en passant target like I would any other capture. 

Castling was implemented by having four booleans, representing the four possible castles that are valid in a game. By default, they're true for the opening position but for now can only be changed by entering a different FEN string. Support for check will be added later.

Anyways, looks like Rosa is shaping up.

![image](https://github.com/user-attachments/assets/2fbe0f12-c546-4a15-b1e5-a106cff48d9c)
The Stafford Gambit, as inspired by Eric Rosen

### Part 3: Negamax
Well here we are. I have decided that I have made a big mistake in programming Rosa. I decided to implement negamax with the provided framework for displaying on IMGUI. That was a mistake. It is unwieldly and prone to errors, and worst of all slow. Maybe even worse htan all of that, I don't really understand what's going on. I will try to remedy this mistake in the future using bitboards.

Anyways, my AI plays about as well as random. The depth that I can achieve in under a minute is maybe depth 5 or 6, which is not fantastic. Because of the way that I set up the data and the functions, Rosa uses a copy of _grid to run calculations. You might notice that that sounds horrible and slow, like I have stated prior, and you would be right. I implemented negamax with alpha beta pruning with much the same way as I did in tictactoe. The only thing is that it doesn't actually work on account of the copy of the grid used for IMGUI not being able to copy correctly due to its lack of interaction with the on screen mouse. Thus, captures don't actually happen when calculating negamax, making every move functionally identical, and thus the AI always chooses the first move. I am quite sad. However, I do think that if my implementation had an actual class of its own, it would likely work. My evaluation function is as simple as it gets, it just sums up the value of the pieces on the board. Anyways, might fix this later, my ego is damaged.

## License
This project is licensed under the MIT License.

## Contributors
- Brian Feng - Initial work

## Acknowledgments
- Chess piece sprites from [Wikipedia](https://en.wikipedia.org/wiki/Chess_piece)
- Original game engine framework by [ocornut](https://github.com/ocornut/imgui)

---
*This README is part of an educational project and is intended to serve as an example of good documentation practices.*
