#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class HouseInfoManager : public BaseManager {
	public:
		inline static HouseInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<HouseInfoManager>();
		}
	};
}
