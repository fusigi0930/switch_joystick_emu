#include "clua.h"
#include <fstream>
#include <sstream>
#include <iostream>

CLua::CLua() {
	mLua = nullptr;
	m_JoyStick = nullptr;
	m_isRunning = 0;
	init();
}

CLua::~CLua() {
	m_JoyStick = nullptr;
	close();
}

void CLua::init() {
	if (nullptr == mLua) {
		mLua = luaL_newstate();
	}
	luaL_openlibs(mLua);
	registerOverrideFunc();
	registerFunc();
	registerConst();

	CLua **pUserdata = reinterpret_cast<CLua**>(lua_newuserdata(mLua, sizeof(CLua*)));
	*pUserdata = this;
	lua_setglobal(mLua, GLOBAL_LUAOBJ);
}

void CLua::close() {
	if (1 == m_isRunning) {
		stop();
	}

	m_runMutex.lock();
	m_runMutex.unlock();

	if (mLua) {
		lua_close(mLua);
		mLua = nullptr;
	}
}

void CLua::setResult(std::string szResult) {
	m_szPrintOut = szResult;
}

void CLua::setJoyStick(void *js) {
	m_JoyStick = js;
}

std::string CLua::getResult() {
	std::string ret = m_szPrintOut;
	m_szPrintOut.clear();
	return ret;
}

void CLua::adjustLuaPath() {
	// compare path environment
	lua_getglobal(mLua, "package");
	lua_getfield(mLua, -1, "path");
	std::string szEnvPath=lua_tostring(mLua, -1);
	size_t pos = m_szFile.rfind('/');
	std::string path = m_szFile.substr(0, pos);

	if (std::string::npos == szEnvPath.find(path)) {
		std::stringstream s;
		s << szEnvPath << ";" << path << "/?.lua;";
		lua_pop(mLua, 1);
		lua_pushstring(mLua, s.str().c_str());
		lua_setfield(mLua, -2, "path");
		lua_pop(mLua, 1);
	}
}

void CLua::runFile(std::string szFile) {
	if (nullptr == mLua) return;
	if (szFile.empty()) return;
	std::ifstream inFile;
	inFile.open(szFile);
	if(!inFile.good()) return;

	m_szFile=szFile;

	adjustLuaPath();
	if (luaL_loadfile(mLua, m_szFile.c_str())) {
		return;
	}

	m_isRunning = 1;
	m_runMutex.lock();

	lua_pcall(mLua, 0, LUA_MULTRET, 0);

	m_isRunning = 0;
	m_runMutex.unlock();

}

void CLua::runString(std::string szScript) {
	if (nullptr == mLua) return;
	if (szScript.empty()) return;

	m_isRunning = 1;
	m_runMutex.lock();

	luaL_dostring(mLua, szScript.c_str());

	m_isRunning = 0;
	m_runMutex.unlock();
}

static void hookCode(lua_State *L, lua_Debug *ar) {
	std::cout << "try to generate error" << std::endl;
	lua_error(L);
}

void CLua::stop() {
	if (nullptr == mLua) return;
	if (1 == m_isRunning) {
		lua_sethook(mLua, hookCode, LUA_HOOKLINE, 0);
	}
}
