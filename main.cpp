//SALVY CAVICCHIO
//AI PROJECT MINI-CAMELOT
// FALL 2017
#include <vector>
#include <string>
#include <iostream>
#include <bitset>
#include <fstream>
#include <time.h>

using namespace std;

class gameBoard {
public:
    gameBoard() {
        // set values for where the castles are;
        whiteCastle[3] = 1;
        whiteCastle[4] = 1;
        blackCastle[107] = 1;
        blackCastle[108] = 1;
        // set values for where illegal board spots are
        legalBoard.set();
        legalBoard[0] = false;
        legalBoard[1] = false;
        legalBoard[2] = false;
        legalBoard[5] = false;
        legalBoard[6] = false;
        legalBoard[7] = false;
        legalBoard[8] = false;
        legalBoard[9] = false;
        legalBoard[14] = false;
        legalBoard[15] = false;
        legalBoard[16] = false;
        legalBoard[23] = false;
        legalBoard[111-0] = false;
        legalBoard[111-1] = false;
        legalBoard[111-2] = false;
        legalBoard[111-5] = false;
        legalBoard[111-6] = false;
        legalBoard[111-7] = false;
        legalBoard[111-8] = false;
        legalBoard[111-9] = false;
        legalBoard[111-14] = false;
        legalBoard[111-15] = false;
        legalBoard[111-16] = false;
        legalBoard[111-23] = false;
        // set initial location of white pieces
        whitePieces[34] = true;
        whitePieces[35] = true;
        whitePieces[36] = true;
        whitePieces[37] = true;
        whitePieces[43] = true;
        whitePieces[44] = true;
        // set initial location of black pieces
        blackPieces[111-34] = true;
        blackPieces[111-35] = true;
        blackPieces[111-36] = true;
        blackPieces[111-37] = true;
        blackPieces[111-43] = true;
        blackPieces[111-44] = true;
    }
    
    // returns a bitset of the spots that are currently occupied by a piece, or are not legal. AKA u cant be here.
    bitset<14*8> getTakenSpots() const {
        return (~legalBoard|whitePieces|blackPieces);
    }
    
    // returns size of the bitset. In this case is constant
    int getLength() const { return 111; }
    
    // main display method
    void showBoard() const {
        
        int nRow = 14;
        int nCol = 8;
        char theChar = '-';
        
        cout << "    A B C D E F G H" << endl;
        cout << "------------------" << endl;
        cout << " 1| ";
        int count = 1;
        
        for (int i = 1; i <= nRow*nCol; i++) {
            if (!legalBoard[i-1]) {theChar = 'X'; }
            else if (whitePieces[i-1]) { theChar = 'W'; }
            else if (blackPieces[i-1]) { theChar = 'B'; }
            else if (blackCastle[i-1]|whiteCastle[i-1]) { theChar = '*'; }
            else { theChar = '-'; }
            
            cout << theChar << " ";
            if (i%nCol == 0) {
                cout << endl;
                count+=1;
                if(count<10)
                    cout << " " << count <<"| ";
                else if (count<15) cout << count <<"| ";
            }
        }
    }
    
    // checks to see if this is a capture move
    bool checkCaptureInside(int startSpot, int theMove, bool white) const {
    //    cout << "entered function checkCapture" << endl;
        // we can do this by seeing if the spot has an enemy piece using math!
        // use math! try and "discover" the index that will be capured from this move
        // if this is positive, then the move was down or right
        // if negative, the move is up or left
        int canterDistance = theMove - startSpot;
 //       cout << "Canter Distance is: " << canterDistance << endl;
        // this is the location of the "target capture" piece
        int canterSpot = startSpot + canterDistance/2;
  //      cout << "Canter Spot is: " << canterSpot << endl;
        if (white) {
            if (blackPieces[canterSpot]) { return true; }
        }
        else {
            if (whitePieces[canterSpot]) { return true; }
        }
        return false;
    };

    // executes a given move of a piece
    void executeMove(int startSpot, int newSpot, bool white) {
 //       cout << "entered execute move" << endl;
        if (white) {
  //          cout << "this was a WHITE move" << endl;
            whitePieces[startSpot] = false;
            whitePieces[newSpot] = true;
  //          cout << "startSpot for EXECUTRE: " << startSpot << endl;
 //           cout << "NEW SPOT FOR EXECUTE: " << newSpot << endl;
            if (checkCaptureInside(startSpot, newSpot, white)) {
             //   cout << "Checking Capture" << endl;
                int canterDistance = newSpot - startSpot;
             //   cout << "newSpot: " << newSpot << endl;
             //   cout << "startSpot: " << startSpot << endl;
             //   cout << "canterDistance: " << canterDistance << endl;
                int canterSpot = startSpot + canterDistance/2;
                blackPieces[canterSpot] = 0;
 //               cout << "You captured the enemy piece (black was captured)!" << endl;
            }
        }
        else {
            blackPieces[startSpot] = 0;
            blackPieces[newSpot] = 1;
            if (checkCaptureInside(startSpot, newSpot, white)) {
            //    cout << "Checking CaptureB" << endl;
                int canterDistance = newSpot - startSpot;
                int canterSpot = startSpot + canterDistance/2;
            //    cout << "newSpot: " << newSpot << endl;
            //    cout << "startSpot: " << startSpot << endl;
            //    cout << "canterDistance: " << canterDistance << endl;
                whitePieces[canterSpot] = 0;
            //   cout << "The enemy captured your piece (white was captured)!" << endl;
            }
        }
  //      cout << "SUCSESSFULLY EXECUTED MOVE!" << endl;
    };
    
    // checks for win with bitset math
    bool checkWin(bool white) const {
        if (white) { return ((whitePieces&blackCastle).count() == 2) | (blackPieces.any()==0); }
        else { return ((blackPieces&whiteCastle).count() == 2)| (whitePieces.any()==0); }
    };
    // checks for draw with bitset math. This is valid if there are not enough pieces left for any player to win.
    bool checkDraw() const {
        return ((whitePieces.count() == 1) & (blackPieces.count() == 1));
    };
    
    // white wants higher numbers...
    
    /// osoo..... for just white benefit... make 1 item in the castle be +250
    // then have the average distance of the white pieces be a factor
    // also factor the number of living white piececs
//    // then for BLACk - do the same thing but subtract it from the white score. Thats the return!
//    int evaluation() const {
//        if (checkWin(true)) { return 1000; }
//        if (checkWin(false)) { return -1000; }
//
//        return 0;
//    };
//
    // evaluation score
    /*
     The evaluation function used takes into account the number 
     of pieces a player has left on the board, the sum of the 
     distances of each piece from the castle, and the distance 
     of the closest piece a player has to a castle. The number 
     of pieces a player has is important because it directly 
     correlates to the number of moves they will be able to perform.
     The weight of the evaluation function is more heavily placed 
     on the distance of the closest piece a player has to a castle
     so that the program will attempt to become “safe” by placing 
     a close piece in the castle rather than moving a “second” 
     piece that is farther away
     
     */
        int evaluation() const {
            if (checkWin(true)) { return 10000; }
            if (checkWin(false)) { return -10000; }
    
            int valForOneCastle = 4500;
            int theScore = 0;
    
            // check if a white or black piece is in the castle
            if ((blackCastle&whitePieces).any()) { theScore += valForOneCastle; }
            if ((whiteCastle&blackPieces).any()) { theScore -= valForOneCastle; }
    
            // add the values for avg dist
            int totalWhiteDist = 0;
            int totalBlackDist = 0;
            int closestWhite = 0;
            int closestBlack = 111;
            
            for (int i = 0; i < whitePieces.size(); i++) {
                if (whitePieces[i] ==1) {
                    closestWhite = max(closestWhite,i);
                    totalWhiteDist+=(111-i-4);
                }
                if (blackPieces[i] == 1) {
                    closestBlack = min(closestBlack,i);
                    totalBlackDist+=(i-4);
                }
            }

            int whitePieceAndDist = 1000-(int(whitePieces.count())*totalWhiteDist + closestWhite*3);
            int blackPieceAndDist = 1000-(int(blackPieces.count())*totalBlackDist + closestBlack*3);
    
            theScore+=whitePieceAndDist;
            theScore-=blackPieceAndDist;
            
            return theScore;
            };

    // checks for win
    int terminalTest() const {
        return checkWin(true)|checkWin(false)|checkDraw();
    }
//    
////    Bitsets were chosen to represent the board so that I could take
//    advantage of the processing speed of bitset operators such as AND,
//    NOT, and OR. These operators are used heavily in the functions that
//    check to see if a move is valid (this is done by checking
//    if(legalMoves&whitePieces == legalMoves|whitePieces), and also in the
//    functions that check if a player has won the game (by performing
//if((blackPieces&whiteCastle).any==1). Bitset math is very efficient
//    and takes 8 times less memory space than a similar boolean array.
//
//    
    bitset<14*8> whitePieces;
    bitset<14*8> blackPieces;
    bitset<14*8> legalBoard;
    bitset<14*8> blackCastle;
    bitset<14*8> whiteCastle;
};

// need to write display function for bitsets to look like the board;
// this was mainly used for initial testing
void displayBitset(bitset<14*8> theSet) {
    
    int nRow = 14;
    int nCol = 8;
    cout << "    A B C D E F G H" << endl;
    cout << "------------------" << endl;
    cout << " 1| ";
    int count = 1;
    
    for (int i = 1; i <= nRow*nCol; i++) {
        cout << theSet[i-1] << " ";
        if (i%nCol == 0) {
            cout << endl;
            count+=1;
            if(count<10)
                cout << " " << count <<"| ";
            else if (count<15) cout << count <<"| ";
        }
    }
}

// gives location of a C7 in the bitset vector
int letterToLocation(char column, int row) {
    int columnNum = int(column - 'A');
 //   cout << "columnNum" << columnNum << endl;
 //   cout << "Location given was spot: " << columnNum+((row-1)*8) << endl;
    return columnNum+((row-1)*8);}

// checks if a move is valid
bool checkSimpleMove(const gameBoard& theBoard, int aMove) {
//    cout << "   Entered function checkSimpleMove" << endl;
    // check start of board... again not nessecary
    if (aMove < 0) { return false; }
//    cout << "   Passed start test" << endl;
    // makes sure move didnt go over the board.. not often nessecary for simple moves.
    if (aMove > theBoard.getLength()) { return false; }
//    cout << "   Passed end test" << endl;
    // checks to see if any pieces are on the board
    if (theBoard.getTakenSpots()[aMove] == true) { return false; }
//    cout << "   Passed taken test" << endl;
    return true;
}

// THIS FUNCTION ONLY CHECKS IF A JUMPING MOVE IS OK, IT DOES NOT CHECK IF IT IS A CAPTURING MOVE
bool checkCanteringMove(const gameBoard& theBoard, int aMove, bool white, int startSpot) {
//    cout << "Entered function checkCanteringMove!" << endl;
    // take advantage of the code I already wrote to see if the move is valid
    // this checks if it is before or after the start of the board,
    // or if it is landing in a taken spot.
//    cout << "aMove = " << aMove << endl;
    
    if (!checkSimpleMove(theBoard, aMove)) { return false; }
//    cout << "Passed the simple move check!" << endl;
    // now for cantering specific checks
    // use math! try and "discover" the index that will be capured from this move
    // if this is positive, then the move was down or right
    // if negative, the move is up or left
    int canterDistance = aMove - startSpot;
    bool isItAbsDistMatch = false;
    int directCanter[4] = {2,14,16,18};
    for (int item : directCanter) {
        if (abs(canterDistance)==item) { isItAbsDistMatch = true; break; }
    }
    if(!isItAbsDistMatch) { return false; }
    
//    cout << "Canter Distance is: " << canterDistance << endl;
    // this is the location of the "target capture" piece
    int canterSpot = startSpot + canterDistance/2;
//    cout << "Canter Spot is: " << canterSpot << endl;
    
    // check to make sure there is actually a piece in the canter spot and that it is in fact a cantering move (dist>8)
    if (!theBoard.getTakenSpots()[canterSpot]) {
//        cout << "Cannot Canter over an empty space" << endl;
        return false;
    }
    // if this somehow does not have a return already... probably unneccesary
    return true;
}

// only used in testing
void displayGoodMoves(const vector<int>& theMoves) {
    bitset<14*8> goodMoves;
    for (size_t i = 0; i < theMoves.size(); i++) {
        goodMoves[theMoves[i]] = true;
    }
    displayBitset(goodMoves);
}

// stops wrap around moves
bool checkIfLeftEdge(int aPiece) {
    for (int i = 0; i <= 111; i+=8) {
        if (aPiece==i) { return true; }
    }
    return false;
}


// stops wrap around moves
bool checkIfRightEdge(int aPiece) {
    for (int i = 7; i <= 111; i+=8) {
        if (aPiece==i) { return true; }
    }
    return false;
}

// gets all the good moves
vector<int> getGoodMoves(const gameBoard& theBoard, int thePiece, bool white) {
//    cout << "Piece:" << thePiece << endl;
//    cout << "White: " << white << endl;
    vector<int> theReturn;
    
    if (white) {
        if (theBoard.blackCastle[thePiece]==1) {
            return theReturn;
        }
    }
    else {
        if (theBoard.whiteCastle[thePiece]==1) {
            return theReturn;
        }
    }

    vector<int> simpleMoves;
    simpleMoves.reserve(8);
    vector<int> canterMoves;
    canterMoves.reserve(8);
    

    int row = 8;
    int column = 1;
    
    //all the simple moves
       int up = thePiece-row;
    int down = thePiece+row;
   
          simpleMoves.push_back(up);
    simpleMoves.push_back(down);
          
    // all the cantering and capturing moves
      int canterUp = thePiece-row*2;
    int canterDown = thePiece+row*2;
            canterMoves.push_back(canterUp);
    canterMoves.push_back(canterDown);
    
    
    if(!checkIfRightEdge(thePiece)) {
        int right = thePiece+column;
        int upRight = thePiece-row+column;
        int downRight =	thePiece+row+column;
        int canterRight = thePiece+column*2;
        int canterUpperRight = thePiece+(-row+column)*2;
        int canterLowerRight = thePiece+(row+column)*2;
        simpleMoves.push_back(right); // for check right edge
        simpleMoves.push_back(upRight); // check right edge
        simpleMoves.push_back(downRight); // check right edge
        canterMoves.push_back(canterRight); // check right edge
        canterMoves.push_back(canterLowerRight);// check right edge
        canterMoves.push_back(canterUpperRight);// check right edge
    }
    
    if(!checkIfLeftEdge(thePiece)) {
        int left = thePiece-column;
        int upLeft = thePiece-row-column;
        int downLeft = thePiece+row-column;
        simpleMoves.push_back(left);
        simpleMoves.push_back(upLeft);
        simpleMoves.push_back(downLeft);
        int canterLeft = thePiece-column*2;
        int canterUpperLeft = thePiece+(-row-column)*2;
        int canterLowerLeft = thePiece+(row-column)*2;
        canterMoves.push_back(canterLeft);
        canterMoves.push_back(canterUpperLeft);
        canterMoves.push_back(canterLowerLeft);
    }
    
    
    //check simple moves and add to return
    for (size_t i = 0; i < simpleMoves.size(); i++) {
        //only adds to return if it is a good simple move
        if (checkSimpleMove(theBoard, simpleMoves[i])) {
 //           cout << "Good Simple Move" << endl;
            theReturn.push_back(simpleMoves[i]);
        }
//        else { cout << "Bad Simple Move" << endl; }
    }
    simpleMoves.clear();
    //check canter moves and add to return
    for (size_t i = 0; i < canterMoves.size(); i++) {
        // if good canter move it will add to the return as well
        if (checkCanteringMove(theBoard, canterMoves[i], white, thePiece)) {
//            cout << "Good Canter Move" << endl;
            theReturn.push_back(canterMoves[i]);
        }
 //       else { cout << "Bad Canter Move" << endl; }
    }
    canterMoves.clear();
    return theReturn;
};

// to check if a move is jumping over an enemy piece (ie: capturing)
// only called if a move is already a cantering move
bool checkCapture(const gameBoard& theBoard, int startSpot, int theMove, bool white) {
 //   cout << "entered function checkCapture" << endl;
    // we can do this by seeing if the spot has an enemy piece using math!
    // use math! try and "discover" the index that will be capured from this move
    // if this is positive, then the move was down or right
    // if negative, the move is up or left
    int canterDistance = theMove - startSpot;
    bool isItAbsDistMatch = false;
    int directCanter[4] = {2,14,16,18};
    for (int item : directCanter) {
        if (abs(canterDistance)==item) { isItAbsDistMatch = true; break; }
    }
    if(!isItAbsDistMatch) {/* cout << "FIANLLY FIXED THE CAPTURING BUG" */ return false; }
    
    int rowA = (startSpot/8)+1;
    int colA = startSpot%8;
    int rowB = (theMove/8)+1;
    int colB = theMove%8;
    
    if (abs(rowA-rowB)!=2|abs(colA-colB)!=2) { return false; }
    
    
    // hardcode a bug fix in capturing a non capture piece. not needed after i put in the fix
    if (startSpot == letterToLocation('E', 10)) {
        if (theMove == letterToLocation('E', 9)) {
            return false;
        }
    }
//    cout << "Canter Distance is: " << canterDistance << endl;
    // this is the location of the "target capture" piece
    int canterSpot = startSpot + canterDistance/2;
//    cout << "Canter Spot is: " << canterSpot << endl;
    if (white) {
        if (theBoard.blackPieces[canterSpot]) { return true; }
    }
    else {
        if (theBoard.whitePieces[canterSpot]) { return true; }
    }
    return false;
};

// creates a vector of all the capturing moves that exist
vector<int> getCaptureMoves(const gameBoard& theBoard, int thePiece, bool white) {
 //   cout << "entered function getCaptureMoves" << endl;
    // initialize return
    vector<int> theReturn;
    // gets a vector of all the good moves
    vector<int> allGoodMoves;
 //   cout << thePiece << " " << white << endl;
 //   theBoard.showBoard();
    allGoodMoves = getGoodMoves(theBoard, thePiece, white);
    for (size_t i = 0; i < allGoodMoves.size(); i++) {
        if (checkCapture(theBoard, thePiece, allGoodMoves[i], white)) {
            theReturn.push_back(allGoodMoves[i]);
        }
    }
    return theReturn;
};

void PlayerMove(gameBoard& theGame, bool white) {
    char startCol;
    int startRow;
    cout << "Enter the piece you want to move: ('C',4)" << endl;
    cin >> startCol >> startRow;
    
    int pieceA = letterToLocation(startCol, startRow);
//    cout << "pieceA: " << pieceA;
    
    bool pieceExists = false;
    while(!pieceExists) {
        if (theGame.whitePieces[pieceA]) {
            pieceExists=true;
            break;
        }
        cout << "That piece does not exists!" << endl;
        cout << "Enter the piece you want to move: ('C',4)" << endl;
        cin >> startCol >> startRow;
        pieceA = letterToLocation(startCol, startRow);
    }
    
    char endCol;
    int endRow;
    cout << "Enter the spot you want to move to: ('C',4)" << endl;
    cin >> endCol >> endRow;
    int spot = letterToLocation(endCol, endRow);
    
    vector<int> captureMoves = getCaptureMoves(theGame, pieceA, white);
    vector<int> goodMoves = getGoodMoves(theGame, pieceA, white);
    bool moveExecuted = false;
    
    while(!moveExecuted) {
        spot = letterToLocation(endCol, endRow);

        if (!captureMoves.empty()) {
            for (size_t i = 0; i < captureMoves.size(); i++) {
                if (captureMoves[i]==spot) {
                    theGame.executeMove(pieceA, spot, white);
  //                  cout << "chose good capture move!" << endl;
                    return;
                }
            }
            cerr << "error, you need to make a capturing move!" << endl;
            cerr<< "Current piece is: " << endCol << endRow << endl;
            cerr<<"avaliable capturing moves:";
            for (int thing : captureMoves) { cout << " " << thing; }
        }
        else {
            for (size_t i = 0; i < goodMoves.size(); i++) {
 //               cout << "spot: " << spot << endl;
 //               cout << "goodMove[i]" << goodMoves[i] << endl;
                if (goodMoves[i]==spot) {
 //                   cout << "PIECE A TO EXECUTE: " << pieceA << endl;
 //                   cout << "SPOT TO EXECUTE TO: " << spot << endl;
                    theGame.executeMove(pieceA, spot, white);
 //                   cout << "chose good move!" << endl;
                    return;
                }
            }
            cerr << "error, you need to choose a good move!" << endl;
        }
        cout << "Enter the spot you want to move to: ('C',4)" << endl;
        cout << "The current piece selected is: " << startCol << startRow << endl;
        cin >> endCol >> endRow;
    }
}

// THIS IS THE ALPHA BETA SEARCH ALGORITM
/*
 function ALPHA-BETA-SEARCH(state) returns an action
 v ← MAX-VALUE(state, minimum_utility, maximum_utility)
 return the action in ACTIONS(state) with value v
 function MAX-VALUE(state, α, β) returns a utility value
 if TERMINAL-TEST(state) then return UTILITY(state)
 if CUTOFF(state) then return EVAL(state)
 v ← -∞
 for the next a in ACTIONS(state) do
 v ← MAX(v, MIN-VALUE(RESULT(state, a), α, β))
 if v ≥ β then return v
 α← MAX(α, v)
 return v
 function MIN-VALUE(state, α, β) returns a utility value
 if TERMINAL-TEST(state) then return UTILITY(state)
 if CUTOFF(state) then return EVAL(state)
 v ← +∞
 for the next a in ACTIONS(state) do
 v ← MIN(v, MAX-VALUE(RESULT(state, a), α, β))
 if v ≤ α then return v
 β ← MIN(β, v)
 CS 4613 Fall 2017
 Project Hints E. K. Wong
 2
 return v
 function CUTOFF(state) returns true or false
 if state.depth = limit then return true else return false
 */

// END OF ALPHA-BETA
// *********************

// generates all possible black moves (we know the AI is always black.)
vector<pair<int,int> > generatePossibleBlackMoves(gameBoard theBoard) {
    // return
    vector<pair<int,int> > theReturn;
    // store locations of pieces
    vector<int> locations;
    // get the locations of black pieces
    for (int i = 0; i < theBoard.blackPieces.size(); i++) {
        if (theBoard.blackPieces[i]==1) { locations.push_back(i); }
    }
    // get possible CANTERING moves of each piece and push into vector<pair>
    if (locations.size()>0) {
    for (int i = 0; i < locations.size(); i++) {
        vector<int> theseMoves = getCaptureMoves(theBoard, locations[i], false);
        for (int j = 0; j < theseMoves.size(); j++) {
            pair<int,int> temp = make_pair(locations[i],theseMoves[j]);
            theReturn.push_back(temp);
        }
    }
    }
    // IF there are no cantering moves, get regular moves
    if (theReturn.empty()) {
        for (int i = 0; i < locations.size(); i++) {
            vector<int> theseMoves = getGoodMoves(theBoard, locations[i], false);
            for (int j = 0; j < theseMoves.size(); j++) {
                pair<int,int> temp = make_pair(locations[i],theseMoves[j]);
                theReturn.push_back(temp);
            }
        }
    }
    return theReturn;
};

// same for the white moves
vector<pair<int,int> > generatePossibleWhiteMoves(gameBoard theBoard) {
    // return
    vector<pair<int,int> > theReturn;
    // store locations of pieces
    vector<int> locations;
    // get the locations of black pieces
    for (int i = 0; i < theBoard.whitePieces.size(); i++) {
        if (theBoard.whitePieces[i]==1) { locations.push_back(i); }
    }
    // get possible CANTERING moves of each piece and push into vector<pair>
    for (int i = 0; i < locations.size(); i++) {
        vector<int> theseMoves = getCaptureMoves(theBoard, locations[i], false);
        for (int j = 0; j < theseMoves.size(); j++) {
            pair<int,int> temp = make_pair(locations[i],theseMoves[j]);
            theReturn.push_back(temp);
        }
    }
    // IF there are no cantering moves, get regular moves
    if (theReturn.empty()) {
        for (int i = 0; i < locations.size(); i++) {
            vector<int> theseMoves = getGoodMoves(theBoard, locations[i], false);
            for (int j = 0; j < theseMoves.size(); j++) {
                pair<int,int> temp = make_pair(locations[i],theseMoves[j]);
                theReturn.push_back(temp);
            }
        }
    }
    return theReturn;
};

vector<gameBoard> makeStates(gameBoard& initial, bool white) {
    vector<gameBoard> theReturn;
    vector<pair<int,int> > moves;
    
   
    
    if(white) {
        moves = generatePossibleWhiteMoves(initial);
     //   cout << "number of possible white moves: " << moves.size() << endl;
    }
    else {
        moves = generatePossibleBlackMoves(initial);
 //       cout << "number of possible black moves: " << moves.size() << endl;
    }
 //   cout << "about to loop nTimes: " << moves.size() << endl;
    for (pair<int,int> each : moves) {
      
        gameBoard temp = initial;
 //       cout << "made temp: " << endl;
 //       temp.showBoard();
        temp.executeMove(each.first, each.second, white);
 //       cout << "executed Move: " << endl;
 //     temp.showBoard();
        theReturn.push_back(temp);
    }
    return theReturn;
}

int minValue(gameBoard& state, int depth, int alpha, int beta);
int maxValue(gameBoard& state, int depth, int alpha, int beta);

gameBoard alphabetaSearch(gameBoard& game);
const int MAX = 10000;
const int MIN = -10000;
int NODES = 0;
int MAX_PRUNES = 0;
int MIN_PRUNES = 0;
int DEPTH_REACHED = 0;
int difficultyLevel;
//const int DRAW = 0;

int maxValue(gameBoard& state, int depth, int alpha, int beta){
    NODES++;
    DEPTH_REACHED = difficultyLevel-depth+1;
    //cout << "in max" << endl;
    
    //if at end of depth for searching return some evaluated value
    if (depth == 0) { return state.evaluation(); }
    
    if( state.terminalTest()) {
        int end = state.evaluation();
//        cout << "max " << end << endl;
        return end ; // game.utilityVal(state);
    }
    // cout << end << endl;
    // if(end != -1) {
    // 	cout << "max "  << end << endl;
    // 	//state.printBoard();
    // 	return end;
    
    // }
    int bestMove = MIN;
    
    vector<gameBoard> newStates = makeStates(state, false);
    if (newStates.size() != 0) {
    for(gameBoard& b : newStates){
     //   b.printBoard();
        // cout << "number of white " << b.numWhite << endl;
        // cout << "number of black " << b.numBlack << endl;
        
        int minReturned = minValue(b, depth-1, alpha, beta);
        //cout << "minReturned " << minReturned << endl;
        bestMove = max(bestMove, minReturned);
        
        if (bestMove >= beta) {
            MAX_PRUNES++;
        //    cout << "in max " << bestMove << " beta " << beta << endl;
            return bestMove;
        }
        alpha = max(alpha, bestMove);
    //    cout << "in max " << bestMove << " alpha " << alpha << endl;
    }
    }
    return bestMove;
}

int minValue(gameBoard& state, int depth, int alpha, int beta){
    NODES++;
    DEPTH_REACHED = difficultyLevel-depth+1;
    //cout << "in max" << endl;
    
    //if at end of depth for searching return some evaluated value
     if (depth == 0) { return state.evaluation(); }
    
    if( state.terminalTest()) {
        int end = state.evaluation();
//        cout << "max " << end << endl;
        return end ; // game.utilityVal(state);
    }
    // cout << end << endl;
    // if(end != -1) {
    // 	cout << "max "  << end << endl;
    // 	//state.printBoard();
    // 	return end;
    
    // }
    int bestMove = MAX;
    
    vector<gameBoard> newStates = makeStates(state, true);
    for(gameBoard& b : newStates){
        //   b.printBoard();
        // cout << "number of white " << b.numWhite << endl;
        // cout << "number of black " << b.numBlack << endl;
        
        int maxReturned = maxValue(b, depth-1, alpha, beta);
        //cout << "minReturned " << minReturned << endl;
        bestMove = min(bestMove, maxReturned);
        
        if (bestMove <= alpha) {
            MIN_PRUNES++;
            //    cout << "in max " << bestMove << " beta " << beta << endl;
            return bestMove;
        }
        beta = min(beta, bestMove);
        //    cout << "in max " << bestMove << " alpha " << alpha << endl;
    }
    return bestMove;
}

gameBoard alphabetaSearch(gameBoard& game, int depth){
    //Board alphabetaSearch(const vector<Board>& states , int player){
    
    
    int bestMove= MIN;
    
    gameBoard bestMoveBoard;
    
    int value;
    
    int alpha = MIN;
    int beta = MAX;
    
    gameBoard b = game;
    
    vector<gameBoard> states = makeStates(b, false);
 //   cout << "made states intially in alphabetaSearch" << endl;
 //   cout << "number of states to search: " << states.size() << endl;
    
    for(int i = 0; i < states.size(); ++i){
        //check for all possible moves
        gameBoard exploreState = states[i];
        
//        cout << "for every i in the states: " << endl;
        value = maxValue(game, depth, alpha, beta);
 //       cout << "done with one max" << endl;
        
        if (value > bestMove){
            //cout << "here" << endl;
            bestMove = value;
            bestMoveBoard = exploreState;
        }
    }
    return bestMoveBoard;
    
}


// START OF MAIN PROGRAM
int main () {
    cout << string(100,'\n') << endl;
    cout << "WELCOME TO MINI-CAMELOT!!!!!!!" << endl << endl << endl << endl << endl;
    cout << "Written by Salvy Cavicchio" << endl << endl << endl << endl << endl;
    gameBoard theGame;
    cout << "Game Start!!" << endl;
    //testing for an edge case
  //  theGame.blackPieces[10] = 1;
  //  vector<int> moves = getGoodMoves(theGame, 10, false);
  //  displayGoodMoves(moves);
//    
//    // while nobody has won
//    // let person move
//    // check for win (break)
//    // let AI move
//    // check for win (break)
//    
    int winner = 0;
    int turnNumber = 1;
    
// if player color == true, then the player is WHITE
    bool playerColor = true;
    char trash;
    
    cout << "What difficulty> (Enter 2, 3, or 4)" << endl;
    cin >> difficultyLevel;

    if (difficultyLevel>4) {
    cout << "WARNING: Difficulty higher than 4 will take a long time to compute!" << endl;
        cout << "Press any key and Enter to Continue." << endl;
        cin >> trash;
    }
    
    cout << "You are going to be the White Player!" << endl;
    cout << "Do you want to go first? (y/n) " << endl;
    char turn;
    cin >> turn;
    
    
    theGame.showBoard();
    
    // player moves first
    if (turn == 'y') {
        PlayerMove(theGame, playerColor);
    }
    
    while (winner==0) {
        cout << string(100,'\n') << endl;
        cout << "Turn number: " << turnNumber << endl;
        cout << "Current Board: " << endl;
        theGame.showBoard();
        
        cout << "\nAI THINKING..." << endl;
        
        theGame = alphabetaSearch(theGame, difficultyLevel);
        
        cout << "Nodes created: " << NODES << endl;
        cout << "Max Prunes: " << MAX_PRUNES << endl;
        cout << "Min Prunes: " << MIN_PRUNES << endl;
        cout << "Depth Reached: " << DEPTH_REACHED << endl;
        
        if (theGame.checkWin(false)) {
            winner = 2;
            break;
        }
        if (theGame.checkDraw()) {
            cout << "END GAME: DRAW" << endl;
            break;
        }
        cout << "Your Move!" << endl;
        theGame.showBoard();
        
        PlayerMove(theGame, playerColor);
        if (theGame.checkWin(true)) {
            winner = 1;
            break;
        }
        turnNumber++;
        NODES = 0;
        MAX_PRUNES = 0;
        MIN_PRUNES = 0;
        DEPTH_REACHED = 0;
    }
    
    theGame.showBoard();
    
    if (winner == 1) {
        cout << "White Won!" << endl;
    }
    if (winner == 2) {
        cout << "Black Won!" << endl;
    }
    return 0;
};
