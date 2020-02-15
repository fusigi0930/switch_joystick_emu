#ifndef __CMD_H__
#define __CMD_H__

#include <string>
#include <sstream>
#include <map>

#define PARAM_CMD	"cmd"

enum PARAM_VALUE {
	VALUE_RUN	=	0x1000,
	VALUE_FILE,
	VALUE_STOP,
	VALUE_QUIT,
	VALUE_DISCONNECT,
};

class CCommand {
protected:
	std::stringstream m_stream;
	std::stringstream m_res;
	std::map<std::string, int> m_mapParam;
	std::map<std::string, std::string> m_mapAppendParam;

public:
	CCommand();
	virtual ~CCommand();

	void command(std::string cmd);
	int getParam(std::string param);
	std::string getResponse();
	std::string getAppendParam(std::string param);
	void setParam(std::string param, int value);
	void setAppendParam(std::string param, std::string value);
	void clearStream();
};

#endif
