#include "cmd.h"
#include <algorithm>
#include <vector>

CCommand::CCommand() {

}

CCommand::~CCommand() {

}

#define CMD_RUN				"run"
#define CMD_FILE			"file"
#define CMD_STOP			"stop"
#define CMD_QUIT			"quit"
#define CMD_DISCONNECT		"disconnect"
#define CMD_ACTION			"act"

static std::map<std::string, int> s_mapCmd = {
	{ CMD_RUN,		VALUE_RUN },
	{ CMD_FILE,		VALUE_FILE },
	{ CMD_STOP,		VALUE_STOP },
	{ CMD_QUIT,		VALUE_QUIT },
	{ CMD_DISCONNECT, VALUE_DISCONNECT},
	{ CMD_ACTION,	VALUE_ACTION },
};

static int command_parser(std::string &cmd, std::vector<std::string> &vtCmd) {
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
	std::stringstream s(cmd);
	std::string arg;

	while (std::getline(s, arg, ' ')) {
		vtCmd.push_back(arg);
	}
	if (0 == vtCmd.size()) {
		cmd = "illegel command\n";
		return -1;
	}

	return 0;
}

void CCommand::clearStream() {
	m_stream.str("");
	m_stream.clear();
	m_res.str("");
	m_res.clear();
}

void CCommand::command(std::string cmd) {
	m_stream << cmd;
	std::string szBuf;
	while (std::getline(m_stream, szBuf)) {
		std::vector<std::string> vtCmd;
		if (0 != command_parser(szBuf, vtCmd)) {
			m_res << szBuf;
			continue;
		}

		std::map<std::string, int>::iterator pCmd = s_mapCmd.find(vtCmd[0]);
		if (s_mapCmd.end() == pCmd) {
			m_res << "not support command " + vtCmd[0] + "\n";
			continue;
		}
		setParam(PARAM_CMD, pCmd->second);
		if(vtCmd.size() > 1)
			setAppendParam(PARAM_CMD, szBuf.substr(vtCmd[0].length() + 1));
	}
}

int CCommand::getParam(std::string param) {
	std::map<std::string, int>::iterator pParam = m_mapParam.find(param);
	if (m_mapParam.end() == pParam) {
		return -1;
	}
	return pParam->second;
}

void CCommand::setParam(std::string param, int value) {
	std::map<std::string, int>::iterator pParam = m_mapParam.find(param);
	if (m_mapParam.end() == pParam) {
		m_mapParam[param] = value;
	}
	else {
		pParam->second = value;
	}
}

std::string CCommand::getAppendParam(std::string param) {
	std::map<std::string, std::string>::iterator pParam = m_mapAppendParam.find(param);
	if (m_mapAppendParam.end() == pParam) {
		return "";
	}
	return pParam->second;
}

void CCommand::setAppendParam(std::string param, std::string value) {
	std::map<std::string, std::string>::iterator pParam = m_mapAppendParam.find(param);
	if (m_mapAppendParam.end() == pParam) {
		m_mapAppendParam[param] = value;
	}
	else {
		pParam->second = value;
	}
}

std::string CCommand::getResponse() {
	return m_res.str();
}
