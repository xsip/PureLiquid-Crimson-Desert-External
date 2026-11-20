#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class FactionManagementInfoManager : public BaseManager {
	public:
		inline static FactionManagementInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<FactionManagementInfoManager>();
		}
	};
}
