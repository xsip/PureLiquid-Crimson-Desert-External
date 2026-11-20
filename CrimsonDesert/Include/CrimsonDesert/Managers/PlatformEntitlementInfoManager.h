#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class PlatformEntitlementInfoManager : public BaseManager {
	public:
		inline static PlatformEntitlementInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<PlatformEntitlementInfoManager>();
		}
	};
}
