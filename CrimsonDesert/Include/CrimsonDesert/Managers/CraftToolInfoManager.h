#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class CraftToolInfoManager : public BaseManager {
	public:
		inline static CraftToolInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<CraftToolInfoManager>();
		}
	};
}
