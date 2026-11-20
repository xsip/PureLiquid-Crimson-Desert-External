#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class GlobalGameEventGroupInfoManager : public BaseManager {
	public:
		inline static GlobalGameEventGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<GlobalGameEventGroupInfoManager>();
		}
	};
}
