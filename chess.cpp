#include <iostream>
#include <vector>
using namespace std;

class Chess_Piece
{
private:
    char piece; // type of piece
    int file;   // column
    int rank;   // row
    bool is_white;
    bool is_captured = false;

public:
    Chess_Piece() : piece(' '), file(' '), rank(' '), is_white(false), is_captured(false) {}
    Chess_Piece(char piece, char file, char rank, bool is_white)
    {
        this->piece = piece;
        this->file = file - 'a';
        this->rank = 7 - (rank - '1');
        this->is_white = is_white;
    };
    ~Chess_Piece() {

    };

    char getPiece() { return piece; }
    char getRank() { return rank; }
    char getFile() { return file; }
};

class Chess_Board
{
private:
    Chess_Piece pieces[32];
    char tiles[8][8];
    bool is_white_turn = true;

public:
    Chess_Board()
    {
        // Initialize the board
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                tiles[i][j] = ' ';
            }
        }

        // Initialize the pieces
        pieces[0] = Chess_Piece('R', 'a', '1', true);
        pieces[1] = Chess_Piece('N', 'b', '1', true);
        pieces[2] = Chess_Piece('B', 'c', '1', true);
        pieces[3] = Chess_Piece('Q', 'd', '1', true);
        pieces[4] = Chess_Piece('K', 'e', '1', true);
        pieces[5] = Chess_Piece('B', 'f', '1', true);
        pieces[6] = Chess_Piece('N', 'g', '1', true);
        pieces[7] = Chess_Piece('R', 'h', '1', true);
        for (int i = 0; i < 8; i++)
        {
            pieces[8 + i] = Chess_Piece('P', 'a' + i, '2', true);
        }

        pieces[16] = Chess_Piece('R', 'a', '8', false);
        pieces[17] = Chess_Piece('N', 'b', '8', false);
        pieces[18] = Chess_Piece('B', 'c', '8', false);
        pieces[19] = Chess_Piece('Q', 'd', '8', false);
        pieces[20] = Chess_Piece('K', 'e', '8', false);
        pieces[21] = Chess_Piece('B', 'f', '8', false);
        pieces[22] = Chess_Piece('N', 'g', '8', false);
        pieces[23] = Chess_Piece('R', 'h', '8', false);
        for (int i = 0; i < 8; i++)
        {
            pieces[24 + i] = Chess_Piece('P', 'a' + i, '7', false);
        }

        set_pieces();
    };

    ~Chess_Board() {

    };

    void set_pieces()
    {
        for (int i = 0; i < 32; i++)
        {
            int rank = pieces[i].getRank();
            int file = pieces[i].getFile();
            tiles[rank][file] = pieces[i].getPiece();
        }
    }

    void display_board()
    {
        std::cout << "   a b c d e f g h" << std::endl;
        std::cout << "   ---------------" << std::endl;
        for (int i = 0; i < 8; i++)
        {
            std::cout << 8 - i << "| ";
            for (int j = 0; j < 8; j++)
            {
                std::cout << tiles[i][j] << " ";
            }
            std::cout << "|" << 8 - i << " ";
            std::cout << std::endl;
        }
        std::cout << "   ---------------" << std::endl;
        std::cout << "   a b c d e f g h" << std::endl;
        std::cout << std::endl;
    }

    void move_piece(string &move)
    {
        char piece = move[0];
        int file = move[1] - 'a'; // column
        int rank = 7 - (move[2] - '1'); // row
        int target_file = move[3] - 'a';
        int target_rank = 7 - (move[4] - '1');

        //TODO Check if it is the player's turn
        

        // Check if move is out of bounds
        if (file < 0 || file > 7 ||
            rank < 0 || rank > 7 ||
            target_file < 0 || target_file > 7 ||
            target_rank < 0 || target_rank > 7)
        {
            std::cout << "Move out of bounds" << std::endl;
            return;
        }

        // Check if piece exists at the initial location
        if (tiles[rank][file] == ' ')
        {
            std::cout << "Empty tile." << std::endl;
            return;
        }

        if (tiles[rank][file] != piece)
        {
            std::cout << "Wrong piece." << std::endl;
            return;
        }

        tiles[rank][file] = ' ';
        tiles[target_rank][target_file] = piece;
        std::cout << "Last move: " << move << std::endl;

        // calculate_possible_moves(Chess_Piece piece, char[] board);
    }
};

void help_message()
{
    std::cout << "This is a chess game." << std::endl;
    std::cout << "Enter a move in algebraic notation." << std::endl;
    std::cout << "Type 'exit' to quit." << std::endl;
}

int main()
{
    std::cout << "Welcome to Chess!" << std::endl;
    Chess_Board board;

    while (true)
    {
        board.display_board();
        std::cout << "Enter a move: ";
        string move;
        std::cin >> move;

        if (move == "exit")
        {
            break;
        }
        else if (move == "help")
        {
            help_message();
            continue;
        }
        // else if (move.length() == 2)
        // {
        //     show_possible_moves(move);
        //     continue;
        // }
        else if (move.length() == 5)
        {
            board.move_piece(move);
            continue;
        }
        else
        {
            std::cout << "Invalid move." << std::endl;
            continue;
        }

        std::cout << "You entered: " << move << std::endl;
    }

    return 0;
}

// void show_possible_moves(string &move)
// {
// }

// vector<char> calculate_possible_moves(Chess_Piece piece, char[] board)
// {
//     vector<char> possible_moves;
//     switch (piece.piece)
//     {
//     case 'P':
//         possible_moves = calculate_pawn_moves(piece, board);
//         break;
//     case 'R':
//         possible_moves = calculate_rook_moves(piece, board);
//         break;
//     case 'N':
//         possible_moves = calculate_knight_moves(piece, board);
//         break;
//     case 'B':
//         possible_moves = calculate_bishop_moves(piece, board);
//         break;
//     case 'Q':
//         possible_moves = calculate_queen_moves(piece, board);
//         break;
//     case 'K':
//         possible_moves = calculate_king_moves(piece, board);
//         break;
//     }
//     return possible_moves;
// }
