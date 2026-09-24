
#include <Overlay/Definitions.h>
#include <Overlay/Overlay.h>
#include <Overlay/Renderer.h>
#include <d3d11.h>
#include <tchar.h>
#include <dwmapi.h>
#include <Overlay/View3D.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx11.h>
#include <Overlay/PerformanceProfiler.h>
#include <DirectXMath.h>
using namespace DirectX;

#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION

#include <vector>

static LONG rawMouseX = 0;
static LONG rawMouseY = 0;

namespace DX11 {
    Renderer* pRenderer = nullptr;
}


// Static member definitions
bool Overlay::menuOpen = false;
int Overlay::windowState = 1;
HWND Overlay::hwnd = NULL;
HWND Overlay::targetHwnd = NULL;
WNDCLASS Overlay::wc = { 0 };

ID3D11Device* Overlay::g_pd3dDevice = nullptr;
ID3D11DeviceContext* Overlay::g_pd3dDeviceContext = nullptr;
IDXGISwapChain* Overlay::g_pSwapChain = nullptr;
bool Overlay::g_SwapChainOccluded = false;
UINT Overlay::g_ResizeWidth = 0;
UINT Overlay::g_ResizeHeight = 0;
ID3D11RenderTargetView* Overlay::g_mainRenderTargetView = nullptr;
bool Overlay::bSetupDone = false;


ProcData Overlay::GetProcData() {
    ProcData d{};

    if (!targetHwnd)
        return d;

    // Get client rect (size of the drawable area)
    RECT clientRect;
    GetClientRect(targetHwnd, &clientRect);

    // Convert top-left corner of client area to screen coordinates
    POINT topLeft = { clientRect.left, clientRect.top };
    ClientToScreen(targetHwnd, &topLeft);

    d.hWnd = hwnd; // overlay window handle
    d.left = topLeft.x;
    d.top = topLeft.y;
    d.width = clientRect.right - clientRect.left;
    d.height = clientRect.bottom - clientRect.top;

    return d;
}

bool Overlay::Initialize() {
    auto data = GetProcData();
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hIcon = LoadIcon(0, IDI_WINLOGO);
    wc.lpszMenuName = 0;
    wc.style = 0;
    wc.hbrBackground = 0;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "Test";



    if (!RegisterClass(&wc)) {
        printf("Coudln't register window class!\n");
        return false;
    }
    auto posX = 0;
    hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT,
        _T("Test"), _T("Test"),
        WS_POPUP,
        data.left, data.top, data.width, data.height,
        NULL, NULL, wc.hInstance, NULL
    );

    DWM_BLURBEHIND bb = { 0 };
    bb.dwFlags = DWM_BB_ENABLE;
    bb.fEnable = FALSE;
    DwmEnableBlurBehindWindow(hwnd, &bb);

    BOOL disableNCRendering = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_ENABLED, &disableNCRendering, sizeof(disableNCRendering));

    BOOL result;
    SystemParametersInfo(SPI_SETMOUSESPEED, 0, (PVOID)10, 0);
    SystemParametersInfo(SPI_SETMOUSE, 0, NULL, SPIF_SENDCHANGE);

    // Raw input for lowest latency
    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01;  // Generic desktop
    rid.usUsage = 0x02;      // Mouse
    rid.dwFlags = RIDEV_INPUTSINK;
    rid.hwndTarget = hwnd;
    RegisterRawInputDevices(&rid, 1, sizeof(rid));

    // SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_COLORKEY | LWA_ALPHA);
    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    // hwnd = CreateWindowEx(WS_EX_TRANSPARENT, wc.lpszClassName, "DX12", WS_OVERLAPPEDWINDOW, 0, 0, 1280, 720, nullptr, nullptr, nullptr, nullptr);
    printf("hwnd: 0x%p\n", hwnd);


    if (!CreateDeviceD3D())
    {
        CleanupDeviceD3D();
        ::UnregisterClassW((LPCWSTR)wc.lpszClassName, wc.hInstance);
        return 1;
    }

    printf("Initialized!\n");
    if (targetHwnd)
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Overlay::KeepOverlayed, NULL, 0, NULL);

    ChangeClickability(true, hwnd);
    Overlay::MessageThread();

    return true;
    // return messages.wParam;
}
void Overlay::ChangeClickability(bool canclick, HWND ownd)
{

    long style = GetWindowLong(
        ownd, GWL_EXSTYLE);
    if (canclick) {
        style &= ~WS_EX_LAYERED;
        SetWindowLong(ownd,
            GWL_EXSTYLE, style);
        // SetForegroundWindow(ownd);
        windowState = 1;
    }
    else {
        style |= WS_EX_LAYERED;
        SetWindowLong(ownd,
            GWL_EXSTYLE, style);
        SetForegroundWindow(targetHwnd);
        windowState = 0;
    }
}


void Overlay::RenderDropShadow(ImTextureID tex_id, float size, ImU8 opacity)
{
    ImVec2 p = ImGui::GetWindowPos();
    ImVec2 s = ImGui::GetWindowSize();
    ImVec2 m = { p.x + s.x, p.y + s.y };
    float uv0 = 0.0f;      // left/top region
    float uv1 = 0.333333f; // leftward/upper region
    float uv2 = 0.666666f; // rightward/lower region
    float uv3 = 1.0f;      // right/bottom region
    ImU32 col = (opacity << 24) | 0xFFFFFF;
    ImDrawList* dl = ImGui::GetWindowDrawList();
    dl->PushClipRectFullScreen();
    dl->AddImage(tex_id, { p.x - size, p.y - size }, { p.x,        p.y }, { uv0, uv0 }, { uv1, uv1 }, col);
    dl->AddImage(tex_id, { p.x,        p.y - size }, { m.x,        p.y }, { uv1, uv0 }, { uv2, uv1 }, col);
    dl->AddImage(tex_id, { m.x,        p.y - size }, { m.x + size, p.y }, { uv2, uv0 }, { uv3, uv1 }, col);
    dl->AddImage(tex_id, { p.x - size, p.y }, { p.x,        m.y }, { uv0, uv1 }, { uv1, uv2 }, col);
    dl->AddImage(tex_id, { m.x,        p.y }, { m.x + size, m.y }, { uv2, uv1 }, { uv3, uv2 }, col);
    dl->AddImage(tex_id, { p.x - size, m.y }, { p.x,        m.y + size }, { uv0, uv2 }, { uv1, uv3 }, col);
    dl->AddImage(tex_id, { p.x,        m.y }, { m.x,        m.y + size }, { uv1, uv2 }, { uv2, uv3 }, col);
    dl->AddImage(tex_id, { m.x,        m.y }, { m.x + size, m.y + size }, { uv2, uv2 }, { uv3, uv3 }, col);
    dl->PopClipRect();
}



void Overlay::KeepOverlayed() {
    while (true) {

        if (GetAsyncKeyState(VK_INSERT) & 1) {
            menuOpen = !menuOpen;
            printf("Menu Open: %i\n", menuOpen);
        }

        auto data = GetProcData();
        RECT out;
        GetWindowRect(hwnd, &out);
        if (out.left != data.left || out.top != data.top || (out.right - out.left) != data.width || (out.bottom - out.top) != data.height)
            SetWindowPos(hwnd, NULL, data.left, data.top, data.width, data.height, NULL);
        Sleep(1);  // Reduced from 10ms for more responsive positioning
    }
}
void Overlay::Show(HWND _hWnd, Renderer* _pRenderer) {
    if (Overlay::bSetupDone)
        return;
    DX11::pRenderer = _pRenderer;
    targetHwnd = _hWnd;
    Overlay::bSetupDone = true;
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Overlay::Initialize, NULL, 0, NULL);
}

bool Overlay::RenderButton(const char* szTitle, bool& checked, ImVec2 pos, ImVec2 size, ImTextureID img) {
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0.25, 0.35, 0.95));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::SetCursorPos(pos);
    // ImGui::PushID(1);
    if (checked) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0.25, 0.35, 0.95));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0.25, 0.35, 0.95));
    }
    else {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0.25, 0.25, 0.90));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0.25, 0.30, 0.95));

    }
    if (ImGui::Button(szTitle, size)) {
        checked = !checked;
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();

        return true;
    }
    if (img) {
        ImGui::SetCursorPos(ImVec2(pos.x + 20, pos.y + (size.y / 2) - 20 / 2));
        ImGui::Image(img, ImVec2(20, 20));
    }

    ImGui::PopStyleVar();

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    return false;
    // ImGui::PopID();

}


bool buttonOneActive = false;
bool buttonTwoActive = false;
bool buttonThreeActive = false;


void Overlay::MessageThread() {

    ShowWindow(Overlay::hwnd, TRUE);
    UpdateWindow(Overlay::hwnd);
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplWin32_EnableAlphaCompositing(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
#ifdef USE_PROFILER
    PerformanceProfiler profiler;
    auto lastProfilePrint = std::chrono::high_resolution_clock::now();
    int frameCounter = 0;
#endif
    bool done = false;

    // Setup 3D world
    static View3D view3D;
    auto data = GetProcData();
    view3D.Init(g_pd3dDevice, g_pd3dDeviceContext, (float)data.width, (float)data.height);
    DX11::pRenderer->SetView3D(&view3D);

    while (!done) {
#ifdef USE_PROFILER
        profiler.BeginSection("1_MessagePump");
#endif
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT) done = true;
        }
#ifdef USE_PROFILER
        profiler.EndSection("1_MessagePump");
#endif
        if (done) break;
#ifdef USE_PROFILER
        profiler.BeginSection("2_OcclusionCheck");
#endif
        // Swap chain occlusion
        if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED) {
            ::Sleep(1);
            continue;
        }
        g_SwapChainOccluded = false;
#ifdef USE_PROFILER
        profiler.EndSection("2_OcclusionCheck");
        profiler.BeginSection("3_ResizeHandling");
#endif
        // Handle resize
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0) {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }
#ifdef USE_PROFILER
        profiler.EndSection("3_ResizeHandling");
        profiler.BeginSection("4_ImGuiNewFrame");
#endif
        // Start ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
#ifdef USE_PROFILER
        profiler.EndSection("4_ImGuiNewFrame");
#endif
        auto forGroundWindow = GetForegroundWindow();
        auto DisplaySize = ImVec2(data.width, data.height);

        if ((forGroundWindow == targetHwnd || forGroundWindow == hwnd)) {
#ifdef USE_PROFILER
            profiler.BeginSection("5_Clickability");
#endif
            if (menuOpen) {
                if (windowState != 1) ChangeClickability(true, hwnd);
            }
            else if (!menuOpen) {
                if (windowState != 0) ChangeClickability(false, hwnd);
            }
#ifdef USE_PROFILER
            profiler.EndSection("5_Clickability");
            profiler.BeginSection("6_CursorPosition");
#endif
            ImGui::SetCurrentContext(OverlayAPI::GetImGuiContext());
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hwnd, &p);
#ifdef USE_PROFILER
            profiler.EndSection("6_CursorPosition");

            profiler.BeginSection("7_D3DClear");
#endif

            const float clear_color_with_alpha[4] = { 0,0,0,0 };
            g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
            g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
            
#ifdef USE_PROFILER
            profiler.EndSection("7_D3DClear");

            profiler.BeginSection("8_View3DSetup");      
#endif

            view3D.Render(g_mainRenderTargetView);
            view3D.BeginFrame();
#ifdef USE_PROFILER
            profiler.EndSection("8_View3DSetup");

            profiler.BeginSection("9_ESPRender");
#endif
            DX11::pRenderer->Render(DisplaySize, { (float)p.x, (float)p.y }, menuOpen, &view3D);
#ifdef USE_PROFILER
            profiler.EndSection("9_ESPRender");

            profiler.BeginSection("10_View3DFlush");
#endif
            view3D.EndFrame();
#ifdef USE_PROFILER
            profiler.EndSection("10_View3DFlush");
#endif
        }
#ifdef USE_PROFILER
        profiler.BeginSection("11_ImGuiRender");
#endif
        // ImGui rendering
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#ifdef USE_PROFILER
        profiler.EndSection("11_ImGuiRender");

        profiler.BeginSection("12_Present");
#endif
        HRESULT hr = g_pSwapChain->Present(0, 0);
        g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
#ifdef USE_PROFILER
        profiler.EndSection("12_Present");

        frameCounter++;

        // Print profile every 3 seconds
        auto now = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - lastProfilePrint).count() >= 3) {
            printf("\n📊 Profile Report (Frames: %d):\n", frameCounter);
            profiler.PrintStats();
            profiler.Reset();
            frameCounter = 0;
            lastProfilePrint = now;
        }
#endif
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW((LPCWSTR)wc.lpszClassName, wc.hInstance);
}


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK Overlay::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_INPUT:
    {
        UINT size;
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));

        LPBYTE buffer = new BYTE[size];
        if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER)) == size) {
            RAWINPUT* raw = (RAWINPUT*)buffer;
            if (raw->header.dwType == RIM_TYPEMOUSE) {
                // Store raw mouse delta
                static LONG rawMouseX = 0;
                static LONG rawMouseY = 0;
                rawMouseX += raw->data.mouse.lLastX;
                rawMouseY += raw->data.mouse.lLastY;

                // You can use rawMouseX/rawMouseY for smoother tracking
            }
        }
        delete[] buffer;
        break;
    }
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}




bool Overlay::CreateDeviceD3D()
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.Flags = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; 
    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    IDXGIDevice1* pDXGIDevice = nullptr;
    if (SUCCEEDED(g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&pDXGIDevice))) {
        pDXGIDevice->SetGPUThreadPriority(7); // Max priority
        pDXGIDevice->Release();
    }

    CreateRenderTarget();



    return true;
}

void Overlay::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void Overlay::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void Overlay::CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }

}
