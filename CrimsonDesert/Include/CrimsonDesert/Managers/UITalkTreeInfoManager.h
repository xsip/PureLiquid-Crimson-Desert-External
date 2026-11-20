#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class UITalkTreeInfoManager : public BaseManager {
	public:
		inline static UITalkTreeInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<UITalkTreeInfoManager>();
		}
	};
}
