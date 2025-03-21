#include <iostream>
#include <vector>
#include <regex>
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
    vector<pair<int, int>> possible_moves; // tiles that the piece could move to
    vector<Chess_Piece *> blocking_pieces; // pieces that are blocking the possible path(s)

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
    vector<pair<int, int>> getPossibleMoves() { return possible_moves; }
    bool isWhite() { return is_white; }
    bool isCaptured() { return is_captured; }
    bool isMoved() { return is_moved; }
    void pieceMoved(int rank, int file)
    {
        is_moved = true;
        this->rank = rank;
        this->file = file;
    }
    void setCaptured()
    {
        std::cout << "Piece captured:" << icon << std::endl;
        is_captured = true;
    }
    void addPossibleMoves(vector<pair<int, int>> moves) { possible_moves = moves; }
    void promote(char piece) { icon = piece; }
};

class Chess_Board
{
private:
    struct tile
    {
        char icon = ' ';
        Chess_Piece *piece;
        bool highlight = false;

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
        void setHighlight(bool highlight)
        {
            this->highlight = highlight;
        }
    };
    Chess_Piece pieces[32];
    tile tiles[8][8];
    bool is_white_turn = true;
    Chess_Piece *last_moved = nullptr;

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

        for (int i = 0; i < 32; i++)
        {
            pieces[i].setCaptured();
        }

        pieces[5] = Chess_Piece('P', 'f', '8', true);
        pieces[23] = Chess_Piece('P', 'h', '1', false);
        set_pieces();
    };

    ~Chess_Board() {

    };
    bool isWhiteTurn() { return is_white_turn; }

    void set_pieces()
    {
        for (int i = 0; i < 32; i++)
        {
            if (!pieces[i].isCaptured())
            {
                int rank = pieces[i].getRank();
                int file = pieces[i].getFile();
                tiles[rank][file].piece = &pieces[i];
                tiles[rank][file].icon = pieces[i].getIcon();
            }
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
                if (tiles[i][j].highlight)
                {
                    if (tiles[i][j].piece != nullptr)
                    {
                        // Indicates a piece that can be captured
                        std::cout << "X ";
                    }
                    else
                    {
                        // Indicates an empty tile that can be moved to
                        std::cout << "* ";
                    }
                }
                else
                {
                    std::cout << tiles[i][j].icon << " ";
                }
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
        pair<int, int> move_result = movement_logic(piece, rank, file, target_tile);

        if (move_result.first == false)
        {
            std::cout << "Invalid move." << std::endl;
            return;
        }
        else if (move_result.second == true)
        {
            std::cout << "Piece captured." << std::endl;
            tiles[target_tile[0]][target_tile[1]].piece->setCaptured();
            tiles[target_tile[0]][target_tile[1]].empty_tile();
        }

        // Move the piece
        tiles[rank][file].piece->pieceMoved(target_rank, target_file);
        tiles[target_rank][target_file].set_tile(tiles[rank][file].piece);
        tiles[rank][file].empty_tile();

        is_white_turn = !is_white_turn;
        last_moved = tiles[target_rank][target_file].piece;
        std::cout << "Last move: " << move << std::endl;
    }

    // Returns a pair of bools, the first bool is true if the move is valid, the second bool is true if a piece was captured
    pair<bool, bool> movement_logic(char piece, int rank, int file, int target_tile[2])
    {

        switch (piece)
        {
        case 'P':
            return pawn_logic(rank, file, target_tile);
            break;
        case 'R':
            return rook_logic(rank, file, target_tile);
            break;
        case 'N':
            return knight_logic(rank, file, target_tile);
            break;
        case 'B':
            return bishop_logic(rank, file, target_tile);
            break;
        case 'Q':
            return queen_logic(rank, file, target_tile);
            break;
        case 'K':
            return king_logic(rank, file, target_tile);
            break;
        default:
            return make_pair(false, false);
            break;
        }
    }

    pair<bool, bool> pawn_logic(int rank, int file, int target_tile[2])
    {
        vector<pair<int, int>> possibleMoves;
        bool captured_piece = false;
        // TODO pawn promotion

        if (is_white_turn)
        {
            // Check if the target tile is diagonally aligned with the pawn
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
                    possibleMoves.push_back(make_pair(rank - 1, file));
                    if (tiles[rank - 2][file].icon == ' ')
                    {
                        possibleMoves.push_back(make_pair(rank - 2, file));
                    }
                }
            }
            else if (rank > 0)
            {
                // If not in the starting position, the pawn can only move 1 tile forward
                if (tiles[rank - 1][file].icon == ' ')
                {
                    possibleMoves.push_back(make_pair(rank - 1, file));
                }
            }

            // Check the other pawn moved last turn and if it moved 2 tiles, the current pawn can capture it (en passant)
            tile *en_passant_target = &tiles[target_tile[0] + 1][target_tile[1]];
            if (rank == 3 &&
                en_passant_target->piece == last_moved &&
                en_passant_target->icon == 'P')
            {
                possibleMoves.push_back(make_pair(rank - 1, file - 1));
                en_passant_target->piece->setCaptured();
                en_passant_target->empty_tile();

                return make_pair(true, false); // false because the piece was captured by en passant
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
                    possibleMoves.push_back(make_pair(rank + 1, file));
                    if (tiles[rank + 2][file].icon == ' ')
                    {
                        possibleMoves.push_back(make_pair(rank + 2, file));
                    }
                }
            }
            else if (rank < 7)
            {
                // If not in the starting position, the pawn can only move 1 tile forward
                if (tiles[rank + 1][file].icon == ' ')
                {
                    possibleMoves.push_back(make_pair(rank + 1, file));
                }
            }
            // Check the other pawn moved last turn and if it moved 2 tiles, the current pawn can capture it (en passant)
            tile *en_passant_target = &tiles[target_tile[0] - 1][target_tile[1]];
            if (rank == 4 &&
                en_passant_target->piece == last_moved &&
                en_passant_target->icon == 'P')
            {
                possibleMoves.push_back(make_pair(rank - 1, file - 1));
                std::cout << "Piece captured." << std::endl;
                en_passant_target->piece->setCaptured();
                en_passant_target->empty_tile();

                return make_pair(true, false); // false because the piece was captured by en passant
            }
        }

        // Check if the move is valid
        bool is_move_valid = false;
        for (size_t i = 0; i < possibleMoves.size(); i++)
        {
            if (possibleMoves[i].first == target_tile[0] && possibleMoves[i].second == target_tile[1])
            {
                is_move_valid = true;
                break;
            }
        }
        return make_pair(is_move_valid, captured_piece);
    }

    pair<bool, bool> rook_logic(int rank, int file, int target_tile[2])
    {
        vector<pair<int, int>> possibleMoves;
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

    pair<bool, bool> knight_logic(int rank, int file, int target_tile[2])
    {
        bool captured_piece = false;
        if ((rank + 2 == target_tile[0] && (file + 1 || file - 1 == target_tile[1])) ||
            (rank + 1 == target_tile[0] && (file + 2 || file - 2 == target_tile[1])) ||
            (rank - 2 == target_tile[0] && (file + 1 || file - 1 == target_tile[1])) ||
            (rank - 1 == target_tile[0] && (file + 2 || file - 2 == target_tile[1])))
        {
            if (tiles[target_tile[0]][target_tile[1]].piece != nullptr)
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
        else
        {
            return make_pair(false, false);
        }

        return make_pair(false, false);
    }

    pair<bool, bool> bishop_logic(int rank, int file, int target_tile[2])
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
            if (tiles[target_tile[0]][target_tile[1]].piece != nullptr)
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

    pair<bool, bool> queen_logic(int rank, int file, int target_tile[2])
    {
        // The queen's movement pattern is a combination of the rook and bishop movement patterns
        pair<bool, bool> rook_moves = rook_logic(rank, file, target_tile);
        pair<bool, bool> bishop_moves = bishop_logic(rank, file, target_tile);

        if (rook_moves.first)
        {
            return rook_moves;
        }
        else if (bishop_moves.first)
        {
            return bishop_moves;
        }
        else
        {
            return make_pair(false, false);
        }
    }

    pair<bool, bool> king_logic(int rank, int file, int target_tile[2])
    {
        bool captured_piece = false;

        // Check if the target tile is within 1 tile of the king
        if (abs(rank - target_tile[0]) <= 1 && abs(file - target_tile[1]) <= 1)
        {
            // Check if the target tile has a capturable piece
            if (tiles[target_tile[0]][target_tile[1]].piece != nullptr)
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

    pair<bool, bool> castling_logic()
    {
        // Check if the king and rook have been moved already
        if (tiles[0][4].piece->isMoved() || tiles[0][7].piece->isMoved())
        {
            return make_pair(false, false);
        }
        // Check if the king is in check

        // Check if the king will move through check
        // Check if the king will move into check
        // Check if the king will move out of check
        // Check if the rook will move through check
        // Check if the rook will move into check
        // Check if the rook will move out of check
        return make_pair(false, false);
    }

    void promote_pawn(string &move)
    {

        int file = move[0] - 'a';       // column
        int rank = 7 - (move[1] - '1'); // row
        char promo_piece = move[2];
        tile *t = &tiles[rank][file];

        // Check if the pawn has reached the end of the board
        if (rank != 0 && rank != 7)
        {
            std::cout << "Invalid move." << std::endl;
            return;
        }

        if ((rank == 7 && !t->piece->isWhite()) ||
            (rank == 0 && t->piece->isWhite()))
        {
            std::cout << "Not your pawn to promote." << std::endl;
            return;
        }

        // Check if the pawn is at the correct position to be promoted
        if (t->icon != 'P')
        {
            std::cout << "No pawn present for promotion" << std::endl;
            return;
        }

        t->piece->promote(promo_piece);
        t->icon = promo_piece;
        is_white_turn = !is_white_turn;
    }

    pair<bool, bool> check_logic()
    {
        // Check if the king is in check
        // Check if the king can move out of check
        // Check if the attacking piece can be captured
        return make_pair(false, false);
    }

    void show_possible_moves(string &move)
    {
        int rank = 7 - (move[1] - '1'); // row
        int file = move[0] - 'a';       // column
        char piece = tiles[rank][file].icon;

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

        calculate_possible_moves(rank, file, piece);
        vector<pair<int, int>> possible_moves = tiles[rank][file].piece->getPossibleMoves();

        for (const auto &move : possible_moves)
        {
            std::cout << char(file + 'a') << 8 - rank << " -> " << char(move.second + 'a') << 8 - move.first << std::endl;
        }

        // Display the board with the possible moves highlighted
        for (size_t i = 0; i < possible_moves.size(); i++)
        {
            int rank = possible_moves[i].first;
            int file = possible_moves[i].second;
            tiles[rank][file].setHighlight(true);
        }
        display_board();
        for (size_t i = 0; i < possible_moves.size(); i++)
        {
            int rank = possible_moves[i].first;
            int file = possible_moves[i].second;
            tiles[rank][file].setHighlight(false);
        }
    }

    void calculate_possible_moves(int rank, int file, char piece)
    {
        switch (piece)
        {
        case 'P':
            calculate_pawn_moves(rank, file);
            break;
        case 'R':
            calculate_rook_moves(rank, file);
            break;
        case 'N':
            calculate_knight_moves(rank, file);
            break;
        case 'B':
            calculate_bishop_moves(rank, file);
            break;
        case 'Q':
            calculate_queen_moves(rank, file);
            break;
        case 'K':
            calculate_king_moves(rank, file);
            break;
        default:
            break;
        }
    }

    void calculate_pawn_moves(int rank, int file)
    {
        vector<pair<int, int>> possibleMoves;
        Chess_Piece *pawn = tiles[rank][file].piece;

        if (pawn->isWhite())
        {
            if (rank == 6)
            {
                if (tiles[rank - 1][file].icon == ' ')
                {
                    // If in the starting position, the pawn can move 1 or 2 tiles
                    possibleMoves.push_back(make_pair(rank - 1, file));
                    if (tiles[rank - 2][file].icon == ' ')
                    {
                        possibleMoves.push_back(make_pair(rank - 2, file));
                    }
                }
            }
            // If not in the starting position, the pawn can only move 1 tile forward
            else if (rank > 0)
            {
                if (tiles[rank - 1][file].icon == ' ')
                {
                    possibleMoves.push_back(make_pair(rank - 1, file));
                }
            }

            // Capturable pieces
            if (tiles[rank - 1][file - 1].icon != ' ' && !is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank - 1, file - 1));
            }
            if (tiles[rank - 1][file + 1].icon != ' ' && !is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank - 1, file + 1));
            }

            // TODO en passant - check if the other pawn moved last turn and if it moved 2 tiles, the current pawn can capture it
            if (rank == 3)
            {
                if (tiles[rank][file - 1].icon == 'P' && tiles[rank][file - 1].piece->isWhite() != is_white_turn && tiles[rank][file - 1].piece == last_moved)
                {
                    possibleMoves.push_back(make_pair(rank - 1, file - 1));
                }
                {
                    possibleMoves.push_back(make_pair(rank - 1, file - 1));
                }
                if (tiles[rank][file + 1].icon == 'P' && tiles[rank][file + 1].piece->isWhite() != is_white_turn && tiles[rank][file + 1].piece == last_moved)
                {
                    possibleMoves.push_back(make_pair(rank - 1, file + 1));
                }
            }
        }
        else
        {
            if (rank == 1)
            {
                if (tiles[rank + 1][file].icon == ' ')
                {
                    // If in the starting position, the pawn can move 1 or 2 tiles
                    possibleMoves.push_back(make_pair(rank + 1, file));
                    if (tiles[rank + 2][file].icon == ' ')
                    {
                        possibleMoves.push_back(make_pair(rank + 2, file));
                    }
                }
            }
            else if (rank < 7)
            {
                // If not in the starting position, the pawn can only move 1 tile forward
                if (tiles[rank + 1][file].icon == ' ')
                {
                    possibleMoves.push_back(make_pair(rank + 1, file));
                }
            }
            if (tiles[rank + 1][file - 1].icon != ' ' && !is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank + 1, file - 1));
            }
            if (tiles[rank + 1][file + 1].icon != ' ' && !is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank + 1, file + 1));
            }
            // en passant - check if the other pawn moved last turn and if it moved 2 tiles, the current pawn can capture it
            if (rank == 4)
            {
                if (tiles[rank][file - 1].icon == 'P' && tiles[rank][file - 1].piece->isWhite() != is_white_turn && tiles[rank][file - 1].piece == last_moved)
                {
                    possibleMoves.push_back(make_pair(rank + 1, file - 1));
                }
                if (tiles[rank][file + 1].icon == 'P' && tiles[rank][file + 1].piece->isWhite() != is_white_turn && tiles[rank][file + 1].piece == last_moved)
                {
                    possibleMoves.push_back(make_pair(rank + 1, file + 1));
                }
            }
        }

        tiles[rank][file].piece->addPossibleMoves(possibleMoves);
    }

    vector<pair<int, int>> calculate_rook_moves(int rank, int file)
    {
        vector<pair<int, int>> possibleMoves;
        Chess_Piece *rook = tiles[rank][file].piece;

        for (int i = rank + 1; i < 8; i++)
        {
            if (tiles[i][file].icon == ' ')
            {
                possibleMoves.push_back(make_pair(i, file));
            }
            else
            {
                if (tiles[i][file].piece->isWhite() != is_white_turn)
                {
                    possibleMoves.push_back(make_pair(i, file));
                }
                break;
            }
        }
        for (int i = rank - 1; i >= 0; i--)
        {
            if (tiles[i][file].icon == ' ')
            {
                possibleMoves.push_back(make_pair(i, file));
            }
            else
            {
                if (tiles[i][file].piece->isWhite() != is_white_turn)
                {
                    possibleMoves.push_back(make_pair(i, file));
                }
                break;
            }
        }
        for (int i = file + 1; i < 8; i++)
        {
            if (tiles[rank][i].icon == ' ')
            {
                possibleMoves.push_back(make_pair(rank, i));
            }
            else
            {
                if (tiles[rank][i].piece->isWhite() != is_white_turn)
                {
                    possibleMoves.push_back(make_pair(rank, i));
                }
                break;
            }
        }
        for (int i = file - 1; i >= 0; i--)
        {
            if (tiles[rank][i].icon == ' ')
            {
                possibleMoves.push_back(make_pair(rank, i));
            }
            else
            {
                if (tiles[rank][i].piece->isWhite() != is_white_turn)
                {
                    possibleMoves.push_back(make_pair(rank, i));
                }
                break;
            }
        }

        rook->addPossibleMoves(possibleMoves);
        return possibleMoves;
    }

    void calculate_knight_moves(int rank, int file)
    {
        vector<pair<int, int>> possibleMoves;
        Chess_Piece *knight = tiles[rank][file].piece;

        if (rank + 2 < 8 && file + 1 < 8)
        {
            if (tiles[rank + 2][file + 1].icon == ' ' || tiles[rank + 2][file + 1].piece->isWhite() != is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank + 2, file + 1));
            }
        }
        if (rank + 1 < 8 && file + 2 < 8)
        {
            if (tiles[rank + 1][file + 2].icon == ' ' || tiles[rank + 1][file + 2].piece->isWhite() != is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank + 1, file + 2));
            }
        }
        if (rank - 2 >= 0 && file + 1 < 8)
        {
            if (tiles[rank - 2][file + 1].icon == ' ' || tiles[rank - 2][file + 1].piece->isWhite() != is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank - 2, file + 1));
            }
        }
        if (rank - 1 >= 0 && file + 2 < 8)
        {
            if (tiles[rank - 1][file + 2].icon == ' ' || tiles[rank - 1][file + 2].piece->isWhite() != is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank - 1, file + 2));
            }
        }
        if (rank + 2 < 8 && file - 1 >= 0)
        {
            if (tiles[rank + 2][file - 1].icon == ' ' || tiles[rank + 2][file - 1].piece->isWhite() != is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank + 2, file - 1));
            }
        }
        if (rank + 1 < 8 && file - 2 >= 0)
        {
            if (tiles[rank + 1][file - 2].icon == ' ' || tiles[rank + 1][file - 2].piece->isWhite() != is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank + 1, file - 2));
            }
        }
        if (rank - 2 >= 0 && file - 1 >= 0)
        {
            if (tiles[rank - 2][file - 1].icon == ' ' || tiles[rank - 2][file - 1].piece->isWhite() != is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank - 2, file - 1));
            }
        }
        if (rank - 1 >= 0 && file - 2 >= 0)
        {
            if (tiles[rank - 1][file - 2].icon == ' ' || tiles[rank - 1][file - 2].piece->isWhite() != is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank - 1, file - 2));
            }
        }

        knight->addPossibleMoves(possibleMoves);
    }

    vector<pair<int, int>> calculate_bishop_moves(int rank, int file)
    {
        vector<pair<int, int>> possibleMoves;
        Chess_Piece *bishop = tiles[rank][file].piece;

        // Search for possible moves in the four diagonal directions
        int i = rank + 1;
        int j = file + 1;
        while (i < 8 && j < 8)
        {
            if (tiles[i][j].icon == ' ')
            {
                possibleMoves.push_back(make_pair(i, j));
            }
            else
            {
                if (tiles[i][j].piece->isWhite() != is_white_turn)
                {
                    possibleMoves.push_back(make_pair(i, j));
                }
                break;
            }
            i++;
            j++;
        }
        i = rank - 1;
        j = file - 1;
        while (i >= 0 && j >= 0)
        {
            if (tiles[i][j].icon == ' ')
            {
                possibleMoves.push_back(make_pair(i, j));
            }
            else
            {
                if (tiles[i][j].piece->isWhite() != is_white_turn)
                {
                    possibleMoves.push_back(make_pair(i, j));
                }
                break;
            }
            i--;
            j--;
        }
        i = rank + 1;
        j = file - 1;
        while (i < 8 && j >= 0)
        {
            if (tiles[i][j].icon == ' ')
            {
                possibleMoves.push_back(make_pair(i, j));
            }
            else
            {
                if (tiles[i][j].piece->isWhite() != is_white_turn)
                {
                    possibleMoves.push_back(make_pair(i, j));
                }
                break;
            }
            i++;
            j--;
        }
        i = rank - 1;
        j = file + 1;
        while (i >= 0 && j < 8)
        {
            if (tiles[i][j].icon == ' ')
            {
                possibleMoves.push_back(make_pair(i, j));
            }
            else
            {
                if (tiles[i][j].piece->isWhite() != is_white_turn)
                {
                    possibleMoves.push_back(make_pair(i, j));
                }
                break;
            }
            i--;
            j++;
        }

        bishop->addPossibleMoves(possibleMoves);
        return possibleMoves;
    }

    void calculate_queen_moves(int rank, int file)
    {
        vector<pair<int, int>> possibleMoves;
        Chess_Piece *queen = tiles[rank][file].piece;

        // Calculate possible moves as a combination of the bishop's and rook's
        vector<pair<int, int>> rook_moves = calculate_rook_moves(rank, file);
        vector<pair<int, int>> bishop_moves = calculate_bishop_moves(rank, file);

        possibleMoves.insert(possibleMoves.end(), rook_moves.begin(), rook_moves.end());
        possibleMoves.insert(possibleMoves.end(), bishop_moves.begin(), bishop_moves.end());

        queen->addPossibleMoves(possibleMoves);
    }

    void calculate_king_moves(int rank, int file)
    {
        vector<pair<int, int>> possibleMoves;
        Chess_Piece *king = tiles[rank][file].piece;

        if (rank + 1 < 8)
        {
            if (tiles[rank + 1][file].icon == ' ' || tiles[rank + 1][file].piece->isWhite() != is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank + 1, file));
            }
        }
        if (rank - 1 >= 0)
        {
            if (tiles[rank - 1][file].icon == ' ' || tiles[rank - 1][file].piece->isWhite() != is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank - 1, file));
            }
        }
        if (file + 1 < 8)
        {
            if (tiles[rank][file + 1].icon == ' ' || tiles[rank][file + 1].piece->isWhite() != is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank, file + 1));
            }
        }
        if (file - 1 >= 0)
        {
            if (tiles[rank][file - 1].icon == ' ' || tiles[rank][file - 1].piece->isWhite() != is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank, file - 1));
            }
        }
        if (rank + 1 < 8 && file + 1 < 8)
        {
            if (tiles[rank + 1][file + 1].icon == ' ' || tiles[rank + 1][file + 1].piece->isWhite() != is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank + 1, file + 1));
            }
        }
        if (rank + 1 < 8 && file - 1 >= 0)
        {
            if (tiles[rank + 1][file - 1].icon == ' ' || tiles[rank + 1][file - 1].piece->isWhite() != is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank + 1, file - 1));
            }
        }
        if (rank - 1 >= 0 && file + 1 < 8)
        {
            if (tiles[rank - 1][file + 1].icon == ' ' || tiles[rank - 1][file + 1].piece->isWhite() != is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank - 1, file + 1));
            }
        }
        if (rank - 1 >= 0 && file - 1 >= 0)
        {
            if (tiles[rank - 1][file - 1].icon == ' ' || tiles[rank - 1][file - 1].piece->isWhite() != is_white_turn)
            {
                possibleMoves.push_back(make_pair(rank - 1, file - 1));
            }
        }

        king->addPossibleMoves(possibleMoves);
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
    bool skip_display = false;

    while (true)
    {
        if (board.isWhiteTurn())
        {
            std::cout << "White's turn." << std::endl;
        }
        else
        {
            std::cout << "Black's turn." << std::endl;
        }

        if (!skip_display)
        {
            board.display_board();
        }
        else
        {
            skip_display = false;
        }
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
        else if (move.length() == 2)
        {
            board.show_possible_moves(move);
            skip_display = true;
            continue;
        }
        else if (regex_match(move, regex("[a-h]{1}[1-8]{1}[QRBN]{1}"))) // Queen Promotion
        {
            board.promote_pawn(move);
        }
        else if (move.length() == 4)
        {
            move = "P" + move;
            board.move_piece(move);
            continue;
        }
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