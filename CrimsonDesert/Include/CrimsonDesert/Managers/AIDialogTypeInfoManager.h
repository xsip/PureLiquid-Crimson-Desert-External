#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class AIDialogTypeInfoManager : public BaseManager {
	public:
		inline static AIDialogTypeInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<AIDialogTypeInfoManager>();
		}
	};
}
