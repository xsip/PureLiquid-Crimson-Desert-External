#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class NpcFriendlyInfoManager : public BaseManager {
	public:
		inline static NpcFriendlyInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<NpcFriendlyInfoManager>();
		}
	};
}
