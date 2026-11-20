#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class AllyGroupInfoManager : public BaseManager {
	public:
		inline static AllyGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<AllyGroupInfoManager>();
		}
	};
}
