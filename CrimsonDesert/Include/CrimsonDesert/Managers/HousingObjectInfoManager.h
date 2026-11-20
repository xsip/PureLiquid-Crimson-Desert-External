#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class HousingObjectInfoManager : public BaseManager {
	public:
		inline static HousingObjectInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<HousingObjectInfoManager>();
		}
	};
}
