#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class FactionSpawnDataInfoManager : public BaseManager {
	public:
		inline static FactionSpawnDataInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<FactionSpawnDataInfoManager>();
		}
	};
}
