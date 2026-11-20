#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class FactionInfoManager : public BaseManager {
	public:
		inline static FactionInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<FactionInfoManager>();
		}
	};
}
