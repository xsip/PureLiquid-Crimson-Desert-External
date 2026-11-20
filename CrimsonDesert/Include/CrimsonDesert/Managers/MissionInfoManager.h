#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class MissionInfoManager : public BaseManager {
	public:
		inline static MissionInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<MissionInfoManager>();
		}
	};
}
