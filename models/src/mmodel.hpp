#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "mmesh.hpp"
#include "shader.hpp"

namespace over {
class MModel {
 public:
  MModel(const std::string& path);

  ~MModel();

  void Draw(Shader& shader);

 private:
  std::vector<MMesh> meshes_;
  std::string directory;
  std::unordered_map<std::string, MTexture> loadedTextures_;

  void LoadModel(const std::string& path);
  void ProcessNode(aiNode* node, const aiScene* scene);
  MMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<MTexture> LoadMaterialTextures(aiMaterial* material,
                                             aiTextureType assimpType,
                                             MTexture::RType overType);
};
}  // namespace over
