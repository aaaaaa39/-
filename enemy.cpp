//=============================================================================
//
// �G�l�~�[���f������ [enemy.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "enemy.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// #define		MODEL_ENEMY			"data/MODEL/shark_001.obj"			// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_BODY			"data/MODEL/shark_body_002.obj"		// �ǂݍ��ރ��f����

#define	MODEL_ENEMY_HEAD			"data/MODEL/shark_head_002.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_TAIL			"data/MODEL/shark_tail_002.obj"		// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(5.0f)						// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// ��]��

#define ENEMY_SHADOW_SIZE	(0.4f)						// �e�̑傫��
#define ENEMY_OFFSET_Y		(7.0f)						// �G�l�~�[�̑��������킹��

#define ENEMY_PARTS_MAX		(2)							// �G�l�~�[�̃p�[�c�̐�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ENEMY			g_Enemy[MAX_ENEMY];						// �G�l�~�[

static ENEMY			g_Parts[MAX_ENEMY][ENEMY_PARTS_MAX];		// �G�l�~�[�̃p�[�c�p

//int g_Enemy_load = 0;

// �G�l�~�[�̊K�w�A�j���[�V�����f�[�^
static INTERPOLATION_DATA MoveTbl_head[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.25f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -0.25f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA MoveTbl_tail[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -0.25f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.25f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

// �G�l�~�[�̐��`��ԃf�[�^
static INTERPOLATION_DATA g_MoveTbl0[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-200.0f, 0.0f, 200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1 },
	{ XMFLOAT3(200.0f, 0.0f, -200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.5f },

};


static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	MoveTbl_head,
	MoveTbl_tail,

	g_MoveTbl0,

};

// �T���̐��`���
static INTERPOLATION_DATA MoveTbl_Enemy_0[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(100.0f, 0.0f, 150.0f), XMFLOAT3(0.0f, 0.25f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 3 },
	{ XMFLOAT3(0.0f, 0.0f, -50.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 3 },
	{ XMFLOAT3(-50.0f, 0.0f, 50.0f), XMFLOAT3(0.0f, -0.25f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 3 },
	{ XMFLOAT3(175.0f, 0.0f, -150.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 3 },

};

static INTERPOLATION_DATA MoveTbl_Enemy_1[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 200.0f), XMFLOAT3(0.0f, -0.25f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-200.0f, 0.0f, 200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(200.0f, 0.0f, -200.0f), XMFLOAT3(0.0f, 0.25f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(100.0f, 0.0f, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

// �G�l�~�[�̐��`��ԃf�[�^
static INTERPOLATION_DATA MoveTbl_Enemy_2[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(300.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 3 },
	{ XMFLOAT3(100.0f, 0.0f, 200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 3 },
	{ XMFLOAT3(0.0f, 0.0f, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 3 },

};

static INTERPOLATION_DATA MoveTbl_Enemy_3[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(200.0f, 0.0f, 200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 3 },
	{ XMFLOAT3(-100.0f, 0.0f, 100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 3 },
	{ XMFLOAT3(150.0f, 0.0f, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 3 },

};

static INTERPOLATION_DATA MoveTbl_Enemy_4[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-180.0f, 0.0f, 250.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-100.0f, 0.0f, 30.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(200.0f, 0.0f, 75.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA MoveTbl_Enemy_5[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-10.0f, 0.0f, 90.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 3 },
	{ XMFLOAT3(0.0f, 0.0f, -60.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 3 },
	{ XMFLOAT3(-180.0f, 0.0f, -30.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 3 },

};

static INTERPOLATION_DATA MoveTbl_Enemy_6[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(350.0f, 0.0f, 90.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(80.0f, 0.0f, 60.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-10.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};


static INTERPOLATION_DATA* g_MoveTblAdr_Enemy[] =
{
	MoveTbl_Enemy_0,
	MoveTbl_Enemy_1,
	MoveTbl_Enemy_2,
	MoveTbl_Enemy_3,
	MoveTbl_Enemy_4,
	MoveTbl_Enemy_5,
	MoveTbl_Enemy_6,

};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_Enemy[i].load = TRUE;
		LoadModel(MODEL_ENEMY_BODY, &g_Enemy[i].model);

		g_Enemy[i].pos = XMFLOAT3(-50.0f + i * 30.0f, 7.0f, 20.0f);
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_Enemy[i].spd = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Enemy[i].use = TRUE;			// TRUE:�����Ă�
		g_Enemy[i].size = ENEMY_SIZE;	// �����蔻��̑傫��
		g_Enemy[i].Blinking = FALSE;	// �_�ŏ����̏�����
		g_Enemy[i].BlinkTimer = FALSE;	// �_�Ōo�ߎ��Ԃ̏�����
		g_Enemy[i].BlinkCnt = 0;		// �_�ŉ񐔂̏�����

		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Enemy[i].model, &g_Enemy[i].diffuse[0]);

		// �����ŃG�l�~�[�p�̉e���쐬���Ă���
		//XMFLOAT3 pos = g_Enemy[i].pos;
		//pos.y -= (ENEMY_OFFSET_Y - 0.1f);
		//g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);

		g_Enemy[i].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
		g_Enemy[i].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Enemy[i].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

		// �K�w�A�j���[�V�����p�̏���������
		g_Enemy[i].parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������


		// �p�[�c�̏�����
		for (int j = 0; j < ENEMY_PARTS_MAX; j++)
		{
			g_Parts[i][j].use = TRUE;

			// �ʒu�E��]�E�X�P�[���̏����ݒ�
			g_Parts[i][j].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Parts[i][j].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Parts[i][j].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

			// �e�q�֌W
			g_Parts[i][j].parent = &g_Enemy[i];		// �� �����ɐe�̃A�h���X������
			//	g_Parts[�r].parent= &g_Player;		// �r��������e�͖{�́i�v���C���[�j
			//	g_Parts[��].parent= &g_Paerts[�r];	// �w���r�̎q���������ꍇ�̗�

			// �K�w�A�j���[�V�����p�̃����o�[�ϐ��̏�����
			g_Parts[i][j].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
			g_Parts[i][j].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
			g_Parts[i][j].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

			// �p�[�c�̓ǂݍ��݂͂܂����Ă��Ȃ�
			g_Parts[i][j].load = TRUE;
		}


		g_Parts[i][0].use = TRUE;
		g_Parts[i][0].parent = &g_Enemy[i];	// �e���Z�b�g
		g_Parts[i][0].tblNo = 0;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Parts[i][0].tblMax = sizeof(MoveTbl_head) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
		g_Parts[i][0].load = TRUE;
		LoadModel(MODEL_ENEMY_HEAD, &g_Parts[i][0].model);

		g_Parts[i][1].use = TRUE;
		g_Parts[i][1].parent = &g_Enemy[i];	// �e���Z�b�g
		g_Parts[i][1].tblNo = 1;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Parts[i][1].tblMax = sizeof(MoveTbl_tail) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
		g_Parts[i][1].load = TRUE;
		LoadModel(MODEL_ENEMY_TAIL, &g_Parts[i][1].model);
	}

	g_Enemy[0].pos = XMFLOAT3(100.0f, 7.0f, 100.0f);
	g_Enemy[1].pos = XMFLOAT3(130.0f, 7.0f, -40.0f);
	g_Enemy[2].pos = XMFLOAT3(160.0f, 7.0f, -20.0f);
	g_Enemy[3].pos = XMFLOAT3(190.0f, 7.0f, -40.0f);
	g_Enemy[4].pos = XMFLOAT3(220.0f, 7.0f, -20.0f);
	g_Enemy[5].pos = XMFLOAT3(250.0f, 7.0f, -40.0f);

	// 0�Ԃ������`��Ԃœ������Ă݂�
	g_Enemy[0].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[0].tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[0].tblMax = sizeof(MoveTbl_Enemy_0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[1].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[1].tblNo = 1;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[1].tblMax = sizeof(MoveTbl_Enemy_1) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[2].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[2].tblNo = 2;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[2].tblMax = sizeof(MoveTbl_Enemy_2) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[3].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[3].tblNo = 3;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[3].tblMax = sizeof(MoveTbl_Enemy_3) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[4].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[4].tblNo = 4;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[4].tblMax = sizeof(MoveTbl_Enemy_3) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[5].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[5].tblNo = 5;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[5].tblMax = sizeof(MoveTbl_Enemy_3) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	// ���f���̊J������
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].load == TRUE)
		{
			UnloadModel(&g_Enemy[i].model); // ���f�������
			g_Enemy[i].load = FALSE;       // ���[�h��Ԃ��X�V
			g_Enemy[i].use = FALSE;		   // �G�l�~�[���J��
		}

	// �p�[�c�̊J������
		for (int j = 0; j < ENEMY_PARTS_MAX; j++)
		{
			if (g_Parts[i][j].load == TRUE)
			{
				UnloadModel(&g_Parts[i][j].model);
				g_Parts[i][j].load = FALSE;
				g_Parts[i][j].use = FALSE;
			}
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// �G�l�~�[�̃��[���h�}�g���b�N�X�v�Z�iBODY�j
		XMMATRIX mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		XMMATRIX mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y, g_Enemy[i].rot.z);
		XMMATRIX mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);

		XMMATRIX enemyWorldMatrix = mtxScl * mtxRot * mtxTranslate;
		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, enemyWorldMatrix);

		// �K�w�A�j���[�V����
		// �G�l�~�[�𓮂����ꍇ�́A�e�����킹�ē���������Y��Ȃ��悤�ɂˁI
		for (int j = 0; j < ENEMY_PARTS_MAX; j++)
		{
			// ���̃G�l�~�[���g���Ă���Ȃ珈������
			if ((g_Parts[i][j].use == TRUE) && (g_Parts[i][j].tblMax > 0))
			{	// ���`��Ԃ̏���
				int nowNo = (int)g_Parts[i][j].time;		// �������ł���e�[�u���ԍ������o���Ă���
				int maxNo = g_Parts[i][j].tblMax;			// �o�^�e�[�u�����𐔂��Ă���
				int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
				INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Parts[i][j].tblNo];	// �s���e�[�u���̃A�h���X���擾

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

				float nowTime = g_Parts[i][j].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

				Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
				Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
				Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

				// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
				XMStoreFloat3(&g_Parts[i][j].pos, nowPos + Pos);

				// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&g_Parts[i][j].rot, nowRot + Rot);

				// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&g_Parts[i][j].scl, nowScl + Scl);

				// frame���g�Ď��Ԍo�ߏ���������
				g_Parts[i][j].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
				if ((int)g_Parts[i][j].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
				{
					g_Parts[i][j].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
				}

			}
		}

		// �G�l�~�[�S�̂̈ړ�����
		if (g_Enemy[i].tblMax > 0)	// ���`��Ԃ����s����H
		{	// ���`��Ԃ̏���
			int nowNo = (int)g_Enemy[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
			int maxNo = g_Enemy[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
			int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
			INTERPOLATION_DATA* tbl = g_MoveTblAdr_Enemy[g_Enemy[i].tblNo];	// �s���e�[�u���̃A�h���X���擾

			XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
			XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
			XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

			XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
			XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
			XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

			float nowTime = g_Enemy[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

			Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
			Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
			Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

			// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
			XMStoreFloat3(&g_Enemy[i].pos, nowPos + Pos);

			// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
			XMStoreFloat3(&g_Enemy[i].rot, nowRot + Rot);

			// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
			XMStoreFloat3(&g_Enemy[i].scl, nowScl + Scl);

			// frame���g�Ď��Ԍo�ߏ���������
			g_Enemy[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
			if ((int)g_Enemy[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
			{
				g_Enemy[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
			}
		}
	}
//#ifdef _DEBUG
//
//	if (GetKeyboardTrigger(DIK_P))
//	{
//		// ���f���̐F��ύX�ł����I�������ɂ��ł����B
//		for (int j = 0; j < g_Enemy[i].model.SubsetNum; j++)
//		{
//			SetModelDiffuse(&g_Enemy[i].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
//		}
//	}
//
//	if (GetKeyboardTrigger(DIK_L))
//	{
//		// ���f���̐F�����ɖ߂��Ă���
//		for (int j = 0; j < g_Enemy[i].model.SubsetNum; j++)
//		{
//			SetModelDiffuse(&g_Enemy[i].model, j, g_Enemy[i].diffuse[j]);
//		}
//	}
//#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);


	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use)
		{
											// ���̍��W					�K���̍��W
			XMFLOAT3 POS[ENEMY_PARTS_MAX] = { XMFLOAT3(0.0f,-1.0f,23.5f),XMFLOAT3(0.0f,0.0f,-29.0f) };
			XMFLOAT3 SCL[ENEMY_PARTS_MAX] = { XMFLOAT3(0.05f, 0.05f, 0.05f),XMFLOAT3(0.0f,0.0f,0.0f) };

			//SetFuchi(1);	// 1=true

			// �܂��͊�{�ƂȂ�a�n�c�x��`�悷��

			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);

			// ���f���`��
			DrawModel(&g_Enemy[i].model);

			// �K�w�A�j���[�V����
			for (int j = 0; j < ENEMY_PARTS_MAX; j++)
			{
				// ���[���h�}�g���b�N�X�̏�����
				mtxWorld = XMMatrixIdentity();

				// �X�P�[���𔽉f
				mtxScl = XMMatrixScaling(g_Parts[i][j].scl.x + SCL[j].x, g_Parts[i][j].scl.y + SCL[j].y, g_Parts[i][j].scl.z + SCL[j].z);
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

				// ��]�𔽉f
				mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i][j].rot.x, g_Parts[i][j].rot.y, g_Parts[i][j].rot.z);
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

				// �ړ��𔽉f
				mtxTranslate = XMMatrixTranslation(g_Parts[i][j].pos.x + POS[j].x, g_Parts[i][j].pos.y + POS[j].y, g_Parts[i][j].pos.z + POS[j].z);
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

				if (g_Parts[i][j].parent != NULL)	// �q����������e�ƌ�������
				{
					mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i][j].parent->mtxWorld));
					// ��
					// g_Enemy[i].mtxWorld���w���Ă���
				}

				XMStoreFloat4x4(&g_Parts[i][j].mtxWorld, mtxWorld);

				// �g���Ă���Ȃ珈������B�����܂ŏ������Ă��闝�R�͑��̃p�[�c�����̃p�[�c���Q�Ƃ��Ă���\�������邩��B
				if (g_Parts[i][j].use == FALSE) continue;

				// ���[���h�}�g���b�N�X�̐ݒ�
				SetWorldMatrix(&mtxWorld);

				// ���f���`��
				DrawModel(&g_Parts[i][j].model);
			}

		}
	}
	//SetFuchi(0);	// 0=false

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
ENEMY* GetEnemy()
{
	return &g_Enemy[0];
}
