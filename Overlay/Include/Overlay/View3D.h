#pragma once
#include <d3d11.h>
#include <Overlay/Definitions.h>
#include <DirectXMath.h>
#include <Math/Vector.h>
#include <vector>
using namespace DirectX;

struct Vertex {
    XMFLOAT3 pos;
    XMFLOAT4 color;
};

struct LineVertex {
    XMFLOAT3 pos;
    XMFLOAT4 color;
};

struct CBFrame { XMMATRIX view; XMMATRIX proj; };
struct CBObject { XMMATRIX world; };

class OVERLAY_API View3D {
public:
    bool Init(ID3D11Device* device, ID3D11DeviceContext* ctx, float width, float height);

    void BeginFrame();
    void EndFrame();

    void SetViewMatrix(const XMMATRIX& view);
    void SetProjection(float width, float height);

    XMFLOAT2 ToScreen(const XMFLOAT3& worldPos);
    void DrawBox(Vector3 pos, Vector3 size, XMFLOAT4 color);
    bool IsLookingAtCapsule(const XMFLOAT3& capsuleCenter, const XMFLOAT3& vMin, const XMFLOAT3& vMax, float radius) const;
    void DrawLine3D(XMFLOAT3 start, XMFLOAT3 end, float thickness, XMFLOAT4 color);
    void DrawThickLine3D(const XMFLOAT3& start, const XMFLOAT3& end, float thickness, const XMFLOAT4& color);
    void DrawLine3DBold(Vector3 start, Vector3 end, XMFLOAT4 color, int repeats = 3);
    void DrawWireBox(Vector3 pos, Vector3 size, XMFLOAT4 color);
    void Render(ID3D11RenderTargetView* rtv);
    void DrawCapsuleMesh(
        Vector vMin, Vector vMax,
        float radius,
        Vector4D_t rotation,
        Vector origin,
        XMFLOAT4 color,
        int segments = 12,
        int stacks = 4,
        float thickness = 1.0f);
    void DrawCapsuleOutline(
        Vector vMin, Vector vMax,
        float radius,
        Vector4D_t rotation,
        Vector origin,
        XMFLOAT4 color,
        int segments = 16,
        int stacks = 4,
        float thickness = 1.0f);
private:
    ID3D11DeviceContext* ctx = nullptr;
    ID3D11VertexShader* vs = nullptr;
    ID3D11PixelShader* ps = nullptr;
    ID3D11InputLayout* layout = nullptr;

    // Buffers
    ID3D11Buffer* cbFrame = nullptr;
    ID3D11Buffer* cbObject = nullptr;
    ID3D11Buffer* vbBox = nullptr;
    ID3D11Buffer* ibBox = nullptr;
    ID3D11Buffer* vbLine = nullptr;

    ID3D11Device* device = nullptr;


public:
    // for W2S ( untransformed )
    XMVECTOR cameraPos{};
    XMMATRIX cpuView = XMMatrixIdentity();
    XMMATRIX cpuProj = XMMatrixIdentity();
    // Cached matrices
    XMMATRIX cachedView = XMMatrixIdentity();
    XMMATRIX cachedProj = XMMatrixIdentity();
private:
    // Batched vertices
    std::vector<Vertex> boxVertices;
    std::vector<LineVertex> lineVertices;

    float Width = 0;
    float Height = 0;

    static constexpr int MaxBoxes = 1024;
    static constexpr int MaxLines = 16384;
};
