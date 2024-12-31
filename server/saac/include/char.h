#ifndef _CHAR_H_
#define _CHAR_H_
#include "version.h"

/* 1������ؤ��Ի��    ƽ�ҷ���(�����̻���) */
#ifdef _MORECHARACTERS_
#define MAXCHAR_PER_USER 4
#else
#define MAXCHAR_PER_USER 2
#endif

int isLocked( char *id );
void charLoadCallback( int ti , int auth_ok , char *c0, char* c1 ,
                       char *c2 , char *c3, char *c4, int i0 , int i1 );
// Spock 2000/11/1
//void charSave( int ti , char *c0 , 
//                       char *c2 , char *c3 , char *c4, int i0 , int i1 );
// return process number
#ifdef _NewSave
int charSave( int ti , char *c0 , 
                       char *c2 , char *c3 , char *c4, int i0 , int i1 , int charindex );
#else
int charSave( int ti , char *c0 , 
                       char *c2 , char *c3 , char *c4, int i0 , int i1 );
#endif
// Spock end

void  charListCallback( int ti , int auth_ok , char *c0 , char *c1 ,
                        char *c2 , char *c3 , char *c4 , int i0 , int i1,int charlistflg );

void charDeleteCallback( int ti , int auth_ok ,
                         char *c0 , char *c1 , char *c2 , char *c3 ,
                         char *c4 , int i0 , int i1 );
// Nuke
void dummyCallback( int ti , int auth_ok ,
                         char *c0 , char *c1 , char *c2 , char *c3 ,
                         char *c4 , int i0 , int i1 );


// Spock 2000/11/1
#ifdef _LOCK_ADD_NAME
int lockUser(  char *gmsvname , char *id , char *name, char *passwd , int lock , 
               char *result, int resultlen,
               char *retdata , int retdatalen , char *process , char *deadline);
#else
int lockUser(  char *gmsvname , char *id , char *passwd , int lock , 
               char *result, int resultlen,
               char *retdata , int retdatalen , char *process , char *deadline);
#endif
// Spock end

int checkValidCharName( char *a);

int loadCharOne( char *idstring , int num, char *output , int outlen );
int saveCharOne( char *idstring , int num , char *input );

int loadCharNameAndOption( char *id , char *output , int outlen);
int getCharIndexByName( char *id , char *charname );
void getCharInfoFromString( char *str , char *out );

#ifdef _CHAR_POOLITEM
int InsertCharPoolItem( char *id , char *input, int sizes);
int saveCharPoolItem( char *id , char *input, int sizes);
int loadCharPoolItemOne( char *id , char *output , int outlen);
int checkCharPoolItem( char *id);
#endif

#ifdef _CHAR_POOLPET
int InsertCharPoolPet( char *id , char *input, int sizes);
int saveCharPoolPet( char *id , char *input, int sizes);
int loadCharPoolPetOne( char *id , char *output , int outlen);
int checkCharPoolPet( char *id);
#endif


#ifdef _ANGEL_SUMMON

#define MAXMISSIONTABLE 200

struct MissionTable
{
	char angelinfo[64];
	char heroinfo[64];
	int mission;
	int flag;
	int time;
	int limittime;
};

extern struct MissionTable missiontable[MAXMISSIONTABLE];

#endif

#endif

