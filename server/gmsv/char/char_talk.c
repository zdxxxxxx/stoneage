#include "version.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "handletime.h"
#include "autil.h"
#include "readmap.h"
#include "object.h"
#include "char.h"
#include "char_base.h"
#include "item_event.h"
#include "chatmagic.h"
#include "battle.h"
#include "log.h"
#include "configfile.h"
#include "lssproto_serv.h"
#include "saacproto_cli.h"
#include "family.h"
#include "buf.h"
#ifdef _CHAR_PROFESSION
#include "profession_skill.h"
#endif
#include "net.h"
#include "char_talk.h"
#ifdef _TALK_MOVE_FLOOR
#include "longzoro/move.h"
#endif
#ifdef _PLAYER_QUESTION_ONLIEN
#include "longzoro/playerquestion.h"
#endif
#ifdef _ALLBLUES_LUA
#include "mylua/function.h"
#endif

extern int channelMember[FAMILY_MAXNUM][FAMILY_MAXCHANNEL][FAMILY_MAXMEMBER];
#ifdef _NEW_ITEM_
extern int CheckCharMaxItem(int charindex);
#endif
#ifdef _CHANNEL_MODIFY
int *piOccChannelMember = NULL;
#endif

#ifdef _TALK_ACTION
void TalkAction(int charaindex, char *message);
#endif

#ifdef _GM_ITEM
static BOOL player_useChatMagic( int charaindex, char* data, BOOL isDebug);
#endif

/*------------------------------------------------------------
 * ����������  ��ĩ����
 ------------------------------------------------------------*/
#define DEBUGCDKEYNUM 100
struct tagDebugCDKey {
	int use;
	char cdkey[9];
};
static struct tagDebugCDKey DebugCDKey[DEBUGCDKEYNUM];

/*====================��������  ܷ====================*/

typedef struct tagCHAR_ChatMagicTable
{
	char			magicname[128];
	CHATMAGICFUNC	func;
	BOOL            isdebug;
	int             hash;
	int							level;
	char*						usestring;
}CHAR_ChatMagicTable;

static CHAR_ChatMagicTable CHAR_cmtbl[]={
	//����ʦר��
	{ "programming_engineer",	CHAR_CHAT_DEBUG_engineer,	TRUE,	0,	3, "" },
	{ "petlevelup",	CHAR_CHAT_DEBUG_petlevelup,	TRUE,	0,	2, "�������� �ȼ� (�˺�)" },
	{ "petexpup",	CHAR_CHAT_DEBUG_petexpup,	TRUE,	0,	2, "�������� ���� (�˺�)" },
	{ "help",			CHAR_CHAT_DEBUG_help,				TRUE,	0,	2, "ָ��/all" },
#ifdef _EQUIT_NEGLECTGUARD
	{ "setneguard",		CHAR_CHAT_DEBUG_setneguard,			TRUE,	0,	3, "waei" },
#endif
	//��������
	{ "info",			CHAR_CHAT_DEBUG_info,			TRUE,	0,	2, "��ֵ" },
	{ "level",			CHAR_CHAT_DEBUG_level,			TRUE,	0,	2, "��ֵ (�˺�)" },
	{ "settrans",		CHAR_CHAT_DEBUG_setTrans,		TRUE,	0,	2, "��ֵ (�˺�)" },
	{ "exp",			CHAR_CHAT_DEBUG_exp,			TRUE,	0,	2, "��ֵ (�˺�)" },
	{ "hp",				CHAR_CHAT_DEBUG_hp,				TRUE,	0,	2, "��ֵ (�˺�)" },
	{ "mp",				CHAR_CHAT_DEBUG_mp,				TRUE,	0,	2, "��ֵ (�˺�)" },
	{ "setmp",			CHAR_CHAT_DEBUG_setmp,			TRUE,	0,	2, "��ֵ (�˺�)" },
	{ "str",			CHAR_CHAT_DEBUG_str,			TRUE,	0,	2, "��ֵ*100 (�˺�)" },
	{ "dex",			CHAR_CHAT_DEBUG_dex,			TRUE,	0,	2, "��ֵ*100 (�˺�)" },
	{ "tgh",			CHAR_CHAT_DEBUG_tgh,			TRUE,	0,	2, "��ֵ*100 (�˺�)" },
	{ "vital",			CHAR_CHAT_DEBUG_vital,			TRUE,	0,	2, "��ֵ*100 (�˺�)" },
	{ "luck",			CHAR_CHAT_DEBUG_luck,			TRUE,	0,	2, "��ֵ (�˺�)" },
	{ "superman",		CHAR_CHAT_DEBUG_superman,		TRUE,	0,	2, "(�˺�)" },
	{ "dp",				CHAR_CHAT_DEBUG_dp,				TRUE,	0,	2, "��ֵ (�˺�)" },
#ifdef _EQUIT_ARRANGE
	{ "arrange",		CHAR_CHAT_DEBUG_arrange,		TRUE,	0,	3, "��ֵ" },
#endif

#ifdef _EQUIT_SEQUENCE
	{ "sequence",		CHAR_CHAT_DEBUG_sequence,		TRUE,	0,	3, "��ֵ" },
#endif

	//ϵͳ
	{ "announce",		CHAR_CHAT_DEBUG_announce,		TRUE,	0,	2, "����" },
	{ "loginannounce",	CHAR_CHAT_DEBUG_loginannounce,	TRUE,	0,	2, "����" },
	{ "sysinfo",		CHAR_CHAT_DEBUG_sysinfo,		TRUE,	0,	2, "" },
	{ "effect",			CHAR_CHAT_DEBUG_effect,			TRUE,	0,	2, "alloff/��ͼ�� ��Ч" },
	{ "reset",			CHAR_CHAT_DEBUG_reset,			TRUE,	0,	3, "enemy/encount/magic/warppoint/petskill/pettalk/npc/all" },
	{ "clean_floor",	CHAR_CHAT_DEBUG_cleanfloor,		TRUE,	0,	3, "��ͼ��" },
	{ "printcount",		CHAR_CHAT_printcount,			TRUE,	0,	2, "" },
	{ "enemyrestart",	CHAR_CHAT_DEBUG_enemyrestart,	TRUE,	0,	3, "��" },
	{ "cleanfreepet",	CHAR_CHAT_DEBUG_cleanfreepet,	TRUE,	0,	3, "��" },
	
#ifdef _GMRELOAD
	{ "gmreload",		CHAR_CHAT_DEBUG_gmreload,		TRUE,	0,	3, "all/cdkey level" },
#endif

	{ "waeikickall",	CHAR_CHAT_DEBUG_waeikickall,	TRUE,	0,	3, "��" },
	{ "checktrade",		CHAR_CHAT_DEBUG_checktrade,		TRUE,	0,	3, "waei" },
	//����	����GM
	{ "debug",			CHAR_CHAT_DEBUG_debug,			TRUE,	0,	1, "on/off" },
	{ "metamo",			CHAR_CHAT_DEBUG_metamo,			TRUE,	0,	1, "����ͼ�� (�˺�)" },
	{ "checklock",		CHAR_CHAT_DEBUG_checklock,		TRUE,	0,	2, "�ʺ�" },
	{ "shutup",			CHAR_CHAT_DEBUG_shutup,			TRUE,	0,	2, "�ʺ� ON/OFF" },
#ifdef _WAEI_KICK
	{ "gmkick",			CHAR_CHAT_DEBUG_gmkick,			TRUE,	0,	1, "�ʺ� LSLOCK/KICK/DEUNLOCK/UNLOCKALL/LOCK/TYPE/UNLOCK" },
#endif
  { "battlein",		CHAR_CHAT_DEBUG_battlein,		TRUE,	0,	2, "��" },
  { "battleout",		CHAR_CHAT_DEBUG_battleout,		TRUE,	0,	2, "��" },
	{ "battlewatch",	CHAR_CHAT_DEBUG_battlewatch,	TRUE,	0,	2, "��" },
	{ "getuser",		CHAR_CHAT_DEBUG_getuser,		TRUE,	0,	2, "������ ��ͼ�� (npc)" },
	{ "warp",			CHAR_CHAT_DEBUG_warp,			TRUE,	0,	2, "��ͼ�� x y" },
	{ "waeikick",		CHAR_CHAT_DEBUG_waeikick,		TRUE,	0,	2, "�ʺ�" },
  { "jail",			CHAR_CHAT_DEBUG_jail,			TRUE,	0,	2, "�ʺ�" },
	{ "send",			CHAR_CHAT_DEBUG_send,			TRUE,	0,	1, "floor x y �ʺ�" },
#ifdef _SendTo
	{ "sendto",			CHAR_CHAT_DEBUG_Sendto,			TRUE,	0,	1, "�ʺ�" },
#endif
	{ "noenemy",		CHAR_CHAT_DEBUG_noenemy,		TRUE,	0,	2, "on/off" },
	{ "watchevent",		CHAR_CHAT_DEBUG_watchevent,		TRUE,	0,	2, "��" },
#ifdef _SEND_EFFECT	   	        // WON ADD AC����ѩ���������Ч
	{ "sendeffect",		CHAR_CHAT_DEBUG_sendeffect,		TRUE,	0,	2, "" },
#endif
	{ "silent",			CHAR_CHAT_DEBUG_silent,			TRUE,	0,	2, "�ʺ� ����" },//����
	{ "unlock",			CHAR_CHAT_DEBUG_unlock,			TRUE,	0,	2, "�ʺ�" },
	{ "eventclean",		CHAR_CHAT_DEBUG_eventclean,		TRUE,	0,	2, "all/��� �ʺ� ������" },
	{ "eventsetend",	CHAR_CHAT_DEBUG_eventsetend,	TRUE,	0,	2, "all/��� �ʺ� ������" },
	{ "eventsetnow",	CHAR_CHAT_DEBUG_eventsetnow,	TRUE,	0,	2, "all/��� �ʺ� ������" },

	{ "playerspread",	CHAR_CHAT_DEBUG_playerspread,	TRUE,	0,	3, "waei" },
	{ "shutupall",		CHAR_CHAT_DEBUG_shutupall,		TRUE,	0,	3, "��" },
	{ "unlockserver",	CHAR_CHAT_DEBUG_unlockserver,	TRUE,	0,	3, "��" },
  { "shutdown",		CHAR_CHAT_DEBUG_shutdown,		TRUE,	0,	3, "(����)" },

	//�Ƴ�
	{ "delitem",		CHAR_CHAT_DEBUG_delitem,		TRUE,	0,	2, "all/λ��" },
	{ "delpet",			CHAR_CHAT_DEBUG_deletepet,		TRUE,	0,	2, "all/λ��" },
#if _ATTESTAION_ID != 3 
	{ "additem",		CHAR_CHAT_DEBUG_additem,		TRUE,	0,	3, "����ID ((����) (�˺�))" },
	{ "petmake",		CHAR_CHAT_DEBUG_petmake,		TRUE,	0,	3, "����ID ((�ȼ�) (�˺�))" },
#endif
	{ "gold",			CHAR_CHAT_DEBUG_gold,			TRUE,	0,	2, "���� (�˺�)" },

	//���幤��
	{ "manorpk",		CHAR_CHAT_DEBUG_manorpk,		TRUE,	0,	2, "allpeace/peace ׯ԰���" },
	{ "fixfmleader",	CHAR_CHAT_DEBUG_fixfmleader,	TRUE,	0,	2, "�ʺ� 1" },
	{ "fixfmpk",		CHAR_CHAT_DEBUG_fixfmpk,		TRUE,	0,	2, "" },
	{ "fixfmdata",		CHAR_CHAT_DEBUG_fixfmdata,		TRUE,	0,	2, "" },
  
	//������


#ifdef _CHAR_POOLITEM
	{ "saveditem",		CHAR_CHAT_DEBUG_saveditem,		TRUE,	0,	3, "" },
	{ "insertditem",	CHAR_CHAT_DEBUG_insertditem,	TRUE,	0,	3, "" },
	{ "showdepot",		CHAR_CHAT_DEBUG_ShowMyDepotItems,	TRUE,	0,	3, "" },
	{ "insidedepot",	CHAR_CHAT_DEBUG_InSideMyDepotItems,	TRUE,	0,	3, "" },
#endif

#ifdef _CHAR_POOLPET
	{ "savedpet",		CHAR_CHAT_DEBUG_savedpet,		TRUE,	0,	3, "" },
	{ "insertdpet",		CHAR_CHAT_DEBUG_insertdpet,		TRUE,	0,	3, "" },
	{ "showdepotpet",	CHAR_CHAT_DEBUG_ShowMyDepotPets,	TRUE,	0,	3, "" },
	{ "insidedepotpet",	CHAR_CHAT_DEBUG_InSideMyDepotPets,	TRUE,	0,	3, "" },
#endif

#if _ATTESTAION_ID != 3
#ifdef _TEST_DROPITEMS
	{ "dropmypet",		CHAR_CHAT_DEBUG_dropmypet,		TRUE,	0,	3, "������" },
	{ "dropmyitem",		CHAR_CHAT_DEBUG_dropmyitem,		TRUE,	0,	2, "���߱��/(0/1)" },
#endif
#endif
#ifdef _CHAR_PROFESSION			// WON ADD ����ְҵ
	{ "addsk",			CHAR_CHAT_DEBUG_addsk,			TRUE,	0,	2, "" },
	{ "delsk",			CHAR_CHAT_DEBUG_delsk,			TRUE,	0,	2, "" },
#endif
	{ "checktime",		CHAR_CHAT_DEBUG_checktime,		TRUE,	0,	3, "" },

#ifdef _GAMBLE_BANK
	{ "setgamblenum",	CHAR_CHAT_DEBUG_setgamblenum,	TRUE,	0,	3, "��ֵ" },
#endif
    // WON ADD ����ָ��
	{ "crash",			CHAR_CHAT_DEBUG_crash,			TRUE,	0,	3, "" },
#ifdef _PETSKILL_SETDUCK
	{ "setDuck",		CHAR_CHAT_DEBUG_SetDuck,		TRUE,	0,	3, "" },
#endif

#ifdef _TYPE_TOXICATION
	{ "setTox",			CHAR_CHAT_DEBUG_Toxication,		TRUE,	0,	3, "" },
#endif
	{ "showMem",		CHAR_CHAT_DEBUG_showMem,		TRUE,	0,	2, "��" },
#ifdef _CHAR_PROFESSION			// WON ADD ����ְҵ
	{ "profession",		CHAR_CHAT_DEBUG_show_profession,TRUE,	0,	2, "" },
	{ "set_regist",		CHAR_CHAT_DEBUG_set_regist,		TRUE,	0,  1, "" },
#endif
	{ "samecode",		CHAR_CHAT_DEBUG_samecode,		TRUE,	0,	3, "pet/item/set ����" },

#ifdef _NEW_MANOR_LAW
	// �趨��������
	{ "set_momentum",CHAR_CHAT_DEBUG_set_momentum,TRUE,0,2,"���� ��ֵ" },
#endif
	// �趨����ӵ��ׯ԰
	{ "set_manor_owner",CHAR_CHAT_DEBUG_set_manor_owner,TRUE,0,2,"ׯ԰ID 0/1 ����ID" },
	// �趨ׯ԰��սʱ��,�趨��ʱ��ΪĿǰÿ����սʱ�ڵľ���ʱ��(��λ:��),ex:��������ս��,����Ҫ��������Ż��
	// ����ս��,����ָ���趨Ϊ 5 ,���������ĵȴ��ڱ����ֻ�������
	{ "set_schedule_time",CHAR_CHAT_DEBUG_set_schedule_time,TRUE,0,2,"ׯ԰ID ����" },


#ifdef _ANGEL_SUMMON
	{ "angelinfo",		CHAR_CHAT_DEBUG_angelinfo,		TRUE,   0,  2, "" },
	{ "angelclean",		CHAR_CHAT_DEBUG_angelclean,		TRUE,   0,  2, "������" },
	{ "angelcreate",	CHAR_CHAT_DEBUG_angelcreate,	TRUE,   0,  2, "" },
	{ "missionreload",	CHAR_CHAT_DEBUG_missionreload,	TRUE,   0,  2, "" },
#endif

	{ "itemreload",	CHAR_CHAT_DEBUG_itemreload,	TRUE,   0,  2, "" },

	{ "skywalker",	CHAR_CHAT_DEBUG_skywalker,	TRUE,   0,  2, "" },
#ifdef _ITEM_ADDEXP
	{ "itemaddexp",	CHAR_CHAT_DEBUG_itemaddexp,	TRUE,   0,  2, "" },
#endif

#ifdef _DEF_GETYOU	  
	{ "getyou",		    CHAR_CHAT_DEBUG_getyou,			TRUE,   0,  2, "���� 1~3" },
#endif 
#ifdef _DEF_NEWSEND
	{ "newsend",        CHAR_CHAT_DEBUG_newsend,        TRUE,   0,  2, "floor x y �ʺ� ��/Ҫ˵�Ļ�" },
#endif
#ifdef _DEF_SUPERSEND
	{ "supersend",      CHAR_CHAT_DEBUG_supersend,      TRUE,   0,  3, "floor x y ���� ��/Ҫ˵�Ļ�" },
#endif
#ifdef _FONT_SIZE
	{ "fsize",		CHAR_CHAT_DEBUG_fsize,	TRUE,   0,  1, "�ֺ�" },
#endif
#ifdef _JOBDAILY
	{ "rejobdaily",		CHAR_CHAT_DEBUG_rejobdaily,     TRUE,   0,  3, "��" },
#endif
#ifdef _CREATE_MM_1_2
	{ "MM",		CHAR_CHAT_DEBUG_MM,      TRUE,   0,  3, "MM 1/2 (�˺�)" },
#endif
#ifdef _GM_ITEM
	{ "gmfunction",		CHAR_CHAT_DEBUG_GMFUNCTION,      TRUE,   0,  3, "���� ���� (�˺�)" },
#endif

#ifdef _GM_RIDE
	{ "setride",		CHAR_CHAT_DEBUG_SETRIDE,      TRUE,   0,  3, "���� (�˺�)" },
	{ "mvride",		CHAR_CHAT_DEBUG_MVRIDE,      TRUE,   0,  3, "���� (�˺�)" },
#endif

#ifdef _LOCK_IP
	{ "addlock",		CHAR_CHAT_DEBUG_ADD_LOCK,      TRUE,   0,  3, "flag �˺�/IP IP" },
	{ "dellock",		CHAR_CHAT_DEBUG_DEL_LOCK,      TRUE,   0,  3, "flag �˺�/IP IP" },
	{ "showip",		CHAR_CHAT_DEBUG_SHOWIP,      TRUE,   0,  3, "�˺�" },
#endif	
	{ "setfame",		CHAR_CHAT_DEBUG_SET_FAME,      TRUE,   0,  3, "�˺� ����ֵ" },
	
#ifdef _AUTO_PK
	{ "pktime",		CHAR_CHAT_DEBUG_SET_AUTOPK,      TRUE,   0,  3, "����" },
#endif

#ifdef _PLAYER_NUM
	{ "playernum",		CHAR_CHAT_DEBUG_SET_PLAYERNUM,      TRUE,   0,  3, "����" },
#endif

#ifdef _RELOAD_CF
	{ "reloadcf",		CHAR_CHAT_DEBUG_SET_RELOADCF,      TRUE,   0,  3, "" },
#endif

#ifdef _TRANS
	{ "trans",		CHAR_CHAT_DEBUG_Trans,      TRUE,   0,  3, "(�˺�)" },
#endif
#ifdef _FUSIONBEIT_FIX
	{ "fusionbeit",		CHAR_CHAT_DEBUG_fusionbeit,      TRUE,   0,  3, "�������� (�˺�)" },
#endif
#ifdef _MAKE_PET_CF
	{ "petmakecf",		CHAR_CHAT_DEBUG_petmakecf,      TRUE,   0,  3, "������ ת��/�ȼ�/�ɳ�" },
#endif
#ifdef _MAKE_PET_ABILITY
	{ "petmakeabi",		CHAR_CHAT_DEBUG_petmakeabi,      TRUE,   0,  3, "������ ת��/�ȼ�/�ɳ�" },
#endif
#ifdef _MAKE_MAP
	{ "delmap",		CHAR_CHAT_DelMap,      TRUE,   0,  3, "" },
	{ "getmap",		CHAR_CHAT_GetMap,      TRUE,   0,  3, "" },
	{ "map",		CHAR_CHAT_Map,      TRUE,   0,  3, "" },
	{ "tile",		CHAR_CHAT_Fixtile,      TRUE,   0,  3, "" },
	{ "obj",		CHAR_CHAT_Fixobj,      TRUE,   0,  3, "" },
//	{ "fukuwa",		CHAR_CHAT_Fukuwa,      TRUE,   0,  3, "" },
#endif
#ifdef _PLAYER_QUESTION_ONLIEN
	{ "PlayerQuestion",		CHAR_CHAT_DEBUG_PlayerQuestion,      TRUE,   0,  3, "���� �� ��Ʒ��� ��Ʒ��ֵ" },
#endif
#ifdef _GM_SAVE_ALL_CHAR
	{ "GmSaveAllChar",		CHAR_CHAT_DEBUG_GmSaveAllChar,      TRUE,   0,  3, "" },
#endif
#ifdef _ALLBLUES_LUA
	{ "reloadlua",		CHAR_CHAT_DEBUG_ReLoadLua,      TRUE,   0,  3, "" },
	{ "newloadlua",		CHAR_CHAT_DEBUG_NewLoadLua,      TRUE,   0,  3, "" },
#endif
#ifdef _KEEP_UP_NO_LOGIN
	{ "keepupnologin",		CHAR_CHAT_DEBUG_KeepUpNoLogin,      TRUE,   0,  3, "" },
#endif
#ifdef _NEW_LOAD_NPC
	{ "newloadnpc",		CHAR_CHAT_DEBUG_NewLoadNpc,      TRUE,   0,  3, "" },
#endif
#ifdef _NEW_LOAD_MAP
	{ "newloadmap",		CHAR_CHAT_DEBUG_NewLoadMap,      TRUE,   0,  3, "" },
#endif
#ifdef _JZ_NEWSCRIPT_LUA
	{ "luainit",		CHAR_CHAT_DEBUG_LUA_INIT,      TRUE,   0,  3, "LUAλ��"},
	{ "luaclose",		CHAR_CHAT_DEBUG_LUA_CLOSE,      TRUE,   0,  3, ""},
#endif
#ifdef _NO_FULLPLAYER_ATT
	{ "nofullplayer",		CHAR_CHAT_NoFullPlayer,      TRUE,   0,  3, ""},
#endif
#ifdef _PET_MAKE_2_TRANS
	{ "petmaketrans",	CHAR_CHAT_DEBUG_petmake2,		TRUE,	0,	2, "ת���� ����ID �ȼ� ����"},
#endif
/////////////////////////////////////////////////////////////////////////
};

void CHAR_initDebugChatCdkey( void)
{
	int i;
	for( i = 0; i < DEBUGCDKEYNUM; i ++ ) {
		DebugCDKey[i].use = FALSE;
		DebugCDKey[i].cdkey[0] = '\0';
	}
}
/*------------------------------------------------------------
 * ��������  ܷ��˦����ب��ë���£�
 * ¦�ѣ�߯Ի��
 *  ئ��
 ------------------------------------------------------------*/
void CHAR_initChatMagic(void)
{
	int i;
	for( i=0 ; i<arraysizeof(CHAR_cmtbl) ; i++ )
		CHAR_cmtbl[i].hash = hashpjw( CHAR_cmtbl[i].magicname);
	for( i = 0; i < DEBUGCDKEYNUM; i ++ ) {
		DebugCDKey[i].use = FALSE;
		DebugCDKey[i].cdkey[0] = '\0';
	}
}

int CHAR_setChatMagicCDKey( int mode, char *cdkey)
{

	int i;
	BOOL found = FALSE;
	if( strlen( cdkey) > 8 ) {
		return -1;
	}
	for( i = 0; i < DEBUGCDKEYNUM; i ++ ) {
		if( DebugCDKey[i].use == FALSE) {
			if( mode == 0 ) {
				DebugCDKey[i].use = TRUE;
				strcpysafe( DebugCDKey[i].cdkey, sizeof( DebugCDKey[i].cdkey), cdkey);
				found = TRUE;
				break;
			}
		}
		else {
			if( mode == 1 ) {
				if( strcmp( DebugCDKey[i].cdkey, cdkey) == 0 ) {
					DebugCDKey[i].use = FALSE;
					DebugCDKey[i].cdkey[0] = '\0';
					found = TRUE;
				}
			}
		}
				
	}
	if( !found ) return -1;
	return i;
}

CHATMAGICFUNC CHAR_getChatMagicFuncPointer(char* name, BOOL isDebug)
{
	int i;
	int hash = hashpjw( name );
	
	for( i=0 ; i<arraysizeof(CHAR_cmtbl) ; i++ ){
		if( CHAR_cmtbl[i].hash == hash
			&& CHAR_cmtbl[i].isdebug == isDebug
			&& strcmp( CHAR_cmtbl[i].magicname, name ) == 0 ){		
				return CHAR_cmtbl[i].func;
		}
	}
	return NULL;
}

int CHAR_getChatMagicFuncLevel(char* name,BOOL isDebug)
{
	int i;
	int hash = hashpjw( name );
	for( i=0 ; i<arraysizeof(CHAR_cmtbl) ; i++ )
		if( CHAR_cmtbl[i].hash == hash
			&& CHAR_cmtbl[i].isdebug == isDebug
			&& strcmp( CHAR_cmtbl[i].magicname, name ) == 0 )
			return CHAR_cmtbl[i].level;
	return -1;
}

int CHAR_getChatMagicFuncNameAndString( int ti, char* name, char *usestring, int level, BOOL isDebug)
{
	if( name == NULL || usestring == NULL ) return -1;
	if( ti < 0 || ti >= arraysizeof( CHAR_cmtbl) ) return -1;
	if( CHAR_cmtbl[ ti].isdebug == isDebug &&
		CHAR_cmtbl[ ti].level <= level ){
		sprintf( name,"%s", CHAR_cmtbl[ ti].magicname );
		sprintf( usestring, "%s", CHAR_cmtbl[ ti].usestring );
		return 1;
	}
	return 0;
}

int CHAR_getChatMagicFuncMaxNum( void)
{
	return arraysizeof( CHAR_cmtbl);
}

#ifdef _ALLBLUES_LUA_1_2 
MAGIC_LuaFunc MAGIC_luaFunc;

BOOL MAGIC_addLUAListFunction(lua_State *L, const char *luafuncname, const char *luafunctable, int gmlevel, char *usestring )
{
	MAGIC_LuaFunc *luaFunc = &MAGIC_luaFunc;

  while(luaFunc->next != NULL){
  	luaFunc = luaFunc->next;
  }

	memset( luaFunc, 0 , sizeof(MAGIC_luaFunc) ); 
  luaFunc->lua = L;
  luaFunc->luafuncname = allocateMemory( strlen(luafuncname) );
  strcpy(luaFunc->luafuncname, luafuncname);
  luaFunc->luafunctable = allocateMemory( strlen(luafunctable) );
  strcpy(luaFunc->luafunctable, luafunctable);
  luaFunc->usestring = allocateMemory( strlen(usestring) );
  strcpy(luaFunc->usestring, usestring);

	luaFunc->gmlevel = gmlevel;

	luaFunc->next = allocateMemory( sizeof(MAGIC_luaFunc) );
	memset( luaFunc->next, 0 , sizeof(MAGIC_luaFunc) );  
	if(luaFunc->next == NULL)return FALSE;
	luaFunc = luaFunc->next;
	luaFunc->next=NULL;

	return TRUE;
}

BOOL MAGIC_getLUAListFunction( char *luafuncname, int gmlevel, int charaindex, char *data )
{
	MAGIC_LuaFunc *luaFunc = &MAGIC_luaFunc;

  while(luaFunc->next != NULL){
  	if(strcmp(luaFunc->luafuncname, luafuncname) == 0){
  		if( gmlevel >= luaFunc->gmlevel ){
	  		lua_getglobal( luaFunc->lua, luaFunc->luafunctable);
				return RunUseChatMagic(charaindex, data, luaFunc->lua);
			}else{
				return FALSE;
			}
		}
  	luaFunc = luaFunc->next;
  }
  return FALSE;
}
#endif
/*------------------------------------------------------------
 * ��������  ܷ
 * ¦��
 *  charaindex      int     ƽ�ҷ��̼������͵�
 *  message         char*   ����������
 *  isDebug         BOOL    ����������������  ܷ��������
 * ߯Ի��
 *  ئ��
 ------------------------------------------------------------*/
static BOOL CHAR_useChatMagic( int charaindex, char* data, BOOL isDebug)
{
	char    magicname[256];
	int     ret;
	int	i;
	int	gmLevel = 0, magicLevel;
	
	CHATMAGICFUNC   func;

#ifdef _GMRELOAD
	extern struct GMINFO gminfo[GMMAXNUM];
#else
#endif
	char *p = CHAR_getChar( charaindex, CHAR_CDKEY);
	if( !p ) {
		fprint( "err nothing cdkey\n");
		return FALSE;
	}

	if( getChatMagicCDKeyCheck() == 1 ){ //��һ��ȷ��GM�ʺ�
		if( CHAR_getWorkInt( charaindex, CHAR_WORKFLG) & WORKFLG_DEBUGMODE ) {
			gmLevel = CHAR_getWorkInt( charaindex, CHAR_WORKGMLEVEL);
		}else{

#ifdef _GMRELOAD
			for (i = 0; i < GMMAXNUM; i++){
				if (strcmp( p, gminfo[i].cdkey) == 0){
					gmLevel = gminfo[i].level;
					CHAR_setWorkInt( charaindex, CHAR_WORKGMLEVEL, gmLevel);
					break;
				}
			}
#else		
#endif
			if( i >= GMMAXNUM ){
				for( i = 0; i < DEBUGCDKEYNUM; i ++ ) {
					if( DebugCDKey[i].use && strcmp( p, DebugCDKey[i].cdkey) == 0 ) {
						break;
					}
				}
				if( i >= DEBUGCDKEYNUM ){
						return FALSE;
				}
			}
		}
	}else {
		gmLevel = 3;
		CHAR_setWorkInt( charaindex, CHAR_WORKGMLEVEL, gmLevel);
	}
		ret = getStringFromIndexWithDelim( data, " ", 1, magicname,  sizeof( magicname));
	if( ret == FALSE)return FALSE;
	// Robin 0618  chaeck GM Level
	magicLevel = CHAR_getChatMagicFuncLevel( magicname, isDebug);
	if( gmLevel < magicLevel )
		return FALSE;
	
	func = CHAR_getChatMagicFuncPointer(magicname,isDebug);
	if( func ){
		LogGM( CHAR_getUseName( charaindex), CHAR_getChar( charaindex, CHAR_CDKEY), data, 
			CHAR_getInt( charaindex, CHAR_FLOOR), CHAR_getInt( charaindex, CHAR_X),
			CHAR_getInt( charaindex, CHAR_Y) );
		func( charaindex, data + strlen( magicname)+1);
		return TRUE;
	}else{
#ifdef _ALLBLUES_LUA_1_2 
		if(isDebug == TRUE){
			if(MAGIC_getLUAListFunction(magicname, gmLevel, charaindex, data + strlen( magicname)+1)==TRUE){
				LogGM( CHAR_getUseName( charaindex), CHAR_getChar( charaindex, CHAR_CDKEY), data, 
					CHAR_getInt( charaindex, CHAR_FLOOR), CHAR_getInt( charaindex, CHAR_X),
					CHAR_getInt( charaindex, CHAR_Y) );
				return TRUE;
			}
		}
#endif
		return FALSE;
	}
}

/*------------------------------------------------------------
 * ���ھ�����ë�ƻ���
 * ¦��
 *  volume  int     ����
 * ߯Ի��
 *  Ѩ����ƥ��Ѩ��
 ------------------------------------------------------------*/
static int CHAR_getRangeFromVolume( int volume )
{
	static int chatvol[]={
		3,5,7,9,11,13,15,17,19,21,23,25,27,29,31
	};

	if( volume < 0 )return 0;
	else if( arraysizeof( chatvol ) <= volume ) {
		return chatvol[arraysizeof(chatvol)-1];
	}
	return chatvol[volume];
}
/*------------------------------------------------------------
 * 뢼�ʾ��ب��ةë���£���ئ��֧Ի���ף�
 * ¦��
 *  mesg        char*       ���������ɺ�
 * ߯Ի��
 *  -1 �����巴�𹫳�
 *  0  �����巴�׹���
 *  1 ��Ի������������깴�年�л����� ! ����
 ------------------------------------------------------------*/
static int CHAR_getVolume( char* mesg )
{
	int stringlen = strlen( mesg );
	if( stringlen == 0 )
		return 0;
	else if( stringlen == 1 ){
    if ( mesg[ 0 ] == '!' )
      return 1;
    else
      return 0;
  }
  else if ( stringlen == 2 ) {
    if ( mesg[ 1 ] == '!' ) {
      if ( mesg[ 0 ] == '!' )
        return 2;
      else
        return 1;
    }
    else
      return 0;
	}else{
		/*  3����   */
		if( mesg[stringlen-1] == '.' ){
			/*  �𹫳𹫼���  ��ؤԻ    */
			if( mesg[stringlen-2] == '.' && mesg[stringlen-3] == '.' ){
				/*  �𹫳�    */
				return -1;
			}
			return 0;
		}else{
			/*  ����� ! ë������*/
			int exnum=0;
			int i;
			for( i=stringlen-1; i>=0 ; i-- ){
				if( mesg[i] != '!' )
					break;
				else
					exnum++;
			}
			return exnum;
		}
	}
}

/*------------------------------------------------------------
 * ���������⼰    ���ն��������⼰      ��ë��Ի����
 * ¦��
 *  message     char*           ����������
 *  kind        char*           p or s or iئ������������
 *  kindlen     int             kind ��Ӯ��
 *  body        char**          ����������    �����̼�������ɡ  ����
 * ߯Ի��
 *  ئ��
 ------------------------------------------------------------*/
void CHAR_getMessageBody(char* message,char* kind,int kindlen,char** body)
{
    int firstchar;

	/* 1  ٯ  ë��֮���ʣ�1  ٯƥؤ�³��練lssproto.html����� */
	// Nuke +1: For invalid message attack
	*body = 0;
    firstchar = message[0];
    if( firstchar == 'P' ||
        firstchar == 'S' ||
        firstchar == 'D'

        // CoolFish: Trade 2001/4/18
	|| firstchar == 'C'
        || firstchar == 'T'
        || firstchar == 'W'
                                
        // CoolFish: Family 2001/5/28
	|| firstchar == 'A'
        || firstchar == 'J'
        || firstchar == 'E'
        || firstchar == 'M'
        
        || firstchar == 'B'
        || firstchar == 'X'
        || firstchar == 'R'
        || firstchar == 'L'

         ){
        if( kindlen >= 2 ){
            kind[0] = firstchar;
            kind[1] = '\0';
        } else {
            return;
        }
    } else {
        return;
    }

	*body = message + 2;
}


static BOOL CHAR_Talk_check( int talkerindex, int talkedindex, int micflg )
{
#if 1
	if( !CHAR_getFlg( talkerindex, CHAR_ISPARTYCHAT )){
		int talker_b_mode = CHAR_getWorkInt( talkerindex, CHAR_WORKBATTLEMODE);
		int talked_b_mode = CHAR_getWorkInt( talkedindex, CHAR_WORKBATTLEMODE);
		if( talker_b_mode != BATTLE_CHARMODE_NONE
			&& CHAR_getInt( talkedindex, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER){
			return FALSE;
		}
		if( micflg != 0 )return TRUE;
		if( talker_b_mode != BATTLE_CHARMODE_NONE && talked_b_mode != BATTLE_CHARMODE_NONE){
			if( CHAR_getWorkInt( talkerindex, CHAR_WORKBATTLEINDEX)
				!= CHAR_getWorkInt( talkedindex, CHAR_WORKBATTLEINDEX)){
				return FALSE;
			}
		}else if( talker_b_mode != BATTLE_CHARMODE_NONE || talked_b_mode != BATTLE_CHARMODE_NONE){
		 	return FALSE;
		}
		return TRUE;
	}
	if( CHAR_getInt( talkerindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER) {
		int talker_b_mode = CHAR_getWorkInt( talkerindex, CHAR_WORKBATTLEMODE);
		int talked_b_mode = CHAR_getWorkInt( talkedindex, CHAR_WORKBATTLEMODE);

		if( talker_b_mode != BATTLE_CHARMODE_NONE && CHAR_getInt( talkedindex, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER){
			return FALSE;
		}
		if( micflg != 0 )return TRUE;
		if( CHAR_getWorkInt( talkerindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE ){
			if( CHAR_getWorkInt( talkedindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE){
				if( CHAR_getWorkInt( talkerindex, CHAR_WORKPARTYINDEX1)
					== CHAR_getWorkInt( talkedindex, CHAR_WORKPARTYINDEX1) ){
					return TRUE;
				}
			}
		}else {
			if( talker_b_mode != BATTLE_CHARMODE_NONE && talked_b_mode != BATTLE_CHARMODE_NONE){
				if( CHAR_getWorkInt( talkerindex, CHAR_WORKBATTLEINDEX)
					!= CHAR_getWorkInt( talkedindex, CHAR_WORKBATTLEINDEX)){
					return FALSE;
				}
			}else if( talker_b_mode != BATTLE_CHARMODE_NONE ||
					talked_b_mode != BATTLE_CHARMODE_NONE){
			 	return FALSE;
			}
			return TRUE;
		}
	}
	return FALSE;

#else
	int		MyBattleMode;
	int		ToBattleMode;

	MyBattleMode = CHAR_getWorkInt( talkerindex, CHAR_WORKBATTLEMODE);
	ToBattleMode = CHAR_getWorkInt( talkedindex, CHAR_WORKBATTLEMODE);

	/*   ��    ���� */
	if( MyBattleMode == BATTLE_CHARMODE_NONE ) {
		if( ToBattleMode == BATTLE_CHARMODE_NONE ) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	/* ��    ���� */
	else {
		/* ��  �ƻ���ئ���б巴˪��ئ�У� */
		if( ToBattleMode == BATTLE_CHARMODE_NONE) {
			return FALSE;
		}
		/*   Ԫ��  ƥ  Ԫ�������б��ƾ�˪����  ئ�� */
		if( CHAR_getWorkInt( talkerindex, CHAR_WORKBATTLEINDEX)
			== CHAR_getWorkInt( talkedindex, CHAR_WORKBATTLEINDEX) &&
			CHAR_getWorkInt( talkerindex, CHAR_WORKBATTLESIDE)
			== CHAR_getWorkInt( talkedindex, CHAR_WORKBATTLESIDE ) )
		{
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	return FALSE;
#endif
}
#ifdef _TELLCHANNEL				// (���ɿ�) Syu ADD ����Ƶ��
static int TalkCount = 0 ; 
void OneByOneTkChannel ( int fd , char *tmp1 , char *tmp2 , int color )
{
	int i ; 
	int myindex ; 
    int playernum = CHAR_getPlayerMaxNum();
	char buf[512];
	int IndexList[10] = { 0 } ; 
	int IndexCount = 0 ; 
	char *addr;
	int target;
	myindex = CONNECT_getCharaindex(fd);

  if ( strlen( tmp2 ) > ( sizeof( buf ) - 1 ) )
    return ;
	memset ( IndexList , -1 , sizeof( IndexList ) ); 

#ifdef _THE_WORLD_SEND
	if(strcmp(tmp1, "����" ) == 0){
		int lastTalkTime = CHAR_getWorkInt(myindex, CHAR_WORKWORLDSENDTIME );
		if( (int)NowTime.tv_sec - lastTalkTime > 5 ) {
			char token[256];
			if(CHAR_getInt( myindex, CHAR_TRANSMIGRATION) >= getTheWorldTrans() 
				&& CHAR_getInt( myindex, CHAR_LV) >= getTheWorldLevel()){
					if(CHAR_getInt(myindex, CHAR_FAME)>=getTheWorldSend()){
					sprintf(token, "[����] %s ˵��%s", CHAR_getChar( myindex , CHAR_NAME), tmp2);
				  for( i = 0 ; i < playernum ; i++) {
				    if( CHAR_getCharUse(i) != FALSE ) {
				    	 if(CHAR_getFlg(i, CHAR_ISWORLD)==TRUE){
							 	CHAR_talkToCli( i, -1, token, CHAR_COLORGREEN);
							}
				    }
					}
					CHAR_setInt(myindex,CHAR_FAME, CHAR_getInt(myindex,CHAR_FAME) - getTheWorldSend());
					CHAR_setWorkInt(myindex, CHAR_WORKWORLDSENDTIME, (int)NowTime.tv_sec );
				}else{
					sprintf(token, "������������%d���޷�ʹ������Ƶ�����ܣ�", getTheWorldSend());
					CHAR_talkToCli( i, -1, token, CHAR_COLORRED);
				}
				return;
			}else{
				sprintf(token, "����ת���ȼ�δ�ﵽҪ��,�ݲ���ʹ������Ƶ��!");
				CHAR_talkToCli( myindex, -1, token, CHAR_COLORRED);
				return;
			}
		}else{
			CHAR_talkToCli( myindex, -1, "ÿ��ʹ������Ƶ�������������ʹ������Ƶ��!", CHAR_COLORRED);
			return;
		}
	}
#endif

#ifdef _ALL_SERV_SEND
	if(strcmp(tmp1, "����" ) == 0){
		char token[256];
		if(CHAR_getInt( myindex, CHAR_TRANSMIGRATION) >= getAllServTrans() 
			&& CHAR_getInt( myindex, CHAR_LV) >= getAllServLevel()){
				if(CHAR_getInt(myindex, CHAR_FAME)>=getAllServSend()){
					sprintf(token, "[����%s] %s ˵��%s", getGameservername(), CHAR_getChar( myindex , CHAR_NAME), tmp2);
					saacproto_AllServSend_send(token);
					
					CHAR_setInt(myindex,CHAR_FAME, CHAR_getInt(myindex,CHAR_FAME) - getAllServSend());

			}else{
				sprintf(token, "������������%d���޷�ʹ������Ƶ�����ܣ�", getAllServSend());
				CHAR_talkToCli( myindex, -1, token, CHAR_COLORRED);
			}
			return;
		}else{
			sprintf(token, "����ת���ȼ�δ�ﵽҪ��,�ݲ���ʹ������Ƶ��!");
			CHAR_talkToCli( myindex, -1, token, CHAR_COLORRED);
			return;
		}
	}
#endif

	for( i = 0 ; i < playernum ; i++) {
		if( CHAR_getCharUse(i) != FALSE ) {
			if (!CHAR_CHECKINDEX(i))       
				continue;
			if ( strcmp ( tmp1 , CHAR_getChar( i , CHAR_NAME ) ) == 0 ) {
				IndexList[ IndexCount ] = i ; 
				IndexCount ++ ; 
        if ( IndexCount >= 10 )
          break;
			}
		}
	}

	if ( IndexCount == 1 ) {
		if ( myindex == IndexList[0] )
			return;
#ifdef _CHANNEL_MODIFY
		if(CHAR_getFlg(IndexList[0],CHAR_ISTELL) == FALSE){
			snprintf(buf,sizeof(buf) - 1,"%s �ر��˴�Ƶ��" ,tmp1);
			CHAR_talkToCli(myindex,-1,buf,color);
			return;
		}
#endif
#ifndef _CHANNEL_MODIFY
		snprintf( buf, sizeof( buf)-1, "�����%s��%s" , tmp1 , tmp2 ) ;
		CHAR_talkToCli( myindex, -1, buf , color);
		snprintf( buf , sizeof( buf)-1,"%s�����㣺%s" , CHAR_getChar ( myindex , CHAR_NAME ) , tmp2 ) ;
		CHAR_talkToCli( IndexList[0] , -1, buf , color);
#else
		snprintf(buf,sizeof(buf) - 1,"P|M|�����%s��%s",tmp1,tmp2);
		lssproto_TK_send(fd,CHAR_getWorkInt(myindex,CHAR_WORKOBJINDEX),buf,color);
		snprintf(buf,sizeof(buf) - 1,"P|M|%s�����㣺%s",CHAR_getChar(myindex,CHAR_NAME),tmp2);
		lssproto_TK_send(getfdFromCharaIndex(IndexList[0]),CHAR_getWorkInt(IndexList[0],CHAR_WORKOBJINDEX),buf,color);
#endif
		TalkCount ++ ; 
	}else if ( IndexCount > 1 && IndexCount < 10 ) {
		print( "\nSyu log ����ͬ��Func" );
		if ( ( addr = strstr ( tmp2 , "/T" ) ) != NULL ) {
			addr = addr + 2 ; 
			target = atoi ( addr ) ; 
			print ( "\nSyu log addr => %s , target => %d " , addr , target ) ; 
			addr = strtok ( tmp2 , "/T" ) ; 
			if (!CHAR_CHECKINDEX(IndexList[target]))       
				return;
			if ( strcmp ( tmp1 , CHAR_getChar ( IndexList[ target ] , CHAR_NAME ) ) == 0 &&
				IndexList[ target ] != -1 ) {
				if ( myindex == IndexList[ target ] ) 
					return ; 
#ifdef _CHANNEL_MODIFY
				if(CHAR_getFlg(IndexList[target],CHAR_ISTELL) == FALSE){
					snprintf(buf,sizeof(buf) - 1,"%s �ر��˴�Ƶ��" ,tmp1);
					CHAR_talkToCli(myindex,-1,buf,color);
					return;
				}
#endif
#ifndef _CHANNEL_MODIFY
				snprintf( buf , sizeof( buf)-1, "�����%s��%s" , tmp1 , addr ) ;
				CHAR_talkToCli( myindex, -1, buf , color);
				snprintf( buf , sizeof( buf)-1, "%s�����㣺%s" , CHAR_getChar ( myindex , CHAR_NAME ) , addr ) ;
				CHAR_talkToCli( IndexList[ target ] , -1, buf , color);
#else
				snprintf(buf,sizeof(buf) - 1,"P|M|�����%s��%s",tmp1,addr);
				lssproto_TK_send(fd,CHAR_getWorkInt(myindex,CHAR_WORKOBJINDEX),buf,color);
				snprintf(buf,sizeof(buf) - 1,"P|M|%s�����㣺%s",CHAR_getChar(myindex,CHAR_NAME),addr);
				lssproto_TK_send(getfdFromCharaIndex(IndexList[target]),CHAR_getWorkInt(IndexList[target],CHAR_WORKOBJINDEX),buf,color);
#endif
				TalkCount ++ ;
			}
		}else {
			for ( i = 0 ; i < 10 ; i ++ ) {
        if ( IndexList[ i ] == -1 )
          break;

        if ( !CHAR_CHECKINDEX( IndexList[ i ] ) )
          break;
#ifndef _CHANNEL_MODIFY
				sprintf( buf , "TK|%d|%d|%d|%s|%s" , 
					i , 
					CHAR_getInt ( IndexList[ i ] , CHAR_TRANSMIGRATION ) , 
					CHAR_getInt ( IndexList[ i ] , CHAR_LV ) , 
					CHAR_getChar ( IndexList[ i ] , CHAR_NAME ) , 
					CHAR_getChar ( IndexList[ i ] , CHAR_OWNTITLE )  
					 ) ; 
				CHAR_talkToCli( myindex , -1, buf , color);
#else
				sprintf( buf , "P|TK|%d|%d|%d|%s|%s" , 
					i , 
					CHAR_getInt ( IndexList[ i ] , CHAR_TRANSMIGRATION ) , 
					CHAR_getInt ( IndexList[ i ] , CHAR_LV ) , 
					CHAR_getChar ( IndexList[ i ] , CHAR_NAME ) , 
					CHAR_getChar ( IndexList[ i ] , CHAR_OWNTITLE )  
					 ) ; 
				lssproto_TK_send(fd,CHAR_getWorkInt(myindex,CHAR_WORKOBJINDEX),buf,color);
#endif
			}
#ifndef _CHANNEL_MODIFY
			snprintf( buf , sizeof( buf)-1, "TE|%s" , tmp2 ) ; 
			CHAR_talkToCli ( myindex , -1 , buf , color ) ; 
#else
			snprintf( buf , sizeof( buf)-1, "P|TE|%s" , tmp2 ) ; 
			lssproto_TK_send(fd,CHAR_getWorkInt(myindex,CHAR_WORKOBJINDEX),buf,color);
#endif
		}
	}else if ( IndexCount == 0 ) {
		sprintf( buf , "û������˻�λ��������" ) ;
		CHAR_talkToCli( myindex, -1, buf , color);
	}
//	print ( "\nSyu log TkChannel use ratio : %d " , TalkCount ) ; 
}
#endif


void CHAR_Talk( int fd, int index,char* message,int color, int area )
{
	char    firstToken[64];
	char    messageeraseescape[512];
	char*   messagebody;
	int     mesgvolume=0;
	int     lastvolume=0;
	int     fl,x,y;
	int     stringlen;
	int		micflg = 0;
#ifdef _TELLCHANNEL				// (���ɿ�) Syu ADD ����Ƶ��
	char    tmp1[128] ; 
	char	*tmp2;
#endif	
	int	fmindexi = CHAR_getWorkInt( index, CHAR_WORKFMINDEXI );
	int	channel = CHAR_getWorkInt( index, CHAR_WORKFMCHANNEL );
	int	quickchannel = CHAR_getWorkInt( index, CHAR_WORKFMCHANNELQUICK );
	{
		char   *cdkey = CHAR_getChar( index, CHAR_CDKEY);
		char *charname = CHAR_getChar( index, CHAR_NAME);
		if( strlen(message) > 3 ){
			if( CHAR_getWorkInt( index, CHAR_WORKFLG) & WORKFLG_DEBUGMODE )	{
				// shan begin
				if(strstr( message, "[")&&strstr( message, "]")){
				}else{
					// original
					LogTalk( charname, cdkey, CHAR_getInt( index, CHAR_FLOOR),
							 CHAR_getInt( index, CHAR_X), CHAR_getInt( index, CHAR_Y),
							 message );
				}
				// end
			}
		}
	}
	//bg|0|r0|fc|d4B8|p0|bn|10|bt|10| 
	// Nuke +1 08/27: For invalid message attack
  if ( *message == 0 )
    return ;
	CHAR_getMessageBody(message,firstToken,sizeof(firstToken),
						&messagebody);
        // Nuke +1: For invalid message attack
  if ( !messagebody )
    return ;
                						
	strcpysafe( messageeraseescape,sizeof(messageeraseescape),
				messagebody );
	makeStringFromEscaped( messageeraseescape );
	stringlen = strlen(  messageeraseescape  );
  if ( stringlen <= 0 )
    return ;
    
#ifdef _FILTER_TALK
{
	int i;
	char *p=NULL;
	while(1){
		for(i=0;i<getFilterTalkNum();i++){
			if(p = strstr(messageeraseescape, getFilterTalk(i))){
				int j;
				int len=strlen(getFilterTalk(i));
				int start = strlen(messageeraseescape) - strlen(p);
				for(j=0; j<len; j++){
					messageeraseescape[start + j] = '*';
				}
				break;
			}
		}
		if(i==getFilterTalkNum())break;
	}
}
#endif	
/*
#if _ATTESTAION_ID == 1
{
	
	char token[256];
	char buff[256];
	getStringFromIndexWithDelim(messageeraseescape, " ", 1, token, sizeof(token));
	if(atoi(token)>1 && atoi(token)<200){
		getStringFromIndexWithDelim(messageeraseescape, " ", 2, buff, sizeof(buff));
	    		lssproto_WN_send( getfdFromCharaIndex(index), atoi(token), 
							WINDOW_BUTTONTYPE_OKCANCEL,
							-1,
							-1,
							buff);
	}
	if(atoi(token)== 0){
		getStringFromIndexWithDelim(messageeraseescape, " ", 2, buff, sizeof(buff));
		lssproto_C_send(getfdFromCharaIndex(index), buff);
	}
}		
#endif	
*/

#ifdef _ALLBLUES_LUA_1_6
	if(CharTalkFunction( index, messageeraseescape, color ) == TRUE){
		return;
	}
#endif
#ifdef _THE_WORLD_SEND
	if (messageeraseescape[0]== '/' && messageeraseescape[1]== 'W' && messageeraseescape[2]== 'D'){
		int lastTalkTime = CHAR_getWorkInt(index, CHAR_WORKWORLDSENDTIME );

		if( (int)NowTime.tv_sec - lastTalkTime > 5 ) {
			char token[256];
			char buff[256];
			char buf[64];
			if(CHAR_getInt( index, CHAR_TRANSMIGRATION) >= getTheWorldTrans() 
						&& CHAR_getInt( index, CHAR_LV) >= getTheWorldLevel()){
				if(CHAR_getInt(index, CHAR_FAME)>=getTheWorldSend()){
	//					easyGetTokenFromString( messageeraseescape , 2 , buff, sizeof(buff));
					sprintf(token, "[����] %s ˵��%s", CHAR_getChar( index , CHAR_NAME), messageeraseescape + 4);
					int i;
					int playernum = CHAR_getPlayerMaxNum();
				  for( i = 0 ; i < playernum ; i++) {
				    if( CHAR_getCharUse(i) != FALSE ) {
#ifdef _CHATROOMPROTOCOL
							 if(CHAR_getFlg(i, CHAR_ISWORLD)==TRUE)
#endif
							 	CHAR_talkToCli( i, -1, token, CHAR_COLORGREEN);
				    }
					}
					CHAR_setInt(index,CHAR_FAME, CHAR_getInt(index,CHAR_FAME) - getTheWorldSend());
					CHAR_setWorkInt(index, CHAR_WORKWORLDSENDTIME, (int)NowTime.tv_sec );
				}else{
					sprintf(token, "������������,���޷�ʹ������Ƶ��!");
					CHAR_talkToCli( index, -1, token, CHAR_COLORRED);
				}
				return;
			}else{
				sprintf(token, "����ת���ȼ�δ�ﵽҪ��,�ݲ���ʹ������Ƶ��!");
				CHAR_talkToCli( index, -1, token, CHAR_COLORRED);
				return;
			}
		}else{
			CHAR_talkToCli( index, -1, "ÿ��ʹ������Ƶ�������������ʹ������Ƶ��!", CHAR_COLORRED);
			return;
		}
	}
#endif
    
#ifdef _ONLINE_TALK_IP
	if (messageeraseescape[0]== '/' && messageeraseescape[1]== 'I' && messageeraseescape[2]== 'P'){
		char token[256];
		char name[64];
		if(getStringFromIndexWithDelim(messageeraseescape, " ", 2, name, sizeof(name))==TRUE){
			int i;
			int playernum = CHAR_getPlayerMaxNum();
			for( i = 0 ; i < playernum ; i++) {
			  if( CHAR_getCharUse(i) != FALSE ) {
					if(strcmp(CHAR_getChar(i, CHAR_NAME), name)==0){
						int	i;
						int itemindex;
						for( i = CHAR_STARTITEMARRAY; i < CheckCharMaxItem(index); i ++ ) {
						  itemindex = CHAR_getItemIndex(index, i);
						  if(strcmp(ITEM_getChar( itemindex, ITEM_USEFUNC), "ITEM_TALKIP")==0){
						  	char buf[256];
						  	
						  	int itemmaxuse = ITEM_getInt( itemindex, ITEM_DAMAGEBREAK);
						  	itemmaxuse--;
								ITEM_setInt( itemindex, ITEM_DAMAGEBREAK, itemmaxuse);
								if( itemmaxuse < 1 )	{
									sprintf( buf, "���� %s��ʧ�ˡ�", ITEM_getChar( itemindex, ITEM_NAME) );
									CHAR_talkToCli(index, -1, buf, CHAR_COLORYELLOW);
									CHAR_DelItem( index, i);
								}else{
									sprintf( buf, "%s����ʹ�ô���ʣ��%d�Ρ�", ITEM_getChar( itemindex, ITEM_NAME), itemmaxuse);
									ITEM_setChar( itemindex, ITEM_EFFECTSTRING, buf);
									CHAR_sendItemDataOne( index, i);
								}
								
								struct tm now;
								time_t timep;
								time(&timep);
								
								memcpy(&now, localtime(&timep), sizeof(now));
								sprintf(buf, "onlinetalkip/%02d-%02d-%02d.txt", 1900 + now.tm_year, 1 + now.tm_mon, now.tm_mday);
								
								FILE * f1 = fopen(buf,"a+");
								sprintf(token, "[%02d-%02d-%02d %02d:%02d:%02d]��� %s ������ %s ���߾ٱ��Է� %s ������ %s  \n",
																																									1900 + now.tm_year, 1 + now.tm_mon, now.tm_mday,
																																									now.tm_hour, now.tm_min, now.tm_sec,
																																									CHAR_getChar(index, CHAR_CDKEY), 
																																									CHAR_getChar(index, CHAR_NAME), 
																																									CHAR_getChar(i, CHAR_CDKEY), 
																																									CHAR_getChar(i, CHAR_NAME));
								fputs(token,f1);
								fclose(f1);
								
								CHAR_talkToCli( index, -1, "��л��ľٱ������ǽ����������ṩ���������к�ʵ����!", CHAR_COLORYELLOW);
								
								break; 
						  }
						}
						if(i ==  CheckCharMaxItem(index)){
							CHAR_talkToCli( index, -1, "�������ϳ��оٱ����߲ſ��Ծٱ����!", CHAR_COLORYELLOW);
						}
					}
				}
			}
		}
		return;
	}
#endif	
		
#ifdef _ONLINE_TALK_GM
	if(messageeraseescape[0]== '|' && messageeraseescape[1]== 'G' && messageeraseescape[2]== 'M' && messageeraseescape[3]== '|'){
		int i, j;
		int num=0;
		int playernum = CHAR_getPlayerMaxNum();
		char token[256];
		sprintf(token, "�յ�������� %s ������ %s ����Ϣ��%s", CHAR_getChar(index, CHAR_CDKEY),
																												 		CHAR_getChar(index, CHAR_NAME), 
																												 		messageeraseescape+4);
		for (i = 0; i < GMMAXNUM; i++){
			for( j = 0 ; j < playernum ; j++) {
			  if( CHAR_getCharUse(j) != FALSE ) {
					if(strcmp(CHAR_getChar(j, CHAR_CDKEY), gminfo[i].cdkey)==0){
						CHAR_talkToCli( j, -1, token, CHAR_COLORPURPLE);
						num++;
					}
				}
			}
		}
		if(num==0){
			CHAR_talkToCli( index, -1, "��ǰ��û��GM����", CHAR_COLORRED);
		}
			
	}
#endif	
		    
#ifdef _TELLCHANNEL				// (���ɿ�) Syu ADD ����Ƶ��
	if (messageeraseescape[0]== '/' && messageeraseescape[1]== 't' &&
		messageeraseescape[2]== 'e' && messageeraseescape[3]== 'l' && 
		messageeraseescape[4]== 'l'){
		getStringFromIndexWithDelim( message , " ", 2, tmp1, sizeof(tmp1));
		if ( (tmp2 = strstr ( message , tmp1 )) != NULL ) {
			//strcpy ( message , tmp2 + strlen ( tmp1 ) + 1 ) ;
			message = tmp2 + strlen(tmp1) + 1 ; 
			if( message == "\0" || strlen( message ) <= 0 ) 
				return;
		}
		OneByOneTkChannel ( fd , tmp1 , message , color ) ; 
		return;
	}
#endif
#ifdef _TALK_SAVE
	if (messageeraseescape[0]== '/' && messageeraseescape[1]== 's' &&
		messageeraseescape[2]== 'a' && messageeraseescape[3]== 'v' && 
		messageeraseescape[4]== 'e'){
			if(CHAR_getInt(index, CHAR_FAME)>=getSaveFame()){
				static int OldTimes = 0;
				if( NowTime.tv_sec - OldTimes > 3  )	{
					if( fd != -1 ) {
						char token[256];
						if(CHAR_charSaveFromConnect(index, FALSE)){
							CHAR_talkToCli(index, -1, "ϵͳ�Զ�Ϊ���浵!", CHAR_COLORRED);
							CHAR_setInt(index,CHAR_FAME, CHAR_getInt(index,CHAR_FAME) - getSaveFame());
						}
						OldTimes = NowTime.tv_sec;
					}
				}else{
					CHAR_talkToCli(index, -1, "3����ֻ����һ����Ҵ浵!", CHAR_COLORRED);
				}
			}else{
				CHAR_talkToCli( index, -1,"���Ѿ�û���㹻���������б����ˣ�", CHAR_COLORRED );
			}
			return;
	}
#endif

#ifdef _CHANNEL_MODIFY
	// ����Ƶ��
	if(messageeraseescape[0] == '/' && messageeraseescape[1] == 'F' && messageeraseescape[2] == 'M'){
		sprintf(messageeraseescape,"%s",messageeraseescape + 3);
	}
	else channel = -1;
#ifdef _CHAR_PROFESSION			// WON ADD ����ְҵ
	// ְҵƵ��
	if(messageeraseescape[0] == '/' && messageeraseescape[1] == 'O' &&
		 messageeraseescape[2] == 'C' && messageeraseescape[3] == 'C'){
		int i,pclass = CHAR_getInt(index,PROFESSION_CLASS) - 1,TalkTo;
		char buf[512];

		sprintf(messageeraseescape,"%s",messageeraseescape + 4);
		for(i=0;i<getFdnum();i++){
			TalkTo = *(piOccChannelMember + (pclass * getFdnum()) + i);
			if(TalkTo > -1){
				// ����Է��п�Ƶ��
				if(CHAR_getFlg(TalkTo,CHAR_ISOCC) == TRUE){
					char escapebuf[2048];
					//snprintf(buf,sizeof(buf) - 1,"P|O|[ְ]%s",messageeraseescape);
					snprintf(buf,sizeof(buf) - 1,"P|O|[ְ]%s", makeEscapeString( messageeraseescape, escapebuf, sizeof(escapebuf)) );
					lssproto_TK_send(getfdFromCharaIndex(TalkTo),CHAR_getWorkInt(TalkTo,CHAR_WORKOBJINDEX),buf,color);
				}
			}
		}
		return;
	}
#endif
#endif
	if( messageeraseescape[0] == '['
		&& messageeraseescape[stringlen-1] == ']' ){
		char*   pass;
		// Arminius: limit ip +2
		unsigned long ip;
		int a,b,c,d;
		messageeraseescape[stringlen-1] = '\0';
		 // Arminius: limit ip
		    ip=CONNECT_get_userip(fd);
		    
		    a=(ip % 0x100); ip=ip / 0x100;
		    b=(ip % 0x100); ip=ip / 0x100;
		    c=(ip % 0x100); ip=ip / 0x100;
		    d=(ip % 0x100);
			//andy_log

		char gm[128];
		pass = strstr( messageeraseescape+1, getChatMagicPasswd());
		if( pass == messageeraseescape+1 ){
			
			if(CHAR_useChatMagic( index,messageeraseescape + 1
							   + strlen(getChatMagicPasswd()) + 1, TRUE)){
				print( "\n�˺�:%s ����:%s\n����ip:%d.%d.%d.%d\nʹ��GM����%s]\n",CHAR_getChar( index, CHAR_CDKEY),
															CHAR_getChar( index, CHAR_NAME), a, b, c, d,messageeraseescape);
				sprintf( gm, "��ʹ��GM����%s]�ѳɹ�", messageeraseescape);
				CHAR_talkToCli( index, -1,gm, CHAR_COLORYELLOW );								
			}
		}else{
			if( CHAR_getWorkInt( index, CHAR_WORKFLG ) & WORKFLG_DEBUGMODE ) {
				if(CHAR_useChatMagic( index,messageeraseescape + 1,TRUE)){
					print( "\n�˺�:%s ����:%s\n����ip:%d.%d.%d.%d\nʹ��GM����%s]\n",CHAR_getChar( index, CHAR_CDKEY),
															CHAR_getChar( index, CHAR_NAME), a, b, c, d,messageeraseescape);
					sprintf( gm, "��ʹ��GM����%s]�ѳɹ�", messageeraseescape);
					CHAR_talkToCli( index, -1,gm, CHAR_COLORYELLOW );
				}
			}else{
#ifdef _GM_ITEM
				if(CHAR_getInt( index, CHAR_GMTIME)>0){
					char magicname[32];
					char token[64];
					getStringFromIndexWithDelim( messageeraseescape + 1, " ", 1, magicname,  sizeof( magicname));
					if(!strcmp( CHAR_getChar( index, CHAR_GMFUNCTION), magicname)){
						player_useChatMagic( index,messageeraseescape + 1,TRUE);
						print( "\n�˺�:%s ����:%s\n����ip:%d.%d.%d.%d\nʹ����Ʒ����%s]����ʣ��%d\n", CHAR_getChar( index, CHAR_CDKEY),
															CHAR_getChar( index, CHAR_NAME), a, b, c, d,messageeraseescape, CHAR_getInt( index, CHAR_GMTIME) - 1);
						CHAR_setInt( index , CHAR_GMTIME, CHAR_getInt( index, CHAR_GMTIME) - 1 );
						if(CHAR_getInt( index, CHAR_GMTIME) > 0){
							sprintf( token, "�㻹��ʹ��%sȨ��%d��!", CHAR_getChar( index, CHAR_GMFUNCTION),CHAR_getInt( index, CHAR_GMTIME));
							CHAR_talkToCli( index, -1,token, CHAR_COLORRED );
						}else{
							sprintf( token, "���Ѿ�û��ʹ��%sȨ����!", CHAR_getChar( index, CHAR_GMFUNCTION));
							CHAR_talkToCli( index, -1,token, CHAR_COLORRED );
						}
					}else if(!strcmp( "help", magicname) || !strcmp( "����", magicname)){
						getStringFromIndexWithDelim( messageeraseescape + 1, " ", 2, magicname,  sizeof( magicname));
						if(!strcmp( CHAR_getChar( index, CHAR_GMFUNCTION), magicname)){
							player_useChatMagic( index,messageeraseescape + 1,TRUE);
							print( "\n�˺�:%s ����:%s\n����ip:%d.%d.%d.%d\nʹ�ð�������%s]\n", CHAR_getChar( index, CHAR_CDKEY),
															CHAR_getChar( index, CHAR_NAME), a, b, c, d,messageeraseescape);
						}
					}
				}else
#endif
						CHAR_useChatMagic( index,messageeraseescape + 1,FALSE);
			}
		}
		messageeraseescape[stringlen-1] = ']';
		return;
	}else {
		if( CHAR_getWorkInt( index, CHAR_WORKFLG) & WORKFLG_DEBUGMODE ) {
			if( strstr( messageeraseescape, "[") != NULL ||
				strstr( messageeraseescape, "]") != NULL ){
				return;
			}
		}
	}
#ifdef _PLAYER_MOVE
	if(getPMove()>-1){
		if(messageeraseescape[0] == '/' && messageeraseescape[1] == 'g' && messageeraseescape[2] == 'o'){
			int point=CHAR_getInt( index , CHAR_AMPOINT);
			if(point>=getPMove() || getPMove()== 0 ){
				char x[4],y[4];
				easyGetTokenFromString( messageeraseescape , 2 , x, sizeof(x));
				easyGetTokenFromString( messageeraseescape , 3 , y, sizeof(y));
				CHAR_warpToSpecificPoint( index, CHAR_getInt( index, CHAR_FLOOR), atoi(x), atoi(y) );
				CHAR_setInt( index , CHAR_AMPOINT, point-getPMove());
#ifdef _AMPOINT_LOG
				if(getPMove()>0){
					LogAmPoint(CHAR_getChar( index, CHAR_NAME ),
			         	CHAR_getChar( index, CHAR_CDKEY ),
			          -getPMove(),
			   				CHAR_getInt( index, CHAR_AMPOINT ),
			         	"(����ƶ�)",
			         	CHAR_getInt( index,CHAR_FLOOR),
			         	CHAR_getInt( index,CHAR_X ),
			         	CHAR_getInt( index,CHAR_Y ));
			 }
#endif
			}else
					CHAR_talkToCli( index, -1,"���Ѿ�û���㹻�Ļ��ֵ���˳���ˣ�", CHAR_COLORRED );
			return;
		}
	}
#endif

#ifdef _PLAYER_ANNOUNCE
	if(getPAnnounce() > -1){
		if(messageeraseescape[0] == '/' && messageeraseescape[1] == 'g' && messageeraseescape[2] == 'g'){
		
			int point=CHAR_getInt( index , CHAR_AMPOINT);
			if(point>=getPAnnounce() && getPAnnounce()>= 0 ){
				int i;
			  int playernum = CHAR_getPlayerMaxNum();
				char buff[255];
			
				easyGetTokenFromString( messageeraseescape , 2 , buff, sizeof(buff));
			  for( i = 0 ; i < playernum ; i++) {
			    if( CHAR_getCharUse(i) != FALSE ){
			    	 char token[256];
			    	 sprintf(token, "<С����> %s ˵��%s", CHAR_getChar( index , CHAR_NAME), buff);
						 CHAR_talkToCli( i, -1, token, CHAR_COLORBLUE);
						}
			  }
			  CHAR_setInt( index , CHAR_AMPOINT, point-getPAnnounce());
#ifdef _AMPOINT_LOG
			  if(getPAnnounce()>0){
					LogAmPoint(CHAR_getChar( index, CHAR_NAME ),
			         	CHAR_getChar( index, CHAR_CDKEY ),
			          -getPAnnounce(),
			   				CHAR_getInt( index, CHAR_AMPOINT ),
			         	"(�������)",
			         	CHAR_getInt( index,CHAR_FLOOR),
			         	CHAR_getInt( index,CHAR_X ),
			         	CHAR_getInt( index,CHAR_Y ));
			  }
#endif
			 
			}else{
				CHAR_talkToCli( index, -1,"���Ѿ�û���㹻�Ļ��ֵ���С�����ˣ�", CHAR_COLORRED );
			}
			return;
		}
	}
#endif

#ifdef _TALK_MOVE_FLOOR
	if(talk_move_map(index, messageeraseescape) == TRUE){
		return;
	}
#endif

#ifdef _PLAYER_QUESTION_ONLIEN
	if(PlayQuestionOnline(index, messageeraseescape) == TRUE){
		return;
	}
#endif

#ifdef _SPECIAL_SUIT
	if(messageeraseescape[0] == '/' && messageeraseescape[1] == 'f' && messageeraseescape[2] == 'l' && messageeraseescape[3] == 'y'){
		char token[255];
		int suit;
		easyGetTokenFromString( messageeraseescape , 2 , token, sizeof(token));
		suit = atoi(token);
		if(suit < 1 || suit > CHAR_EQUIPPLACENUM){
			sprintf(token, "������1 ~ %d֮�䣡", CHAR_EQUIPPLACENUM);
			CHAR_talkToCli( index, -1, token, CHAR_COLORRED );
			return;
		}
		int itemindex = CHAR_getItemIndex( index, suit-1);
		if( !ITEM_CHECKINDEX( itemindex) ){
			CHAR_talkToCli( index, -1, "�������λ����û����װװ����", CHAR_COLORRED );
			return;
		}
		int i, num=0;
		int defCode = ITEM_getInt( itemindex, ITEM_SUITCODE);
	
		for( i=0; i<CHAR_STARTITEMARRAY; i++){
			int itemidx = CHAR_getItemIndex( index, i );
			if(defCode == ITEM_getInt( itemidx, ITEM_SUITCODE)){
				num++;
			}
		}
		if(num>=5){
			int ff=0, fx=0, fy=0;
			char* arg = ITEM_getChar(itemindex, ITEM_ARGUMENT );
			if( sscanf( arg, "%d %d %d", &ff, &fx, &fy) == 3 ){
				if( ITEM_WarpForAny(index, ff, fx, fy, 0) == FALSE )return;
			}
		}else{
			sprintf(token, "������װ������%d�����޷�ʹ�øù��ܣ�", 5);
			CHAR_talkToCli( index, -1, token, CHAR_COLORRED );
		}
		return;
	}
#endif

#ifdef _TALK_ACTION
	TalkAction(index, messageeraseescape);
#endif	
#ifdef _TALK_CHECK
	CHAR_setWorkInt(index, CHAR_WORK_TALKCHECK, atoi(messageeraseescape));
#endif
	mesgvolume = CHAR_getVolume( messageeraseescape );
	if( area == 0 ) area = 3;
	if( mesgvolume == -1 )		lastvolume = -1;
	else lastvolume = area - 1 + mesgvolume;
	if( CHAR_getWorkInt( index, CHAR_WORKFLG ) & WORKFLG_MICMODE ){
		lastvolume = 15;
		micflg = 1;
	}
	fl = CHAR_getInt( index, CHAR_FLOOR) ;
	x  = CHAR_getInt( index, CHAR_X) ;
	y  = CHAR_getInt( index, CHAR_Y) ;
	// Robin 0705 channel
	if( messageeraseescape[0] == ':'){
		if( (channel == -1) && (quickchannel != -1) )
			channel = quickchannel;
		else if( channel != -1 )
			channel = -1;
	}	
	{
		typedef void (*TALKF)(int,int,char*,int,int);
		TALKF   talkedfunc=NULL;
		talkedfunc = (TALKF)CHAR_getFunctionPointer(index, CHAR_TALKEDFUNC);
		if( talkedfunc ) {
			CHAR_CHAT_DEBUG_SAVESHH( index,index,messageeraseescape,color, channel );
			talkedfunc(index,index,messageeraseescape,color, channel);
		}
	}
	if( lastvolume == -1 ){
		int     j;
		int xx[2]={ x, x+CHAR_getDX(CHAR_getInt(index,CHAR_DIR))};
		int yy[2]={ y, y+CHAR_getDY(CHAR_getInt(index,CHAR_DIR))};
		BOOL talk=FALSE;
		for( j=0 ; j<2 ; j ++ ){
			OBJECT  object;
			for( object=MAP_getTopObj(fl,xx[j],yy[j]) ; object ; object = NEXT_OBJECT( object ) ){
				int objindex = GET_OBJINDEX(object);
				int	toindex = OBJECT_getIndex( objindex);
				if( OBJECT_getType(objindex) == OBJTYPE_CHARA && toindex != index ){
					if( CHAR_Talk_check( index, toindex, 0 )) {
						if( CHAR_getInt(toindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER){
							talk = TRUE;
						}
						typedef void (*TALKF)(int,int,char*,int,int);
						TALKF   talkedfunc=NULL;
						talkedfunc = (TALKF)CHAR_getFunctionPointer( toindex, CHAR_TALKEDFUNC);
						if( talkedfunc ) {
							CHAR_CHAT_DEBUG_SAVESHH( index,index,messageeraseescape,color, channel );
							talkedfunc(toindex,   index,messageeraseescape,color,-1);
						}
#ifdef _ALLBLUES_LUA
						else{
							RunCharTalkedEvent(toindex,index,messageeraseescape,color, -1);
						}
#endif
					}
				}
			}
		}
		if( talk ){
			CHAR_setInt(index,CHAR_TALKCOUNT, CHAR_getInt( index, CHAR_TALKCOUNT) + 1);
		}
	}else{
		int range = CHAR_getRangeFromVolume( lastvolume );
		int i,j;
		BOOL    talk=FALSE;
		BOOL	channelTalk=FALSE;
		int		partyindex[CHAR_PARTYMAX];
		for( i = 0; i < CHAR_PARTYMAX; i ++ ) {
			partyindex[i] = -1;
		}
#ifndef _CHANNEL_MODIFY
		if( CHAR_getWorkInt( index, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE )
#else
		if(CHAR_getWorkInt( index, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE && channel < 0)
#endif
		{
			for( i = 0; i < CHAR_PARTYMAX; i ++ ) {
				int toindex = CHAR_getPartyIndex( index, i);
				if( CHAR_CHECKINDEX( toindex) && toindex != index) {
					talk = TRUE;
					partyindex[i] = toindex;
					typedef void (*TALKF)(int,int,char*,int,int);
					TALKF   talkedfunc=NULL;
					talkedfunc = (TALKF)CHAR_getFunctionPointer( toindex, CHAR_TALKEDFUNC);
					if( talkedfunc ){
						CHAR_CHAT_DEBUG_SAVESHH( index,index,messageeraseescape,color, channel );
						talkedfunc( toindex,index, messageeraseescape, color, -1 );
					}		
				}
			}
		}
		if( channel > -1 && fmindexi >= 0 ) {
			int i, tindex;

			channelTalk = TRUE;
			i = 0;
#ifdef _FMVER21			
			if( channel == FAMILY_MAXCHANNEL && CHAR_getInt( index, CHAR_FMLEADERFLAG ) == FMMEMBER_LEADER)
#else
			if( channel == FAMILY_MAXCHANNEL && CHAR_getInt( index, CHAR_FMLEADERFLAG ) == FMMEMBER_MEMBER)
#endif			
			{
				char buf[512];
				sprintf(buf, "[�峤�㲥]%s: %s", CHAR_getChar( index, CHAR_NAME ), messageeraseescape);
				saacproto_ACFMAnnounce_send( acfd, 
					CHAR_getChar( index, CHAR_FMNAME), 
					CHAR_getInt( index, CHAR_FMINDEX),
					CHAR_getWorkInt( index, CHAR_WORKFMINDEXI),
					buf,
					color
				);
				return;			
			}
			else
			
				for(i=0;i<FAMILY_MAXMEMBER;i++){
					tindex = channelMember[fmindexi][channel][i];
          if ( !CHAR_CHECKINDEX( tindex ) )
            continue;
					if( tindex >= 0 && tindex != index) {
						
#ifdef _CHANNEL_MODIFY
            if ( CHAR_getFlg( tindex, CHAR_ISFM ) == FALSE )
              continue;
#endif
						typedef void (*TALKF)(int,int,char*,int,int);
						TALKF   talkedfunc=NULL;
						talk = TRUE;
						talkedfunc = (TALKF)CHAR_getFunctionPointer(tindex,CHAR_TALKEDFUNC);
						
						if( talkedfunc ){
							CHAR_CHAT_DEBUG_SAVESHH( index,index,messageeraseescape,color, channel );
							talkedfunc( tindex,index, messageeraseescape, color, channel );
						}				
					}
				}
		}else {
			for( i=x-range/2 ; i<=x+range/2 ; i++ ){
				for( j=y-range/2 ; j<=y+range/2 ; j++ ){
					OBJECT  object;
					for( object = MAP_getTopObj( fl,i,j ) ; object ; object = NEXT_OBJECT(object) ){
						int objindex = GET_OBJINDEX(object);
						int	toindex = OBJECT_getIndex( objindex);
						if( OBJECT_getType(objindex) == OBJTYPE_CHARA && toindex != index ){
							int k;
							for( k = 0; k < CHAR_PARTYMAX; k ++ ) {
								if( toindex == partyindex[k] ) {
									break;
								}
							}
							if( k != CHAR_PARTYMAX )
								continue;
							if( CHAR_Talk_check( index, toindex, micflg ) ) {
								if( CHAR_getInt( toindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER){
									talk = TRUE;
								}
								typedef void (*TALKF)(int,int,char*,int,int);
								TALKF   talkedfunc=NULL;
								talkedfunc = (TALKF)CHAR_getFunctionPointer( toindex, CHAR_TALKEDFUNC);
								if( talkedfunc ) {
									CHAR_CHAT_DEBUG_SAVESHH( index,index,messageeraseescape,color, channel );
									talkedfunc( toindex,index, messageeraseescape, color, -1 );
								}
#ifdef _ALLBLUES_LUA
								else{
									RunCharTalkedEvent(toindex,index,messageeraseescape,color, -1);
								}
#endif
							}
						}
					}
				}
			}
		}
		if( CHAR_getInt( index, CHAR_POPUPNAMECOLOR) != color ) {
			CHAR_setInt( index, CHAR_POPUPNAMECOLOR, color);
			if( talk) {
				int opt[1];
				opt[0] = color;
				CHAR_sendWatchEvent( CHAR_getWorkInt( index, CHAR_WORKOBJINDEX),
									CHAR_ACTPOPUPNAME,opt,1,FALSE);
			}
		}
		if( talk ) {
			CHAR_setInt(index,CHAR_TALKCOUNT, CHAR_getInt(index,CHAR_TALKCOUNT) + 1);
		}
	}
}

#ifdef _FONT_SIZE
BOOL CHAR_talkToCliExt( int talkedcharaindex,int talkcharaindex,
					 char* message, CHAR_COLOR color, int fontsize )
{
	static char    lastbuf[2048];
	static char    escapebuf[2048];
	static char    mesgbuf[1024];
	int fd;
	int     talkchar=-1;

  if ( !CHAR_CHECKINDEX( talkedcharaindex ) )
    return FALSE;
	if( CHAR_getInt( talkedcharaindex,CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER){
//		print("err CHAR_talkToCli CHAR_WHICHTYPE != CHAR_TYPEPLAYER\n");
		return FALSE;
	}
	
	fd = getfdFromCharaIndex( talkedcharaindex );	
	if( fd == -1 ){
//		print("err CHAR_talkToCli can't get fd from:%d \n", talkedcharaindex);
		return FALSE;    
	}

	if( color < CHAR_COLORWHITE && color > CHAR_COLORGREEN2 ){
		print("CHAR_talkToCli color err\n");
		return FALSE;
	}
#ifndef _CHANNEL_MODIFY
	snprintf( lastbuf, sizeof(lastbuf), "P|%s|%d",
			makeEscapeString( CHAR_appendNameAndTitle(talkcharaindex, message, mesgbuf,sizeof(mesgbuf)),
			escapebuf,sizeof(escapebuf) ), fontsize);
#else
	snprintf( lastbuf, sizeof(lastbuf), "P|P|%s|%d",
			makeEscapeString( CHAR_appendNameAndTitle(talkcharaindex, message, mesgbuf,sizeof(mesgbuf)),
			escapebuf,sizeof(escapebuf) ), fontsize);
#endif
	if( talkcharaindex == -1 )
		talkchar = -1;
	else
		talkchar = CHAR_getWorkInt(talkcharaindex,CHAR_WORKOBJINDEX);
	lssproto_TK_send( fd, talkchar, lastbuf, color);
	return TRUE;
}
BOOL CHAR_talkToCli( int talkedcharaindex,int talkcharaindex,
					 char* message, CHAR_COLOR color )
{
	return CHAR_talkToCliExt( talkedcharaindex, talkcharaindex,
					 message, color, 0 );

}
#else
BOOL CHAR_talkToCli( int talkedcharaindex,int talkcharaindex,
					 char* message, CHAR_COLOR color )
{
	static char    lastbuf[2048];
	static char    escapebuf[2048];
	static char    mesgbuf[1024];
	int fd;
	int     talkchar=-1;

  if ( !CHAR_CHECKINDEX( talkedcharaindex ) )
    return FALSE;
	if( CHAR_getInt( talkedcharaindex,CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER){
//		print("err CHAR_talkToCli CHAR_WHICHTYPE != CHAR_TYPEPLAYER\n");
		return FALSE;
	}
	
	fd = getfdFromCharaIndex( talkedcharaindex );	
	if( fd == -1 ){
//		print("err CHAR_talkToCli can't get fd from:%d \n", talkedcharaindex);
		return FALSE;    
	}

	if( color < CHAR_COLORWHITE && color > CHAR_COLORGREEN2 ){
		print("CHAR_talkToCli color err\n");
		return FALSE;
	}
#ifndef _CHANNEL_MODIFY
	snprintf( lastbuf, sizeof(lastbuf), "P|%s",
#else
	snprintf( lastbuf, sizeof(lastbuf), "P|P|%s",
#endif
			makeEscapeString( CHAR_appendNameAndTitle(talkcharaindex, message, mesgbuf,sizeof(mesgbuf)),
			escapebuf,sizeof(escapebuf) ));
	if( talkcharaindex == -1 )
		talkchar = -1;
	else
		talkchar = CHAR_getWorkInt(talkcharaindex,CHAR_WORKOBJINDEX);
	lssproto_TK_send( fd, talkchar, lastbuf, color);
	return TRUE;
}
#endif // _FONT_SIZE

#ifdef _DROPSTAKENEW
void CHAR_talkToFloor(int floor, int talkindex, char* message, CHAR_COLOR color)
{
	int i = 0; 
	int playernum = CHAR_getPlayerMaxNum();
	for (i = 0; i < playernum; i++){
		if (CHAR_getCharUse(i) == FALSE) continue;
	  if ( !CHAR_CHECKINDEX( i ) )
	    continue;
	  if ( CHAR_getInt( i, CHAR_FLOOR ) == floor ) {
			//if (CHAR_getWorkInt(i, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE)
				CHAR_talkToCli(i, talkindex, message, color);
		}
	}
}
#endif

void CHAR_talkToAll(int talkindex, char* message, CHAR_COLOR color)
{
	int i = 0; 
	int playernum = CHAR_getPlayerMaxNum();
	for (i = 0; i < playernum; i++){
		if (CHAR_getCharUse(i) == FALSE) continue;
	  if ( !CHAR_CHECKINDEX( i ) )
	    continue;
			CHAR_talkToCli(i, talkindex, message, color);
	}
}
#ifdef _CHAR_PROFESSION			// WON ADD ����ְҵ
#ifdef _CHANNEL_MODIFY
// ��ʼ��ְҵƵ��
int InitOccChannel(void)
{
	if(piOccChannelMember == NULL){
		piOccChannelMember = (int*)calloc( 1,(PROFESSION_CLASS_NUM - 1) * getFdnum() * sizeof(int));
		if(piOccChannelMember == NULL){
			printf("Char_talk.c:InitOccChannel() memory allocate failed!\n");
			return 0;
		}
		memset(piOccChannelMember,-1,(PROFESSION_CLASS_NUM - 1) * getFdnum() * sizeof(int));
	}
	return 1;
}
#endif
#endif
#ifdef _TALK_ACTION
void TalkAction(int charaindex, char *message)
{
	int i;
	typedef struct{
		char talk[5];
		int action;
	}tagTlakAction;
	tagTlakAction TlakAction[13]={
		{ "����", 11},{ "����", 12},{ "��ͷ", 18},
		{ "����", 13},{ "����", 14},{ "����", 15},
		{ "վ��", 19},{ "�߶�", 17},{ "�ε�", 10},
		{ "����", 2 },{ "����", 16},{ "����", 4 },
		{ "Ͷ��", 3 }
	};
	for(i=0;i<13;i++)
		if(strstr(message,TlakAction[i].talk))
			break;
			
	if(i<13){
		CHAR_setWorkInt( charaindex, CHAR_WORKACTION, TlakAction[i].action );
		CHAR_sendWatchEvent(CHAR_getWorkInt( charaindex, CHAR_WORKOBJINDEX),	TlakAction[i].action, NULL, 0, TRUE);
	}
}
#endif
#ifdef _GM_ITEM
static BOOL player_useChatMagic( int charaindex, char* data, BOOL isDebug)
{
	char    magicname[256];
	int     ret;
	
	CHATMAGICFUNC   func;

	ret = getStringFromIndexWithDelim( data, " ", 1, magicname,  sizeof( magicname));
	if( ret == FALSE)return FALSE;
	
	func = CHAR_getChatMagicFuncPointer(magicname,isDebug);
	
	if( func ){
		LogGM( CHAR_getUseName( charaindex), CHAR_getChar( charaindex, CHAR_CDKEY), data, 
			CHAR_getInt( charaindex, CHAR_FLOOR), CHAR_getInt( charaindex, CHAR_X),
			CHAR_getInt( charaindex, CHAR_Y) );
		func( charaindex, data + strlen( magicname)+1);
		return TRUE;
	}else{
		return FALSE;
	}
}
#endif

#ifdef _RE_GM_COMMAND
int re_gm_command()
{
	FILE* fp;
#ifdef _CRYPTO_DATA		
	BOOL crypto = FALSE;
	fp = fopen( "data/regmcommand.txt.allblues", "r");
	if( fp != NULL ){
		crypto = TRUE;
	}else
#endif
{
	fp = fopen("data/regmcommand.txt", "r");
}
	if (fp == NULL)
	{
		print("�޷����ļ�\n");
		return FALSE;
	}
	int i;
	while(1){
		char line[1024], buf[128];
		if (fgets(line, sizeof(line), fp) == NULL)break;
#ifdef _CRYPTO_DATA		
		if(crypto==TRUE){
			DecryptKey(line);
		}
#endif
		chop(line);
		if(line[0]=='#')continue;
		getStringFromIndexWithDelim(line,"=", 1, buf, sizeof(buf));
		for( i=0 ; i<arraysizeof(CHAR_cmtbl) ; i++ ){
			if(strcmp(buf, CHAR_cmtbl[i].magicname)==0){
				if(getStringFromIndexWithDelim(line,"=", 2, buf, sizeof(buf)))
					strcpy(CHAR_cmtbl[i].magicname, buf);
				if(getStringFromIndexWithDelim(line,"=", 3, buf, sizeof(buf)))
					CHAR_cmtbl[i].level = atoi(buf);
				CHAR_cmtbl[i].hash = hashpjw( CHAR_cmtbl[i].magicname );
			}
		}
	}
	fclose(fp);
	return TRUE;
}

#endif

#ifdef _FILTER_TALK
char FilterTalk[100][32];
int FilterTalkNum = 0;
int ReadFilterTalk()
{
	FILE* fp;
#ifdef _CRYPTO_DATA		
	BOOL crypto = FALSE;
	fp = fopen( "data/filter.txt.allblues", "r");
	if( fp != NULL ){
		crypto = TRUE;
	}else
#endif
{
	fp = fopen("data/filter.txt", "r");
}
	if (fp == NULL)
	{
		print("�޷����ļ�\n");
		return FALSE;
	}
	while(1){
		char line[1024];
		if (fgets(line, sizeof(line), fp) == NULL)break;
#ifdef _CRYPTO_DATA		
		if(crypto==TRUE){
			DecryptKey(line);
		}
#endif
		chop(line);
		if(line[0]=='#')continue;
		strcpy(FilterTalk[FilterTalkNum], line);

		FilterTalkNum++;
	}
	fclose(fp);
	return TRUE;
}

char *getFilterTalk(int index)
{
	if(index > FilterTalkNum)index=FilterTalkNum;
	return FilterTalk[index];
}

int getFilterTalkNum()
{
	return FilterTalkNum;
}
#endif


