#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class DropSetInfoManager : public BaseManager {
	public:
		inline static DropSetInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<DropSetInfoManager>();
		}
	};
}
