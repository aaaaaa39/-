//=============================================================================
//
// スコア処理 [playerstatus.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Player_Status_DIGIT			(1)			// 桁数




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayerStatus(void);
void UninitPlayerStatus(void);
void UpdatePlayerStatus(void);
void DrawPlayerStatus(void);

//void AddPlayerStatus(int add);
int GetPlayerStatus(void);
void SetPlayerStatus(int score);



