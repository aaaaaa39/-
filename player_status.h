//=============================================================================
//
// �X�R�A���� [playerstatus.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Player_Status_DIGIT			(1)			// ����




//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayerStatus(void);
void UninitPlayerStatus(void);
void UpdatePlayerStatus(void);
void DrawPlayerStatus(void);

//void AddPlayerStatus(int add);
int GetPlayerStatus(void);
void SetPlayerStatus(int score);



