#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class GlobalGameEventInfoManager : public BaseManager {
	public:
		inline static GlobalGameEventInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<GlobalGameEventInfoManager>();
		}
	};
}
