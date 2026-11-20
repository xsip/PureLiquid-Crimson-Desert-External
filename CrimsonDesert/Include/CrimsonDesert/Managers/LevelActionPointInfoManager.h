#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class LevelActionPointInfoManager : public BaseManager {
	public:
		inline static LevelActionPointInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<LevelActionPointInfoManager>();
		}
	};
}
