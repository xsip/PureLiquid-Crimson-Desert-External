#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class FailMessageInfoManager : public BaseManager {
	public:
		inline static FailMessageInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<FailMessageInfoManager>();
		}
	};
}
