#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <over/core/Mesh.hpp>
#include <over/core/Shader.hpp>
#include <over/core/Transform.hpp>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

namespace over {
class Model {
 public:
  Model(const std::string& path);

  ~Model();

  void Draw(Shader& shader);

  Transform& GetTransform() noexcept { return _transform; }

  std::vector<Mesh>& GetMeshes() noexcept { return _meshes; };
  const std::vector<Mesh>& GetMeshes() const noexcept { return _meshes; }

 private:
  Transform _transform;
  std::vector<Mesh> _meshes;
  std::string _directory;

  mutable std::unordered_map<std::string, Texture> _loadedTextures;

  void LoadModel(const std::string& path);
  void ProcessNode(aiNode* node, const aiScene* scene);
  Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> LoadMaterialTextures(aiMaterial* material,
                                            aiTextureType assimpType,
                                            Texture::Type overType);
};
}  // namespace over
