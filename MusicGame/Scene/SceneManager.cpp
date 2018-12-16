#include "../pch.h"
#include "SceneManager.h"
#include "d3d11.h"
#include "SimpleMath.h"
#include "WICTextureLoader.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SceneManager::SceneManager()
{
	m_base = nullptr;
}

SceneManager::~SceneManager()
{

}

//�V�[���ύX
void SceneManager::ChangeScene(SceneBase* nextScene)
{
	// �ꎞ�I�ɃX�R�A���Ǘ�����ϐ�
	int score = 0;

	if (m_base != nullptr)
	{
		score = m_base->GetScore();
		m_base->Dispose();
		m_base = nullptr;
	}

	m_base = nextScene;
	m_base->Initialize();
	m_base->SetScore(score);

}

//�X�V�֐�
void SceneManager::Update()
{
	m_base->UpdateScene(this);
}

//�`��֐�
void SceneManager::Render()
{
	
	m_base->Render();
}
