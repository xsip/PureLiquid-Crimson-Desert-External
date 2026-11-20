#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class QuickSlotInfoManager : public BaseManager {
	public:
		inline static QuickSlotInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<QuickSlotInfoManager>();
		}
	};
}
