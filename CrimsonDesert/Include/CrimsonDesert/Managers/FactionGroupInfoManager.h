#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class FactionGroupInfoManager : public BaseManager {
	public:
		inline static FactionGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<FactionGroupInfoManager>();
		}
	};
}
