#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class LocalStringInfoManager : public BaseManager {
	public:
		inline static LocalStringInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<LocalStringInfoManager>();
		}
	};
}
