#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class NpcInfoManager : public BaseManager {
	public:
		inline static NpcInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<NpcInfoManager>();
		}
	};
}
