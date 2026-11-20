#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class GamePlayVariableInfoManager : public BaseManager {
	public:
		inline static GamePlayVariableInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<GamePlayVariableInfoManager>();
		}
	};
}
