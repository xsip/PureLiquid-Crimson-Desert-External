#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class ActionRestrictionOrderInfoManager : public BaseManager {
	public:
		inline static ActionRestrictionOrderInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<ActionRestrictionOrderInfoManager>();
		}
	};
}
