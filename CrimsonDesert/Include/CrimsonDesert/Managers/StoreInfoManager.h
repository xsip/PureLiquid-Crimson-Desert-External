#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class StoreInfoManager : public BaseManager {
	public:
		inline static StoreInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<StoreInfoManager>();
		}
	};
}
