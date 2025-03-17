#include <iostream>
#include <vector>
using namespace std;

class Chess_Piece
{
private:
    char icon; // type of piece
    int file;   // column
    int rank;   // row
    bool is_white = true;
    bool is_captured = false;
    bool is_moved = false;

public:
    Chess_Piece() : icon(' '), file(' '), rank(' '), is_white(false), is_captured(false) {}
    Chess_Piece(char piece, char file, char rank, bool is_white)
    {
        this->icon = piece;
        this->file = file - 'a';
        this->rank = 7 - (rank - '1');
        this->is_white = is_white;
    };
    ~Chess_Piece() {

    };

    char getPiece() { return icon; }
    char getRank() { return rank; }
    char getFile() { return file; }
};

class Chess_Board
{
private:
    struct tile
    {
        char icon = ' ';
        Chess_Piece *piece;

        tile(char piece_icon, Chess_Piece *piece)
        {
            this->icon = piece_icon;
            this->piece = piece;
        }
        tile() : icon(' '), piece(nullptr) {}
        void empty_tile()
        {
            icon = ' ';
            piece = nullptr;
        }
        char set_tile(Chess_Piece *piece)
        {
            this->piece = piece;
            icon = piece->getPiece();
            return icon;

        }
    };
    Chess_Piece pieces[32];
    tile tiles[8][8];
    bool is_white_turn = true;
    

public:
    Chess_Board()
    {
        // Initialize the board
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                tiles[i][j] = tile(' ', nullptr);
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
            tiles[rank][file].piece = &pieces[i];
            tiles[rank][file].icon = pieces[i].getPiece();
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
                std::cout << tiles[i][j].icon << " ";
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
        int file = move[1] - 'a';       // column
        int rank = 7 - (move[2] - '1'); // row
        int target_file = move[3] - 'a';
        int target_rank = 7 - (move[4] - '1');

        // TODO Check if it is the player's turn

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
        if (tiles[rank][file].icon == ' ')
        {
            std::cout << "No piece at this position" << std::endl;
            return;
        }

        if (tiles[rank][file].icon != piece)
        {
            std::cout << "Wrong piece." << std::endl;
            return;
        }

        vector<pair<int, int>> validTiles = calculate_possible_moves(piece, rank, file);
        
        // Check if the move is valid
        bool is_move_valid = false;
        for (int i = 0; i < validTiles.size(); i++)
        {
            if (validTiles[i].first == target_rank && validTiles[i].second == target_file)
            {
                is_move_valid = true;
                break;
            }
        }
        if (!is_move_valid)
        {
            std::cout << "Invalid move." << std::endl;
            return;
        }
        
        // Move the piece
        tiles[target_rank][target_file].set_tile(tiles[rank][file].piece);
        tiles[rank][file].empty_tile();
        is_white_turn = !is_white_turn;
        std::cout << "Last move: " << move << std::endl;
    }

    vector<pair<int, int>> calculate_possible_moves(char piece, int rank, int file)
    {
        vector<pair<int, int>> validTiles;
        switch (piece)
        {
        case 'P':
            validTiles = calculate_pawn_moves(rank, file);
            break;
        case 'R':
            validTiles = calculate_rook_moves(rank, file);
            break;
        case 'N':
            validTiles = calculate_knight_moves(rank, file);
            break;
        case 'B':
            validTiles = calculate_bishop_moves(rank, file);
            break;
        case 'Q':
            validTiles = calculate_queen_moves(rank, file);
            break;
        case 'K':
            validTiles = calculate_king_moves(rank, file);
            break;
        default:
            break;
        }
        return validTiles;
    }

    vector<pair<int, int>> calculate_pawn_moves(int rank, int file)
    {
        vector<pair<int, int>> moves;
        // Add pawn move logic here
        if (is_white_turn)
        {
            if (rank == 6)
            {
                if (tiles[rank - 1][file].icon == ' ')
                {
                    //If in the starting position, the pawn can move 1 or 2 tiles
                    moves.push_back(make_pair(rank - 1, file));
                    if (tiles[rank - 2][file].icon == ' ')
                    {
                        moves.push_back(make_pair(rank - 2, file));
                    }
                }
            }
            else if (rank > 0)
            {
                //If not in the starting position, the pawn can only move 1 tile forward
                if (tiles[rank - 1][file].icon == ' ')
                {
                    moves.push_back(make_pair(rank - 1, file));
                }
            }
            // TODO check the other pawn moved last turn and if it moved 2 tiles, the current pawn can capture it (en passant)
                // possibly add a last_move variable to the class to store the last move
            if (rank > 0 && file > 0 && tiles[rank - 1][file - 1].icon != ' ' && !is_white_turn)
            {
                // If there is an enemy piece diagonally to the left, the pawn can capture it (en passant)
                moves.push_back(make_pair(rank - 1, file - 1));
            }
        }
        else
        {
            if (rank == 1)
            {
                if (tiles[rank + 1][file].icon == ' ')
                {
                    //If in the starting position, the pawn can move 1 or 2 tiles
                    moves.push_back(make_pair(rank + 1, file));
                    if (tiles[rank + 2][file].icon == ' ')
                    {
                        moves.push_back(make_pair(rank + 2, file));
                    }
                }
            }
            else if (rank < 7)
            {
                //If not in the starting position, the pawn can only move 1 tile forward
                if (tiles[rank + 1][file].icon == ' ')
                {
                    moves.push_back(make_pair(rank + 1, file));
                }
            }
            // TODO check the other pawn moved last turn and if it moved 2 tiles, the current pawn can capture it (en passant)
                // possibly add a last_move variable to the class to store the last move
            if (rank < 7 && file < 7 && tiles[rank + 1][file + 1].icon != ' ' && is_white_turn)
            {
                // If there is an enemy piece diagonally to the right, the pawn can capture it (en passant)
                moves.push_back(make_pair(rank + 1, file + 1));
            }
        }
        
        return moves;
    }

    vector<pair<int, int>> calculate_rook_moves(int rank, int file)
    {
        vector<pair<int, int>> moves;
        // Add rook move logic here
        return moves;
    }

    vector<pair<int, int>> calculate_knight_moves(int rank, int file)
    {
        vector<pair<int, int>> moves;
        // Add knight move logic here
        return moves;
    }

    vector<pair<int, int>> calculate_bishop_moves(int rank, int file)
    {
        vector<pair<int, int>> moves;
        // Add bishop move logic here
        return moves;
    }

    vector<pair<int, int>> calculate_queen_moves(int rank, int file)
    {
        vector<pair<int, int>> moves;
        // Add queen move logic here
        return moves;
    }

    vector<pair<int, int>> calculate_king_moves(int rank, int file)
    {
        vector<pair<int, int>> moves;
        // Add king move logic here
        return moves;
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
