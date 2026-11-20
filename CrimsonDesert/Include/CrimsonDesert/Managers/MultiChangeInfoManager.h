#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class MultiChangeInfoManager : public BaseManager {
	public:
		inline static MultiChangeInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<MultiChangeInfoManager>();
		}
	};
}
