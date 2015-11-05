#include <stdio.h> 

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

// gcc main.c -I /usr/include/lua5.2 -llua5.2

#define MAX_COLOR 255

int read_number(lua_State *L)
{
    // eg: key = value
    lua_getglobal(L, "width");
    lua_getglobal(L, "height");

    // check value type
    if (!lua_isnumber(L, -2) || !lua_isnumber(L, -1)) {
        printf("width and height should be a number!\n");
        return -1;
    }

    int width = lua_tointeger(L, -2);
    int height = lua_tointeger(L, -1);
    printf( "width: [%d], height: [%d]\n", width, height);

    return 0;
}

int get_color_field(lua_State *L, const char *key)
{
    lua_pushstring(L, key); // push key
    lua_gettable(L, -2); // get background[key]
    // lua_pushstring , lua_gettable == lua_getfield
    // lua_getfield(L, -1, key);

    if (!lua_isnumber(L, -1)) {
        printf("invalid component in background color");
        return -1;
    }
    int result;
    result = (int)(lua_tonumber(L, -1) * MAX_COLOR);
    lua_pop(L, 1); // remove number
    return result;
}

int read_table(lua_State *L)
{
    // eg: k = {k1 = v1, k2 = v2, k3 = v3}
    lua_getglobal(L, "background");
    if (!lua_istable(L, -1)) {
        printf("background is not a table");
        return -1;
    }
    int red = get_color_field(L, "r");
    int green = get_color_field(L, "g");
    int blue = get_color_field(L, "b");
    printf("r: [%d], g: [%d], b: [%d]\n", red, green, blue);
    return 0;
}

double func(lua_State *L, double x, double y)
{
    int isnum;
    double z;

    lua_getglobal(L, "f"); // function to be called
    lua_pushnumber(L, x); // push 1st arg
    lua_pushnumber(L, y); // push 2nd arg

    // do the call
    if (lua_pcall(L, 2, 1, 0) != LUA_OK) {
        printf("run function faled");
        return -1;
    }
    
    // get result
    z = lua_tonumberx(L, -1, &isnum);
    if (!isnum) {
        printf("function f nust return a number");
        return -1;
    }

    lua_pop(L, 1); // pop returned value
    return z;
}

int main ()
{
    const char *fname = "lua.lua";
    /* initialize Lua */
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0)) {
        printf("can't run config, file: [%s]\n", lua_tostring(L, -1));
        return -1;
    }

    // read integer
    read_number(L);

    // read table
    read_table(L);

    // call lua functions
    double res = func(L, 2, 3);
    printf("function return value: [%f]\n", res);
    
    /* cleanup Lua */
    lua_close(L);

    return 0;
}
