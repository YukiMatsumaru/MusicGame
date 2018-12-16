#pragma once

#include "Singleton.h"

#include <Keyboard.h>
#include <SpriteFont.h>
#include <CommonStates.h>

namespace DXTK
{
	class DXTKManager : public Singleton<DXTKManager>
	{
	public:
		void Initializer(ID3D11Device* device, ID3D11DeviceContext* context);

		void UpdateInputState();

		// デバイス
		ID3D11Device* m_device;

		// コンテキスト
		ID3D11DeviceContext* m_context;

		// キーボード関係
		std::unique_ptr<DirectX::Keyboard> m_keyboard;
		std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyTracker;

		// コモンステート
		std::unique_ptr<DirectX::CommonStates> m_state;

		// スプライトバッチ
		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

		// スプライトフォント
		std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	private:
		friend class Singleton<DXTKManager>;

		DXTKManager();
	};
}

