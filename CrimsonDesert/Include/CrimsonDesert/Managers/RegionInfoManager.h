#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class RegionInfoManager : public BaseManager {
	public:
		inline static RegionInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<RegionInfoManager>();
		}
	};
}
