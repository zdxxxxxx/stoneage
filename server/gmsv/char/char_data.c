#include "version.h"
#include <string.h>
#include <math.h>
#ifdef _REDHAT_V9
#include <errno.h>
#endif
#include "autil.h"
#include "readmap.h"
#include "common.h"
#include "util.h"
#include "anim_tbl.h"
#include "battle.h"
#include "char.h"
#include "char_data.h"
#include "buf.h"
#include "item.h"
#include "log.h"
#include "pet.h"
#include "enemy.h"
#ifdef _PERSONAL_FAME	// Arminius: �����������
#include "char_base.h"
#include "configfile.h"
#endif
#ifdef _NEW_ITEM_
extern int CheckCharMaxItem(int charindex);
#endif

#include "defaultPlayer.h"

#include "ls2data.dat"

// CoolFish: Family 2001/7/28
#include "family.h"
#include "saacproto_cli.h"

#ifdef	_PET_LIMITLEVEL
void CHAR_CheckPetDoLimitlevel( int petindex , int toindex, int level);	
#endif


typedef struct tagdefaultCharcterGet
{
    int             imagenumber;
    Char*           initchardata;
    LevelUpPattern* lvpattern;
    CHAR_ImageType  imgtype;
}defaultCharacterGet;

static defaultCharacterGet CHAR_defaultCharacterGet[]=
{
  /*  CHAR_DEFAULTPLAYER  */
//�Ҽ��ɨ�
    {SPR_001em,&player,&lvplayer00,CHAR_IMAGETYPE_CHILDBOY},
    {SPR_002em,&player,&lvplayer00,CHAR_IMAGETYPE_CHILDBOY},
    {SPR_003em,&player,&lvplayer00,CHAR_IMAGETYPE_CHILDBOY},
    {SPR_004em,&player,&lvplayer00,CHAR_IMAGETYPE_CHILDBOY},

//��  ��
    {SPR_011em,&player,&lvplayer00,CHAR_IMAGETYPE_BOY},
    {SPR_012em,&player,&lvplayer00,CHAR_IMAGETYPE_BOY},
    {SPR_013em,&player,&lvplayer00,CHAR_IMAGETYPE_BOY},
    {SPR_014em,&player,&lvplayer00,CHAR_IMAGETYPE_BOY},

//��  ��
    {SPR_021em,&player,&lvplayer00,CHAR_IMAGETYPE_BOY},
    {SPR_022em,&player,&lvplayer00,CHAR_IMAGETYPE_BOY},
    {SPR_023em,&player,&lvplayer00,CHAR_IMAGETYPE_BOY},
    {SPR_024em,&player,&lvplayer00,CHAR_IMAGETYPE_BOY},

//��  ��
    {SPR_031em,&player,&lvplayer00,CHAR_IMAGETYPE_BOY},
    {SPR_032em,&player,&lvplayer00,CHAR_IMAGETYPE_BOY},
    {SPR_033em,&player,&lvplayer00,CHAR_IMAGETYPE_BOY},
    {SPR_034em,&player,&lvplayer00,CHAR_IMAGETYPE_BOY},

//��  ��
    {SPR_041em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
    {SPR_042em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
    {SPR_043em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
    {SPR_044em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},

//��  ��
    {SPR_051em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
    {SPR_052em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
    {SPR_053em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
    {SPR_054em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},

//�Ҽ�Ԩ�
    {SPR_061em,&player,&lvplayer00,CHAR_IMAGETYPE_CHILDGIRL},
    {SPR_062em,&player,&lvplayer00,CHAR_IMAGETYPE_CHILDGIRL},
    {SPR_063em,&player,&lvplayer00,CHAR_IMAGETYPE_CHILDGIRL},
    {SPR_064em,&player,&lvplayer00,CHAR_IMAGETYPE_CHILDGIRL},

//���Ԩ�
    {SPR_071em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},
    {SPR_072em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},
    {SPR_073em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},
    {SPR_074em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},

//���Ԩ�
    {SPR_081em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},
    {SPR_082em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},
    {SPR_083em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},
    {SPR_084em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},

//���Ԩ�
    {SPR_091em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},
    {SPR_092em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},
    {SPR_093em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},
    {SPR_094em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},

//�����
    {SPR_101em,&player,&lvplayer00,CHAR_IMAGETYPE_WOMAN},
    {SPR_102em,&player,&lvplayer00,CHAR_IMAGETYPE_WOMAN},
    {SPR_103em,&player,&lvplayer00,CHAR_IMAGETYPE_WOMAN},
    {SPR_104em,&player,&lvplayer00,CHAR_IMAGETYPE_WOMAN},

//�����
    {SPR_111em,&player,&lvplayer00,CHAR_IMAGETYPE_WOMAN},
    {SPR_112em,&player,&lvplayer00,CHAR_IMAGETYPE_WOMAN},
    {SPR_113em,&player,&lvplayer00,CHAR_IMAGETYPE_WOMAN},
    {SPR_114em,&player,&lvplayer00,CHAR_IMAGETYPE_WOMAN},

    {SPR_pet001,&player,&lvplayer00,CHAR_IMAGETYPE_DOG},
    {SPR_pet002,&player,&lvplayer00,CHAR_IMAGETYPE_DOG},
    {SPR_pet003,&player,&lvplayer00,CHAR_IMAGETYPE_DOG},
    {SPR_pet004,&player,&lvplayer00,CHAR_IMAGETYPE_DOG},
    {SPR_pet011,&player,&lvplayer00,CHAR_IMAGETYPE_DOG},

#ifdef _MO_IMAGE_EXTENSION
    {SPRNEW_001em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
    {SPRNEW_002em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
    {SPRNEW_003em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
    {SPRNEW_004em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},

    {SPRNEW_005em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},
    {SPRNEW_006em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},
    {SPRNEW_007em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},
    {SPRNEW_008em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},

    {SPRNEW_009em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},
    {SPRNEW_010em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},
    {SPRNEW_011em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},
    {SPRNEW_012em,&player,&lvplayer00,CHAR_IMAGETYPE_GIRL},

    {SPRNEW_013em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
    {SPRNEW_014em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
    {SPRNEW_015em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
    {SPRNEW_016em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},

    {SPRNEW_017em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
    {SPRNEW_018em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
    {SPRNEW_019em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
    {SPRNEW_020em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},

    {SPRNEW_021em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
    {SPRNEW_022em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
    {SPRNEW_023em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
    {SPRNEW_024em,&player,&lvplayer00,CHAR_IMAGETYPE_MAN},
#endif
};

BOOL CHAR_getDefaultChar( Char*  nc  , int imagenumber )
{
	int     i, j;
	int     defcharaindex;

	Char*   defaultchar;
	defaultchar = CHAR_defaultCharacterGet[arraysizeof(CHAR_defaultCharacterGet) - 1].initchardata;

	memset( nc,0,sizeof(Char) );
	defcharaindex = 0;
    for( i = 0 ; i < arraysizeof( CHAR_defaultCharacterGet ) ; i  ++ ){
		 if( CHAR_defaultCharacterGet[i].imagenumber == imagenumber ){
            defaultchar = CHAR_defaultCharacterGet[i].initchardata;
            defcharaindex = i;
            break;
		 }
	}
	nc->data[CHAR_IMAGETYPE] = CHAR_defaultCharacterGet[defcharaindex].imgtype;
    nc->use = TRUE;

#ifdef _CHAR_FIXDATADEF
	for( j = 0 ; j < CHAR_DATAINTNUM ; j ++ ){
		if( j < CHAR_INITDATA ) nc->data[j] = defaultchar->data[j];
		else nc->data[j] = 0;
	}
#else
    for( j = 0 ; j < CHAR_DATAINTNUM ; j ++ )
        nc->data[j] = defaultchar->data[j];
#endif
    for( j = 0 ; j < arraysizeof( nc->flg ) ; j ++ )
        nc->flg[j] = defaultchar->flg[j];
	extern int CheckCharMaxItemChar(Char* ch);
    for( j = 0 ; j < CHAR_DATACHARNUM ; j ++ )
		nc->string[j].string[0] = '\0';

    for( j = 0 ; j <CHAR_MAXITEMHAVE ; j ++ )
        nc->indexOfExistItems[j] = -1;
    for( j = 0 ; j < CHAR_MAXPETHAVE ; j ++ )
        nc->unionTable.indexOfPet[j] = -1;
    for( j = 0 ; j < CHAR_MAXPOOLITEMHAVE ; j ++ )
        nc->indexOfExistPoolItems[j] = -1;
    for( j = 0 ; j < CHAR_MAXPETSKILLHAVE ; j ++ )
        nc->unionTable.indexOfPetskill[j] = -1;
    for( j = 0 ; j < CHAR_MAXPOOLPETHAVE ; j ++ )
        nc->indexOfPoolPet[j] = -1;
    for( j = 0 ; j < CHAR_SKILLMAXHAVE ; j ++ )
        nc->haveSkill[j].use = FALSE;
    for( j = 0 ; j < CHAR_TITLEMAXHAVE ; j ++ )
        nc->indexOfHaveTitle[j] = -1;
    for( j=0 ; j<ADDRESSBOOK_MAX ;j++ )
        memset( &nc->addressBook[j],0,sizeof(nc->addressBook[j]) );
    for( j = 0 ; j < CHAR_WORKDATAINTNUM ; j ++ )
        nc->workint[j] = 0;
    for( j = 0 ; j < CHAR_WORKDATACHARNUM ; j ++ )
        nc->workchar[j].string[0] = '\0';
	nc->workint[CHAR_WORKFD] = -1;
#ifdef _CHATROOMPROTOCOL			// (���ɿ�) Syu ADD ������Ƶ��
	nc->workint[CHAR_WORKCHATROOMNUM] = -1;
#endif
    return TRUE;
}

static int CHAR_playerImageNumber[]=
{
  /*  CHAR_DEFAULTPLAYER  */
    SPR_001em,SPR_002em,SPR_003em,SPR_004em,
    SPR_011em,SPR_012em,SPR_013em,SPR_014em,
    SPR_021em,SPR_022em,SPR_023em,SPR_024em,
    SPR_031em,SPR_032em,SPR_033em,SPR_034em,
    SPR_041em,SPR_042em,SPR_043em,SPR_044em,
    SPR_051em,SPR_052em,SPR_053em,SPR_054em,
    SPR_061em,SPR_062em,SPR_063em,SPR_064em,
    SPR_071em,SPR_072em,SPR_073em,SPR_074em,
    SPR_081em,SPR_082em,SPR_083em,SPR_084em,
    SPR_091em,SPR_092em,SPR_093em,SPR_094em,
    SPR_101em,SPR_102em,SPR_103em,SPR_104em,
    SPR_111em,SPR_112em,SPR_113em,SPR_114em
#ifdef _MO_IMAGE_EXTENSION
,SPRNEW_001em,SPRNEW_001ax,SPRNEW_001cl,SPRNEW_001sp,SPRNEW_001bw
,SPRNEW_002em,SPRNEW_002ax,SPRNEW_002cl,SPRNEW_002sp,SPRNEW_002bw
,SPRNEW_003em,SPRNEW_003ax,SPRNEW_003cl,SPRNEW_003sp,SPRNEW_003bw
,SPRNEW_004em,SPRNEW_004ax,SPRNEW_004cl,SPRNEW_004sp,SPRNEW_004bw
,SPRNEW_005em,SPRNEW_005ax,SPRNEW_005cl,SPRNEW_005sp,SPRNEW_005bw
,SPRNEW_006em,SPRNEW_006ax,SPRNEW_006cl,SPRNEW_006sp,SPRNEW_006bw
,SPRNEW_007em,SPRNEW_007ax,SPRNEW_007cl,SPRNEW_007sp,SPRNEW_007bw
,SPRNEW_008em,SPRNEW_008ax,SPRNEW_008cl,SPRNEW_008sp,SPRNEW_008bw
,SPRNEW_009em,SPRNEW_009ax,SPRNEW_009cl,SPRNEW_009sp,SPRNEW_009bw
,SPRNEW_010em,SPRNEW_010ax,SPRNEW_010cl,SPRNEW_010sp,SPRNEW_010bw
,SPRNEW_011em,SPRNEW_011ax,SPRNEW_011cl,SPRNEW_011sp,SPRNEW_011bw
,SPRNEW_012em,SPRNEW_012ax,SPRNEW_012cl,SPRNEW_012sp,SPRNEW_012bw
,SPRNEW_013em,SPRNEW_013ax,SPRNEW_013cl,SPRNEW_013sp,SPRNEW_013bw
,SPRNEW_014em,SPRNEW_014ax,SPRNEW_014cl,SPRNEW_014sp,SPRNEW_014bw
,SPRNEW_015em,SPRNEW_015ax,SPRNEW_015cl,SPRNEW_015sp,SPRNEW_015bw
,SPRNEW_016em,SPRNEW_016ax,SPRNEW_016cl,SPRNEW_016sp,SPRNEW_016bw
,SPRNEW_017em,SPRNEW_017ax,SPRNEW_017cl,SPRNEW_017sp,SPRNEW_017bw
,SPRNEW_018em,SPRNEW_018ax,SPRNEW_018cl,SPRNEW_018sp,SPRNEW_018bw
,SPRNEW_019em,SPRNEW_019ax,SPRNEW_019cl,SPRNEW_019sp,SPRNEW_019bw
,SPRNEW_020em,SPRNEW_020ax,SPRNEW_020cl,SPRNEW_020sp,SPRNEW_020bw
,SPRNEW_021em,SPRNEW_021ax,SPRNEW_021cl,SPRNEW_021sp,SPRNEW_021bw
,SPRNEW_022em,SPRNEW_022ax,SPRNEW_022cl,SPRNEW_022sp,SPRNEW_022bw
,SPRNEW_023em,SPRNEW_023ax,SPRNEW_023cl,SPRNEW_023sp,SPRNEW_023bw
,SPRNEW_024em,SPRNEW_024ax,SPRNEW_024cl,SPRNEW_024sp,SPRNEW_024bw
#endif
};

BOOL CHAR_checkPlayerImageNumber( int imagenumber)
{
	int i;
	for( i = 0; i < arraysizeof( CHAR_playerImageNumber); i ++  ) {
		if( imagenumber == CHAR_playerImageNumber[i] ) break;
	}
	if( i == arraysizeof( CHAR_playerImageNumber)) return FALSE;
	else return TRUE;
}
/*------------------------------------------------------------
 * ӿ  �  į����������
 *
 * ӿ  �  į����Ʊ�����ɡ�Ѩ����ƥ����Ȼ�����
 * number = ����  ��
 * CG_CHR_MAKE_FACE + (number*100) + (  ��  number * 25) + (     į * 5 ) + ��  į
 *
 ------------------------------------------------------------*/
BOOL CHAR_checkFaceImageNumber( int imagenumber, int faceimagenumber)
{
#ifdef _MO_IMAGE_EXTENSION

	if (imagenumber>=SPRNEW_001em) {
		/*
		int image = imagenumber - SPRNEW_001em;
		int number;
		int color;
		number = (faceimagenumber - CG_CHR_MAKE_NEWFACE)/ 100*20;
		color = ((faceimagenumber - CG_CHR_MAKE_NEWFACE)% 100) / 25*5;
		if( image != number + color ) return FALSE;
		*/
		return TRUE;
	}else
#endif
	{
	int image = imagenumber - SPR_001em;
	int number;
	int color;

	number = (faceimagenumber - CG_CHR_MAKE_FACE)/ 100*20;
	color = ((faceimagenumber - CG_CHR_MAKE_FACE)% 100) / 25*5;
	if( image != number + color ) return FALSE;
	return TRUE;
	}
}
// ƽ�ҷ�  �  ��Ƥ��ۨ�  ؤ��Ի��  ����
int CHAR_eqimagetbl[][5]={
    // ��ƥ       �������� ����       ��         ��
    { SPR_001em,SPR_001ax,SPR_001cl,SPR_001sp,SPR_001bw },
    { SPR_002em,SPR_002ax,SPR_002cl,SPR_002sp,SPR_002bw },
    { SPR_003em,SPR_003ax,SPR_003cl,SPR_003sp,SPR_003bw },
    { SPR_004em,SPR_004ax,SPR_004cl,SPR_004sp,SPR_004bw },
    { SPR_011em,SPR_011ax,SPR_011cl,SPR_011sp,SPR_011bw },
    { SPR_012em,SPR_012ax,SPR_012cl,SPR_012sp,SPR_012bw },
    { SPR_013em,SPR_013ax,SPR_013cl,SPR_013sp,SPR_013bw },
    { SPR_014em,SPR_014ax,SPR_014cl,SPR_014sp,SPR_014bw },
    { SPR_021em,SPR_021ax,SPR_021cl,SPR_021sp,SPR_021bw },
    { SPR_022em,SPR_022ax,SPR_022cl,SPR_022sp,SPR_022bw },
    { SPR_023em,SPR_023ax,SPR_023cl,SPR_023sp,SPR_023bw },
    { SPR_024em,SPR_024ax,SPR_024cl,SPR_024sp,SPR_024bw },
    { SPR_021em,SPR_021ax,SPR_021cl,SPR_021sp,SPR_021bw },
    { SPR_031em,SPR_031ax,SPR_031cl,SPR_031sp,SPR_031bw },
    { SPR_032em,SPR_032ax,SPR_032cl,SPR_032sp,SPR_032bw },
    { SPR_033em,SPR_033ax,SPR_033cl,SPR_033sp,SPR_033bw },
    { SPR_034em,SPR_034ax,SPR_034cl,SPR_034sp,SPR_034bw },
    { SPR_041em,SPR_041ax,SPR_041cl,SPR_041sp,SPR_041bw },
    { SPR_042em,SPR_042ax,SPR_042cl,SPR_042sp,SPR_042bw },
    { SPR_043em,SPR_043ax,SPR_043cl,SPR_043sp,SPR_043bw },
    { SPR_044em,SPR_044ax,SPR_044cl,SPR_044sp,SPR_044bw },

    { SPR_051em,SPR_051ax,SPR_051cl,SPR_051sp,SPR_051bw },
    { SPR_052em,SPR_052ax,SPR_052cl,SPR_052sp,SPR_052bw },
    { SPR_053em,SPR_053ax,SPR_053cl,SPR_053sp,SPR_053bw },
    { SPR_054em,SPR_054ax,SPR_054cl,SPR_054sp,SPR_054bw },

    { SPR_061em,SPR_061ax,SPR_061cl,SPR_061sp,SPR_061bw },
    { SPR_062em,SPR_062ax,SPR_062cl,SPR_062sp,SPR_062bw },
    { SPR_063em,SPR_063ax,SPR_063cl,SPR_063sp,SPR_063bw },
    { SPR_064em,SPR_064ax,SPR_064cl,SPR_064sp,SPR_064bw },

    { SPR_071em,SPR_071ax,SPR_071cl,SPR_071sp,SPR_071bw },
    { SPR_072em,SPR_072ax,SPR_072cl,SPR_072sp,SPR_072bw },
    { SPR_073em,SPR_073ax,SPR_073cl,SPR_073sp,SPR_073bw },
    { SPR_074em,SPR_074ax,SPR_074cl,SPR_074sp,SPR_074bw },

    { SPR_081em,SPR_081ax,SPR_081cl,SPR_081sp,SPR_081bw },
    { SPR_082em,SPR_082ax,SPR_082cl,SPR_082sp,SPR_082bw },
    { SPR_083em,SPR_083ax,SPR_083cl,SPR_083sp,SPR_083bw },
    { SPR_084em,SPR_084ax,SPR_084cl,SPR_084sp,SPR_084bw },

    { SPR_091em,SPR_091ax,SPR_091cl,SPR_091sp,SPR_091bw },
    { SPR_092em,SPR_092ax,SPR_092cl,SPR_092sp,SPR_092bw },
    { SPR_093em,SPR_093ax,SPR_093cl,SPR_093sp,SPR_093bw },
    { SPR_094em,SPR_094ax,SPR_094cl,SPR_094sp,SPR_094bw },

    { SPR_101em,SPR_101ax,SPR_101cl,SPR_101sp,SPR_101bw },
    { SPR_102em,SPR_102ax,SPR_102cl,SPR_102sp,SPR_102bw },
    { SPR_103em,SPR_103ax,SPR_103cl,SPR_103sp,SPR_103bw },
    { SPR_104em,SPR_104ax,SPR_104cl,SPR_104sp,SPR_104bw },

    { SPR_111em,SPR_111ax,SPR_111cl,SPR_111sp,SPR_111bw },
    { SPR_112em,SPR_112ax,SPR_112cl,SPR_112sp,SPR_112bw },
    { SPR_113em,SPR_113ax,SPR_113cl,SPR_113sp,SPR_113bw },
    { SPR_114em,SPR_114ax,SPR_114cl,SPR_114sp,SPR_114bw },

    { SPR_091em,SPR_091ax,SPR_091cl,SPR_091sp,SPR_091bw },
    { SPR_092em,SPR_092ax,SPR_092cl,SPR_092sp,SPR_092bw },

	{ SPR_pet001,SPR_pet001,SPR_pet001,SPR_pet001,SPR_pet001},
	{ SPR_pet002,SPR_pet002,SPR_pet002,SPR_pet002,SPR_pet002},
	{ SPR_pet003,SPR_pet003,SPR_pet003,SPR_pet003,SPR_pet003},
	{ SPR_pet004,SPR_pet004,SPR_pet004,SPR_pet004,SPR_pet004},

	{ SPR_pet011,SPR_pet011,SPR_pet011,SPR_pet011,SPR_pet011},

    { SPR_121em,SPR_121ax,SPR_121cl,SPR_121sp,SPR_121bw },
    { SPR_122em,SPR_122ax,SPR_122cl,SPR_122sp,SPR_122bw },

    { SPR_131em,SPR_131ax,SPR_131cl,SPR_131sp,SPR_131bw },
    { SPR_132em,SPR_132ax,SPR_132cl,SPR_132sp,SPR_132bw },
    { SPR_133em,SPR_133ax,SPR_133cl,SPR_133sp,SPR_133bw },

    { SPR_141em,SPR_141ax,SPR_141cl,SPR_141sp,SPR_141bw },
    { SPR_142em,SPR_142ax,SPR_142cl,SPR_142sp,SPR_142bw },

    { SPR_151em,SPR_151ax,SPR_151cl,SPR_151sp,SPR_151bw },
    { SPR_152em,SPR_152ax,SPR_152cl,SPR_152sp,SPR_152bw },

    { SPR_161em,SPR_161ax,SPR_161cl,SPR_161sp,SPR_161bw },
    { SPR_162em,SPR_162ax,SPR_162cl,SPR_162sp,SPR_162bw },
    
    { 100400, 100401, 100402, 100403, 100404 },
		{ 100405, 100406, 100407, 100408, 100409 },
		
    { 100431, 100432, 100433, 100434, 100435 },
		{ 100436, 100437, 100438, 100439, 100440 },
				
		{ 103000, 103001, 103002, 103003, 103004 },
		{ 103005, 103006, 103007, 103008, 103009 },
		{ 103010, 103011, 103012, 103013, 103014 },
		{ 103015, 103016, 103017, 103018, 103019 },
		{ 103020, 103021, 103022, 103023, 103024 },

#ifdef _MO_IMAGE_EXTENSION
		{ SPRNEW_001em,SPRNEW_001ax,SPRNEW_001cl,SPRNEW_001sp,SPRNEW_001bw },
		{ SPRNEW_002em,SPRNEW_002ax,SPRNEW_002cl,SPRNEW_002sp,SPRNEW_002bw },
		{ SPRNEW_003em,SPRNEW_003ax,SPRNEW_003cl,SPRNEW_003sp,SPRNEW_003bw },
		{ SPRNEW_004em,SPRNEW_004ax,SPRNEW_004cl,SPRNEW_004sp,SPRNEW_004bw },
		{ SPRNEW_005em,SPRNEW_005ax,SPRNEW_005cl,SPRNEW_005sp,SPRNEW_005bw },
		{ SPRNEW_006em,SPRNEW_006ax,SPRNEW_006cl,SPRNEW_006sp,SPRNEW_006bw },
		{ SPRNEW_007em,SPRNEW_007ax,SPRNEW_007cl,SPRNEW_007sp,SPRNEW_007bw },
		{ SPRNEW_008em,SPRNEW_008ax,SPRNEW_008cl,SPRNEW_008sp,SPRNEW_008bw },
		{ SPRNEW_009em,SPRNEW_009ax,SPRNEW_009cl,SPRNEW_009sp,SPRNEW_009bw },
		{ SPRNEW_010em,SPRNEW_010ax,SPRNEW_010cl,SPRNEW_010sp,SPRNEW_010bw },
		{ SPRNEW_011em,SPRNEW_011ax,SPRNEW_011cl,SPRNEW_011sp,SPRNEW_011bw },
		{ SPRNEW_012em,SPRNEW_012ax,SPRNEW_012cl,SPRNEW_012sp,SPRNEW_012bw },
		{ SPRNEW_013em,SPRNEW_013ax,SPRNEW_013cl,SPRNEW_013sp,SPRNEW_013bw },
		{ SPRNEW_014em,SPRNEW_014ax,SPRNEW_014cl,SPRNEW_014sp,SPRNEW_014bw },
		{ SPRNEW_015em,SPRNEW_015ax,SPRNEW_015cl,SPRNEW_015sp,SPRNEW_015bw },
		{ SPRNEW_016em,SPRNEW_016ax,SPRNEW_016cl,SPRNEW_016sp,SPRNEW_016bw },
		{ SPRNEW_017em,SPRNEW_017ax,SPRNEW_017cl,SPRNEW_017sp,SPRNEW_017bw },
		{ SPRNEW_018em,SPRNEW_018ax,SPRNEW_018cl,SPRNEW_018sp,SPRNEW_018bw },
		{ SPRNEW_019em,SPRNEW_019ax,SPRNEW_019cl,SPRNEW_019sp,SPRNEW_019bw },
		{ SPRNEW_020em,SPRNEW_020ax,SPRNEW_020cl,SPRNEW_020sp,SPRNEW_020bw },
		{ SPRNEW_021em,SPRNEW_021ax,SPRNEW_021cl,SPRNEW_021sp,SPRNEW_021bw },
		{ SPRNEW_022em,SPRNEW_022ax,SPRNEW_022cl,SPRNEW_022sp,SPRNEW_022bw },
		{ SPRNEW_023em,SPRNEW_023ax,SPRNEW_023cl,SPRNEW_023sp,SPRNEW_023bw },
		{ SPRNEW_024em,SPRNEW_024ax,SPRNEW_024cl,SPRNEW_024sp,SPRNEW_024bw },
#endif
};

/*------------------------------------------------------------
 * ��    ��  �  į����ޥ����  �  įë  �£�
 * ¦��
 *  basebaseimagenumber     int             ���ئ��  �  į
 *  category                ITEM_CATEGORY   ʧ��  ة����  ����
 * ߯Ի��
 *  ޥ����  �  į�۾޷¡����ݷ���-1
 ------------------------------------------------------------*/
int CHAR_getNewImagenumberFromEquip(int index, int basebaseimagenumber, ITEM_CATEGORY category )
{
	if(index == -1){
    int i;
    /*  ITEM_CATEGORY ��������  */
    if( category < 0 || category > arraysizeof(CHAR_eqimagetbl[0]) )
        return -1;
    for( i=0 ; i<arraysizeof(CHAR_eqimagetbl) ; i++ ){
        if( CHAR_eqimagetbl[i][0] == basebaseimagenumber )
            return CHAR_eqimagetbl[i][category];
    }

}else{
	if(CHAR_getItemIndex(index,CHAR_EQBELT) == -1){
	    int i;
	    /*  ITEM_CATEGORY ��������  */
	    if( category < 0 || category > arraysizeof(CHAR_eqimagetbl[0]) )
	        return -1;
	    for( i=0 ; i<arraysizeof(CHAR_eqimagetbl) ; i++ ){
	        if( CHAR_eqimagetbl[i][0] == basebaseimagenumber )
	            return CHAR_eqimagetbl[i][category];
	    }
	}else{
		return CHAR_getInt(index,CHAR_BASEIMAGENUMBER);
	}
}
    return -1;
}




/*------------------------------------------------------------
 * hash ��ë�ƻ��£�
 * ¦��
 * ߯Ի��
 *  ئ��
 ------------------------------------------------------------*/
/*
void CHAR_initSeekGraphicNumberFromString( void )
{
    int     i;
    char buf[257];
    FILE *fp;
    char *name;
    char *ip;
    fp = fopen("./ls2data.dat", "r");
    if(!fp) {
	    printf("Cannot load graphics, please check ls2data.dat\n");
	    exit(0);
    }
    memset(buf, 0, 257);
    fgets(buf, 256, fp);
    cconvertStringNumber = atoi(buf);
    convertStringNumber = malloc(12 * cconvertStringNumber);
    for( i = 0 ; i < cconvertStringNumber ; i ++ ) {
    	memset(buf, 0, 257);
    	fgets(buf, 256, fp);
      name = buf;
      for(ip = buf; *ip != ' '; ip++);
      *ip = 0;
      ip++;
    	convertStringNumber[i].hash = 0;
    	convertStringNumber[i].name = (char *)malloc(strlen(name) + 1);
    	memset(convertStringNumber[i].name, 0, strlen(name) + 1);
    	memcpy(convertStringNumber[i].name, name, strlen(name));
    	convertStringNumber[i].graphicnumber = atoi(ip);
    	printf("Graphic %d loaded, name: [%s], graphicnumber: [%d]\n", i+1, convertStringNumber[i].name, convertStringNumber[i].graphicnumber);
    }
    fclose(fp);
//    for( i = 0 ; i < arraysizeof( convertStringNumber ) ; i ++ )
    for( i = 0 ; i < cconvertStringNumber ; i ++ )
        convertStringNumber[i].hash = hashpjw(convertStringNumber[i].name);
}
*/
/*------------------------------------------------------------
 *   ٯ  ����  �  įë  �£�
 * ¦��
 *  string      char*         ٯ
 * ߯Ի��
 *  �޷¡� -1
 *  ��ľ��½�����°�ū����  į
 ------------------------------------------------------------*/
int CHAR_seekGraphicNumberFromString( char* string )
{
    int     hashvalue;
    int     i;
    int     ret;

    /* ������������ٯ�������չ�������߯�ʣ� */
    if( strtolchecknum( string, &ret,10,INT ) )return ret;

    hashvalue = hashpjw( string );
    for( i = 0 ; i < arraysizeof( convertStringNumber ); i ++ )
        if( convertStringNumber[i].hash == hashvalue )
            if( strcasecmp( convertStringNumber[i].name , string ) == 0 )
                return convertStringNumber[i].graphicnumber;
    return -1;
}




/*====================  ������====================*/
/*  ������ë��  ����    */
typedef struct tagCHAR_invinsibleArea
{
    CHAR_AREAKIND   kind;
    int     floor;
    RECT    area;
    int     work[10];
}CHAR_invincibleArea;
CHAR_invincibleArea*    CHAR_invarea;
int                     CHAR_invareanum;

/*------------------------------------------------------------
 *   �������������ë���£�
 * ¦��
 *  filename        char*       ɬ�ð����̻�
 * ߯Ի��
 *  ��      TRUE(1)
 *  ��      FALSE(0)
 *------------------------------------------------------------*/
BOOL CHAR_initInvinciblePlace( char* filename )
{
    FILE*   f;
    char    line[256];
    int     linenum=0;
    int     invreadlen=0;
#ifdef _CRYPTO_DATA		
	char realopfile[256];
	BOOL crypto = FALSE;
	sprintf(realopfile, "%s.allblues", filename);
	f = fopen( realopfile, "r");
	if( f != NULL ){
		crypto = TRUE;
	}else
#endif
{
    f = fopen(filename,"r");
}
    if( f == NULL ){
        errorprint;
        return FALSE;
    }

    CHAR_invareanum=0;

    /*  ����  ��ئ�滥�ϵ�ؤ�¾�������Ʃ����    */
    while( fgets( line, sizeof( line ), f ) ){
#ifdef _CRYPTO_DATA		
				if(crypto==TRUE){
					DecryptKey(line);
				}
#endif
        linenum ++;
        if( line[0] == '#' )continue;        /* comment */
        if( line[0] == '\n' )continue;       /* none    */
        chomp( line );

        CHAR_invareanum++;
    }

    if( fseek( f, 0, SEEK_SET ) == -1 ){
        fprint( "Seek Error\n" );
        fclose(f);
        return FALSE;
    }

    CHAR_invarea = allocateMemory( sizeof(CHAR_invincibleArea)
                                   * CHAR_invareanum );
    if( CHAR_invarea == NULL ){
        fprint( "Can't allocate Memory %d\n" ,
                sizeof(CHAR_invincibleArea)*CHAR_invareanum);
        fclose( f );
        return FALSE;
    }
    /* ����� */
{
    int     i;
    for( i = 0; i < CHAR_invareanum; i ++ ) {
        int j;
        CHAR_invarea[i].kind = CHAR_AREAKIND_NONE;
        CHAR_invarea[i].floor = 0;
        CHAR_invarea[i].area.x = 0;
        CHAR_invarea[i].area.y = 0;
        CHAR_invarea[i].area.width = 0;
        CHAR_invarea[i].area.height = 0;
        for( j = 0; j < sizeof(CHAR_invarea[i].work); j ++ ) {
            CHAR_invarea[i].work[j] = -1;
        }
    }
}
    linenum = 0;
    while( fgets( line, sizeof( line ), f ) ){
#ifdef _CRYPTO_DATA		
				if(crypto==TRUE){
					DecryptKey(line);
				}
#endif
        linenum ++;
        if( line[0] == '#' )continue;        /* comment */
        if( line[0] == '\n' )continue;       /* none    */
        chomp( line );
        replaceString( line, '\t' , ' ' );
        deleteSequentChar( line, " " );
        if( line[0] == ' ' )continue;
        {
            char    token[256];
            int     ret;
            int     lx,ly,rx,ry;
            CHAR_AREAKIND   kind=-1;

            ret = getStringFromIndexWithDelim( line, " ", 1, token, sizeof(token));
            if( ret==FALSE ){
                fprint("Syntax Error file:%s line:%d\n",filename,linenum);
                continue;
            }else{
                static struct CHAR_charareakindtbl
                {
                    char*   idstring;
                    CHAR_AREAKIND   kind;
                }CHAR_areakindtbl[]={
                    {"INV",  CHAR_INVAREA,},
                    {"ITM",  CHAR_CANNOTDROPAREA,},
                    {"MAG",  CHAR_CANNOTMAGICAREA,},
                };
                int i;
                for( i=0 ; i<arraysizeof(CHAR_areakindtbl) ; i++ )
                    if( strcmp( token,CHAR_areakindtbl[i].idstring )==0 ){
                        kind = CHAR_areakindtbl[i].kind;
                        break;
                    }
                if( kind == -1 ){
                    print( "���Ϸ�Χ����: %d...",linenum );
                    continue;
                }
            }

            CHAR_invarea[invreadlen].kind = kind;
            ret = getStringFromIndexWithDelim( line, " ", 2, token, sizeof(token));
            if( ret==FALSE ){
                fprint("�ļ��﷨����:%s ��%d��\n",filename,linenum);
                continue;
            }else
                CHAR_invarea[invreadlen].floor = atoi( token );

            ret = getStringFromIndexWithDelim( line, " ", 3, token, sizeof(token));
            if( ret ==FALSE){
                fprint("�ļ��﷨����:%s ��%d��\n",filename,linenum);
                continue;
            }else
                getFourIntsFromString( token,&lx,&ly,&rx,&ry );

            if( lx > rx )swap(lx,rx);
            if( ly > ry )swap(ly,ry);

            CHAR_invarea[invreadlen].area.x = lx;
            CHAR_invarea[invreadlen].area.y = ly;
            CHAR_invarea[invreadlen].area.width = rx-lx;
            CHAR_invarea[invreadlen].area.height= ry-ly;
            invreadlen ++;
        }
    }
    fclose(f);
    CHAR_invareanum = invreadlen;
    print( "��Ч����ս����Ч���� %d...", CHAR_invareanum );

#ifdef DEBUG
    {
        int i;
        for( i=0; i <CHAR_invareanum ; i++ )
            print( "kind(%d) %d(%d,%d)-(%d,%d)\n",
                   CHAR_invarea[i].kind,
                   CHAR_invarea[i].floor,
                   CHAR_invarea[i].area.x,CHAR_invarea[i].area.y,
                   CHAR_invarea[i].area.width,
                   CHAR_invarea[i].area.height );
    }
#endif /*DEBUG*/

    return TRUE;
}

BOOL CHAR_isInvincibleArea( int floor , int x, int y )
{
    int     i;
    for( i=0 ; i<CHAR_invareanum ; i++ )
        if( CHAR_invarea[i].kind == CHAR_INVAREA
            && CHAR_invarea[i].floor == floor
            && CoordinateInRect( &CHAR_invarea[i].area,x,y ) )
            return TRUE;
    return FALSE;
}

BOOL CHAR_isCannotMagicArea( int floor , int x, int y, int magicnum )
{
    int     i;
    for( i=0 ; i<CHAR_invareanum ; i++ ) {
        if( CHAR_invarea[i].kind == CHAR_CANNOTMAGICAREA
            && CHAR_invarea[i].floor == floor
            && CoordinateInRect( &CHAR_invarea[i].area,x,y ) )
        {
            int     j;
            for( j = 0;
                j < sizeof(CHAR_invarea[i].work) &&
                CHAR_invarea[i].work[j] != -1;
                j ++ )
            {
                if( CHAR_invarea[i].work[j] == magicnum ) {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

typedef struct tagCHAR_appearposition
{
    int     floor;
    int     x,y;
}CHAR_appearposition;
CHAR_appearposition*    CHAR_appear;
int                     CHAR_appearnum;

BOOL CHAR_initAppearPosition( char* filename )
{
    FILE*   f;
    char    line[256];
    int     linenum=0;
    int     appearreadlen=0;
#ifdef _CRYPTO_DATA		
	char realopfile[256];
	BOOL crypto = FALSE;
	sprintf(realopfile, "%s.allblues", filename);
	f = fopen( realopfile, "r");
	if( f != NULL ){
		crypto = TRUE;
	}else
#endif
{
    f = fopen(filename,"r");
}
    if( f == NULL ){
        errorprint;
        return FALSE;
    }

    CHAR_appearnum=0;
    while( fgets( line, sizeof( line ), f ) ){
#ifdef _CRYPTO_DATA		
				if(crypto==TRUE){
					DecryptKey(line);
				}
#endif
        linenum ++;
        if( line[0] == '#' )continue;        /* comment */
        if( line[0] == '\n' )continue;       /* none    */
        chomp( line );

        CHAR_appearnum++;
    }

    if( fseek( f, 0, SEEK_SET ) == -1 ){
        fprint( "Seek Error\n" );
        fclose(f);
        return FALSE;
    }

    CHAR_appear = allocateMemory( sizeof(CHAR_appearposition)
                                   * CHAR_appearnum );
    if( CHAR_appear == NULL ){
        fprint( "Can't allocate Memory %d\n" ,
                sizeof(CHAR_appearposition)*CHAR_appearnum);
        fclose( f );
        return FALSE;
    }
{
    int     i;
    for( i = 0; i < CHAR_appearnum; i ++ ) {
        CHAR_appear[i].floor = 0;
        CHAR_appear[i].x = 0;
        CHAR_appear[i].y = 0;

    }
}
    linenum = 0;
    while( fgets( line, sizeof( line ), f ) ){
#ifdef _CRYPTO_DATA		
				if(crypto==TRUE){
					DecryptKey(line);
				}
#endif
        linenum ++;
        if( line[0] == '#' )continue;        /* comment */
        if( line[0] == '\n' )continue;       /* none    */
        chomp( line );
        replaceString( line, '\t' , ' ' );
        deleteSequentChar( line, " " );
        if( line[0] == ' ' )continue;

        {
            char    token[256];
            int     ret;

            /*  ���繴���������ͼ�ëέ��    */
            ret = getStringFromIndexWithDelim( line," ",1,token,
                                               sizeof(token));
            if( ret==FALSE ){
                fprint("Syntax Error file:%s line:%d\n",filename,linenum);
                continue;
            }
            CHAR_appear[appearreadlen].floor = atoi(token);
            ret = getStringFromIndexWithDelim( line," ",2,token,
                                               sizeof(token));
            if( ret==FALSE ){
                fprint("Syntax Error file:%s line:%d\n",filename,linenum);
                continue;
            }

            CHAR_appear[appearreadlen].x = atoi( token );

            ret = getStringFromIndexWithDelim( line," ",3,token,
                                               sizeof(token));
            if( ret ==FALSE){
                fprint("Syntax Error file:%s line:%d\n",filename,linenum);
                continue;
            }

            CHAR_appear[appearreadlen].y = atoi( token );

            appearreadlen ++;
        }
    }
    fclose(f);

    CHAR_appearnum = appearreadlen;

    print( "��Ч��ʾλ�÷�Χ���� %d..", CHAR_appearnum );
#ifdef DEBUG

    {
        int i;
        for( i=0; i <CHAR_appearnum ; i++ )
            print( "Appear fl[%d] x[%d] y[%d]\n",
                   CHAR_appear[i].floor,
                   CHAR_appear[i].x,
                   CHAR_appear[i].y );
    }
#endif
    return TRUE;
}

BOOL CHAR_isAppearPosition( int floor, int *x, int *y)
{
    int i;
    for( i = 0; i < CHAR_appearnum; i ++ ) {
        if( CHAR_appear[i].floor == floor ) {
            *x = CHAR_appear[i].x;
            *y = CHAR_appear[i].y;
            return( TRUE);
        }
    }
    return( FALSE);
}

BOOL CHAR_isCannotDropArea( int floor , int x, int y )
{
    int     i;
    for( i=0 ; i<CHAR_invareanum ; i++ )
        if( CHAR_invarea[i].kind == CHAR_CANNOTDROPAREA
            && CHAR_invarea[i].floor == floor
            && CoordinateInRect( &CHAR_invarea[i].area,x,y ) )
            return TRUE;
    return FALSE;
}

typedef struct tagEldersPosition
{
    int     floor;
    int     x;
    int     y;
}EldersPosition;

#define MAXELDERS 128
static EldersPosition elders[MAXELDERS]=
{
  { 1006,15,22 },
  { 2006,20,16 },
	{ 3006,21,16 },
  { 4006,14,20 },
	{ 7770, 9,10 },

};

BOOL CHAR_getInitElderPosition( Char* ch,int hometown)
{
    int   index = 0;
	  int		point;
    if( hometown < 0 || hometown > 3 ) {
    	return FALSE;
    }
    
#ifdef _UNIFIDE_MALINASI
		index=1;
#else
		index=hometown;
#endif
    if( index >= arraysizeof( elders ) ){
 		return FALSE;
    }

    ch->data[CHAR_FLOOR] = elders[index].floor;
    ch->data[CHAR_X] = elders[index].x;
    ch->data[CHAR_Y] = elders[index].y;
	ch->data[CHAR_LASTTALKELDER] = index;
	point = ch->data[CHAR_SAVEPOINT];

	point = point |  (1<<hometown) ;

	ch->data[CHAR_SAVEPOINT] = point;

	if( MAP_checkCoordinates( ch->data[CHAR_FLOOR], ch->data[CHAR_X],
									ch->data[CHAR_Y] ) == FALSE ){
		return FALSE;
	}

    return TRUE;
}

/*------------------------------------------------------------
 * �޻�ĸ��������ë  ��
 * ¦��
 *  elderindex      int     �޻�ĸ���̼������͵�
 *  fl              int*    �׷�ʧë������������
 *  x               int*    xë������������
 *  y               int*    yë������������
 * ¦��
 *  ��    TRUE(1)
 *  ��    FALSE(0)
 ------------------------------------------------------------*/
BOOL CHAR_getElderPosition( int elderindex, int* fl, int* x, int* y )
{
	    if( elderindex<0 || arraysizeof(elders)<=elderindex )return FALSE;

    *fl = elders[elderindex].floor;
    *x  = elders[elderindex].x;
    *y  = elders[elderindex].y;
    return TRUE;
}
/*------------------------------------------------------------
 * Ӯ  NPC�����м��������������Ӯ    ��̤���꼰�֣�
 * �����׻����̼����������̵�
 * ¦��
 *  index   int ������ƥ ��  ������FALSE ë������
 *  fl      int ��  ����
 *  x       int
 *  y       int
 * NPC����������������ľ�¼��֣�
 ------------------------------------------------------------*/
BOOL CHAR_ElderSetPosition( int elderindex ,int fl,int x ,int y)
{
    if( elderindex < ELDERINDEXSTART ||
        elderindex >= MAXELDERS ){
        return FALSE;
    }

    elders[elderindex].floor = fl;
    elders[elderindex].x = x;
    elders[elderindex].y = y;
    return TRUE;
}


void CHAR_setInitValues( Char* ch )
{

#ifndef _PROFESSION_SKILL			// WON ADD ����ְҵ����

    static int CHAR_titleindextbl[] ={ 0,1,2,3,4,5,6 };
    int     elderindex;
    int     i;
    BOOL    magician=FALSE;
    BOOL    priest=FALSE;

    elderindex = ch->data[CHAR_LASTTALKELDER];
    if( 0 <= elderindex
        && elderindex < arraysizeof( CHAR_titleindextbl ) )
        ch->indexOfHaveTitle[0] = CHAR_titleindextbl[elderindex];


    for( i=0 ; i<CHAR_SKILLMAXHAVE ; i++ ){
        static struct addIteTbl
        {
            int itemid[2];
        }additemtbl[SKILL_NUM] = {
            {   {  17, -1}   },
            {   {  -1, -1}   },
            {   {  -1, -1}   },
            {   {  17, -1}   },
            {   {  17, -1}   },
            {   {  17, -1}   },
            {   {  17, -1}   },
            {   {  17, -1}   },
            {   {  17, -1}   },
            {   {  17, -1}   },
            {   {  -1, -1}   }, 
            {   {  17, -1}   },
            {   { 601, -1}   },
            {   { 101, -1}   },
            {   { 201, -1}   },
            {   { 300, -1}   },
            {   { 400, -1}   },
            {   { 501, -1}   },
            {   {  17, -1}   },
        };

        if( ch->haveSkill[i].use ){
            int     j;
            int     skindex=ch->haveSkill[i].skill.data[SKILL_IDENTITY];

            for( j=0 ; j<arraysizeof( additemtbl[0].itemid ); j++ )
                CHAR_addItemToChar( ch, additemtbl[skindex].itemid[j] );

            if( skindex == SKILL_MERCHANT )
                ch->data[CHAR_GOLD] = 20;

            if( skindex == SKILL_MAGICIAN )
                magician = TRUE;
            else if( skindex == SKILL_PRIEST )
                priest = TRUE;

        }
    }

    if( magician || priest ) CHAR_addItemToChar( ch, 802 );

#endif
}


#if 1
static int EnemyExpTbl[]={
0,			//  0
2,			//  1
4,			//  2
6,			//  3
8,			//  4
10,			//  5
12,			//  6
14,			//  7
16,			//  8
18,			//  9
20,			// 10
26,			// 11
32,			// 12
38,			// 13
44,			// 14
50,			// 15
60,			// 16
70,			// 17
80,			// 18
90,			// 19
100,		// 20
115,		// 21
130,		// 22
145,		// 23
160,		// 24
175,		// 25
190,		// 26
205,		// 27
220,		// 28
235,		// 29
250,		// 30
270,		// 31
290,		// 32
310,		// 33
330,		// 34
350,		// 35
375,		// 36
400,		// 37
425,		// 38
450,		// 39
475,		// 40
500,		// 41
525,		// 42
550,		// 43
575,		// 44
600,		// 45
625,		// 46
650,		// 47
675,		// 48
700,		// 49
725,		// 50
750,		// 51
775,		// 52
800,		// 53
825,		// 54
850,		// 55
875,		// 56
900,		// 57
925,		// 58
950,		// 59
980,		// 60
1010,		// 61
1040,		// 62
1070,		// 63
1100,		// 64
1130,		// 65
1160,		// 66
1190,		// 67
1220,		// 68
1250,		// 69
1280,		// 70
1310,		// 71
1345,		// 72
1380,		// 73
1415,		// 74
1450,		// 75
1485,		// 76
1520,		// 77
1555,		// 78
1590,		// 79
1630,		// 80
1670,		// 81
1710,		// 82
1750,		// 83
1790,		// 84
1830,		// 85
1870,		// 86
1910,		// 87
1950,		// 88
1990		// 89
};           // 90
















// ���ö�ئ��������ң
int GetEnemyExp( int level ){
/*
	int exp = 0;
	if( level < 5 ){
		exp = 2 * level;
	}else
	if( level < 10 ){
		exp = 10 * level;
	}else
	if( level < 20 ){
		exp = 15 * level;
	}else
	if( level < 30 ){
		exp = 30 * level;
	}else
	if( level < 40 ){
		exp = 50 * level;
	}else
	if( level < 50 ){
		exp =60 * level;
	}else
	if( level < 60 ){
		exp =60 * level;
	}else{
		exp =70 * level;
	}
*/
	if( level >= arraysizeof( EnemyExpTbl ) )level = arraysizeof( EnemyExpTbl )-1;
	return EnemyExpTbl[level];
}
#endif

#ifdef _NEWOPEN_MAXEXP
/*static int NeedLevelUpTbls[] = {
			0, 0,
	        2,        6,       18,       37,       67,      110,      170,
	      246,      344,      464,      610,      782,      986,     1221,
	     1491,     1798,     2146,     2534,     2968,     3448,     3978,
		 4558,     5194,     5885,     6635,     7446,     8322,     9262,
	    10272,    11352,    12506,    13734,    15042,    16429,    17899,
		19454,    21098,    22830,    24656,    26576,    28594,    30710,
		32930,    35253,    37683,    40222,    42874,    45638,    48520,
		51520,    54642,    57886,    61258,    64757,    68387,    72150,
		76050,    80086,    84264,   106110,   113412,   121149,   129352,
	   138044,   147256,   157019,   167366,   178334,   189958,   202282,
	   215348,   229205,   243901,   259495,   276041,   293606,   312258,
	   332071,   353126,   375511,   399318,   424655,   451631,   480370,
	   511007,   543686,   578571,   615838,   655680,   698312,   743971,
	   792917,   845443,   901868,   962554,  1027899,  1098353,  1174420,
	  1256663,  1345723,  1442322,  1547281,  1661531,  1786143,  1922340,
	  2071533,  2235351,  2415689,  2614754,  2835137,  3079892,  3352633,
	  3657676,  4000195,  4386445,  4824041,  5322323,  5892866,  6550125,
	 12326614, 15496114, 20025638, 26821885, 37698249, 56734876, 68097265,
	 68290815, 68487425, 68687119, 68889921, 69095855, 69304945, 69517215,
	 69732689, 69951391, 70173345, 70398575, 70627105, 70858959, 71244161,
	 71342735, 71584705, 71830095, 72078929, 72331231, 72587025, 72846335,
	 73109185, 73615599, 73655601, 73929215, 74206465, 74487375, 74771969,
	 75060271, 75352305, 75648095, 75947665, 76421039, 76563241, 76874295,
	 77189225, 77508055, 77830809, 78157511, 78488185, 78822855, 79161545,
	 79724279, 79856081, 80206975, 80561985, 80921135, 81284449, 81651951,
	 82023665, 82399615, 82779825, 83434319, 83558121, 83951255, 84348745,
	 84750615, 85156889, 85567591, 85982745, 86402375, 86826505, 87575159,
	 87693361, 88131135, 88573505, 89020495, 89472129, 89928431, 90389425,
	 90855135, 91325585, 91800799
};
*/
#endif
static int LevelUpTbl[] = { // ����ֵ �ȼ� �����
0,			//  0 EX=(lv^4)/10
0,			//  1
	        2,        8,       25,       62,      129,
	      240,      409,      656,     1000,     1464,		// 11
	     2073,     2856,     3841,     5062,     6553,		// 16
	     8352,    10497,    13032,    16000,    19448,    23425,		// 22
	    27984,    33177,    39062,    45697,    53144,    61465,		// 28
	    70728,    81000,    92352,   104857,   118592,   133633,		// 34
	   150062,   167961,   187416,   208513,   231344,   256000,		// 40
	   282576,   311169,   341880,   374809,   410062,   447745,		// 46
	   487968,   530841,   576480,   625000,   676520,   731161,		// 52
	   789048,   850305,   915062,   983449,  1055600,  1131649,		// 58
	  1211736,  1296000,
	  1402110,    // 61	EX=(lv^4)/(10-0.125*(lv-60))
	  1515521,   1636671,   1766022,   1904066,   2051322,   2208342,	// 67
	  2375708,   2554041,   2744000,   2946281,   3161630,   3390834,	// 73
	  3634736,   3894230,   4170272,   4463878,   4776136,   5108207,	// 79
	  5461333,   5836843,   6236162,   6660816,   7112448,   7592818,	// 85
	  8103824,   8647511,   9226082,   9841920,  10497600,  11195912,	// 91
	 11939882,  12732800,  13578242,  14480111,  15442664,  16470563,	// 97
	 17568917,  18743336,  20000000,  21345723,  22788045,  24335325,	// 103
	 25996856,  27783000,  29705340,  31776872,  34012224,  36427912,	// 109
	 39042666,  41877804,  44957696,  48310329,  51968004,  55968200,	// 115
	 60354645,  65178685,  70501009,  76393874,  82944000,

	  95270613,	// 121 Nuke 0624 EX=(lv^4)/(2.5-0.25*(lv-120))
	 110766728, 130792366, 157614250, 195312500,						// 125
	 252047376,      // 126 EX=lv^4+60000000*(lv-126)^2 Nuke 0816
	 320144641, 388435456, 456922881, 525610000, 594499921, 663595776,	// 132
	 732900721, 802417936, 872150625, 942102016,1012275361,1082673936,	// 138
	1153301041,1224160000      // 140
};

int CHAR_GetLevel()
{	
	return arraysizeof( LevelUpTbl )-1;
}
int CHAR_GetLevelExp( int charaindex, int level)
{
#ifdef _NEWOPEN_MAXEXP
#ifdef _USER_EXP_CF
	if(level>getMaxLevel()){
		if(level>CHAR_getInt(charaindex, CHAR_LIMITLEVEL))
			return -1;
	}else if(level>getYBLevel()){
		if (CHAR_getInt(charaindex, CHAR_WHICHTYPE)==CHAR_TYPEPET 
#ifdef _PET_FUSION
			|| CHAR_getInt( charaindex, CHAR_FUSIONBEIT ) == 1
#endif
			) {
				
			if(getPettrans() != -1 ){
				if(CHAR_getInt(charaindex,CHAR_TRANSMIGRATION)>=getPettrans())
					return getNeedLevelUpTbls(level);
			}else{
				if(level<=CHAR_getInt(charaindex, CHAR_LIMITLEVEL))
					return getNeedLevelUpTbls(level);
			}
			return -1;
		}
#ifdef _PLAYER_NPC
		else if (CHAR_getInt( charaindex, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYERNPC
			|| CHAR_getInt( charaindex, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYERPETNPC){
				 return -1;
		}
#endif
		
		else{
			if(CHAR_getInt(charaindex,CHAR_TRANSMIGRATION)<getChartrans()){
				return -1;
			}
		}
	}
	return getNeedLevelUpTbls(level);
#endif
/*	if( level >= arraysizeof( NeedLevelUpTbls ) ){
		return -1;
	}
	return NeedLevelUpTbls[level];*/
#else
	if( level >= arraysizeof( LevelUpTbl ) ){
		return -1;
	}
	return LevelUpTbl[level];
#endif
}

int CHAR_GetOldLevelExp( int level)
{
	if( level >= arraysizeof( LevelUpTbl ) ){
		return -1;
	}
	return LevelUpTbl[level];
}

int CHAR_LevelUpCheck( int charaindex , int toindex)
{
	int exp, level, nextexp, iRet = FALSE;
	if( CHAR_CHECKINDEX( charaindex  ) == FALSE )return 0;
	exp = CHAR_getInt( charaindex, CHAR_EXP );
	while( 1 ){
		level = CHAR_getInt( charaindex, CHAR_LV );
		nextexp = CHAR_GetLevelExp( charaindex, level+1);
		if( nextexp < 0 )
			break;
		// Arminius 7.30 pet limit lv
		if (CHAR_getInt(charaindex, CHAR_WHICHTYPE)==CHAR_TYPEPET
#ifdef _PLAYER_NPC
			|| CHAR_getInt( charaindex, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYERNPC
			|| CHAR_getInt( charaindex, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYERPETNPC
#endif
			) {
			if (level >= CHAR_getInt(charaindex, CHAR_LIMITLEVEL) && 
				CHAR_getInt(charaindex, CHAR_LIMITLEVEL) > 0 )	{
#ifdef _NEWOPEN_MAXEXP
				CHAR_setInt( charaindex, CHAR_EXP , 0);
#else
#ifdef	_PET_LIMITLEVEL
				if( CHAR_getInt( charaindex, CHAR_PETID) == 718 
#ifdef _PET_2LIMITLEVEL
					|| CHAR_getInt( charaindex, CHAR_PETID) == 401 
#endif
					)	{
					nextexp = CHAR_GetLevelExp( charaindex, CHAR_getInt(charaindex, CHAR_LIMITLEVEL));
					CHAR_setInt( charaindex, CHAR_EXP , nextexp );
				}
#endif
#endif
				break;
			}
		}
		if( exp >= nextexp ){
			iRet ++;
			if( CHAR_getInt( charaindex, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER ){
				CHAR_setInt( charaindex, CHAR_DUELPOINT,
				CHAR_getInt( charaindex, CHAR_DUELPOINT) + (level+1)*10 );
			}
#ifdef	_PET_LIMITLEVEL
			else {
				CHAR_CheckPetDoLimitlevel( charaindex, toindex, level );
			}
#endif
			
#ifdef _NEWOPEN_MAXEXP
			if( level >= CHAR_MAXUPLEVEL ){
			}else{
				if( CHAR_HandleExp( charaindex) != -1 ){
					exp = CHAR_getInt( charaindex, CHAR_EXP );
					CHAR_setInt( charaindex, CHAR_LV, level+1 );
				}
			}
#else
			CHAR_setInt( charaindex, CHAR_LV, level+1 );
#endif
		}else{
			break;
		}
	}
	return iRet;
}

#ifdef	_PET_LIMITLEVEL
void CHAR_CheckPetDoLimitlevel( int petindex , int toindex, int level)	
{
	int LevelUpPoint,vital,str,tgh,dex;
	int j,k;
	char token[56];
#define RAND(x,y)   ((x-1)+1+ (int)( (double)(y-(x-1))*rand()/(RAND_MAX+1.0)) )

	if( CHAR_getInt( petindex, CHAR_PETID) == 718 
#ifdef	_PET_2LIMITLEVEL
		|| CHAR_getInt( petindex, CHAR_PETID) == 401
#endif
		)	{
		LevelUpPoint = CHAR_getInt( petindex, CHAR_ALLOCPOINT );
		vital = (int )(( LevelUpPoint >> 24 ) & 0xFF);
		str = (int)(( LevelUpPoint >> 16 ) & 0xFF);
		tgh = (int)(( LevelUpPoint >> 8 ) & 0xFF);
		dex = (int)(( LevelUpPoint >> 0 ) & 0xFF);

		if( strcmp( CHAR_getChar( petindex, CHAR_OWNERCDKEY), CHAR_getChar( toindex, CHAR_CDKEY) ) ||
			strcmp( CHAR_getChar( petindex, CHAR_OWNERCHARANAME), CHAR_getChar( toindex, CHAR_NAME) )){
			sprintf(token,"�Ҳ���ʶ��ѽ������һ��������ˣ�");
			//andy 1/11 reEdit
			vital -= RAND(2,10);  if (vital<0) vital=0;
			str -= RAND(2,10);    if (str<0) str=0;
			tgh -= RAND(2,10);    if (tgh<0) tgh=0;
			dex -= RAND(2,10);    if (dex<0) dex=0;
			LevelUpPoint = (vital<<24)+(str<<16)+(tgh<<8)+(dex<<0);

			CHAR_setInt( petindex, CHAR_ALLOCPOINT , LevelUpPoint);
			CHAR_talkToCli( toindex, petindex, token, CHAR_COLORWHITE);

			LogPetPointChange(
					CHAR_getChar( toindex, CHAR_NAME ),
					CHAR_getChar( toindex, CHAR_CDKEY ),
					CHAR_getChar( petindex, CHAR_NAME),
					petindex, 2,
					CHAR_getInt( petindex, CHAR_LV),
					"No_master(���ٲ�������)",
					CHAR_getInt( toindex,CHAR_FLOOR),
					CHAR_getInt( toindex,CHAR_X ),
					CHAR_getInt( toindex,CHAR_Y )
					);
		}

		if( ( level % 20 ) == 0 )	{
			for( j=0; j < 3; j++)	{
				k = RAND( 0, 3);
				if( k == 0 )
					vital--;
				if( k == 1 )
					str--;
				if( k == 2 )
					tgh--;
				if( k == 3 )
					dex--;
			}
			if( vital < 0 ) vital =0;
			if( str < 0 ) str =0;
			if( tgh < 0 ) tgh =0;
			if( dex < 0 ) dex =0;
			LevelUpPoint = (vital<<24)+(str<<16)+(tgh<<8)+(dex<<0);
			CHAR_setInt( petindex, CHAR_ALLOCPOINT , LevelUpPoint);
			//print("\n << ( level % 20 ) == 0 >> ");
			LogPetPointChange(
					CHAR_getChar( toindex, CHAR_NAME ),
					CHAR_getChar( toindex, CHAR_CDKEY ),
					CHAR_getChar( petindex, CHAR_NAME),
					petindex,3,
					CHAR_getInt( petindex, CHAR_LV),
					"level%20",
					CHAR_getInt( toindex,CHAR_FLOOR),
					CHAR_getInt( toindex,CHAR_X ),
					CHAR_getInt( toindex,CHAR_Y )
					);
		}
	}
}
#endif

int CHAR_PetTakeDrop( int petindex, int floor, int ox, int oy)	//ǿ�ƶ�����������
{
	int x, y, objindex;

	x = rand()%10;
	y = rand()%10;

	//���³���	CHAR_endCharOneArray
	objindex = PET_dropPetAbsolute( petindex,floor,x,y, FALSE );
	if( objindex == -1 )	{
		return -1;
	}
	CHAR_setWorkInt( petindex,CHAR_WORKOBJINDEX,objindex );
	CHAR_setInt( petindex, CHAR_FLOOR, floor);
	CHAR_setInt( petindex, CHAR_X, x);
	CHAR_setInt( petindex, CHAR_Y, y);
	CHAR_sendCToArroundCharacter( objindex);

	return floor;
}

int CHAR_PetTakeLevelUp( int petindex, int lv)//ǿ�Ƴ�������
{
	int k;
	for( k = 1; k < lv; k ++ ){
		CHAR_PetLevelUp( petindex );
		CHAR_PetAddVariableAi( petindex, AI_FIX_PETLEVELUP );
		CHAR_setInt( petindex, CHAR_LV, CHAR_getInt( petindex, CHAR_LV) +1 );
	}

	CHAR_complianceParameter( petindex );
	return k;
}

#ifdef _NEW_MANOR_LAW
extern  struct  FM_POINTLIST fmpointlist;
#endif

int CHAR_PetLevelUp( int petindex )
{
	struct _RankRandTbl{
		int min;
		int max;
	}RankRandTbl[] = {
		{ 450, 500 },
		{ 470, 520 },
		{ 490, 540 },
		{ 510, 560 },
		{ 530, 580 },
		{ 550, 600 },
	};
	
	float Param[4] = { 0.0, 0.0, 0.0, 0.0 };
	int LevelUpPoint, petrank;
	float str, vital, dex, tgh, fRand, i;
#ifdef _TEACHER_SYSTEM
	int iGetFame = 0;
#endif	

	if( CHAR_CHECKINDEX( petindex ) == FALSE )	return -1;
	if( CHAR_getInt( petindex, CHAR_WHICHTYPE ) != CHAR_TYPEPET 
#ifdef _PLAYER_NPC
			&& CHAR_getInt( petindex, CHAR_WHICHTYPE ) != CHAR_TYPEPLAYERNPC
			&& CHAR_getInt( petindex, CHAR_WHICHTYPE ) != CHAR_TYPEPLAYERPETNPC
#endif
		){
			return -1;
	}

	LevelUpPoint = CHAR_getInt( petindex, CHAR_ALLOCPOINT );
	
	// ʸ�������¼���
	petrank = CHAR_getInt( petindex, CHAR_PETRANK );
	if( petrank < 0 || petrank > 5 ) petrank = 0;
	
	vital = (float)(( LevelUpPoint >> 24 ) & 0xFF);
	str = (float)(( LevelUpPoint >> 16 ) & 0xFF);
	tgh = (float)(( LevelUpPoint >> 8 ) & 0xFF);
	dex = (float)(( LevelUpPoint >> 0 ) & 0xFF);

	// ��߼Ԩ�    �������ë���Ȼ�����  �������
	for( i = 0; i < 10; i ++ ){
		Param[RAND( 0, 3 )] += 1.0;
	}
	
	// �¼��ͱ巽��  �����¼�ĸة
	fRand = (float)RAND( RankRandTbl[petrank].min, RankRandTbl[petrank].max )
		* 0.01;
	
	// �Ի��Ի���̼�����Ի��ң
	vital = (float)vital * fRand + Param[0] * fRand;
	str = (float)str   * fRand + Param[1] * fRand;
	tgh = (float)tgh   * fRand + Param[2] * fRand;
	dex = (float)dex   * fRand + Param[3] * fRand;
	
	// ��ң
	CHAR_setInt( petindex, CHAR_VITAL,
		CHAR_getInt( petindex, CHAR_VITAL ) + max(0, (int)vital) );
	CHAR_setInt( petindex, CHAR_STR,
		CHAR_getInt( petindex, CHAR_STR ) + max(0, (int)str) );
	CHAR_setInt( petindex, CHAR_TOUGH,
		CHAR_getInt( petindex, CHAR_TOUGH ) + max(0, (int)tgh) );
	CHAR_setInt( petindex, CHAR_DEX,
		CHAR_getInt( petindex, CHAR_DEX ) + max(0, (int)dex) );
		
	// CoolFish: ����������������
	{
		int level = CHAR_getInt(petindex, CHAR_LV);
		int exp1 = 0, exp2 = 0;
		if (level > 30){
			char	tmpbuf[128];
#ifdef _NEW_MANOR_LAW
			char	tmpbuf1[16];
#endif
			int ownerindex = CHAR_getWorkInt(petindex, CHAR_WORKPLAYERINDEX);
#ifdef _FMVER21
#else
			if (CHAR_getInt(ownerindex, CHAR_FMINDEX) < 0)	return	 0;
			if (CHAR_getInt(ownerindex, CHAR_FMLEADERFLAG) <= 0 )    return  0;
			if (strcmp(CHAR_getChar(ownerindex, CHAR_FMNAME), "") == 0)	return	0;
#endif	   	    
			if (CHAR_CHECKINDEX(ownerindex)){
				exp1 = CHAR_GetLevelExp( petindex, level);
				exp2 = CHAR_GetLevelExp( petindex, level - 1);
				 
//#ifdef _PERSONAL_FAME	// Arminius 8.30: �����������
//				feedpoint = (exp1 - exp2) / 20000;	// ����
//#else
//				feedpoint = (exp1 - exp2) / 10000;
//#endif
				if ( exp1 - exp2 > 0){
					int feedpoint;
					// CoolFish: 2001/10/03
					int fd = getfdFromCharaIndex(ownerindex);

					feedpoint=exp2 / 20000;
#ifdef _OFFLINE_SYSTEM
					if(CHAR_getWorkInt( ownerindex, CHAR_WORK_OFFLINE ) > 0){
						feedpoint=feedpoint / 2;
					}
#endif	
#ifdef _PERSONAL_FAME	// Arminius 8.30: �����������
					CHAR_earnFame(ownerindex, feedpoint);
					sprintf(tmpbuf, "%d", feedpoint);
#endif
					
#ifdef _NEW_MANOR_LAW
					sprintf(tmpbuf1, "%d", CHAR_getInt(ownerindex,CHAR_FAME));
#endif
#ifdef _FMVER21
					if (CHAR_getInt(ownerindex, CHAR_FMLEADERFLAG) > 0
						 && CHAR_getInt(ownerindex, CHAR_FMLEADERFLAG) != FMMEMBER_APPLY){
						saacproto_ACFixFMData_send(acfd,
							 CHAR_getChar(ownerindex, CHAR_FMNAME),
							 CHAR_getInt(ownerindex, CHAR_FMINDEX),
							 CHAR_getWorkInt(ownerindex, CHAR_WORKFMINDEXI),
							 FM_FIX_FMFEED, tmpbuf,
#ifndef _NEW_MANOR_LAW
							 "",
#else
							 tmpbuf1,
#endif
							 // CoolFish: 2001/10/03
							 CHAR_getWorkInt(ownerindex, CHAR_WORKFMCHARINDEX),
							 CONNECT_getFdid(fd));
						 // CHAR_getWorkInt(ownerindex, CHAR_WORKFMCHARINDEX), 0);
#ifdef _NEW_MANOR_LAW
						{
							int i;
							for(i=0;i<FAMILY_MAXHOME;i++){
								// �κ�һ��ׯ԰������ս�ų�,����ֵһ�ı����ACҪ������������
								if(fmpointlist.fm_inwar[i]){
									saacproto_ACShowTopFMList_send(acfd, FM_TOP_MOMENTUM);
									break;
								}
							}
						}
#endif
					}
#ifdef _NEW_MANOR_LAW
					// δ���ͨ��ʱֻ���³�Ա����ֵ
					else if(CHAR_getInt(ownerindex, CHAR_FMLEADERFLAG) == FMMEMBER_APPLY){
						sprintf(tmpbuf,"%d",CHAR_getInt(ownerindex,CHAR_FAME));
						saacproto_ACFixFMData_send(acfd,
							CHAR_getChar(ownerindex,CHAR_FMNAME),
							CHAR_getInt(ownerindex,CHAR_FMINDEX),
							CHAR_getWorkInt(ownerindex,CHAR_WORKFMINDEXI),
							FM_FIX_FAME,tmpbuf,"",
							CHAR_getWorkInt(ownerindex,CHAR_WORKFMCHARINDEX),
							CONNECT_getFdid(fd));
					}
#endif
#else
					saacproto_ACFixFMData_send(acfd,
						CHAR_getChar(ownerindex, CHAR_FMNAME),
						CHAR_getInt(ownerindex, CHAR_FMINDEX),
						CHAR_getWorkInt(ownerindex, CHAR_WORKFMINDEXI),
						FM_FIX_FMFEED, tmpbuf, "",
						CHAR_getWorkInt(ownerindex, CHAR_WORKFMCHARINDEX), 0);
#endif
#ifdef _TEACHER_SYSTEM
					iGetFame = feedpoint/20; // ��ʦ���ѧ������������ 5% (1/20)
					// ������õ�������û�д��0
					if(iGetFame > 0){
						// �����û�е�ʦ
						if(strlen(CHAR_getChar(ownerindex,CHAR_TEACHER_ID)) > 0 && strlen(CHAR_getChar(ownerindex,CHAR_TEACHER_NAME)) > 0){
							int iPlayernum = CHAR_getPlayerMaxNum(),i;
							char szMsg[128];
							
							// ��鵼ʦ�ڲ�������
							for(i=0;i<iPlayernum;i++){
								if(CHAR_getCharUse(i) == FALSE) continue;
								if(strcmp(CHAR_getChar(ownerindex,CHAR_TEACHER_ID),CHAR_getChar(i,CHAR_CDKEY)) == 0 &&
									strcmp(CHAR_getChar(ownerindex,CHAR_TEACHER_NAME),CHAR_getChar(i,CHAR_NAME)) == 0){
									float fGetFame = (float)iGetFame/100;
									// ��ʦ������
									CHAR_setWorkInt(i,CHAR_WORK_GET_TEACHER_FAME,CHAR_getWorkInt(i,CHAR_WORK_GET_TEACHER_FAME) + iGetFame);
									sprintf(szMsg,"���ѧ�� %s %.2f ������",CHAR_getChar(ownerindex,CHAR_NAME),fGetFame);
									CHAR_talkToCli(i,-1,szMsg,CHAR_COLORYELLOW);
									break;
								}
							}
						}
					}
#endif
				}
			}
		}
	}
	
#ifdef _LEVELUP_RECOVERY
			CHAR_complianceParameter( petindex );
    CHAR_setInt( petindex , CHAR_HP ,CHAR_getWorkInt( petindex, CHAR_WORKMAXHP ) );
#endif

	return 0;
}
#ifdef _NPC_FUSION
int PETFUSION_FusionPetSub( int charaindex, int Subindex1, int Subindex2, int *work, int *skill)
{
	int i;
	int base[4]={0,0,0,0};
	int petskill[7]={-1,-1,-1,-1,-1,-1,-1};
	if( !CHAR_CHECKINDEX( Subindex1)) return 0;
	if( EVOLUTION_getPetFusionCode( CHAR_getInt( Subindex1, CHAR_PETID) ) < 0 ){
		return 0;
	}

	if( CHAR_getInt( Subindex1, CHAR_FUSIONBEIT) == 1 ||
		CHAR_getInt( Subindex1, CHAR_FUSIONRAISE) > 0 )return 0;//����Ƿ�Ϊ�ںϳ�
	for( i=0; i<4; i++)	{
		work[i] = 0;
	}
	if( PET_getBaseAndSkill( charaindex, Subindex1, base, petskill, 0) == FALSE )
		return 0;
	if( CHAR_getInt( Subindex1, CHAR_LV) < 120 ){//�ȼ�����
		for( i=0; i<4; i++)	{
			base[i] = base[i]*0.7;
		}
	}else	if( CHAR_getInt( Subindex1, CHAR_LV) < 140 ){//�ȼ�����
		for( i=0; i<4; i++)	{
			base[i] = base[i]*0.8;
		}
	}
	for( i=0; i<4; i++)	{
		work[i] = base[i];
	}
	for( i=0; i<7; i++)	{
		skill[i] = petskill[i];
	}
	if( !CHAR_CHECKINDEX( Subindex2)) return 1;
	if( EVOLUTION_getPetFusionCode( CHAR_getInt( Subindex2, CHAR_PETID) ) < 0 ){
		return 0;
	}

	if( CHAR_getInt( Subindex2, CHAR_FUSIONBEIT) == 1 ||
		CHAR_getInt( Subindex2, CHAR_FUSIONRAISE) > 0 )return 0;//����Ƿ�Ϊ�ںϳ�

	if( PET_getBaseAndSkill( charaindex, Subindex2, base, NULL, 0) == FALSE )
		return 0;
	if( CHAR_getInt( Subindex2, CHAR_LV) < 120 ){//�ȼ�����
		for( i=0; i<4; i++)	{
			base[i] = base[i]*0.7;
		}
	}else if( CHAR_getInt( Subindex2, CHAR_LV) < 140 ){//�ȼ�����
		for( i=0; i<4; i++)	{
			base[i] = base[i]*0.8;
		}
	}
	for( i=0; i<4; i++)	{
		work[i] += base[i];
	}
	return 2;
}

BOOL PETFUSION_FusionPetMain( int charaindex, int Mainindex, int *work, int *skill)
{
	int i;
	int base[4]={0,0,0,0};
	int petskill[7]={-1,-1,-1,-1,-1,-1,-1};
	if( !CHAR_CHECKINDEX( Mainindex)) return FALSE;
	if( EVOLUTION_getPetFusionCode( CHAR_getInt( Mainindex, CHAR_PETID) ) < 0 ){
		return 0;
	}
	if( CHAR_getInt( Mainindex, CHAR_FUSIONBEIT) == 1 ||
		CHAR_getInt( Mainindex, CHAR_FUSIONRAISE) > 0 )return FALSE;//����Ƿ�Ϊ�ںϳ�

	if( PET_getBaseAndSkill( charaindex, Mainindex, base, petskill, 0) == FALSE )
		return -1;
	if( CHAR_getInt( Mainindex, CHAR_LV) < 120 ){//�ȼ�����
		for( i=0; i<4; i++)	{
			base[i] = base[i]*0.7;
		}
	}else if( CHAR_getInt( Mainindex, CHAR_LV) < 140 ){//�ȼ�����
		for( i=0; i<4; i++)	{
			base[i] = base[i]*0.8;
		}
	}else
	for( i=0; i<4; i++)	{
		work[i] += base[i]*0.6;
	}
	for( i=0; i<7; i++)	{
		skill[i] = petskill[i];
	}

	return TRUE;
}

BOOL PETFUSION_DelPet( int toindex, int Mainindex, int Subindex1, int Subindex2, int flg)
{
	int i, j, cnt=0;
	int petindex[3];


	petindex[0] = Mainindex;
	petindex[1] = Subindex1;
	petindex[2] = Subindex2;

	for( i=0; i<3; i++)	{
		if( !CHAR_CHECKINDEX( petindex[i] ) ) continue;
		for( j=0; j<CHAR_MAXPETHAVE; j++)	{
			int pindex = CHAR_getCharPet( toindex, j);
			if( !CHAR_CHECKINDEX( pindex) ) continue;
			if( pindex == petindex[i] )
				break;
		}
		if( j >= CHAR_MAXPETHAVE){

		}else	{
			char szPet[256];
			char msgbuf[256];
			CHAR_setCharPet( toindex, j, -1);
			snprintf( szPet, sizeof( szPet ), "K%d", j);
			CHAR_sendStatusString( toindex, szPet );

			snprintf( msgbuf,sizeof( msgbuf), "����%s��", CHAR_getChar( petindex[i], CHAR_NAME));
			CHAR_talkToCli( toindex, -1, msgbuf,  CHAR_COLORYELLOW);
				LogPet(			
					CHAR_getChar( toindex, CHAR_NAME ),
					CHAR_getChar( toindex, CHAR_CDKEY ),
					CHAR_getChar( petindex[i], CHAR_NAME),
					CHAR_getInt( petindex[i], CHAR_LV),
					"TenseiDel",
					CHAR_getInt( toindex,CHAR_FLOOR),
					CHAR_getInt( toindex,CHAR_X ),
					CHAR_getInt( toindex,CHAR_Y ),
					CHAR_getChar( petindex[i], CHAR_UNIQUECODE)   // shan 2001/12/14
				);
		}
		CHAR_endCharOneArray( petindex[i] );
		cnt++;
	}
	if( cnt >= flg )
		return TRUE;
	return FALSE;
}

int PETFUSION_Evolution( int charaindex, int petindex)
{
	char buf[256], buf1[256];
	int newindex=-1;
	CHAR_setInt( petindex, CHAR_FUSIONTIMELIMIT, -1);

	sprintf( buf, "����%s��������", CHAR_getChar( petindex, CHAR_NAME));
	newindex = EVOLUTION_createPetFromEnemyIndex( charaindex, petindex, 0);
	if( !CHAR_CHECKINDEX( newindex) ){
		CHAR_talkToCli( charaindex, -1, "���������������", CHAR_COLORYELLOW);
		return -1;
	}
	sprintf( buf1, "��%s����", CHAR_getChar( newindex, CHAR_NAME));
	strcat( buf, buf1);
	CHAR_talkToCli( charaindex, -1, buf, CHAR_COLORYELLOW);

	LogPetFeed(
		CHAR_getChar( charaindex, CHAR_NAME),
		CHAR_getChar( charaindex, CHAR_CDKEY),
		CHAR_getChar( petindex, CHAR_NAME),
		petindex,
		CHAR_getInt( petindex, CHAR_LV),
		buf, // Key
		CHAR_getInt( charaindex, CHAR_FLOOR),
		CHAR_getInt( charaindex, CHAR_X),
		CHAR_getInt( charaindex, CHAR_Y),
		CHAR_getChar( petindex, CHAR_UNIQUECODE) );

	return newindex;
}
#endif

#ifdef _PET_TRANS
int PETTRANS_getPetBase( int petindex, int *work, int *petrank)
{
	int i, total=-1;
	if( !CHAR_CHECKINDEX( petindex) ){
		total = 0;
		*petrank = 1;
		for( i=0; i<4; i++)	{
			work[i] = 50;
			total += work[i];
		}
	}else{
		int LevelUpPoint = CHAR_getInt( petindex, CHAR_ALLOCPOINT );
		*petrank = CHAR_getInt( petindex, CHAR_PETRANK );
		work[0] = (float)(( LevelUpPoint >> 24 ) & 0xFF);
		work[1] = (float)(( LevelUpPoint >> 16 ) & 0xFF);
		work[2] = (float)(( LevelUpPoint >> 8 ) & 0xFF);
		work[3] = (float)(( LevelUpPoint >> 0 ) & 0xFF);
		total = ( work[0] + work[1] + work[2] + work[3] );
		if( total > 150 )	total = 150;
		if( total < 0 )		total = 0;
	}
	return total;
}

#ifdef _PET_2TRANS
int NPC_PetTransManGetAns( int total1, int total2, int LV, int rank, int tran )
#else
int NPC_PetTransManGetAns( int total1, int total2, int LV, int rank )
#endif
{
	int ans=0 , TransLV = 100;
	float total = 0.00;
	int Fx=1;
#if _ATTESTAION_ID == 1
	if( LV > 130 ) LV = 130;
#else
	if( LV > 140 ) LV = 140;
#endif
	total = ( (float)total1/100 ); // ������� total1=200
	total = total*total*total*total*total; 
	if( total < 1 ) total = 0;
	else total= total*1.3; // ����total���=41.6 ��С=0
	Fx = (int)((5-rank)*1.2)+5; // rank=0~6 ���� Fx���=11(rank=0)��С=4(rank=6)
	ans = (int)total + total2 + ((LV-TransLV)/Fx); //42+150+30/11=194

#ifdef _PET_2TRANS
#ifdef _PET_TRANS_ABILITY
	if( tran == 0 ){
		if( ans > getPetTransAbility1() )	
			ans = getPetTransAbility1();
	}
	else{
		if( ans > getPetTransAbility2() )	
			ans = getPetTransAbility2() ;
	}
#else
	if( tran == 0 ){
		if( ans > 150 )	
			ans = 150;
	}
	else{
		if( ans > 200 )	
			ans = 200;
	}
#endif
#else
	if( ans > 150 )	{
		ans = 150;
	}
#endif
	return ans;
}

int PETTRANS_PetTransManStatus( int toindex, int petindex1, int petindex2)
{
	int petrank=0,i;
	int total1,total2,total,petLV,ans;
	int petID,enemynum,ret=-1;
	int work[4]={0,0,0,0};
	int base[4]={0,0,0,0};

#define RAND(x,y)   ((x-1)+1+ (int)( (double)(y-(x-1))*rand()/(RAND_MAX+1.0)) )
	total1 = PETTRANS_getPetBase( petindex1, work, &petrank);
	total2 = PETTRANS_getPetBase( petindex2, base, &petrank);
	petLV	= CHAR_getInt( petindex2, CHAR_LV);
#ifdef _PET_2TRANS
	ans = NPC_PetTransManGetAns( total1, total2, petLV, petrank, CHAR_getInt( petindex2, CHAR_TRANSMIGRATION ) );
#else
	ans = NPC_PetTransManGetAns( total1, total2, petLV, petrank );
#endif
	total = total1 + (total2*4);
	total2 = 0;
	for( i=0; i<4; i++)	{
		work[i] = work[i]*4;
		total2 += work[i];
	}
	total = total1 + total2;
	for( i=0; i<4; i++)	{
		base[i] = max(1, (ans * (base[i]+work[i])) /total);
	}
	enemynum = ENEMY_getEnemyNum();
	petID = CHAR_getInt( petindex2, CHAR_PETID);
	for( i=0; i<enemynum; i++)	{
		if( ENEMY_getInt(i, ENEMY_TEMPNO) == petID )
			break;
	}
	if( i == enemynum )
		return -1;
	
	ret = GetNewPet( toindex, petindex2, i, base);
	if( ret < 0 )
		return -1;
	return ret;
}
#endif

#ifdef _CHIKULA_STONE
void CHAR_AutoChikulaStone( int charaindex, int Dflg)
{
	int Myhp, i, dnums;
	if( !CHAR_CHECKINDEX( charaindex) ) return;
	if( CHAR_getWorkInt( charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE ) return;
	switch( Dflg ){
	case 1:	//HP
		Myhp = CHAR_getInt( charaindex, CHAR_HP);
		dnums = CHAR_getWorkInt( charaindex, CHAR_WORKCHIKULAHP);
		Myhp += dnums;
		if( Myhp > CHAR_getWorkInt( charaindex, CHAR_WORKMAXHP) ){
			Myhp = CHAR_getWorkInt( charaindex, CHAR_WORKMAXHP);
		}
		CHAR_setInt( charaindex, CHAR_HP, Myhp);
		CHAR_complianceParameter( charaindex );
		CHAR_send_P_StatusString( charaindex, CHAR_P_STRING_HP);
		for( i=0; i<CHAR_MAXPETHAVE; i++)	{
			int petindex = CHAR_getCharPet( charaindex, i);
			if( !CHAR_CHECKINDEX( petindex) ) continue;
			Myhp = CHAR_getInt( petindex, CHAR_HP);
			Myhp += dnums;
			if( Myhp > CHAR_getWorkInt( petindex, CHAR_WORKMAXHP) ){
				Myhp = CHAR_getWorkInt( petindex, CHAR_WORKMAXHP);
			}
			CHAR_setInt( petindex, CHAR_HP, Myhp);
			CHAR_send_K_StatusString( charaindex, i, CHAR_K_STRING_HP|CHAR_K_STRING_AI);
		}
		break;
	case 2://MP
		Myhp = CHAR_getInt( charaindex, CHAR_MP);
		dnums = CHAR_getWorkInt( charaindex, CHAR_WORKCHIKULAMP);
		Myhp += dnums;
		if( Myhp > CHAR_getWorkInt( charaindex, CHAR_WORKMAXMP) ){
			Myhp = CHAR_getWorkInt( charaindex, CHAR_WORKMAXMP);
		}
		CHAR_setInt( charaindex, CHAR_MP, Myhp);
		CHAR_complianceParameter( charaindex );
		CHAR_send_P_StatusString( charaindex, CHAR_P_STRING_MP);
		break;
	}
}
#endif

#ifdef _STATUS_WATERWORD //ˮ����״̬
void CHAR_CheckWaterStatus( int charaindex)
{
	if( !CHAR_CHECKINDEX( charaindex) ) return;
	if( CHAR_getWorkInt( charaindex, CHAR_WORKSTATUSWATER) == -1 ){
		return;
	}
	if( CHAR_getWorkInt( charaindex, CHAR_WORKSTATUSWATER) > 0 ){
		CHAR_setWorkInt( charaindex, CHAR_WORKSTATUSWATER,
		CHAR_getWorkInt( charaindex, CHAR_WORKSTATUSWATER) -1 );

		if( CHAR_getWorkInt( charaindex, CHAR_WORKSTATUSWATER) != 0 &&
			CHAR_getWorkInt( charaindex, CHAR_WORKSTATUSWATER)%10 == 0 ){
			char buf1[256];
			sprintf( buf1, "ˮ�к���ʱ��ʣ��%d�֡�",
				CHAR_getWorkInt( charaindex, CHAR_WORKSTATUSWATER));
			CHAR_talkToCli( charaindex, -1, buf1, CHAR_COLORYELLOW);
		}
	}

	if( CHAR_getWorkInt( charaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE ){
		return;
	}
	if( CHAR_getWorkInt( charaindex, CHAR_WORKMAPFLOORTYPE) == 1 &&
		CHAR_getWorkInt( charaindex, CHAR_WORKSTATUSWATER) <= 0 ){//ˮ����
		char token[256];
		int defhp=0;
		int maxhp = CHAR_getWorkInt( charaindex, CHAR_WORKMAXHP);
		int myhp = CHAR_getInt( charaindex, CHAR_HP);
		defhp = (maxhp*0.033);
		if( defhp <= 0 ) defhp = 1;
		myhp = myhp - defhp;
		sprintf( token, "���޷���������%d HP��", defhp);
		CHAR_talkToCli( charaindex, -1, token, CHAR_COLORYELLOW);

		if( myhp <= 0 ){
			myhp = 1;
			if(CHAR_getInt(charaindex,CHAR_FLOOR) == 151 || CHAR_getInt(charaindex,CHAR_FLOOR) == 160
				|| CHAR_getInt(charaindex,CHAR_FLOOR) == 161 ) CHAR_warpToSpecificPoint(charaindex,702,213,27);
			else CHAR_warpToSpecificPoint( charaindex, 200,102,1021);
		}
		CHAR_setInt( charaindex, CHAR_HP, myhp);
		CHAR_send_P_StatusString( charaindex, CHAR_P_STRING_HP);
	}

}
#endif

int CHAR_findSurplusPetBox( int charaindex )
{
	int i, remnants=0;
	for( i=0; i<CHAR_MAXPETHAVE; i++){
    	int petindex = CHAR_getCharPet( charaindex, i);
		if( !CHAR_CHECKINDEX( petindex) ) remnants++;
	}

	return remnants;

}

#ifdef _FM_METAMO
void CHAR_ReMetamo( int charaindex )
{
	int oldMetamo=CHAR_getInt( charaindex , CHAR_BASEIMAGENUMBER);
	if( oldMetamo>=100700 && oldMetamo<100819){
		int newMetamo;
		int hbMetamo[]={
			 100000,100010,100025,100035,100055,100045,
			 100060,100070,100095,100090,100100,100110,
			 100135,100125,100145,100150,100165,100160,
			 100190,100185,100200,100210,100230,100220,
		};
		newMetamo=hbMetamo[(oldMetamo-100700)/5];
		CHAR_setInt( charaindex , CHAR_BASEIMAGENUMBER , newMetamo );
		CHAR_setInt( charaindex , CHAR_BASEBASEIMAGENUMBER , newMetamo );
		CHAR_sendCToArroundCharacter( CHAR_getWorkInt( charaindex , CHAR_WORKOBJINDEX ));
		CHAR_send_P_StatusString( charaindex , CHAR_P_STRING_BASEBASEIMAGENUMBER);
		CHAR_complianceParameter( charaindex );
	}
}
#endif

#ifdef _NEW_PLAYER_RIDE
void CHAR_PlayerRide( int charaindex )
{
		int i,j;
		int MetamoList[5][13]={
		/*{ С����   ������  �����к�  ���   ��Ƥ��   ���    С����  ��Ƥ��  ñ����  �̷�����  ����Ů   ����    ��}, ����Ϊ˵���� */
			{ 100000, 100025, 100055, 100060, 100095, 100100, 100135, 100145, 100165, 100190, 100200, 100230, 2483},	//��
			{ 100005, 100030, 100050, 100065, 100085, 100115, 100120, 100140, 100170, 100195, 100210, 100225, 2481},	//��
			{ 100010, 100035, 100045, 100070, 100090, 100110, 100125, 100150, 100160, 100185, 100215, 100220, 2484},	//��
			{ 100015, 100020, 100040, 100075, 100080, 100105, 100130, 100155, 100175, 100180, 100205, 100235, 2482},	//��
			{   331 ,   483 ,   330 ,   332 ,   483 ,   332 ,   333 ,   333 ,   331 ,   330 ,   332 ,   483 ,  0 },	//��
		};
		for(i=0;i<4;i++)
				for(j=0;j<12;j++)
				 if(CHAR_getInt( charaindex, CHAR_BASEIMAGENUMBER) == MetamoList[i][j]){
				 	  if(strstr( getPlayerRide(), "�����ͻ�����")){
				 			setNewplayergivepet(3,MetamoList[i][12]);
				 			setNewplayergivepet(4,MetamoList[4][j]);
				 		}else if(strstr( getPlayerRide(), "��������")){
				 			setNewplayergivepet(4,MetamoList[4][j]);
				 		}else if(strstr( getPlayerRide(), "�����ͻ�")){
				 			setNewplayergivepet(4,MetamoList[i][12]);
				 		}
				 		break;
				 	}
}
#endif
