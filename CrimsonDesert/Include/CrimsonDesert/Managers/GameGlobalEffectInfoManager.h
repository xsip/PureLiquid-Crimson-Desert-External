#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class GameGlobalEffectInfoManager : public BaseManager {
	public:
		inline static GameGlobalEffectInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<GameGlobalEffectInfoManager>();
		}
	};
}
