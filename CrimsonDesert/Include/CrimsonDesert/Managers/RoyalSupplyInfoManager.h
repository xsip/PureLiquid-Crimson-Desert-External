#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class RoyalSupplyInfoManager : public BaseManager {
	public:
		inline static RoyalSupplyInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<RoyalSupplyInfoManager>();
		}
	};
}
