#pragma once
#include <GlobalData/Include.h>
#include <CrimsonDesert/Structures/BaseManager.h>
#include <Math/Vector.h>

#include <cstdint>
#include <cstddef>

using namespace Globals;

namespace CrimsonDesert {
	class BankInfoManager : public BaseManager {
	public:
		inline static BankInfoManager* GetInstance() {
			return BaseManager::GetInstanceFor<BankInfoManager>();
		}
	};
}
