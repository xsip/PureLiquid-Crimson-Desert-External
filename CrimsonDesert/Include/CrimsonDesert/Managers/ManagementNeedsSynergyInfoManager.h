#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class ManagementNeedsSynergyInfoManager : public BaseManager {
	public:
		inline static ManagementNeedsSynergyInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<ManagementNeedsSynergyInfoManager>();
		}
	};
}
