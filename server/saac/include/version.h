#ifndef __VERSION_H__
#define __VERSION_H__

/* �뽫����ר���� definition �����������
 * Ȼ����� remark/unmark �����ع���
 */

//���⹦�ܿ���----------------------------------------

#define _SAVE_ZIP                //�Զ�����
#define _IP_VIP                  //IP��֤
#define _FIX_FMPOINTTIME						//��������ռ��ʱ��
#define _FAMILYBADGE_								//�������
#define _MORECHARACTERS_     			//��ҳ����
#define _SQL_BUY_FUNC_ONE_CDKEY		//ÿ���ʺŽ�����ȡһ�ε���

#define _ATTESTAION_ID 1

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2
	//#define _VERSION_25
#endif

#ifndef _VERSION_25
	#define _TEACHER_SYSTEM   // ��ʦϵͳ(δ����)
	#define _ALLDOMAN				// (�ɿ���) Syu ADD ���а�NPC
#endif
	#define _CHANNEL_MODIFY		// Ƶ����������
	#define _NEW_MANOR_LAW		// ��ׯ԰����
/* -------------------------------------------------------------------
 * ר��������ְ������幦�ܲ���
 * ��Ա��С褡���ѫ��־ΰ��С��
 * ˵����
 */
#define _LEADERFLAG_VER		// �š��ߡ����.... (�ɿ���)
#define _FAMILY			//  (�ɿ���)
#define _PERSONAL_FAME		// Arminius 8.30 �����������  (�ɿ���)
#define _FMVER21			// CoolFish: �����峤ְ������� 2001/8/30 (�ɿ���)
#define _CHAR_POOLITEM			// (�ɿ���) ANDY ���òֿ�
#define _CHAR_POOLPET			// (�ɿ���) Robin ���ó���ֿ�

/* -------------------------------------------------------------------
 * ר������������˷֮�������ϴ浵��ʽ����
 * ��Ա��С褡���ѫ
 * ˵���������������ϴ浵��ʽ
 */
#define _NewSave		// CoolFish: 2001/10/16 (�ɿ���)

/* -------------------------------------------------------------------
 * ר��������£գ�����
 * ��Ա��С�
 * ˵����
 */
#define _LEADERFUNCHECK	// CoolFish: 2002/02/06 �峤���ܴ������ô�������(�ɿ���)

/* -------------------------------------------------------------------
 * ר��������������
 * ��Ա�����
 * ˵����
 */
#define _WAEI_KICK				// (�ɿ���)(�������ؿ�)

/* -------------------------------------------------------------------
 * ר�����Ŵ�ׯ԰(4.0)
 * ��Ա��־��
 * ˵����
 */
#ifndef _VERSION_25
#define _FIX_10_FMPOINT			// (�ɿ���) WON ADD �Ŵ�ׯ԰
#endif
#define _FIX_LEADER_ERR			// (�ɿ���) WON ADD �����峤����
#define _REMOVE_FM_LOG          // (�ɿ���) WON ADD �Ƴ�����Ҫ�ļ��� LOG

/* -------------------------------------------------------------------
 * ר����ʯ��6.0
 * ��Ա��
 * ˵����
 */
#define _SEND_EFFECT		    // (�ɿ���) WON ADD ����ѩ���������Ч

// -------------------------------------------------------------------
#define _AC_SEND_FM_PK			// (�ɿ���) WON ADD ׯ԰��ս�б�����AC
#define _ACFMPK_LIST			// (�ɿ���) ANDY ׯ԰��ս�б�
/* -------------------------------------------------------------------
 * ר����ʯ��7.0
 * ��Ա��
 * ˵����
 */

#define _LOCK_ADD_NAME		// LockNode ����������λ(δ����)
// -------------------------------------------------------------------
#define _SLEEP_CHAR		// Robin 2004/02/12	��Ծ������ǻ�Ծ����ֿ�����

// Terry define start -------------------------------------------------------------------
#define _FM_MODIFY				// ���幦���޸�

// Terry define end   -------------------------------------------------------------------

#define _ANGEL_SUMMON	// Robin ������ٻ�

//--------------------------------------------------------
//by longzoro
#define _SASQL                   // ʯ�����Ͽ�
#ifdef _SASQL
	#define _SQL_BACKGROUND        //��ҳ��̨����   ��ͬʱ����_SASQL1
	#define _SQL_REGISTER          //�Զ�ע�Ṧ��   ��ͬʱ����_SASQL1
#endif

#if _ATTESTAION_ID != 11
#define _LOGNZORO_FUNC_ALL       // ��zoro��������
#endif
#ifdef _LOGNZORO_FUNC_ALL
	#define _NEW_VIP_SHOP            // ��Ա�̵�
	
	#if _ATTESTAION_ID != 8 && _ATTESTAION_ID != 10 && _ATTESTAION_ID != 21 
		#define _ITEM_PET_LOCKED         // ��Ʒ������
	#endif
	
	#if _ATTESTAION_ID != 3
		#define _ONLINE_COST             // ���߳�ֵ
		#define _SQL_BUY_FUNC            // SQL������
		#define _VIPPOINT_OLD_TO_NEW     // ��Ա���ת��
	#endif
	
	#if _ATTESTAION_ID == 5
		#define _FORMULATE_AUTO_PK       // �ƶ��Զ���PKϵͳ
	#else
		#define _OLDPS_TO_MD5PS         // �Ѿɵ�����ת��MD5����
		#ifdef _NEW_VIP_SHOP
			#if _ATTESTAION_ID != 3
				#define _COST_ITEM             // ��ֵ����
			#endif
		#endif
		#if _ATTESTAION_ID != 21
			#define _AUTO_BACKUP           // �Զ�����
		#endif
		#define _FIX_CAHR_VERSION      // �浵�汾����
		#if _ATTESTAION_ID != 38 && _ATTESTAION_ID != 49
			#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 23 || _ATTESTAION_ID == 28 || _ATTESTAION_ID >= 30
				#define _LOTTERY_SYSTEM          // ��Ʊϵͳ
			#endif	
		#endif	
	#endif
#endif

#if _ATTESTAION_ID != 5 && _ATTESTAION_ID != 11
	#define FAMILY_MAX_MEMBER 			100 	// ��������
#else
	#define FAMILY_MAX_MEMBER       50		// ��������
#endif

#if _ATTESTAION_ID != 11
	#define _CHARADATA_SAVE_SQL      //������д��SQL
#endif
#if 1
	#define _ONE_SERVER              // ������
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 21 || _ATTESTAION_ID ==31 || _ATTESTAION_ID ==33 || _ATTESTAION_ID >=35
	#define _ALL_SERV_SEND       // ����Ƶ��
#endif
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 21 || _ATTESTAION_ID ==31 || _ATTESTAION_ID ==33 || _ATTESTAION_ID ==35 || _ATTESTAION_ID >=42
	#if _ATTESTAION_ID != 43 && _ATTESTAION_ID != 47 && _ATTESTAION_ID != 49 
		//#define _DEATH_FAMILY_LOGIN_CHECK			//ɾ����������֮�����̫��δ����֮��Ա
	#endif
#endif


#if _ATTESTAION_ID == 1
	//#define _SET_FMPOINT
	//#define _FAMILY_TOTEM
	#define _NEWCLISETMAC
	#define _FAMILY_PAY_POINT			//�����ֵ����
#endif

#if _ATTESTAION_ID == 0
	#define _DEBUG
#endif

#endif



