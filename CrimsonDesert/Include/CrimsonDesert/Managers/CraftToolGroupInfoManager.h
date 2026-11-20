#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class CraftToolGroupInfoManager : public BaseManager {
	public:
		inline static CraftToolGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<CraftToolGroupInfoManager>();
		}
	};
}
