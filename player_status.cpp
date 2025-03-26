//=============================================================================
//
// �v���C���[�X�e�[�^�X���� [playerstatus.cpp]
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
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(124)	// HP�T�C�Y
#define TEXTURE_HEIGHT				(32)	// 
#define TEXTURE_MAX					(2)		// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;							// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static PLAYER		g_Player;										// �v���C���[

static char* g_TextureName[] = {
	"data/TEXTURE/HP Bar0.png",		// HPMAX
	"data/TEXTURE/HP Bar1.png",
	"data/TEXTURE/HP Bar2.png",
	"data/TEXTURE/HP Bar3.png",
	"data/TEXTURE/HP Bar4.png",
	"data/TEXTURE/HP Bar5.png",
	"data/TEXTURE/HP Bar6.png",		// HP0

};


static bool						g_Use;						// true:�g���Ă���  false:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

static int						g_PlayerStatus;				// HP �_�f�Q�[�W�Ȃ�

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayerStatus(void)
{
	ID3D11Device* pDevice = GetDevice();

	//�e�N�X�`������
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


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �v���C���[�̃X�e�[�^�XUI�̏�����
	g_Use = TRUE;
	g_w = TEXTURE_WIDTH;
	g_h = TEXTURE_HEIGHT;
	g_Pos = { 100.0f, 20.0f, 0.0f };
	g_TexNo = 0;

	g_Player.damageCount = 0;
	g_Player.max_damage = 6;
	g_Player.O2bottle = 0;

	g_PlayerStatus = 0;	// �̏�����

	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
//=============================================================================
void UpdatePlayerStatus(void)
{
	if (g_Player.damageCount >= 6){			// �_���[�W��6��󂯂���
		g_TexNo = 6;
	}
	else {
		g_TexNo = g_Player.damageCount;		// �_���[�W�̉񐔂ɉ����đΉ������e�N�X�`����\��

	}
#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayerStatus(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	int number = g_PlayerStatus;

	// �`��̐ݒ菈��
	for (int i = 0; i < Player_Status_DIGIT; i++)
	{
		// ����\������HP
		float x = (float)(number % 10);

		// HP�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_Pos.x + g_w * i;	// HP�̕\���ʒuX
		float py = g_Pos.y;				// HP�̕\���ʒuY

		float pw = g_w;					// HP�̕\����
		float ph = g_h;					// HP�̕\������

		float tw = 1.0f;				// �e�N�X�`���̕�
		float th = 1.0f;				// �e�N�X�`���̍���
		float tx = x * tw;				// �e�N�X�`���̍���X���W
		float ty = 0.0f;				// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

	}
}


////=============================================================================
//// HP�����Z����
//// ����:add :�ǉ�����_���B�}�C�i�X���\
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

