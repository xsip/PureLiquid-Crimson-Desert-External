#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class JobInfoManager : public BaseManager {
	public:
		inline static JobInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<JobInfoManager>();
		}
	};
}
