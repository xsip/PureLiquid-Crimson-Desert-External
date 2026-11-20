#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class KnowledgeGroupInfoManager : public BaseManager {
	public:
		inline static KnowledgeGroupInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<KnowledgeGroupInfoManager>();
		}
	};
}
