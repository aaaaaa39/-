//=============================================================================
//
// UI処理 [ui.cpp (レティクル)]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "ui.h"
#include "player.h"
#include "sound.h"
#include "sprite.h"
#include "bullet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(130)	// 横幅
#define TEXTURE_HEIGHT				(TEXTURE_WIDTH*0.8)	// 縦幅
#define TEXTURE_MAX					(7)		// テクスチャの数
#define cnt_max						(5)		

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/Crosshair_03.png",
	"data/TEXTURE/Crosshair_04.png",
	"data/TEXTURE/Crosshair_08.png",
	"data/TEXTURE/Crosshair_21.png",
	"data/TEXTURE/Crosshair_23.png",
	"data/TEXTURE/Crosshair_25.png",

};

static tex g_tex[TEXTURE_MAX];
BOOL flag45;
BOOL display[TEXTURE_MAX];
int cnt;

static MODE g_mode;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitUi(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
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

	for (int i = 0; i < TEXTURE_MAX; i++) {
		g_tex[i].Use = TRUE;
		display[i] = FALSE;
	}

	g_tex[0].TexNo = 0;
	g_tex[1].TexNo = 1;
	g_tex[2].TexNo = 2;
	g_tex[3].TexNo = 3;
	g_tex[4].TexNo = 4;
	g_tex[5].TexNo = 5;
	g_tex[6].TexNo = 5;

	g_tex[0].w = 30;
	g_tex[0].h = 350;
	g_tex[0].Pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f };

	g_tex[1].w = 30;
	g_tex[1].h = 350;
	g_tex[1].Pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f };

	g_tex[2].w = TEXTURE_WIDTH;
	g_tex[2].h = TEXTURE_HEIGHT;
	g_tex[2].Pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f };


	g_tex[3].w = 30;
	g_tex[3].h = 30;
	g_tex[3].Pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f };

	g_tex[4].w = 60;
	g_tex[4].h = 70;
	g_tex[4].Pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f };

	g_tex[5].w = 60;
	g_tex[5].h = 70;
	g_tex[5].Pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f };

	g_tex[6].w = 60;
	g_tex[6].h = 70;
	g_tex[6].Pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f };


	flag45 = FALSE;
	display[2] = TRUE;
	g_mode.cnt = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitUi(void)
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
void UpdateUi(void)
{
	PLAYER* player = GetPlayer();

	float angle = (XM_PI / 30) * 30;

	if (GetKeyboardTrigger(DIK_RETURN)) {
		if (g_mode.use == 0) {
			flag45 = TRUE;
			player->ammo -= 0.1f;
			PlaySound(SOUND_LABEL_SE_laser000);
			if (player->ammo <= 0) {
				g_mode.cnt++;
			}
		}
		else if (g_mode.use == 1) {
			flag45 = TRUE;
			player->ammo -= 0.1f;
			PlaySound(SOUND_LABEL_SE_laser000);
			g_mode.broken_cnt++;
		}
		else {
			flag45 = TRUE;
			PlaySound(SOUND_LABEL_SE_laser000);
		}
	}


	if (flag45 == TRUE) {
		float angle = (2 * XM_PI / cnt_max) * cnt;
		float move = 15 * sinf(angle);
		if (player->ammo <= 0) {

			g_tex[0].Pos.x += move;
			g_tex[1].Pos.x += move;
			g_tex[2].Pos.x += move;
			g_tex[4].Pos.x += move;
			g_tex[5].Pos.x += move;
			g_tex[6].Pos.x += move;
		}
		else {
			g_tex[2].Pos.y += move;
		}
		cnt++;

		if (cnt > cnt_max) {
			flag45 = FALSE;
			cnt = 0;
		}
	}

	if (g_mode.cnt >= 10) {
		g_mode.use = 1;
	}
	else {
		g_mode.use = 0;
	}

	if (g_mode.broken_cnt >= 10) {
		g_mode.use = 2;

	}

	//UI表示
	if (GetKeyboardTrigger(DIK_F2)) {
		display[0] = display[0] ? FALSE : TRUE;
		display[1] = display[1] ? FALSE : TRUE;
		display[2] = display[2] ? FALSE : TRUE;
		display[4] = display[4] ? FALSE : TRUE;
		display[5] = display[5] ? FALSE : TRUE;
		display[6] = display[6] ? FALSE : TRUE;
	}



#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawUi(void)
{
	PLAYER* player = GetPlayer();
	for (int i = 0; i < TEXTURE_MAX; i++) {
		{
			if (display[i] == FALSE)continue;

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
			if (i == 1) {
				material.Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
			}
			else if ((i == 0 || i == 4) && (g_mode.use == 1)) {
				material.Diffuse = XMFLOAT4(0.992f, 0.827f, 0.188f, 1.0f);
			}

			else {
				material.Diffuse = XMFLOAT4(1.0f,1.0f, 1.0f, 1.0f);
			}
			SetMaterial(material);


			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_tex[i].TexNo]);

			float px = g_tex[i].Pos.x;
			float py = g_tex[i].Pos.y;
			float pw = g_tex[i].w;
			float ph = g_tex[i].h;

			float tw = 1.0f;
			float th = 1.0f;
			float tx = 0.0f;
			float ty = 0.0f;

			if (i == 0 || i == 1) {
				if (i == 1) {
					ph = ph * player->ammo;
				}
				SetSpriteLeftTop(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);
			}
			else {
				// 1枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			}

			/*
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));*/

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

		}
	}
}


MODE* GetModeUi(void)
{
	return &g_mode;
}

