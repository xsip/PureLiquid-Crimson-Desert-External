#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class GimmickGateInfoManager : public BaseManager {
	public:
		inline static GimmickGateInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<GimmickGateInfoManager>();
		}
	};
}
