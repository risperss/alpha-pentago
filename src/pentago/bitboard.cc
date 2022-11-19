#include <cassert>

#include "bitboard.h"

namespace apentago {

namespace {

const Move kMove[] = {
	"a1L1", "a1R1", "a1L2", "a1R2", "a1L3", "a1R3", "a1L4", "a1R4", 
	"a2L1", "a2R1", "a2L2", "a2R2", "a2L3", "a2R3", "a2L4", "a2R4", 
	"a3L1", "a3R1", "a3L2", "a3R2", "a3L3", "a3R3", "a3L4", "a3R4", 
	"a4L1", "a4R1", "a4L2", "a4R2", "a4L3", "a4R3", "a4L4", "a4R4", 
	"a5L1", "a5R1", "a5L2", "a5R2", "a5L3", "a5R3", "a5L4", "a5R4", 
	"a6L1", "a6R1", "a6L2", "a6R2", "a6L3", "a6R3", "a6L4", "a6R4", 
	"b1L1", "b1R1", "b1L2", "b1R2", "b1L3", "b1R3", "b1L4", "b1R4", 
	"b2L1", "b2R1", "b2L2", "b2R2", "b2L3", "b2R3", "b2L4", "b2R4", 
	"b3L1", "b3R1", "b3L2", "b3R2", "b3L3", "b3R3", "b3L4", "b3R4", 
	"b4L1", "b4R1", "b4L2", "b4R2", "b4L3", "b4R3", "b4L4", "b4R4", 
	"b5L1", "b5R1", "b5L2", "b5R2", "b5L3", "b5R3", "b5L4", "b5R4", 
	"b6L1", "b6R1", "b6L2", "b6R2", "b6L3", "b6R3", "b6L4", "b6R4", 
	"c1L1", "c1R1", "c1L2", "c1R2", "c1L3", "c1R3", "c1L4", "c1R4", 
	"c2L1", "c2R1", "c2L2", "c2R2", "c2L3", "c2R3", "c2L4", "c2R4", 
	"c3L1", "c3R1", "c3L2", "c3R2", "c3L3", "c3R3", "c3L4", "c3R4", 
	"c4L1", "c4R1", "c4L2", "c4R2", "c4L3", "c4R3", "c4L4", "c4R4", 
	"c5L1", "c5R1", "c5L2", "c5R2", "c5L3", "c5R3", "c5L4", "c5R4", 
	"c6L1", "c6R1", "c6L2", "c6R2", "c6L3", "c6R3", "c6L4", "c6R4", 
	"d1L1", "d1R1", "d1L2", "d1R2", "d1L3", "d1R3", "d1L4", "d1R4", 
	"d2L1", "d2R1", "d2L2", "d2R2", "d2L3", "d2R3", "d2L4", "d2R4", 
	"d3L1", "d3R1", "d3L2", "d3R2", "d3L3", "d3R3", "d3L4", "d3R4", 
	"d4L1", "d4R1", "d4L2", "d4R2", "d4L3", "d4R3", "d4L4", "d4R4", 
	"d5L1", "d5R1", "d5L2", "d5R2", "d5L3", "d5R3", "d5L4", "d5R4", 
	"d6L1", "d6R1", "d6L2", "d6R2", "d6L3", "d6R3", "d6L4", "d6R4", 
	"e1L1", "e1R1", "e1L2", "e1R2", "e1L3", "e1R3", "e1L4", "e1R4", 
	"e2L1", "e2R1", "e2L2", "e2R2", "e2L3", "e2R3", "e2L4", "e2R4", 
	"e3L1", "e3R1", "e3L2", "e3R2", "e3L3", "e3R3", "e3L4", "e3R4", 
	"e4L1", "e4R1", "e4L2", "e4R2", "e4L3", "e4R3", "e4L4", "e4R4", 
	"e5L1", "e5R1", "e5L2", "e5R2", "e5L3", "e5R3", "e5L4", "e5R4", 
	"e6L1", "e6R1", "e6L2", "e6R2", "e6L3", "e6R3", "e6L4", "e6R4", 
	"f1L1", "f1R1", "f1L2", "f1R2", "f1L3", "f1R3", "f1L4", "f1R4", 
	"f2L1", "f2R1", "f2L2", "f2R2", "f2L3", "f2R3", "f2L4", "f2R4", 
	"f3L1", "f3R1", "f3L2", "f3R2", "f3L3", "f3R3", "f3L4", "f3R4", 
	"f4L1", "f4R1", "f4L2", "f4R2", "f4L3", "f4R3", "f4L4", "f4R4", 
	"f5L1", "f5R1", "f5L2", "f5R2", "f5L3", "f5R3", "f5L4", "f5R4", 
	"f6L1", "f6R1", "f6L2", "f6R2", "f6L3", "f6R3", "f6L4", "f6R4"};

}

BoardSquare::BoardSquare(const BitBoard& board, int squarePos)  {
        int shiftValue = 0;
        switch (squarePos) {
        case 1:
            break;
        case 2:
            shiftValue = 3;
            break;
        case 3:
            shiftValue = 18;
            break;
        case 4:
            shiftValue = 21;
            break;
        default:
            assert(false);
        }
        square_ = (board.as_int() >> shiftValue) & kSquareMask;
    }


}

int main(void) {
	return 0;
}
