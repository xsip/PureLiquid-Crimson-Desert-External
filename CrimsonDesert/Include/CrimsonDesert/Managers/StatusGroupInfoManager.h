#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class StatusGroupInfoManager : public BaseManager {
	public:
		inline static StatusGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<StatusGroupInfoManager>();
		}
	};
}
