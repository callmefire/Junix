#ifndef __BITOPS_H__
#define __BITOPS_H__

#include <asm/types.h>

/* Hamming weight */
static u8 b1 = 0x55; /* 01010101 */
static u8 b2 = 0x33; /* 00110011 */
static u8 b4 = 0x0f; /* 00001111 */

static inline u8 countbit_8(u8 val)
{
    val = (val & b1) + ((val >> 1) & b1);
    val = (val & b2) + ((val >> 2) & b2);
    val = (val & b4) + ((val >> 4) & b4);
    return val;
}

static u16 t1 = 0x5555; /* 01010101 01010101 */
static u16 t2 = 0x3333; /* 00110011 00110011 */
static u16 t4 = 0x0f0f; /* 00001111 00001111 */
static u16 t8 = 0x00ff; /* 00000000 11111111 */

static inline u16 countbit_16(u16 val)
{
    val = (val & t1) + ((val >> 1) & t1);    
    val = (val & t2) + ((val >> 2) & t2);    
    val = (val & t4) + ((val >> 4) & t4);    
    val = (val & t8) + ((val >> 8) & t8);   
    return val; 
}


static u32 w1 = 0x55555555UL; /* 01010101 01010101 01010101 01010101 */
static u32 w2 = 0x33333333UL; /* 00110011 00110011 00110011 00110011 */
static u32 w4 = 0x0f0f0f0fUL; /* 00001111 00001111 00001111 00001111 */
static u32 w8 = 0x00ff00ffUL; /* 00000000 11111111 00000000 11111111 */
static u32 w16= 0x0000ffffUL; /* 00000000 00000000 11111111 11111111 */

static inline u32 countbit_32(u32 val)
{
    val = (val & w1) + ((val >> 1) & w1);
    val = (val & w2) + ((val >> 2) & w2);
    val = (val & w4) + ((val >> 4) & w4);
    val = (val & w8) + ((val >> 8) & w8);
    val = (val & w16) + ((val >> 16) & w16);
    return val;
}

static u64 d1 = 0x5555555555555555ULL;
static u64 d2 = 0x3333333333333333ULL;
static u64 d4 = 0x0f0f0f0f0f0f0f0fULL;
static u64 d8 = 0x00ff00ff00ff00ffULL;
static u64 d16= 0x0000ffff0000ffffULL;
static u64 d32= 0x00000000ffffffffULL;

static inline u64 countbit_64(u64 val)
{
    val = (val & d1) + ((val >> 1) & d1);
    val = (val & d2) + ((val >> 2) & d2);
    val = (val & d4) + ((val >> 4) & d4);
    val = (val & d8) + ((val >> 8) & d8);
    val = (val & d16) + ((val >> 16) & d16);
    val = (val & d32) + ((val >> 32) & d32);
    return val;
}

#endif
