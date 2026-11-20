#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class ReserveSlotInfoManager : public BaseManager {
	public:
		inline static ReserveSlotInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<ReserveSlotInfoManager>();
		}
	};
}
