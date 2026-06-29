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

  ~Model() = default;

  void Draw(Shader& shader);
  void Draw();

  Transform& GetTransform() noexcept { return _transform; }

  std::vector<Mesh>& GetMeshes() noexcept { return _meshes; };
  const std::vector<Mesh>& GetMeshes() const noexcept { return _meshes; }

 private:
  void LoadModel(const std::string& path);
  void ProcessNode(aiNode* node, const aiScene* scene);
  Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* material,
                                                aiTextureType assimpType,
                                                MeshTexture::Type overType);

  std::string _directory;
  Transform _transform;

  mutable std::unordered_map<std::string, usize> _paths;
  mutable std::vector<gl::TextureWrapper<>> _wrappers;

  std::vector<Mesh> _meshes;
};
}  // namespace over
