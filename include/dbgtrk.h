#define DBGTRK 1

EXT boolean dbgTrk;
#define TRKBUFSIZE (4*64)
EXT int16_t trkidx;
EXT int16_t trkbuf[TRKBUFSIZE];

constexpr auto DBGTRK2WL0 = 1;  /* encoder capature */
constexpr auto DBGTRK2WL1 = 0;  /* interal capature */

constexpr auto DBGTRK1W = 0;
inline void dbgTrk1W(uint16_t val1  __attribute__((unused))) {}

constexpr auto DBGTRK1L = 0;
inline void dbgTrk1L(uint32_t val1  __attribute__((unused))) {}

constexpr auto DBGTRK2L = 0;
inline void dbgTrk2L(uint32_t val1  __attribute__((unused)),
	uint32_t val2  __attribute__((unused))) {}

constexpr auto DBGTRK2WL = 1;
inline void dbgTrk2WL(uint16_t val1  __attribute__((unused)),
	uint16_t val2  __attribute__((unused)),
	uint32_t val3  __attribute__((unused))) \
{ \
 int16_t *p = (int16_t *) &trkbuf[trkidx]; \
 *p++ = val1; \
 *p++ = val2; \
 *((int32_t *) p) = val3; \
 trkidx += 4; \
 trkidx &= (TRKBUFSIZE - 1); \
}

