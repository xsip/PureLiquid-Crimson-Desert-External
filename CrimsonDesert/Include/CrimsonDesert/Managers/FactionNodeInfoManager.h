#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class FactionNodeInfoManager : public BaseManager {
	public:
		inline static FactionNodeInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<FactionNodeInfoManager>();
		}
	};
}
