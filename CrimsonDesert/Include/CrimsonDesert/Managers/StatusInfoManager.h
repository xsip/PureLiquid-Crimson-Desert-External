#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class StatusInfoManager : public BaseManager {
	public:
		inline static StatusInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<StatusInfoManager>();
		}
	};
}
