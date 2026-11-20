#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class PartPrefabDyeSlotInfoManager : public BaseManager {
	public:
		inline static PartPrefabDyeSlotInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<PartPrefabDyeSlotInfoManager>();
		}
	};
}
