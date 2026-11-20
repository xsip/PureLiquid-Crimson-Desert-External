#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class AIEventTableInfoManager : public BaseManager {
	public:
		inline static AIEventTableInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<AIEventTableInfoManager>();
		}
	};
}
