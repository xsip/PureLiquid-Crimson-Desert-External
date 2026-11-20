#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class ConditionInfoManager : public BaseManager {
	public:
		inline static ConditionInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<ConditionInfoManager>();
		}
	};
}
