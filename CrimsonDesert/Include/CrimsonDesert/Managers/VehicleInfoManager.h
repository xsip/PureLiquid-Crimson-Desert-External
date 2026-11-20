#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class VehicleInfoManager : public BaseManager {
	public:
		inline static VehicleInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<VehicleInfoManager>();
		}
	};
}
