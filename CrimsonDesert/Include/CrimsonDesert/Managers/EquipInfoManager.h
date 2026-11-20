#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class EquipInfoManager : public BaseManager {
	public:
		inline static EquipInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<EquipInfoManager>();
		}
	};
}
