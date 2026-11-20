#include <External/Include.h>
#include <CrimsonDesert/Include.h>
#include <CrimsonDesert/Managers/Include.h>
#ifdef INCLUDE_OVERLAY
Renderer renderer{ {} };
#endif

namespace Globals {
    LiquidHookEx::Process* pProc = nullptr;
    LiquidHookEx::RemoteModule* pExeMod = nullptr;
}


bool IsProcessRunning(const std::wstring& exeName)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
        return false;

    PROCESSENTRY32W entry{};
    entry.dwSize = sizeof(entry);

    bool found = false;

    if (Process32FirstW(snapshot, &entry))
    {
        do
        {
            if (_wcsicmp(entry.szExeFile, exeName.c_str()) == 0)
            {
                found = true;
                break;
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return found;
}



 // pa::uiCommonScript::UIGamePlayControlRootWorldMap
// 48 8B C4 48 89 58 ? 48 89 48 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? C5 F8 29 70 ? C5 F8 29 78 ? C5 F8 28 F9
/*
  _RSI = UIGamePlayControlRootWorldMap_A1;
 {
      v18 = *(_DWORD *)(_RSI + 0x3B0);
      if ( v18 != 0 )
      {
        v19 = 0;
        for ( j = v18; j != 0; --j )
        {
          MarkerLoop_sub_F6ECE0(
            a1: _RSI,
            a2: v7,
            a3: *(_QWORD *)(*(_QWORD *)(_RSI + 0x3A8) + v19),
            a4: &v209,
            a5: (unsigned int *)&v226,
            a6: (__int64)&v212);
          v19 += 8;
        }
        goto LABEL_14;
      }
    }
*/
void DumpMarkers(CrimsonDesert::UIGamePlayControlRootWorldMap* _rsi)
{

    uintptr_t rsi = reinterpret_cast<uintptr_t>(_rsi);
    auto vInst = CrimsonDesert::CPosition::GetInstance();

    auto count = pProc->ReadDirect<uint32_t>(rsi + 0x3B0);

    if (count == 0) {
        printf("Count = 0!!\n");
        return;
    }

    printf("Count: %u\n", count);

    uintptr_t outerArray =
        pProc->ReadDirect<uintptr_t>(rsi + 0x3A8);

    if (!outerArray) {
        printf("Outer array is null!\n");
        return;
    }

    uintptr_t v19 = 0;

    for (uint32_t j = 0; j < count; ++j)
    {
        uintptr_t A3 =
            pProc->ReadDirect<uintptr_t>(outerArray + v19);

        printf("\n=== Outer index %u | A3: %p ===\n",
            j, (void*)A3);

        if (!A3) {
            v19 += 8;
            continue;
        }

        uintptr_t innerArray =
            pProc->ReadDirect<uintptr_t>(A3 + 0x40);

        if (!innerArray) {
            printf("Inner array is null!\n");
            v19 += 8;
            continue;
        }

        // This is the count of inner entries.
        // Use the appropriate count if known.
        for (int markerIdx = 0; markerIdx < 1000; ++markerIdx)
        {
            uintptr_t v37 =
                pProc->ReadDirect<uintptr_t>(
                    innerArray + markerIdx * 8
                );

            if (!v37)
                break;

            uintptr_t marker =
                pProc->ReadDirect<uintptr_t>(v37 + 0xE0);

            if (!marker)
                break;
            Vector3 vPosition = pProc->ReadDirect<Vector3>(marker + 0xE0);
            vPosition.x += 10000;
            vPosition.z += 3000;

            auto markerName = pProc->ReadString(pProc->ReadDirect<uintptr_t>(pProc->ReadDirect<uintptr_t>(marker + 0x238)));
            // if (markerName == "Bell") {

                printf(
                    "Index: %d  | %s: %p | %.2f %.2f %.2f\n",
                    markerIdx,
                    markerName.c_str(),
                    (void*)marker,
                    vPosition.x, vPosition.y, vPosition.z
                );
            // }
        }

        v19 += 8;
    }


    auto vPos = vInst->vLocalPos;
    // vInst->vLocalPos = { -664.17f, 617.01f, -837.61f };
    printf("LocalPosition: %.2f %.2f %.2f\n", vPos.x, vPos.y, vPos.z);
    printf("RSI: 0x%p\n", rsi);
}



int main() {
    SetConsoleTitle("PureLiquid Crimson Desert External");
    while (!IsProcessRunning(L"CrimsonDesert.exe")) {}
    Sleep(500);
    LiquidHookEx::INIT("CrimsonDesert.exe");
    pProc = LiquidHookEx::proc;

    pExeMod = pProc->GetRemoteModule("CrimsonDesert.exe");
    printf("pExeMod: 0x%p\n", pExeMod->GetAddr());
    /*CrimsonDesert::ManagerLoopDumper::Dump();
    // system("pause");
    CrimsonDesert::ManagerLoopDumper::Generate(false);
    // system("pause");

    
    
    auto pNpcManager = CrimsonDesert::NpcInfoManager::GetInstance();
    printf("%i Entities\n", pNpcManager->m_iCount);

    for (int i = 0; i < pNpcManager->m_iCount; i++) {
        auto npc = pNpcManager->GetEntry(i);
        auto hash = npc->m_iHash;
        auto pFirstNpc = npc->m_pMetaData;
        auto idx = pProc->ReadDirect<int>(reinterpret_cast<uintptr_t>(pFirstNpc) + 0x8);
        printf("Npc[%i => 0x%p ( %i )] => %s | %s\n", i, pFirstNpc, idx, pFirstNpc->GetName().c_str(), pFirstNpc->GetLocation().c_str());
    }
    printf("NpCManager: 0x%p\n", pNpcManager);
    system("pause");*/
     // DumpMarkers(pProc->ReadDirect<CrimsonDesert::UIGamePlayControlRootWorldMap*>(pProc->ReadDirect<uintptr_t>(pProc->ReadDirect<uintptr_t>(pProc->ReadDirect<uintptr_t>(pExeMod->GetAddr() + 0x07252870) + 0x850) + 0x40)));
    auto pGlobalCtx = CrimsonDesert::GlobalContext::GetInstance();
    auto pRootWorldMap = pGlobalCtx->GetUIGamePlayControlRootWorldMap();
    DumpMarkers(pRootWorldMap);
    printf("pRootWorldMap: 0x%p\n", pRootWorldMap);
    while (true) {
        // printf("IsHovering: %i\n", pRootWorldMap->m_bIsHoveringMapItem);
        if (pRootWorldMap->m_bIsHoveringMapItem) {
            auto icn = pRootWorldMap->m_hoverdMapIconContainer->m_pIcon;
            if (!icn) {
                continue;
            }
            
            auto pos = icn->GetWorldPosition();
            auto pos2 = icn->GetWorldPosition(true);
            printf("Hovering 0x%p: %s\nPos: ( %.2f %.2f %.2f ) \nOri: ( %.2f %.2f %.2f ) \n", icn, icn->GetIconName().c_str(), pos.x, pos.y, pos.z, pos2.x, pos2.y, pos2.z);
        }
        Sleep(100);

    }
    return 1;
    
#ifdef INCLUDE_OVERLAY
    OverlayAPI::InitImGuiContext();
    Overlay::Show(pProc->GetHwnd(), &renderer);
    renderer.AddRenderObject(new RenderObjects::Position());
    // renderer.AddRenderObject(new RenderObjects::MapHelper());
#endif

    if (!CrimsonDesert::ServerTransformSyncActorComponent::HookUpdate()) {
        printf("Error Hooking ServerTransformSyncActorComponent::Update!!\n");
        return 1;
    }
    CrimsonDesert::ServerTransformSyncActorComponent::DisableFallDamage();

    // CrimsonDesert::UIGamePlayControlCommonCurrencyIndicator::GetInstance()->m_iCoins = 999000; // Give 1 coin to a beggar or trade to update coins
    while (!GetAsyncKeyState(VK_DELETE) & 1) {
        if (GetAsyncKeyState(4)) {
            //printf("UIGamePlayControlCommonCurrencyIndicator*: 0x%p + 0xD8\n", CrimsonDesert::UIGamePlayControlCommonCurrencyIndicator::GetInstance());

            /*pProc->DumpVTable(0x3A53BE0A460, 5, "RotateCamera");
            printf("ServerTransformSyncActorComponent*: 0x%p\n", CrimsonDesert::ServerTransformSyncActorComponent::GetInstance());
            */
            auto vPos = CrimsonDesert::CPosition::GetInstance()->vLocalPos;
            vPos.y += 15.0f;
            CrimsonDesert::CPosition::GetInstance()->vLocalPos = vPos;
            Sleep(10);
        }
    }

    CrimsonDesert::ServerTransformSyncActorComponent::EnableFallDamage();
    Sleep(100);
    if (!CrimsonDesert::ServerTransformSyncActorComponent::Unhook()) {
        printf("Error Un-Hooking ServerTransformSyncActorComponent::Update!!\n");
    }

    return 1;

}
