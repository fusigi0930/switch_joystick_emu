#ifndef __JS_H__
#define __JS_H__

#include "js_def.h"
#include <vector>
#include <string>
#include <mutex>

class CJoyStick {
protected:
	std::vector<uint8_t> m_vtReportData;
	std::string m_szDev;
	int m_nfd;
	std::mutex m_sendMutex;

public:
	CJoyStick();
	CJoyStick(std::string dev);

	virtual ~CJoyStick();

	void setDev(std::string dev);
	void xferData(char *data, int leng);

	virtual void init();
	virtual void resetJoyStick();
	virtual void resetData();
	virtual int sendReport();
	virtual void close();

	virtual void button(std::string button, int press);
	virtual void axis(int val);
	virtual void aaxis(int lx, int ly, int rx, int ry);
};

#endif
