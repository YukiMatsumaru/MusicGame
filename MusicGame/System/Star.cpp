#include "../pch.h"
#include "Star.h"

#include "WICTextureLoader.h"
#include "../System/DXTKManager.h"

#include "../Scene/PlayScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

using namespace std;

// Star�N���X�̏���������
void Star::Initialize(ID3D11ShaderResourceView* starTexture,int laneNum, judge judge)
{
	// DXTK���Ǘ�����C���X�^���X���擾
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	// Star��RGBA�̏�����
	switch (judge)
	{
	case PERFECT:
		red_ = 0.0f;
		green_ = 0.5f;
		blue_ = 0.5f;
		alpha_ = 1.0f;
		break;
	case GREAT:
		red_ = 1.0f;
		green_ = 1.0f;
		blue_ = 0.0f;
		alpha_ = 1.0f;
		break;
	case GOOD:
		red_ = 1.0f;
		green_ = 0.4f;
		blue_ = 0.8f;
		alpha_ = 1.0f;
		break;
	default:
		break;
	}

	// Star�̑傫���̏�����
	starScale_ = 0.0f;
	
	// �e�N�X�`���̃��[�h
	starTexture_ = starTexture;

	// �L�[�����������[���̍��W
	pos_ = NOTES_POSITION[laneNum];

}

// Star�N���X�̍X�V����
bool Star::Update()
{	
	starScale_ += 0.1f;
	alpha_ -= 0.05f;

	if (alpha_ >= 0.0f)
	{
		return true;
	}

	return false;
}

// Star�N���X�̕`�揈��
void Star::Render()
{
	// DXTK���Ǘ�����C���X�^���X���擾
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	RECT rect = {0,0,150,150};

	// Star�e�N�X�`���̕`��
	dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
	dxtk.m_spriteBatch->Draw(starTexture_,
		Vector2(pos_ - 50.0f + 75.0f, -5.0f + 500.0f),
		&rect,
		Vector4(red_, green_, blue_, alpha_), 0.0f, Vector2(75, 75), Vector2(starScale_));
	dxtk.m_spriteBatch->End();

}
