#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class ItemGroupInfoManager : public BaseManager {
	public:
		inline static ItemGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<ItemGroupInfoManager>();
		}
	};
}
