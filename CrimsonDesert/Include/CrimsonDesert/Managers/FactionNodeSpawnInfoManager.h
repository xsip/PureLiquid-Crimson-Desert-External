#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class FactionNodeSpawnInfoManager : public BaseManager {
	public:
		inline static FactionNodeSpawnInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<FactionNodeSpawnInfoManager>();
		}
	};
}
