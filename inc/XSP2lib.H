#ifndef XSP2LIB_H
#define XSP2LIB_H


/* gcc 10 �ȍ~�ł́AC �̊֐��ďo���K��𖳎����� inline asm �Ŋ֐������s */
#if defined(__GNUC__)
#	if (__GNUC__ >= 10)
#		define XSP_USE_INLINE_ASM (1)
#		define XSP_FORCE_INLINE	__attribute__((__always_inline__)) inline
#	endif
#endif

/* xsp_set_st , xobj_set_st �̈����\���� */
typedef	struct {
	short	x;				/* X ���W */
	short	y;				/* Y ���W */
	short	pt;				/* �X�v���C�g�p�^�[�� No. */
	short	info;			/* ���]�R�[�h�E�F�E�D��x��\�킷�f�[�^ */
} XSP_SET_ARG;


/* �����X�v���C�g�̃t���[���f�[�^�\���� */
typedef	struct	{
	short	vx;				/* ���΍��W�f�[�^ */
	short	vy;				/* ���΍��W�f�[�^ */
	short	pt;				/* �X�v���C�g�p�^�[�� No. */
	short	rv;				/* ���]�R�[�h */
} XOBJ_FRM_DAT;


/* �����X�v���C�g�̃��t�@�����X�f�[�^�\���� */
typedef	struct	{
	short	num;			/* �����X�v���C�g�� */
	void	*ptr;			/* �J�n�ʒu�̃|�C���^ */
	short	unused;			/*�i���g�p�j*/
} XOBJ_REF_DAT;


/* ���X�^�[���荞�ݏ����̃^�C���`���[�g */
typedef	struct	{
	short	ras_no;			/* ���荞�݃��X�^�i���o�[ */
	void	(* proc)();		/* ���荞�ݏ����֐��̃|�C���^ */
} XSP_TIME_CHART;



void	xsp_on(void);
void	xsp_off(void);
void	xsp_pcgdat_set(const void *pcg_dat, char *pcg_alt, short alt_size);
void	xsp_pcgmask_on(short start_no, short end_no);
void	xsp_pcgmask_off(short start_no, short end_no);
void	xsp_mode(short mode_no);
void	xsp_objdat_set(const void *sp_ref);
short	xsp_vsync(short n);			/* obsoleted */
short	xsp_vsync2(short max_delay);
#if !defined(XSP_USE_INLINE_ASM)
short	xsp_set(short x, short y, short pt, short info);
void	xobj_set(short x, short y, short pt, short info);
short	xsp_set_st(const void *arg);
void	xobj_set_st(const void *arg);
#else
	static XSP_FORCE_INLINE short xsp_set(short x, short y, short pt, short info) {
		register short reg_d0 asm ("d0") = x;
		register short reg_d1 asm ("d1") = y;
		register short reg_d2 asm ("d2") = pt;
		register short reg_d3 asm ("d3") = info;
		asm volatile (
				"	jbsr	_xsp_set_asm\n"
		:	/* �o�� */	"+r"	(reg_d0)	/* in out %0 (���́��߂�l) */
		:	/* ���� */	"r"		(reg_d1),	/* in     %1 (���́��ێ�) */
						"r"		(reg_d2),	/* in     %2 (���́��ێ�) */
						"r"		(reg_d3)	/* in     %3 (���́��ێ�) */
		:	/* �j�� */	"a0"
		);
		return reg_d0;
	}
	static XSP_FORCE_INLINE short xsp_set_st(const void *arg) {
		register short       reg_d0 asm ("d0");
		register const void *reg_a0 asm ("a0") = arg;
		asm volatile (
				"	jbsr	_xsp_set_st_asm\n"
		:	/* �o�� */	"=r"	(reg_d0),	/*    out %0 (�߂�l) */
						"+r"	(reg_a0)	/* in out %1 (���́��j��) */
		:	/* ���� */
		:	/* �j�� */	"memory",			/* �������o���A��v�� */
						"a1"
		);
		return reg_d0;
	}
	static XSP_FORCE_INLINE void xobj_set(short x, short y, short pt, short info) {
		register short reg_d0 asm ("d0") = x;
		register short reg_d1 asm ("d1") = y;
		register short reg_d2 asm ("d2") = pt;
		register short reg_d3 asm ("d3") = info;
		asm volatile (
				"	jbsr	_xobj_set_asm\n"
		:	/* �o�� */	"+r"	(reg_d0),	/* in out %0 (���́��j��) */
						"+r"	(reg_d1),	/* in out %1 (���́��j��) */
						"+r"	(reg_d2),	/* in out %2 (���́��j��) */
						"+r"	(reg_d3)	/* in out %3 (���́��j��) */
		:	/* ���� */
		:	/* �j�� */	"d4",
						"a0","a1","a2"
		);
	}
	static XSP_FORCE_INLINE void xobj_set_st(const void *arg) {
		register short       reg_d0 asm ("d0");
		register const void *reg_a0 asm ("a0") = arg;
		asm volatile (
				"	jbsr	_xobj_set_st_asm\n"
		:	/* �o�� */	"=r"	(reg_d0),	/* out %0 (�j��) */
						"+r"	(reg_a0)	/* out %1 (���́��j��) */
		:	/* ���� */
		:	/* �j�� */	"memory",			/* �������o���A��v�� */
						"d1","d2","d3","d4",
						"a1","a2"
		);
	}
#endif
short	xsp_out(void);
short	xsp_out2(void *vsyncint_arg);
void	xsp_vertical(short flag);

void	xsp_vsyncint_on(void *proc);
void	xsp_vsyncint_off(void);
void	xsp_hsyncint_on(const void *time_chart);
void	xsp_hsyncint_off(void);

void	xsp_auto_adjust_divy(short flag);
void	xsp_min_divh_set(short h);
short	xsp_divy_get(short i);
void	xsp_raster_ofs_for31khz_set(short ofs);
short	xsp_raster_ofs_for31khz_get(void);
void	xsp_raster_ofs_for15khz_set(short ofs);
short	xsp_raster_ofs_for15khz_get(void);

void	xsp_vsync_interval_set(short interval);
short	xsp_vsync_interval_get(void);

#endif