#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class DyeColorGroupInfoManager : public BaseManager {
	public:
		inline static DyeColorGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<DyeColorGroupInfoManager>();
		}
	};
}
