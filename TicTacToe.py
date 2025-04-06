import math

# Initialize the empty board
board = [" " for _ in range(9)]

# Display the board
def print_board():
    for i in range(3):
        row = board[i*3:(i+1)*3]
        print("|".join(row))
        if i < 2:
            print("-" * 5)

# Check for winner
def check_winner(b, player):
    win_patterns = [
        [0, 1, 2], [3, 4, 5], [6, 7, 8],  # rows
        [0, 3, 6], [1, 4, 7], [2, 5, 8],  # cols
        [0, 4, 8], [2, 4, 6]              # diagonals
    ]
    for pattern in win_patterns:
        if all(b[i] == player for i in pattern):
            return True
    return False

# Check for draw
def is_draw(b):
    return " " not in b

# Evaluation function
def evaluate(b):
    if check_winner(b, "O"):
        return 1   # AI wins
    elif check_winner(b, "X"):
        return -1  # Human wins
    else:
        return 0   # Draw

# Minimax algorithm
def minimax(b, depth, is_maximizing):
    score = evaluate(b)

    if score != 0 or is_draw(b):
        return score

    if is_maximizing:
        best = -math.inf
        for i in range(9):
            if b[i] == " ":
                b[i] = "O"
                best = max(best, minimax(b, depth + 1, False))
                b[i] = " "
        return best
    else:
        best = math.inf
        for i in range(9):
            if b[i] == " ":
                b[i] = "X"
                best = min(best, minimax(b, depth + 1, True))
                b[i] = " "
        return best

# Find best move for AI
def find_best_move():
    best_val = -math.inf
    best_move = -1

    for i in range(9):
        if board[i] == " ":
            board[i] = "O"
            move_val = minimax(board, 0, False)
            board[i] = " "

            if move_val > best_val:
                best_val = move_val
                best_move = i

    return best_move

# Main game loop
def play_game():
    print("Welcome to Tic Tac Toe! You are 'X', AI is 'O'")
    print_board()

    while True:
        # Human move
        move = int(input("Enter your move (0-8): "))
        if board[move] != " ":
            print("Invalid move. Try again.")
            continue
        board[move] = "X"
        print_board()

        if check_winner(board, "X"):
            print("You win!")
            break
        if is_draw(board):
            print("It's a draw!")
            break

        # AI move
        ai_move = find_best_move()
        board[ai_move] = "O"
        print("AI played:")
        print_board()

        if check_winner(board, "O"):
            print("AI wins!")
            break
        if is_draw(board):
            print("It's a draw!")
            break

play_game()
