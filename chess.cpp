#include <iostream>
#include <vector>
using namespace std;

class Chess_Piece
{
private:
    char icon; // type of piece
    int file;  // column
    int rank;  // row
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

    char getIcon() { return icon; }
    char getRank() { return rank; }
    char getFile() { return file; }
    bool isWhite() { return is_white; }
    bool isCaptured() { return is_captured; }
    void pieceMoved() { is_moved = true; }
    void setCaptured() { 
        std::cout << "Piece captured:" << icon << std::endl;
        is_captured = true; 
    }
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
            icon = piece->getIcon();
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
            // pieces[8 + i].setCaptured();
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
    bool isWhiteTurn() { return is_white_turn; }

    void set_pieces()
    {
        for (int i = 0; i < 32; i++)
        {
            if (pieces[i].isCaptured())
            {
                continue;
            }
            int rank = pieces[i].getRank();
            int file = pieces[i].getFile();
            tiles[rank][file].piece = &pieces[i];
            tiles[rank][file].icon = pieces[i].getIcon();
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

        int target_tile[2] = {target_rank, target_file};
        pair<int, int> move_result = calculate_possible_moves(piece, rank, file, target_tile);

        if (move_result.first == false)
        {
            std::cout << "Invalid move." << std::endl;
            return;
        }else if (move_result.second == true)
        {
            std::cout << "Piece captured." << std::endl;
            tiles[target_tile[0]][target_tile[1]].piece->setCaptured();
            tiles[target_tile[0]][target_tile[1]].empty_tile();
        }
        
        // Move the piece
        tiles[rank][file].piece->pieceMoved();
        tiles[target_rank][target_file].set_tile(tiles[rank][file].piece);
        tiles[rank][file].empty_tile();

        is_white_turn = !is_white_turn;
        std::cout << "Last move: " << move << std::endl;
    }

    // Returns a pair of bools, the first bool is true if the move is valid, the second bool is true if a piece was captured
    pair<bool, bool> calculate_possible_moves(char piece, int rank, int file, int target_tile[2])
    {

        switch (piece)
        {
        case 'P':
            return calculate_pawn_moves(rank, file, target_tile);
            break;
        case 'R':
            return calculate_rook_moves(rank, file, target_tile);
            break;
        case 'N':
            return calculate_knight_moves(rank, file, target_tile);
            break;
        case 'B':
            return calculate_bishop_moves(rank, file, target_tile);
            break;
        // case 'Q':
        //     return calculate_queen_moves(rank, file);
        //     break;
        case 'K':
            return calculate_king_moves(rank, file, target_tile);
            break;
        default:
            return make_pair(false, false);
            break;
        }
    }

    pair<bool, bool> calculate_pawn_moves(int rank, int file, int target_tile[2])
    {
        vector<pair<int, int>> moves;
        bool captured_piece = false;

        // Add pawn move logic here
        if (is_white_turn)
        {
            if (target_tile[0] == rank - 1 &&
                (target_tile[1] == file - 1 || target_tile[1] == file + 1) &&
                tiles[target_tile[0]][target_tile[1]].icon != ' ')
            {
                captured_piece = true;
            }
            else if (rank == 6)
            {
                if (tiles[rank - 1][file].icon == ' ')
                {
                    // If in the starting position, the pawn can move 1 or 2 tiles
                    moves.push_back(make_pair(rank - 1, file));
                    if (tiles[rank - 2][file].icon == ' ')
                    {
                        moves.push_back(make_pair(rank - 2, file));
                    }
                }
            }
            else if (rank > 0)
            {
                // If not in the starting position, the pawn can only move 1 tile forward
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
            if (target_tile[0] == rank + 1 &&
                (target_tile[1] == file - 1 || target_tile[1] == file + 1) &&
                tiles[target_tile[0]][target_tile[1]].icon != ' ')
            {
                if (tiles[target_tile[0]][target_tile[1]].piece->isWhite() != is_white_turn)
                {
                    captured_piece = true;
                }
            }
            else if (rank == 1)
            {
                if (tiles[rank + 1][file].icon == ' ')
                {
                    // If in the starting position, the pawn can move 1 or 2 tiles
                    moves.push_back(make_pair(rank + 1, file));
                    if (tiles[rank + 2][file].icon == ' ')
                    {
                        moves.push_back(make_pair(rank + 2, file));
                    }
                }
            }
            else if (rank < 7)
            {
                // If not in the starting position, the pawn can only move 1 tile forward
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

        // Check if the move is valid
        bool is_move_valid = false;
        for (size_t i = 0; i < moves.size(); i++)
        {
            if (moves[i].first == target_tile[0] && moves[i].second == target_tile[1])
            {
                is_move_valid = true;
                break;
            }
        }

        return make_pair(is_move_valid, captured_piece);
    }

    pair<bool, bool> calculate_rook_moves(int rank, int file, int target_tile[2])
    {
        vector<pair<int, int>> moves;
        bool captured_piece = false;

        // Check if the target tile is in the same row or column
        if (rank == target_tile[0] || file == target_tile[1])
        {
            // Check if there are any pieces in between the rook and the target tile
            // Verticlaly
            if (rank == target_tile[0])
            {
                int start = min(file, target_tile[1]);
                int end = max(file, target_tile[1]);
                for (int i = start + 1; i < end; i++)
                {
                    if (tiles[rank][i].icon != ' ')
                    {
                        return make_pair(false, false);
                    }
                }
            }
            // Horizontally
            else if (file == target_tile[1])
            {
                int start = min(rank, target_tile[0]);
                int end = max(rank, target_tile[0]);
                for (int i = start + 1; i < end; i++)
                {
                    if (tiles[i][file].icon != ' ')
                    {
                        return make_pair(false, false);
                    }
                }
            }

            // Check if the target tile is occupied by an piece
            if (tiles[target_tile[0]][target_tile[1]].piece != nullptr)
            {
                // Check if the piece belongs to the opponent
                if (tiles[target_tile[0]][target_tile[1]].piece->isWhite() != is_white_turn)
                {
                    captured_piece = true;
                }
                else
                {
                    return make_pair(false, false);
                }
            }

            return make_pair(true, captured_piece);
        }
        else
        {
            return make_pair(false, false);
        }

        return make_pair(false, false);
    }

    pair<bool, bool> calculate_knight_moves(int rank, int file, int target_tile[2])
    {
        bool captured_piece = false;
        if((rank + 2 == target_tile[0] && (file + 1 || file - 1 == target_tile[1])) ||
        (rank + 1 == target_tile[0] && (file + 2 || file - 2 == target_tile[1])) ||
        (rank - 2 == target_tile[0] && (file + 1 || file - 1 == target_tile[1])) ||
        (rank - 1 == target_tile[0] && (file + 2 || file - 2 == target_tile[1]))){
            if(tiles[target_tile[0]][target_tile[1]].piece != nullptr)
            {
                if (tiles[target_tile[0]][target_tile[1]].piece->isWhite() != is_white_turn)
                {
                    captured_piece = true;
                }
                else
                {
                    return make_pair(false, false);
                }
            }
            return make_pair(true, captured_piece);
        }else{
            return make_pair(false, false);
        }

        return make_pair(false, false);
    }

    pair<bool, bool> calculate_bishop_moves(int rank, int file, int target_tile[2])
    {
        bool captured_piece = false;

        // Check if the target tile is diagonally aligned with the bishop
        if (abs(rank - target_tile[0]) == abs(file - target_tile[1]))
        {
            int row_dir = (target_tile[0] - rank) / abs(target_tile[0] - rank);
            int col_dir = (target_tile[1] - file) / abs(target_tile[1] - file);
            int i = rank + row_dir;
            int j = file + col_dir;

            // Check if there are any pieces in between the bishop and the target tile
            while (i != target_tile[0] && j != target_tile[1])
            {
                if (tiles[i][j].icon != ' ')
                {
                    return make_pair(false, false);
                }
                i += row_dir;
                j += col_dir;
            }
            // Check if the target tile is occupied by an piece to capture
            if(tiles[target_tile[0]][target_tile[1]].piece != nullptr)
            {
                if (tiles[target_tile[0]][target_tile[1]].piece->isWhite() != is_white_turn)
                {
                    captured_piece = true;
                }
                else
                {
                    return make_pair(false, false);
                }
            }
            return make_pair(true, captured_piece);
        }
        return make_pair(false, false);
    }

    // pair<bool, bool> calculate_queen_moves(int rank, int file)
    // {
    //     vector<pair<int, int>> moves;
    //     // Add queen move logic here
    //     return moves;
    // }

    pair<bool, bool> calculate_king_moves(int rank, int file, int target_tile[2])
    {
        bool captured_piece = false;

        // Check if the target tile is within 1 tile of the king
        if (abs(rank - target_tile[0]) <= 1 && abs(file - target_tile[1]) <= 1)
        {
            // Check if the target tile has a capturable piece
            if(tiles[target_tile[0]][target_tile[1]].piece != nullptr)
            {
                if (tiles[target_tile[0]][target_tile[1]].piece->isWhite() != is_white_turn)
                {
                    captured_piece = true;
                }
                else
                {
                    return make_pair(false, false);
                }
            }
            return make_pair(true, captured_piece);
        }
        return make_pair(false, false);
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
        if(board.isWhiteTurn())
        {
            std::cout << "White's turn." << std::endl;
        }else
        {
            std::cout << "Black's turn." << std::endl;
        }
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
