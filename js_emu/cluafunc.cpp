#include "clua.h"
#include <typeinfo>
#include "luna_four_code.h"
#include "js.h"

#include <iostream>
#include <sstream>
#include <string>
#include <exception>

#define VAR_EVENT_MODE "__var_event_mode__"
#define VAR_TERMINATE_LUA "__var_terminate_lua__"

static const struct luaL_Reg override_func[] = {
	{ "print", CLua::luaPrint },
	{ nullptr, nullptr },
};

static const struct luaL_Reg additional_func[] = {
	{ "run_thread",			CLua::luaRunThread },
	{ "wait",				CLua::luaSleep },
	{ "js_button", 			CLua::luaSendButton },
	{ "js_direct", 			CLua::luaSendDirect },
	{ "js_analog", 			CLua::luaSendAnalog },
	{ nullptr,				nullptr },
};

struct SConst {
    const char *name;
	int value;
};

static const struct SConst const_data[] = {
	{ "UP",					0x00 },
	{ "UPRIGHT",			0x01 },
	{ "RIGHT",				0x02 },
	{ "DOWNRIGHT",			0x03 },
	{ "DOWN",				0x04 },
	{ "DOWNLEFT",			0x05 },
	{ "LEFT",				0x06 },
	{ "UPLEFT",				0x07 },
	{ "NONE",				0x08 },
	{ nullptr, 				0x00 },
};

void CLua::registerConst() {
	if (nullptr == mLua) return;

	for (int i=0; nullptr != const_data[i].name; i++) {
		lua_pushnumber(mLua, const_data[i].value);
		lua_setglobal(mLua, const_data[i].name);
	}
}

void CLua::registerFunc() {
	if (nullptr == mLua) return;

	for (int i=0; nullptr != additional_func[i].func; i++) {
		lua_pushcfunction(mLua, additional_func[i].func);
		lua_setglobal(mLua, additional_func[i].name);
	}
}

void CLua::registerOverrideFunc() {
	if (nullptr == mLua) return;

	for (int i=0; nullptr != override_func[i].func; i++) {
		lua_pushcfunction(mLua, override_func[i].func);
		lua_setglobal(mLua, override_func[i].name);
	}
}

int CLua::luaPrint(lua_State *L) {
	if (nullptr == L) return 0;

	CLua *pLua = nullptr;
	CLua **p;
	std::stringstream s;
	lua_getglobal(L, GLOBAL_LUAOBJ);

	for (int i=1; i<=lua_gettop(L); i++) {
		switch (lua_type(L, i)) {
			case LUA_TUSERDATA:
				p=reinterpret_cast<CLua**>(lua_touserdata(L, i));
				if (nullptr != p && typeid(CLua) == typeid(**p))
					pLua=*p;
				break;
			case LUA_TSTRING:
				s << const_cast<char*>(lua_tostring(L, i));
				break;
			case LUA_TNUMBER:
				s << lua_tointeger(L, i);
				break;
			default:
				break;
		}
	}

	std::cout << s.str() << std::endl;
	if (pLua) {
		pLua->setResult(s.str());
	}

	return 0;
}

int CLua::luaRunThread(lua_State *L) {
	if (nullptr == L) return 0;

	int nArgNum=lua_gettop(L);
	if (0 >= nArgNum) {
		luaL_error(L, "zero argument number for run_thread function\n");
		return 0;
	}

	char *szFunc=const_cast<char*>(lua_tostring(L, -1));
	if (nullptr == szFunc || 0 == strlen(szFunc)) {
		std::cout << "function name is null" << std::endl;
		return 0;
	}

	std::string szBuf = szFunc;

	lua_getglobal(L, GLOBAL_LUAOBJ);
	CLua *pLua = nullptr;
	for (int i=0; i<=lua_gettop(L); i++) {
		if (LUA_TUSERDATA == lua_type(L, i)) {
			CLua **p=reinterpret_cast<CLua**>(lua_touserdata(L, i));
			if (nullptr != p && typeid(CLua) == typeid(**p))
				pLua=*p;
			break;
		}
	}

	if (nullptr == pLua) return 0;

	std::map<std::string, std::thread*>::iterator pThread=pLua->m_mapLuaThread.find(szBuf);
	if (pThread == pLua->m_mapLuaThread.end()) {
		std::thread *t = new std::thread([](lua_State *L, char *func, CLua *pLua)->void{
			if (nullptr == L || nullptr == func) return;

			lua_getglobal(L, func);
			lua_call(L, 0, 0);

			std::map<std::string, std::thread*>::iterator pThread=pLua->m_mapLuaThread.find(func);
			if (pLua->m_mapLuaThread.end() != pThread) {
				delete pThread->second;
				pLua->m_mapLuaThread.erase(pThread);
			}
		}, L, szFunc, pLua);

		pLua->m_mapLuaThread[szBuf] = t;

	}

	return 0;
}

int CLua::luaSleep(lua_State *L) {
	if (nullptr == L) return 0;

	lua_gettop(L);
	int nmSec=lua_tointeger(L, -1);

	std::this_thread::sleep_for(std::chrono::milliseconds(nmSec));
	return 0;
}

int CLua::luaSendButton(lua_State *L) {
	if (nullptr == L) return 0;

	lua_getglobal(L, GLOBAL_LUAOBJ);
	CLua *pLua = nullptr;
	std::string szButtons;
	int press = 0;
	for (int i=0; i<=lua_gettop(L); i++) {
		switch(lua_type(L, i)) {
			case LUA_TUSERDATA: {
				CLua **p=reinterpret_cast<CLua**>(lua_touserdata(L, i));
				if (nullptr != p && typeid(CLua) == typeid(**p))
					pLua=*p;
			} break;
			case LUA_TSTRING:
				szButtons = const_cast<char*>(lua_tostring(L, i));
				press = lua_tointeger(L, i + 1);
				break;
		}
	}

	if (nullptr == pLua) {
		return 1;
	}

	CJoyStick *js = reinterpret_cast<CJoyStick*>(pLua->m_JoyStick);
	if (nullptr == js || typeid(CJoyStick) != typeid(*js)) {
		std::cout << "joystick object error" << std::endl;
		return 1;
	}

	js->button(szButtons, press);
	js->sendReport();

	return 0;
}

int CLua::luaSendDirect(lua_State *L) {
	if (nullptr == L) return 0;

	lua_getglobal(L, GLOBAL_LUAOBJ);
	CLua *pLua = nullptr;
	int direction = 0;
	for (int i=0; i<=lua_gettop(L); i++) {
		switch(lua_type(L, i)) {
			case LUA_TUSERDATA: {
				CLua **p=reinterpret_cast<CLua**>(lua_touserdata(L, i));
				if (nullptr != p && typeid(CLua) == typeid(**p))
					pLua=*p;
			} break;
			case LUA_TNUMBER:
				direction = lua_tointeger(L, i);
				break;
		}
	}

	if (nullptr == pLua) {
		return 1;
	}

	CJoyStick *js = reinterpret_cast<CJoyStick*>(pLua->m_JoyStick);
	if (nullptr == js || typeid(CJoyStick) != typeid(*js)) {
		std::cout << "joystick object error" << std::endl;
		return 1;
	}

	js->axis(direction);
	js->sendReport();

	return 0;
}

int CLua::luaSendAnalog(lua_State *L) {
	if (nullptr == L) return 0;

	lua_getglobal(L, GLOBAL_LUAOBJ);
	CLua *pLua = nullptr;
	int axis[4] = {0};
	int nIndex = 0;
	for (int i=0; i<=lua_gettop(L); i++) {
		switch(lua_type(L, i)) {
			case LUA_TUSERDATA: {
				CLua **p=reinterpret_cast<CLua**>(lua_touserdata(L, i));
				if (nullptr != p && typeid(CLua) == typeid(**p))
					pLua=*p;
			} break;
			case LUA_TNUMBER:
				axis[nIndex++] = lua_tointeger(L, i);
				break;
		}
	}

	if (nullptr == pLua) {
		return 1;
	}

	CJoyStick *js = reinterpret_cast<CJoyStick*>(pLua->m_JoyStick);
	if (nullptr == js || typeid(CJoyStick) != typeid(*js)) {
		std::cout << "joystick object error" << std::endl;
		return 1;
	}

	js->aaxis(axis[0], axis[1], axis[2], axis[3]);
	js->sendReport();

	return 0;
}
