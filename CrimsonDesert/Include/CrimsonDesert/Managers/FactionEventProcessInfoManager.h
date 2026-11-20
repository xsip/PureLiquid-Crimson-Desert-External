#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class FactionEventProcessInfoManager : public BaseManager {
	public:
		inline static FactionEventProcessInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<FactionEventProcessInfoManager>();
		}
	};
}
