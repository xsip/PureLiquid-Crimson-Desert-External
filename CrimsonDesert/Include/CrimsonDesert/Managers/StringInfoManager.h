#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class StringInfoManager : public BaseManager {
	public:
		inline static StringInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<StringInfoManager>();
		}
	};
}
