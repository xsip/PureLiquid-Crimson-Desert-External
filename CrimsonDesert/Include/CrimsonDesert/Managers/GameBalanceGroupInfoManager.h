#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class GameBalanceGroupInfoManager : public BaseManager {
	public:
		inline static GameBalanceGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<GameBalanceGroupInfoManager>();
		}
	};
}
