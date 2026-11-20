#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class ItemInfoManager : public BaseManager {
	public:
		inline static ItemInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<ItemInfoManager>();
		}
	};
}
