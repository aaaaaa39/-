//=============================================================================
//
// 弾発射処理 [bullet.h]
// Author : 
//
//=============================================================================
#pragma once

#include "model.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_BULLET		(100)	// 弾最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	XMFLOAT4X4	mtxWorld;		// ワールドマトリックス
	XMFLOAT3	pos;			// 位置
	XMFLOAT3	rot;			// 角度
	XMFLOAT3	scl;			// スケール
	XMFLOAT3	dir;			// 向き
	MATERIAL	material;		// マテリアル

	DX11_MODEL	model;		// モデル情報

	float		spd;			// 移動量
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	int			shadowIdx;		// 影ID
	int			cnt;
	BOOL		use;			// 使用しているかどうか

} BULLET;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(float deltaTime);
void DrawBullet(void);

int SetBullet(XMFLOAT3 pos, XMFLOAT3 rot);

float GetDeltaTime();
BULLET *GetBullet(void);

