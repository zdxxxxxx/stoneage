#include <string.h>
#include "common.h"
#include "version.h"
#include "profession_skill.h"
#include "mylua/base.h"
#include "char.h"
#include "item.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "lssproto_serv.h"

#ifdef _PROFESSION_SKILL			// WON ADD ����ְҵ����
#ifdef _ALLBLUES_LUA_1_8 
#include "mylua/mylua.h"
extern MY_Lua MYLua;

static CharBase ProfessionSkillBaseInt[] = {
	{{"���"},							PROFESSION_SKILL_ID}
	,{{"ְҵ"},							PROFESSION_SKILL_PROFESSION_CLASS}
	,{{"Ŀ��"},							PROFESSION_SKILL_TARGET}
	,{{"�ķ�MP"},						PROFESSION_SKILL_COST_MP}
	,{{"���"},							PROFESSION_SKILL_USE_FLAG}
	,{{"����"},							PROFESSION_SKILL_KIND}
	,{{"ͼ��"},							PROFESSION_SKILL_ICON}
	,{{"����ǰͼ��"},				PROFESSION_SKILL_IMG_1}
	,{{"����ͼ��"},					PROFESSION_SKILL_IMG_2}
	,{{"���"},							PROFESSION_SKILL_COST}
	,{{"������ֵ"},					PROFESSION_SKILL_FIX_VALUE}
	,{{"����1"},						PROFESSION_SKILL_LIMIT1}
	,{{"����������1"},			PROFESSION_SKILL_PERCENT1}
	,{{"����2"},						PROFESSION_SKILL_LIMIT2}
	,{{"����������2"},			PROFESSION_SKILL_PERCENT2}
	,{{"����3"},						PROFESSION_SKILL_LIMIT3}
	,{{"����������3"},			PROFESSION_SKILL_PERCENT3}
	,{{"����4"},						PROFESSION_SKILL_LIMIT4}
	,{{"����������4"},			PROFESSION_SKILL_PERCENT4}
}; 

static CharBase ProfessionSkillBaseChar[] = {
	{{"����"},					PROFESSION_SKILL_NAME}
	,{{"ע��"},					PROFESSION_SKILL_TXT}
	,{{"����"},					PROFESSION_SKILL_FUNCNAME}
	,{{"ѡ��"},					PROFESSION_SKILL_OPTION}
}; 

static int getInt(lua_State *L) 
{
	const int index = luaL_checkint(L, 1);
	const int element = getCharBaseValue(L, 2, ProfessionSkillBaseInt, arraysizeof(ProfessionSkillBaseInt));

	lua_pushinteger(L, PROFESSION_SKILL_getInt(index, element));
	return 1;
}

static int setInt(lua_State *L) 
{
	const int index = luaL_checkint(L, 1);
	const int element = getCharBaseValue(L, 2, ProfessionSkillBaseInt, arraysizeof(ProfessionSkillBaseInt));
	const int data = luaL_checkint(L, 3);
	
  if(PROFESSION_SKILL_setInt(index, element, data) == -1){
  	return 0;
  }else{
  	return 1;
  }
}

static int getChar(lua_State *L) 
{
	const int index = luaL_checkint(L, 1);
	const int element = getCharBaseValue(L, 2, ProfessionSkillBaseChar, arraysizeof(ProfessionSkillBaseChar));

	lua_pushstring(L, PROFESSION_SKILL_getChar(index, element));
	return 1;
}

static int setChar(lua_State *L) 
{
	size_t l;
	const int index = luaL_checkint(L, 1);
	const int element = getCharBaseValue(L, 2, ProfessionSkillBaseChar, arraysizeof(ProfessionSkillBaseChar));
	char *data = luaL_checklstring(L, 3, &l);
	
  if(PROFESSION_SKILL_setChar(index, element, data) == -1){
  	return 0;
  }else{
  	return 1;
  }
}

static int getProfessionskillArray(lua_State *L) 
{
	const int index = luaL_checkint(L, 1);

	lua_pushinteger(L, PROFESSION_SKILL_getskillArray(index));
	return 1;
}

static const luaL_Reg Professionskilllib[] = {
	{"getInt", 									getInt},
	{"setInt", 									setInt},
	{"getChar", 								getChar},
	{"setChar", 								setChar},
	{"getProfessionskillArray", getProfessionskillArray},
  {NULL, 						NULL}
};

LUALIB_API int luaopen_ProfessionSkill (lua_State *L) {
  luaL_register(L, "Professionskill", Professionskilllib);
  return 1;
}
#endif
#endif

