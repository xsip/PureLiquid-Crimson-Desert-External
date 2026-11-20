#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class BuffInfoManager : public BaseManager {
	public:
		inline static BuffInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<BuffInfoManager>();
		}
	};
}
