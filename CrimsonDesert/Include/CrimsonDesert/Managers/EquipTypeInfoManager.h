#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class EquipTypeInfoManager : public BaseManager {
	public:
		inline static EquipTypeInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<EquipTypeInfoManager>();
		}
	};
}
