#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class FactionOperationGroupInfoManager : public BaseManager {
	public:
		inline static FactionOperationGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<FactionOperationGroupInfoManager>();
		}
	};
}
