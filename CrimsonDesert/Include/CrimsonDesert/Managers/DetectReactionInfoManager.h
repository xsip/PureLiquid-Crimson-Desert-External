#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class DetectReactionInfoManager : public BaseManager {
	public:
		inline static DetectReactionInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<DetectReactionInfoManager>();
		}
	};
}
