#include "clua.h"
#include "luafunc.h"
#include <fstream>

CLua::CLua()
{
	mLua = nullptr;
	init();
}

CLua::~CLua() {
	close();
}

void CLua::init() {
	if (NULL == mLua) {
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
	if (mLua) {
		lua_close(mLua);
		mLua = nullptr;
	}
}

void CLua::setResult(std::string szResult) {
	m_szPrintOut = szResult;
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
}

void CLua::runFile(std::string szFile) {
	if (NULL == mLua) return;
	if (szFile.empyt()) return;
	std::ifstream inFile;
	inFile.open(szFile);
	if(inFile.good()) return;

	m_szFile=szFile;

	adjustLuaPath();
	if (luaL_loadfile(mLua, m_szFile.c_str()) {
		return;
	}

	if (lua_pcall(mLua, 0, LUA_MULTRET, 0)) {
		return;
	}

}

void CLua::runString(std::string szScript) {
	if (NULL == mLua) return;
	if (szScript.empty()) return;

	luaL_dostring(mLua, szScript.c_str());
}
