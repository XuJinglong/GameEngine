#include "StaticMesh.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

void StaticMesh::LoadFromFile(const char* FileName)
{
	Assimp::Importer MeshImporter;
	auto AssimpScene = MeshImporter.ReadFile(FileName, aiProcess_Triangulate);
    if (!AssimpScene || AssimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !AssimpScene->mMeshes)
    {
        return;
    }
    for (int i = 0; i < AssimpScene->mNumMeshes; i++) 
    {
        auto CurMesh = AssimpScene->mMeshes[i];
        if (CurMesh) 
        {
            auto Vertices = CurMesh->mVertices;
            for (int j = 0; j < CurMesh->mNumVertices; j++) 
            {
                //get uv
                auto uvList0 = CurMesh->mTextureCoords[0];
                auto uvChannelNum0 = CurMesh->mNumUVComponents[0];
                auto UV = Vector2D(0, 0);
                if (uvList0 && uvChannelNum0 == 2) 
                {
                    UV = Vector2D(uvList0[j].x, uvList0[j].y);
                }

                //get normal
                auto normal = Vector3D(0, 0, 0);
                if (CurMesh->HasNormals()) 
                {
                    normal = Vector3D(CurMesh->mNormals[j].x, CurMesh->mNormals[j].y, CurMesh->mNormals[j].z);
                }
                //make vertex
                m_Vertexs.push_back({ Vector3D(Vertices[j].x * 10, Vertices[j].y * 10, (Vertices[j].z * 10)), normal, UV});
            }

            for (int p = 0; p < CurMesh->mNumFaces; p++) 
            {
                auto AssimpFace = CurMesh->mFaces[p];
                for (int q = 0; q < AssimpFace.mNumIndices; q++) 
                {
                    m_Indexs.push_back(AssimpFace.mIndices[q]);
                }
            }
        }
        break;
    }
}

void StaticMesh::GetVertexData(uint8_t** VertexBuffer, uint32_t& VertexBSize, uint8_t** IndexBuffer, uint32_t& IndexBSize)
{
    if (!m_Vertexs.empty() && !m_Indexs.empty())
    {
        *VertexBuffer = (uint8_t*)&m_Vertexs[0];
        VertexBSize = m_Vertexs.size() * sizeof(Vertex);
        *IndexBuffer = (uint8_t*)&m_Indexs[0];
        IndexBSize = m_Indexs.size() * 4;
    }
}
