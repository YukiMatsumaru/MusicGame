#include "../pch.h"
#include "BackGround.h"

#include "WICTextureLoader.h"
#include "../System/DXTKManager.h"

#include "../Scene/PlayScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

using namespace std;

// BackGround�N���X�̏���������
void BackGround::Initialize()
{

	// DXTK���Ǘ�����C���X�^���X���擾
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	// �e�N�X�`���̃��[�h
	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/BackEffect.png",
		nullptr, this->backEffectTexture_.GetAddressOf());

	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/back.png",
		nullptr, this->backGroundTexture_.GetAddressOf());

	// �G�t�F�N�g�̉�]���̏�����
	rotate_ = 0.0f;

	// �G�t�F�N�g��RGB�̏�����
	red_ = 1.0f;
	green_ = 1.0f;
	blue_ = 1.0f;

	// �G�t�F�N�g�̐F�ύX�p�ϐ��̏�����
	changeColor_ = 1.0f;

	// �G�t�F�N�g�̑傫���ύX�p�ϐ��̏�����
	scale_ = 1.0f;

	changeScale_ = 1.0f;

}

// BackGround�N���X�̍X�V����
void BackGround::Update()
{
	// �G�t�F�N�g�̉�]���𓮂���
	rotate_ += 0.01f;

	// �G�t�F�N�g�̐F��ύX����
	changeColor_ += 0.05f;
	
	red_ = 1.0f;
	green_ = sinf(changeColor_ / 4.0f);
	blue_ = sinf(changeColor_ / 2.0f);

	// �G�t�F�N�g�̑傫����ύX����
	changeScale_ += 0.05f;
	scale_ = sinf(changeScale_);

	// Scale��0�ȉ��ɂȂ�����
	if (scale_ <= 0)
	{
		// �����I�ɑ傫����1�ɂ���
		scale_ = 1.0f;
		changeScale_ = 1.0f;
	}
}

// BackGround�N���X�̕`�揈��
void BackGround::Render()
{
	// DXTK���Ǘ�����C���X�^���X���擾
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	// �w�i�̕`��
	dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
	dxtk.m_spriteBatch->Draw(backGroundTexture_.Get(),
		Vector2(0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	dxtk.m_spriteBatch->End();

	RECT rect = { 0,0,800,600 };

	// �w�i�G�t�F�N�g�p�e�N�X�`���̕`��
	dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
	dxtk.m_spriteBatch->Draw(backEffectTexture_.Get(),
		Vector2(400.0f, 300.0f), &rect, Vector4(red_, green_, blue_, 0.5f), rotate_, Vector2(400, 300),Vector2(scale_));
	dxtk.m_spriteBatch->End();
}
