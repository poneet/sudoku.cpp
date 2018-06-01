#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <math.h>
using namespace std;

#define MAXFREEBEES 80                              // maximum number of revealed numbers on the puzzle.
#define FREEBEES 50                                 // the amount of revealed numbers on the puzzle. Higher value = easier puzzle.

class Sudoku {
private:
    
    int freebeeaddress[80];                         // value=0=init, value=1=freebee (revealed number) ... index/9 = row, index%9 = column
    int puzzle[9][9];                               // puzzle to be output + played on.
    int solution[9][9];                             // complete 3x3, 9x9 puzzle solution.
    
    void Debug(void);                               // debugging function. use to output whatever.
    void Draw(void);                                // draws puzzle with a grid.
    int FreebeeCheck(int r, int c);                 // returns 1 if the given address points to a freebee, 0 if not.
    int Get(string prompt);                         // returns a valid number from 1-9 from the user.
    void Reveal(void);                              // reveals random freebees inside puzzle.
    int WinCheck(void);                             // win check. returns 1=won, 0=lost, 2=incomplete.
    
public:
    
    Sudoku(int solution_in[9][9]);                  // constructor.
    ~Sudoku() {};                                   // deconstructor.
    
    void DrawDebug(int array[9][9]);                // draws a 9x9 puzzle without a grid (debugging).
    void Play(void);                                // beta 01 of play.
};

// constructor
Sudoku::Sudoku(int solution_in[9][9]) {
    int i = 0, j = 0;
    for (i = 0; i <= 8; i++) {
        for (j = 0; j <= 8; j++) {
            // pulls in the solution for use by sudoku class functions.
            solution[i][j] = solution_in[i][j];
            // initializes puzzle (fills it with zeros).
            puzzle[i][j] = 0;
        }
    }
    
    // init freebeeaddress
    for (i = 0; i <= MAXFREEBEES; i++) freebeeaddress[i] = 0;
    
}
// ---------------------------------
//
//	    private
//
// ---------------------------------
int Sudoku::Get(string prompt) {
    //>>>cin.ignore() - Decimal values aren't handled properly.
    //FOR THE FUTURE: This code won't reprompt the user, placing reprompt within the loop causes it to reprompt more than needed.
    //Function: Will prompt the user, and only accept/return a valid integer between 1 and 9.
    int y = 0;
    cout << "\n" << prompt;
    cin >> y;
    while (y < 1 || y > 9) {
        // putting a reprompt inside the while loop will reprompt the user n-times (n=length of the user input). this loop ends when the user cooperates.
        cin >> y;
        cin.clear();
        cin.ignore();
    }
    return (y);
}
void Sudoku::Draw(void) {
    //Function: Draws the Sudoku Puzzle, coordinates and grid lines using ASCII characters. Yay.
    int i, j = 0;
    cout << " |1 2 3|4 5 6|7 8 9|\n-|- - -|- - -|- - -|\n";
    for (i = 0; i <= 8; i++) {
        // outputs row number.
        cout << i + 1 << "|";
        for (j = 0; j <= 8; j++) {
            // outputs values. 0=blank space.
            if (puzzle[i][j] == 0) {
                cout << " ";
            } else {
                cout << puzzle[i][j];
            }
            // horizontal grid lines.
            if ((j + 1) % 3 == 0) {
                cout << "|";
            } else {
                cout << " ";
            }
        }
        // new row.
        cout << endl;
        // vertical grid lines.
        if ((i + 1) % 3 == 0) {
            cout << "-|- - -|- - -|- - -|\n";
        }
    }
}
int Sudoku::FreebeeCheck(int r, int c) {
    //Function: Will return 1 if the given coordinate is a freebee. 0 if it is not.
    int i = 0, result = 0;
    // target (index value from 0-80, 81 total values) = (r)ow*9 + (c)olumn
    int target = r * 9 + c;
    
    if (freebeeaddress[target] != 0) {
        result = 1;
    }
    // result = 1 means target is a freebee (therefore not O.K. to write a value at the location)
    // result = 0 means target is empty (therefore O.K. to write the user's input to the location)
    return (result);
}
void Sudoku::Reveal(void) {
    //Function: Populates puzzle with freebees and stores their indexes in freebeeaddress.
    int i = 0, r = 0, c = 0, ri = 0;
    
    time_t t = time(NULL);
    srand(t);
    
    // a freebee is a revealed number. freebeeaddress[n] = 1 means it should be revealed/given for free to the user.
    while (i < FREEBEES) {
        // generates a (r)andom (i)ndex (ri) number from 0-80
        ri = rand() % 81;
        
        // checks to see if the location at ri is switched off (==0) so it may assign the freebee (=1) and post-incriment the counter i, prevents duplicated random freebees from incrimenting the counter i
        if (freebeeaddress[ri] == 0) {
            freebeeaddress[ri] = 1;
            i++;
        }
    }
    // convert freebees into addresses (row=/9,column=%9) and reveal them on "puzzle" using values stored in "solution".
    for (i = 0; i <= MAXFREEBEES; i++) {
        if (freebeeaddress[i] != 0) {
            r = i / 9;
            c = i % 9;
            // reveal the freebee by taking the value from the solution array.
            puzzle[r][c] = solution[r][c];
        }
    }
}
int Sudoku::WinCheck(void) {
    //Function: Will return 1 if the puzzle is won, return 0 if lost, 2 if the puzzle is incomplete.
    int won = 1;
    
    for (int i = 0; i <= 8; i++) {
        for (int j = 0; j <= 8; j++) {
            if (puzzle[i][j] != solution[i][j]) {
                // something in puzzle is different than the solution.
                if (puzzle[i][j] == 0) {
                    // it's an empty spot. return=2 (incomplete)
                    won = 2;
                } else if (won != 2) {
                    // it's a mistake. return=0 (lose)
                    won = 0;
                }
            }
        }
    }
    // won = 1 = winner
    // won = 0 = loser (mistake made)
    // won = 2 = incomplete
    return (won);
}
void Sudoku::Debug(void) {
    for (int i = 0; i <= MAXFREEBEES; i++) {
        cout << "freebeeaddress[" << i << "] = " << freebeeaddress[i] << "\n";
    }
}
// ---------------------------------
//
// 	     public
//
// ---------------------------------
void Sudoku::DrawDebug(int array[9][9]) {
    //Function: Outputs the contents of array.
    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            cout << array[i][j];
        }
        cout << endl;
    }
}
void Sudoku::Play(void) {
    int num = 0, r = 0, c = 0, isWon = 0, fcheck = 0;
    int emptySquares = 81 - FREEBEES; // used to prevent calling WinCheck before the puzzle is full.
    string ErrorMessage = "";
    
    Reveal();
    Draw();
    cout << endl;
    
    while (true) {
        // get input
        num = Get("Insert number: ");
        r = Get("Row: ") - 1;
        c = Get("Column: ") - 1;
        
        fcheck = FreebeeCheck(r, c); // 1=freebee, 0=okay
        
        if (fcheck == 1) {
            ErrorMessage = "You can't overwrite this number.";
        } else if (fcheck == 0) {
            if (puzzle[r][c] == 0) emptySquares--;
            puzzle[r][c] = num;
            ErrorMessage = "";
        }
        
        // reset input, clear screen, redraw the updated puzzle, output error message.
        num = 0, r = 0, c = 0;
        system("cls");
        Draw();
        cout << ErrorMessage << endl;
        
        // when the puzzle should be full, initiate win check.
        if (emptySquares == 0) {
            isWon = WinCheck(); // 0=lose, 1=win, 2=incomplete
            
            switch (isWon) {
                case 0:
                    system("cls");
                    cout << "FAIL." << endl;
                    break;
                    
                case 1:
                    system("cls");
                    cout << "YOU ARE AWESOME YOU WON THE GAME!!!!!1111!!111one\nYOU'RE KING/QUEEN OF THE WORLD HAHAHA!" << endl;
                    break;
            }
            break;
        }
    }
} //****************************************************************************************
int main() {
    int CompletePuzzle[9][9]={
        {9,6,5,3,7,1,2,4,8},
        {3,1,8,2,5,4,9,6,7},
        {7,4,2,6,8,9,1,5,3},
        {1,5,3,9,6,8,4,7,2},
        {2,9,6,1,4,7,8,3,5},
        {4,8,7,5,2,3,6,1,9},
        {6,7,1,8,9,5,3,2,4},
        {8,3,4,7,1,2,5,9,6},
        {5,2,9,4,3,6,7,8,1} };
    
    string PlayAgain = "";
    int Quit = 0;
    //play forever!
    while (true) {
        // make new puzzle.
        Sudoku NewPuzzle(CompletePuzzle);
        // play sudoku.
        NewPuzzle.Play();
        //play again?
        cout << "Play again (y/n)? ";
        while (true) {
            cin >> PlayAgain;
            if (PlayAgain == "n") {
                Quit = 1;
                break;
            } else if (PlayAgain == "y") {
                break;
            }
        }
        if (Quit == 1) {
            break;
        }
        system("cls");
    }
}