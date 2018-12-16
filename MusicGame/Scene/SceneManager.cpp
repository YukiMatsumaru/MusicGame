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

//シーン変更
void SceneManager::ChangeScene(SceneBase* nextScene)
{
	// 一時的にスコアを管理する変数
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

//更新関数
void SceneManager::Update()
{
	m_base->UpdateScene(this);
}

//描画関数
void SceneManager::Render()
{
	
	m_base->Render();
}
