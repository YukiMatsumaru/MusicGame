#include "../pch.h"
#include "d3d11.h"
#include "SimpleMath.h"
#include "WICTextureLoader.h"

#include "../System/DXTKManager.h"

#include "ResultScene.h"
#include "TitleScene.h"
#include "PlayScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

using namespace std;

SceneBase* ResultScene::m_base = nullptr;

SceneBase * ResultScene::GetInstance()
{
	if (m_base == nullptr)
		m_base = new ResultScene();
	return m_base;
}

// �R���X�g���N�^
ResultScene::ResultScene()
{
	
}

// �f�X�g���N�^
ResultScene::~ResultScene()
{

}

// ����������
void ResultScene::Initialize()
{
	// �L�[�{�[�h�̏�����
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	keyboard_ = dxtk.m_keyboard.get();

	// �G�t�F�N�g�t�@�N�g������
	factory_ = std::make_unique<EffectFactory>(dxtk.m_device);

	// �e�N�X�`���̓ǂݍ��݃p�X
	factory_->SetDirectory(L"Resources");

	spriteBatch_ = std::make_unique<SpriteBatch>(dxtk.m_context);
	spriteFont_ = std::make_unique<SpriteFont>(dxtk.m_device, L"myfile.spritefont");

	// �t�F�[�h�p�t���O�̏�����
	fadeFlag_ = false;

	// �t�F�[�h�p�J�E���g�̏�����
	fadeCnt_ = 0;

	// �t�F�[�h�pRGBA�̏�����
	red_ = 1.0f;
	green_ = 1.0f;
	blue_ = 1.0f;
	alpha_ = 1.0f;

	// �摜�p�ϐ��̏�����
	x = 0;
	y = 0;
	w = TEXTURE_POSITION;
	h = TEXTURE_POSITION;

	// �X�R�A�̏�����
	int score = 0;

	// �X�R�A�摜�̐؂�o�����W��������
	for (int i = 0; i < ALL_NUM; i++)
	{
		int offsetX = x + (w * i);
		rect[i] = { offsetX, y, offsetX + w, y + h };
	}

	// �e�N�X�`���̃��[�h
	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/Fade.png",
		nullptr, this->texture_.GetAddressOf());

	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/number.png",
		nullptr, this->scoreTexture_.GetAddressOf());


}

// �X�V����
void ResultScene::UpdateScene(SceneManager* scene)
{
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();
	dxtk.m_keyTracker->Update(dxtk.m_keyboard.get()->GetState());

	if (dxtk.m_keyTracker->IsKeyPressed(Keyboard::Keys::K))
	{
		// �X�R�A�̏�����
		m_base->SetScore(0);

		// TitleScene�ɐ؂�ւ��
		scene->ChangeScene(TitleScene::GetInstance());
	}

	if (dxtk.m_keyTracker->IsKeyPressed(Keyboard::Keys::D))
	{
		// �X�R�A�̏�����
		m_base->SetScore(0);

		// PlayScene�ɐ؂�ւ��
		scene->ChangeScene(PlayScene::GetInstance());

	}

	// �t�F�[�h�t���O������Ă�����
	if (!fadeFlag_)
	{
		fadeCnt_++;
		red_ -= 0.01f;
		green_ -= 0.01f;
		blue_ -= 0.01f;
		alpha_ += 0.01f;
	}
	
	if (fadeCnt_ >= FADE_TIME)
	{
		fadeFlag_ = true;
	}

}

// �`�揈��
void ResultScene::Render()
{
	// DXTK���Ǘ�����C���X�^���X���擾
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
	dxtk.m_spriteBatch->Draw(texture_.Get(),
		Vector2(0.0f, 0.0f), Vector4(red_, green_, blue_, alpha_));
	dxtk.m_spriteBatch->End();

	// �X�R�A�̕`��
	DrawScore();

	spriteBatch_->Begin();
	spriteFont_->DrawString(spriteBatch_.get(), L"D:PLAY AGAIN", XMFLOAT2(100, 400));
	spriteFont_->DrawString(spriteBatch_.get(), L"K:REUTURN TO TITLE", XMFLOAT2(500, 400));
	spriteFont_->DrawString(spriteBatch_.get(), L"PRESS SPACE KEY", XMFLOAT2(300, 320));
	spriteBatch_->End();

}

// �X�R�A���v�Z���ĕ`�悷��֐�
void ResultScene::DrawScore()
{
	// ���� (�ő包��)
	int division_num = 1e+5;

	// �X�R�A�̎擾
	score_ = m_base->GetScore();

	// ��̌�����`�悷��
	for (int i = SCORE_NUM - 1; i >= 0; i--)
	{
		// division_num���������̒l�����o��
		int num = score_ / division_num;

		// ���l�̕`��
		DrawNum(i, num);

		// �X�R�A�̕`�悵����������菜��
		if (num * division_num > 0)
			score_ %= num * division_num;

		// ��������炷
		division_num /= 10;
	}

}

// �X�R�A�̒l�����`�悷��֐�
void ResultScene::DrawNum(int digit, int num)
{

	// �摜�̂��ꕝ
	Vector2 offset(-50.0f, 0.0f);
	Vector2 anchorPoint(490.0f, 250.0f);

	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();
	dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
	dxtk.m_spriteBatch->Draw(scoreTexture_.Get(),
		anchorPoint + (offset * digit),
		&rect[num],
		Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, Vector2(0.0f, 0.0f), 0.7f);
	dxtk.m_spriteBatch->End();
}

// ResultScene�N���X�̃I�u�W�F�N�g��j������
void ResultScene::Dispose()
{
	if (m_base != nullptr)
	{
		delete m_base;
		m_base = nullptr;
	}
}
