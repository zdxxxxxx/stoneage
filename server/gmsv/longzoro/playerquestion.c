#include <string.h>
#include "version.h"
#include "longzoro/playerquestion.h"
#include "char_base.h"
#include "char.h"
#include "autil.h"
#include "enemy.h"
#include "family.h"
#include "log.h"

#ifdef _PLAYER_QUESTION_ONLIEN
PlayerQuestion_t PlayerQuestion;

BOOL PlayQuestionOnline(int charaindex, char *messageeraseescape)
{
	if(messageeraseescape[0] == '/' && messageeraseescape[1] == 'h' && messageeraseescape[2] == 'd'){
		
		if(strlen(PlayerQuestion.question)==0 || strlen(PlayerQuestion.result)==0){
			CHAR_talkToCli(charaindex,-1,"GM��δ���߷�����!",CHAR_COLORRED);
			return FALSE;
		}
		
		char token[32];
		easyGetTokenFromString( messageeraseescape , 2 , token, sizeof(token));
		
		if(strcmp( token, PlayerQuestion.result) == 0){
			switch(PlayerQuestion.type){
				case 0:
					// ��Ǯ
				  CHAR_setInt(charaindex,CHAR_GOLD, CHAR_getInt(charaindex,CHAR_GOLD) + PlayerQuestion.value);
					CHAR_send_P_StatusString(charaindex,CHAR_P_STRING_GOLD);
					
					break;
				case 1:
					{
					  int itemindex=-1;
					  char token[256];
					  int ret;

					  itemindex = CHAR_findEmptyItemBox( charaindex );
						if( itemindex < 0 )	{
							CHAR_talkToCli( charaindex, -1, "�ܱ�Ǹ,�����Ʒ������!",  CHAR_COLORYELLOW);
							break;
						}
	
						itemindex = ITEM_makeItemAndRegist( PlayerQuestion.value );
					  if( itemindex != -1 ){
							ret = CHAR_addItemSpecificItemIndex( charaindex, itemindex);
#ifdef _NEW_ITEM_
extern int CheckCharMaxItem(int charindex);
#endif
							if( ret < 0 ||
#ifdef _NEW_ITEM_
								ret >= CheckCharMaxItem(charaindex)
#else
								ret >= CHAR_MAXITEMHAVE
#endif
								) {
								ITEM_endExistItemsOne( itemindex);
								CHAR_talkToCli( charaindex, -1, "�ܱ�Ǹ,�޷���������Ʒ����!",  CHAR_COLORYELLOW);
							break;
							}
					
							sprintf( token,"��ϲ����:%s",ITEM_getChar( itemindex, ITEM_NAME));
							CHAR_talkToCli( charaindex, -1, token, CHAR_COLORYELLOW );
	
							CHAR_sendItemDataOne( charaindex, ret);
					  }
					}
					break;
				case 2:
					{
						char msgbuf[64];
						int	enemynum;
						int	i,j;
						int petindex;
					
						for( i = 0 ;i < CHAR_MAXPETHAVE ; i++) {
							if( CHAR_getCharPet( charaindex, i) == -1  )
								break;
						}
					
					  if( i == CHAR_MAXPETHAVE ){
							CHAR_talkToCli( charaindex, -1, "�ܱ�Ǹ,�����ϳ�������!",  CHAR_COLORYELLOW);
							break;
						}
					
						enemynum = ENEMY_getEnemyNum();
						for( i = 0; i < enemynum; i ++ ) {
							if( ENEMY_getInt( i, ENEMY_ID) == PlayerQuestion.value) {
									break;
								}
						}
					
						if( i == enemynum ){
							CHAR_talkToCli( charaindex, -1, "�ܱ�Ǹ,�ó��ﲻ����",  CHAR_COLORYELLOW);
							break;
						}
					
						petindex = ENEMY_createPetFromEnemyIndex( charaindex, i);
						for( i = 0; i < CHAR_MAXPETHAVE; i ++ )	{
							if( CHAR_getCharPet( charaindex, i ) == petindex )
								break;
						}
		
						if( !CHAR_CHECKINDEX( petindex) ){
							CHAR_talkToCli( charaindex, -1, "�ܱ�Ǹ,�ó��ﲻ����",  CHAR_COLORYELLOW);
						}
						snprintf( msgbuf,sizeof( msgbuf), "��ϲ���ó���:%s��", CHAR_getChar(petindex,CHAR_NAME));
						CHAR_talkToCli( charaindex, -1, msgbuf,  CHAR_COLORYELLOW);
						CHAR_setInt(petindex,CHAR_VARIABLEAI,10000);
						for(j = 0; j < CHAR_MAXPETHAVE; j++){
							petindex = CHAR_getCharPet(charaindex, j);
							if( !CHAR_CHECKINDEX( petindex) )
								continue;
							CHAR_complianceParameter( petindex );
							snprintf( msgbuf, sizeof( msgbuf ), "K%d", j );
							CHAR_sendStatusString( charaindex, msgbuf );
							snprintf( msgbuf, sizeof( msgbuf ), "W%d", j );
							CHAR_sendStatusString( charaindex, msgbuf );
						}
					}
					break;
				case 3:
					CHAR_setInt(charaindex,CHAR_FAME,CHAR_getInt(charaindex,CHAR_FAME) + PlayerQuestion.value * 100);
/*
					char buf[64];
					sprintf(buf,"%d", CHAR_getInt(charaindex,CHAR_FAME));
					saacproto_ACFixFMData_send(acfd,
							CHAR_getChar(charaindex,CHAR_FMNAME),CHAR_getInt(charaindex,CHAR_FMINDEX),CHAR_getWorkInt(charaindex,CHAR_WORKFMINDEXI),
							FM_FIX_FMFEED,buf,"",CHAR_getWorkInt(charaindex,CHAR_WORKFMCHARINDEX),CONNECT_getFdid(getfdFromCharaIndex(charaindex)));
*/
					sprintf( token, "��ϲ���ø���������%d", PlayerQuestion.value);
					CHAR_talkToCli(charaindex,-1,token,CHAR_COLORRED);
					break;
				case 4:
					CHAR_setInt(charaindex,CHAR_AMPOINT,CHAR_getInt(charaindex,CHAR_AMPOINT) + PlayerQuestion.value);
#ifdef _AMPOINT_LOG
					LogAmPoint(CHAR_getChar( charaindex, CHAR_NAME ),
									         	CHAR_getChar( charaindex, CHAR_CDKEY ),
									          PlayerQuestion.value,
									   				CHAR_getInt( charaindex, CHAR_AMPOINT ),
									         	"(��������)",
									         	CHAR_getInt( charaindex,CHAR_FLOOR),
									         	CHAR_getInt( charaindex,CHAR_X ),
									         	CHAR_getInt( charaindex,CHAR_Y ));
#endif
					sprintf( token, "��ϲ���ø��˻��֣�%d", PlayerQuestion.value);
					CHAR_talkToCli(charaindex,-1,token,CHAR_COLORRED);
					break;
			}
			{
				char type[5][32]={"ʯ��","���߱��Ϊ","������Ϊ","����","����"};
				
				int i;
				int playernum = CHAR_getPlayerMaxNum();
			  char token1[128];
			  char token2[128];
			  sprintf(token1, "��ϲ��� %s �ش���ȷ,����:%s", CHAR_getChar(charaindex, CHAR_NAME), PlayerQuestion.result);
				sprintf(token2, "�����ǹ�ϲ�����%s %d", type[PlayerQuestion.type], PlayerQuestion.value);

				for( i = 0 ; i < playernum ; i++) {
				  if( CHAR_getCharUse(i) != FALSE ){
				  	CHAR_talkToCli( i, -1, token1, CHAR_COLORRED);
				  	CHAR_talkToCli( i, -1, token2, CHAR_COLORRED);
					}
				}
			}
			strcpy(PlayerQuestion.question,"");
			strcpy(PlayerQuestion.result,"");
		}else{
			CHAR_talkToCli(charaindex,-1,"�ܱ�Ǹ,��ش���ȷ!",CHAR_COLORRED);
			return FALSE;
		}
	}
	return FALSE;
}

#endif


