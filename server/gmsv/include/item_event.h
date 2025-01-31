#ifndef __ITEM_EVENT_H__
#define __ITEM_EVENT_H__
#include "item.h"
int ITEM_eventDrop( int itemindex, int charaindex, int itemcharaindex );
BOOL ITEM_MedicineInit( ITEM_Item* itm );
void ITEM_MedicineUsed( int charaindex, int to_charaindex, int itemindex );
void ITEM_SandClockDetach( int charindex , int itemindex );
void ITEM_SandClockLogin( int charaindex );
void ITEM_SandClockLogout( int charaindex );
void ITEM_chantMagicAttack( int charaindex, int itemindex, int toindex, float* damage );
void ITEM_addTitleAttach( int charaindex, int itemindex );
void ITEM_delTitleDetach( int charaindex, int itemindex );
void ITEM_DeleteByWatched( int myobjindex, int moveobjindex, CHAR_ACTION act,
    int x, int y, int dir, int* opt, int optlen );
void ITEM_DeleteTimeWatched( int myobjindex, int moveobjindex, CHAR_ACTION act,
    int x, int y, int dir, int* opt, int optlen );
void ITEM_useEffectTohelos( int charaindex, int to_charaindex, int haveitemindex);
void ITEM_useRecovery( int charaindex, int toindex, int haveitemindex);
void ITEM_WarpDelErrorItem( int charaindex );
void ITEM_useStatusChange( int charaindex, int toindex, int haveitemindex);
void ITEM_useMagicDef( int charaindex, int toindex, int haveitemindex);
void ITEM_useParamChange( int charaindex, int toindex, int haveitemindex);
void ITEM_useFieldChange( int charaindex, int toindex, int haveitemindex);
void ITEM_useAttReverse( int charaindex, int toindex, int haveitemindex);
void ITEM_useStatusRecovery( int charaindex, int toindex, int haveitemindex);
void ITEM_useRessurect( int charaindex, int toindex, int haveitemindex);
void ITEM_useMic( int charaindex, int to_charaindex, int haveitemindex );
void ITEM_dropMic( int charaindex, int itemindex );
void ITEM_useCaptureUp( int charaindex, int toindex, int haveitemindex);
void ITEM_useRenameItem( int charaindex, int toindex, int haveitemindex);
void ITEM_useRenameItem_WindowResult( int charaindex, int seqno, int select, char *data);
void ITEM_dropDice( int charaindex, int itemindex);
void ITEM_pickupDice( int charaindex, int itemindex);
void ITEM_useLottery( int charaindex, int toindex, int haveitemindex);
BOOL ITEM_WarpForAny(int charaindex, int ff, int fx, int fy,int flg);
void ITEM_useWarp( int charaindex, int toindex, int haveitemindex );
void ITEM_petFollow( int charaindex, int toindex, int haveitemindex );

#ifdef _PETFOLLOW_NEW_
void ITEM_petFollowNew( int charaindex, int toindex, int haveitemindex );
#endif

BOOL ITEM_initLottery(ITEM_Item* itm);
void ITEM_useSkup( int charaindex, int toindex, int haveitemindex ); // Nuke 0624
void ITEM_useNoenemy( int charaindex, int toindex, int haveitemindex ); // Nuke 0626
void ITEM_equipNoenemy( int charaindex, int itemindex ); // Arminius 7.2: Ra's amulet
void ITEM_remNoenemy( int charaindex, int itemindex ); // Arminius 7.2: Ra's amulet
BOOL ITEM_getArgument( char* argument , char* entryname , char* buf , int buflen );	// Arminius 7.2: Ra's amulet
void ITEM_useEncounter( int charaindex, int toindex, int haveitemindex); // Arminius 7.31 cursed stone
#ifdef _ITEM_METAMO
void ITEM_metamo( int charaindex, int toindex, int haveitemindex );
void ITEM_ColorMetamo( int charaindex, int toindex, int haveitemindex);
void ITEM_CharaMetamo( int charaindex, int toindex, int haveitemindex);
void ITEM_SexMetamo( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _ITEM_CRACKER
void ITEM_Cracker(int charaindex,int toindex,int haveitemindex);
#endif
#ifdef _DEL_DROP_GOLD
void GOLD_DeleteTimeCheckLoop( void);
#endif
#ifdef _ITEM_ADDEXP	//vincent 经验提升
void ITEM_Addexp(int charaindex,int toindex,int haveitemindex);
#endif
#ifdef _ITEM_REFRESH //vincent 解除异常状态道具
void ITEM_Refresh(int charaindex,int toindex,int haveitemindex);
#endif
//Terry 2001/12/21
#ifdef _ITEM_FIRECRACKER
void ITEM_firecracker(int charaindex,int toindex,int haveitemindex);
#endif
//Terry end

#ifdef _PET_LIMITLEVEL
void ITEM_useOtherEditBase( int charaindex, int toindex, int haveitemindex);
#endif

#ifdef _ITEM_EDITBASES
void ITEM_useFusionEditBase( int charaindex, int toindex, int haveitemindex);
#endif

void ITEM_WearEquip( int charaindex, int itemindex);
void ITEM_ReWearEquip( int charaindex, int itemindex);


#ifdef _ITEM_CONSTITUTION
void ITEM_Constitution( int charaindex, int toindex, int haveitemindex);
#endif

#ifdef _Item_ReLifeAct
void ITEM_DIErelife( int charaindex, int itemindex, int eqw );
#endif

#ifdef _Item_MoonAct
void ITEM_randEnemyEquipOne( int charaindex, int toindex, int haveitemindex);
void ITEM_randEnemyEquip( int charaindex, int itemindex);
void ITEM_RerandEnemyEquip( int charaindex, int itemindex);
#endif

#ifdef _ITEM_ORNAMENTS
void ITEM_PutOrnaments( int charaindex, int itemindex);
#endif

#ifdef _CHIKULA_STONE
void ITEM_ChikulaStone( int charaindex, int toindex, int haveitemindex);
#endif

#ifdef _SUIT_ITEM
void ITEM_suitEquip( int charaindex, int itemindex);
void ITEM_ResuitEquip( int charaindex, int itemindex);
#endif

#ifdef _EQUIT_DEFMAGIC
void ITEM_MagicEquitWear( int charaindex, int itemindex);
void ITEM_MagicEquitReWear( int charaindex, int itemindex);
#endif

#ifdef _EQUIT_RESIST
void ITEM_MagicResist( int charaindex, int itemindex);
void ITEM_MagicReResist( int charaindex, int itemindex);
#endif

#ifdef _MAGIC_RESIST_EQUIT			// WON ADD 职业抗性装备
void ITEM_P_MagicEquitWear( int charaindex, int itemindex );
void ITEM_P_MagicEquitReWear( int charaindex, int itemindex );
#endif


#ifdef _Item_DeathAct
void ITEM_UseDeathCounter( int charaindex, int toindex, int haveitemindex);
#endif

#ifdef _FEV_ADD_NEW_ITEM			// FEV ADD 增加复活守精
void ITEM_ResAndDef( int charaindex, int toindex, int haveitemindex );
#endif

#ifdef _CHRISTMAS_REDSOCKS
void ITEM_useMaxRedSocks( int charaindex, int toindex, int haveitemindex);
#endif

#ifdef _CHRISTMAS_REDSOCKS_NEW
void ITEM_useMaxRedSocksNew( int charaindex, int toindex, int haveitemindex);
#endif

#ifdef _PETSKILL_CANNEDFOOD
void ITEM_useSkillCanned( int charaindex, int toindex, int itemNo);
#endif

#ifdef _NEW_RIDEPETS
void ITEM_useLearnRideCode( int charaindex, int toindex, int haveitemindex);
#endif

#ifdef _ITEM_WARP_FIX_BI
void recoverbi(int index);
#endif

#ifdef _ITEM_TIME_LIMIT
void ITEM_TimeLimit( int charaindex);
#endif

#ifdef _USEWARP_FORNUM
void ITEM_useWarpForNum( int charaindex, int toindex, int haveitemindex );
#endif

#ifdef _IMPRECATE_ITEM
void ITEM_useImprecate( int charaindex, int toNo, int haveitemindex );
#endif

#ifdef _THROWITEM_ITEMS
void ITEM_ThrowItemBox( int charaindex, int toindex, int haveitemindex);
#endif

#ifdef _ITEM_WATERWORDSTATUS
void ITEM_WaterWordStatus( int charaindex, int toindex, int haveitemindex);
#endif

#ifdef _ITEM_LOVERPARTY
void ITEM_LoverSelectUser( int charaindex, int toindex, int haveitemindex);
#endif

#ifdef _ITEM_MAGICRECOVERY
void ITEM_useMRecovery( int charaindex, int toindex, int haveitemindex);
#endif

#ifdef _ITEM_USEMAGIC
void ITEM_useMagic( int charaindex, int toindex, int haveitemindex);
#endif
 
#ifdef _PETSKILL_CANNEDFOOD
void ITEM_usePetSkillCanned_WindowResult( int charaindex, int seqno, int select, char *data);
#endif

void ITEM_AddPRSkillPoint(int charaindex,int toindex,int haveitemindex);
void ITEM_AddPRSkillPercent( int charaindex,int toindex,int haveitemindex);

#ifdef _ANGEL_SUMMON
void ITEM_AngelToken( int charaindex, int toindex, int haveitemindex );
void ITEM_HeroToken( int charaindex, int toindex, int haveitemindex );
#endif
#ifdef _HALLOWEEN_EFFECT
void ITEM_MapEffect(int charaindex,int toindex,int haveitemindex);
#endif
void ITEM_changePetOwner( int charaindex, int toindex, int haveitemindex);

#ifdef _TIME_TICKET
void ITEM_timeticketEx( int charaindex, int toindex, int haveitemindex, int flag);
void ITEM_timeticket( int charaindex, int toindex, int haveitemindex);
#endif

#ifdef _ITEM_SETLOVER
void ITEM_SetLoverUser( int charaindex, int toindex, int haveitemindex);
void ITEM_LoverWarp( int charaindex, int toindex, int haveitemindex);
void ITEM_LoverUnmarry( int charaindex, int toindex, int haveitemindex);
#endif

#ifdef _GM_ITEM
void ITEM_GMFUNCTION( int charaindex, int toindex, int haveitemindex);
#endif

#ifdef _VIP_SERVER
void ITEM_AddMemberPoint( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _VIP_RIDE
void ITEM_VipRide( int charaindex, int toindex, int haveitemindex);
#ifdef _NEW_NAME      //自定义称号
void ITEM_NewName( int charaindex, int toindex, int haveitemindex);
#endif
#endif
#ifdef _FM_ITEM
void ITEM_AddFame( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _LUCK_ITEM
void ITEM_Luck( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _ITEM_METAMO_TIME
void ITEM_MetamoTime( int charaindex, int toindex, int haveitemindex );
#endif
#ifdef _ITEM_GOLD
void ITEM_Gold( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _MYSTERIOUS_GIFT
void ITEM_MysteriousGift( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _BATTLE_PK
void ITEM_BattlePK( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _SILENTZERO
void ITEM_SetSilentZero( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _PET_LEVEL_ITEM
void ITEM_PetLevelItem( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _ITEM_EFMETAMO
void ITEM_efMetamo( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _PET_BEATITUDE
void PET_BEATITUDE( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _GET_MULTI_ITEM
void ITEM_GetMultiItem( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _SUPER_FLOOR_MIC
void ITEM_useSuperMic( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _ITEM_PET_LOCKED
void ITEM_ItemPetLocked( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _ONLINE_COST
void ITEM_OnlineCost( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _VIPPOINT_OLD_TO_NEW
void ITEM_OldToNew( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _BOUND_TIME
void ITEM_BoundTime( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _TALK_SIZE_ITEM
void ITEM_TalkSize( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _FORMULATE_AUTO_PK
void ITEM_PointToSQLPkPoint( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _SUPER_MAN_ITEM
void ITEM_SuperManItem( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _COST_ITEM
void ITEM_CostItem( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _ITEM_EXP
void ITEM_EXP( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _MEMORY_ITEM
void ITEM_WarpItem( int charaindex, int toindex, int haveitemindex );
#endif
#ifdef _NULL_CHECK_ITEM
void ITEM_NullCheck( int charaindex, int toindex, int haveitemindex);
#endif
#ifdef _SPECIAL_SUIT
void ITEM_SpecialSuitEquip( int charaindex, int itemindex);
void ITEM_SpecialResuitEquip( int charaindex, int itemindex);
#endif
#ifdef _MANOR_EQUIP
void ITEM_ManorEquip( int charaindex, int itemindex);
void ITEM_ReManorEquip( int charaindex, int itemindex);
#endif
#ifdef _FIND_TREASURES
void ITEM_FindTreasures( int charaindex, int toindex, int haveitemindex );
BOOL FindTreasures_init();
#endif
#ifdef _ITEM_POOLITEM
void ITEM_PoolItem( int charaindex, int toindex, int haveitemindex );
#endif
#ifdef _ITEM_POOLPET
void ITEM_PoolPet( int charaindex, int toindex, int haveitemindex );
#endif
#ifdef _NEW_GM_ITEM
void ITEM_NewGMItem( int charaindex, int toindex, int haveitemindex );
#endif
#ifdef _SHOW_PET_ABL
void ITEM_ShowPetAbl( int charaindex, int toindex, int haveitemindex );
#endif
#ifdef _NEWEVENT_ITEM
void ITEM_NeweventItem( int charaindex, int toindex, int haveitemindex );
#endif
#ifdef _NEW_PET_BEATITUDE
void ITEM_NewPetBeatitude( int charaindex, int toindex, int haveitemindex );
#endif
#ifdef _OLYMPIC_TORCH
void ITEM_OlympicTorck( int charaindex, int toindex, int haveitemindex );
#endif
#ifdef _PLAYER_DIY_MAP
void ITEM_PlayerDiyMapObj( int charaindex, int toindex, int haveitemindex );
void ITEM_PlayerDiyMapTile( int charaindex, int toindex, int haveitemindex );
#endif
#ifdef _SHOW_ITEM
void ITEM_ShowItem( int charaindex, int toindex, int haveitemindex );
#endif
#ifdef _ITEM_LUA
void ITEM_Lua(int charaindex, int toindex, int haveitemindex);
#endif
enum{
	BD_KIND_HP,
	BD_KIND_MP,
	BD_KIND_CHARM,
	BD_KIND_AI,
	BD_KIND_CURSE,
	BD_KIND_BESTOW,
	BD_KIND_WISHES,
#ifdef _ITEM_UNBECOMEPIG
	BD_KIND_UNBECOMEPIG,
#endif
#ifdef _ITEM_PROPERTY
    BD_KIND_PROPERTY,
#endif
	BD_KIND_END
};

#endif
