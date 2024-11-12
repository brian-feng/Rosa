# Rosa - Chess AI for CMPM 170

## Description
This is Rosa, a chess engined named after the late Rosa at the Monterey Bay Aquarium. May she dine on infinite shrimp in the afterlife. As of right now the project only allowed two human players to play a simple game of chess. Key features are yet to be implemented, like check and en passant. However, all the pieces move like they should and captures work great.

There have been a few key changes made to the infrastructure of the provided skeleton. First, the function to check if a move is legal no longer believes that piece color is stored within game tags. I have refactored game.cpp to exclude that possibility. Outside of that, all the work is done in chess.cpp. In fact, a majority of the calculation is being done in the function canBitMoveFromTo(), which returns whether a move is legal or not, considering captures, blocking, pawn rules, and castling. It does not however consider check, which I plan on implementing further in the future. In retrospect, there are a lot of design decisions in the skeleton that I consider to be suboptimal, like game tags not including color, and the different classes Square and ChessPiece having a column and row value while BitHolder does not, requiring precious cpu cycles to static cast BitHolders into ChessPieces. I would have also liked to use a more bit manipulation in my approach towards move legality, but that proved insanely hard. I have decided that if I ever get the free time after this quarter, I will try to implement a quicker move search. Anyways, here's a demo.

![image](https://github.com/user-attachments/assets/2fbe0f12-c546-4a15-b1e5-a106cff48d9c)
The Stafford Gambit, as inspired by Eric Rosen

## License
This project is licensed under the MIT License.

## Contributors
- Brian Feng - Initial work

## Acknowledgments
- Chess piece sprites from [Wikipedia](https://en.wikipedia.org/wiki/Chess_piece)
- Original game engine framework by [ocornut](https://github.com/ocornut/imgui)

---
*This README is part of an educational project and is intended to serve as an example of good documentation practices.*
