#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class QuestGaugeInfoManager : public BaseManager {
	public:
		inline static QuestGaugeInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<QuestGaugeInfoManager>();
		}
	};
}
