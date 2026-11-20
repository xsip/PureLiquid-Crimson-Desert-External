#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class DetectInfoManager : public BaseManager {
	public:
		inline static DetectInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<DetectInfoManager>();
		}
	};
}
