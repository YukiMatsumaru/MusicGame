#pragma once

#include <CommonStates.h>
#include <Effects.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <vector>
#include <SpriteFont.h>

#include "SceneManager.h"
#include "ResultScene.h"

class ResultScene :public SceneBase
{
private:
	static SceneBase* m_base;

	// �L�[�{�[�h
	DirectX::Keyboard* keyboard_;
	
	// �G�t�F�N�g�t�@�N�g���[
	std::unique_ptr<DirectX::EffectFactory> factory_;

	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture_;

	// Score�p�e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> scoreTexture_;

	// �t�H���g
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch_;
	std::unique_ptr<DirectX::SpriteFont> spriteFont_;


	// �t�F�[�h�pRGBA
	float red_;
	float green_;
	float blue_;
	float alpha_;

	// �t�F�[�h�p�t���O
	bool fadeFlag_;

	// �t�F�[�h�p�J�E���g
	int fadeCnt_;

	// �X�R�A
	int score_;

	// �摜�p�ϐ�
	int x, y, w, h;

	// �X�R�A�p�ϐ�
	RECT rect[ALL_NUM];

public:
	static SceneBase* GetInstance();

	ResultScene();
	~ResultScene();

	// ������
	void Initialize() override;

	// �X�V����
	void UpdateScene(SceneManager* scene) override;

	// �`�揈��
	void Render() override;

	// �X�R�A���v�Z���ĕ`�悷��֐�
	void  DrawScore();

	// �X�R�A�̒l�����`�悷��֐�
	void DrawNum(int digit, int num);

	//�I�u�W�F�N�g��j������
	void Dispose() override;

};