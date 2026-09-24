#pragma once
#include <GlobalData/Include.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class ServerTransformSyncActorComponent {
	private:

		inline static ServerTransformSyncActorComponent* pInstAddr = 0x0;
	public:
		PROPERTY(m_bIsFalling, bool, 0x30);
		static void DisableFallDamage();
		static ServerTransformSyncActorComponent* GetInstance();
	};
}