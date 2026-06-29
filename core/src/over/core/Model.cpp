#include <over/core/Model.hpp>

#include <vector>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <stb_image.h>

#include <fmt/core.h>

namespace over {
Model::Model(const std::string& path)
    : _directory(), _transform(), _paths(), _wrappers(), _meshes() {
  LoadModel(path);
}

void Model::Draw(Shader& shader) {
  shader.SetMatrix4f("camera.model", _transform.GetModel());
  for (auto& mesh : _meshes) {
    mesh.Draw(shader);
  }
}

void Model::Draw() {
  Shader shader = Shader::GetCurrent();
  Draw(shader);
}

void Model::LoadModel(const std::string& path) {
  Assimp::Importer importer;
  const aiScene* scene =
      importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
      !scene->mRootNode) {
    throw std::runtime_error(
        fmt::format("error::assimp::{}", importer.GetErrorString()));
  }

  _directory = path.substr(0, path.find_last_of('/'));

  ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
  for (size_t i = 0; i < node->mNumMeshes; i++) {
    auto* mesh = scene->mMeshes[node->mMeshes[i]];
    _meshes.push_back(ProcessMesh(mesh, scene));
  }

  for (size_t i = 0; i < node->mNumChildren; i++) {
    ProcessNode(node->mChildren[i], scene);
  }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<Vertex> vertices;
  std::vector<Element> elements;
  std::vector<MeshTexture> textures;

  for (usize i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;

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

  for (usize i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    elements.emplace_back(
        Element(face.mIndices[0], face.mIndices[1], face.mIndices[2]));
  }

  if (mesh->mMaterialIndex >= 0) {
    auto* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<MeshTexture> diffuse = LoadMaterialTextures(
        material, aiTextureType_DIFFUSE, MeshTexture::Type::DIFFUSE);
    textures.insert(textures.end(), diffuse.begin(), diffuse.end());
    std::vector<MeshTexture> specular = LoadMaterialTextures(
        material, aiTextureType_SPECULAR, MeshTexture::Type::SPECULAR);
    textures.insert(textures.end(), specular.begin(), specular.end());
  }

  return Mesh(vertices, elements, textures);
}

std::vector<MeshTexture> Model::LoadMaterialTextures(
    aiMaterial* material, aiTextureType assimpType,
    MeshTexture::Type overType) {
  std::vector<MeshTexture> textures;
  for (usize i = 0; i < material->GetTextureCount(assimpType); i++) {
    aiString str;
    material->GetTexture(assimpType, static_cast<unsigned int>(i), &str);

    std::string path = str.C_Str();
    if (_paths.count(path) == 0) {
      _paths.emplace(path, _wrappers.size());

      stbi_set_flip_vertically_on_load(true);

      auto img = host::Image2D::FromFile(_directory + '/' + str.C_Str());

      _wrappers.emplace_back(
          gl::TextureWrapper<>::FromImage2D<gl::TextureTarget::TEXTURE_2D>(
              img));

      _wrappers.back().As<gl::TextureTarget::TEXTURE_2D>(
          [&](gl::Texture2DView& self) {
            self.GenerateMipmap();

            self.SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
            self.SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
            self.SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            self.SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          });
    }

    MeshTexture texture;
    texture.view = _wrappers[_paths[path]].As<gl::TextureTarget::TEXTURE_2D>();
    texture.type = overType;

    textures.emplace_back(texture);
  }
  return textures;
}

}  // namespace over
