#pragma once
#include "EngineCommon.h"

class StaticMesh
{
public:
    StaticMesh() {}
    ~StaticMesh() {}

    void LoadFromFile(const char* FileName);
    void GetVertexData(uint8_t** VertexBuffer, uint32_t& VertexBSize, uint8_t** IndexBuffer, uint32_t& IndexBSize);
public:
    struct Vertex
    {
        Vector3D Position;
        Vector3D Normal;
        Vector2D UV;
    };
private:
    FArray<Vertex> m_Vertexs;
    FArray<uint32_t> m_Indexs;
    //顶点布局类型
};

