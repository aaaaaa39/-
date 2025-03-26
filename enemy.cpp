//=============================================================================
//
// エネミーモデル処理 [enemy.cpp]
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
// マクロ定義
//*****************************************************************************
// #define		MODEL_ENEMY			"data/MODEL/shark_001.obj"			// 読み込むモデル名
#define	MODEL_ENEMY_BODY			"data/MODEL/shark_body_002.obj"		// 読み込むモデル名

#define	MODEL_ENEMY_HEAD			"data/MODEL/shark_head_002.obj"		// 読み込むモデル名
#define	MODEL_ENEMY_TAIL			"data/MODEL/shark_tail_002.obj"		// 読み込むモデル名

#define	VALUE_MOVE			(5.0f)						// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// 回転量

#define ENEMY_SHADOW_SIZE	(0.4f)						// 影の大きさ
#define ENEMY_OFFSET_Y		(7.0f)						// エネミーの足元をあわせる

#define ENEMY_PARTS_MAX		(2)							// エネミーのパーツの数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ENEMY			g_Enemy[MAX_ENEMY];						// エネミー

static ENEMY			g_Parts[MAX_ENEMY][ENEMY_PARTS_MAX];		// エネミーのパーツ用

//int g_Enemy_load = 0;

// エネミーの階層アニメーションデータ
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

// エネミーの線形補間データ
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

// サメの線形補間
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

// エネミーの線形補間データ
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
// 初期化処理
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
		g_Enemy[i].spd = 0.0f;			// 移動スピードクリア
		g_Enemy[i].use = TRUE;			// TRUE:生きてる
		g_Enemy[i].size = ENEMY_SIZE;	// 当たり判定の大きさ
		g_Enemy[i].Blinking = FALSE;	// 点滅処理の初期化
		g_Enemy[i].BlinkTimer = FALSE;	// 点滅経過時間の初期化
		g_Enemy[i].BlinkCnt = 0;		// 点滅回数の初期化

		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Enemy[i].model, &g_Enemy[i].diffuse[0]);

		// ここでエネミー用の影を作成している
		//XMFLOAT3 pos = g_Enemy[i].pos;
		//pos.y -= (ENEMY_OFFSET_Y - 0.1f);
		//g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);

		g_Enemy[i].time = 0.0f;			// 線形補間用のタイマーをクリア
		g_Enemy[i].tblNo = 0;			// 再生する行動データテーブルNoをセット
		g_Enemy[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

		// 階層アニメーション用の初期化処理
		g_Enemy[i].parent = NULL;			// 本体（親）なのでNULLを入れる


		// パーツの初期化
		for (int j = 0; j < ENEMY_PARTS_MAX; j++)
		{
			g_Parts[i][j].use = TRUE;

			// 位置・回転・スケールの初期設定
			g_Parts[i][j].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Parts[i][j].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Parts[i][j].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

			// 親子関係
			g_Parts[i][j].parent = &g_Enemy[i];		// ← ここに親のアドレスを入れる
			//	g_Parts[腕].parent= &g_Player;		// 腕だったら親は本体（プレイヤー）
			//	g_Parts[手].parent= &g_Paerts[腕];	// 指が腕の子供だった場合の例

			// 階層アニメーション用のメンバー変数の初期化
			g_Parts[i][j].time = 0.0f;			// 線形補間用のタイマーをクリア
			g_Parts[i][j].tblNo = 0;			// 再生する行動データテーブルNoをセット
			g_Parts[i][j].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

			// パーツの読み込みはまだしていない
			g_Parts[i][j].load = TRUE;
		}


		g_Parts[i][0].use = TRUE;
		g_Parts[i][0].parent = &g_Enemy[i];	// 親をセット
		g_Parts[i][0].tblNo = 0;			// 再生するアニメデータの先頭アドレスをセット
		g_Parts[i][0].tblMax = sizeof(MoveTbl_head) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
		g_Parts[i][0].load = TRUE;
		LoadModel(MODEL_ENEMY_HEAD, &g_Parts[i][0].model);

		g_Parts[i][1].use = TRUE;
		g_Parts[i][1].parent = &g_Enemy[i];	// 親をセット
		g_Parts[i][1].tblNo = 1;			// 再生するアニメデータの先頭アドレスをセット
		g_Parts[i][1].tblMax = sizeof(MoveTbl_tail) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット
		g_Parts[i][1].load = TRUE;
		LoadModel(MODEL_ENEMY_TAIL, &g_Parts[i][1].model);
	}

	g_Enemy[0].pos = XMFLOAT3(100.0f, 7.0f, 100.0f);
	g_Enemy[1].pos = XMFLOAT3(130.0f, 7.0f, -40.0f);
	g_Enemy[2].pos = XMFLOAT3(160.0f, 7.0f, -20.0f);
	g_Enemy[3].pos = XMFLOAT3(190.0f, 7.0f, -40.0f);
	g_Enemy[4].pos = XMFLOAT3(220.0f, 7.0f, -20.0f);
	g_Enemy[5].pos = XMFLOAT3(250.0f, 7.0f, -40.0f);

	// 0番だけ線形補間で動かしてみる
	g_Enemy[0].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[0].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[0].tblMax = sizeof(MoveTbl_Enemy_0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[1].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[1].tblNo = 1;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[1].tblMax = sizeof(MoveTbl_Enemy_1) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[2].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[2].tblNo = 2;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[2].tblMax = sizeof(MoveTbl_Enemy_2) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[3].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[3].tblNo = 3;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[3].tblMax = sizeof(MoveTbl_Enemy_3) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[4].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[4].tblNo = 4;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[4].tblMax = sizeof(MoveTbl_Enemy_3) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[5].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[5].tblNo = 5;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[5].tblMax = sizeof(MoveTbl_Enemy_3) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	// モデルの開放処理
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].load == TRUE)
		{
			UnloadModel(&g_Enemy[i].model); // モデルを解放
			g_Enemy[i].load = FALSE;       // ロード状態を更新
			g_Enemy[i].use = FALSE;		   // エネミーを開放
		}

	// パーツの開放処理
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
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// エネミーのワールドマトリックス計算（BODY）
		XMMATRIX mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		XMMATRIX mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y, g_Enemy[i].rot.z);
		XMMATRIX mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);

		XMMATRIX enemyWorldMatrix = mtxScl * mtxRot * mtxTranslate;
		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, enemyWorldMatrix);

		// 階層アニメーション
		// エネミーを動かく場合は、影も合わせて動かす事を忘れないようにね！
		for (int j = 0; j < ENEMY_PARTS_MAX; j++)
		{
			// このエネミーが使われているなら処理する
			if ((g_Parts[i][j].use == TRUE) && (g_Parts[i][j].tblMax > 0))
			{	// 線形補間の処理
				int nowNo = (int)g_Parts[i][j].time;		// 整数分であるテーブル番号を取り出している
				int maxNo = g_Parts[i][j].tblMax;			// 登録テーブル数を数えている
				int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
				INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Parts[i][j].tblNo];	// 行動テーブルのアドレスを取得

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

				float nowTime = g_Parts[i][j].time - nowNo;	// 時間部分である少数を取り出している

				Pos *= nowTime;								// 現在の移動量を計算している
				Rot *= nowTime;								// 現在の回転量を計算している
				Scl *= nowTime;								// 現在の拡大率を計算している

				// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
				XMStoreFloat3(&g_Parts[i][j].pos, nowPos + Pos);

				// 計算して求めた回転量を現在の移動テーブルに足している
				XMStoreFloat3(&g_Parts[i][j].rot, nowRot + Rot);

				// 計算して求めた拡大率を現在の移動テーブルに足している
				XMStoreFloat3(&g_Parts[i][j].scl, nowScl + Scl);

				// frameを使て時間経過処理をする
				g_Parts[i][j].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
				if ((int)g_Parts[i][j].time >= maxNo)			// 登録テーブル最後まで移動したか？
				{
					g_Parts[i][j].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
				}

			}
		}

		// エネミー全体の移動処理
		if (g_Enemy[i].tblMax > 0)	// 線形補間を実行する？
		{	// 線形補間の処理
			int nowNo = (int)g_Enemy[i].time;			// 整数分であるテーブル番号を取り出している
			int maxNo = g_Enemy[i].tblMax;				// 登録テーブル数を数えている
			int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
			INTERPOLATION_DATA* tbl = g_MoveTblAdr_Enemy[g_Enemy[i].tblNo];	// 行動テーブルのアドレスを取得

			XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
			XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
			XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

			XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
			XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
			XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

			float nowTime = g_Enemy[i].time - nowNo;	// 時間部分である少数を取り出している

			Pos *= nowTime;								// 現在の移動量を計算している
			Rot *= nowTime;								// 現在の回転量を計算している
			Scl *= nowTime;								// 現在の拡大率を計算している

			// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
			XMStoreFloat3(&g_Enemy[i].pos, nowPos + Pos);

			// 計算して求めた回転量を現在の移動テーブルに足している
			XMStoreFloat3(&g_Enemy[i].rot, nowRot + Rot);

			// 計算して求めた拡大率を現在の移動テーブルに足している
			XMStoreFloat3(&g_Enemy[i].scl, nowScl + Scl);

			// frameを使て時間経過処理をする
			g_Enemy[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
			if ((int)g_Enemy[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
			{
				g_Enemy[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
			}
		}
	}
//#ifdef _DEBUG
//
//	if (GetKeyboardTrigger(DIK_P))
//	{
//		// モデルの色を変更できるよ！半透明にもできるよ。
//		for (int j = 0; j < g_Enemy[i].model.SubsetNum; j++)
//		{
//			SetModelDiffuse(&g_Enemy[i].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
//		}
//	}
//
//	if (GetKeyboardTrigger(DIK_L))
//	{
//		// モデルの色を元に戻している
//		for (int j = 0; j < g_Enemy[i].model.SubsetNum; j++)
//		{
//			SetModelDiffuse(&g_Enemy[i].model, j, g_Enemy[i].diffuse[j]);
//		}
//	}
//#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);


	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use)
		{
											// 頭の座標					尻尾の座標
			XMFLOAT3 POS[ENEMY_PARTS_MAX] = { XMFLOAT3(0.0f,-1.0f,23.5f),XMFLOAT3(0.0f,0.0f,-29.0f) };
			XMFLOAT3 SCL[ENEMY_PARTS_MAX] = { XMFLOAT3(0.05f, 0.05f, 0.05f),XMFLOAT3(0.0f,0.0f,0.0f) };

			//SetFuchi(1);	// 1=true

			// まずは基本となるＢＯＤＹを描画する

			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);

			// モデル描画
			DrawModel(&g_Enemy[i].model);

			// 階層アニメーション
			for (int j = 0; j < ENEMY_PARTS_MAX; j++)
			{
				// ワールドマトリックスの初期化
				mtxWorld = XMMatrixIdentity();

				// スケールを反映
				mtxScl = XMMatrixScaling(g_Parts[i][j].scl.x + SCL[j].x, g_Parts[i][j].scl.y + SCL[j].y, g_Parts[i][j].scl.z + SCL[j].z);
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

				// 回転を反映
				mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i][j].rot.x, g_Parts[i][j].rot.y, g_Parts[i][j].rot.z);
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

				// 移動を反映
				mtxTranslate = XMMatrixTranslation(g_Parts[i][j].pos.x + POS[j].x, g_Parts[i][j].pos.y + POS[j].y, g_Parts[i][j].pos.z + POS[j].z);
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

				if (g_Parts[i][j].parent != NULL)	// 子供だったら親と結合する
				{
					mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i][j].parent->mtxWorld));
					// ↑
					// g_Enemy[i].mtxWorldを指している
				}

				XMStoreFloat4x4(&g_Parts[i][j].mtxWorld, mtxWorld);

				// 使われているなら処理する。ここまで処理している理由は他のパーツがこのパーツを参照している可能性があるから。
				if (g_Parts[i][j].use == FALSE) continue;

				// ワールドマトリックスの設定
				SetWorldMatrix(&mtxWorld);

				// モデル描画
				DrawModel(&g_Parts[i][j].model);
			}

		}
	}
	//SetFuchi(0);	// 0=false

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// エネミーの取得
//=============================================================================
ENEMY* GetEnemy()
{
	return &g_Enemy[0];
}
