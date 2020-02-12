#ifndef __JS_DEF_H__
#define __JS_DEF_H__

#define REPORT_LENG	8

#define SET_BUTTON(d,l,v)		(d[l] |= v)
#define UNSET_BUTTON(d,l,v)	(d[l] &= ~v)

#define SET_AXIS(d,v)				(d[2] = v)
#define AAXIS(d,l,v) \
	{ \
		if (v > 127) d[l] = 0xff; \
		else if (v < -128) d[l] = 0x0; \
		else d[l] = static_cast<uint8_t>(v + 0x80); \
	}

#define RESET_REPORT(d) memset(d, 0, REPORT_LENG)

// byte 0
#define BTN_A				0x04
#define BTN_B				0x02
#define BTN_X				0x08
#define BTN_Y				0x01
#define BTN_L				0x10
#define BTN_R				0x20
#define BTN_ZL			0x40
#define BTN_ZR			0x80

// byte 1
#define BTN_M				0x01
#define BTN_P				0x02
#define BTN_L3			0x04
#define BTN_R3			0x08
#define BTN_HOME		0x10
#define BTN_CAPTURE	0x20

#define AX_NONE				(8)
#define AX_UP					(0)
#define AX_UPRIGHT		(1)
#define AX_RIGHT			(2)
#define AX_DOWNRIGHT	(3)
#define AX_DOWN				(4)
#define AX_DOWNLEFT		(5)
#define AX_LEFT				(6)
#define AX_UPLEFT			(7)

#endif
