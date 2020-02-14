#ifndef __CLUA_H__
#define __CLUA_H__
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <string>
#include <map>
#include <vector>
#include <thread>

#define GLOBAL_LUAOBJ "__LUA_OBJECT__"

class CLua
{
private:
	void registerOverrideFunc();
	void registerFunc();
	void registerConst();
	void adjustLuaPath();

protected:
	lua_State *mLua;
	std::string m_szPrintOut;
	std::string m_szFile;
	std::map<QString, QThread*> m_mapLuaThread;

public:
	CLua();
	virtual ~CLua();

	void init();
	void close();
	void setResult(std::string szResult);

	void runString(std::string szScript);
	void runFile(std::string szFile);
	std::string getResult();

	// override original Lua function
	static int luaPrint(lua_State* L);

	// additional functions
	static int luaRunThread(lua_State *L);
	static int luaSendEvent(lua_State *L);
	static int luaSetEventMode(lua_State *L);
	static int luaSleep(lua_State *L);

};

#endif
