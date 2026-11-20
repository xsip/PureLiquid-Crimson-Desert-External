#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class MercenaryGroupInfoManager : public BaseManager {
	public:
		inline static MercenaryGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<MercenaryGroupInfoManager>();
		}
	};
}
