#include <Overlay/View3D.h>
#include <d3dcompiler.h>
#include <iostream>
#include <cassert>
#include <Math/Quaternion.h>
#include <Math/Vector.h>
#include <Math/Matrix.h>
#pragma comment(lib,"d3dcompiler.lib")
#undef min
#undef max

bool View3D::Init(ID3D11Device* _device, ID3D11DeviceContext* _ctx, float width, float height) {

    ctx = _ctx;
    Width = width;
    Height = height;
    device = _device;

    // Constant buffers
    D3D11_BUFFER_DESC cbd = {};
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.ByteWidth = sizeof(CBFrame);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    if (FAILED(device->CreateBuffer(&cbd, nullptr, &cbFrame))) return false;

    cbd.ByteWidth = sizeof(CBObject);
    if (FAILED(device->CreateBuffer(&cbd, nullptr, &cbObject))) return false;

    // Dynamic VB for boxes
    D3D11_BUFFER_DESC bdBox = {};
    bdBox.Usage = D3D11_USAGE_DYNAMIC;
    bdBox.ByteWidth = sizeof(Vertex) * 8 * MaxBoxes;
    bdBox.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bdBox.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    if (FAILED(device->CreateBuffer(&bdBox, nullptr, &vbBox))) return false;

    // Static IB for boxes
    std::vector<uint16_t> indices;
    indices.reserve(MaxBoxes * 36);
    for (int i = 0; i < MaxBoxes; i++) {
        uint16_t base = i * 8;
        uint16_t idx[36] = {
            0,2,1,1,2,3, 4,5,6,5,7,6, 0,1,4,1,5,4,
            2,6,3,3,6,7, 0,4,2,2,4,6, 1,3,5,3,7,5
        };
        for (int j = 0; j < 36; j++) idx[j] += base;
        indices.insert(indices.end(), idx, idx + 36);
    }
    D3D11_BUFFER_DESC ibd = {};
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = (UINT)(sizeof(uint16_t) * indices.size());
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    D3D11_SUBRESOURCE_DATA ibData = { indices.data(),0,0 };
    if (FAILED(device->CreateBuffer(&ibd, &ibData, &ibBox))) return false;

    // Dynamic VB for lines
    D3D11_BUFFER_DESC bdLine = {};
    bdLine.Usage = D3D11_USAGE_DYNAMIC;
    bdLine.ByteWidth = sizeof(LineVertex) * MaxLines;
    bdLine.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bdLine.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    if (FAILED(device->CreateBuffer(&bdLine, nullptr, &vbLine))) return false;

    // Compile shaders
    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    const char* vsCode =
        "cbuffer CBObject : register(b0) { matrix World; };"
        "cbuffer CBFrame : register(b1) { matrix View; matrix Proj; };"
        "struct VS_INPUT { float3 Pos:POSITION; float4 Color:COLOR; };"
        "struct PS_INPUT { float4 Pos:SV_POSITION; float4 Color:COLOR; };"
        "PS_INPUT main(VS_INPUT input) { PS_INPUT o; o.Pos=mul(float4(input.Pos,1),World);"
        "o.Pos=mul(o.Pos,View); o.Pos=mul(o.Pos,Proj); o.Color=input.Color; return o; }";

    if (FAILED(D3DCompile(vsCode, strlen(vsCode), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &vsBlob, &errorBlob))) {
        if (errorBlob) { OutputDebugStringA((char*)errorBlob->GetBufferPointer()); errorBlob->Release(); }
        return false;
    }
    if (FAILED(device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vs))) { vsBlob->Release(); return false; }

    D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
    };
    if (FAILED(device->CreateInputLayout(layoutDesc, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &layout))) { vsBlob->Release(); return false; }
    vsBlob->Release();

    const char* psCode = "struct PS_INPUT{float4 Pos:SV_POSITION; float4 Color:COLOR;}; float4 main(PS_INPUT i):SV_TARGET{return i.Color;}";
    if (FAILED(D3DCompile(psCode, strlen(psCode), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &psBlob, &errorBlob))) {
        if (errorBlob) { OutputDebugStringA((char*)errorBlob->GetBufferPointer()); errorBlob->Release(); }
        return false;
    }
    if (FAILED(device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &ps))) { psBlob->Release(); return false; }
    psBlob->Release();

    SetProjection(width, height);
    return true;
}

Matrix3x4_t QuatToMatrix3x4(const Vector4D_t& q)
{
    float xx = q.x * q.x;
    float yy = q.y * q.y;
    float zz = q.z * q.z;
    float xy = q.x * q.y;
    float xz = q.x * q.z;
    float yz = q.y * q.z;
    float wx = q.w * q.x;
    float wy = q.w * q.y;
    float wz = q.w * q.z;

    Matrix3x4_t m;
    m[0][0] = 1.0f - 2.0f * (yy + zz);
    m[0][1] = 2.0f * (xy - wz);
    m[0][2] = 2.0f * (xz + wy);
    m[0][3] = 0.0f;

    m[1][0] = 2.0f * (xy + wz);
    m[1][1] = 1.0f - 2.0f * (xx + zz);
    m[1][2] = 2.0f * (yz - wx);
    m[1][3] = 0.0f;

    m[2][0] = 2.0f * (xz - wy);
    m[2][1] = 2.0f * (yz + wx);
    m[2][2] = 1.0f - 2.0f * (xx + yy);
    m[2][3] = 0.0f;

    return m;
}

void create_circle_perpendicular(const Vector& center, const Vector& axis, float radius, std::vector<Vector>& outPoints, int segments = 12)
{
    outPoints.clear();

    // Find two perpendicular vectors to the axis
    Vector arbitrary = (fabs(axis.x) < 0.99f) ? Vector(1, 0, 0) : Vector(0, 1, 0);
    Vector u = axis.CrossProduct(arbitrary).Normalized();
    Vector v = axis.CrossProduct(u).Normalized();

    for (int i = 0; i <= segments; ++i)
    {
        float angle = (2.0f * M_PI * i) / segments;
        Vector point = center + (u * cosf(angle) + v * sinf(angle)) * radius;
        outPoints.push_back(point);
    }
}
void View3D::DrawCapsuleMesh(
    Vector vMin, Vector vMax,
    float radius,
    Vector4D_t rotation,
    Vector origin,
    XMFLOAT4 color,
    int segments,
    int stacks,
    float thickness)
{
    // Convert quaternion to rotation matrix
    Matrix3x4_t rotMat = QuatToMatrix3x4(rotation);

    // Transform capsule ends to world space
    Vector bottom = vMin.Transform(rotMat) + origin;
    Vector top = vMax.Transform(rotMat) + origin;

    // Temporary containers for circle vertices
    std::vector<Vector> topRing, bottomRing;

    // Generate base circle rings in XY plane around top and bottom
    for (int i = 0; i <= segments; ++i) {
        float angle = (2.0f * M_PI * i) / segments;
        float cx = cosf(angle) * radius;
        float cy = sinf(angle) * radius;
        topRing.push_back(Vector(top.x + cx, top.y + cy, top.z));
        bottomRing.push_back(Vector(bottom.x + cx, bottom.y + cy, bottom.z));
    }

    // Draw vertical lines connecting top/bottom rings
    for (int i = 0; i < segments; ++i) {
        DrawLine3D({ topRing[i].x, topRing[i].y, topRing[i].z },
            { bottomRing[i].x, bottomRing[i].y, bottomRing[i].z }, thickness, color);
    }

    // Draw top and bottom circle outlines
    for (int i = 0; i < segments; ++i) {
        DrawLine3D({ topRing[i].x, topRing[i].y, topRing[i].z },
            { topRing[i + 1].x, topRing[i + 1].y, topRing[i + 1].z }, thickness, color);
        DrawLine3D({ bottomRing[i].x, bottomRing[i].y, bottomRing[i].z },
            { bottomRing[i + 1].x, bottomRing[i + 1].y, bottomRing[i + 1].z }, thickness, color);
    }

    // Draw hemispheres stacks
    for (int s = 1; s <= stacks; ++s) {
        float phi = (float)s / stacks * (M_PI / 2.0f);
        float ringRadius = radius * cosf(phi);
        float zOffsetTop = radius * sinf(phi);
        float zOffsetBot = radius * sinf(phi);

        std::vector<Vector> topStack, bottomStack;
        for (int i = 0; i <= segments; ++i) {
            float angle = (2.0f * M_PI * i) / segments;
            float cx = cosf(angle) * ringRadius;
            float cy = sinf(angle) * ringRadius;
            topStack.push_back(Vector(top.x + cx, top.y + cy, top.z + zOffsetTop));
            bottomStack.push_back(Vector(bottom.x + cx, bottom.y + cy, bottom.z - zOffsetBot));
        }

        // Connect previous ring to current stack
        for (int i = 0; i < segments; ++i) {
            DrawLine3D({ topRing[i].x, topRing[i].y, topRing[i].z },
                { topStack[i].x, topStack[i].y, topStack[i].z }, thickness, color);
            DrawLine3D({ bottomRing[i].x, bottomRing[i].y, bottomRing[i].z },
                { bottomStack[i].x, bottomStack[i].y, bottomStack[i].z }, thickness, color);

            DrawLine3D({ topStack[i].x, topStack[i].y, topStack[i].z },
                { topStack[i + 1].x, topStack[i + 1].y, topStack[i + 1].z }, thickness, color);
            DrawLine3D({ bottomStack[i].x, bottomStack[i].y, bottomStack[i].z },
                { bottomStack[i + 1].x, bottomStack[i + 1].y, bottomStack[i + 1].z }, thickness, color);
        }

        topRing = topStack;
        bottomRing = bottomStack;
    }
}

void View3D::DrawCapsuleOutline(
    Vector vMin, Vector vMax,
    float radius,
    Vector4D_t rotation,
    Vector origin,
    XMFLOAT4 color,
    int segments,
    int stacks,
    float thickness)
{
    // Convert rotation quaternion to matrix
    Matrix3x4_t rotMat = QuatToMatrix3x4(rotation);

    // Transform capsule ends to world space
    Vector3 bottom = vMin.Transform(rotMat) + origin;
    Vector3 top = vMax.Transform(rotMat) + origin;

    // Compute capsule axis
    Vector3 axis = (top - bottom).Normalized();

    // Create two perpendicular vectors to axis for the circle plane
    Vector3 arbitrary = (fabs(axis.x) < 0.99f) ? Vector3(1, 0, 0) : Vector3(0, 1, 0);
    Vector3 u = axis.CrossProduct(arbitrary).Normalized();
    Vector3 v = axis.CrossProduct(u).Normalized();

    // Generate top & bottom circle points
    std::vector<Vector3> topCircle, bottomCircle;
    for (int i = 0; i < segments; ++i)
    {
        float angle = 2.0f * M_PI * i / segments;
        Vector3 offset = u * cosf(angle) * radius + v * sinf(angle) * radius;
        topCircle.push_back(top + offset);
        bottomCircle.push_back(bottom + offset);
    }

    // Draw top & bottom circles
    for (int i = 0; i < segments; ++i)
    {
        int next = (i + 1) % segments;
        DrawLine3D({ topCircle[i].x, topCircle[i].y, topCircle[i].z },
            { topCircle[next].x, topCircle[next].y, topCircle[next].z }, 1.0f, color);

        DrawLine3D({ bottomCircle[i].x, bottomCircle[i].y, bottomCircle[i].z },
            { bottomCircle[next].x, bottomCircle[next].y, bottomCircle[next].z }, 1.0f, color);

        // Connect top & bottom circle points
        DrawLine3D({ topCircle[i].x, topCircle[i].y, topCircle[i].z },
            { bottomCircle[i].x, bottomCircle[i].y, bottomCircle[i].z }, 1.0f, color);
    }
}


void View3D::BeginFrame() {
    boxVertices.clear();
    lineVertices.clear();

    boxVertices.reserve(MaxBoxes * 8);      // 8 vertices per box
    lineVertices.reserve(MaxLines);          // Conservative estimate
}

void View3D::EndFrame()
{
    ctx->VSSetShader(vs, nullptr, 0);
    ctx->PSSetShader(ps, nullptr, 0);
    ctx->IASetInputLayout(layout);

    CBObject obj;
    obj.world = XMMatrixTranspose(XMMatrixIdentity());
    ctx->UpdateSubresource(cbObject, 0, nullptr, &obj, 0, 0);
    ctx->VSSetConstantBuffers(0, 1, &cbObject);
    ctx->VSSetConstantBuffers(1, 1, &cbFrame);

    UINT numBoxes = (UINT)(boxVertices.size() / 8);
    if (numBoxes > 0)
    {
        D3D11_MAPPED_SUBRESOURCE mapped = {};
        ctx->Map(vbBox, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
        memcpy(mapped.pData, boxVertices.data(), boxVertices.size() * sizeof(Vertex));
        ctx->Unmap(vbBox, 0);

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        ctx->IASetVertexBuffers(0, 1, &vbBox, &stride, &offset);
        ctx->IASetIndexBuffer(ibBox, DXGI_FORMAT_R16_UINT, 0);
        ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        ctx->DrawIndexed(numBoxes * 36, 0, 0);
    }

    UINT numLines = (UINT)lineVertices.size();
    if (numLines > 0)
    {
        D3D11_MAPPED_SUBRESOURCE mapped = {};
        ctx->Map(vbLine, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
        memcpy(mapped.pData, lineVertices.data(), lineVertices.size() * sizeof(LineVertex));
        ctx->Unmap(vbLine, 0);

        UINT stride = sizeof(LineVertex);
        UINT offset = 0;
        ctx->IASetVertexBuffers(0, 1, &vbLine, &stride, &offset);
        ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

        ctx->Draw(numLines, 0);
    }
}



bool View3D::IsLookingAtCapsule(const XMFLOAT3& capsuleCenter, const XMFLOAT3& vMin, const XMFLOAT3& vMax, float radius) const
{
    // 1. Compute camera forward vector from cachedView
    XMVECTOR forward = XMVectorSet(
        cachedView.r[2].m128_f32[0],
        cachedView.r[2].m128_f32[1],
        cachedView.r[2].m128_f32[2],
        0.0f
    );
    forward = XMVector3Normalize(forward);

    XMVECTOR camPos = cameraPos;
    XMVECTOR capMin = XMLoadFloat3(&vMin);
    XMVECTOR capMax = XMLoadFloat3(&vMax);

    // 2. Closest point on capsule line segment to camera
    XMVECTOR seg = capMax - capMin;
    XMVECTOR pt = capMin;
    XMVECTOR camToMin = camPos - capMin;

    float t = XMVectorGetX(XMVector3Dot(camToMin, seg)) / XMVectorGetX(XMVector3Dot(seg, seg));
    if (t < 0.0f) t = 0.0f;
    else if (t > 1.0f) t = 1.0f;

    pt = capMin + seg * t;
    // 3. Vector from camera to closest point
    XMVECTOR toCapsule = pt - camPos;
    float distance = XMVectorGetX(XMVector3Length(toCapsule));

    if (distance < 0.001f)
        return true; // practically inside

    XMVECTOR dirToCapsule = XMVector3Normalize(toCapsule);

    // 4. Dot between camera forward and capsule direction
    float dot = XMVectorGetX(XMVector3Dot(forward, dirToCapsule));

    // 5. Consider radius, adjust field of view
    float radiusAngle = radius / distance; // small-angle approx
    return dot >= cosf(radiusAngle); // true if within capsule radius cone
}



void View3D::SetViewMatrix(const XMMATRIX& view) {
    cpuView = view;               // store for ToScreen
    cachedView = XMMatrixTranspose(view); // for shader

    CBFrame cbf{ cachedView, cachedProj };
    ctx->UpdateSubresource(cbFrame, 0, nullptr, &cbf, 0, 0);

    cameraPos = XMMatrixInverse(nullptr, view).r[3]; // extract camera position
}

void View3D::SetProjection(float width, float height) {
    Width = width;
    Height = height;
    float fov = XMConvertToRadians(90.0f);
    int aspect = static_cast<int>(Width / Height);

    cpuProj = XMMatrixPerspectiveFovLH(fov, aspect, 0.1f, 10000.0f); // for ToScreen
    cachedProj = XMMatrixTranspose(cpuProj); // for shader

    CBFrame cbf{ cachedView, cachedProj };
    ctx->UpdateSubresource(cbFrame, 0, nullptr, &cbf, 0, 0);
}




XMFLOAT2 View3D::ToScreen(const XMFLOAT3& worldPos)
{
    // Load world position
    XMVECTOR pos = XMLoadFloat3(&worldPos);

    // Combine CPU matrices (untransposed)
    XMMATRIX viewProj = cpuView * cpuProj;

    // Transform to clip space with W divide handled automatically
    XMVECTOR clip = XMVector3TransformCoord(pos, viewProj);

    XMFLOAT3 ndc;
    XMStoreFloat3(&ndc, clip);

    // Behind camera check
    if (ndc.z < 0.0f || ndc.z > 1.0f)
        return XMFLOAT2{ -1.0f, -1.0f };

    float screenX = (Width / 2.0f) + (ndc.x * Width / 2.0f);
    float screenY = (Height / 2.0f) - (ndc.y * Height / 2.0f);

    return XMFLOAT2{ screenX, screenY };
}


void View3D::DrawBox(Vector3 pos, Vector3 size, XMFLOAT4 color) {
    if (boxVertices.size() / 8 >= MaxBoxes) return; // prevent overflow

    XMMATRIX world = XMMatrixTranslation(pos.x, pos.y, pos.z);
    for (int i = 0; i < 8; i++) {
        XMFLOAT3 v = { (i & 1 ? -0.5f : 0.5f),(i & 2 ? -0.5f : 0.5f),(i & 4 ? -0.5f : 0.5f) };
        v.x *= size.x; v.y *= size.y; v.z *= size.z;

        XMVECTOR vec = XMVector3Transform(XMLoadFloat3(&v), world);
        XMFLOAT3 out;
        XMStoreFloat3(&out, vec);
        boxVertices.push_back({ out, color });
    }
}

void View3D::DrawWireBox(Vector3 pos, Vector3 size, XMFLOAT4 color)
{
    if (lineVertices.size() + 24 >= MaxLines) return;

    // Calculate 8 corners of the box
    XMMATRIX world = XMMatrixTranslation(pos.x, pos.y, pos.z);
    XMFLOAT3 corners[8];

    for (int i = 0; i < 8; i++)
    {
        XMFLOAT3 v = {
            (i & 1 ? -0.5f : 0.5f) * size.x,
            (i & 2 ? -0.5f : 0.5f) * size.y,
            (i & 4 ? -0.5f : 0.5f) * size.z
        };
        XMVECTOR vec = XMVector3Transform(XMLoadFloat3(&v), world);
        XMStoreFloat3(&corners[i], vec);
    }

    const int edges[12][2] = {
        {0,1}, {1,3}, {3,2}, {2,0}, // bottom face
        {4,5}, {5,7}, {7,6}, {6,4}, // top face
        {0,4}, {1,5}, {2,6}, {3,7}  // vertical edges
    };

    for (int i = 0; i < 12; i++)
    {
        lineVertices.push_back({ corners[edges[i][0]], color });
        lineVertices.push_back({ corners[edges[i][1]], color });
    }
}






void View3D::DrawThickLine3D(const XMFLOAT3& start, const XMFLOAT3& end, float thickness, const XMFLOAT4& color)
{
    // Direction vector
    XMVECTOR vStart = XMLoadFloat3(&start);
    XMVECTOR vEnd = XMLoadFloat3(&end);
    XMVECTOR dir = XMVectorSubtract(vEnd, vStart);

    // Length of the line
    float length = XMVectorGetX(XMVector3Length(dir));
    if (length <= 0.001f) return;

    // Midpoint of the line
    XMVECTOR mid = XMVectorAdd(vStart, XMVectorScale(dir, 0.5f));

    // Create rotation matrix to align box along dir
    XMVECTOR up = XMVectorSet(0, 0, 1, 0); // default box along Z
    XMVECTOR axis = XMVector3Cross(up, dir);
    float angle = XMVectorGetX(XMVector3AngleBetweenNormals(up, XMVector3Normalize(dir)));

    XMMATRIX rot = XMMatrixIdentity();
    if (XMVector3Length(axis).m128_f32[0] > 0.001f)
        rot = XMMatrixRotationAxis(XMVector3Normalize(axis), angle);

    // Scale box: thickness x thickness x length
    XMMATRIX scale = XMMatrixScaling(thickness, thickness, length);

    // Translate to midpoint
    XMMATRIX trans = XMMatrixTranslationFromVector(mid);

    // World matrix
    XMMATRIX world = scale * rot * trans;

    // Push vertices
    if (boxVertices.size() / 8 >= MaxBoxes) return;
    for (int i = 0; i < 8; i++)
    {
        XMFLOAT3 v = { (i & 1 ? -0.5f : 0.5f), (i & 2 ? -0.5f : 0.5f), (i & 4 ? -0.5f : 0.5f) };
        XMVECTOR vec = XMVector3Transform(XMLoadFloat3(&v), world);
        XMFLOAT3 out;
        XMStoreFloat3(&out, vec);
        boxVertices.push_back({ out, color });
    }
}
void View3D::DrawLine3DBold(Vector3 start, Vector3 end, XMFLOAT4 color, int repeats) {
    float offset = 0.001f; // tiny world-space offset
    for (int i = 0; i < repeats; i++) {
        Vector3 delta(offset * i, 0, 0);
        DrawLine3D({ start.x + delta.x,start.y + delta.y,start.z + delta.z }, { end.x + delta.x,end.y + delta.y,end.z + delta.z }, 1.0f, color);
    }
}

void View3D::DrawLine3D(XMFLOAT3 start, XMFLOAT3 end, float thickness, XMFLOAT4 color) {
    if (lineVertices.size() >= MaxLines) return;
    lineVertices.push_back({ start, color });
    lineVertices.push_back({ end, color });
}

void View3D::Render(ID3D11RenderTargetView* rtv) {
    ctx->OMSetRenderTargets(1, &rtv, nullptr);
    D3D11_VIEWPORT vp = {};
    vp.Width = Width; vp.Height = Height;
    vp.MinDepth = 0; vp.MaxDepth = 1; vp.TopLeftX = 0; vp.TopLeftY = 0;
    ctx->RSSetViewports(1, &vp);
}
