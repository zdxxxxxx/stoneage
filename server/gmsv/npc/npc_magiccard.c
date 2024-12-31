#include "version.h"
#include <time.h>
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"
#include "handletime.h"
#include "readmap.h"
#include "log.h"
#include "enemy.h"
#include "npc_eventaction.h"
#include "pet.h"
#include "battle.h"
#include "chatmagic.h"

#ifdef _NPC_MAGICCARD
#include "npc_magiccard.h"

void NPC_Magiccard_selectWindow(int meindex,int toindex,int num,int select);
int NPC_magiccard_checkplace( int meindex , int toindex , char *token );
void NPC_magiccard_gameinit( int meindex , int bkid );
void NPC_magiccard_selectcard( int meindex , int bkid );
void NPC_magiccard_checkselect( int meindex , int toindex ,int choise);
void NPC_magiccard_waitselect( int meindex , int bkid );
void NPC_magiccard_aiselect( int meindex , int bkid );
//void NPC_magiccard_showcard( int meindex , int bkid );
//void NPC_magiccard_summonpet(int meindex , int bkid , int charindex , int mode );
//void NPC_magiccard_fightcard( int meindex , int bkid , int playtotal);
//void NPC_magiccard_damage( int meindex , int bkid , int charindex , int player , int playtotal);
void NPC_magiccard_out( int meindex , int bkid , int player );
void NPC_magiccard_turnover( int meindex , int bkid	);
void NPC_magiccard_gameover( int meindex , int bkid );
void NPC_magiccard_resetbanker( int meindex , int bkid );
int NPC_magiccard_checkExist( int meindex , int bkid , int player );
//void NPC_magiccard_action( int meindex , int charindex , int bkid , int player );
void NPC_magiccard_selectback( int meindex , int toindex );
void NPC_magiccard_action2( int meindex , int charindex , int user , int card , int action );
void NPC_magiccard_checkdamage( int meindex , int bkid );
void NPC_magiccard_showdamage( int meindex , int charindex , int position , int damage );
void NPC_magiccard_specialcard( int meindex , int bkid );
int NPC_magiccard_sort( const void *card1, const void *card2 );

enum {
	NPC_WORK_BANKER = CHAR_NPCWORKINT1,		// ��¼����һ��ׯ����
	NPC_WORK_GIVEGOLD = CHAR_NPCWORKINT2,   // ��¼��ս���Ƿ��Ǹ���֤���
	//NPC_WORK_ABANDON = CHAR_NPCWORKINT2,	// ��ս���Ƿ��������
	//NPC_WORK_TURN = CHAR_NPCWORKINT3,		// ��¼�غ���
	NPC_WORK_HAND = CHAR_NPCWORKINT4,		// ��¼��ս�����ϵ���
	NPC_WORK_SELECT = CHAR_NPCWORKINT5,		// ��¼��ս��ѡ����ڼ�����
	NPC_WORK_ACTION = CHAR_NPCWORKINT6,		// ��¼��ս��ѡ�񹥻����Ƿ���
	NPC_WORK_HISTORY = CHAR_NPCWORKINT7,	// ��¼��ս�߳�������
	NPC_WORK_DAMAGE = CHAR_NPCWORKINT8,		// ��¼��ս���ܵ��˺�
	//NPC_WORK_LOCATION = CHAR_NPCWORKINT9,	// ��¼��ս�ߵ�λ��
	//NPC_WORK_ = CHAR_NPCWORKINT10,		// ��¼ׯ�ҵ�index	
};

enum {	
	ATTACK=1,
	DEFENCE,
	MAGIC,
};

enum {
	START,
	RULES,
	INGAME,	
	SCORE,
	END,
};


#define LOOPTIME			100 // 0.1��
#define SHOWCARDLOOPTIME	800//1000  //1��  
#define FIGHTLOOPTIME		800//1000//2000 
#define TURNOVERTIME		1300//1500//4000
#define GAMEOVERTIME        4800//5000

#define MAXHP				50
#define MAXGUY				6
#define BANKERID            6

#define SHIFTBASE			1	//�������ƵĻ�׼ֵ
//#define MAGIC_MOVE_SCREEN_XY			(18 << 16) | 10		// ӫĻҪ�ƶ�����XY����
//#define MAGIC_MOVE_SCREEN_XY			(14 << 16) | 13		// ӫĻҪ�ƶ�����XY����

struct BANKER banker[BANKERTOTAL];  

int  Bettotal[BANKERTOTAL];
int  Wintotal[BANKERTOTAL];
int  tickettotal[BANKERTOTAL];

//λ������
typedef struct Posstruct
{
	int humanX;
	int humanY;
	int petX;
	int petY;	
	int npcX;
	int npcY;
	int direct;
}Position;
static Position Positiontable[]=
{
	{19,12,17,12,15,13,6},	//0 ��ս��
	{18,17,16,15,15,14,7},	//1 ��ս��
	{13,18,13,16,14,14,0},	//2 ��ս��
	{ 9,14,11,14,13,13,2},	//3 ��ս��
	{10, 9,12,11,13,12,3},	//4 ��ս��
	{15, 8,15,10,14,12,4},	//5 ��ս��
	{15,12,14,13,14,13,5},	//6 ׯ��
	{24,18, 0, 0,0,0,6},	//7 ����	
	
/*
	{23, 9,21, 9,6},	//0 ��ս��
	{22,14,20,12,7},	//1 ��ս��
	{17,15,17,13,0},	//2 ��ս��
	{13,11,15,11,2},	//3 ��ս��
	{14, 6,16, 8,3},	//4 ��ս��
	{19, 5,19, 7,4},	//5 ��ս��
	{18,18,18,10,5},	//6 ׯ��
	{15,13, 0, 0,6},	//7 ����
*/
};

typedef struct st_cardbox
{
	BOOL use;
	int cardindex;
}CardBoxType;
static CardBoxType Cardbox[BANKERTOTAL*7][30];

//ħ����
typedef struct Cardstruct
{
    STRING64    name;
	int			petid;
	int			attack;
	int			defence;	
	int			effect;
    void*       functionPointer;   
}Cardfunction;
static Cardfunction Cardfunctiontable[]=
{
	{{"������1:	 �Ա����    �� 13 �� 1"}               ,2565 ,13 , 1 , 101437,NULL},
	{{"������2:	 ���޿�      �� 13 �� 1"}               ,0    ,13 , 1 , 101962,NULL},
	{{"������3:	 ������      �� 13 �� 1"}               ,2571 ,13 , 1 , 101616,NULL},
	{{"������4:	 ����        �� 1 �� 13"}               ,2563 , 1 ,13 , 101760,NULL},
	{{"������5:	 �����ŵ    �� 1 �� 13"}               ,0    , 1 ,13 , 101946,NULL},
	{{"������6:	 ����ŷ��    �� 1 �� 13"}               ,0    , 1 ,13 , 101485,NULL},
	{{"������7:  ����        �� 8 �� 8"}                ,0    , 8 , 8 , 101180,NULL},
	{{"������8:  ��¶��      �� 8 �� 8"}                ,0    , 8 , 8 , 100872,NULL},
	{{"������9:	 ��ŵ��      �� 8 �� 7"}                ,0    , 8 , 7 , 101952,NULL},	
	{{"������10: ���ַ�      �� 8 �� 6"}                ,0    , 8 , 6 , 101875,NULL},
	{{"������11: ��˹��      �� 7 �� 7"}                ,0    , 7 , 7 , 101914,NULL},	
	{{"������12: ����ŵ˹    �� 7 �� 7"}                ,2456 , 7 , 7 , 101819,NULL},	
	{{"������13: �׵�˹��    �� 7 �� 8"}                ,0    , 7 , 8 , 101501,NULL},	
	{{"������14: ������      �� 7 �� 6"}                ,0    , 7 , 6 , 101922,NULL},	
	{{"������15: ������˹    �� 6 �� 6"}                ,0    , 6 , 6 , 101464,NULL},	
	{{"������16: ������      �� 6 �� 6"}                ,0    , 6 , 6 , 101928,NULL},		
	{{"������17: ��˹��      �� 6 �� 8"}                ,0    , 6 , 8 , 101446,NULL},	
	{{"������18: ������      �� 6 �� 7"}                ,0    , 6 , 7 , 100390,NULL},	//	
	{{"������19: ��˹�ŵ�    �� 3 �� 3"}                ,0    , 3 , 3 , 101954,NULL},	
	{{"������20: ������˾    �� 3 �� 3"}                ,0    , 3 , 3 , 101957,NULL},	
	{{"������21: ʷ����      �� 3 �� 3"}                ,0    , 3 , 3 , 101556,NULL},	
	{{"������22: ��˹��      �� 3 �� 2"}                ,0    , 3 , 2 , 101933,NULL},	
	{{"������23: �ɿɶ�      �� 2 �� 3"}                ,2570 , 2 , 3 , 100898,NULL},	
	{{"������24: ����        �� 2 �� 2"}                ,0    , 2 , 2 , 101870,NULL},	
	{{"������25: ������      �� 2 �� 2"}                ,0    , 2 , 2 , 101918,NULL},	
	{{"ħ����1: �þ��������Է��Ĺ���������ȥ��"}       ,2604 , 0 , 0 , 101652,NULL},
	{{"ħ����2: ��Ӱ�������ܵ��Է��κι�����"}		   ,2605 , 0 , 0 , 101695,NULL},
	{{"ħ����3: ��ӡ�������Է���ͣһ�غϡ�"}           ,2606 , 0 , 0 , 101120,NULL},
	{{"ħ����4: ���������ظ�HP->��Ѫ��50%��"}          ,2607 , 0 , 0 , 101828,NULL},
	{{"ħ����5: ���������������  �� 20��"}            ,2608 , 20 , 0 , 101628,NULL},	

};

BOOL NPC_Magiccard_Init( int meindex )
{
	char argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	char buf[64];
	int j,k;

	memset( banker,0,sizeof(banker));
	for(j=0;j<BANKERTOTAL;j++){
		banker[j].state = -1;
		Bettotal[j]=0;
		Wintotal[j]=0;
		tickettotal[j]=0;
	}
	CHAR_setInt( meindex, CHAR_LOOPINTERVAL, LOOPTIME );
	
	for(j=0;j<14;j++){
		for(k=0;k<30;k++){
			Cardbox[j][k].use = 0;
			Cardbox[j][k].cardindex = k;
		}
	}

	if( NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr)) == NULL)	return FALSE;

	NPC_Util_GetStrFromStrWithDelim( argstr , "ID", buf, sizeof( buf));

	CHAR_setInt( meindex, CHAR_LV, atoi(buf));  // ׯ�ҵ�id��¼��CHAR_LV
	print("\n ------------bankerid= %d ------------------- ",atoi(buf));
	return TRUE;
}

void NPC_Magiccard_Talked( int meindex , int talkerindex , char *szMes , int color )
{
	char	argstr[NPC_UTIL_GETARGSTR_BUFSIZE];

	if( CHAR_getInt( talkerindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ){
    		return;
	}
	if(NPC_Util_isFaceToFace( meindex ,talkerindex , 2) == FALSE) {
		if(NPC_Util_isFaceToChara( talkerindex, meindex, 1) == FALSE) return;
	}

	if( NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr)) == NULL )	{
		print("\n read argstr err");
		return;
	}	
	CHAR_setWorkInt( talkerindex , CHAR_WORKSHOPRELEVANT,0);
	NPC_Magiccard_selectWindow( meindex , talkerindex, START , -1 );
}

void NPC_Magiccard_selectWindow(int meindex,int toindex,int num,int select)
{
	int fd;
	char	argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	int i,pagenum;
	int buttontype = 0, windowtype = 0, windowno = 0;
	char token[1024],buf2[256],buf1[256];

	windowtype = WINDOW_MESSAGETYPE_MESSAGE;
    fd = getfdFromCharaIndex( toindex );
    if( fd == -1 )  return;

	if( NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr)) == NULL )	{
		print("\n read argstr err");
		return;
	}

	switch(num) 
	{
	case START:  //һ�еĿ�ʼ
		i = 1;
		strcpy( token, "\0");
		sprintf( buf2,"%s%d", "menu_msg0", i);
		while( NPC_Util_GetStrFromStrWithDelim( argstr, buf2, buf1, sizeof( buf1) ) != NULL )	{
			if( strstr( buf1, "NULL" ) == NULL )	{
				strcat( token , buf1);
				strcat( token, "\n");
			}else	{
				strcat( token, "\n");
			}
			i++;
			sprintf( buf2,"%s%d", "menu_msg0", i);
		}
		windowtype = WINDOW_MESSAGETYPE_SELECT;
		buttontype = WINDOW_BUTTONTYPE_NONE;
		windowno = CHAR_WINDOWTYPE_MAGIC_START;
		break;	
	case RULES:
		pagenum = CHAR_getWorkInt( toindex , CHAR_WORKSHOPRELEVANT);
		strcpy( token, "\0");
		if( select >= 0 ){
			sprintf( buf2,"%s%d", "page_num", select);
			CHAR_setWorkInt( toindex , CHAR_WORKSHOPRELEVANT, select+1);
		}else{
			sprintf( buf2,"%s%d", "page_num", pagenum );
			pagenum+=1;
			CHAR_setWorkInt( toindex , CHAR_WORKSHOPRELEVANT, pagenum);
		}

		if( NPC_Util_GetStrFromStrWithDelim( argstr, buf2, token, sizeof( token) ) == NULL )	{
			print("\n error: not found token:%s", token);
			CHAR_setWorkInt( toindex , CHAR_WORKSHOPRELEVANT,0);
			return;
		}

		sprintf( buf2,"%s%d", "page_num", CHAR_getWorkInt( toindex , CHAR_WORKSHOPRELEVANT));
		if( NPC_Util_GetStrFromStrWithDelim( argstr, buf2, buf1, sizeof( buf1) ) == NULL )	{		
			buttontype = WINDOW_BUTTONTYPE_OK;
			windowno = CHAR_WINDOWTYPE_MAGIC_NORMAL;
		}else	{
			buttontype = WINDOW_BUTTONTYPE_NEXT;
			windowno = CHAR_WINDOWTYPE_MAGIC_RULES;
		}
		break;
	case INGAME:
		//warp��ս�ߵ�ս��λ���� ��ʼ����
		if( NPC_magiccard_checkplace( meindex , toindex , token ) == TRUE ){ //�����û�п�λ
			return; 
		}else{
			//���ܲμ�
			buttontype = WINDOW_BUTTONTYPE_OK;
			windowno = CHAR_WINDOWTYPE_MAGIC_NORMAL;
		}
		break;
	case SCORE:
		{	
			int  G_num = CHAR_getInt( toindex, CHAR_GAMBLENUM );

			if( G_num < 0 ) G_num = 0;
			sprintf( token,"������ֳ�����Ϊ%d��", G_num );
			buttontype = WINDOW_BUTTONTYPE_OK;
			windowno = CHAR_WINDOWTYPE_MAGIC_NORMAL;
		}
		break;
	case END:
		if( NPC_Util_GetStrFromStrWithDelim( argstr, "end_msg", token, sizeof( token) ) == NULL ){
			print("\n read arg err");
		}
		buttontype = WINDOW_BUTTONTYPE_OK;
		windowno = CHAR_WINDOWTYPE_MAGIC_NORMAL;
		break;
	}

	lssproto_WN_send( fd, windowtype, buttontype, windowno, 
		CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX), token);
}

void NPC_Magiccard_WindowTalked( int meindex, int talkerindex, int seqno, int select, char *data)
{
	int pagenum,datanum;

	switch(seqno)
	{
	case CHAR_WINDOWTYPE_MAGIC_START:
		pagenum = atoi( data );
		if( pagenum == 4 ){				// �뿪
			NPC_Magiccard_selectWindow( meindex, talkerindex, END , -1 );
		}else	{
			if( pagenum == 3 ){			// ���˻���
				NPC_Magiccard_selectWindow( meindex, talkerindex, SCORE , -1 );
			}else if( pagenum == 2 ){	// �μ���Ϸ				
				NPC_Magiccard_selectWindow( meindex, talkerindex, INGAME , -1 );
			}else{						// ��Ϸ����
				NPC_Magiccard_selectWindow( meindex, talkerindex, RULES, 1 );
			}
		}
		break;
	case CHAR_WINDOWTYPE_MAGIC_RULES:
		NPC_Magiccard_selectWindow( meindex, talkerindex, RULES, -1 );
		break;
	case CHAR_WINDOWTYPE_MAGIC_NORMAL:
		break;
	case CHAR_WINDOWTYPE_MAGIC_SELECT:
		datanum = atoi(data);
		if( CHAR_getWorkInt( talkerindex , NPC_WORK_SELECT ) == -1 ) {	
			NPC_magiccard_checkselect( meindex , talkerindex ,datanum );
		}else{
			if( select == WINDOW_BUTTONTYPE_OK ){
				CHAR_setWorkInt( talkerindex , NPC_WORK_ACTION , MAGIC );	
			}else if( select == WINDOW_BUTTONTYPE_CANCEL ){
				CHAR_setWorkInt( talkerindex , NPC_WORK_SELECT , -1 );
				NPC_magiccard_selectback( meindex , talkerindex );
			}else{
				if(datanum == 2 )
					CHAR_setWorkInt( talkerindex , NPC_WORK_ACTION , ATTACK );
				else if(datanum == 4)
					CHAR_setWorkInt( talkerindex , NPC_WORK_ACTION , DEFENCE );
			}
		}
		break;
	}	
}

void NPC_Magiccard_Loop( int meindex )  //ׯ�����Լ���LOOP
{
	int j,playertotal,bkid;
	char buf[256];

	if( !CHAR_CHECKINDEX( meindex) ) return;
	bkid = CHAR_getInt( meindex, CHAR_LV );
	
	playertotal = 0;
	for( j=0 ; j < MAXGUY ; j++){
		if( NPC_magiccard_checkExist( meindex , bkid , j ) != -1 )
			playertotal++;
	}
	if( playertotal == 0 ) NPC_magiccard_resetbanker( meindex , bkid );  //���ׯ���Ѿ�û����ս������
	else banker[bkid].playertotal = playertotal ;

	switch( banker[bkid].state )  //��ׯ������������
	{ 
	case 0: //�ȴ���ս��
		if( banker[bkid].waittime < NowTime.tv_sec ){		
			NPC_magiccard_gameinit( meindex , bkid);
		}else{
			if((long)banker[bkid].waittime - NowTime.tv_sec != banker[bkid].turntime ){
				banker[bkid].turntime = (long)banker[bkid].waittime - NowTime.tv_sec;
				if( banker[bkid].turntime < 6 ){
					sprintf( buf ,"��Ϸ���ڡ� %d�� ����ʼ", banker[bkid].turntime );
					showString( meindex, buf , 0);
				}
			}
		}
		break;
	case 1: //Ҫ�����
		NPC_magiccard_selectcard( meindex , bkid);			
		break;
	case 2: //�ȴ�����
		if( (long)banker[bkid].turntime - NowTime.tv_sec != banker[bkid].waittime ){
			banker[bkid].waittime = (long)banker[bkid].turntime - NowTime.tv_sec;
			if(banker[bkid].waittime < 4 && banker[bkid].waittime >= 0 ){
				sprintf( buf ,"����ʱ��ʣ��%d��", banker[bkid].waittime );
				showString( meindex, buf , 0);
			}
		}
		NPC_magiccard_waitselect( meindex , bkid);
		break;
	case 3: //�����˺����㶯��
		NPC_magiccard_checkdamage( meindex,bkid);
		break;
	case 4: //������
		NPC_magiccard_specialcard( meindex , bkid);
		break;
	case 5: //�غϽ���
		NPC_magiccard_turnover( meindex , bkid);
		break;
	case 9: //������������Ӯ
		NPC_magiccard_gameover( meindex , bkid);
		break;
	default:
		break;
	}
}

int NPC_magiccard_checkplace( int meindex , int toindex , char *token )
{
	int i,offsetx,offsety;
	unsigned int movescreenXY=0;
	char argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	char buf[64];
	OBJECT object;
	int bossindex,objindex;
	
	if( NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr)) == NULL )	{
		print("\n read argstr err");
		return FALSE;
	}		
	NPC_Util_GetStrFromStrWithDelim( argstr , "offsetx", buf, sizeof( buf));
	offsetx = atoi(buf);
	NPC_Util_GetStrFromStrWithDelim( argstr , "offsety", buf, sizeof( buf));
	offsety = atoi(buf);
    
	if( CHAR_getWorkInt( toindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE )
	{
		strcpy(token,"����в��ܲμ�!!!");
		return FALSE;
	}

	//�ж���û���ʸ����
	if( CHAR_getInt( toindex , CHAR_GAMBLENUM) < 6 && CHAR_getInt( toindex , CHAR_GOLD) < 15000 ){
		strcpy(token,"��û���㹻��Ǯ����֤�����Ʊร��ܱ�Ǹ���ܲμ�!!!");
		return FALSE;			
	}else if( CHAR_getInt( toindex , CHAR_GOLD) < 5000 ){
		strcpy(token,"��û���㹻��ʯ�Ҹ���Ʊร��ܱ�Ǹ���ܲμ�!!!");
		return FALSE;			
	}

	i = CHAR_getInt( meindex, CHAR_LV );
	if( banker[i].state > 0 || banker[i].playertotal >= MAXGUY ){ //continue; //���˻���Ϸ������
		strcpy(token,"�����Ѿ������˻��ǽ����У���Ⱥ���һ��лл");
		return FALSE;	
	}

	if( banker[i].playertotal == 0 ){  //��һλ��ս��		
		banker[i].waittime = NowTime.tv_sec + 15 ; //30 
		banker[i].state = 0 ;		//����ȴ�loop
	}else
		banker[i].waittime = NowTime.tv_sec + 10 ;  //ֻҪ����ս�߽������ټ�10��
		
	sprintf( banker[i].cdkey[banker[i].playertotal] , "%s" , CHAR_getChar( toindex , CHAR_CDKEY ));
	sprintf( banker[i].name[banker[i].playertotal] , "%s" , CHAR_getChar( toindex , CHAR_NAME ));			

	if(CHAR_getInt( toindex , CHAR_GAMBLENUM) < 6){
		CHAR_setInt( toindex , CHAR_GOLD, CHAR_getInt( toindex , CHAR_GOLD)-15000 ); 
		CHAR_setWorkInt( toindex , NPC_WORK_GIVEGOLD , 1 );
		sprintf( buf,"�ѽɽ���Ʊ5000�Ӻͱ�֤��10000��" ); 
		CHAR_talkToCli( toindex , -1 , buf,  CHAR_COLORYELLOW);
	}else{
		CHAR_setInt( toindex , CHAR_GOLD, CHAR_getInt( toindex , CHAR_GOLD)-5000 ); 
		CHAR_setInt( toindex , CHAR_GAMBLENUM, CHAR_getInt( toindex , CHAR_GAMBLENUM) - 6 );		
		CHAR_setWorkInt( toindex , NPC_WORK_GIVEGOLD , 0 );
		sprintf( buf,"�ѽɽ���Ʊ5000��" ); 
		CHAR_talkToCli( toindex , -1 , buf,  CHAR_COLORYELLOW);
	}

	Bettotal[i] += 6;
	tickettotal[i]++;
	
	CHAR_setInt( toindex , CHAR_DIR , Positiontable[banker[i].playertotal].direct );
	CHAR_setWorkInt( toindex , NPC_WORK_BANKER , i );
	//��¼��������ȷ���Ƿ�����;�뿪	
	CHAR_setWorkInt(toindex, CHAR_WORK_TEMP_FLOOR, CHAR_getInt(meindex, CHAR_FLOOR));	

	//sprintf( buf ,"��¼�ĵ�ͼΪ%d", CHAR_getWorkInt(toindex, CHAR_WORK_TEMP_FLOOR ));
	//showString( meindex, buf , 0);

	CHAR_warpToSpecificPoint( toindex, CHAR_getInt( meindex, CHAR_FLOOR), 
		Positiontable[banker[i].playertotal].humanX + offsetx ,
		Positiontable[banker[i].playertotal].humanY + offsety );
	//�ƶ�����ս�ߵ��ӽ�	
	movescreenXY = ((Positiontable[banker[i].playertotal].humanX + offsetx) << 16) | ( Positiontable[banker[i].playertotal].humanY + offsety)	;
	lssproto_MoveScreen_send(getfdFromCharaIndex(toindex), TRUE, movescreenXY);	
	banker[i].playertotal += 1;		//����һ����ս��

	//�׶�����
	object = MAP_getTopObj(CHAR_getInt(meindex, CHAR_FLOOR), Positiontable[BANKERID].humanX + offsetx, Positiontable[BANKERID].humanY + offsety );
	if( object ){
		objindex = GET_OBJINDEX( object);
		bossindex = OBJECT_getIndex(objindex);
		if( banker[i].playertotal < 3 )
			CHAR_CHAT_DEBUG_metamo( bossindex , "101813" );
		else if( banker[i].playertotal < 5 )
			CHAR_CHAT_DEBUG_metamo( bossindex , "101814" );
		else
			CHAR_CHAT_DEBUG_metamo( bossindex , "101815" );
	}

	return TRUE;
}

void NPC_magiccard_gameinit( int meindex , int bkid )
{
	int  i,j=0,charindex,k;	
	unsigned int card;
	char buf[256];
	unsigned int movescreenXY=0;
	int offsetx,offsety;
	char argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	BOOL flag = FALSE;

	if( NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr)) == NULL )	{
		print("\n read argstr err");
		return ;
	}		
	NPC_Util_GetStrFromStrWithDelim( argstr , "offsetx", buf, sizeof( buf));
	offsetx = atoi(buf);
	NPC_Util_GetStrFromStrWithDelim( argstr , "offsety", buf, sizeof( buf));
	offsety = atoi(buf);

	banker[bkid].turn = 1;	
	banker[bkid].hand = 0;
	banker[bkid].history = 0;
	banker[bkid].turntime = NowTime.tv_sec + 20 ;
	banker[bkid].process = 0;	
	sprintf( buf,"��һ�غϿ�ʼ ս��" );
	showString( meindex, buf , 0);
	
	//ׯ�ҵ���
	while(j<5){
		if( tickettotal[bkid] > 5000 && flag == FALSE ){
			i = rand()%100;
			if( i > 50 ) k = 25 + rand()%5;
			else if( i > 20 ) k = rand()%3;
			else k = 3 + rand()%3;					
			flag = TRUE;
		}else{
			k = rand()%(30-j);
			if( Cardbox[6+7*bkid][k].cardindex == 28 && flag == FALSE && rand()%100 > 50){	//����������ֻ���
				flag = TRUE;
				continue;
			}
		}
		Cardbox[6+7*bkid][k].use = 1;
		card = SHIFTBASE << Cardbox[6+7*bkid][k].cardindex ; 
		banker[bkid].hand |= card;
		qsort( &Cardbox[6+7*bkid] , 30 , sizeof(CardBoxType) , &NPC_magiccard_sort );
		j++;
	}
	flag = FALSE;
	for( i=0;i<MAXGUY;i++){
		//�����ս�߻��ڲ��� 
		charindex = NPC_magiccard_checkExist(meindex,bkid,i);
		if( charindex == -1 ) continue;
		
		banker[bkid].starttotal += 1;
		
		CHAR_setWorkInt( charindex , NPC_WORK_DAMAGE , 0 );		
		banker[bkid].hp[i] = MAXHP ;							//��Ѫ��
		CHAR_setWorkInt( charindex , NPC_WORK_HAND , 0 );		//��������
		CHAR_setWorkInt( charindex , NPC_WORK_HISTORY , 0 );	//��¼���
		CHAR_setWorkInt( charindex , NPC_WORK_SELECT , -1 );		//ѡ�����
		CHAR_setWorkInt( charindex , NPC_WORK_ACTION , 0 );		//�������
		j=0;
		while(j<5){
			k = rand()%(30-j);
			if( Cardbox[i+7*bkid][k].cardindex == 28 && flag == FALSE && rand()%100 > 50){	//����������ֻ���
				flag = TRUE;
				continue;
			}
			Cardbox[i+7*bkid][k].use = 1;
			card = SHIFTBASE << Cardbox[i+7*bkid][k].cardindex ; 
			CHAR_setWorkInt( charindex , NPC_WORK_HAND ,
				(unsigned int)CHAR_getWorkInt( charindex , NPC_WORK_HAND )|card );			
			qsort( &Cardbox[i+7*bkid] , 30 , sizeof(CardBoxType) , &NPC_magiccard_sort );
			j++;
		}
		/*
		for( j=0;j<30;j++){
			char tempbuf[256];
			sprintf( tempbuf,"%d ",Cardbox[i+7*bkid][j].use );	
			strcat( buf, tempbuf );	
		}
		showString( meindex, buf , 0);
		buf[0]='\0';
		for( j=0;j<30;j++){
			char tempbuf[256];
			sprintf( tempbuf,"%d ",Cardbox[i+7*bkid][j].cardindex );	
			strcat( buf, tempbuf );	
		}
		showString( meindex, buf , 0);
		*/

		//�ƶ�����ս�ߵ��ӽ�	
		movescreenXY = ((14+offsetx) << 16) | (13+offsety)	;
		lssproto_MoveScreen_send(getfdFromCharaIndex(charindex), TRUE, movescreenXY);	

	}			
	banker[bkid].hp[BANKERID] = banker[bkid].starttotal * MAXHP ;
	CHAR_setInt( meindex, CHAR_LOOPINTERVAL, 1000 );
	banker[bkid].state = 1;
}

//����ս��ѡ���ʲô��
void NPC_magiccard_selectcard( int meindex , int bkid )
{
	int i,j,fd,allhp=0,charindex;
	unsigned int card=1;
	char token[256];
	
	for(i=0;i<MAXGUY;i++) allhp += banker[bkid].hp[i];
	for(i=0;i<MAXGUY;i++)
	{	
		charindex = NPC_magiccard_checkExist( meindex , bkid , i );
		if( charindex == -1 ) continue;
////////////
		if( banker[bkid].pauseflag[i] == TRUE ){	
			sprintf( token,"%d����ս��%s����ӡ�ˣ���ͣһ�غ�",i+1,CHAR_getChar( charindex, CHAR_NAME) );
			showString( meindex, token , 0);
			continue;
		}
////////////
		fd = getfdFromCharaIndex( charindex );		
		sprintf( token, "ħ��HP%d ��ս��HP%d�����HP%d��\n",banker[bkid].hp[BANKERID],allhp,banker[bkid].hp[i]);
		for( j=0;j<30;j++){
			if( (card<<j) & (unsigned int)CHAR_getWorkInt( charindex , NPC_WORK_HAND ) ){
				strcat( token , Cardfunctiontable[j].name.string );
				strcat( token, "\n");
			}
		}	
		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_SELECT ,
			WINDOW_BUTTONTYPE_NONE , 
			CHAR_WINDOWTYPE_MAGIC_SELECT , 
			CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX), token);

	}		
	banker[bkid].waittime = NowTime.tv_sec + 20 ;
	banker[bkid].state = 2;
}

//�����ʲô�� 
void NPC_magiccard_checkselect( int meindex , int toindex ,int choise )
{
	int fd,i,j=0;
	unsigned int card=1;
	char token[128];
	int buttontype;

    fd = getfdFromCharaIndex( toindex );
    if( fd == -1 )  return;//Ҫ�����ս�߻��ڲ��� Ŀǰ��û���
		
	for( i=0;i<30;i++){
		if( (card<<i) & (unsigned int)CHAR_getWorkInt( toindex , NPC_WORK_HAND) ){
			j++;
			if( j == choise ){ 
				CHAR_setWorkInt( toindex , NPC_WORK_SELECT , i ); //��¼��ս��ѡ�������0~29
				sprintf( token, "%s\n",Cardfunctiontable[i].name.string);
				//if( Cardfunctiontable[i].attack > 0 || Cardfunctiontable[i].defence > 0 ){
				if( i < 25 ){
					//ȷ���ǳ�����				
					strcat( token , "\n����\n" );				
					strcat( token , "\n����");
					buttontype = WINDOW_BUTTONTYPE_CANCEL;
				}else{
					buttontype = WINDOW_BUTTONTYPE_OK|WINDOW_BUTTONTYPE_CANCEL;
				}
				lssproto_WN_send( fd, WINDOW_MESSAGETYPE_SELECT ,
					buttontype, 
					CHAR_WINDOWTYPE_MAGIC_SELECT , 
					CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX), token);
				break;
			}	
		}
	}
}

//������Ҫ��ѡʱ
void NPC_magiccard_selectback( int meindex , int toindex )
{
	unsigned int handcard;
	char token[256];
	int i,j,fd,bkid,allhp=0;

	handcard = CHAR_getWorkInt( toindex , NPC_WORK_HAND);
    bkid = CHAR_getWorkInt( toindex , NPC_WORK_BANKER );
	for(i=0;i<MAXGUY;i++) allhp += banker[bkid].hp[i];	
	for(i=0;i<MAXGUY;i++){
		if( strcmp( banker[bkid].cdkey[i] , CHAR_getChar( toindex, CHAR_CDKEY) ) == 0 &&
			strcmp( banker[bkid].name[i] , CHAR_getChar( toindex, CHAR_NAME) ) == 0){
			break;
		}
	}

	fd = getfdFromCharaIndex( toindex );
	if( fd == -1 )  return;

	sprintf( token, "ħ��HP%d ��ս��HP%d�����HP%d��\n",banker[bkid].hp[BANKERID],allhp,banker[bkid].hp[i]); 
	for( j=0;j<30;j++){
		if( (SHIFTBASE<<j) & handcard ){
			strcat( token , Cardfunctiontable[j].name.string );
			strcat( token, "\n");
		}
	}	
	
	lssproto_WN_send( fd, WINDOW_MESSAGETYPE_SELECT ,
		WINDOW_BUTTONTYPE_NONE , 
		CHAR_WINDOWTYPE_MAGIC_SELECT , 
		CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX), token);

}

//�����ս���Ƿ�ѡ�����
void NPC_magiccard_waitselect( int meindex , int bkid )
{
	int i,charindex,select=0;
	BOOL okflag = FALSE;
	char buf[256];

	for( i=0;i<MAXGUY;i++){		
		charindex = NPC_magiccard_checkExist( meindex , bkid , i );
		if( charindex == -1 ) continue;
		if( //CHAR_getWorkInt( charindex , NPC_WORK_SELECT ) != -1 &&
			CHAR_getWorkInt( charindex , NPC_WORK_ACTION ) != 0 || banker[bkid].pauseflag[i] == TRUE ){	
			select++;  //ѡ����
		}else if( banker[bkid].turntime < NowTime.tv_sec ){
			CHAR_setWorkInt( charindex , NPC_WORK_SELECT , -1 );
			CHAR_setWorkInt( charindex , NPC_WORK_ACTION , 0 );
			//ȡ��ѡ��			
			lssproto_MagiccardDamage_send(getfdFromCharaIndex(charindex), 10 , 0 ,0 ,0 );
			sprintf(buf,"����ʱ�䵽��%d����ս�߷�������",i+1);	
			showString( meindex, buf , 0);
			select++;  //�����˲�����ѡ�� 
		}
	}	 
	if( select >= banker[bkid].playertotal )  okflag = TRUE;	//��Ҷ�ѡ����
	//sprintf(buf,"ʣ��%d��ս��  ��ѡ����ս����%d ",banker[bkid].playertotal,select);	
	//showString( meindex, buf , 0);
	if( okflag == TRUE ) {
		banker[bkid].state = 3;
		NPC_magiccard_aiselect( meindex , bkid );
		
		//sprintf(buf,"ai select %d action %d",banker[bkid].select,banker[bkid].action);	
		//showString( meindex, buf , 0);

		CHAR_setInt( meindex, CHAR_LOOPINTERVAL, SHOWCARDLOOPTIME );	
	}
}

void NPC_magiccard_aiselect( int meindex , int bkid )
{
	int i,j,k,enemy=0;	
	//int maxattack=0,maxdefence=0;
	//int maxacard=0,maxdcard=0;
	char buf[256];
	int charindex,allattack,alldefence,bestattack,bestdefence;
	int getdamage=0,givedamage=0;	
	int card25=0,allhp=0;
	int attacklist[5],defencelist[5],cardnum[5];
	BOOL selectflag = FALSE;
	BOOL checkflag = FALSE;

	for( i=0;i<6;i++){
		attacklist[i] = 0;
		defencelist[i] = 0;
		cardnum[i]=0;
		if(banker[bkid].pauseflag[i] == TRUE){
			banker[bkid].pauseflag[i] = FALSE;
			checkflag = TRUE;  //���˱���ӡ �������
		}
	}
	allattack=0;
	alldefence=0;
	bestattack=0;
	bestdefence=0;
//-----------------------------------------------------------------------------	
	if( banker[bkid].pauseflag[BANKERID] == TRUE )
	{
		banker[bkid].pauseflag[BANKERID] = FALSE;
		sprintf( buf,"ħ������ӡ�� ��ͣһ�غ�" );
		showString( meindex, buf , 0);
		return;
	}
//-----------------------------------------------------------------------------
	if( rand()%200 > 5 + banker[bkid].playertotal * 1 || checkflag == TRUE || banker[bkid].turn == 10 ){	//80%
		selectflag = FALSE;
		for( i=0;i<MAXGUY;i++){
			charindex = NPC_magiccard_checkExist( meindex , bkid , i );
			if( charindex == -1 ) continue;
			enemy++;
			allhp += banker[bkid].hp[i];
			if( CHAR_getWorkInt( charindex , NPC_WORK_ACTION ) == 0 ) continue;
			if( CHAR_getWorkInt( charindex , NPC_WORK_ACTION ) == ATTACK ){
				allattack += Cardfunctiontable[CHAR_getWorkInt(charindex,NPC_WORK_SELECT)].attack ;
			}else if( CHAR_getWorkInt( charindex , NPC_WORK_ACTION ) == DEFENCE ){
				alldefence += Cardfunctiontable[CHAR_getWorkInt(charindex,NPC_WORK_SELECT)].defence ;
			}else if( CHAR_getWorkInt( charindex , NPC_WORK_SELECT ) == 25 ){ //�þ�
				if( rand()%100 > 40 + 5 * banker[bkid].playertotal ){
					allattack += rand()%10;						
					alldefence += rand()%10;
				}else{
					alldefence += Cardfunctiontable[0].attack ;		//�ɵֵ���ǿ�Ĺ���-->����µ�
					card25++;
				}
			}else if( CHAR_getWorkInt( charindex , NPC_WORK_SELECT ) == 26 ){ //��Ӱ
				if( rand()%100 > 40 + 5 * banker[bkid].playertotal )
					alldefence += rand()%10;		
				else
					alldefence += Cardfunctiontable[0].attack ;		
			}else if( CHAR_getWorkInt( charindex , NPC_WORK_SELECT ) == 27 ){ //��ӡ
				if( rand()%100 > 30 + 5 * banker[bkid].playertotal ){
					allattack += rand()%5;						
					alldefence += rand()%5;
				}
			}else if( CHAR_getWorkInt( charindex , NPC_WORK_SELECT ) == 28 ){ //����
				if( rand()%100 > 70 + 5 * banker[bkid].playertotal ){
					allattack += rand()%5;						
					alldefence += rand()%5;
				}
			}else if( CHAR_getWorkInt( charindex , NPC_WORK_SELECT ) == 29 ){ //����		
				if( rand()%100 > 30 + 5 * banker[bkid].playertotal ){
					allattack += rand()%5;						
					alldefence += rand()%5;
				}	
			}
		}
		//sprintf( buf,"allA=%d   allD=%d",allattack,alldefence );
		//showString( meindex, buf , 0);

		//��¼ÿһ���ƵĽ�� û�и�ֵ��ʾ��0
		j=0;
		for( k=0;k<30;k++){
			if((SHIFTBASE<<k) & banker[bkid].hand ){
				if( k < 25 ){
					//����
					if( 1 ){ //Cardfunctiontable[k].attack * enemy > alldefence ){
						givedamage =  Cardfunctiontable[k].attack * enemy - alldefence ;
						if( allhp > banker[bkid].hp[BANKERID] )
							getdamage = allattack + Cardfunctiontable[k].attack * card25;				
						else
							getdamage = allattack ;				
						attacklist[j] = givedamage - getdamage;
					}
					//����
					if( Cardfunctiontable[k].defence * enemy < allattack ){
						defencelist[j] = Cardfunctiontable[k].defence * enemy - allattack;
					}
				}else{ //ħ����
					attacklist[j] -= allattack;	//�������жϳ��Ʋ��ÿ��ǵ�ħ����
					defencelist[j] -= allattack; 
				}
				cardnum[j]=k;
				j++;	
			}
		}
		//sprintf( buf,"������������ %d %d %d %d %d",cardnum[0],cardnum[1],cardnum[2],cardnum[3],cardnum[4]);
		//showString( meindex, buf , 0);

//������---------------------------------------------------------------------
		for( i=0;i<5;i++){
			if( cardnum[i] == 28 ){	
				if( banker[bkid].turn == 10 )
					selectflag = TRUE;
				else{
					switch( banker[bkid].playertotal ){
					case 1:
					case 2:
						if( banker[bkid].hp[BANKERID] < banker[bkid].starttotal * (MAXHP-25) )
							selectflag = TRUE;
						break;
					case 3:
					case 4:
						if( banker[bkid].hp[BANKERID] < banker[bkid].starttotal * (MAXHP-20) || banker[bkid].turn > 8 )
							selectflag = TRUE;
						break;
					case 5:
					case 6:
						if( banker[bkid].hp[BANKERID] < banker[bkid].starttotal * (MAXHP-15) &&
							 allattack < banker[bkid].playertotal * 8  )
							selectflag = TRUE;
						else if( banker[bkid].turn > 7 )
							selectflag = TRUE;
						break;
					}
				}
				if( selectflag == TRUE ){				
					banker[bkid].select = 28;
					banker[bkid].action = MAGIC;
					return;						
				}
			}
		}
//��ӡ��----------------------------------------------------------------	
		for( i=0;i<5;i++){
			if( cardnum[i]== 27 && banker[bkid].turn != 10 && banker[bkid].hp[BANKERID] >= banker[bkid].playertotal * 13){  
				switch( banker[bkid].playertotal ){
				case 1:		
				case 2:
					for( j=0;j<5;j++){
						if( rand()%100 > 50 && Cardfunctiontable[cardnum[j]].attack > 6 && allattack < banker[bkid].playertotal * 13 )
							selectflag = TRUE;		
					}
					break;
				case 3:
				case 4:
					for( j=0;j<5;j++){
						if( rand()%100 > 70 && Cardfunctiontable[cardnum[j]].attack > 7 && allattack < banker[bkid].playertotal * 8 )
							selectflag = TRUE;		
					}
					break;
				case 5:
				case 6:
					for( j=0;j<5;j++){
						if( rand()%100 > 50 && Cardfunctiontable[cardnum[j]].attack == 13 && allattack < banker[bkid].playertotal * 8 )
							selectflag = TRUE;							
						else if( rand()%100 > 80 && Cardfunctiontable[cardnum[j]].attack > 7 && allattack < banker[bkid].playertotal * 8 )
							selectflag = TRUE;												
					}
					break;
				}
				if( selectflag == TRUE ){				
					banker[bkid].select = 27;
					banker[bkid].action = MAGIC;
					return;						
				}
			}
		}
//����л�Ӱ���ж�Ҫ��Ҫʹ��-----------------------------------------------------------------
		for( i=0;i<5;i++){
			if( cardnum[i] == 26 && checkflag == FALSE ){
				if( banker[bkid].turn == 10 && allhp <= banker[bkid].hp[BANKERID] ){
					selectflag = TRUE;
				}else{
					switch(	banker[bkid].playertotal ){
					case 1:
					case 2:
						if( banker[bkid].turn < 8 && rand()%100 > 80 ) selectflag = TRUE;					
						else if( banker[bkid].turn > 7 && banker[bkid].hp[BANKERID] < banker[bkid].starttotal * 15 ) selectflag = TRUE;				
						else if( allattack > 8 * banker[bkid].playertotal && rand()%100 > 40 ) selectflag = TRUE;
						break;					
					case 3:
					case 4:
						if( allhp > banker[bkid].hp[BANKERID] && allattack > 8 * banker[bkid].playertotal ) selectflag = TRUE;
						else if( allhp - banker[bkid].hp[BANKERID] > 10 && banker[bkid].turn > 5 &&
							allattack > 6 * banker[bkid].playertotal ){
							selectflag = TRUE;
						}
						break;		
					case 5:
					case 6:
						if( allhp - banker[bkid].hp[BANKERID] > 10 &&
							allattack > 8 * banker[bkid].playertotal ) selectflag = TRUE;	
						else if( banker[bkid].hp[BANKERID] > allhp && banker[bkid].turn > 5 && rand()%100 > 80) selectflag = TRUE;	
						else{
							for( j=0;j<MAXGUY;j++){
								charindex = NPC_magiccard_checkExist( meindex , bkid , i );
								if( charindex == -1 ) continue;
								if( CHAR_getWorkInt( charindex , NPC_WORK_SELECT ) == 27 ){
									if( rand()%100 > (70-banker[bkid].turn*8) ){
										selectflag = TRUE;
										break;
									}
								}		
							}
						}
						break;
					}
				}
				if( banker[bkid].turn == 10 && allhp > banker[bkid].hp[BANKERID] ) selectflag = FALSE;
				if( selectflag == TRUE ){
					banker[bkid].select = 26;
					banker[bkid].action = MAGIC;
					return;						
				}
			}
		}
//������������ж�Ҫ��Ҫʹ��-----------------------------------------------------------------		 
		for( i=0;i<5;i++){
			if( cardnum[i] == 29 && checkflag == FALSE){
				switch(banker[bkid].playertotal){
				case 1:
				case 2:
					if( rand()%100 > 80 && banker[bkid].hp[BANKERID] < allhp ) selectflag = TRUE;
					else if( banker[bkid].turn == 10 && (allhp-banker[bkid].hp[BANKERID]) >= 10 ) selectflag = TRUE;
					break;			
				case 3:
				case 4:
					if( allattack < Cardfunctiontable[29].attack && rand()%100>30 )	selectflag = TRUE;
					else if( rand()%100 > 70 && banker[bkid].hp[BANKERID] < allhp ) selectflag = TRUE; 
					break;				
				case 5:
				case 6:
					if( allattack < Cardfunctiontable[29].attack+10 ) selectflag = TRUE;
					else if( banker[bkid].turn == 10 && (allhp - banker[bkid].hp[BANKERID]) >= 13 ) selectflag = TRUE;
					break;
				}
				if( selectflag == TRUE ){
					banker[bkid].select = 29;
					banker[bkid].action = MAGIC;
					return;						
				}
			}	
		}
//������---------------------------------------------------------------------------------		
		for( i=0;i<5;i++){
			if( cardnum[i] == 25 && checkflag == FALSE ){
				if( banker[bkid].turn == 10 ){
					selectflag = TRUE;
				}else{
					switch(banker[bkid].playertotal){
					case 1:
					case 2:
						if( rand()%100 > 50 ) selectflag = TRUE;		
						else if( rand()%100>70-banker[bkid].turn*2 && allattack >= 6 * banker[bkid].playertotal ) selectflag = TRUE;						
						break;			
					case 3:
					case 4:
						if( rand()%100 > 60 ) selectflag = TRUE;
						else if( rand()%100 > 50 && allhp < banker[bkid].hp[BANKERID] ) selectflag = TRUE;
						else if( rand()%100>60-banker[bkid].turn*2 && allattack >= 7 * banker[bkid].playertotal ) selectflag = TRUE;
						break;				
					case 5:
					case 6:
						if( rand()%100 > 70 && allhp > banker[bkid].hp[BANKERID] ) selectflag = TRUE;
						else if( rand()%100>50-banker[bkid].turn*2 && allattack >= 8 * banker[bkid].playertotal ) selectflag = TRUE;
						else if( banker[bkid].turn > 7 && allhp < banker[bkid].hp[BANKERID] ) selectflag = TRUE;
						break;
					}
					if( selectflag == TRUE ){
						banker[bkid].select = 25;
						banker[bkid].action = MAGIC;
						return;						
					}
				}
			}	
		}
//���һ�غϻ���һ������ǿ�Ĺ���----------------------------------------------------
		if( ( banker[bkid].turn == 10 && allhp > banker[bkid].hp[BANKERID] ) || checkflag == TRUE ){
			bestattack = 0;
			for( i=0;i<5;i++){
				if( cardnum[i] >= 25) continue;	
				if( Cardfunctiontable[cardnum[i]].attack > bestattack ){
					bestattack = Cardfunctiontable[cardnum[i]].attack;
					banker[bkid].select = cardnum[i];
					banker[bkid].action = ATTACK;						
				}
			}			
		}
//���Ŀǰ��Ӯ��״̬ ͬʱ������ֳ������˿�ûѪ��  ����ǿ����------------------------
		if( banker[bkid].action == 0 ){
			if( allhp < banker[bkid].hp[BANKERID] ){
				for( j=0;j<BANKERID;j++){
					if( banker[bkid].hp[j] > 0 && banker[bkid].hp[j] < MAXHP/4 ){ 
						bestattack = 6;
						for( i=0;i<5;i++){
							if( cardnum[i] >= 25) continue;	
							if( Cardfunctiontable[cardnum[i]].attack > bestattack ){
								bestattack = Cardfunctiontable[cardnum[i]].attack;
								banker[bkid].select = cardnum[i];
								banker[bkid].action = ATTACK;						
							}
						}						
					}
				}
			}
		}
//-----------------------------------------------------------------------------------
		bestattack = 0;	
		bestdefence = 0;
		if( banker[bkid].action == 0 ){	
			if( allhp >= banker[bkid].hp[BANKERID] || rand()%100 > 50 ){	//��Ӯ����
				for( i=0;i<5;i++){
					if( cardnum[i] >= 25) continue;	
					if( banker[bkid].hp[BANKERID] < banker[bkid].playertotal * 13 ) break; //��ûѪ��
					if( attacklist[i] > bestattack ){
						bestattack = attacklist[i];
						banker[bkid].select = cardnum[i];
						banker[bkid].action = ATTACK;	
					}
				}		
			}
		}
		if( banker[bkid].action == 0 ){
			if( rand()%100 > 50 + banker[bkid].playertotal * 3 ){
				//����
				if( rand()%100 > 50 ){	
					bestdefence = defencelist[4] ; 
					for( i=4;i>=0;i--){	//�����ú���
						if( cardnum[i] >= 25 ) continue;		
						if( defencelist[i] > bestdefence ){
							bestdefence = defencelist[i] ; 
							banker[bkid].select = cardnum[i];
							banker[bkid].action = DEFENCE;
						}
					}
				}else{
					bestdefence = defencelist[0] ; 
					for( i=0;i<5;i++){	//����������
						if( cardnum[i] >= 25 ) continue;	
						if( defencelist[i] > bestdefence ){
							bestdefence = defencelist[i] ; 
							banker[bkid].select = cardnum[i];
							banker[bkid].action = DEFENCE;
						}
					}
				}
			}else{
				//����
				if( rand()%100 > 50 ){	
					bestattack = attacklist[4] ; 
					for( i=4;i>=0;i--){	//�����ñȽϺõ���
						if( cardnum[i] >= 25 ) continue;	
						if( attacklist[i] > bestattack ){
							bestattack = attacklist[i] ; 
							banker[bkid].select = cardnum[i];
							banker[bkid].action = ATTACK;
						}
					}
				}else{
					bestattack = attacklist[0] ; 
					for( i=0;i<5;i++){	//�����ñȽϲ����
						if( cardnum[i] >= 25 ) continue;	
						if( attacklist[i] > bestattack ){
							bestattack = attacklist[i] ; 
							banker[bkid].select = cardnum[i];
							banker[bkid].action = ATTACK;
						}
					}
				}
			}
		}
	}

	//���û��ѡ�������������� 
	if( banker[bkid].action == 0 )
	{	
		j=0;
		for( k=0;k<25;k++){
			if((SHIFTBASE<<k) & banker[bkid].hand ){
				cardnum[j]=k;
				j++;
			}
		}
		//sprintf( buf,"��������������� %d %d %d %d %d",cardnum[0],cardnum[1],cardnum[2],cardnum[3],cardnum[4]);
		//showString( meindex, buf , 0);

		if( j==0 ) j = 1; 
		banker[bkid].select = cardnum[rand()%j];
		if( rand()%100 > 50 )
			banker[bkid].action = ATTACK;
		else
			banker[bkid].action = DEFENCE;
	}

	//���ռ��
	if( banker[bkid].select < 25 ){	
		if( Cardfunctiontable[banker[bkid].select].attack == 13 )
			banker[bkid].action = ATTACK;
		if( Cardfunctiontable[banker[bkid].select].defence == 13 )
			banker[bkid].action = DEFENCE;
	}else
		banker[bkid].action = MAGIC;
}

void NPC_magiccard_checkdamage( int meindex , int bkid )
{
	int charindex,i;
	int bankerdamage=0,playerdamage=0;
	int select;
	//char buf[256];

	for( i=0;i<MAXGUY;i++)
	{
		charindex = NPC_magiccard_checkExist( meindex , bkid , i );
		if( charindex == -1 ) continue;
		CHAR_setWorkInt( charindex , NPC_WORK_DAMAGE , 0 );
		select = CHAR_getWorkInt( charindex , NPC_WORK_SELECT );	

		if( select == 28 ){ //��ս����������
			CHAR_setWorkInt( charindex , NPC_WORK_DAMAGE , -MAXHP/2);
			playerdamage += -MAXHP/2;
			//sprintf( buf,"%d����ս��%sʹ��������",i+1,CHAR_getChar( charindex, CHAR_NAME));
			//showString( meindex, buf , 0);
		}
		if( CHAR_getWorkInt( charindex , NPC_WORK_ACTION ) == ATTACK && banker[bkid].select != 26 )
		{
			if( banker[bkid].select == 25 ){ //�þ���
				CHAR_setWorkInt( charindex , NPC_WORK_DAMAGE , Cardfunctiontable[select].attack );
				playerdamage += Cardfunctiontable[select].attack;
			}else if( banker[bkid].action == DEFENCE ){
				if( Cardfunctiontable[select].attack > Cardfunctiontable[banker[bkid].select].defence )
					bankerdamage += (Cardfunctiontable[select].attack-Cardfunctiontable[banker[bkid].select].defence);
			}else
				bankerdamage += Cardfunctiontable[select].attack;
		}	
		if( banker[bkid].action == ATTACK && CHAR_getWorkInt( charindex , NPC_WORK_SELECT ) != 26 )
		{
			if( select == 25 ){
				bankerdamage += Cardfunctiontable[banker[bkid].select].attack;
			}else if( CHAR_getWorkInt( charindex , NPC_WORK_ACTION ) == DEFENCE ){
				if( Cardfunctiontable[banker[bkid].select].attack > Cardfunctiontable[select].defence )
				{
					playerdamage += (Cardfunctiontable[banker[bkid].select].attack-Cardfunctiontable[select].defence);
					CHAR_setWorkInt( charindex , NPC_WORK_DAMAGE , (Cardfunctiontable[banker[bkid].select].attack-Cardfunctiontable[select].defence) );
				}
			}else{
				playerdamage += Cardfunctiontable[banker[bkid].select].attack;
				CHAR_setWorkInt( charindex , NPC_WORK_DAMAGE , 
					CHAR_getWorkInt( charindex , NPC_WORK_DAMAGE ) + Cardfunctiontable[banker[bkid].select].attack );	
			}		
		}
		//sprintf( buf,"��ս��%d select=%d �˺�%d",i,select,CHAR_getWorkInt( charindex , NPC_WORK_DAMAGE ) );
		//showString( meindex, buf , 0);		
	}
	//sprintf( buf,"ׯ�����˺�%d  ��ս�����˺�%d",bankerdamage,playerdamage);
	//showString( meindex, buf , 0);
	
	//�㶯��
	if( banker[bkid].select != 29 &&  banker[bkid].select != 27 ){
		//sprintf( buf,"-----------------ħ��ѡ��� ��%d��------------------",banker[bkid].select);
		//showString( meindex, buf , 0);
		NPC_magiccard_action2( meindex , meindex , BANKERID , banker[bkid].select , banker[bkid].action );
	}
	
	for( i=0;i<MAXGUY;i++ ){
		charindex = NPC_magiccard_checkExist( meindex , bkid , i );
		if( charindex == -1 ) continue;	 
		//��ս�߿�Ѫ
		banker[bkid].hp[i] -= CHAR_getWorkInt( charindex , NPC_WORK_DAMAGE ) ;
		NPC_magiccard_showdamage( meindex , charindex , i , CHAR_getWorkInt( charindex , NPC_WORK_DAMAGE ) );
		//if( CHAR_getWorkInt( charindex , NPC_WORK_ACTION ) == 0 ) continue; //��������
		//sprintf( buf,"i=%d Action=%d",i,CHAR_getWorkInt( charindex , NPC_WORK_ACTION ));
		//showString( meindex, buf , 0);
		if( banker[bkid].hp[i] > MAXHP ) banker[bkid].hp[i] = MAXHP;
		if( CHAR_getWorkInt( charindex , NPC_WORK_SELECT ) != 29 &&
			CHAR_getWorkInt( charindex , NPC_WORK_SELECT ) != 27 )
			NPC_magiccard_action2( meindex , charindex , i , CHAR_getWorkInt( charindex , NPC_WORK_SELECT ) , CHAR_getWorkInt( charindex , NPC_WORK_ACTION ) );	
	}

	//ׯ�ҿ�Ѫ
	if( banker[bkid].select == 28 ){ //ׯ����������
		bankerdamage = bankerdamage - banker[bkid].starttotal*MAXHP/2;	
		//sprintf( buf,"ħ�����������  bankdamage=%d �Լ�=%d",bankerdamage,banker[bkid].starttotal*MAXHP/2);
		//showString( meindex, buf , 0);

	}
	banker[bkid].hp[BANKERID] -= bankerdamage;
	NPC_magiccard_showdamage( meindex, meindex , BANKERID , bankerdamage );
	if( banker[bkid].hp[BANKERID] > banker[bkid].starttotal * MAXHP )
		banker[bkid].hp[BANKERID] = banker[bkid].starttotal * MAXHP;
	CHAR_setInt( meindex, CHAR_LOOPINTERVAL, FIGHTLOOPTIME );
	banker[bkid].state = 4;
}

void NPC_magiccard_specialcard( int meindex , int bkid )
{
	int thunder=0,allhp=0; 
	int i,j,k,target,targetarray[6],charindex,damage=0;
	char buf[256],buf1[256],buf2[256];	

	//�����м�����ʹ��������
	for( i=0;i<MAXGUY;i++)
	{
		charindex = NPC_magiccard_checkExist( meindex , bkid , i );
		if( charindex == -1 ) continue;	 
		if( CHAR_getWorkInt( charindex , NPC_WORK_SELECT ) == 29 ){
			thunder++;
			sprintf( buf,"%d����ս��ʹ��������",i+1);				
			showString( meindex, buf , 0);
		}
		allhp += banker[bkid].hp[i];
	}
	if( banker[bkid].select == 29 ){
		thunder++;	
		sprintf( buf,"ħ��ʹ��������");				
		showString( meindex, buf , 0);
	}
	
	//sprintf( buf,"������%d",thunder);
	//showString( meindex, buf , 0);
	i=0;
	while(thunder){	
		
		if( rand()%100 > 50 + 5 * banker[bkid].playertotal )
			target = BANKERID;	
		else
		{
			k = 0 ;
			for( j=0;j<BANKERID;j++ ){
				if( NPC_magiccard_checkExist( meindex , bkid , j ) == -1 ) continue;
				targetarray[k] = j;
				k++;
			}
			if( k == 0 )  target = BANKERID;  //û������ڳ���
			else  target = targetarray[rand()%k];
		}
	
		if(target == BANKERID ) //��ׯ��
		{
			charindex = meindex;
			if( banker[bkid].select != 26 )	damage = Cardfunctiontable[29].attack;
			else damage = 0;			

			banker[bkid].hp[target] -= damage;
			i++;
			if( damage == 0 ){
				sprintf( buf,"ħ������%d�����׻��У����Ӱ����û��",i);		
			}else{
				sprintf( buf,"ħ������%d�����׻���",i);				
			}
			showString( meindex, buf , 0);
		}else{
			charindex = NPC_magiccard_checkExist( meindex , bkid , target );
			if( charindex == -1 ) continue;
			
			if( CHAR_getWorkInt( charindex , NPC_WORK_SELECT ) == 26 ) damage = 0;
			else damage = Cardfunctiontable[29].attack;			

			banker[bkid].hp[target] -= damage;	
			allhp -= damage;
			i++;	
			if( damage == 0 ){
				sprintf( buf,"%d����ս�߱���%d�����׻��У����Ӱ����û��",target+1,i);		
			}else{
				sprintf( buf,"%d����ս�߱���%d�����׻���",target+1,i);				
			}
			showString( meindex, buf , 0);
		}
		NPC_magiccard_action2( meindex , charindex , target , 29 , MAGIC );
		NPC_magiccard_showdamage( meindex , meindex , target , damage );	
		thunder--;
	}

	buf1[0] ='\0';
	buf2[0] ='\0';	
	//�����ӡ��
	if( banker[bkid].select == 27 )
	{
		sprintf( buf,"ħ��ʹ�÷�ӡ��");				
		showString( meindex, buf , 0);

		for( i=0;i<MAXGUY;i++ ){
			charindex = NPC_magiccard_checkExist( meindex , bkid , i );
			if( charindex == -1 ) continue;	 
			NPC_magiccard_action2( meindex , meindex , i , 27 , MAGIC );

			if( ( rand()%100 > ( 30 - 4 * banker[bkid].playertotal ) ) &&
				CHAR_getWorkInt( charindex , NPC_WORK_SELECT ) != 26 ){				
				banker[bkid].pauseflag[i] = TRUE;
				NPC_magiccard_showdamage(meindex , meindex , i , 1111 );
				sprintf(buf2,"%d����� ",i+1);
				strcat( buf1,buf2);
			}else 
				NPC_magiccard_showdamage(meindex , meindex , i , 2222 );		
		}
	}
	for( i=0;i<MAXGUY;i++)
	{
		charindex = NPC_magiccard_checkExist( meindex , bkid , i );
		if( charindex == -1 ) continue;	 
		if( CHAR_getWorkInt( charindex , NPC_WORK_SELECT ) == 27 ){
		
			sprintf( buf,"%d����ս��ʹ�÷�ӡ��",i+1);					
			NPC_magiccard_action2( meindex , charindex , BANKERID , 27 , MAGIC );
			if( banker[bkid].select == 26 )
			{
				NPC_magiccard_showdamage(meindex , meindex , BANKERID , 2222 );
				strcat(buf," ��ħ��ʹ�û�Ӱ��ʧ��");
			}else if( rand()%100 > ( 50 + 5 * banker[bkid].playertotal ) ){	
				banker[bkid].pauseflag[BANKERID] = TRUE;
				sprintf( buf2,"ħ�� ");
				strcat( buf1,buf2);
				NPC_magiccard_showdamage( meindex , meindex , BANKERID , 1111 );
				strcat(buf," ����");	
			}else{
				NPC_magiccard_showdamage(meindex , meindex , BANKERID , 2222 ); 								
				strcat(buf," ʧ��");
			}
			showString( meindex, buf , 0);
		}	
	}
	
	if(strcmp( buf1 , "" ) !=0 ){
		strcat( buf1,"����ӡ��");
	}

	//�����غ϶�ս���	
	if( allhp > banker[bkid].hp[BANKERID] )
		sprintf( buf,"��%d�غϽ�������ս��HP%d�����ȡ�ħ��HP%d��%s",banker[bkid].turn,allhp,banker[bkid].hp[BANKERID],buf1);
	else if( allhp < banker[bkid].hp[BANKERID] )
		sprintf( buf,"��%d�غϽ�����ħ��HP%d�����ȡ���ս��HP%d��%s",banker[bkid].turn,banker[bkid].hp[BANKERID],allhp,buf1);
	else
		sprintf( buf,"��%d�غϽ�����˫����HP%d��ƽ�� %s",banker[bkid].turn,allhp,buf1);
	showString( meindex, buf , 0);

	banker[bkid].state = 5;
	CHAR_setInt( meindex, CHAR_LOOPINTERVAL, TURNOVERTIME );
}

// �µ���ͼ����
void NPC_magiccard_action2(int meindex , int charindex , int user , int card , int action )
{
	int x,y,floor;
	OBJECT object;
	int objindex,othercharindex;
	int j,k,dir,animno;
	char data[1024],buf[256];
	int offsetx,offsety;
	char argstr[NPC_UTIL_GETARGSTR_BUFSIZE];

	if( NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr)) == NULL )	{
		print("\n read argstr err");
		return ;
	}		
	NPC_Util_GetStrFromStrWithDelim( argstr , "offsetx", buf, sizeof( buf));
	offsetx = atoi(buf);
	NPC_Util_GetStrFromStrWithDelim( argstr , "offsety", buf, sizeof( buf));
	offsety = atoi(buf);

	floor = CHAR_getInt(charindex, CHAR_FLOOR);
	x = Positiontable[user].petX + offsetx;
	y = Positiontable[user].petY + offsety;
	dir = Positiontable[user].direct;
	animno = Cardfunctiontable[card].effect;

	//��͸����npc
	if( ( card == 27 || card == 29 ) && user <= BANKERID ){
		object=MAP_getTopObj(floor, Positiontable[user].npcX + offsetx, Positiontable[user].npcY + offsety );
		if(object){ 
			objindex = GET_OBJINDEX( object);
			charindex = OBJECT_getIndex(objindex);
		}
		//sprintf( data,"npcindex %d  name: %s",charindex,CHAR_getChar(charindex, CHAR_NAME));
		//showString( charindex, data , 0);
	}

	for( j = x-CHAR_DEFAULTSEESIZ ; j <= x+CHAR_DEFAULTSEESIZ ; j++ ){
		for( k = y-CHAR_DEFAULTSEESIZ ; k <= y+CHAR_DEFAULTSEESIZ ; k ++ ){									
			for (object = MAP_getTopObj(floor, j, k); object; object = NEXT_OBJECT(object))
			{
				objindex = GET_OBJINDEX( object);
				othercharindex	= OBJECT_getIndex(objindex);

				if (OBJECT_getType(objindex) == OBJTYPE_NOUSE)
					continue;
				//if (OBJECT_getType(objindex) == OBJTYPE_CHARA &&
				//		!CHAR_getFlg(OBJECT_getIndex(objindex), CHAR_ISVISIBLE))
				//	continue;	
				if (OBJECT_getType(objindex) == OBJTYPE_CHARA &&
						CHAR_getInt(othercharindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER &&
						CHAR_getWorkInt(othercharindex, CHAR_WORKBATTLEMODE ) == BATTLE_CHARMODE_NONE){
					sprintf( data,"%d|%d|%d|%d|%d|%d|%d|%d|",CHAR_getWorkInt( charindex, CHAR_WORKOBJINDEX)
					//sprintf( data,"%d|%d|%d|%d|%d|%d|",CHAR_getWorkInt( npccharindex, CHAR_WORKOBJINDEX)
						,user,card,dir,animno,action,offsetx,offsety);
					lssproto_MagiccardAction_send(getfdFromCharaIndex(othercharindex), data );
					//lssproto_Firework_send(getfdFromCharaIndex(othercharindex), CHAR_getWorkInt( charindex, CHAR_WORKOBJINDEX), 0, 101888);
				}
			}
		}
	}
}

void NPC_magiccard_showdamage(int meindex , int charindex , int position , int damage )
{
	int x,y,j,k,floor;
	OBJECT object;
	int objindex,othercharindex,offsetx,offsety;	
	char argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	char buf[256];
	
	if( NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr)) == NULL )	{
		print("\n read argstr err");
		return ;
	}		
	NPC_Util_GetStrFromStrWithDelim( argstr , "offsetx", buf, sizeof( buf));
	offsetx = atoi(buf);
	NPC_Util_GetStrFromStrWithDelim( argstr , "offsety", buf, sizeof( buf));
	offsety = atoi(buf);

	floor = CHAR_getInt(charindex, CHAR_FLOOR);
	x = Positiontable[position].humanX + offsetx;
	y = Positiontable[position].humanY + offsety;

	for( j = x-CHAR_DEFAULTSEESIZ/2 ; j <= x+CHAR_DEFAULTSEESIZ/2 ; j++ ){
		for( k = y-CHAR_DEFAULTSEESIZ/2 ; k <= y+CHAR_DEFAULTSEESIZ/2 ; k ++ ){									
			for (object = MAP_getTopObj(floor, j, k); object; object = NEXT_OBJECT(object))
			{
				objindex = GET_OBJINDEX( object);
				othercharindex	= OBJECT_getIndex(objindex);

				if (OBJECT_getType(objindex) == OBJTYPE_NOUSE)
					continue;
				if (OBJECT_getType(objindex) == OBJTYPE_CHARA &&
						!CHAR_getFlg(OBJECT_getIndex(objindex), CHAR_ISVISIBLE))
					continue;
				if (OBJECT_getType(objindex) == OBJTYPE_CHARA &&
						CHAR_getInt(othercharindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER &&
						CHAR_getWorkInt(othercharindex, CHAR_WORKBATTLEMODE ) == BATTLE_CHARMODE_NONE){
					lssproto_MagiccardDamage_send(getfdFromCharaIndex(othercharindex), position,damage,offsetx,offsety);			
					//sprintf( buf,"name: %s  fd%d posi %d damage %d ox %d oy %d",CHAR_getChar(othercharindex, CHAR_NAME),
					//	getfdFromCharaIndex(othercharindex),position,damage,offsetx,offsety		
					//	);
					//showString( meindex, buf , 0);
				}
			}
		}
	}

}

//�ĸ���ս�߳���
void NPC_magiccard_out( int meindex , int bkid , int player )
{
	int gift=0,charindex,allhp=0,i;
	char buf[128];
	char argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	int offsetx,offsety;

	if( NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr)) == NULL )	{
		print("\n read argstr err");
		return ;
	}		
	NPC_Util_GetStrFromStrWithDelim( argstr , "offsetx", buf, sizeof( buf));
	offsetx = atoi(buf);
	NPC_Util_GetStrFromStrWithDelim( argstr , "offsety", buf, sizeof( buf));
	offsety = atoi(buf);

	charindex = NPC_magiccard_checkExist( meindex , bkid , player );
	if( !CHAR_CHECKINDEX(charindex)) return;
	
	for( i=0;i<MAXGUY;i++) allhp += banker[bkid].hp[i];	

	if(banker[bkid].hp[player]<=0||banker[bkid].hp[BANKERID]>=allhp ){
		if( CHAR_getWorkInt( charindex , NPC_WORK_GIVEGOLD ) == 1 ){
			sprintf(buf,"Ŀǰ�ܻ���Ϊ%d��лл����",CHAR_getInt( charindex , CHAR_GAMBLENUM));
			CHAR_talkToCli( charindex , -1 , buf,  CHAR_COLORYELLOW);
		}else{
			sprintf(buf,"�ܱ�Ǹ�����˿� 6 ����֣�Ŀǰ�ܻ���Ϊ%d��лл����",CHAR_getInt( charindex , CHAR_GAMBLENUM));
			CHAR_talkToCli( charindex , -1 , buf,  CHAR_COLORYELLOW);
		}
	}else{		
		if( banker[bkid].turn <= 10 && banker[bkid].hp[BANKERID] <= 0) gift = 20;
		else if(allhp >= banker[bkid].starttotal*MAXHP*80/100 ) gift = 15;
		else if(allhp >= banker[bkid].starttotal*MAXHP*60/100 ) gift = 10;	
		else gift = 5;

		if( CHAR_getWorkInt( charindex , NPC_WORK_GIVEGOLD ) == 1 ){
			CHAR_setInt( charindex , CHAR_GOLD, CHAR_getInt( charindex , CHAR_GOLD)+10000 );
			CHAR_setInt( charindex , CHAR_GAMBLENUM, CHAR_getInt( charindex , CHAR_GAMBLENUM)+gift );
			sprintf(buf,"���ûر�֤��10000��");
			CHAR_talkToCli( charindex , -1 , buf,  CHAR_COLORYELLOW);
		}else{
			CHAR_setInt( charindex , CHAR_GAMBLENUM, CHAR_getInt( charindex , CHAR_GAMBLENUM)+gift+6 );
		}
		sprintf(buf,"��ϲ���õ� %d ����֣�Ŀǰ�ܻ���Ϊ%d��лл����",gift,CHAR_getInt( charindex , CHAR_GAMBLENUM));
		CHAR_talkToCli( charindex , -1 , buf,  CHAR_COLORYELLOW);
		Bettotal[bkid] -= 6;
		Wintotal[bkid] += gift;
		if( CHAR_getInt( charindex , CHAR_GAMBLENUM ) > 10000 )
			CHAR_setInt( charindex , CHAR_GAMBLENUM	, 10000 );
	}
		
	CHAR_setWorkInt( charindex , CHAR_WORK_TEMP_FLOOR, 0);
	// ӫĻ��λ
	lssproto_MoveScreen_send(getfdFromCharaIndex(charindex), FALSE, 0);

	CHAR_warpToSpecificPoint( charindex , CHAR_getInt( meindex, CHAR_FLOOR) 
				,Positiontable[7].humanX + offsetx , Positiontable[7].humanY + offsety);	
	//banker[bkid].playertotal -= 1; 
}

//һ�غϽ������³���
void NPC_magiccard_turnover( int meindex , int bkid	)
{
	char buf[256];
	unsigned int  card;
	int i,k,charindex,offsetx,offsety;	
	BOOL goflag = FALSE;
	OBJECT object;
	int bossindex,objindex;
	char argstr[NPC_UTIL_GETARGSTR_BUFSIZE];

	if( NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr)) == NULL )	{
		print("\n read argstr err");
		return ;
	}		
	NPC_Util_GetStrFromStrWithDelim( argstr , "offsetx", buf, sizeof( buf));
	offsetx = atoi(buf);
	NPC_Util_GetStrFromStrWithDelim( argstr , "offsety", buf, sizeof( buf));
	offsety = atoi(buf);


	//������� �� ����
	for( i=0;i<MAXGUY;i++)
	{
		charindex = NPC_magiccard_checkExist( meindex , bkid , i );
		if( charindex == -1 ) continue;

		//��黹��û���ʸ������
		if( banker[bkid].hp[i] <= 0 ) {
			sprintf( buf,"%d����ս�߳�����",i+1);
			showString( meindex, buf , 0);	
			NPC_magiccard_out( meindex , bkid , i );
			continue;
		}
		goflag = TRUE; //��������ҿ��Խ����»غ�
		if( CHAR_getWorkInt( charindex , NPC_WORK_ACTION ) == 0 ) continue;	//�������Ƶ���
		//�����������
		card = SHIFTBASE << CHAR_getWorkInt( charindex , NPC_WORK_SELECT );
		CHAR_setWorkInt( charindex , NPC_WORK_HISTORY , (unsigned int)CHAR_getWorkInt( charindex , NPC_WORK_HISTORY ) | card ); 
		card = ~card;
		CHAR_setWorkInt( charindex , NPC_WORK_HAND , (unsigned int)CHAR_getWorkInt( charindex , NPC_WORK_HAND ) & card );	
		//����
		k = rand()%(26-banker[bkid].turn);
		if( Cardbox[i+7*bkid][k].cardindex == 28 && rand()%100 > 50 ) k = rand()%(26-banker[bkid].turn); //����������ֻ���

		Cardbox[i+7*bkid][k].use = 1;
		card = SHIFTBASE << Cardbox[i+7*bkid][k].cardindex ; 
		CHAR_setWorkInt( charindex , NPC_WORK_HAND ,
			(unsigned int)CHAR_getWorkInt( charindex , NPC_WORK_HAND )|card );			
		qsort( &Cardbox[i+7*bkid] , 30 , sizeof(CardBoxType) , &NPC_magiccard_sort );

		CHAR_setWorkInt( charindex , NPC_WORK_SELECT , -1 );  //ѡ�����
		CHAR_setWorkInt( charindex , NPC_WORK_ACTION , 0 );	//�������
		CHAR_setWorkInt( charindex , NPC_WORK_DAMAGE , 0 ); //�˺����

	}

	if( banker[bkid].action != 0 )  //û�б���ӡ
	{
		//��ׯ�ҳ��������õ� 
		card = SHIFTBASE << banker[bkid].select ;	
		banker[bkid].history |= card;
		card = ~card;
		banker[bkid].hand &= card;

		//ׯ�ҳ���
		k = rand()%(26-banker[bkid].turn);
		if( Cardbox[6+7*bkid][k].cardindex == 28 && rand()%100 > 50 ) k = rand()%(26-banker[bkid].turn); //����������ֻ���
		Cardbox[6+7*bkid][k].use = 1;
		card = SHIFTBASE << Cardbox[6+7*bkid][k].cardindex ; 
		banker[bkid].hand |= card;
		qsort( &Cardbox[6+7*bkid] , 30 , sizeof(CardBoxType) , &NPC_magiccard_sort );
	}

	if( banker[bkid].turn == 10 || banker[bkid].hp[BANKERID] <= 0 || goflag == FALSE ){
		//��������	
		if( banker[bkid].hp[BANKERID] <= 0 && goflag == TRUE )
		{
			object = MAP_getTopObj(CHAR_getInt(meindex, CHAR_FLOOR), Positiontable[BANKERID].humanX + offsetx, Positiontable[BANKERID].humanY + offsety );
			if( object ){
				objindex = GET_OBJINDEX( object);
				bossindex = OBJECT_getIndex(objindex);
				CHAR_sendAction( bossindex , 2 , FALSE);	//�ε�	
			}
		}
		CHAR_setInt( meindex, CHAR_LOOPINTERVAL, GAMEOVERTIME );
		banker[bkid].state = 9;			
	}else{
		banker[bkid].turn += 1;
		if( banker[bkid].turn == 10 ){
			sprintf(buf,"��10�غ� ���һ�غφ� ս��");
		}else{
			sprintf(buf,"��%d�غ� ս��",banker[bkid].turn);
		}
		showString( meindex, buf , 0);
		banker[bkid].turntime = NowTime.tv_sec + 20 ;
		banker[bkid].process = 0;
		banker[bkid].select = 0;
		banker[bkid].action = 0;
		banker[bkid].state = 1; 
		CHAR_setInt( meindex, CHAR_LOOPINTERVAL, LOOPTIME );
	}
}

void NPC_magiccard_gameover( int meindex , int bkid )
{
	int i,allhp=0;
	char buf[256];

	
	for( i=0;i<MAXGUY;i++) allhp += banker[bkid].hp[i];	

	sprintf(buf,"ս����������ħ��HP%d��vs����ս���ܺ�HP%d��",banker[bkid].hp[MAXGUY],allhp);
	if( banker[bkid].hp[MAXGUY] >= allhp ){
		strcat(buf,"��ħ����ʤ");
	}else{
		strcat(buf,"��ϲ��ս�߻�ʤ");
	}
	showString( meindex, buf , 0);

	for( i=0;i<MAXGUY;i++) NPC_magiccard_out(  meindex , bkid , i );	//������ս�߳���
	NPC_magiccard_resetbanker(meindex,bkid);	
}

void NPC_magiccard_resetbanker( int meindex , int bkid )
{
	OBJECT object;
	int bossindex,objindex,offsetx,offsety;
	char argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	char buf[256];
	int j,k;
	
	if( NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr)) == NULL )	{
		print("\n read argstr err");
		return ;
	}		
	NPC_Util_GetStrFromStrWithDelim( argstr , "offsetx", buf, sizeof( buf));
	offsetx = atoi(buf);
	NPC_Util_GetStrFromStrWithDelim( argstr , "offsety", buf, sizeof( buf));
	offsety = atoi(buf);

	for(j=0;j<7;j++){
		for(k=0;k<30;k++){
			Cardbox[j+bkid*7][k].use = 0;
			Cardbox[j+bkid*7][k].cardindex = k;
		}
	}

	memset(&banker[bkid],0,sizeof(banker[bkid]));	
	banker[bkid].state = -1;
	CHAR_setInt( meindex, CHAR_LOOPINTERVAL, LOOPTIME );
	
	//�׶�����
	object = MAP_getTopObj(CHAR_getInt(meindex, CHAR_FLOOR), Positiontable[BANKERID].humanX + offsetx, Positiontable[BANKERID].humanY + offsety );
	if( object ){
		objindex = GET_OBJINDEX( object);
		bossindex = OBJECT_getIndex(objindex);
		CHAR_sendAction( bossindex , 3 , FALSE);
		CHAR_CHAT_DEBUG_metamo( bossindex , "0" );
	}	
}

int NPC_magiccard_checkExist( int meindex , int bkid , int player )
{
	int i,floor,offsetx,offsety;
	int playernum = CHAR_getPlayerMaxNum();
	char *cdkey = NULL;
	char *name = NULL;
	OBJECT object;
	char argstr[NPC_UTIL_GETARGSTR_BUFSIZE],buf[256];

	if( NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr)) == NULL )	{
		print("\n read argstr err");
		return -1;
	}		
	NPC_Util_GetStrFromStrWithDelim( argstr , "offsetx", buf, sizeof( buf));
	offsetx = atoi(buf);
	NPC_Util_GetStrFromStrWithDelim( argstr , "offsety", buf, sizeof( buf));
	offsety = atoi(buf);

	cdkey = banker[bkid].cdkey[player];
	name = banker[bkid].name[player];
	if( cdkey == NULL || name == NULL )
		return -1;
	for( i = 0 ; i < playernum ; i ++) {
		if( CHAR_CHECKINDEX( i) &&
			strcmp( CHAR_getChar( i, CHAR_CDKEY) , cdkey ) == 0 &&
			strcmp( CHAR_getChar( i, CHAR_NAME) , name ) == 0 ){
			break;
		}
	}
	if( i == playernum ) return -1;		//û���ҵ���ս��
	floor = CHAR_getInt( meindex , CHAR_FLOOR);
	object = MAP_getTopObj( floor, Positiontable[player].humanX + offsetx , Positiontable[player].humanY + offsety );
	if( object ){  
		int findex;
		int objindex = GET_OBJINDEX(object);
		findex = OBJECT_getIndex( objindex);
		if( findex == i ) 
			return i;		//���ҵ�
	}
	return -1;	
}

int NPC_magiccard_sort( const void *card1, const void *card2 )
{
	return ( ((CardBoxType*)card1)->use > ((CardBoxType*)card2)->use )? 1:0;
}
#endif
