#include <iostream>
#include <vector> // Include vector header
using namespace std;

vector<vector<char>> board = {
    {'r','n','b','q','k','b','n','r'},
    {'p','p','p','p','p','p','p','p'},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {'P','P','P','P','P','P','P','P'},
    {'R','N','B','Q','K','B','N','R'}
};

void display(){
    cout << "   a   b   c   d   e   f   g   h\n";
    cout << " +---+---+---+---+---+---+---+---+\n";
    for(int i=0; i<8; i++){
        cout << i+1 << "|";
        for(int j=0; j<8; j++){
            cout <<" "<< board[i][j] << " |";
        }
        cout << "\n +---+---+---+---+---+---+---+---+\n";
    }
}

bool isValidPawnMove(char piece, pair<int,int> from, pair<int, int>to){
    int fx = from.first;
    int fy = from.second;
    int tx = to.first;
    int ty = to.second;

    if(piece == 'P'){
        if(fx == tx+1 && fy == ty && board[tx][ty] == ' ') return 1;
        if(fx == 6 && fx == tx + 2 && fy == ty && board[tx][ty] == ' ' && board[fx-1][fy] == ' ') return 1;
        if(fx == tx + 1 && abs(fy-ty) == 1 && board[tx][ty] >= 'a' && board[tx][ty] <= 'z') return 1;
    }

    if(piece == 'p'){
        if(fx == tx-1 && fy == ty && board[tx][ty] == ' ') return 1;
        if(fx == 1 && fx == tx - 2 && fy == ty && board[tx][ty] == ' ' && board[fx+1][fy] == ' ') return 1;
        if(fx == tx - 1 && abs(fy-ty) == 1 && board[tx][ty] >= 'A' && board[tx][ty] <= 'Z') return 1;
    }
    return 0;
}

bool isValidKnightMove(char piece, pair<int, int>from , pair<int, int>to){
    int fx = from.first;
    int tx = from.second;
    int fy = to.first;
    int ty = to.second;

    if((abs(fx-tx) == 2 && abs(fy-ty) ==1) || (abs(fx-tx) == 1 && abs(fy-ty) == 2)){
        if(board[tx][ty] == ' ') return 1;
        if(islower(board[tx][ty]) != islower(piece)) return 1;
    }
    return 0;
}

bool isValidRookMove(char piece, pair<int,int>from, pair<int,int>to){
    int fx = from.first;
    int fy = from.second;
    int tx = to.first;
    int ty = to.second;

    if(fx != tx && fy != ty) return 0;

    while(fx != tx || fy != ty){
        if(fx < tx) fx++;
        else if(fx > tx) fx--;
        else if(fy < ty) fy++;
        else if(fy > ty) fy--;
        if(fx == tx && fy == ty) break;
        if(board[fx][fy] != ' ') return 0;
    }

    if(board[tx][ty] == ' ') return 1;
    if(islower(board[tx][ty]) != islower(piece)) return 1;
    return 0;
}

bool isValidBishopMove(char piece, pair<int,int>from, pair<int,int>to){
    int fx = from.first;
    int fy = from.second;
    int tx = to.first;
    int ty = to.second;

    if(abs(fx-tx) != abs(fy-ty)) return 0;

    while(fx != tx || fy != ty){
        if(fx < tx) fx++;
        else if(fx > tx) fx--;
        if(fy < ty) fy++;
        else if(fy > ty) fy--;
        if(fx == tx && fy == ty) break;
        if(board[fx][fy] != ' ') return 0;
    }

    if(board[tx][ty] == ' ') return 1;
    if(islower(board[tx][ty]) != islower(piece)) return 1;
    return 0;
}

bool isValidQueenMove(char piece, pair<int,int>from, pair<int,int>to){
    return isValidRookMove(piece, from, to) || isValidBishopMove(piece, from, to);
}

bool isValidKingMove(char piece, pair<int,int>from, pair<int,int>to){
    int fx = from.first;
    int fy = from.second;
    int tx = to.first;
    int ty = to.second;

    if(abs(fx-tx) <= 1 && abs(fy-ty) <= 1){
        if(board[tx][ty] == ' ') return 1;
        if(islower(board[tx][ty]) != islower(piece)) return 1;
    }
    return 0;  
}

bool isValidMove(char piece, pair<int,int>from, pair<int,int>to){
    if(piece == 'P' || piece == 'p') return isValidPawnMove(piece, from, to);
    if(piece == 'N' || piece == 'n') return isValidKnightMove(piece, from, to);
    if(piece == 'R' || piece == 'r') return isValidRookMove(piece, from, to);
    if(piece == 'B' || piece == 'b') return isValidBishopMove(piece, from, to);
    if(piece == 'Q' || piece == 'q') return isValidQueenMove(piece, from, to);
    if(piece == 'K' || piece == 'k') return isValidKingMove(piece, from, to);
    return 0;
}

bool makeMove(pair<int,int>from, pair<int,int>to, bool isWhiteTurn){
    char piece = board[from.first][from.second];

    if(isWhiteTurn && islower(piece)) return 0;

    if(isValidMove(piece, from, to)){
        board[to.first][to.second] = piece;
        board[from.first][from.second] = ' ';
        return 1;
    }
    return 0;
}

pair<int, int> parsePosition(string pos){
    int row = pos[1] - '1';
    int col = pos[0] - 'a';
    return {row, col};
}

void startGame(){
    string s1,s2;
    bool isWhiteTurn = 1;

    while(1){
        cout << (isWhiteTurn ? "\nWhite's Move" : "\nBlack's Move") << endl;
        cout << "\nEnter your move (e.g., e2 e4): ";
        cin >> s1 >> s2;

        pair<int,int> from = parsePosition(s1);
        pair<int,int> to = parsePosition(s2);

        if(s1.length() != 2 || s2.length() != 2){
            cout << "Invalid input. Try again.\n";
            continue;
        }

        if(from.first < 0 || from.first > 7 || from.second < 0 || from.second > 7 ||
           to.first < 0 || to.first > 7 || to.second < 0 || to.second > 7){
            cout << "Invalid move. Try again.\n";
            continue;
        }
        if(board[from.first][from.second] == ' '){
            cout << "No piece at the source position. Try again.\n";
            continue;
        }
        if(makeMove(from, to, isWhiteTurn)) isWhiteTurn = !isWhiteTurn;
        else cout << "Invalid move. Try again.\n";
        display();
    }
}

int main(){
    cout << "\nWelcome to Chess!\n\n";
    display();
    startGame();
    cout << "Game Over!\n";
    return 0;
}

