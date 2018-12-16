#pragma once

#include <VertexTypes.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <SpriteFont.h>

#include "SceneManager.h"
#include "../System/Notes.h"
#include "../System/DXTKManager.h"

class TitleScene :public SceneBase
{
private:
	static SceneBase* m_base;

	// �G�t�F�N�g�t�@�N�g���[
	std::unique_ptr<DirectX::EffectFactory> factory_;

	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture_;

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

public:
	static SceneBase* GetInstance();
	 TitleScene();
	~TitleScene();

	//������
	void Initialize() override;

	//�X�V����
	void UpdateScene(SceneManager* scene) override;

	//�`�揈��
	void Render() override;

	//�I�u�W�F�N�g��j������
	void Dispose() override;

};

