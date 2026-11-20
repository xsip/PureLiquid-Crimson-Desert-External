#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class AutoSpawnFilterInfoManager : public BaseManager {
	public:
		inline static AutoSpawnFilterInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<AutoSpawnFilterInfoManager>();
		}
	};
}
