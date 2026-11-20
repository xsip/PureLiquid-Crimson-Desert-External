#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class InteractionInfoManager : public BaseManager {
	public:
		inline static InteractionInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<InteractionInfoManager>();
		}
	};
}
