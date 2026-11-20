#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class ActionPointInfoManager : public BaseManager {
	public:
		inline static ActionPointInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<ActionPointInfoManager>();
		}
	};
}
