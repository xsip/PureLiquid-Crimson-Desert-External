#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class TriggerRegionInfoManager : public BaseManager {
	public:
		inline static TriggerRegionInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<TriggerRegionInfoManager>();
		}
	};
}
