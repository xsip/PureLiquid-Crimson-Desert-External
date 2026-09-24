#pragma once
#include <GlobalData/Include.h>
#include <LiquidHookEx/Include.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class ServerTransformSyncActorComponent {
	private:

		struct IsFallingFlagContainer {
			byte bIsFalling;
		};

		class ServerTransformSyncActorComponentInst {
		private:
			char pad[0x30];
		public:
			IsFallingFlagContainer* pIsFallingFlagContainer;
		};

		inline static ServerTransformSyncActorComponent* pInstAddr = 0x0;
		
		static void SetFallDamageEnabled(bool bEnabled);
	public:
		struct UpdateHookData : public LiquidHookEx::Detour::BaseHookData {
			ServerTransformSyncActorComponent* pInst;
			bool bFallDamageEnabled;
		};
	private:
		inline static LiquidHookEx::Detour m_Hook = LiquidHookEx::Detour("ServerTransformSyncActorComponent::Update");


		static __int64 __fastcall hkUpdate(ServerTransformSyncActorComponent::ServerTransformSyncActorComponentInst* a1, __int64* a2);
		static void hkUpdateEnd();


	public:
		PROPERTY(m_bIsFalling, bool, 0x30);
		static void DisableFallDamage();
		static void EnableFallDamage();
		static bool IsFalldamageEnabled();
		static bool HookUpdate();
		static bool Unhook();
		static ServerTransformSyncActorComponent* GetInstance();
	};
}