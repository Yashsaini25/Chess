board = {
    #white pieces
    "a1": "R", "a2": "P",
    "b1": "N", "b2": "P",
    "c1": "B", "c2": "P",
    "d1": "Q", "d2": "P",
    "e1": "K", "e2": "P",
    "f1": "B", "f2": "P",
    "g1": "N", "g2": "P",
    "h1": "R", "h2": "P",

    #black pieces
    "a7": "p", "a8": "r",
    "b7": "p", "b8": "n",
    "c7": "p", "c8": "b",
    "d7": "p", "d8": "q",
    "e7": "p", "e8": "k",
    "f7": "p", "f8": "b",
    "g7": "p", "g8": "n",
    "h7": "p", "h8": "r",
}

def print_board():
    print("   a   b   c   d   e   f   g   h")
    print(" +-------------------------------+")
    for i in range(8, 0, -1):
        row = str(i) + "| "
        for j in "abcdefgh":
            row += board.get(j + str(i), ".") + " | "
        print(row)
        print(" +-------------------------------+")
    print("   a   b   c   d   e   f   g   h")

print_board()


