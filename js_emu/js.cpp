#include "js.h"
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

CJoyStick::CJoyStick() : m_nfd(-1) {

}

CJoyStick::CJoyStick(std::string dev) : m_nfd(-1) {
	m_szDev = dev;
}

CJoyStick::~CJoyStick() {
	if (-1 != m_nfd) {
		::close(m_nfd);
		m_nfd = -1;
	}
}
void CJoyStick::init() {
	m_vtReportData.reserve(REPORT_LENG);
	std::fill_n(m_vtReportData.begin(), REPORT_LENG, 0);
	close();
	if (!m_szDev.empty()) {
		m_nfd = ::open(m_szDev.c_str(), O_RDWR);
	}
}

void CJoyStick::close() {
	if (-1 != m_nfd) {
		::close(m_nfd);
		m_nfd = -1;
	}
}

void CJoyStick::setDev(std::string dev) {
	m_szDev = dev;
}

int CJoyStick::sendReport() {
	if (-1 == m_nfd) return 0;

	return static_cast<int>(::write(m_nfd, m_vtReportData.data(), m_vtReportData.capacity()));
}

void CJoyStick::resetData() {
	std::fill_n(m_vtReportData.begin(), REPORT_LENG, 0);
	axis(AX_NONE);
	aaxis(0, 0, 0, 0);
}

void CJoyStick::resetJoyStick() {
	if (-1 == m_nfd) {
		init();
	}
	resetData();
	sendReport();
	usleep(50000);
	button("lr", 1);
	sendReport();

	usleep(50000);

	button("lr", 0);
	sendReport();
}

#define SET_BTN(l,v)		SET_BUTTON(m_vtReportData,l,v)
#define UNSET_BTN(l,v)		UNSET_BUTTON(m_vtReportData,l,v)
#define SET_ARROW(v)		SET_AXIS(m_vtReportData, v)
#define ANALOG(l,v)			AAXIS(m_vtReportData,l,v)

void CJoyStick::button(std::string buttons, int press) {
	if (0 == buttons.length()) return;

	std::transform(buttons.begin(), buttons.end(), buttons.begin(), ::tolower);

	while (0 < buttons.length()) {
		char button = buttons.at(0);
		buttons = buttons.substr(1);
		switch(button) {
			default: break;
			case 'a': 0 != press ? SET_BTN(0, BTN_A) : UNSET_BTN(0, BTN_A); break;
			case 'b': 0 != press ? SET_BTN(0, BTN_B) : UNSET_BTN(0, BTN_B); break;
			case 'x': 0 != press ? SET_BTN(0, BTN_X) : UNSET_BTN(0, BTN_X); break;
			case 'y': 0 != press ? SET_BTN(0, BTN_Y) : UNSET_BTN(0, BTN_Y); break;
			case 'l': 0 != press ? SET_BTN(0, BTN_L) : UNSET_BTN(0, BTN_L); break;
			case 'r': 0 != press ? SET_BTN(0, BTN_R) : UNSET_BTN(0, BTN_R); break;
			case 'z':
				button = buttons.at(0);
				buttons = buttons.substr(1);
				switch (button) {
					default: break;
					case 'l': 0 != press ? SET_BTN(0, BTN_ZL) : UNSET_BTN(0, BTN_ZL); break;
					case 'r': 0 != press ? SET_BTN(0, BTN_ZR) : UNSET_BTN(0, BTN_ZR); break;
				}
				break;
			case '-': 0 != press ? SET_BTN(1, BTN_M) : UNSET_BTN(1, BTN_M); break;
			case '+': 0 != press ? SET_BTN(1, BTN_P) : UNSET_BTN(1, BTN_P); break;
			case '3':
				button = buttons.at(0);
				buttons = buttons.substr(1);
				switch (button) {
					default: break;
					case 'l': 0 != press ? SET_BTN(1, BTN_L3) : UNSET_BTN(1, BTN_L3); break;
					case 'r': 0 != press ? SET_BTN(1, BTN_R3) : UNSET_BTN(1, BTN_R3); break;
				}
				break;
			case 'h': 0 != press ? SET_BTN(1, BTN_HOME) : UNSET_BTN(1, BTN_HOME); break;
			case 'c': 0 != press ? SET_BTN(1, BTN_CAPTURE) : UNSET_BTN(1, BTN_CAPTURE); break;
		}
	}
}

void CJoyStick::axis(int val) {
	if (0 > val) val = 0;
	if (8 < val) val = 8;
	SET_ARROW(val);
}

void CJoyStick::aaxis(int lx, int ly, int rx, int ry) {
	if (AAXIS_NO_CHANGE != lx) {
		ANALOG(L_AAXIS_X, lx);
	}
	if (AAXIS_NO_CHANGE != ly) {
		ANALOG(L_AAXIS_Y, ly);
	}
	if (AAXIS_NO_CHANGE != rx) {
		ANALOG(R_AAXIS_X, rx);
	}
	if (AAXIS_NO_CHANGE != ry) {
		ANALOG(R_AAXIS_Y, ry);
	}
}
