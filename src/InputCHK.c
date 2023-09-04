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

/* �O���[�o���ϐ� */
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

int8_t		sKeyName[128][8] 	= {"����`",	"ESC",	"1",	"2",	"3",	"4",	"5",	"6",	"7",	"8",	"9",	"0",	"-",	"^",	"\\",	"BS",	"TAB",	"Q",	"W",	"E",	"R",	"T",	"Y",	"U",	"I",	"O",	"P",	"@",	"[",	"CR",	"A",	"S",	"D",	"F",	"G",	"H",	"J",	"K",	"L",	";",	":",	"]",	"Z",	"X",	"C",	"V",	"B",	"N",	"M",	",",	".",	"/",	"_",	"SPC",	"HOME",	"DEL",	"ROLLUP",	"ROLLDN",	"UNDO",	"��",	"��",	"��",	"��",	"CLR",	"/",	"*",	"-",	"7",	"8",	"9",	"+",	"4",	"5",	"6",	"=",	"1",	"2",	"3",	"ENTER",	"0",	",",	".",	"�L��",	"�o�^",	"HELP",	"XF1",	"XF2",	"XF3",	"XF4",	"XF5",	"����",	"���[�}",	"�R�[�h",	"CAPS",	"INS",	"�Ђ�",	"�S�p",	"BREAK",	"COPY",	"F1",	"F2",	"F3",	"F4",	"F5",	"F6",	"F7",	"F8",	"F9",	"F10",	"����`",	"����`",	"����`",	"SHIFT",	"CTRL",	"OPT.1",	"OPT.2",	"����`",	"����`",	"����`",	"����`",	"����`",	"����`",	"����`",	"����`",	"����`",	"����`",	"����`",	"����`"};
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

/* �O���[�o���\���� */

/* �֐��̃v���g�^�C�v�錾 */
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

void (*usr_abort)(void);	/* ���[�U�̃A�{�[�g�����֐� */

/* �֐� */
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t main(int16_t argc, int8_t** argv)
{
	int16_t	ret = 0;


	uint16_t	uFreeRunCount=0u;

	int16_t	loop;
	int16_t	pushCount = 0;

	uint8_t	bMode;
	
	ST_TASK		stTask = {0}; 
	
	/* �ϐ��̏����� */
	SetGameMode(1);
	
	/* ��O�n���h�����O���� */
	usr_abort = App_exit;	/* ��O�����ŏI�����������{���� */
	init_trap14();			/* �f�o�b�O�p�v���I�G���[�n���h�����O */
#if 0	/* �A�h���X�G���[���� */
	{
		char buf[10];
		int *A = (int *)&buf[1];
		int B = *A;
		return B;
	}
#endif

	App_Init();	/* ������ */
	
	loop = 1;

	SetGameMode(1);
	SetTaskInfo(SCENE_INIT);	/* �������V�[���֐ݒ� */
	
	/* ���� */
	srandom(TIMEGET());	/* �����̏����� */
	
	do	/* ���C�����[�v���� */
	{
		uint32_t time_st;

		/* �I������ */
		if(loop == 0)
		{
			break;
		}
		
		/* �����ݒ� */
		GetNowTime(&time_st);	/* ���C�������̊J�n�������擾 */
		SetStartTime(time_st);	/* ���C�������̊J�n�������L�� */
		
		/* �^�X�N���� */
		TaskManage();				/* �^�X�N���Ǘ����� */
		GetTaskInfo(&stTask);		/* �^�X�N�̏��𓾂� */

		/* ���[�h�����n�� */
		GetGameMode(&bMode);
		
		/* �I�� */
		if((g_Input & KEY_b_ESC ) != 0u)	/* �d�r�b�L�[ */
		{
			pushCount = Minc(pushCount, 1);
			if(pushCount >= 5)
			{
				SetTaskInfo(SCENE_EXIT);	/* �I���V�[���֐ݒ� */
			}
		}
		else
		{
			pushCount = 0;
		}

		switch(stTask.bScene)
		{
			case SCENE_INIT:	/* �������V�[�� */
			{
				T_Clear();	/* �e�L�X�g�N���A */
				SetTaskInfo(SCENE_START_S);	/* �V�[��(�J�n����)�֐ݒ� */
				break;
			}
			case SCENE_START_S:	/* �V�[��(�J�n����) */
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
						case 0:		/* �������Ȃ� */
						{
							break;
						}
						case 1:		/* �m�[�}���T�C�Y */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 24, 24, 0xFFFF, T_WHITE );
							break;
						}
						case 2:		/* �m�[�}���T�C�Y(��) */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 26, 26, 0xFFFF, T_WHITE );
							break;
						}
						case 3:		/* ����L�[(��) XF1 */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 30, 24, 0xFFFF, T_WHITE );
							break;
						}
						case 4:		/* ����L�[(��) XF3 TAB OPT.1 */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 36, 24, 0xFFFF, T_WHITE );
							break;
						}
						case 5:		/* ����L�[(CTRL) */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 44, 24, 0xFFFF, T_WHITE );
							break;
						}
						case 6:		/* ����L�[(SHIFT) */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 50, 24, 0xFFFF, T_WHITE );
							T_Box(nKeyPosX[i] + x_ofst + 320, nKeyPosY[i] + y_ofst, 50, 24, 0xFFFF, T_WHITE );
							break;
						}
						case 7:		/* ����L�[(SPC) */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 80, 24, 0xFFFF, T_WHITE );
							break;
						}
						case 8:		/* ����L�[(�J�[�\��) */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 24, 48+2, 0xFFFF, T_WHITE );
							break;
						}
						case 9:		/* ����L�[(CR) */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 36, 48+2, 0xFFFF, T_WHITE );
							break;
						}
						case 10:	/* ����L�[(�����v��) */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 24, 24, 0xFFFF, T_WHITE );
							break;
						}
						case 11:	/* ����L�[(�����v��) */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 24, 24, 0xFFFF, T_WHITE );
							break;
						}
						case 20:	/* ����L�[(�t�@���N�V�����L�[) */
						{
							T_Box(nKeyPosX[i] + x_ofst, nKeyPosY[i] + y_ofst, 30, 14, 0xFFFF, T_WHITE );
							break;
						}
						default:	/* �������Ȃ� */
						{
							break;
						}
					}
				}

				/* �}�E�X */
				x=0;
				y=0;
				x_ofst = 620;
				y_ofst = 16;
				for(i = 0; i < 5; i++)
				{
					switch(nMouseMode[i])
					{
						case 0:		/* �������Ȃ� */
						{
							break;
						}
						case 1:		/* �{�^���P */
						{
							T_Box(nMousePosX[i] + x_ofst, nMousePosY[i] + y_ofst, 64, 32, 0xFFFF, T_WHITE );
							break;
						}
						case 2:		/* �{�^���Q */
						{
							T_Box(nMousePosX[i] + x_ofst, nMousePosY[i] + y_ofst, 8, 16, 0xFFFF, T_WHITE );
							break;
						}
						case 3:		/* �{�[�� */
						{
							T_Circle(nMousePosX[i] + x_ofst, nMousePosY[i] + y_ofst, 128, 128, 0xFFFF, T_WHITE );
							T_Circle(nMousePosX[i] + x_ofst + 1, nMousePosY[i] + y_ofst + 1, 126, 126, 0, T_BLACK1);
							break;
						}
					}
				}
				
				
				/* �W���C�X�e�B�b�N */
				for(j = 0; j < 2; j++)
				{
					if(j == 0)			/* �W���C�X�e�B�b�N1 */
					{
						x=0;
						y=0;
						x_ofst = 4;
						y_ofst = 248;
					}
					else if(j == 1)		/* �W���C�X�e�B�b�N2 */
					{
						x=0;
						y=0;
						x_ofst = 320;
						y_ofst = 248;
					}
					else
					{
						/* �������Ȃ� */
					}
					
					for(i = 0; i < 8; i++)
					{
						switch(nJoyMode[i])
						{
							case 0:		/* �������Ȃ� */
							{
								break;
							}
							case 1:		/* �\���L�[ */
							{
								T_Box(nJoyPosX[i] + x_ofst, nJoyPosY[i] + y_ofst, 32, 32, 0xFFFF, T_WHITE );
								break;
							}
							case 2:		/* �{�^�� */
							{
								T_Circle(nJoyPosX[i] + x_ofst, nJoyPosY[i] + y_ofst, 32, 32, 0xFFFF, T_WHITE );
								T_Circle(nJoyPosX[i] + x_ofst + 1, nJoyPosY[i] + y_ofst + 1, 30, 30, 0, T_BLACK1);
								break;
							}
						}
					}
					T_Fill(nJoyPosX[2] + x_ofst + 32 - 1, nJoyPosY[2] + y_ofst - 1, 32 + 2, 32 + 2, 0, T_BLACK1);	/* �\���̐^�� */
					T_Fill(nJoyPosX[2] + x_ofst + 32, nJoyPosY[2] + y_ofst, 32, 32, 0, T_BLACK1);	/* �\���̐^�� */
				}

				T_Box( 0, 0, 768, 512, 0xFFFF, T_WHITE );
				_iocs_b_putmes(T_WHITE, 1, 28, 40, "Press and hold the ESC key to exit.");
				_iocs_b_putmes(T_WHITE, 40, 29, 52, "Input Check PRO-68k(InputCHK.x) ver1.02 (C)2023 �J�^");
				
				g_Vwait = 1;

				SetTaskInfo(SCENE_START);	/* �V�[��(���{����)�֐ݒ� */
				break;
			}
			case SCENE_START:	/* �V�[��(���{����) */
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

				/* �L�[�{�[�h */
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

				/* �O���[�v�� */
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
						case 0:		/* �������Ȃ� */
						{
							break;
						}
						case 1:		/* �m�[�}���T�C�Y */
						{
							Draw_Fill( x, y, x + 24, y + 24, color);
							break;
						}
						case 2:		/* �m�[�}���T�C�Y(��) */
						{
							Draw_Fill( x, y, x + 26, y + 26, color);
							break;
						}
						case 3:		/* ����L�[(��) XF1 */
						{
							Draw_Fill( x, y, x + 30, y + 24, color);
							break;
						}
						case 4:		/* ����L�[(��) XF3 TAB OPT.1 */
						{
							Draw_Fill( x, y, x + 36, y + 24, color);
							break;
						}
						case 5:		/* ����L�[(CTRL) */
						{
							Draw_Fill( x, y, x + 44, y + 24, color);
							break;
						}
						case 6:		/* ����L�[(SHIFT) */
						{
							Draw_Fill( x, y, x + 50, y + 24, color);
							Draw_Fill( x + 320, y, x + 320 + 50, y + 24, color);
							break;
						}
						case 7:		/* ����L�[(SPC) */
						{
							Draw_Fill( x, y, x + 80, y + 24, color);
							break;
						}
						case 8:		/* ����L�[(�J�[�\��) */
						{
							Draw_Fill( x, y, x + 24, y + 48+2, color);
							break;
						}
						case 9:		/* ����L�[(CR) */
						{
							Draw_Fill( x, y, x + 36, y + 48+2, color);
							break;
						}
						case 10:	/* ����L�[(�����v��) */
						{
							Draw_Fill( x, y, x + 24, y + 24, color);
							break;
						}
						case 11:	/* ����L�[(�����v��) */
						{
							Draw_Fill( x, y, x + 24, y + 24, color);
							break;
						}
						case 20:	/* ����L�[(�t�@���N�V�����L�[) */
						{
							Draw_Fill( x, y, x + 30, y + 14, color);
							break;
						}
						default:	/* �������Ȃ� */
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

				/* �L�[�{�[�h��LED */
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

				/* �}�E�X */
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

				/* �W���C�X�e�B�b�N1/2 */
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
			case SCENE_START_E:	/* �V�[��(�I������) */
			{
				SetTaskInfo(SCENE_START_E+1);	/* �V�[��(�I������)�֐ݒ� */
				break;
			}
			default:	/* �ُ�V�[�� */
			{
				loop = 0;	/* ���[�v�I�� */
				break;
			}
		}

		uFreeRunCount++;	/* 16bit �t���[�����J�E���^�X�V */

		/* �����҂� */
		wait_vdisp(g_Vwait);

	}
	while( loop );

	g_bExit = FALSE;
	App_exit();	/* �I������ */
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static void App_Init(void)
{
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init �J�n");
#endif
	/* �X�[�p�[�o�C�U�[���[�h�J�n */
	g_nSuperchk = _dos_super(0);
	if( g_nSuperchk < 0 ) {
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		puts("App_Init ���ł�_dos_super");
#endif
	} else {
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
		puts("App_Init _dos_super�J�n");
#endif
	}

	/* MFP */
	MFP_INIT();	/* V-Sync���荞�ݓ��̏��������� */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init MFP");
#endif

	/* ��� */
	g_nCrtmod = CRTC_INIT(16);	/* mode=16 764x512 col:16 31kHz */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init ���");
#endif

	/* �e�L�X�g */
	T_INIT();	/* �e�L�X�g�u�q�`�l������ */
	T_PALET();	/* �e�L�X�g�p���b�g������ */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init T_INIT");
#endif
	
	/* �O���t�B�b�N */
	G_INIT();			/* ��ʂ̏����ݒ� */
	G_CLR();			/* �N���b�s���O�G���A�S�J������ */
	G_HOME(0);			/* �z�[���ʒu�ݒ� */
//	G_VIDEO_INIT();		/* �r�f�I�R���g���[���[�̏����� */
	G_PaletteSetZero();	/* 0�ԃp���b�g�ύX */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
//	puts("App_Init �O���t�B�b�N");
#endif
	
	/* �X�v���C�g�^�a�f */
//	PCG_INIT();	/* �X�v���C�g�^�a�f�̏����� */
//	PCG_INIT_CHAR();	/* �X�v���C�g���a�f��`�Z�b�g */
//	PCG_VIEW(0x00u);	/* �X�v���C�g���a�f��\�� */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
//	puts("App_Init �X�v���C�g�^�a�f");
#endif

	/* �}�E�X������ */
//	Mouse_Init();	/* �}�E�X������ */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init �}�E�X");
#endif
	
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_Init �I��");
#endif
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
static void App_exit(void)
{
	/* �e�L�X�g�N���A */
	T_Clear();	/* �e�L�X�g�N���A */
	
	/* �O���t�B�b�N */
	G_CLR();			/* �N���b�s���O�G���A�S�J������ */

#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit �J�n");
#endif
	
	if(g_bExit == TRUE)
	{
		puts("�G���[���L���b�`�I ESC to skip");
		KeyHitESC();	/* �f�o�b�O�p */
	}

	Mouse_Exit();	/* �}�E�X��\�� */
	
	/* �X�v���C�g���a�f */
//	PCG_VIEW(0x00u);		/* �X�v���C�g���a�f��\�� */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit �X�v���C�g");
#endif

	/* ��� */
	CRTMOD(0x100 + g_nCrtmod);	/* ���[�h�����Ƃɖ߂� */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit ���");
#endif

	/* MFP */
	MFP_EXIT();				/* MFP�֘A�̉��� */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit MFP");
#endif

	/* �e�L�X�g */
	T_EXIT();				/* �e�L�X�g�I������ */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit �e�L�X�g");
#endif

	MyMfree(0);				/* �S�Ẵ���������� */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit ������");
#endif

	_dos_kflushio(0xFF);	/* �L�[�o�b�t�@���N���A */
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit �L�[�o�b�t�@�N���A");
#endif

	/*�X�[�p�[�o�C�U�[���[�h�I��*/
	_dos_super(g_nSuperchk);
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit �X�[�p�[�o�C�U�[���[�h�I��");
#endif
	
#ifdef DEBUG	/* �f�o�b�O�R�[�i�[ */
	puts("App_exit �I��");
#endif
}


/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	App_FlipProc(void)
{
	int16_t	ret = 0;
	
	if(g_bFlip == FALSE)	/* �`�撆�Ȃ̂Ńt���b�v���Ȃ� */
	{
		return ret;
	}
	
	SetFlip(FALSE);			/* �t���b�v�֎~ */
	
	/* ��ʐ؂�ւ� */
	G_HOME(g_mode);
	
	/* ���[�h�`�F���W */
	if(g_mode == 1u)		/* �㑤���� */
	{
		SetGameMode(2);
	}
	else if(g_mode == 2u)	/* �������� */
	{
		SetGameMode(1);
	}
	else					/* ���̑� */
	{
		SetGameMode(0);
	}
	
	ret = 1;

	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	SetFlip(uint8_t bFlag)
{
	int16_t	ret = 0;

	Set_DI();	/* ���荞�݋֎~ */
	g_bFlip_old = g_bFlip;	/* �t���b�v�O��l�X�V */
	g_bFlip = bFlag;
	Set_EI();	/* ���荞�݋��� */
	
	return ret;
}
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void App_TimerProc( void )
{
	ST_TASK stTask;
	
	TaskManage();
	GetTaskInfo(&stTask);	/* �^�X�N�擾 */
	/* ������ ���̊Ԃɏ��������� ������ */

	if(stTask.b96ms == TRUE)	/* 96ms���� */
	{
		switch(stTask.bScene)
		{
			case SCENE_START:
			{
//				G_CLR();			/* �N���b�s���O�G���A�S�J������ */
			}
			break;
		}
	}

	/* ������ ���̊Ԃɏ��������� ������ */

	/* �^�X�N���� */
	UpdateTaskInfo();		/* �^�X�N�̏����X�V */

}
/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t App_RasterProc( uint16_t *pRaster_cnt )
{
	int16_t	ret = 0;

//	RasterProc(pRaster_cnt);	/*�@���X�^�[���荞�ݏ��� */

	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
void App_VsyncProc( void )
{
	/* ���͏��� */
	Input_Main();
	
	/* ���X�^�[���荞�ݏ����Z�b�g */
//	RasterVcyncProc();
	
	/* ��ʐ؂�ւ� */
	App_FlipProc();

}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
/*===========================================================================================*/
int16_t	GetGameMode(uint8_t *bMode)
{
	int16_t	ret = 0;
	
	*bMode = g_mode;
	
	return ret;
}

/*===========================================================================================*/
/* �֐���	�F	*/
/* ����		�F	*/
/* �߂�l	�F	*/
/*-------------------------------------------------------------------------------------------*/
/* �@�\		�F	*/
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
