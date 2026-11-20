#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class GimmickGateConnectionInfoManager : public BaseManager {
	public:
		inline static GimmickGateConnectionInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<GimmickGateConnectionInfoManager>();
		}
	};
}
