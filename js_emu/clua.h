#ifndef __CLUA_H__
#define __CLUA_H__
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include <string>
#include <map>
#include <vector>
#include <thread>
#include <mutex>

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
	std::map<std::string, std::thread*> m_mapLuaThread;
	void *m_JoyStick;
	int m_isRunning;
	std::mutex m_runMutex;

public:
	CLua();
	virtual ~CLua();

	void init();
	void close();
	void setResult(std::string szResult);
	void setJoyStick(void *js);

	void runString(std::string szScript);
	void runFile(std::string szFile);
	void stop();
	std::string getResult();

	// override original Lua function
	static int luaPrint(lua_State* L);

	// additional functions
	static int luaRunThread(lua_State *L);
	static int luaSleep(lua_State *L);
	static int luaSendButton(lua_State *L);
	static int luaSendDirect(lua_State *L);
	static int luaSendAnalog(lua_State *L);

};

#endif
