#ifndef __JS_DEF_H__
#define __JS_DEF_H__

#ifdef BUTTONS_12
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

#ifdef BUTTONS_16_FROM_GITHUB
#define BTN_1       (1ULL << 48)
#define BTN_2       (1ULL << 49)
#define BTN_3       (1ULL << 50)
#define BTN_4       (1ULL << 51)

#define BTN_5       (1ULL << 52)
#define BTN_6       (1ULL << 53)
#define BTN_7       (1ULL << 54)
#define BTN_8       (1ULL << 55)

#define BTN_9       (1ULL << 40)
#define BTN_10      (1ULL << 41)
#define BTN_11      (1ULL << 42)
#define BTN_12      (1ULL << 43)

#define BTN_13      (1ULL << 44)
#define BTN_14      (1ULL << 45)
#define BTN_15      (1ULL << 46)
#define BTN_16      (1ULL << 47)

#define AX_NONE       (8ULL << 32)
#define AX_UP         (0ULL << 32)
#define AX_UPRIGHT    (1ULL << 32)
#define AX_RIGHT      (2ULL << 32)
#define AX_DOWNRIGHT  (3ULL << 32)
#define AX_DOWN       (4ULL << 32)
#define AX_DOWNLEFT   (5ULL << 32)
#define AX_LEFT       (6ULL << 32)
#define AX_UPLEFT     (7ULL << 32)
#define AX_MASK       ~(0x0fULL << 32)

#define JS_INIT_VALUE	0x0000000880808080ULL

#define SET_BUTTON(v,a)		{ v &= ~a; v |= a; }
#define UNSET_BUTTON(v,a)	{ v &= ~a; }

#define SET_AXIS(v,a)	{ v &= AX_MASK; v |= a; }
#define UNSET_AXIS(v)	{ SET_ASIX(v,AX_NONE); }

#endif

#endif
