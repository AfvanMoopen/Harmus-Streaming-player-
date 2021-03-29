/*!
 * \copy
 *     Copyright (c)  2013, Cisco Systems
 *     All rights reserved.
 *
 *     Redistribution and use in source and binary forms, with or without
 *     modification, are permitted provided that the following conditions
 *     are met:
 *
 *        * Redistributions of source code must retain the above copyright
 *          notice, this list of conditions and the following disclaimer.
 *
 *        * Redistributions in binary form must reproduce the above copyright
 *          notice, this list of conditions and the following disclaimer in
 *          the documentation and/or other materials provided with the
 *          distribution.
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *     "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *     FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *     COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *     INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *     BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *     CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *     LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *     ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *     POSSIBILITY OF SUCH DAMAGE.
 *
 */

// exp_data.c
// export date cross various modules (.c)
#include "md.h"
#include "vlc_encoder.h"
namespace WelsEnc {
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// extern at mb_cache.h
const uint8_t g_kuiSmb4AddrIn256[16] = {
  0,           4,            16 * 4,      16 * 4 + 4,
  8,           12,           16 * 4 + 8,  16 * 4 + 12,
  16 * 8,      16 * 8 + 4,   16 * 12,     16 * 12 + 4,
  16 * 8 + 8,  16 * 8 + 12,  16 * 12 + 8, 16 * 12 + 12
};


const uint8_t g_kuiCache12_8x8RefIdx[4] = { //mv or uiRefIndex cache scan index, 4*4 block as basic unit
  5, 6,
  9, 10
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// extern at md.h
const int32_t g_kiQpCostTable[52] = {
  1, 1, 1, 1, 1, 1, 1, 1,  /*  0-7 */
  1, 1, 1, 1,              /*  8-11 */
  1, 1, 1, 1, 2, 2, 2, 2,  /* 12-19 */
  3, 3, 3, 4, 4, 4, 5, 6,  /* 20-27 */
  6, 7, 8, 9, 10, 11, 13, 14, /* 28-35 */
  16, 18, 20, 23, 25, 29, 32, 36, /* 36-43 */
  40, 45, 51, 57, 64, 72, 81, 91 /* 44-51 */
};
const int8_t g_kiMapModeI16x16[7] = {
  0, 1, 2, 3, 2, 2, 2
};//{I16_PRED_V, I16_PRED_H, I16_PRED_DC, I16_PRED_P, I16_PRED_DC, I16_PRED_DC, I16_PRED_DC};

const int8_t g_kiMapModeIntraChroma[7] = {
  0, 1, 2, 3, 0, 0, 0
};//{C_PRED_DC, C_PRED_H, C_PRED_V, C_PRED_P, C_PRED_DC_L, C_PRED_DC_T, C_PRED_DC_128};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// extern at vlc_encoder.h

//g_kuiVlcCoeffToken[nc][total-coeff][trailing-ones][0--value, 1--bit count]
const uint8_t g_kuiVlcCoeffToken[5][17][4][2] = {
  {
    //0<=nc<2
    { { 1,  1}, { 0,  0}, { 0,  0}, { 0,  0} },//0
    { { 5,  6}, { 1,  2}, { 0,  0}, { 0,  0} },//1
    { { 7,  8}, { 4,  6}, { 1,  3}, { 0,  0} },//2
    { { 7,  9}, { 6,  8}, { 5,  7}, { 3,  5} },//3
    { { 7, 10}, { 6,  9}, { 5,  8}, { 3,  6} },//4
    { { 7, 11}, { 6, 10}, { 5,  9}, { 4,  7} },//5
    { {15, 13}, { 6, 11}, { 5, 10}, { 4,  8} },//6
    { {11, 13}, {14, 13}, { 5, 11}, { 4,  9} },//7
    { { 8, 13}, {10, 13}, {13, 13}, { 4, 10} },//8
    { {15, 14}, {14, 14}, { 9, 13}, { 4, 11} },//9
    { {11, 14}, {10, 14}, {13, 14}, {12, 13} },//10
    { {15, 15}, {14, 15}, { 9, 14}, {12, 14} },//11
    { {11, 15}, {10, 15}, {13, 15}, { 8, 14} },//12
    { {15, 16}, { 1, 15}, { 9, 15}, {12, 15} },//13
    { {11, 16}, {14, 16}, {13, 16}, { 8, 15} },//14
    { { 7, 16}, {10, 16}, { 9, 16}, {12, 16} },//15
    { { 4, 16}, { 6, 16}, { 5, 16}, { 8, 16} } //16
  },

  {
    //2<=nc<4
    { { 3,  2}, { 0,  0}, { 0,  0}, { 0,  0} },//0
    { {11,  6}, { 2,  2}, { 0,  0}, { 0,  0} },//1
    { { 7,  6}, { 7,  5}, { 3,  3}, { 0,  0} },//2
    { { 7,  7}, {10,  6}, { 9,  6}, { 5,  4} },//3
    { { 7,  8}, { 6,  6}, { 5,  6}, { 4,  4} },//4
    { { 4,  8}, { 6,  7}, { 5,  7}, { 6,  5} },//5
    { { 7,  9}, { 6,  8}, { 5,  8}, { 8,  6} },//6
    { {15, 11}, { 6,  9}, { 5,  9}, { 4,  6} },//7
    { {11, 11}, {14, 11}, {13, 11}, { 4,  7} },//8
    { {15, 12}, {10, 11}, { 9, 11}, { 4,  9} },//9
    { {11, 12}, {14, 12}, {13, 12}, {12, 11} },//10
    { { 8, 12}, {10, 12}, { 9, 12}, { 8, 11} },//11
    { {15, 13}, {14, 13}, {13, 13}, {12, 12} },//12
    { {11, 13}, {10, 13}, { 9, 13}, {12, 13} },//13
    { { 7, 13}, {11, 14}, { 6, 13}, { 8, 13} },//14
    { { 9, 14}, { 8, 14}, {10, 14}, { 1, 13} },//15
    { { 7, 14}, { 6, 14}, { 5, 14}, { 4, 14} } //16
  },

  {
    //4<=nc<8
    { {15,  4}, { 0,  0}, { 0,  0}, { 0,  0} },//0
    { {15,  6}, {14,  4}, { 0,  0}, { 0,  0} },//1
    { {11,  6}, {15,  5}, {13,  4}, { 0,  0} },//2
    { { 8,  6}, {12,  5}, {14,  5}, {12,  4} },//3
    { {15,  7}, {10,  5}, {11,  5}, {11,  4} },//4
    { {11,  7}, { 8,  5}, { 9,  5}, {10,  4} },//5
    { { 9,  7}, {14,  6}, {13,  6}, { 9,  4} },//6
    { { 8,  7}, {10,  6}, { 9,  6}, { 8,  4} },//7
    { {15,  8}, {14,  7}, {13,  7}, {13,  5} },//8
    { {11,  8}, {14,  8}, {10,  7}, {12,  6} },//9
    { {15,  9}, {10,  8}, {13,  8}, {12,  7} },//10
    { {11,  9}, {14,  9}, { 9,  8}, {12,  8} },//11
    { { 8,  9}, {10,  9}, {13,  9}, { 8,  8} },//12
    { {13, 10}, { 7,  9}, { 9,  9}, {12,  9} },//13
    { { 9, 10}, {12, 10}, {11, 10}, {10, 10} },//14
    { { 5, 10}, { 8, 10}, { 7, 10}, { 6, 10} },//15
    { { 1, 10}, { 4, 10}, { 3, 10}, { 2, 10} } //16
  },

  {
    //8<=nc
    { { 3,  6}, { 0,  0}, { 0,  0}, { 0,  0} },//0
    { { 0,  6}, { 1,  6}, { 0,  0}, { 0,  0} },//1
    { { 4,  6}, { 5,  6}, { 6,  6}, { 0,  0} },//2
    { { 8,  6}, { 9,  6}, {10,  6}, {11,  6} },//3
    { {12,  6}, {13,  6}, {14,  6}, {15,  6} },//4
    { {16,  6}, {17,  6}, {18,  6}, {19,  6} },//5
    { {20,  6}, {21,  6}, {22,  6}, {23,  6} },//6
    { {24,  6}, {25,  6}, {26,  6}, {27,  6} },//7
    { {28,  6}, {29,  6}, {30,  6}, {31,  6} },//8
    { {32,  6}, {33,  6}, {34,  6}, {35,  6} },//9
    { {36,  6}, {37,  6}, {38,  6}, {39,  6} },//10
    { {40,  6}, {41,  6}, {42,  6}, {43,  6} },//11
    { {44,  6}, {45,  6}, {46,  6}, {47,  6} },//12
    { {48,  6}, {49,  6}, {50,  6}, {51,  6} },//13
    { {52,  6}, {53,  6}, {54,  6}, {55,  6} },//14
    { {56,  6}, {57,  6}, {58,  6}, {59,  6} },//15
    { {60,  6}, {61,  6}, {62,  6}, {63,  6} } //16
  },

  {
    //nc == -1
    { { 1,  2}, { 0,  0}, { 0,  0}, { 0,  0} },//0
    { { 7,  6}, { 1,  1}, { 0,  0}, { 0,  0} },//1
    { { 4,  6}, { 6,  6}, { 1,  3}, { 0,  0} },//2
    { { 3,  6}, { 3,  7}, { 2,  7}, { 5,  6} },//3
    { { 2,  6}, { 3,  8}, { 2,  8}, { 0,  7} },//4
    { { 0,  0}, { 0,  0}, { 0,  0}, { 0,  0} },//5
    { { 0,  0}, { 0,  0}, { 0,  0}, { 0,  0} },//6
    { { 0,  0}, { 0,  0}, { 0,  0}, { 0,  0} },//7
    { { 0,  0}, { 0,  0}, { 0,  0}, { 0,  0} },//8
    { { 0,  0}, { 0,  0}, { 0,  0}, { 0,  0} },//9
    { { 0,  0}, { 0,  0}, { 0,  0}, { 0,  0} },//10
    { { 0,  0}, { 0,  0}, { 0,  0}, { 0,  0} },//11
    { { 0,  0}, { 0,  0}, { 0,  0}, { 0,  0} },//12
    { { 0,  0}, { 0,  0}, { 0,  0}, { 0,  0} },//13
    { { 0,  0}, { 0,  0}, { 0,  0}, { 0,  0} },//14
    { { 0,  0}, { 0,  0}, { 0,  0}, { 0,  0} },//15
    { { 0,  0}, { 0,  0}, { 0,  0}, { 0,  0} } //16
  }
};

//const uint8_t g_kuiVlcLevelPrefix[15][2] =
//{
//  {1, 1}, {1, 2}
//};

//g_kuiVlcTotalZeros[tzVlcIndex][total_zeros][0--value, 1--bit count]
const uint8_t g_kuiVlcTotalZeros[16][16][2] = {
  {
    //0 not available
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //1
    {1, 1}, {3, 3}, {2, 3}, {3, 4}, {2, 4}, {3, 5}, {2, 5}, {3, 6}, {2, 6}, {3, 7}, {2, 7}, {3, 8}, {2, 8}, {3, 9}, {2, 9}, {1, 9}
  },
  {
    //2
    {7, 3}, {6, 3}, {5, 3}, {4, 3}, {3, 3}, {5, 4}, {4, 4}, {3, 4}, {2, 4}, {3, 5}, {2, 5}, {3, 6}, {2, 6}, {1, 6}, {0, 6}, {0, 0}
  },
  {
    //3
    {5, 4}, {7, 3}, {6, 3}, {5, 3}, {4, 4}, {3, 4}, {4, 3}, {3, 3}, {2, 4}, {3, 5}, {2, 5}, {1, 6}, {1, 5}, {0, 6}, {0, 0}, {0, 0}
  },
  {
    //4
    {3, 5}, {7, 3}, {5, 4}, {4, 4}, {6, 3}, {5, 3}, {4, 3}, {3, 4}, {3, 3}, {2, 4}, {2, 5}, {1, 5}, {0, 5}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //5
    {5, 4}, {4, 4}, {3, 4}, {7, 3}, {6, 3}, {5, 3}, {4, 3}, {3, 3}, {2, 4}, {1, 5}, {1, 4}, {0, 5}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //6
    {1, 6}, {1, 5}, {7, 3}, {6, 3}, {5, 3}, {4, 3}, {3, 3}, {2, 3}, {1, 4}, {1, 3}, {0, 6}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //7
    {1, 6}, {1, 5}, {5, 3}, {4, 3}, {3, 3}, {3, 2}, {2, 3}, {1, 4}, {1, 3}, {0, 6}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //8
    {1, 6}, {1, 4}, {1, 5}, {3, 3}, {3, 2}, {2, 2}, {2, 3}, {1, 3}, {0, 6}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //9
    {1, 6}, {0, 6}, {1, 4}, {3, 2}, {2, 2}, {1, 3}, {1, 2}, {1, 5}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //10
    {1, 5}, {0, 5}, {1, 3}, {3, 2}, {2, 2}, {1, 2}, {1, 4}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //11
    {0, 4}, {1, 4}, {1, 3}, {2, 3}, {1, 1}, {3, 3}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //12
    {0, 4}, {1, 4}, {1, 2}, {1, 1}, {1, 3}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //13
    {0, 3}, {1, 3}, {1, 1}, {1, 2}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //14
    {0, 2}, {1, 2}, {1, 1}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //15
    {0, 1}, {1, 1}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  }
};

const uint8_t g_kuiVlcTotalZerosChromaDc[4][4][2] = {
  {
    {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    {1, 1}, {1, 2}, {1, 3}, {0, 3}
  },
  {
    {1, 1}, {1, 2}, {0, 2}, {0, 0}
  },
  {
    {1, 1}, {0, 1}, {0, 0}, {0, 0}
  }
};
//

//g_kuiVlcRunBefore[zeros-left][run-before][0--value, 1--bit count]
const uint8_t g_kuiVlcRunBefore[8][15][2] = {
  {
    //0 not available
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //1
    {1, 1}, {0, 1}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //2
    {1, 1}, {1, 2}, {0, 2}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //3
    {3, 2}, {2, 2}, {1, 2}, {0, 2}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //4
    {3, 2}, {2, 2}, {1, 2}, {1, 3}, {0, 3}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //5
    {3, 2}, {2, 2}, {3, 3}, {2, 3}, {1, 3}, {0, 3}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //6
    {3, 2}, {0, 3}, {1, 3}, {3, 3}, {2, 3}, {5, 3}, {4, 3}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
  },
  {
    //>6
    {7, 3}, {6, 3}, {5, 3}, {4, 3}, {3, 3}, {2, 3}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {1, 9}, {1, 10}, {1, 11}
  }
};

const ALIGNED_DECLARE (uint8_t, g_kuiEncNcMapTable[18], 16) = {
  0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4
};



const uint8_t   g_kuiTemporalIdListTable[MAX_TEMPORAL_LEVEL][MAX_GOP_SIZE + 1] = {
  {
    0, 0, 0, 0, 0, 0, 0, 0,
    0
  },  // uiGopSize = 1
  {
    0, 1, 0, 0, 0, 0, 0, 0,
    0
  },  // uiGopSize = 2
  {
    0, 2, 1, 2, 0, 0, 0, 0,
    0
  },  // uiGopSize = 4
  {
    0, 3, 2, 3, 1, 3, 2, 3,
    0
  }  //uiGopSize = 8
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// extern at svc_encode_slice.h
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
