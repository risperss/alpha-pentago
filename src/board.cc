#include <iostream>

#include "board.h"
#include "bitboard.h"


namespace pen {

namespace {
    std::uint16_t kMoveNum[] = {
        0x0100, 0x0000, 0x0140, 0x0040, 0x0180, 0x0080, 0x01C0, 0x00C0,
        0x0106, 0x0006, 0x0146, 0x0046, 0x0186, 0x0086, 0x01C6, 0x00C6,
        0x010C, 0x000C, 0x014C, 0x004C, 0x018C, 0x008C, 0x01CC, 0x00CC,
        0x0112, 0x0012, 0x0152, 0x0052, 0x0192, 0x0092, 0x01D2, 0x00D2,
        0x0118, 0x0018, 0x0158, 0x0058, 0x0198, 0x0098, 0x01D8, 0x00D8,
        0x011E, 0x001E, 0x015E, 0x005E, 0x019E, 0x009E, 0x01DE, 0x00DE,
        0x0101, 0x0001, 0x0141, 0x0041, 0x0181, 0x0081, 0x01C1, 0x00C1,
        0x0107, 0x0007, 0x0147, 0x0047, 0x0187, 0x0087, 0x01C7, 0x00C7,
        0x010D, 0x000D, 0x014D, 0x004D, 0x018D, 0x008D, 0x01CD, 0x00CD,
        0x0113, 0x0013, 0x0153, 0x0053, 0x0193, 0x0093, 0x01D3, 0x00D3,
        0x0119, 0x0019, 0x0159, 0x0059, 0x0199, 0x0099, 0x01D9, 0x00D9,
        0x011F, 0x001F, 0x015F, 0x005F, 0x019F, 0x009F, 0x01DF, 0x00DF,
        0x0102, 0x0002, 0x0142, 0x0042, 0x0182, 0x0082, 0x01C2, 0x00C2,
        0x0108, 0x0008, 0x0148, 0x0048, 0x0188, 0x0088, 0x01C8, 0x00C8,
        0x010E, 0x000E, 0x014E, 0x004E, 0x018E, 0x008E, 0x01CE, 0x00CE,
        0x0114, 0x0014, 0x0154, 0x0054, 0x0194, 0x0094, 0x01D4, 0x00D4,
        0x011A, 0x001A, 0x015A, 0x005A, 0x019A, 0x009A, 0x01DA, 0x00DA,
        0x0120, 0x0020, 0x0160, 0x0060, 0x01A0, 0x00A0, 0x01E0, 0x00E0,
        0x0103, 0x0003, 0x0143, 0x0043, 0x0183, 0x0083, 0x01C3, 0x00C3,
        0x0109, 0x0009, 0x0149, 0x0049, 0x0189, 0x0089, 0x01C9, 0x00C9,
        0x010F, 0x000F, 0x014F, 0x004F, 0x018F, 0x008F, 0x01CF, 0x00CF,
        0x0115, 0x0015, 0x0155, 0x0055, 0x0195, 0x0095, 0x01D5, 0x00D5,
        0x011B, 0x001B, 0x015B, 0x005B, 0x019B, 0x009B, 0x01DB, 0x00DB,
        0x0121, 0x0021, 0x0161, 0x0061, 0x01A1, 0x00A1, 0x01E1, 0x00E1,
        0x0104, 0x0004, 0x0144, 0x0044, 0x0184, 0x0084, 0x01C4, 0x00C4,
        0x010A, 0x000A, 0x014A, 0x004A, 0x018A, 0x008A, 0x01CA, 0x00CA,
        0x0110, 0x0010, 0x0150, 0x0050, 0x0190, 0x0090, 0x01D0, 0x00D0,
        0x0116, 0x0016, 0x0156, 0x0056, 0x0196, 0x0096, 0x01D6, 0x00D6,
        0x011C, 0x001C, 0x015C, 0x005C, 0x019C, 0x009C, 0x01DC, 0x00DC,
        0x0122, 0x0022, 0x0162, 0x0062, 0x01A2, 0x00A2, 0x01E2, 0x00E2,
        0x0105, 0x0005, 0x0145, 0x0045, 0x0185, 0x0085, 0x01C5, 0x00C5,
        0x010B, 0x000B, 0x014B, 0x004B, 0x018B, 0x008B, 0x01CB, 0x00CB,
        0x0111, 0x0011, 0x0151, 0x0051, 0x0191, 0x0091, 0x01D1, 0x00D1,
        0x0117, 0x0017, 0x0157, 0x0057, 0x0197, 0x0097, 0x01D7, 0x00D7,
        0x011D, 0x001D, 0x015D, 0x005D, 0x019D, 0x009D, 0x01DD, 0x00DD,
        0x0123, 0x0023, 0x0163, 0x0063, 0x01A3, 0x00A3, 0x01E3, 0x00E3};

    std::uint64_t kWinningMasks[] = {
        0x000000000000001F, 0x000000000000003E, 0x0000000001041041, 0x0000000041041040,
        0x00000000000007C0, 0x0000000000000F80, 0x0000000002082082, 0x0000000082082080,
        0x000000000001F000, 0x000000000003E000, 0x0000000004104104, 0x0000000104104100,
        0x00000000007C0000, 0x0000000000F80000, 0x0000000008208208, 0x0000000208208200,
        0x000000001F000000, 0x000000003E000000, 0x0000000010410410, 0x0000000410410400,
        0x00000007C0000000, 0x0000000F80000000, 0x0000000020820820, 0x0000000820820800,
        0x0000000010204081, 0x0000000810204080, 0x0000000002108420, 0x0000000042108400,
        0x0000000408102040, 0x0000000020408102, 0x0000000001084210, 0x0000000084210800};
}

MoveList PentagoBoard::GenerateLegalMoves() const {
    MoveList result;
    result.reserve(144);

    BitBoard mut_our_pieces = our_pieces();

    int i = 0;
    while (i < 288) {
        Move move = Move(kMoveNum[i]);

        if (our_pieces().get(move.node()) || their_pieces().get(move.node())) {
            i += 8;
            continue;
        }

        bool placedNode = false;
        bool nodeOnRotatedSquare = false;
        bool passMoveRecorded = false;
        Move passMove;
        int j = 0;

        while (j < 8) {
            Move nodeMove = Move(kMoveNum[i+j]);

            if (!nodeOnRotatedSquare && nodeMove.nodeOnSquare()) {
                nodeOnRotatedSquare = true;
                mut_our_pieces.set(nodeMove.node());
            }

            BoardSquare ourSquare = BoardSquare(mut_our_pieces.as_int(), nodeMove.squarePos());
            BoardSquare theirSquare = BoardSquare(their_pieces().as_int(), nodeMove.squarePos());

            if (!(ourSquare.IsSymmetrical() && theirSquare.IsSymmetrical())) {
                placedNode = true;
                result.emplace_back(nodeMove);
                result.emplace_back(Move(kMoveNum[i+j+1]));
            } else if (!passMoveRecorded) {
                passMoveRecorded = true;
                passMove = nodeMove;
            }

            if (nodeOnRotatedSquare) {
                mut_our_pieces.reset(nodeMove.node());
            }

            j += 2;
        }

        if (!placedNode) {
            result.emplace_back(move);
        } else if (passMoveRecorded) {
            result.emplace_back(passMove);
        }

        i += 8;
    }
    return result;
}

void PentagoBoard::ApplyMove(Move move) {
    our_pieces_.set(move.node());

    BoardSquare ourSquare = BoardSquare(our_pieces_.as_int(), move.squarePos());
    BoardSquare theirSquare = BoardSquare(their_pieces().as_int(), move.squarePos());

    ourSquare.rotate(move.clockwise());
    theirSquare.rotate(move.clockwise());

    our_pieces_.set(ourSquare, move.squarePos());
    their_pieces_.set(theirSquare, move.squarePos());
}

BoardResult PentagoBoard::ComputeBoardResult() const {
    bool weWon = false;
    bool theyWon = false;

    for ( std::uint64_t mask : kWinningMasks ) {
        if (!weWon && (our_pieces() & mask) == mask) {
            weWon = true;
            continue;
        }
        if (!theyWon && (their_pieces() & mask) == mask) {
            theyWon = true;
        }
    }

    if (weWon && theyWon) {
        return BoardResult::DRAW;
    } else if (weWon) {
        return BoardResult::WE_WON;
    } else if (theyWon) {
        return BoardResult::THEY_WON;
    } else if (full()) {
        return BoardResult::DRAW;
    } else {
        return BoardResult::UNDECIDED;
    }
}

std::string PentagoBoard::DebugString(bool blackToMove) const {
    std::string res;
    char usChar = blackToMove ? 'b' : 'w';
    char themChar = blackToMove ? 'w' : 'b';

    for (int i = 5; i >= 0; --i) {
        for (int j = 0; j < 6; ++j) {
            if (our_pieces().get(i, j)) {
                res += usChar;
            } else if (their_pieces().get(i, j)) {
                res += themChar;
            } else {
                res += '.';
            }
            res += ' ';
        }
        res += '\n';
    }
    return res;
}

} // namespace pen
