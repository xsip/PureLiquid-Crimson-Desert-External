#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class BreakableObjectInfoManager : public BaseManager {
	public:
		inline static BreakableObjectInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<BreakableObjectInfoManager>();
		}
	};
}
