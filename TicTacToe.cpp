#include <iostream>
#include <vector>
#include <limits>
using namespace std;

vector<char> board(9, ' ');

// Function to print the Tic Tac Toe board
void printBoard(){
    cout<<endl;
    for(int i=0; i<9; i++){
        cout<<" "<<board[i]<<" ";
        if(i%3 != 2) cout<<"|";
        if(i%3==2 && i!=8) cout<<"\n---+---+---\n";
    }
    cout<<endl<<endl;
}

// Function to check if a player has won
bool checkWinner(char player){
    int winPatterns[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8},
        {0,3,6}, {1,4,7}, {2,5,8},
        {0,4,8}, {2,4,6}
    };

    for(auto pattern : winPatterns){
        if(board[pattern[0]] == player && board[pattern[1]] == player && board[pattern[2]] == player)
        return 1;
    }
    return 0;
}

// Function to check if the game is a draw
bool isDraw(){
    for(int i=0; i<9; i++){
        if(board[i] == ' ') return 0;
    }
    return 1;
}

// Function to evaluate the board
// Returns positive value if AI wins, negative if player wins, and 0 for draw
// The depth is used to minimize the number of moves for the AI
int evaluate(int depth){
    if(checkWinner('O')) return 10-depth;
    if(checkWinner('X')) return depth-10;
    return 0;
}

// Minimax algorithm with alpha-beta pruning
int minimax(int depth, bool isMaximizing, int alpha, int beta){
    int score = evaluate(depth);

    if(score != 0 || isDraw()) return score;

    if(isMaximizing){
        int bestValue = INT_MIN;
        for(int i=0; i<9; i++){
            if(board[i] == ' '){
                board[i] = 'O';
                bestValue = max(bestValue, minimax(depth + 1, false, alpha, beta));
                board[i] = ' ';
                alpha = max(alpha, bestValue);
                if(beta <= alpha) break; 
            }
        }
        return bestValue;
    }
    else{
        int bestValue = INT_MAX;
        for(int i=0; i<9; i++){
            if(board[i] == ' '){
                board[i] = 'X';
                bestValue = min(bestValue, minimax(depth + 1, true, alpha, beta));
                board[i] = ' ';
                beta = min(beta, bestValue);
                if(beta <= alpha) break;
            }
        }
        return bestValue;
    }
}

// Function to find the best move for AI
int findBestMove(){
    int bestValue=-1;
    int bestMove=-1;

    for(int i=0; i<9; i++){
        if(board[i] == ' '){
            board[i]='O';
            int moveValue = minimax(0, false, INT_MIN, INT_MAX);
            board[i]=' ';
            if(moveValue > bestValue){
                bestValue = moveValue;
                bestMove = i;
            }
        }
    }
    return bestMove;
}

void playGame() {
    cout << "\nWelcome to Tic Tac Toe (You = X, AI = O)\n";
    printBoard();

    while (true) {
        // Player's turn
        int move;
        cout << "Enter your move (1-9): ";
        cin >> move;
        if (move < 1 || move > 9 || board[move-1] != ' ') {
            cout << "Invalid move. Try again.\n";
            continue;
        }

        board[move-1] = 'X';
        printBoard();

        if (checkWinner('X')) {
            cout << "You win!\n\n";
            break;
        }
        if (isDraw()) {
            cout << "It's a draw!\n\n";
            break;
        }

        //AI's turn
        int aiMove = findBestMove();
        board[aiMove] = 'O';
        cout << "AI played:\n";
        printBoard();

        if (checkWinner('O')) {
            cout << "AI wins!\n\n";
            break;
        }
        if (isDraw()) {
            cout << "It's a draw!\n\n";
            break;
        }
    }
 }

int main() {
    playGame();
    return 0;
}
