#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class FactionWayPointInfoManager : public BaseManager {
	public:
		inline static FactionWayPointInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<FactionWayPointInfoManager>();
		}
	};
}
