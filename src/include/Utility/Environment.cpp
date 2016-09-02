#include <Environment.hpp>

#include <Lua.hpp>

#include <iostream>

#include <Debug.hpp>

void reportErrors(lua_State *L, int code)
{
    if(code)
    {
        std::cout << "Lua error: " << lua_tostring(L, code) << std::endl;
        lua_pop(L, 1); //remove error
    }
}

void scanLibrary()
{
    lua_State *L = lua_open();
 
    // load the libs
    luaL_openlibs(L);

    //run a Lua script here
    reportErrors(L, luaL_dofile(L, "scripts/dirtree.lua"));
    reportErrors(L, luaL_dofile(L, "scripts/scan_images.lua"));
    reportErrors(L, luaL_dofile(L, "scripts/scan_library.lua"));
 
    lua_close(L);
}
