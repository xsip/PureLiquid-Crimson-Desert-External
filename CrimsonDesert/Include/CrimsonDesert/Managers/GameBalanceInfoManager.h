#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class GameBalanceInfoManager : public BaseManager {
	public:
		inline static GameBalanceInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<GameBalanceInfoManager>();
		}
	};
}
