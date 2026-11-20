#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class KnowledgeInfoManager : public BaseManager {
	public:
		inline static KnowledgeInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<KnowledgeInfoManager>();
		}
	};
}
