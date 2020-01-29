#ifndef __JS_DEF_H__
#define __JS_DEF_H__

#define BTN_A		(1ULL << 20)
#define BTN_B		(1ULL << 21)
#define BTN_X		(1ULL << 22)
#define BTN_Y		(1ULL << 23)

#define BTN_L		(1ULL << 8)
#define BTN_R		(1ULL << 9)
#define BTN_LZ		(1ULL << 10)
#define BTN_RZ		(1ULL << 11)

#define BTN_MINUS	(1ULL << 12)
#define BTN_PLUS	(1ULL << 13)
#define BTN_L2		(1ULL << 14)
#define BTN_R2		(1ULL << 15)

#define AX_NONE		(8ULL << 16)
#define AX_UP		(0ULL << 16)
#define AX_UPRIGHT	(1ULL << 16)
#define AX_RIGHT	(2ULL << 16)
#define AX_DOWNRIGHT	(3ULL << 16)
#define AX_DOWN		(4ULL << 16)
#define AX_DOWNLEFT	(5ULL << 16)
#define AX_LEFT		(6ULL << 16)
#define AX_UPLEFT	(7ULL << 16)
#define AX_MASK		~(0x0fULL << 16)

#define JS_INIT_VALUE	0x8080808080080000ULL

#define SET_BUTTON(v,a)		{ v &= ~a; v |= a; }
#define UNSET_BUTTON(v,a)	{ v &= ~a; }

#define SET_AXIS(v,a)	{ v &= AX_MASK; v |= a; }
#define UNSET_AXIS(v)	{ SET_ASIX(v,AX_NONE); }

#endif
