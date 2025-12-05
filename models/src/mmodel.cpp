#include "mmodel.hpp"

#include <vector>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <fmt/core.h>

namespace over {

unsigned int TextureFromFile(const char* path, const std::string& directory,
                             bool gamma = false);

MModel::MModel(const std::string& path) {
  LoadModel(path);
}

void MModel::Draw(Shader& shader) {
  for (auto& mesh : meshes_) {
    mesh.Draw(shader);
  }
}

void MModel::LoadModel(const std::string& path) {
  Assimp::Importer importer;
  const aiScene* scene =
      importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
      !scene->mRootNode) {
    fmt::println("error::assimp::{}", importer.GetErrorString());
    return;
  }

  directory = path.substr(0, path.find_last_of('/'));

  ProcessNode(scene->mRootNode, scene);
}

void MModel::ProcessNode(aiNode* node, const aiScene* scene) {
  for (size_t i = 0; i < node->mNumMeshes; i++) {
    auto* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes_.push_back(ProcessMesh(mesh, scene));
  }

  for (size_t i = 0; i < node->mNumChildren; i++) {
    ProcessNode(node->mChildren[i], scene);
  }
}

MMesh MModel::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<MVertex> vertices;
  std::vector<MElement> elements;
  std::vector<MTexture> textures;

  for (std::size_t i = 0; i < mesh->mNumVertices; i++) {
    MVertex vertex;

    auto aiVertex = mesh->mVertices[i];
    vertex.position = glm::vec3(aiVertex.x, aiVertex.y, aiVertex.z);

    auto aiNormal = mesh->mNormals[i];
    vertex.normal = glm::vec3(aiNormal.x, aiNormal.y, aiNormal.z);

    if (mesh->mTextureCoords[0]) {
      auto aiTexCoord = mesh->mTextureCoords[0][i];
      vertex.texCoord = glm::vec2(aiTexCoord.x, aiTexCoord.y);
    }

    vertices.emplace_back(vertex);
  }

  for (std::size_t i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    elements.emplace_back(
        MElement(face.mIndices[0], face.mIndices[1], face.mIndices[2]));
  }

  if (mesh->mMaterialIndex >= 0) {
    auto* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<MTexture> diffuse = LoadMaterialTextures(
        material, aiTextureType_DIFFUSE, MTexture::RType::DIFFUSE);
    textures.insert(textures.end(), diffuse.begin(), diffuse.end());
    std::vector<MTexture> specular = LoadMaterialTextures(
        material, aiTextureType_SPECULAR, MTexture::RType::SPECULAR);
    textures.insert(textures.end(), specular.begin(), specular.end());
  }

  return MMesh(vertices, elements, textures);
}

std::vector<MTexture> MModel::LoadMaterialTextures(aiMaterial* material,
                                                   aiTextureType assimpType,
                                                   MTexture::RType overType) {
  std::vector<MTexture> textures;
  for (std::size_t i = 0; i < material->GetTextureCount(assimpType); i++) {
    aiString str;
    material->GetTexture(assimpType, static_cast<unsigned int>(i), &str);
    std::string value = str.C_Str();
    bool skip = loadedTextures_.find(value) != loadedTextures_.end();
    if (skip) {
      continue;
    }
    loadedTextures_.insert(value);

    MTexture texture;
    texture.id = TextureFromFile(str.C_Str(), directory);
    texture.type = overType;
    texture.path = value;
    textures.emplace_back(texture);
  }
  return textures;
}

unsigned int TextureFromFile(const char* path, const std::string& directory,
                             bool gamma) {
  std::string filename = std::string(path);
  filename = directory + '/' + filename;

  unsigned int textureID;
  glGenTextures(1, &textureID);

  stbi_set_flip_vertically_on_load(true);

  int width, height, nrComponents;
  unsigned char* data =
      stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    fmt::println("Texture failed to load at path: {}", path);
    stbi_image_free(data);
  }

  return textureID;
}

}  // namespace over
