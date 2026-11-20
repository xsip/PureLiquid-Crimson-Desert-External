#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class SocketInfoManager : public BaseManager {
	public:
		inline static SocketInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<SocketInfoManager>();
		}
	};
}
