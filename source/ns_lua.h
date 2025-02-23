#include <lua.hpp>

namespace lua
{
  class state
  {
  public:
    ~state();
    void create();
    void close();
    lua_State* operator->();
    operator lua_State*();
    
  private:
    lua_State* m_state = nullptr;
  };
};
