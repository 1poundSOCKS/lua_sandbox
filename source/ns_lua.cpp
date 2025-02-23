#include "ns_lua.h"

lua::state::~state()
{
  close();
}

void lua::state::create()
{
  close();
  m_state = luaL_newstate();
}

void lua::state::close()
{
  if( m_state )
  {
    lua_close(m_state);
    m_state = nullptr;
  }
}

lua_State* lua::state::operator->()
{
  return m_state;
}

lua::state::operator lua_State*()
{
  return m_state;
}
