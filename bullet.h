//=============================================================================
//
// �e���ˏ��� [bullet.h]
// Author : 
//
//=============================================================================
#pragma once

#include "model.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_BULLET		(100)	// �e�ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	XMFLOAT4X4	mtxWorld;		// ���[���h�}�g���b�N�X
	XMFLOAT3	pos;			// �ʒu
	XMFLOAT3	rot;			// �p�x
	XMFLOAT3	scl;			// �X�P�[��
	XMFLOAT3	dir;			// ����
	MATERIAL	material;		// �}�e���A��

	DX11_MODEL	model;		// ���f�����

	float		spd;			// �ړ���
	float		fWidth;			// ��
	float		fHeight;		// ����
	int			shadowIdx;		// �eID
	int			cnt;
	BOOL		use;			// �g�p���Ă��邩�ǂ���

} BULLET;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(float deltaTime);
void DrawBullet(void);

int SetBullet(XMFLOAT3 pos, XMFLOAT3 rot);

float GetDeltaTime();
BULLET *GetBullet(void);

