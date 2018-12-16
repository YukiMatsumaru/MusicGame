#include "../pch.h"
#include "d3d11.h"
#include "SimpleMath.h"
#include "WICTextureLoader.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "../System/DXTKManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SceneBase* TitleScene::m_base = nullptr;

//TitleScene�N���X�̃C���X�^���X���擾����
SceneBase* TitleScene::GetInstance()
{
	if (m_base == nullptr)
		m_base = new TitleScene();
	return m_base;
}

//�R���X�g���N�^
TitleScene::TitleScene()
{

}

//�f�X�g���N�^
TitleScene::~TitleScene()
{

}

//TitleScene�N���X�̏������֐�
void TitleScene::Initialize()
{
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	// �G�t�F�N�g�t�@�N�g������
	factory_ = std::make_unique<EffectFactory>(dxtk.m_device);

	// �e�N�X�`���̓ǂݍ��݃p�X
	factory_->SetDirectory(L"Resources");

	spriteBatch_ = std::make_unique<SpriteBatch>(dxtk.m_context);
	spriteFont_ = std::make_unique<SpriteFont>(dxtk.m_device, L"myfile.spritefont");

	// �e�N�X�`���̃��[�h
	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/Fade.png",
		nullptr, this->texture_.GetAddressOf());

	// �t�F�[�h�pRGBA�̏�����
	red_ = 0.0f;
	green_ = 0.0f;
	blue_ = 0.0f;
	alpha_ = 1.0f;

	// �t�F�[�h�p�t���O�̏�����
	fadeFlag_ = false;

	// �t�F�[�h�p�J�E���g�̏�����
	fadeCnt_ = 0;
}

//TitleScene�N���X�̍X�V�֐�
void TitleScene::UpdateScene(SceneManager* scene)
{
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	dxtk.m_keyTracker->Update(dxtk.m_keyboard.get()->GetState());

	if (dxtk.m_keyTracker->IsKeyPressed(Keyboard::Keys::Space))
	{
		// �t�F�[�h�t���O�𗧂Ă�
		fadeFlag_ = true;
	}

	// �t�F�[�h�t���O�������Ă�����
	if (fadeFlag_)
	{
		fadeCnt_++;
		red_ += 0.01f;
		green_ += 0.01f;
		blue_ += 0.01f;
		alpha_ -= 0.001f;
	}

	if (fadeCnt_ >= FADE_TIME)
	{
		//PlayScene�ɐ؂�ւ��
		scene->ChangeScene(PlayScene::GetInstance());
	}

}

//TitleScene�N���X�̕`��֐�
void TitleScene::Render()
{

	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
	dxtk.m_spriteBatch->Draw(texture_.Get(),
		Vector2(0.0f, 0.0f), Vector4(red_, green_, blue_, alpha_));
	dxtk.m_spriteBatch->End();

	spriteBatch_->Begin();
	spriteFont_->DrawString(spriteBatch_.get(), L"MUSIC GAME", XMFLOAT2(315, 280));
	spriteFont_->DrawString(spriteBatch_.get(), L"PRESS SPACE KEY", XMFLOAT2(300, 320));
	spriteBatch_->End();
}

//TitleScene�I�u�W�F�N�g��j������
void TitleScene::Dispose()
{
	if (m_base != nullptr)
	{
		delete m_base;
		m_base = nullptr;
	}
}
