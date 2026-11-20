#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class FactionReblockadingInfoManager : public BaseManager {
	public:
		inline static FactionReblockadingInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<FactionReblockadingInfoManager>();
		}
	};
}
