#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class FrameEventAttrGroupInfoManager : public BaseManager {
	public:
		inline static FrameEventAttrGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<FrameEventAttrGroupInfoManager>();
		}
	};
}
