#include <External/Include.h>
#include <CrimsonDesert/Include.h>
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



int main() {
    SetConsoleTitle("PureLiquid Crimson Desert External");
    while (!IsProcessRunning(L"CrimsonDesert.exe")) {}
    Sleep(500);
    LiquidHookEx::INIT("CrimsonDesert.exe");
    pProc = LiquidHookEx::proc;

    pExeMod = pProc->GetRemoteModule("CrimsonDesert.exe");
    printf("pExeMod: 0x%p\n", pExeMod->GetAddr());


#ifdef INCLUDE_OVERLAY
    OverlayAPI::InitImGuiContext();
    Overlay::Show(pProc->GetHwnd(), &renderer);
    renderer.AddRenderObject(new RenderObjects::Position());
#endif

    while (true) {
        if (GetAsyncKeyState(4) & 1) {
            auto vPos = CrimsonDesert::CPosition::GetInstance()->vLocalPos;
            vPos.y += 15.0f;
            CrimsonDesert::CPosition::GetInstance()->vLocalPos = vPos;
            Sleep(1000);
        }
    }
}
