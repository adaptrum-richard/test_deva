#include <stdio.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

int load_lua(const char *lua_code)
{
	int result = -1;
	if(!lua_code)
		return -1;
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	if(luaL_loadfile(L, lua_code) || lua_pcall(L, 0,0,0)){
   		printf("error %s\n", lua_tostring(L,-1));
    		return -1;
  	}
	lua_getglobal(L,"result");
	result = lua_tointeger(L,-1);
	lua_close(L);
	return result;
}

int main() 
{
	char *t1 = "test1.lua";
	printf("%s result = %d\n", t1, load_lua(t1));
	return 0;
}
