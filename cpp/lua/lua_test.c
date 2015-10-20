#include<stdio.h>  

// g++ lua_test.c -I /usr/include/lua5.2 -llua5.2
// ./a.out conf

//如不用extern会出现连接错误，编译成了C++文件  
extern "C"   
{  
#include "lua.h"  
#include "lauxlib.h"  
#include "lualib.h"  
}  

#pragma comment(lib, "lua5.1.lib")  

void ReadVariable (lua_State *L,int *width,int *height){  
    lua_getglobal(L,"width");   //获得Lua中变量的值，将其放入栈中  
    lua_getglobal(L,"height");  

    if(!lua_isnumber(L,-2))     //栈顶为-1，然后依次减少  
        luaL_error(L,"`width' should be a number\n");  

    if(!lua_isnumber(L,-1))  
        luaL_error(L,"`height' should be a number\n");  
    *width = (int)lua_tonumber(L,-2);    //将栈顶元素转化为数字  
    *height = (int)lua_tonumber(L,-1);  
    printf("width is %d ,height is %d\n\n",*width,*height);  
}  

static void ReadTableFromItem(lua_State *L, const char* lpszTableName, const char* lpszTableItem)  
{  
    lua_getglobal(L, lpszTableName);  

    lua_pushstring(L, lpszTableItem);//首先把元素的名字压入栈顶  
    lua_gettable(L, -2);//值会放在栈顶，同时刚才压入的元素名字被弹出  
    printf("%s.%s=%d\n", lpszTableName, lpszTableItem, (int)lua_tonumber(L, -1));  
    lua_pop(L, 2);  
}  

static void ReadTableFromIndex(lua_State *L, const char* lpszTableName, int index)  
{  
    lua_getglobal(L, lpszTableName);  
    lua_rawgeti(L, -1, index);//获得第一个元素  
    printf("%s[%d]=%d\n", lpszTableName, index, (int)lua_tonumber(L, -1));  
    lua_pop(L, 2);  
}  

static void EnumTableItem(lua_State *L, const char* lpszTableName)  
{  
    lua_getglobal(L, lpszTableName);  
    int it = lua_gettop(L);  
    lua_pushnil(L);  
    printf("Enum %s:", lpszTableName);  
    while(lua_next(L, it))//用lua_next可以遍历这个数组  
    {  
        printf("  %d", (int)lua_tonumber(L, -1));  
        lua_pop(L, 1);  
    }  
    printf("\n\n");  
    lua_pop(L, 1);  
}  

int main(int argc, char* argv[])  
{  
    if (argc != 2)  
    {  
        printf("param error!\n");  
        return 0;  
    }  

    char *filename = argv[1];  

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    if(luaL_loadfile(L,filename) || lua_pcall(L,0,0,0)){  
        luaL_error(L,"loadfile error! %s \n",lua_tostring(L,-1));  
    }  

    int w=1, h=2;  
    ReadVariable(L, &w, &h);  

    ReadTableFromItem(L, "luat_Test1", "a");  // 等价与lua代码：print(luat_Test1.a)  
    ReadTableFromItem(L, "luat_Test1", "b");  
    ReadTableFromItem(L, "luat_Test1", "c");  
    EnumTableItem(L, "luat_Test1");    // 枚举Table  

    ReadTableFromIndex(L, "luat_Test2", 1);  // 等价与lua代码：print(luat_Test1[1])  
    ReadTableFromIndex(L, "luat_Test2", 2);  
    ReadTableFromIndex(L, "luat_Test2", 3);  
    EnumTableItem(L, "luat_Test2");  
    return 0;
}
