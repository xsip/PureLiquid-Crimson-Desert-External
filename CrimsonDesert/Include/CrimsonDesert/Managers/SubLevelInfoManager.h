#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class SubLevelInfoManager : public BaseManager {
	public:
		inline static SubLevelInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<SubLevelInfoManager>();
		}
	};
}
