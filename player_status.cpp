//=============================================================================
//
// プレイヤーステータス処理 [playerstatus.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "score.h"
#include "sprite.h"
#include "player_status.h"
#include "player.h"
#include "enemy.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(124)	// HPサイズ
#define TEXTURE_HEIGHT				(32)	// 
#define TEXTURE_MAX					(2)		// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;							// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static PLAYER		g_Player;										// プレイヤー

static char* g_TextureName[] = {
	"data/TEXTURE/HP Bar0.png",		// HPMAX
	"data/TEXTURE/HP Bar1.png",
	"data/TEXTURE/HP Bar2.png",
	"data/TEXTURE/HP Bar3.png",
	"data/TEXTURE/HP Bar4.png",
	"data/TEXTURE/HP Bar5.png",
	"data/TEXTURE/HP Bar6.png",		// HP0

};


static bool						g_Use;						// true:使っている  false:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

static int						g_PlayerStatus;				// HP 酸素ゲージなど

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayerStatus(void)
{
	ID3D11Device* pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// プレイヤーのステータスUIの初期化
	g_Use = TRUE;
	g_w = TEXTURE_WIDTH;
	g_h = TEXTURE_HEIGHT;
	g_Pos = { 100.0f, 20.0f, 0.0f };
	g_TexNo = 0;

	g_Player.damageCount = 0;
	g_Player.max_damage = 6;
	g_Player.O2bottle = 0;

	g_PlayerStatus = 0;	// の初期化

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayerStatus(void)
{
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayerStatus(void)
{
	if (g_Player.damageCount >= 6){			// ダメージを6回受けたら
		g_TexNo = 6;
	}
	else {
		g_TexNo = g_Player.damageCount;		// ダメージの回数に応じて対応したテクスチャを表示

	}
#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayerStatus(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	int number = g_PlayerStatus;

	// 描画の設定処理
	for (int i = 0; i < Player_Status_DIGIT; i++)
	{
		// 今回表示するHP
		float x = (float)(number % 10);

		// HPの位置やテクスチャー座標を反映
		float px = g_Pos.x + g_w * i;	// HPの表示位置X
		float py = g_Pos.y;				// HPの表示位置Y

		float pw = g_w;					// HPの表示幅
		float ph = g_h;					// HPの表示高さ

		float tw = 1.0f;				// テクスチャの幅
		float th = 1.0f;				// テクスチャの高さ
		float tx = x * tw;				// テクスチャの左上X座標
		float ty = 0.0f;				// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

	}
}


////=============================================================================
//// HPを加算する
//// 引数:add :追加する点数。マイナスも可能
////=============================================================================
//void AddPlayerStatus(int add)
//{
//	g_PlayerStatus += add;
//	if (g_PlayerStatus > SCORE_MAX)
//	{
//		g_PlayerStatus = SCORE_MAX;
//	}
//
//}


int GetPlayerStatus(void)
{
	return g_PlayerStatus;
}


void SetPlayerStatus(int hpO2)
{
	g_PlayerStatus = hpO2;
}

