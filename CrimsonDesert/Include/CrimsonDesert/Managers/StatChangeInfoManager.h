#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class StatChangeInfoManager : public BaseManager {
	public:
		inline static StatChangeInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<StatChangeInfoManager>();
		}
	};
}
