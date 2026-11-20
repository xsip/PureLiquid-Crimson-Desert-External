#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class ActivityPresetInfoManager : public BaseManager {
	public:
		inline static ActivityPresetInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<ActivityPresetInfoManager>();
		}
	};
}
