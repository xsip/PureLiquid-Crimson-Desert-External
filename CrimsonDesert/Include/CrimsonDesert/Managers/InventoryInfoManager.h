#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class InventoryInfoManager : public BaseManager {
	public:
		inline static InventoryInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<InventoryInfoManager>();
		}
	};
}
