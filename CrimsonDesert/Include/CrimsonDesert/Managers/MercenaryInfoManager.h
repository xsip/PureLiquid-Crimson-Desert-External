#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class MercenaryInfoManager : public BaseManager {
	public:
		inline static MercenaryInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<MercenaryInfoManager>();
		}
	};
}
