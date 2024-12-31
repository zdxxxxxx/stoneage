﻿/************************/
/*	produce.h			*/
/************************/
#ifndef _PRODUCE_H_
#define _PRODUCE_H_

// ????????????
enum{
	PRODUCE_UP_ACCELE,			// ?﹩?去
	PRODUCE_UP_BRAKE,			// ?﹩?去
	
	PRODUCE_DOWN_ACCELE,		// ??﹩?去
	PRODUCE_DOWN_BRAKE,			// ??﹩?去
	
	PRODUCE_LEFT_ACCELE,		// ??﹩?去
	PRODUCE_LEFT_BRAKE,			// ??﹩?去
	
	PRODUCE_RIGHT_ACCELE,		// ??﹩?去
	PRODUCE_RIGHT_BRAKE,		// ??﹩?去
	
	PRODUCE_LEFT_RIGHT_ACCELE,	// ???﹩?去
	PRODUCE_LEFT_RIGHT_BRAKE,	// ???﹩?去
	
	PRODUCE_UP_DOWM_ACCELE,		// ??﹩?去
	PRODUCE_UP_DOWM_BRAKE,		// ??﹩?去
	
	PRODUCE_4WAY_OUT,			// ???去????
	PRODUCE_4WAY_IN,			// ???去????
	
	PRODUCE_HAGARE_OUT,			// ?????????
	PRODUCE_HAGARE_IN,			// ??????????
	
	PRODUCE_HAGARE_OCHI_OUT,	// ?????????
	PRODUCE_HAGARE_OCHI_IN,		// ??????????
	
	PRODUCE_BRAN_SMALL,			// ???
	PRODUCE_BRAN_BIG,			// ?????

	PRODUCE_BREAK_UP1,			//???????
	PRODUCE_BREAK_UP2,			//??????

	PRODUCE_BREAK_UP3,			//???????
	PRODUCE_BREAK_UP4,			//???????

	PRODUCE_BREAK_UP5,			//????????
	PRODUCE_BREAK_UP6,			//???????

	PRODUCE_BREAK_UP7,			//????????
	PRODUCE_BREAK_UP8,			//??????

	PRODUCE_CENTER_PRESSIN,		// ????????
	PRODUCE_CENTER_PRESSOUT,	// ?????????

	PRODUCE_END,				// ?

};

// ???????
extern int ProduceInitFlag;

// ???????????? **************************************************/
void CopyBackBuffer( void );

// ?丙? ********************************************************************/
BOOL DrawProduce( int no );

// ????? ********************************************************************/
void TitleProduce( void );

#endif
