#include <iostream>
using namespace std;

vector<vector<char>> board = {
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '}
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
    int fy = from.second;
    int tx = to.first;
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
    int row = '8' - pos[1];
    int col = pos[0] - 'a';
    return {row, col};
}

string convertPosition(pair<int,int> pos){
    string s;
    s += (char)(pos.second + 'a');
    s += (char)(pos.first + '1');
    return s;
}

bool isInCheck(bool isWhiteTurn){
    pair<int,int> kingPos = {-1,-1};

    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(board[i][j] == (isWhiteTurn ? 'K' : 'k')){
                kingPos = {i,j};
                break;
            }
        }
    }

    if(kingPos.first == -1) return 0;

    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            char attacker = board[i][j];
            if(attacker == ' ') continue;
            if(islower(attacker) != isWhiteTurn) continue;
            if(isValidMove(attacker, {i,j}, kingPos)) return 1; 
        }
    }
    return 0;
}

bool isCheckmate(bool isWhiteTurn){
    if(!isInCheck(isWhiteTurn)) return false;

    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            char piece = board[i][j];
            
            if(piece == ' ' || islower(piece) != isWhiteTurn) continue;
            
            pair<int, int> from = {i,j};

            for(int x=0; x<8; x++){
                for(int y=0; y<8; y++){
                    pair<int, int> to = {x,y};
                    char captured = board[to.first][to.second];

                    if(makeMove(from, to, isWhiteTurn)){
                        if(!isInCheck(isWhiteTurn)){
                            board[from.first][from.second] = piece;
                            board[to.first][to.second] = captured;
                            return false;
                        }
                        board[from.first][from.second] = piece;
                        board[to.first][to.second] = captured;
                    }
                }
            }
        }
    }
    return true;
}

bool isStalemate(bool isWhiteTurn){
    if(isInCheck(isWhiteTurn)) return false;

    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            char piece = board[i][j];
            
            if(piece == ' ' || islower(piece) != isWhiteTurn) continue;
            
            pair<int, int> from = {i,j};

            for(int x=0; x<8; x++){
                for(int y=0; y<8; y++){
                    pair<int, int> to = {x,y};
                    char captured = board[to.first][to.second];

                    if(makeMove(from, to, isWhiteTurn)){
                        if(!isInCheck(isWhiteTurn)){
                            board[from.first][from.second] = piece;
                            board[to.first][to.second] = captured;
                            return false;
                        }
                        board[from.first][from.second] = piece;
                        board[to.first][to.second] = captured;
                    }
                }
            }
        }
    }
    return true;
}

int evaluateBoard(){
    int score = 0;

    int pawnTable[8][8] = {
        {0,0,0,0,0,0,0,0},
        {5,5,5,5,5,5,5,5},
        {1,1,2,3,3,2,1,1},
        {0,0,0,2,2,0,0,0},
        {0,0,0,-2,-2,0,0,0},
        {1,-1,-2,0,0,-2,-1,1},
        {1,2,2,-2,-2,2,2,1},
        {0,0,0,0,0,0,0,0}
    };

    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            char piece = board[i][j];
            int base = 0, bonus = 0;

            switch(piece){
                case 'P': base = 1; bonus = pawnTable[i][j]; break;
                case 'N': base = 3; break;
                case 'B': base = 3; break;
                case 'R': base = 5; break;
                case 'Q': base = 9; break;
                case 'K': base = 100; break;
                case 'p': base = -1; bonus = -pawnTable[7-i][j]; break;
                case 'n': base = -3; break;
                case 'b': base = -3; break;
                case 'r': base = -5; break;
                case 'q': base = -9; break;
                case 'k': base = -100; break;
            }
            score += base + bonus;
        }
    }
    return score;
}


int minimax(int depth, bool isMaximizing, int alpha, int beta){
    if(depth == 0 || isCheckmate(true) || isCheckmate(false) || isStalemate(true) || isStalemate(false)){
        return evaluateBoard();
    }

    if(isMaximizing){
        int maxVal = INT_MIN;
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                char piece = board[i][j];
                if(islower(piece)){
                    pair<int, int> from = {i,j};

                    for(int x=0; x<8; x++){
                        for(int y=0; y<8; y++){
                            pair<int, int> to = {x,y};
                            char captured = board[to.first][to.second];

                            if(makeMove(from, to, 0)){
                                if(isInCheck(false)){
                                    board[from.first][from.second] = piece;
                                    board[to.first][to.second] = captured;
                                    continue;
                                }

                                int moveValue = minimax(depth-1, false, alpha, beta);
                                board[from.first][from.second] = piece;
                                board[to.first][to.second] = captured;
                                maxVal = max(maxVal, moveValue);
                                alpha = max(alpha, moveValue);
                                if(beta <= alpha) return maxVal; 
                            }
                        }
                    }
                }
            }
        }
        return maxVal;
    }

    else{
        int minVal = INT_MAX;
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                char piece = board[i][j];
                if(isupper(piece)){
                    pair<int, int> from = {i,j};

                    for(int x=0; x<8; x++){
                        for(int y=0; y<8; y++){
                            pair<int, int> to = {x,y};
                            char captured = board[to.first][to.second];

                            if(makeMove(from, to, 1)){
                                if(isInCheck(true)){
                                    board[from.first][from.second] = piece;
                                    board[to.first][to.second] = captured;
                                    continue;
                                }

                                int moveValue = minimax(depth-1, true, alpha, beta);
                                board[from.first][from.second] = piece;
                                board[to.first][to.second] = captured;
                                minVal = min(minVal, moveValue);
                                beta = min(beta, moveValue);
                                if(beta <= alpha) return minVal;
                            }
                        }
                    }
                }
            }
        }
        return minVal;
    }
}

pair<pair<int, int>, pair<int, int>> findBestMove(){
    int bestValue = INT_MIN;
    pair<pair<int, int>, pair<int, int>> bestMove;

    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            char piece = board[i][j];
            if(islower(piece)){
                pair<int, int> from = {i,j};

                for(int x=0; x<8; x++){
                    for(int y=0; y<8; y++){
                        pair<int, int> to = {x,y};
                        char captured = board[to.first][to.second];

                        if(makeMove(from, to, 0)){
                            if(isInCheck(false)){
                                board[from.first][from.second] = piece;
                                board[to.first][to.second] = captured;
                                continue;
                            }
                            int moveValue = minimax(6, 0, INT_MIN, INT_MAX);
                            board[from.first][from.second] = piece;
                            board[to.first][to.second] = captured;
                            if(moveValue > bestValue){
                                bestValue = moveValue;
                                bestMove = {from, to};
                            }
                        }
                    }
                }
            }
        }
    }
    return bestMove;
}

bool handleStates(bool isWhiteTurn){
    if(isCheckmate(isWhiteTurn)){
        cout << (isWhiteTurn ? "AI wins!" : "You won!") << endl;
        return 1;
    }
    if(isStalemate(isWhiteTurn)){
        cout << "Stalemate! It's a draw." << endl;
        return 1;
    }
    if(isInCheck(isWhiteTurn)){
        cout << (isWhiteTurn ? "You is in check!" : "Ai is in check!") << endl;
    }
    return 0;
}

void startGame(){
    string s1,s2;
    bool isWhiteTurn = 1;
    pair<int,int> from, to;

    while(1){
        cout << "\nEnter your move (e.g., e2 e4): ";
        cin >> s1 >> s2;

        from = parsePosition(s1);
        to = parsePosition(s2);

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
        if(makeMove(from, to, isWhiteTurn)){
            isWhiteTurn = !isWhiteTurn;
            cout << "\nMove made from " << convertPosition(from) << " to " << convertPosition(to) << endl<<endl;
            bool gameOver = handleStates(isWhiteTurn);
            display();
            if(gameOver) return;
        } 
        else{
            cout << "Invalid move. Try again.\n";
            continue;
        }

        cout << "\nAI's turn...\n";
        pair<pair<int, int>, pair<int, int>> bestMove = findBestMove();
        from = bestMove.first;
        to = bestMove.second;
        char piece = board[from.first][from.second];
        char captured = board[to.first][to.second];
        makeMove(from, to, 0);
        cout << "\nAI moved " << " from " << convertPosition(from) << " to " << convertPosition(to) << endl<<endl;
        bool gameOver = handleStates(isWhiteTurn);
        isWhiteTurn = !isWhiteTurn;
        display();
        if(gameOver) return;
        cout << "\nYour turn...\n";
    }
}


string getFEN(bool whiteToMove) {
    string fen = "";

    for (int row = 0; row < 8; ++row) {  
        int emptyCount = 0;
        for (int col = 0; col < 8; ++col) {
            char piece = board[row][col];

            if (piece == '.' || piece == ' ') {
                ++emptyCount;
            } else {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += piece;
            }
        }
        if (emptyCount > 0) {
            fen += to_string(emptyCount);
        }
        if (row != 7) {
            fen += '/';
        }
    }

    fen += whiteToMove ? " w " : " b ";
    fen += "KQkq ";  
    fen += "- ";     
    fen += "0 1";

    return fen;
}


void loadBoardFromFEN(const string& fen) {
    int row = 0;  
    int col = 0;

    for (char c : fen) {
        if (c == ' ') break; 

        if (c == '/') {
            row++;     
            col = 0;   
        } else if (isdigit(c)) {
            col += c - '0'; 
        } else {
            board[row][col] = c; 
            col++;
        }
    }
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "{ \"valid\": false, \"error\": \"No move provided\" }";
        return 1;
    }

    std::string move = argv[1];
    std::string fen = argv[2];
    if (move.size() < 5 || move[2] != '-') {
        std::cout << "{ \"valid\": false, \"error\": \"Invalid move format\" }";
        return 1;
    }

    std::string s1 = move.substr(0, 2);
    std::string s2 = move.substr(3, 2);

    std::pair<int, int> from = parsePosition(s1);
    std::pair<int, int> to = parsePosition(s2);

    loadBoardFromFEN(fen); 

    bool isValid = makeMove(from, to, 1); 
    if (!isValid) {
        std::cout << "{ \"valid\": false }";
        return 0;
    }

    auto bestMove = findBestMove();
    makeMove(bestMove.first, bestMove.second, 0); 

    fen = getFEN(1); 

    std::cout << "{";
    std::cout << "\"valid\": true, ";
    std::cout << "\"fen\": \"" << fen << "\"";
    std::cout << "}";

    return 0;
}



