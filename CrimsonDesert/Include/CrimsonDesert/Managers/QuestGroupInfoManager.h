#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class QuestGroupInfoManager : public BaseManager {
	public:
		inline static QuestGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<QuestGroupInfoManager>();
		}
	};
}
