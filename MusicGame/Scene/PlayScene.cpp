#include <fstream>
#include <sstream>

#include "../pch.h"
#include "d3d11.h"
#include "SimpleMath.h"
#include "WICTextureLoader.h"

#include "../System/DXTKManager.h"
#include "../Sound/ADX2Le.h"
#include "../Sound/CueSheet_0.h"

#pragma comment(lib, "cri_ware_pcx86_LE_import.lib")

#include "PlayScene.h"
#include "ResultScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

using namespace std;

// �L�[����p���X�g
const unordered_map<int, Keyboard::Keys> PlayScene::checkKeyList_ =
{
	{ 0, Keyboard::Keys::D },
	{ 1, Keyboard::Keys::F },
	{ 2, Keyboard::Keys::J },
	{ 3, Keyboard::Keys::K }
};

SceneBase* PlayScene::m_base = nullptr;

SceneBase* PlayScene::GetInstance()
{
	if (m_base == nullptr)
		m_base = new PlayScene();
	return m_base;
}

// ����������
void PlayScene::Initialize()
{

	// DXTK���Ǘ�����C���X�^���X���擾
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	spriteBatch_ = std::make_unique<SpriteBatch>(dxtk.m_context);
	spriteFont_ = std::make_unique<SpriteFont>(dxtk.m_device, L"myfile.spritefont");

	// �L�[�{�[�h�̏�����
	keyboard_ = dxtk.m_keyboard.get();

	// �G�t�F�N�g�t�@�N�g������
	factory_ = std::make_unique<EffectFactory>(dxtk.m_device);

	// �e�N�X�`���̓ǂݍ��݃p�X
	factory_->SetDirectory(L"Resources");

	// �T�E���h�̏�����
	ADX2Le::Initialize("Resources/SoundData/Sound_data.acf");
	ADX2Le::LoadAcb("Resources/SoundData/CueSheet_0.acb", "Resources/SoundData/CueSheet_0.awb");


	// NotesManager�N���X�̏�����
	notesManager_ = new NotesManager();
	notesManager_->Initialize();

	// Lane�N���X�̏�����
	lane_ = new Lane();
	lane_->Initialize();

	// BackGround�N���X�̏�����
	background_ = new BackGround();
	background_->Initialize();

	// ����̏�����
	nowJudge_ = judge::BLANK;

	// �J�E���g�̏�����
	fallCnt_ = 0;
	displayCnt_ = 0;

	// �^�C�}�[�̏�����
	gametimer_ = 0;
	bgmtimer_ = 0;

	// �t�F�[�h�p�t���O�̏�����
	fadeFlag_ = true;

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

	// �X�R�A�摜�̐؂�o�����W��������
	for (int i = 0; i < ALL_NUM; i++)
	{
		int offsetX = x + (w * i);
		rect[i] = { offsetX, y, offsetX + w, y + h };
	}

	// �X�R�A�ϐ��̏�����
	score_ = 0;

	// ���[���ԍ����L�^����ϐ��̏�����
	saveLaneNum_ = 0;

	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/Fade.png",
		nullptr, this->fadeTexture_.GetAddressOf());
	
	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/number.png",
		nullptr, this->scoreTexture_.GetAddressOf());

	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/Perfect.png",
		nullptr, this->perfectTexture_.GetAddressOf());

	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/Great.png",
		nullptr, this->greatTexture_.GetAddressOf());

	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/Good.png",
		nullptr, this->goodTexture_.GetAddressOf());

	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/Miss.png",
		nullptr, this->missTexture_.GetAddressOf());
	
	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/Star.png",
		nullptr, this->effectTexture_.GetAddressOf());

	// CSV�̓ǂݍ���
	Load_CSV();

	// ���y�̍Đ�
	ADX2Le::Play(CRI_CUESHEET_0_MUSIC1);

}

// �X�V����
void PlayScene::UpdateScene(SceneManager* scene)
{
	
	// NotesManager�N���X�̍X�V
	notesManager_->Update();

	auto kb = keyboard_->GetState();

	for (int i = 0; i < 4; i++)
	{
		if (kb.IsKeyDown(checkKeyList_.at(i)))
		{
			lane_->SetKeyPressd(i);
		}
	}

	// Lane�N���X�̍X�V
	lane_->Update();

	// BackGround�N���X�̍X�V
	background_->Update();

	// �����蔻����s��
	Notesjudge();

	// �X�^�[�N���X���X�V
	for(std::list<Star>::iterator itr =effectStar_.begin(); itr !=effectStar_.end(); itr++)
	{
		bool result = false;
		result = itr->Update();

		if (!result)
		{
			itr = effectStar_.erase(itr);

			if (itr == effectStar_.end())
			{
				break;
			}
		}

	}

	// �t�F�[�h�t���O������Ă�����
	if (fadeFlag_)
	{
		fadeCnt_++;

		// Color�𔒂��猳�̐F�֖߂��Ă���
		red_ -= 0.01f;
		green_ -= 0.01f;
		blue_ -= 0.01f;
		alpha_ -= 0.01f;
	}

	// �t�F�[�h���Ԃ��߂��Ă���A���A���t�@�l��0.9�ɂȂ��Ă�����
	if ((fadeCnt_ >= FADE_TIME) && (alpha_ >= 0.9f))
	{
		// �t�F�[�h�t���O�����낷
		fadeFlag_ = false;
	}

	if (gametimer_ >= BGM_TIME)
	{
		// Color��i�X���ɂ��Ă���
		red_ -= 0.01f;
		green_ -= 0.01f;
		blue_ -= 0.01f;
		alpha_ -= 0.01f;

		// ResultScene�ɐ؂�ւ��
		scene->ChangeScene(ResultScene::GetInstance());

		// ���y�̒�~
		ADX2Le::Stop();

	}

	gametimer_ += 1.0f / 60.0f;

}

// �`�揈��
void PlayScene::Render()
{
	// DXTK���Ǘ�����C���X�^���X���擾
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	// BackGround�N���X�̕`��
	background_->Render();

	// Lane�N���X�̕`��
	lane_->Render();

	// ���[���Ɍ���`�悷��
	lane_->DrawLightColor();

	// �X�^�[�̕`��
	for (std::list<Star>::iterator itr = effectStar_.begin(); itr != effectStar_.end(); itr++)
	{
		itr->Render();
	}

	// �t�F�[�h�p�e�N�X�`���̕`��
	dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
	dxtk.m_spriteBatch->Draw(fadeTexture_.Get(),
		Vector2(0.0f, 0.0f), Vector4(red_, green_, blue_, alpha_));
	dxtk.m_spriteBatch->End();

	// ����̃e�N�X�`����`��
	DrawJudgeTexture();

	// �X�R�A��`��
	DrawScore();

	// NotesManager�N���X�̕`��
	notesManager_->Render();

}

// PlayScene�N���X�̃I�u�W�F�N�g��j������
void PlayScene::Dispose()
{
	if (m_base != nullptr)
	{
		delete m_base;
		m_base = nullptr;
	}
}

// CSV�f�[�^��ǂݍ���
void PlayScene::Load_CSV()
{
	ifstream ifs("Resources/MusicData/MusicData.csv");

	string line;

	// �����f�[�^��ǂݍ���
	while (getline(ifs, line))
	{

		// �J���}���󔒂ɕϊ�
		std::replace(line.begin(), line.end(),',',' ');

		// 1�s���𕶎���X�g���[���ɕϊ�
		istringstream stream(line);

		MusicData LineNumber;

		// ���[���ԍ���n��
		stream >> LineNumber.LaneNumber;

		// �����������Ă��鎞�Ԃ�n��
		stream >> LineNumber.FallTime;

		// �m�[�c�̎�ނ�n��
		stream >> LineNumber.NotesNumber;

		// �z��Ƀ��[���ԍ��Ɖ����̗����Ă��鎞�Ԃ��i�[����
		musicdata_.push_back(LineNumber);

	}
}

// �X�R�A���v�Z���ĕ`�悷��֐�
void PlayScene::DrawScore()
{
	// ���� (�ő包��)
	int division_num = 1e+5;

	// ��̌�����`�悷��
	for (int i = SCORE_NUM - 1; i >= 0; i--)
	{
		// division_num���������̒l�����o��
		int num = score_  / division_num;

		// ���l�̕`��
		DrawNum(i, num);

		// �X�R�A�̕`�悵����������菜��
		if (num * division_num > 0)
			score_ %= num * division_num;
				
		// ��������炷
		division_num /= 10;
	}

}

// ����̃e�N�X�`����`�悷��֐�
void PlayScene::DrawJudgeTexture()
{
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	// ���肪MISS�̏ꍇ
	if ((notesManager_->GetNotesPosition() >= JUDGE_END_POSITION)||  (displayJudge_ == judge::MISS))
	{
		dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
		dxtk.m_spriteBatch->Draw(missTexture_.Get(),
			Vector2(280.0f, 200.0f), Vector4(1.0f, 1.0f, 1.0f, 0.8f));
		dxtk.m_spriteBatch->End();

	}

	// ���肪GOOD�̏ꍇ
	if (displayJudge_ == judge::GOOD)
	{
		dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
		dxtk.m_spriteBatch->Draw(goodTexture_.Get(),
			Vector2(280.0f, 200.0f), Vector4(1.0f, 1.0f, 1.0f, 0.8f));
		dxtk.m_spriteBatch->End();

	}

	// ���肪GREAT�̏ꍇ
	if (displayJudge_ == judge::GREAT) 
	{
		dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
		dxtk.m_spriteBatch->Draw(greatTexture_.Get(),
			Vector2(280.0f, 200.0f), Vector4(1.0f, 1.0f, 1.0f, 0.8f));
		dxtk.m_spriteBatch->End();

	}

	// ���肪PERFECT�̏ꍇ
	if (displayJudge_ == judge::PERFECT)
	{
		dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
		dxtk.m_spriteBatch->Draw(perfectTexture_.Get(),
			Vector2(280.0f, 200.0f), Vector4(1.0f, 1.0f, 1.0f, 0.8f));
		dxtk.m_spriteBatch->End();
	}

	// ���肪BLANK�ȊO�Ȃ�
	if (nowJudge_ != judge::BLANK)
	{
		displayCnt_ = 0;

	}

	// �\�����Ԃ��I��������
	if (displayCnt_ == 0)
	{
		// ����\���̈ꎞ�ێ�
		displayJudge_ = nowJudge_;


	}

	// �\���J�E���g��2�b�o�߂�����
	if (displayCnt_ >= DISPLAY_TIME)
	{
		// �\���ϐ���BLANK�ɂ���
		displayJudge_ = judge::BLANK;
	}

	// �J�E���g��i�߂�
	displayCnt_++;

}

// �X�R�A�̒l�����`�悷��֐�
void PlayScene::DrawNum(int digit, int num)
{

	// �摜�̂��ꕝ
	Vector2 offset(-50.0f, 0.0f);
	Vector2 anchorPoint(730.0f, 0.0f);

	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();
	dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
	dxtk.m_spriteBatch->Draw(scoreTexture_.Get(),
		anchorPoint + (offset * digit),
		&rect[num],
		Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, Vector2(0.0f, 0.0f), 0.7f);
	dxtk.m_spriteBatch->End();
}

// �m�[�c�̓����蔻����s��
void PlayScene::Notesjudge()
{

	auto kb = keyboard_->GetState();

	// �m�[�c�����Ԗڂ����擾����
	int notesNums = 0;
	notesNums = notesManager_->GetNotesNum();

	if (notesNums > 0)
	{	

		// ���[���ԍ����擾����
		int laneNumber = musicdata_[notesNums - 1].LaneNumber;

		// �m�[�c�̎�ނ��擾����
		int notesType = musicdata_[notesNums - 1].NotesNumber;

		// ���������������
		nowJudge_ = BLANK;

		// �L�[�̔���
		if (kb.IsKeyDown(checkKeyList_.at(laneNumber)))
		{
			// ������s��
			nowJudge_ = notesManager_->PlessButton_Check(laneNumber, notesType);

			if (nowJudge_ != BLANK || nowJudge_ != MISS)
			{
				// �X�^�[�N���X���i�[
				Star star = Star();
				star.Initialize(effectTexture_.Get(),laneNumber,nowJudge_);
				effectStar_.push_back(star);
			}

			// ���[���ԍ����L�^����
			saveLaneNum_ = laneNumber;

			// ���[���t���O�𗧂Ă�
			lane_->SetLaneFlag(true);

		}
		else
			// ���[���t���O���~�낷
			lane_->SetLaneFlag(false);

			// �X�R�A���擾����
			score_ = m_base->GetScore();		

	}
}