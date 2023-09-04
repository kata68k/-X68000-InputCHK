#ifndef	INPUTCHK_C
#define	INPUTCHK_C

#include <iocslib.h>
#include <stdio.h>
#include <stdlib.h>
#include <doslib.h>
#include <io.h>
#include <math.h>
#include <interrupt.h>

#include <usr_macro.h>
#include <apicglib.h>

#include "InputCHK.h"

#include "BIOS_CRTC.h"
#include "BIOS_DMAC.h"
#include "BIOS_MFP.h"
#include "BIOS_PCG.h"
#include "BIOS_MPU.h"
#include "IF_Draw.h"
#include "IF_FileManager.h"
#include "IF_Graphic.h"
#include "IF_Input.h"
#include "IF_MACS.h"
#include "IF_Math.h"
#include "IF_Memory.h"
#include "IF_Mouse.h"
#include "IF_Music.h"
#include "IF_PCG.h"
#include "IF_Text.h"
#include "IF_Task.h"

#define T_BLACK1	(0)
#define T_BLACK2	(1)
#define T_RED		(2)
#define T_WHITE		(3)
#define G_BLACK		(0x00)
#define G_RED		(0x0A)
#define G_GREEN		(0x0B)

/* グローバル変数 */
uint32_t	g_unTime_cal = 0u;
uint32_t	g_unTime_cal_PH = 0u;
int32_t		g_nSuperchk = 0;
int32_t		g_nCrtmod = 0;
int32_t		g_nMaxMemSize;
int16_t		g_CpuTime = 0;
uint8_t		g_mode;
uint8_t		g_mode_rev;
uint8_t		g_Vwait = 1;
uint8_t		g_bFlip = FALSE;
uint8_t		g_bFlip_old = FALSE;
uint8_t		g_bExit = TRUE;

int8_t		sKeyName[128][8] 	= {"未定義",	"ESC",	"1",	"2",	"3",	"4",	"5",	"6",	"7",	"8",	"9",	"0",	"-",	"^",	"\\",	"BS",	"TAB",	"Q",	"W",	"E",	"R",	"T",	"Y",	"U",	"I",	"O",	"P",	"@",	"[",	"CR",	"A",	"S",	"D",	"F",	"G",	"H",	"J",	"K",	"L",	";",	":",	"]",	"Z",	"X",	"C",	"V",	"B",	"N",	"M",	",",	".",	"/",	"_",	"SPC",	"HOME",	"DEL",	"ROLLUP",	"ROLLDN",	"UNDO",	"←",	"↑",	"→",	"↓",	"CLR",	"/",	"*",	"-",	"7",	"8",	"9",	"+",	"4",	"5",	"6",	"=",	"1",	"2",	"3",	"ENTER",	"0",	",",	".",	"記号",	"登録",	"HELP",	"XF1",	"XF2",	"XF3",	"XF4",	"XF5",	"かな",	"ローマ",	"コード",	"CAPS",	"INS",	"ひら",	"全角",	"BREAK",	"COPY",	"F1",	"F2",	"F3",	"F4",	"F5",	"F6",	"F7",	"F8",	"F9",	"F10",	"未定義",	"未定義",	"未定義",	"SHIFT",	"CTRL",	"OPT.1",	"OPT.2",	"未定義",	"未定義",	"未定義",	"未定義",	"未定義",	"未定義",	"未定義",	"未定義",	"未定義",	"未定義",	"未定義",	"未定義"};
int16_t		nKeyPosX[128] 		= {0,	0,	24,	48,	72,	96,	120,	144,	168,	192,	216,	240,	264,	288,	312,	336,	0,	38,	62,	86,	110,	134,	158,	182,	206,	230,	254,	278,	302,	336,	48,	72,	96,	120,	144,	168,	192,	216,	240,	264,	288,	312,	52,	76,	100,	124,	148,	172,	196,	220,	244,	268,	292,	132,	384,	432,	384,	408,	432,	384,	408,	432,	408,	460,	484,	508,	532,	460,	484,	508,	532,	460,	484,	508,	532,	460,	484,	508,	532,	460,	484,	508,	484,	508,	532,	72,	102,	212,	248,	278,	384,	408,	432,	460,	408,	48,	308,	0,	24,	58,	90,	122,	154,	186,	220,	252,	284,	316,	348,	0,	0,	0,	0,	0,	384,	420,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 };
int16_t		nKeyPosY[128] 		= {0,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	40,	66,	66,	66,	66,	66,	66,	66,	66,	66,	66,	66,	66,	66,	66,	92,	92,	92,	92,	92,	92,	92,	92,	92,	92,	92,	92,	118,	118,	118,	118,	118,	118,	118,	118,	118,	118,	118,	144,	40,	40,	66,	66,	66,	92,	92,	92,	118,	40,	40,	40,	40,	66,	66,	66,	66,	92,	92,	92,	92,	118,	118,	118,	118,	144,	144,	144,	0,	0,	0,	144,	144,	144,	144,	144,	0,	0,	0,	0,	40,	144,	144,	0,	0,	12,	12,	12,	12,	12,	12,	12,	12,	12,	12,	0,	0,	0,	118,	92,	144,	144,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 };
int16_t		nKeyMode[128] 		= {0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	4,	4,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	9,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	7,	1,	1,	1,	1,	1,	8,	1,	8,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	8,	1,	1,	1,	1,	1,	1,	3,	3,	4,	3,	3,	10,	10,	10,	10,	10,	11,	11,	2,	2,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	0,	0,	0,	6,	5,	4,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0};

int8_t		sMouseName[5][8] 	= {"Left",	"Right",	"Left",	"Right",	"Ball"};
int16_t		nMousePosX[5] 		= {8, 72, 0, 136, 8};
int16_t		nMousePosY[5] 		= {0, 0, 28, 28, 32 };
int16_t		nMouseMode[5] 		= {1, 1, 2, 2, 3};

int8_t		sJoyName[8][8] 		= { "UP", "DOWN", "LEFT", "RIGHT", "      ", "B", "A", "      " };
int16_t		nJoyPosX[8] 		= { 32, 32,  0, 64, 0, 160, 224, 0 };
int16_t		nJoyPosY[8] 		= {  0, 64, 32, 32, 0,  64,  64, 0 };
int16_t		nJoyMode[8] 		= {  1,  1,  1,  1, 0,   2,   2, 0 };

/* グローバル構造体 */

/* 関数のプロトタイプ宣言 */
int16_t main(int16_t, int8_t**);
static void App_Init(void);
static void App_exit(void);
int16_t	BG_main(uint32_t);
void App_TimerProc( void );
int16_t App_RasterProc( uint16_t * );
void App_VsyncProc( void );
void App_HsyncProc( void );
int16_t	App_FlipProc( void );
int16_t	SetFlip(uint8_t);
int16_t	GetGameMode(uint8_t *);
int16_t	SetGameMode(uint8_t);

void (*usr_abort)(void);	/* ユーザのアボート処理関数 */

/* 関数 */
/*===========================================================================================*/
/* 関数名	：	*/
/* 引数		：	*/
/* 戻り値	：	*/
/*-------------------------------------------------------------------------------------------*/
/* 機能		：	*/
/*===========================================================================================*/
int16_t main(int16_t argc, int8_t** argv)
{
	int16_t	ret = 0;


	uint16_t	uFreeRunCount=0u;

	int16_t	loop;
	int16_t	pushCount = 0;

	uint8_t	bMode;
	
	ST_TASK		stTask = {0}; 
	
	/* 変数の初期化 */
	SetGameMode(1);
	
	/* 例外ハンドリング処理 */
	usr_abort = App_exit;	/* 例外発生で終了処理を実施する */
	init_trap14();			/* デバッグ用致命的エラーハンドリング */
#if 0	/* アドレスエラー発生 */
	{
		char buf[10];
		int *A = (int *)&buf[1];
		int B = *A;
		return B;
	}
#endif

	App_Init();	/* 初期化 */
	
	loop = 1;

	SetGameMode(1);
	SetTaskInfo(SCENE_INIT);	/* 初期化シーンへ設定 */
	
	/* 乱数 */
	srandom(TIMEGET());	/* 乱数の初期化 */
	
	do	/* メインループ処理 */
	{
		uint32_t time_st;

		/* 終了処理 */
		if(loop == 0)
		{
			break;
		}
		
		/* 時刻設定 */
		GetNowTime(&time_st);	/* メイン処理の開始時刻を取得 */
		SetStartTime(time_st);	/* メイン処理の開始時刻を記憶 */
		
		/* タスク処理 */
		TaskManage();				/* タスクを管理する */
		GetTaskInfo(&stTask);		/* タスクの情報を得る */

		/* モード引き渡し */
		GetGameMode(&bMode);
		
		/* 終了 */
		if((g_Input & KEY_b_ESC ) != 0u)	/* ＥＳＣキー */
		{
			pushCount = Minc(pushCount, 1);
			if(pushCount >= 5)
			{
				SetTaskInfo(SCENE_EXIT);	/* 終了シーンへ設定 */
			}
		}
		else
		{
			pushCount = 0;
		}

		switch(stTask.bScene)
		{
			case SCENE_INIT:	/* 初期化シーン */
			{
				T_Clear();	/* テキストクリア */
				SetTaskInfo(SCENE_START_S);	/* シーン(開始処理)へ設定 */
				break;
			}
			case SCENE_START_S:	/* シーン(開始処理) */
			{
				int32_t i, j;
				int32_t x, y;
				int32_t x_ofst, y_ofst;
				
				x=0;
				y=0;
				x_ofst = 4;
				y_ofst = 16;

				for(i = 0; i < 128; i++)
				{
					switch(nKeyMode[i])
					{
						case 0:		/* 何もしない */
						{
							break;
						}
						case 1:		/* ノーマルサイズ */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 24, 24, 0xFFFF, T_WHITE );
							break;
						}
						case 2:		/* ノーマルサイズ(大) */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 26, 26, 0xFFFF, T_WHITE );
							break;
						}
						case 3:		/* 特殊キー(小) XF1 */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 30, 24, 0xFFFF, T_WHITE );
							break;
						}
						case 4:		/* 特殊キー(中) XF3 TAB OPT.1 */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 36, 24, 0xFFFF, T_WHITE );
							break;
						}
						case 5:		/* 特殊キー(CTRL) */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 44, 24, 0xFFFF, T_WHITE );
							break;
						}
						case 6:		/* 特殊キー(SHIFT) */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 50, 24, 0xFFFF, T_WHITE );
							T_Box(nKeyPosX[i] + x_ofst + 320, nKeyPosY[i] + y_ofst, 50, 24, 0xFFFF, T_WHITE );
							break;
						}
						case 7:		/* 特殊キー(SPC) */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 80, 24, 0xFFFF, T_WHITE );
							break;
						}
						case 8:		/* 特殊キー(カーソル) */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 24, 48+2, 0xFFFF, T_WHITE );
							break;
						}
						case 9:		/* 特殊キー(CR) */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 36, 48+2, 0xFFFF, T_WHITE );
							break;
						}
						case 10:	/* 特殊キー(ランプ赤) */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 24, 24, 0xFFFF, T_WHITE );
							break;
						}
						case 11:	/* 特殊キー(ランプ緑) */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 24, 24, 0xFFFF, T_WHITE );
							break;
						}
						case 20:	/* 特殊キー(ファンクションキー) */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 30, 14, 0xFFFF, T_WHITE );
							break;
						}
						default:	/* 何もしない */
						{
							break;
						}
					}
				}

				/* マウス */
				x=0;
				y=0;
				x_ofst = 620;
				y_ofst = 16;
				for(i = 0; i < 5; i++)
				{
					switch(nMouseMode[i])
					{
						case 0:		/* 何もしない */
						{
							break;
						}
						case 1:		/* ボタン１ */
						{
							T_Box(nMousePosX[i] + x_ofst, nMousePosY[i] + y_ofst, 64, 32, 0xFFFF, T_WHITE );
							break;
						}
						case 2:		/* ボタン２ */
						{
							T_Box(nMousePosX[i] + x_ofst, nMousePosY[i] + y_ofst, 8, 16, 0xFFFF, T_WHITE );
							break;
						}
						case 3:		/* ボール */
						{
							T_Circle(nMousePosX[i] + x_ofst, nMousePosY[i] + y_ofst, 128, 128, 0xFFFF, T_WHITE );
							T_Circle(nMousePosX[i] + x_ofst + 1, nMousePosY[i] + y_ofst + 1, 126, 126, 0, T_BLACK1);
							break;
						}
					}
				}
				
				
				/* ジョイスティック */
				for(j = 0; j < 2; j++)
				{
					if(j == 0)			/* ジョイスティック1 */
					{
						x=0;
						y=0;
						x_ofst = 4;
						y_ofst = 248;
					}
					else if(j == 1)		/* ジョイスティック2 */
					{
						x=0;
						y=0;
						x_ofst = 320;
						y_ofst = 248;
					}
					else
					{
						/* 何もしない */
					}
					
					for(i = 0; i < 8; i++)
					{
						switch(nJoyMode[i])
						{
							case 0:		/* 何もしない */
							{
								break;
							}
							case 1:		/* 十字キー */
							{
								T_Box(nJoyPosX[i] + x_ofst, nJoyPosY[i] + y_ofst, 32, 32, 0xFFFF, T_WHITE );
								break;
							}
							case 2:		/* ボタン */
							{
								T_Circle(nJoyPosX[i] + x_ofst, nJoyPosY[i] + y_ofst, 32, 32, 0xFFFF, T_WHITE );
								T_Circle(nJoyPosX[i] + x_ofst + 1, nJoyPosY[i] + y_ofst + 1, 30, 30, 0, T_BLACK1);
								break;
							}
						}
					}
					T_Fill(nJoyPosX[2] + x_ofst + 32 - 1, nJoyPosY[2] + y_ofst - 1, 32 + 2, 32 + 2, 0, T_BLACK1);	/* 十字の真ん中 */
					T_Fill(nJoyPosX[2] + x_ofst + 32, nJoyPosY[2] + y_ofst, 32, 32, 0, T_BLACK1);	/* 十字の真ん中 */
				}

				T_Box( 0, 0, 768, 512, 0xFFFF, T_WHITE );
				_iocs_b_putmes(T_WHITE, 1, 28, 40, "Press and hold the ESC key to exit.");
				_iocs_b_putmes(T_WHITE, 40, 29, 52, "Input Check PRO-68k(InputCHK.x) ver1.02 (C)2023 カタ");
				
				g_Vwait = 1;

				SetTaskInfo(SCENE_START);	/* シーン(実施処理)へ設定 */
				break;
			}
			case SCENE_START:	/* シーン(実施処理) */
			{
				static int32_t input_old = 1;
				static int32_t LED_old;
				static int32_t move_count;
				int32_t color;
				int32_t bitsns, input;
				int32_t num;
				int32_t i,j;
				int32_t x, y;
				int8_t dx, dy;
				int8_t left, right;
				static int8_t dx_o, dy_o = 1;
				static int8_t left_o, right_o;
				int32_t x_ofst, y_ofst;
				int32_t cur_x, cur_y;
				int32_t r;
				int32_t bit, bit_o;
				int8_t sStr[32];
				static int32_t joy_old[2];

				/* キーボード */
				x=0;
				y=0;
				x_ofst = 4;
				y_ofst = 16;
				num = 0;
				color = G_BLACK;

				bitsns = _iocs_b_keysns();
				if(bitsns != 0)
				{
					input = _iocs_b_keyinp();
					input = input >> 8;
				}
				else
				{
					input = 0;
				}

				/* グループ分 */
				for(i = 0; i < 2; i++)
				{
					if( i != 0 )
					{
						if(input == 0)
						{
							break;
						}
						num = input;
						color = G_RED;
					}
					else
					{
						num = input_old;
						color = G_BLACK;
					}

					x = nKeyPosX[num] + x_ofst;
					y = nKeyPosY[num] + y_ofst;

					switch(nKeyMode[num])
					{
						case 0:		/* 何もしない */
						{
							break;
						}
						case 1:		/* ノーマルサイズ */
						{
							Draw_Fill( x, y, x + 24, y + 24, color);
							break;
						}
						case 2:		/* ノーマルサイズ(大) */
						{
							Draw_Fill( x, y, x + 26, y + 26, color);
							break;
						}
						case 3:		/* 特殊キー(小) XF1 */
						{
							Draw_Fill( x, y, x + 30, y + 24, color);
							break;
						}
						case 4:		/* 特殊キー(中) XF3 TAB OPT.1 */
						{
							Draw_Fill( x, y, x + 36, y + 24, color);
							break;
						}
						case 5:		/* 特殊キー(CTRL) */
						{
							Draw_Fill( x, y, x + 44, y + 24, color);
							break;
						}
						case 6:		/* 特殊キー(SHIFT) */
						{
							Draw_Fill( x, y, x + 50, y + 24, color);
							Draw_Fill( x + 320, y, x + 320 + 50, y + 24, color);
							break;
						}
						case 7:		/* 特殊キー(SPC) */
						{
							Draw_Fill( x, y, x + 80, y + 24, color);
							break;
						}
						case 8:		/* 特殊キー(カーソル) */
						{
							Draw_Fill( x, y, x + 24, y + 48+2, color);
							break;
						}
						case 9:		/* 特殊キー(CR) */
						{
							Draw_Fill( x, y, x + 36, y + 48+2, color);
							break;
						}
						case 10:	/* 特殊キー(ランプ赤) */
						{
							Draw_Fill( x, y, x + 24, y + 24, color);
							break;
						}
						case 11:	/* 特殊キー(ランプ緑) */
						{
							Draw_Fill( x, y, x + 24, y + 24, color);
							break;
						}
						case 20:	/* 特殊キー(ファンクションキー) */
						{
							Draw_Fill( x, y, x + 30, y + 14, color);
							break;
						}
						default:	/* 何もしない */
						{
							break;
						}
					}
				}
				color = T_WHITE;
				cur_x = 1;
				cur_y = 13;

				memset(sStr, 0, sizeof(sStr));
				if(input != 0)
				{
					sprintf(sStr, "Keybord(%s(0x%x))", &sKeyName[num][0], input);
					_iocs_b_putmes(color, cur_x, cur_y, 24, sStr);
				}
				else
				{
					if(input_old != 0)
					{
						sprintf(sStr, "Keybord(%s(0x%x))", &sKeyName[num][0], input_old);
						_iocs_b_putmes(color, cur_x, cur_y, 24, sStr);
					}
				}
				input_old = input;

				/* キーボードのLED */
				bitsns = _iocs_b_sftsns();
				bitsns = bitsns >> 8;
				for(i = 0; i < 7; i++)
				{
//					bit		= (bitsns >> i) & 0x01;
//					bit_o	= (LED_old >> i) & 0x01;

//					if( (bit ^ bit_o) == 0u )continue;

					num = i + 90;
					x = nKeyPosX[num] + x_ofst;
					y = nKeyPosY[num] + y_ofst;

					if(((bitsns >> i) & 0x01) != 0u)
					{
						if( (num >= 90) && (num <= 94) )
						{
							color = G_RED;
						}
						if( (num >= 95) && (num <= 96) )
						{
							color = G_GREEN;
						}
					}
					else
					{
						color = G_BLACK;
					}
					Draw_Fill( x+8, y+12, x + 16, y + 23, color);
				}
				LED_old = bitsns;

				/* マウス */
				x=0;
				y=0;
				dx=0;
				dy=0;
				x_ofst = 620;
				y_ofst = 16;
				color = G_RED;

				Mouse_GetDataPos(&dx, &dy, &left, &right);
				if( (dx == dx_o) &&
					(dy == dy_o) &&
					(left == left_o) &&
					(right == right_o) )
				{
					move_count = Mdec(move_count, 1);
					if(move_count == 0)
					{
						x = nMousePosX[0] + x_ofst;
						y = nMousePosY[0] + y_ofst;
						color = G_BLACK;							
						Draw_Fill( x, y + 32, x + 128, y + 32 + 128, color);
					}
				}
				else
				{
					move_count = 5;

					memset(sStr, 0, sizeof(sStr));
					strcpy(sStr, "Mouse(");
					
					dx_o = dx;
					dy_o = dy;
					left_o = left;
					right_o = right;

					if( (left & 0xFF) != 0u)
					{
						color = G_RED;
						x = nMousePosX[0] + x_ofst;
						y = nMousePosY[0] + y_ofst;
						Draw_Fill( x, y, x + 64, y + 32, color);
						x = nMousePosX[2] + x_ofst;
						y = nMousePosY[2] + y_ofst;
						Draw_Fill( x, y, x + 8, y + 16, color);
						strncat(sStr, &sMouseName[0][0], 1);
					}
					else
					{
						color = G_BLACK;							
						x = nMousePosX[0] + x_ofst;
						y = nMousePosY[0] + y_ofst;
						Draw_Fill( x, y, x + 64, y + 32, color);
						x = nMousePosX[2] + x_ofst;
						y = nMousePosY[2] + y_ofst;
						Draw_Fill( x, y, x + 8, y + 16, color);
						strcat(sStr, " ");
					}

					if( (right & 0xFF) != 0u)
					{
						color = G_RED;
						x = nMousePosX[1] + x_ofst;
						y = nMousePosY[1] + y_ofst;
						Draw_Fill( x, y, x + 64, y + 32, color);
						x = nMousePosX[3] + x_ofst;
						y = nMousePosY[3] + y_ofst;
						Draw_Fill( x, y, x + 8, y + 16, color);
						strncat(sStr, &sMouseName[1][0], 1);
					}
					else
					{
						color = G_BLACK;							
						x = nMousePosX[1] + x_ofst;
						y = nMousePosY[1] + y_ofst;
						Draw_Fill( x, y, x + 64, y + 32, color);
						x = nMousePosX[3] + x_ofst;
						y = nMousePosY[3] + y_ofst;
						Draw_Fill( x, y, x + 8, y + 16, color);
						strcat(sStr, " ");
					}
				
					color = G_RED;
					x = nMousePosX[0] + x_ofst + 64;
					y = nMousePosY[0] + y_ofst + 96;
					Draw_Line(x, y, x + (dx / 2), y + (dy / 2), color, 0xFFFF);

					color = T_WHITE;
					cur_x = 82;
					cur_y = 13;
					strcat(sStr, ")");
					_iocs_b_putmes(color, cur_x, cur_y, 10, sStr);
				}

				/* ジョイスティック1/2 */
				for(i = 0; i < 2; i++)
				{
					x=0;
					y=0;

					switch(i)
					{
						case 0:
						default:
						{
							cur_x = 1;
							cur_y = 23;
							x_ofst = 4;
							y_ofst = 248;
							break;		
						}
						case 1:
						{
							cur_x = 40;
							cur_y = 23;
							x_ofst = 320;
							y_ofst = 248;
							break;		
						}
					}

					bitsns = g_Input_P[i];

					memset(sStr, 0, sizeof(sStr));
					sprintf(sStr, "JoyStick%d(0x%02x)(", i+1, bitsns);

					for(j = 0; j < 8; j++)
					{

						num = j;
						x = nJoyPosX[num] + x_ofst;
						y = nJoyPosY[num] + y_ofst;

						bit		= (bitsns >> j) & 0x01;
						bit_o	= (joy_old[i] >> j) & 0x01;

						if( bit != 0 )
						{
							strncat(sStr, &sJoyName[num][0], 1);
							color = G_RED;
						}
						else
						{
							strcat(sStr, " ");
							color = G_BLACK;							
						}

						if( (bit ^ bit_o) == 0u )continue;

						if(nJoyMode[num] != 0)
						{
							if(nJoyMode[num] == 2)
							{
								r = 16;
								Draw_FillCircle( x+16, y+16, r, color, 0, 360, 255 );
							}
							else
							{
								Draw_Fill( x, y, x + 32, y + 32, color);
							}
						}
					}
					color = T_WHITE;
					strcat(sStr, ")");
					_iocs_b_putmes(color, cur_x, cur_y, 24, sStr);

					joy_old[i] = bitsns;
				}

				break;
			}
			case SCENE_START_E:	/* シーン(終了処理) */
			{
				SetTaskInfo(SCENE_START_E+1);	/* シーン(終了処理)へ設定 */
				break;
			}
			default:	/* 異常シーン */
			{
				loop = 0;	/* ループ終了 */
				break;
			}
		}

		uFreeRunCount++;	/* 16bit フリーランカウンタ更新 */

		/* 同期待ち */
		wait_vdisp(g_Vwait);

	}
	while( loop );

	g_bExit = FALSE;
	App_exit();	/* 終了処理 */
	
	return ret;
}

/*===========================================================================================*/
/* 関数名	：	*/
/* 引数		：	*/
/* 戻り値	：	*/
/*-------------------------------------------------------------------------------------------*/
/* 機能		：	*/
/*===========================================================================================*/
static void App_Init(void)
{
#ifdef DEBUG	/* デバッグコーナー */
	puts("App_Init 開始");
#endif
	/* スーパーバイザーモード開始 */
	g_nSuperchk = _dos_super(0);
	if( g_nSuperchk < 0 ) {
#ifdef DEBUG	/* デバッグコーナー */
		puts("App_Init すでに_dos_super");
#endif
	} else {
#ifdef DEBUG	/* デバッグコーナー */
		puts("App_Init _dos_super開始");
#endif
	}

	/* MFP */
	MFP_INIT();	/* V-Sync割り込み等の初期化処理 */
#ifdef DEBUG	/* デバッグコーナー */
	puts("App_Init MFP");
#endif

	/* 画面 */
	g_nCrtmod = CRTC_INIT(16);	/* mode=16 764x512 col:16 31kHz */
#ifdef DEBUG	/* デバッグコーナー */
	puts("App_Init 画面");
#endif

	/* テキスト */
	T_INIT();	/* テキストＶＲＡＭ初期化 */
	T_PALET();	/* テキストパレット初期化 */
#ifdef DEBUG	/* デバッグコーナー */
	puts("App_Init T_INIT");
#endif
	
	/* グラフィック */
	G_INIT();			/* 画面の初期設定 */
	G_CLR();			/* クリッピングエリア全開＆消す */
	G_HOME(0);			/* ホーム位置設定 */
//	G_VIDEO_INIT();		/* ビデオコントローラーの初期化 */
	G_PaletteSetZero();	/* 0番パレット変更 */
#ifdef DEBUG	/* デバッグコーナー */
//	puts("App_Init グラフィック");
#endif
	
	/* スプライト／ＢＧ */
//	PCG_INIT();	/* スプライト／ＢＧの初期化 */
//	PCG_INIT_CHAR();	/* スプライト＆ＢＧ定義セット */
//	PCG_VIEW(0x00u);	/* スプライト＆ＢＧ非表示 */
#ifdef DEBUG	/* デバッグコーナー */
//	puts("App_Init スプライト／ＢＧ");
#endif

	/* マウス初期化 */
//	Mouse_Init();	/* マウス初期化 */
#ifdef DEBUG	/* デバッグコーナー */
	puts("App_Init マウス");
#endif
	
#ifdef DEBUG	/* デバッグコーナー */
	puts("App_Init 終了");
#endif
}

/*===========================================================================================*/
/* 関数名	：	*/
/* 引数		：	*/
/* 戻り値	：	*/
/*-------------------------------------------------------------------------------------------*/
/* 機能		：	*/
/*===========================================================================================*/
static void App_exit(void)
{
	/* テキストクリア */
	T_Clear();	/* テキストクリア */
	
	/* グラフィック */
	G_CLR();			/* クリッピングエリア全開＆消す */

#ifdef DEBUG	/* デバッグコーナー */
	puts("App_exit 開始");
#endif
	
	if(g_bExit == TRUE)
	{
		puts("エラーをキャッチ！ ESC to skip");
		KeyHitESC();	/* デバッグ用 */
	}

	Mouse_Exit();	/* マウス非表示 */
	
	/* スプライト＆ＢＧ */
//	PCG_VIEW(0x00u);		/* スプライト＆ＢＧ非表示 */
#ifdef DEBUG	/* デバッグコーナー */
	puts("App_exit スプライト");
#endif

	/* 画面 */
	CRTMOD(0x100 + g_nCrtmod);	/* モードをもとに戻す */
#ifdef DEBUG	/* デバッグコーナー */
	puts("App_exit 画面");
#endif

	/* MFP */
	MFP_EXIT();				/* MFP関連の解除 */
#ifdef DEBUG	/* デバッグコーナー */
	puts("App_exit MFP");
#endif

	/* テキスト */
	T_EXIT();				/* テキスト終了処理 */
#ifdef DEBUG	/* デバッグコーナー */
	puts("App_exit テキスト");
#endif

	MyMfree(0);				/* 全てのメモリを解放 */
#ifdef DEBUG	/* デバッグコーナー */
	puts("App_exit メモリ");
#endif

	_dos_kflushio(0xFF);	/* キーバッファをクリア */
#ifdef DEBUG	/* デバッグコーナー */
	puts("App_exit キーバッファクリア");
#endif

	/*スーパーバイザーモード終了*/
	_dos_super(g_nSuperchk);
#ifdef DEBUG	/* デバッグコーナー */
	puts("App_exit スーパーバイザーモード終了");
#endif
	
#ifdef DEBUG	/* デバッグコーナー */
	puts("App_exit 終了");
#endif
}


/*===========================================================================================*/
/* 関数名	：	*/
/* 引数		：	*/
/* 戻り値	：	*/
/*-------------------------------------------------------------------------------------------*/
/* 機能		：	*/
/*===========================================================================================*/
int16_t	App_FlipProc(void)
{
	int16_t	ret = 0;
	
	if(g_bFlip == FALSE)	/* 描画中なのでフリップしない */
	{
		return ret;
	}
	
	SetFlip(FALSE);			/* フリップ禁止 */
	
	/* 画面切り替え */
	G_HOME(g_mode);
	
	/* モードチェンジ */
	if(g_mode == 1u)		/* 上側判定 */
	{
		SetGameMode(2);
	}
	else if(g_mode == 2u)	/* 下側判定 */
	{
		SetGameMode(1);
	}
	else					/* その他 */
	{
		SetGameMode(0);
	}
	
	ret = 1;

	return ret;
}

/*===========================================================================================*/
/* 関数名	：	*/
/* 引数		：	*/
/* 戻り値	：	*/
/*-------------------------------------------------------------------------------------------*/
/* 機能		：	*/
/*===========================================================================================*/
int16_t	SetFlip(uint8_t bFlag)
{
	int16_t	ret = 0;

	Set_DI();	/* 割り込み禁止 */
	g_bFlip_old = g_bFlip;	/* フリップ前回値更新 */
	g_bFlip = bFlag;
	Set_EI();	/* 割り込み許可 */
	
	return ret;
}
/*===========================================================================================*/
/* 関数名	：	*/
/* 引数		：	*/
/* 戻り値	：	*/
/*-------------------------------------------------------------------------------------------*/
/* 機能		：	*/
/*===========================================================================================*/
void App_TimerProc( void )
{
	ST_TASK stTask;
	
	TaskManage();
	GetTaskInfo(&stTask);	/* タスク取得 */
	/* ↓↓↓ この間に処理を書く ↓↓↓ */

	if(stTask.b96ms == TRUE)	/* 96ms周期 */
	{
		switch(stTask.bScene)
		{
			case SCENE_START:
			{
//				G_CLR();			/* クリッピングエリア全開＆消す */
			}
			break;
		}
	}

	/* ↑↑↑ この間に処理を書く ↑↑↑ */

	/* タスク処理 */
	UpdateTaskInfo();		/* タスクの情報を更新 */

}
/*===========================================================================================*/
/* 関数名	：	*/
/* 引数		：	*/
/* 戻り値	：	*/
/*-------------------------------------------------------------------------------------------*/
/* 機能		：	*/
/*===========================================================================================*/
int16_t App_RasterProc( uint16_t *pRaster_cnt )
{
	int16_t	ret = 0;

//	RasterProc(pRaster_cnt);	/*　ラスター割り込み処理 */

	return ret;
}

/*===========================================================================================*/
/* 関数名	：	*/
/* 引数		：	*/
/* 戻り値	：	*/
/*-------------------------------------------------------------------------------------------*/
/* 機能		：	*/
/*===========================================================================================*/
void App_VsyncProc( void )
{
	/* 入力処理 */
	Input_Main();
	
	/* ラスター割り込み処理セット */
//	RasterVcyncProc();
	
	/* 画面切り替え */
	App_FlipProc();

}

/*===========================================================================================*/
/* 関数名	：	*/
/* 引数		：	*/
/* 戻り値	：	*/
/*-------------------------------------------------------------------------------------------*/
/* 機能		：	*/
/*===========================================================================================*/
int16_t	GetGameMode(uint8_t *bMode)
{
	int16_t	ret = 0;
	
	*bMode = g_mode;
	
	return ret;
}

/*===========================================================================================*/
/* 関数名	：	*/
/* 引数		：	*/
/* 戻り値	：	*/
/*-------------------------------------------------------------------------------------------*/
/* 機能		：	*/
/*===========================================================================================*/
int16_t	SetGameMode(uint8_t bMode)
{
	int16_t	ret = 0;
	
	g_mode = bMode;
	if(bMode == 1)
	{
		g_mode_rev = 2;
	}
	else
	{
		g_mode_rev = 1;
	}
	
	return ret;
}

#endif	/* INPUTCHK_C */
