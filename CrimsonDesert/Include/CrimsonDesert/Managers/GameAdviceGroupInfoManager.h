#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class GameAdviceGroupInfoManager : public BaseManager {
	public:
		inline static GameAdviceGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<GameAdviceGroupInfoManager>();
		}
	};
}
