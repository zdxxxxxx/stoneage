#ifndef __ALLBLUES_H__
#define __ALLBLUES_H__

#include "longzoro.h"


//˽���汾����----------------------------------------

#if _ATTESTAION_ID == 1
	#define _VERSION_NEW
	#define _VERSION_NEW60
//#define _VERSION_GF
#define _VERSION_60
#define _VERSION_80
//#define _NEW_SA80                // ��ʯ��8.0
#else
#if _ATTESTAION_ID == 2
	#define _VERSION_25
#else
	#define _VERSION_80
#endif
#endif


#ifdef _VERSION_80
	#if _ATTESTAION_ID == 0 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 23 || _ATTESTAION_ID == 28 || _ATTESTAION_ID == 29 || _ATTESTAION_ID == 30 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 32 || _ATTESTAION_ID == 33 || _ATTESTAION_ID == 34 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 36 || _ATTESTAION_ID == 37 || _ATTESTAION_ID >= 39
		#if  _ATTESTAION_ID != 49
			#define _NEW_SA80                // ��ʯ��8.0
		#endif
	#endif
	
	#ifdef _NEW_SA80
		#if _ATTESTAION_ID != 43
			#define _ONLINE_SHOP             // 8.0�����̳�
		#endif
		#define _ONLINE_TALK_IP          // ��Ҿٱ�
		#define _ONLINE_TALK_GM          // ������ϵGM
		#define _NEW_ONLINE_SHOP         // �������̳�
	#endif
	#define _ANGLE_EMAIL             // ���ٺ��Զ�������Ƭ
#endif

#ifdef _VERSION_NEW
	#define _CHATROOMPROTOCOL        // (�ɿ���) Syu ADD ������Ƶ�� 8/27
	#define _OBJSEND_C               // (�ɿ���) ANDY _OBJSEND_C
	#define _ADD_SHOW_ITEMDAMAGE     // (�ɿ���) WON  ��ʾ��Ʒ�;ö�
	#define _CHANGETRADERULE         // (�ɿ���) Syu ADD ���׹����޶�
	#define _TRADE_SHOWHP            // (�ɿ���) Syu ADD ������ʾѪ��
	#define _TRADESYSTEM2            // (�ɿ���) Syu ADD �½���ϵͳ
	#define _NPC_MAKEPAIR            // (�ɿ���) ANDY NPC ���
	#define _ITEM_PILENUMS           // (�ɿ���) ANDY �����زĶѵ� �� itemset4
	#define _ITEM_PILEFORTRADE       // (�ɿ���) ANDY ���׶ѵ�
	#define _ITEM_EQUITSPACE         // (�ɿ���) ANDY ��װ����λ
	#define _EQUIT_ARRANGE           // (�ɿ���) ANDY ���� ��Ҫ itemset5.txt
	#define _EQUIT_ADDPILE           // (�ɿ���) ANDY ���ӿɶѵ��� ��Ҫ itemset5.txt
	#define _EQUIT_NEWGLOVE          // (�ɿ���) ANDY ������λ
	// �¹��ܣ�ְҵ
	#define _NEWREQUESTPROTOCOL      // (�ɿ���) Syu ADD ����ProtocolҪ��ϸ��
	#define _OUTOFBATTLESKILL        // (�ɿ���) Syu ADD ��ս��ʱ����Protocol
	#define _NPC_ADDLEVELUP          // (�ɿ���) ANDY NPC������ҵȼ�
	#define _CHAR_NEWLOGOUT          // (�ɿ���) ANDY �ǳ��»���
	#define _NPC_ProfessionTrans     // (�ɿ�) Change ��ְҵNPC�����ж�ת��
	#define _CHANNEL_MODIFY          // Ƶ����������(����)
	//--------------------------------------------------------------------------
	#define _STREET_VENDOR           // ��̯����(����)
	#define _WATCH_EVENT             // ��ѯ������(����)
	#define _ADD_DUNGEON             // (�ɿ�) Change ׷�ӵ���
	#define _ADD_STATUS_2            // ������������״̬�ڶ���
	#define  _ALCHEMIST              // Robin ��  ����
	#define _JOBDAILY                // cyg ������־����
	#define _FONT_SIZE               // (�ɿ�) Robin ���ʹ�С����
	#define _PETSKILL_FIXITEM        // (�ɿ���) ANDY �޸� && ͬ������
	#define _CHECK_ITEMDAMAGE        // (�ɿ���) ����������  7/26
	#define _TEAM_KICKPARTY          // (�ɿ���) ANDY �ӳ�����(��ս��״̬)
	#define _TELLCHANNEL             // (�ɿ���) Syu ADD ����Ƶ��
	#define _EMENY_CHANCEMAN         // (�ɿ���) ANDY EMENY ѡ��
	#define _STANDBYPET              // Robin ��������
	#define _CHAR_POOLITEM           // (�ɿ���) ANDY ������ֿ߲⹲��
	#define _CHAR_POOLPET            // (�ɿ���) Robin �������ֿ⹲��
	#define _NPC_DEPOTPET            // (�ɿ���) Robin �������ֿ⹲��
	#define _NPC_DEPOTITEM           // (�ɿ���) ANDY ������ֿ߲⹲��
	#define _NEW_MANOR_LAW           // ��ׯ԰����(����)
	#define _CHANNEL_MODIFY          // Ƶ����������(����)
	#define _TAKE_ITEMDAMAGE         // (�ɿ���) ANDY ������  ITEMSET2
#ifdef _VERSION_NEW60
	#define _SKILL_NOCAST            // (�ɿ���) vincent  �輼:��Ĭ //��Ҫ��#define _MAGIC_NOCAST
	#define _PET_FUSION              // (�ɿ���) ANDY �����ں� enemybase1.txt
	#define _NPC_FUSION              // (�ɿ���) ANDY NPC�����ں�
	#define _ITEM_EDITBASES          // (�ɿ���) ANDY Ӫ����
	#define _PET_EVOLUTION           // (�ɿ���) ANDY ������
	#define _STATUS_WATERWORD        // (�ɿ���) ANDY ˮ����״̬
	#define _ITEM_WATERWORDSTATUS    // (�ɿ���) ANDY ˮ����״̬����
	#define _PETSKILL_ACUPUNCTURE    // (�ɿ���) Change �輼:�����Ƥ (client�˵�_PETSKILL_ACUPUNCTUREҲҪ��)
	#define _SHOW_FUSION             // (�ɿ�) Change �ںϳ�����CLIENT��ֻ��ʾת����,�޸�Ϊ�ںϳ���ʾΪ�ں�(client��ҲҪ��)
	#define _TREASURE_BOX            // (�ɿ���) ANDY ����
	#define _SHOOTCHESTNUT           // (�ɿ���) Syu ADD �輼��������
#endif

#endif

#ifdef _VERSION_60
	#define _PETSKILL_FIXITEM        // (�ɿ���) ANDY �޸� && ͬ������
	#define _CHECK_ITEMDAMAGE        // (�ɿ���) ����������  7/26
	#define _TAKE_ITEMDAMAGE         // (�ɿ���) ANDY ������  ITEMSET2
	#define _SKILL_NOCAST            // (�ɿ���) vincent  �輼:��Ĭ //��Ҫ��#define _MAGIC_NOCAST
	#define _EMENY_CHANCEMAN         // (�ɿ���) ANDY EMENY ѡ��
	#define _PET_FUSION              // (�ɿ���) ANDY �����ں� enemybase1.txt
	#define _NPC_FUSION              // (�ɿ���) ANDY NPC�����ں�
	#define _ITEM_EDITBASES          // (�ɿ���) ANDY Ӫ����
	#define _PET_EVOLUTION           // (�ɿ���) ANDY ������
	#define _TEAM_KICKPARTY          // (�ɿ���) ANDY �ӳ�����(��ս��״̬)
	#define _STATUS_WATERWORD        // (�ɿ���) ANDY ˮ����״̬
	#define _ITEM_WATERWORDSTATUS    // (�ɿ���) ANDY ˮ����״̬����
	#define _TELLCHANNEL             // (�ɿ���) Syu ADD ����Ƶ��
	#define _PETSKILL_ACUPUNCTURE    // (�ɿ���) Change �輼:�����Ƥ (client�˵�_PETSKILL_ACUPUNCTUREҲҪ��)
	#define _SHOW_FUSION             // (�ɿ�) Change �ںϳ�����CLIENT��ֻ��ʾת����,�޸�Ϊ�ںϳ���ʾΪ�ں�(client��ҲҪ��)
	#define _FONT_SIZE               // (�ɿ�) Robin ���ʹ�С����
	#define _TREASURE_BOX            // (�ɿ���) ANDY ����
	#define _DP_NPC                  // DP�ű�
	#define _SHOOTCHESTNUT           // (�ɿ���) Syu ADD �輼��������
#endif

#ifdef _VERSION_80
	#define _PETSKILL_FIXITEM        // (�ɿ���) ANDY �޸� && ͬ������
	#define _CHECK_ITEMDAMAGE        // (�ɿ���) ����������  7/26
	#define _TAKE_ITEMDAMAGE         // (�ɿ���) ANDY ������  ITEMSET2
	#define _SKILL_NOCAST            // (�ɿ���) vincent  �輼:��Ĭ //��Ҫ��#define _MAGIC_NOCAST
	#define _EMENY_CHANCEMAN         // (�ɿ���) ANDY EMENY ѡ��
	#define _PET_FUSION              // (�ɿ���) ANDY �����ں� enemybase1.txt
	#define _NPC_FUSION              // (�ɿ���) ANDY NPC�����ں�
	#define _ITEM_EDITBASES          // (�ɿ���) ANDY Ӫ����
	#define _PET_EVOLUTION           // (�ɿ���) ANDY ������
	#define _TEAM_KICKPARTY          // (�ɿ���) ANDY �ӳ�����(��ս��״̬)
	#define _STATUS_WATERWORD        // (�ɿ���) ANDY ˮ����״̬
	#define _ITEM_WATERWORDSTATUS    // (�ɿ���) ANDY ˮ����״̬����
	#define _TELLCHANNEL             // (�ɿ���) Syu ADD ����Ƶ��
	#define _PETSKILL_ACUPUNCTURE    // (�ɿ���) Change �輼:�����Ƥ (client�˵�_PETSKILL_ACUPUNCTUREҲҪ��)
	#define _SHOW_FUSION             // (�ɿ�) Change �ںϳ�����CLIENT��ֻ��ʾת����,�޸�Ϊ�ںϳ���ʾΪ�ں�(client��ҲҪ��)
	#define _FONT_SIZE               // (�ɿ�) Robin ���ʹ�С����
	#define _TREASURE_BOX            // (�ɿ���) ANDY ����
	#define _DP_NPC                  // DP�ű�
	
	//#define _PREVENT_TEAMATTACK      // (�ɿ���) ANDY ��ֹͬ�ӻ��� 12/04 ����
	#define _OBJSEND_C               // (�ɿ���) ANDY _OBJSEND_C
	#define _ADD_SHOW_ITEMDAMAGE     // (�ɿ���) WON  ��ʾ��Ʒ�;ö�
	#define _CHANGETRADERULE         // (�ɿ���) Syu ADD ���׹����޶�
	#define _TRADE_SHOWHP            // (�ɿ���) Syu ADD ������ʾѪ��
	#define _SHOOTCHESTNUT           // (�ɿ���) Syu ADD �輼��������
	#define _SKILLLIMIT              // (�ɿ���) Syu ADD ���ù����ҷ�����
	#define _TRADESYSTEM2            // (�ɿ���) Syu ADD �½���ϵͳ
	#define _NPC_MAKEPAIR            // (�ɿ���) ANDY NPC ���
	#define _ITEM_PILENUMS           // (�ɿ���) ANDY �����زĶѵ� �� itemset4
	#define _ITEM_PILEFORTRADE       // (�ɿ���) ANDY ���׶ѵ�
	#define _ITEM_EQUITSPACE         // (�ɿ���) ANDY ��װ����λ
	#define _EQUIT_ARRANGE           // (�ɿ���) ANDY ���� ��Ҫ itemset5.txt
	#define _EQUIT_ADDPILE           // (�ɿ���) ANDY ���ӿɶѵ��� ��Ҫ itemset5.txt
	#define _EQUIT_NEWGLOVE          // (�ɿ���) ANDY ������λ
	// �¹��ܣ�ְҵ
	#define _NEWREQUESTPROTOCOL      // (�ɿ���) Syu ADD ����ProtocolҪ��ϸ��
	#define _OUTOFBATTLESKILL        // (�ɿ���) Syu ADD ��ս��ʱ����Protocol
	#define _CHAR_PROFESSION         // (�ɿ���) WON ADD ����ְҵ��λ
	#define _PROFESSION_SKILL        // (�ɿ���) WON ADD ����ְҵ����
	#define _NPC_WELFARE             // (�ɿ���) WON ADD ְҵNPC
	#define _NPC_ADDLEVELUP          // (�ɿ���) ANDY NPC������ҵȼ�
	#define _CHAR_NEWLOGOUT          // (�ɿ���) ANDY �ǳ��»���
	#define _CHATROOMPROTOCOL        // (�ɿ���) Syu ADD ������Ƶ�� 8/27
	#define _CHAR_POOLITEM           // (�ɿ���) ANDY ������ֿ߲⹲��
	#define _CHAR_POOLPET            // (�ɿ���) Robin �������ֿ⹲��
	#define _NPC_DEPOTPET            // (�ɿ���) Robin �������ֿ⹲��
	#define _NPC_DEPOTITEM           // (�ɿ���) ANDY ������ֿ߲⹲��
	#define _NPC_ProfessionTrans     // (�ɿ�) Change ��ְҵNPC�����ж�ת��
	#define _CHANNEL_MODIFY          // Ƶ����������(����)
	#define _STANDBYPET              // Robin ��������
	#define  _PROSKILL_OPTIMUM       // Robin ְҵ���ܴ�����ѻ�
	//2004/05/12
	
	#define _MAGIC_RESIST_EQUIT      // (�ɿ�) WON ADD ְҵ����װ��
	#define _ALLDOMAN                // (�ɿ���) Syu ADD ���а�NPC
	#define _ALLDOMAN_DEBUG          // (�ɿ�) Change ��������server�����а���ʧbug
	// -------------------------------------------------------------------
	// �¹��ܣ�GM��Ƶ��������
	//���̰�
	#define _REDHAT_V9               //(�ɿ�) Change ��REDHAT9.0�İ汾�±���ʱ��
	//--------------------------------------------------------------------------
	#define _NPC_EXCHANGEMANTRANS    // (�ɿ�) Change ��exchangeman�����ж�ת��
	#define  _ANGEL_SUMMON           // Robin ��ʹ�ٻ�
	#define _STREET_VENDOR           // ��̯����(����)
	#define _WATCH_EVENT             // ��ѯ������(����)
	#define _NEW_MANOR_LAW           // ��ׯ԰����(����)
	#define _PETSKILL_LER            // �׶�����(����)
	#define _FIX_MAGIC_RESIST        // (�ɿ�) Change ְҵħ������
	#define _ADD_DUNGEON             // (�ɿ�) Change ׷�ӵ���
	#define _PROFESSION_ADDSKILL     // (�ɿ�) Change ׷��ְҵ����
	#define _EQUIT_RESIST            // (�ɿ�) Change ��һװ������(�ɿ���һ�����쳣״̬)�п��� �� �� ���� ħ�� ��Ĭ ����
	#define _PROSK99                 // (�ɿ�) Change ���ְ���ȼ��Ͱ���
	#define _ADD_STATUS_2            // ������������״̬�ڶ���
	#define _SUIT_ADDPART4           // (�ɿ�) Change ��װ���ܵ��ĵ� �ж�����% ��ǿ��ʦħ��(����30%) �ֿ���ʦ��ħ��


	#define _ITEM_TYPETABLE          // (�ɿ�) Change ׷��typetable
	#define _RIGHTCLICK              // Robin �����Ҽ�����
	#define _JOBDAILY                // cyg ������־����
	#define  _ALCHEMIST              // Robin ��  ����
	#define _TEACHER_SYSTEM          // ��ʦϵͳ(����)
	#define _PET_ITEM                // ����װ��
#endif


//��ͨ����������-------------------------------------
#define _PUB_ALL
#ifdef _PUB_ALL
	//#define _PROSKILL_ERR_KICK       // �Ƿ������Զ�T����
	#define _ITEM_SETLOVER           // ��鹦��
	#define _CREATE_MM_1_2           // ������Ʒ���ٷ���
	#define _SendTo                  // ���͵�������
	#define _GET_BATTLE_EXP          // ��������
	#define _NEW_PLAYER_CF           // ����״̬����
	#define _USER_EXP_CF             // �û��Զ��徭��
	#define _TRANS_LEVEL_CF          // ת����Խ160������
	#define _UNLAW_WARP_FLOOR        // ��ֹ���͵���
	#define _UNREG_NEMA              // ��ֹ��������
	#if _ATTESTAION_ID != 1
		#define _GM_ITEM                 // ���GM������Ʒ
	#endif
	#define _GM_METAMO_RIDE          // GM���������г�
	#ifdef _NEW_RIDEPETS
		#define _GM_RIDE                 // GM�����������Ȩ��
	#endif
	#define _LOCK_IP                 // ����IP
	#define _POINT                   // ��������
	#define _VIP_SERVER              // ��Ա����
	#define _WATCH_FLOOR             // �Զ���ɹ�ս��ͼ
	#define _BATTLE_FLOOR            // �Զ���ǿ��ս����ͼ
	#define _VIP_SHOP                // ��Ա�̵�
	#if _ATTESTAION_ID != 1
		#define _LOOP_ANNOUNCE           // ѭ������
	#endif
	#define _SKILLUPPOINT_CF         // �Զ�����������
	#define _RIDELEVEL               // �Զ���������ȼ�
	#define _REVLEVEL                // �Զ���ԭ�ȼ�
	#define _FM_ITEM                 // ��������
	#define _LUCK_ITEM               // ��������
	#define _NEW_PLAYER_RIDE         // �������������
	#define _FIX_CHARLOOPS           // ������ħ��ʯ����ʱ��
	#if _ATTESTAION_ID != 1
		#define _PLAYER_ANNOUNCE         // �������
	#endif
	#define _BATTLE_GOLD             // ��ȡ����ͬʱ��ý�Ǯ
	#define _ITEM_GOLD               // ��Ǯ����Ʒ
	#define _PLAYER_NUM              // �����������
	#define _PET_MM                  // �����ʯMM
	#define _MYSTERIOUS_GIFT         // ��������
	#define _RELOAD_CF               // �ض�CF
	#define _PET_AND_ITEM_UP         // �ɼ���˶����ĳ�������
	#define _TRANS                   // ����ת������
	#ifdef _NEW_RIDEPETS
		#define _RIDEMODE_20             // 2.0���ģʽ
	#endif
	#define _TALK_ACTION             // ˵������
	#define _ENEMY_ACTION            // ��Ҳ������л���
	#define _CHAR_CDKEY              // ��ֹ��������
//	#define _GMSV_DEBUG              // GMSV������Ϣ
	#define _CHECK_PEPEAT            // �Ƿ�������
	#define _SILENTZERO              // ������Ʒ
	#define _SAVE_GAME_ID            // ��ʾ����ķ�������
#endif

//��Ա��׷�ӹ���--------------------------------------
#define _VIP_ALL
#ifdef _VIP_ALL
	#define _NEW_RIDEPETS            // (�ɿ���) ANDY �����
	#define _RIDEMODE_20             // 2.0���ģʽ
	#define _VIP_RIDE                // ��Ա��ڼ�
	#define _AUTO_PK                 // �Զ���PKϵͳ
	//#define _FM_METAMO               // ����ר�ñ����ָ
	#define _ITEM_METAMO_TIME        // ���Զ������Ľ�ָ
	#define _SHOW_VIP_CF             // �Զ������VIP�����ˣ����壬������ʾ��ʽ
	#define _GM_KING                 // GM��ħ��
	#define _ANGEL_TIME              // �Զ��徫���ٻ�ʱ��
	#define _FM_POINT_PK             // ׯ԰���廥��ׯ԰
	#if _ATTESTAION_ID != 1
		#define _PLAYER_MOVE             // ���˳�ƹ���
	#endif
	#define _BATTLE_PK               // ǿ��PK���ߣ����˻ؼ�¼��
	#ifdef _PET_FUSION
		#define _FUSIONBEIT_TRANS        // �ںϳ���ת��
		#define _FUSIONBEIT_FIX          // ȡ���ںϳ������
	#endif
	#define _TRANS_7_NPC             // ������תNPC
	#ifdef _NEW_MANOR_LAW
		#define _MOMENTUM_NPC            // ���ƽű�
	#endif
	
	#ifdef _NEW_RIDEPETS
		#define _RIDE_CF                 // �Զ������
		#define _FM_LEADER_RIDE          // �峤ר�����
	#endif
	#define _PET_LEVEL_ITEM          // ����ȼ�������Ʒ
	//#define _DBSAVE_CHAR             // ˫��浵
	#define _FM_JOINLIMIT            // ������ٴμ�������ʱ������
	#define _PET_BEATITUDE           // ����ף��
	#define _ITEM_EFMETAMO           // ���ñ����ָ
	#define _ITEM_RIDE               // ѱ��װ��
	#define _MAKE_MAP                // ��ͼ����
	#define _MAKE_PET_CF             // �Զ������ɳ�
	#define _MAKE_PET_ABILITY        // �Զ��������Χ
	#define _GET_MULTI_ITEM          // һ�θ������Ʒ
	#define _RE_GM_COMMAND           // �Զ���GM��������
	#define _ONLINE_TIME           // ����ʱ��
	#define _NPC_INT_SET           // NPC��������
	#define _VIP_ONLINE              // ��Ա������ʾ

#endif

#if _ATTESTAION_ID != 9 && _ATTESTAION_ID != 11
	#define _LOGNZORO_FUNC_ALL       // ��zoro��������
#else
	#define _NEW_VIP_SHOP            // ��Ա�̵�(��SAAC���)
	#define _ONLINE_COST             // ���߳�ֵ(��SAAC���)
	#define _NULL_CHECK_ITEM         // ��ͷ֧Ʊ����
	#define _COST_ITEM               // ��ֵ����
	#define _CRYPTO_DATA             // ��������ݼ���ϵͳ
#endif
#ifdef _LOGNZORO_FUNC_ALL
	#if _ATTESTAION_ID != 1
		#define _TRANS_7_SHOW            // ����7ת��ʾ
	#endif
	#define _ITEM_COLOER             // ��Ʒ��ɫ����
	#define _TALK_MOVE_FLOOR         // ˵���ƶ�����
	#ifdef _TRADESYSTEM2
		#define _TRADE_PK                // ����PK
	#endif
	#if _ATTESTAION_ID != 1
		#define _VIP_POINT_PK            // ���ֵ�PK
	#endif
	#define _MAP_HEALERALLHEAL       // ָ����ͼս�����Զ��ظ�����
	#if _ATTESTAION_ID != 1
		#define _NEW_AUTO_PK             // �µ��Զ���PKϵͳ
	#endif
	#define _OPEN_E_PETSKILL         // ���ų���E����
	#if _ATTESTAION_ID != 1
		#define _VIP_ONLINE              // ��Ա������ʾ
	#endif
	#define _VIP_LOGOUT              // ��Ա�������ʱ����ʾ
	#if _ATTESTAION_ID != 1
		#define _THE_WORLD_SEND          // ����˵��Ƶ��
	#endif
#define _THE_WORLD_SEND          // ����˵��Ƶ��
	#define _SUPER_FLOOR_MIC         // ��ͼ��˷�
	#define _NPC_GET_PETABI          // NPC���Զ���ɳ��ĳ���
	#define _SAMETHING_SAVEPOINT     // ���ĳЩ�¼��Զ��浵
	#define _NEW_VIP_SHOP            // ��Ա�̵�(��SAAC���)
	#if _ATTESTAION_ID != 8 && _ATTESTAION_ID != 10 && _ATTESTAION_ID != 21 
		#define _ITEM_PET_LOCKED         // ��Ʒ������(��SAAC���)
	#endif
	#define _ONLINE_COST             // ���߳�ֵ(��SAAC���)
	#if _ATTESTAION_ID != 3
		#define _VIPPOINT_OLD_TO_NEW     // ���ֵ��ת��(��SAAC���)
	#endif
	#define _LOGIN_DISPLAY           // ���������ʾ����
	//#define _NEED_ITEM_ENEMY         // ץ��������Ʒ�Զ���
	#define _SUPER_MAN_ITEM          // ��Ʒ�˵���
	#define _AUTO_DEL_PET            // �Զ�ɾ������
	#define _AUTO_DEL_ITEM           // �Զ�ɾ����Ʒ
	#define _POOL_ITEM_BUG           // ��ֹǿ���BUG
	
	#if _ATTESTAION_ID != 5
		#if _ATTESTAION_ID != 1
			#define _BOUND_TIME              // ��Ұ�ʱ����
		#endif
		#define _SPECIAL_MAP             // �����ͼ
		#ifdef _FONT_SIZE
			#define _TALK_SIZE_ITEM          // ����������
		#endif
		#define _NEW_NAME                // �Զ���ƺ�
		#define _CRYPTO_DATA             // ��������ݼ���ϵͳ
		#ifdef _NEW_VIP_SHOP
			#define _COST_ITEM             // ��ֵ����
		#endif
		#define _ITEM_EXP                // ����ҩˮ(�������)
		#define _NULL_CHECK_ITEM         // ��ͷ֧Ʊ����
		#define _MEMORY_ITEM             // �����ָ
		#define _NPC_SCRIPT              // NPC�ű���
		//#define _BT_PET                  // BT���ﱶ������
		#define _BT_ITEM                 // BT��Ʒ��������
		#define _NO_JOIN_FLOOR           // ��ֹ���͵���
		#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 4 || _ATTESTAION_ID == 6 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 10 || _ATTESTAION_ID == 14 || _ATTESTAION_ID == 15 || _ATTESTAION_ID == 17 || _ATTESTAION_ID >= 19
			#ifdef _STREET_VENDOR
				#define _NEW_STREET_VENDOR       // �°�̯����
			#endif
		#endif

		#if _ATTESTAION_ID != 1 && _ATTESTAION_ID != 18
			#define _LUCK_STAR               // ������(�����ʱ�������������Ʒ�����)
		#endif
		#if _ATTESTAION_ID != 1 
			#define _BT_ITEM                 // BT��Ʒ��������
		#endif
		#ifdef _CHATROOMPROTOCOL
			#define _NPC_SAVEPOINT           // ��¼�����������
		#endif
		#define _SPECIAL_SUIT            // ������װ
		#define _MANOR_EQUIP             // ׯ԰ר����װ
		#define _ITEM_UPLEVEL            // װ������
		#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 4 || _ATTESTAION_ID == 6 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 10 || _ATTESTAION_ID == 14 || _ATTESTAION_ID == 15 || _ATTESTAION_ID >= 17
			//#define _TRANS_POINT_UP          // ÿת��������
			#if _ATTESTAION_ID != 8
				#if _ATTESTAION_ID != 1 
					#define _FIND_TREASURES          // Ѱ�ұ���
				#endif
				#define _UNLAW_THIS_LOGOUT       // ��ֹԭ��
				#define _ITEM_PERCENTAGE         // ������Ʒ�ٷֱ�
			#endif
			#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 4 || _ATTESTAION_ID == 6 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 10 || _ATTESTAION_ID == 14 || _ATTESTAION_ID == 17 || _ATTESTAION_ID >= 20 
				#define _ITEM_POOLITEM           // ������ֿ߲�
				#define _ITEM_POOLPET            // �������ֿ�
				#define _ADD_NEWEVENT_1024       // ���������1024
				#define _SUPER                   // ���ƶ˵���������֧��
				#define _NEW_SKILL               // �¼��ܿ���
				#if _ATTESTAION_ID != 1
					#define _NEW_GM_ITEM             // �����GM������Ʒ
				#endif
				#define _SHOW_PET_ABL            // ��ʾ��������
				#define _NEWEVENT_ITEM           // �����������
				#ifdef _NEW_RIDEPETS
					#define _ADD_RIDE_CF             // �����Զ����������
				#endif
				#define _NO_STW_ENEMY            // ��ֹ��ҿ�������
				#define _NEW_PET_BEATITUDE       // ��һ������ף��
				#define _BATTLE_GETITEM_RATE     // ս�������Ʒ����
				#define _INSLAY_NOT_PILENUMS     // ���ܾ���������Ʒ
				#if _ATTESTAION_ID == 14 || _ATTESTAION_ID == 28
					#define _ONLINE_TIME           // ����ʱ��
					#define _NPC_INT_SET           // NPC��������
				#endif
				#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 4 || _ATTESTAION_ID == 6 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 14 || _ATTESTAION_ID == 20 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 25
					#if _ATTESTAION_ID != 1 || _ATTESTAION_ID != 25 
						#define _TALK_SAVE               // ˵���浵
						#define _PLAYER_QUESTION_ONLIEN  // ������ߴ���
					#endif
				#endif
				#if _ATTESTAION_ID == 28
					#define _LOTTERY_SYSTEM          // ��Ʊϵͳ
					#define _NPC_CHECK_SCRIPT          // �жϽű���
				#endif
				#if _ATTESTAION_ID == 38
					//#define _PET_TRANS_ABILITY       // ����ת��������������
				#endif
				#if _ATTESTAION_ID != 38 && _ATTESTAION_ID != 49
					#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 30
						#define _OLYMPIC_TORCH           // ����ƥ�˻��
						#if _ATTESTAION_ID != 1
							#define _LOTTERY_SYSTEM          // ��Ʊϵͳ
							#define _PLAYER_DIY_MAP          // ���DIY��ͼ����
						#endif
						#define _AMPOINT_LOG             // ���ֵ��¼
						#define _SQL_VIPPOINT_LOG        // �ػرҼ�¼
						#define _NPC_CHECK_SCRIPT        // �жϽű���
						#define _TALK_CHECK              // ˵����֤
						#define _ONE_PET_SKILL           // һ���Գ輼
						#define _MAGIC_SPECIAL           // ����ħ��
						#if _ATTESTAION_ID != 1 
							#define _BATTLE_PK_TYPE          // PK����ѡ��
						#endif
						#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 31
							#if _ATTESTAION_ID != 1
								#define _NPC_AUTO_MOVE			   // �ܹ������ƶ���NPC
							#endif
							#define _SYSTEM_SPEAD          // ϵͳ����  �ݲ����ţ���Ӱ��ս���һ�
							#define _FILTER_TALK    		   // ����˵��
							#if _ATTESTAION_ID != 34
									#define _ALL_SERV_SEND       // ����Ƶ��
									#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 32 || _ATTESTAION_ID == 33 || _ATTESTAION_ID >= 35 || _ATTESTAION_ID != 39
										#define _PET_TRANS_ABILITY       // ����ת��������������
										#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 32 || _ATTESTAION_ID == 35 || _ATTESTAION_ID >= 45
											#define _CANCEL_ANGLE_TRANS      // ȡ�������ٻ�
										#endif
										#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 33 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 36 || _ATTESTAION_ID == 37 || _ATTESTAION_ID == 40 || _ATTESTAION_ID >= 43
											#define _COMPOUND_EXP          // �ϳɾ���
											#define _ASSESS_ABILITY        // ��������
											#define _ONLINE_TIME           // ����ʱ��
											#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 37 || _ATTESTAION_ID >= 43
												#ifndef _VERSION_25
													#define _PAUCTION_MAN          // ����ϵͳ
												#endif
											#endif
											#define _NOT_ESCAPE          	 // ��ֹ����
											#define _PLAYER_OVERLAP_PK     // ����ص�ʱPK
											#define _FIMALY_PK_TIME        // ���忪Ƭʱ���趨
										#endif
									#endif
							#endif
						#endif
					#endif
				#endif
			#endif
		#endif
	#else
		#define _FORMULATE_AUTO_PK       // �ƶ��Զ���PKϵͳ(��SAAC���)
	#endif
	
	#ifdef _MAGIC_SPECIAL
//		#define _MAGIC_FEATHERS          // ħ����ë
	#endif
	
	#ifdef _TALK_CHECK
		#define _NPC_TALK_CHECK          // NPC˵����֤
		#define _ITEM_TALK_CHECK         // ����˵����֤
	#endif
	
	#ifdef _NPC_SCRIPT
		#define _NPC_CHECKTIME           // NPCʱ���ж�
		#define _FAME_CHECK_NPC          // NPC�����ж�
		#define _SQL_BUY_FUNC            // SQL������
		#define _DEL_FAME                // �ۼ�����
		#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 36 || _ATTESTAION_ID == 37 || _ATTESTAION_ID == 40 || _ATTESTAION_ID >= 43
			#define _NPC_INT_SET             // NPC��������
		#endif
	#endif

	#ifdef _NEW_SKILL
		#define _ITEM_ATTSKILLMAGIC      // (�ɿ���) ANDY ���߼���
		#define _MASSAGE_PETSKILL        // ��ɱ������
		#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 30
			#define _EXPANSION_ITEM_INSLAY      // ��չ�ⱦʯ
			#define _STRENGTH_PETSKILL          // Ԫ���輼
			#define _EXPANSION_POWERBALANCE     // ��չPETSKILL_PowerBalance
			#define _RESURRECTION_PETSKILL      // ����輼
			#define _LOSTLOST_PETSKILL          // ���ܾ���
			#define _GRAPPLING_PETSKILL         // ������
			#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 31
				#define _PETSKILL_EXPLODE		        // (���ɿ�) Change �輼:���ѹ���
				#define _PETOUT_PETSKILL		        // ����Ϣ�輼
				#define _FIX_PETSKILL_HECTOR		    // �޸��ü����޷�����turn
			#endif
		#endif
	#endif
#endif

#if _ATTESTAION_ID == 1 && _ATTESTAION_ID != 2 && _ATTESTAION_ID != 3 && _ATTESTAION_ID != 5 && _ATTESTAION_ID != 6 && _ATTESTAION_ID != 9 && _ATTESTAION_ID != 10 && _ATTESTAION_ID != 11 && _ATTESTAION_ID != 14 && _ATTESTAION_ID != 15 && _ATTESTAION_ID != 19 && _ATTESTAION_ID != 26 && _ATTESTAION_ID != 27 && _ATTESTAION_ID != 28 &&  _ATTESTAION_ID != 31 && _ATTESTAION_ID != 33 && _ATTESTAION_ID != 34 && _ATTESTAION_ID != 36 && _ATTESTAION_ID != 38 && _ATTESTAION_ID != 39 && _ATTESTAION_ID != 45
//	#define MANOR_MAX_NUM 			2        // (4.0) ����ݵ�
#else
	#define MANOR_MAX_NUM       4        // (4.0) ׯ԰����
#endif

#if _ATTESTAION_ID != 5 && _ATTESTAION_ID != 11
	//#define FAMILY_MAX_MEMBER 			50 	// ��������
#else
	#define FAMILY_MAX_MEMBER       50		// ��������
#endif

#define _FIX_COPY_MM_BUG         //�޸�����MM��BUG
#define _FIX_PETSKILL_BUG        //�޸����＼���ص�ʹ��

#if _ATTESTAION_ID != 3 && _ATTESTAION_ID != 5 && _ATTESTAION_ID != 11
//#define _CHARADATA_SAVE_SQL      //������д��SQL
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 23 || _ATTESTAION_ID == 28 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 33 || _ATTESTAION_ID == 34 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 36 || _ATTESTAION_ID == 37 || _ATTESTAION_ID >= 40
	#if _ATTESTAION_ID != 49
		#define _SASQL                // ����MYSQL����
	#endif
#endif
#ifdef _SASQL
#ifdef _STREET_VENDOR
	#if _ATTESTAION_ID != 43
		#if _ATTESTAION_ID != 28 && _ATTESTAION_ID != 34
			#define _ONLINE_SHOP_MYSQL       // �����̳Ǹĳ�MYSQL�ػر�
		#endif
		#define _STREET_VENDOR_MYSQL     // MYSQL�ػرҰ�̯
	#endif
#endif
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 23 || _ATTESTAION_ID == 33
	#define _EXPANSION_VARY_WOLF      // ��չ���±���
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 21
	#define _SHOW_ITEM                // ��װ��
	#define _SHOW_ITEM_NAME           // ��װ�������
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32
#ifdef _VERSION_80
	#define _DISABLE_PROFESSION_SKILL      // ��ֹʹ��ְҵ���ܼ�
	#define _TAKE_ITEMDAMAGE_FIX           // ����װ����
	#define _TAKE_ITEMDAMAGE_FOR_PET       // ��װװ����
#endif
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 35
	#define _GM_SAVE_ALL_CHAR      // GM����浵
#endif

#if _ATTESTAION_ID == 35
	//#define _SHOW_FMPOINT					//��ʾׯ԰
	#define _NOT_FMPK_WAIT				//��ս����ս��
#endif

#if _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 16 || _ATTESTAION_ID == 33 || _ATTESTAION_ID == 34 || _ATTESTAION_ID == 41
	#ifdef _BOUND_TIME
		#define _VIP_BOUND_TIME
		#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 41
			#define _BOUND_TIME_EXPANSION      // ��ʱ������չ
		#endif
	#endif
#endif

//#define _ABSOLUTE_DEBUG              // ���Ե���


#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 14 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 28 || _ATTESTAION_ID == 30 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 32 || _ATTESTAION_ID == 33 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 36 || _ATTESTAION_ID == 37 || _ATTESTAION_ID == 40 || _ATTESTAION_ID == 44 || _ATTESTAION_ID == 45 || _ATTESTAION_ID == 46
	#define _ALLBLUES_LUA      // ʹ��LUA�ű�
#endif

#ifdef _ALLBLUES_LUA
	#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32 
		#define _ALLBLUES_LUA_1_1      // LUA1.1��
	#endif
	#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32 
		#define _ALLBLUES_LUA_1_2      // LUA1.2��
		#define _ALLBLUES_LUA_1_3      // LUA1.3��
	#endif
	#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 32 
		#define _ALLBLUES_LUA_1_4      // LUA1.4��
	#endif
	
	#if  _ATTESTAION_ID == 1
		#define _ALLBLUES_LUA_1_5      // LUA1.5��
	#endif
	
	#if  _ATTESTAION_ID == 1
		#define _ALLBLUES_LUA_1_6      // LUA1.6��
	#endif
	#if  _ATTESTAION_ID == 1
		#define _ALLBLUES_LUA_1_7      // LUA1.7��
	#endif
	#if  _ATTESTAION_ID == 1
		#define _ALLBLUES_LUA_1_8      // LUA1.8��
	#endif
	
	#if  _ATTESTAION_ID == 1
		#define _ALLBLUES_LUA_1_9      // LUA1.9��
	#endif
	
	#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32 
		#define _PETSKILL_SHOP_LUA       // ���＼���̵�LUA
	#endif
	#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32 
		#define _OFFLINE_SYSTEM       // ����ϵͳ
	#endif
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3  || _ATTESTAION_ID == 32 
	#define _PLAYER_NPC      // ������
#endif

#if _ATTESTAION_ID == 0 || _ATTESTAION_ID == 3 
	#define _OTHER_SAAC_LINK  // ��һ��SAAC����
#endif

#if _ATTESTAION_ID == 1
	#define _ONE_SERVER       // ������
#endif

#if _ATTESTAION_ID == 32
	#define _NOT_NOCRYPTO_LUA // ���ö�ȡ�Ǽ���LUA�ű�
#endif

#if _ATTESTAION_ID == 1
	#define _CRYPTO_LUA    		// lua�ű�����
#endif

#if _ATTESTAION_ID == 1
	#define _CAMEO_MONEY      		// ���ǻ���
	#define _MISSION_TRAIN    		// ������
	#define _DEL_DROP_PET    			// ɾ�����ϳ���
	#define _PET_EVOLVE    				// �������
	//#define _PET_ADD_EXP      		// ���ﾭ����������
	#define _FIX_ADD_EXP      		// �޸����Ӿ������
	#define _FMRIDE_NOT_FMSPRITE	// ������費�ֹ����ڰ�
	#define _NOT_SHOW_ONLINE_NUM	// ����ʾ��������
	#define _ITEM_COLOER_DIFFER		// ������ɫ��ͬ
	#define _SPECIAL_COUNTER			// ������
	#define _SPECIAL_TIMER				// ��ʱ��
	#define _RECORD_IP						// ��¼IP
	#define _FIX_STW_SPEED_ENEMY	// �޸���ҿ�������
	//#define _DEL_NOT_25_NEED_ITEM	// ɾ����2.5�����ץ��
	#define _LOCK_PET_ITEM				// �󶨳��������
	#define _ULTIMATE_ANNOUNCE		// ����֪ͨ
	#define _SHIP_MATEMO					// ���ı�������
	#define _FIX_CORE_LOOP				// �޸�CORE_LOOP
	#define _VIP_BATTLE_EXP				// ��Աս������
	#define _FLOOR_DIFI_FIELD			// ¥���ֶ�
	#define _FM_FMPOINTPK_LIST		// ׯ԰��ս�б�
	//#define _BATTLE_BOUT_TIME			// ս���غ�ʱ��
	#define _ACTIVE_GAME			    // ����ֵ
	#define _INVERSION_PETSKILL   // ��ת����
	#define _NO_HELP_MAP					// ��ֹHELP
	#define _BATTLE_TIME					// ս��ʱ��
	//#define _SAME_IP_ONLINE_NUM		// ͬIP��½��
	#define _VIP_TRANS						// VIPת������
	#define _STREET_VENDOR_TRANS	// ��̯����ת��
	#define _KEEP_UP_NO_LOGIN			// ά����ֹ��½
	#define _FIX_CHAR_LOOP				// ��������ѭ��
	#define _FIX_SAVE_CHAR				// �������浵����
	#define _FIX_P_VS_P_PARENT		// ����PVP����ս��
	#define _FMPOINT_PKTIME_SELECT// ����ѡ����սʱ��
	#define _CHECK_SEVER_IP				// �жϷ�����IP
	#define _DAMMAGE_CALC					// �Զ����˺�
	#define _PLAYER_TITLE					// ��ҳƺ�
	#define _VIP_PET_TRANS				// ��Աת��
	//#define _UNIFIDE_MALINASI			// ͳһ������˿������
	//#define _FIX_PET_SKILL				// �޸����＼��
	//#define _NO1_B2_NONO					// B2ѧ�ڵ�һλʱ�Զ�����21��
	//#define _FMPOINT_APPEND_ABI		// ׯ԰�����Ա��������
	#define _ITEM_OVER_LAP				// �����ظ�����
	#define _NEW_LOAD_NPC					// ��ȡNPC
	#define _NEW_LOAD_MAP					// ��ȡ��ͼ
	#define _NO_CLS_EVENT_255			// �����255������
	#define _MISSION_TIME					// �����ʱ
	#define _MVP_LOCK_JP					// ��MVP��Ʒ��
	//#define _FMPOINTPK_FOR_TWO		// ÿ����ս���Ϊ����
	#define _BURST_LOG						// �ֶ���־
	#define _STREET_VENDOR_EFFECT // ��̯������ʾ
	//#define _MASK_ENCOUNTER 			// ����ԭ�ط��
	#define _NEWCLISETMAC		 			// ��¼�ͻ��˵�MAC��ַ
	#define _NEWCLISETSERVID 			// ��¼�ͻ��˷�����ID
	//#define _FMPOINT_PK_MESSAGE		// ��սȫ��ͨ��
	//#define _FMPOINT_WELFARE			// ׯ԰����
	#define _REGISTRATION_TIME		// ǩ��ʱ��
	#define _FAME_REG_TIME				// ����ǩ��ʱ��
#ifdef _OFFLINE_SYSTEM
	#define _OFFLINE_TIME					// ����ʱ��
#endif
	#define _DATA_INFO_SHOW				// ������Ϣ��ʾ
	#define _ITEM_USE_TIME				// ����ʹ��ʱ��
//	#define _PET_VALIDITY  				// ������Ч��
	//#define _PET_FUSION_LUA  			// �ںϳ�LUA
	//#define _PET_ENEMY_DEVELOP_UP //ȫ����������ɳ�
	#define _FIRST_LOCK_ITEM			//��һ�ν��׺��Զ�����Ʒ
	#define _CTRL_TRANS_DEVELOP		//���Ƴɳ�
	#define _CANCEL_STREET_VENDOR		//��ֹ����̯
	//#define _ABDUCT_NO_PETOUT			//�ó̻���ֱͬʱ����
	//#define _SUPER_FMPOINT				//����ׯ԰
	#define _ATTACK_RAND_SORT			//����˳�����
	#define _POWER_UP							//��������
	#define _FM_POINT_APPLY_FAME	//ׯ԰������������
	#define _PET_MAKE_2_TRANS						// Terry �����˥͡�GM����

#endif
#if _ATTESTAION_ID == 0
	#define _DEBUG_ALL
#endif

#if _ATTESTAION_ID == 35
	#define _DAMMAGE_CALC					// �Զ����˺�
#endif

#endif

//                       SA_REVERSION NEW ADD
//======================================================================================
#define _BRAND_NEW_25				//2.5�汾�ܿ���  ����֮�󽫴�2.5���еĹ���
//======================================================================================

//###############                                            ������������Ϣ����                                    ####################
#define _SA_VERSION 'N'
#define _DEFAULT_PKEY "starttwneo"		// Ĭ�ϵ���Կ
#define _RUNNING_KEY  "runtwtrinity"		// ��������Կ
//###############                                            ������������Ϣ����                                    ####################

//###############                                            ͨ�ð汾���ܶ�����                                    ####################
#define		_JZ_NEWSCRIPT_LUA				 // ���� ��LUA�ű�����
#ifdef		_JZ_NEWSCRIPT_LUA
	#define _MO_LNS_NLSUOXU            // LNS-NL������
	#define _MO_LNS_CHARSUOXU          // LNS-CHAR������
	#define _MO_LNS_NLGSUOXU           // LNS-NLG������
	#define _MO_LNS_MAPSUOXU           // LNS-MAP������
	#define _MO_LNS_MYSQLSUOXU         // LNS-MYSQL������
	#define _MO_LUA_BETT01			        // LUA���ӽӿ������
	#define _LUA_Debug                  // LUA�������
	#define _MO_ADD_LUADX					    // LUAһЩ��չ
	#define _ITEM_LUA				            // ����LUA֧��
#endif
//#define _MO_SHOW_DEBUG			//��ʾ�����Ϣ
//#define _NO_ATTACK						 // ����CC����
//#define	_NO_FULLPLAYER_ATT	     // �����˹���
//#define _NEW_VERIFICAITON 	//ĭ�޸��µ�LUA��֤���㷨
#define _SHARE_EXP					  // ���鹲��
#define EXPGET_MAXLEVEL 5			//���鲻˥�����ȼ�
#define EXPGET_DIV 15				//����ָ�ȼ�
#ifdef _SHARE_EXP
#define _COMBO_EXP					// �ϻ��ӳ�
#endif
#define _GAMBLE_POINT				//SQL�Ĳ�����
#define _TRIAL_POINT				//SQL��������
#define 	_MO_LUA_FMPOINT			//�����ݵ���ص�lua�ӿ�
#define _DEFEND_BIGBAO				//�������
#define _SERVER_DEF                 // �����ȫ����
//#define _DEBUG_RET_CLI
#define _NOT_FMPK_WAIT				//��ս����ս��
#define _SINGLE_ONLINE				//��������
#define _CHAR_LOOP_TIME				//char����ʱ��
#define _MO_IMAGE_EXTENSION			//������չ

#define _CHANGE_PETNAME_FIX			//��ֹ����Ƿ�����
#define		_MO_RELOAD_NPC			//�ض�NPC
#define _ROOKIE_ITEM				//���ֵ��� 1ת֮��ɾ�� ���޷�װ��
#define _NO_TRANS_ITEM				//��ֹת��Я���ĵ���
#define _MAX_MERGE_LEVEL			//��ߺϳɵ��ߵȼ�
#define _MO_MAP_AUTO_UPDATE				//�Զ����µ�ͼ
#define _NEW_FUNC_DECRYPT		//�½��ܻ���

#define _PLAYER_EFFECT									//����Ч��
#define _MO_PLAYER_EFFECT							//��������Ч������ֶ�������
#define _MO_LUA_KS_CALLBACK					//KS�ӿ�LUA�ص� ��������ս��ѡ���¼�
#define _MO_LOGIN_NO_KICK							//��¼����������
#define _ATTACK_EFFECT								  // Terry ����Ч��
#define _STREET_VENDOR_CHECK					//��̯���ⳤ������
#define _MO_LUA_TALK_EX								//ablua˵��������չ
#define _MO_LUA_ENCOUNTER_CONTROL		//ablua���п���
#define _MO_ILLEGAL_NAME            // ��ֹ��������
//#define _MO_NO_WALK											//��ֹ��·
#define _MO_FIX_ULTIMATE							//�޸������ж�
#define _TENSE_FIELD										//��ʱ�ֶ�

#define _CHAR_NANOR_             //������¹⻷
#define FAMILY_MANOR_         //ׯ԰�⻷
#define _NETLOG_              //����T����־
#define _PETCOM_              //�������
#define _MORECHARACTERS_    //��ҳ����
#define _FAMILYBADGE_				//�������
#define _SECONDARY_WINDOW_DATA_       //�����������ݷ��
#define _TRUNTABLE_                 //ת�̸��·��     ע��������Ҫ�����Ӹ�����꣬�÷������Ҫ��Ϊ���ݸ���
#define _PETFOLLOW_NEW_                 //����5ֻ�����Ȧ
#define _TRUMP_EQUIPMENT					//����װ��
#define _FIX_SUPERPERSON_STRANS //�޸����6תBUG
#define _PETMAIL_TIME				     // �����ʼ��Զ���ʱ�䣨Ҳ���ǳ������ݣ�
#define _MO_LUA_RESIST							//���⿹�Ե�lua�ӿڼ��ֶ�
#define _MO_FIX_STREET_VENDOR	//������̯�ӿڹ������µĿ���
#define _MO_LUA_BATTLE_START		//ս����ʼ���ӿڻص�
#define _REDUCE_MERGE_FAME				//�����޹ҷ��ļ���ϳ������ȼ���������
#define _ALLDOMAN                // (�ɿ���) Syu ADD ���а�NPC
#define _ALLDOMAN_DEBUG          // (�ɿ�) Change ��������server�����а���ʧbug
#define _MO_LUA_MERGE_CALLBACK		//�ϳ������lua�ص�
#define _ALWAYS_TEAM_OPEN					//��¼Ĭ�Ͽ������
#define _FIX_LER_IMG								//�޸��׶�����
#define _NPC_MAGICCARD							// ħ����NPC
#define _MOVE_SCREEN								//�ƶ���Ļ
#define _BATTLE_OVER_STAR_TIME// ս�������¿�ʼʱ����

#define _NEW_SQL_MONEY										//����SQL����

//###############                                            ͨ�ð汾���ܶ���������                                    ####################

//###############                                            2.5���й��ܶ�����                                    ####################
#ifdef _BRAND_NEW_25
#define _DEFAULT_PKEY "shiqi"		// Ĭ�ϵ���Կ
#define _RUNNING_KEY  "shiqi.hk"		// ��������Կ
#define DENGLUKEY1 "6D27AEB8326A2A8E7D04B9F9B44C59C7"

#define _NEW_TITLE											//�³ƺ�ϵͳ
#define _NEW_UPDATETITLE							//���³ƺŷ��
#define CHAR_RECORD_POINT               //�����¼��ͻ��˶�ȡ
#define _FREE_SAVE    				// ��������¼
#define FAMILY_MAX_MEMBER 			50 	// ��������
#define MANOR_MAX_NUM 			2        // (4.0) ����ݵ�
#define _LEVELUP_RECOVERY					   //������Ѫ
#define _NEW_ITEM_          //�µ�����

#define _VISUAL_BEATITUDE							//�¿��ӻ�ף������
#define _RIDEFLG_        						 //��迪��
#define _RIDEQUERY_                            //����ѯ
#define _PREVENT_TEAMATTACK				//��ֹͬ�ӻ���
#define _SHOW_VISUAL_BEATITUDE		//���װ�̯��ʾ���ӻ�ף��
#define _MO_REDUCE_PVP_COMBO				//����PVP�ϻ���
#define _BATTLE_OVER_STAR_TIME// ս�������¿�ʼʱ����
//#define _CANCEL_BORN_PET			// ȡ��������

#define _NPC_EVENT_NOTICE							//ĭ����NPC����״̬��ʾ���ܣ���ͻ��˿����ú깦��
#ifdef _NPC_EVENT_NOTICE
	#define EVENT_NOT_RESPOND					120138	//��ɫ̾�� ��������ɻ�����δ�ﵽ�������
	#define EVENT_CAN_ACCECPT					120137	//��ɫ̾�� ������Խ���
	#define EVENT_READY_TO_START			120139	//��ɫ�ʺ� ���Խ�����
	#define EVENT_CANNOT_START				120140	//��ɫ�ʺ� ���ܽ�����
#endif
#define _CHAR_TITLE_STR_                //���ֳƺ�
#define _NEW_UPDATETITLE							//���³ƺŷ��

#define _NO_TEAMWARP_SKYLAND			//��ֹ����������
#define _GLORY_POINT								//SQL��������
#define _MO_LUA_GOLRYCALLBACK		//�����ص�
#define _RED_MEMOY_                 //���
#endif
//###############                                            2.5���й��ܶ����� ����                                   ####################
