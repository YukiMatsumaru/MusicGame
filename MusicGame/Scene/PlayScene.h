#pragma once

#include <VertexTypes.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <vector>
#include <SpriteFont.h>
#include <unordered_map>


#include "../System/NotesManager.h"
#include "../System/Lane.h"
#include "../System/Star.h"
#include "../System/BackGround.h"
#include "../Resources/MusicData/MusicData.h"

class PlayScene :public SceneBase
{
private:
	static SceneBase* m_base;

	// �L�[����p���X�g
	static const std::unordered_map<int, DirectX::Keyboard::Keys> checkKeyList_;

	// �L�[�{�[�h
	DirectX::Keyboard* keyboard_;

	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch_;

	// �t�H���g
	std::unique_ptr<DirectX::SpriteFont> spriteFont_;

	// �G�t�F�N�g�t�@�N�g���[
	std::unique_ptr<DirectX::EffectFactory> factory_;

	// Fade�p�e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> fadeTexture_;

	// Score�p�e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> scoreTexture_;

	// effect�p�e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> effectTexture_;

	// ����\���p�e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> perfectTexture_;	
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> greatTexture_;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> goodTexture_;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> missTexture_;

	// �y���Ǘ��p�ϐ�
	std::vector<MusicData> musicdata_;

	// �m�[�c�}�l�[�W���[
	NotesManager* notesManager_;

	// ���[���N���X
	Lane* lane_;

	// �X�^�[�N���X
	std::list<Star> effectStar_;

	// �w�i�N���X
	BackGround* background_;

	// ����p�ϐ�
	judge nowJudge_;

	//����\���p�ϐ�
	judge displayJudge_;

	// �X�R�A�p�ϐ�
	RECT rect[ALL_NUM];

	int outputWidth_;
	int outputHeight_;

	// ����\�����������߂̃J�E���g�ϐ�
	int displayCnt_;

	// �m�[�c���~�点�邽�߂̃J�E���g�ϐ�
	int fallCnt_;

	// �摜�p�ϐ�
	int x, y, w, h;

	// �t�F�[�h�p�J�E���g
	int fadeCnt_;

	// �X�R�A�p�ϐ�
	int score_;

	// ���[���ԍ����L�^����ϐ�
	int saveLaneNum_;

	// ����\���^�C�}�[
	const int DISPLAY_TIME = 30;

	// �a�f�l�^�C�}�[
	float bgmtimer_;

	// �Q�[���^�C�}�[
	float gametimer_;

	// �t�F�[�h�pRGBA
	float red_;
	float green_;
	float blue_;
	float alpha_;

	// �t�F�[�h�p�t���O
	bool fadeFlag_;

private:
	// CSV�f�[�^��ǂݍ���
	void Load_CSV();

	// �X�R�A���v�Z���ĕ`�悷��֐�
	void  DrawScore();

	// ����̃e�N�X�`����`�悷��֐�
	void  DrawJudgeTexture();

	// �X�R�A�̒l�����`�悷��֐�
	void DrawNum(int digit, int num);

	// �����蔻����s��
	void Notesjudge();

public:
	static SceneBase* GetInstance();
	PlayScene() {};
	~PlayScene() {};

	// ������
	void Initialize() override;

	// �X�V����
	void UpdateScene(SceneManager* scene) override;

	// �`�揈��
	void Render() override;

	// �I�u�W�F�N�g��j������
	void Dispose() override;

	// �m�[�c�̔z��f�[�^���擾����
	const std::vector<MusicData>& GetNotesData() { return musicdata_; }

	// �^�C�}�[���擾����
	float GetTimer() { return gametimer_; }

};
