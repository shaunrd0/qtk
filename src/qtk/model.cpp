/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Model class for importing with Assimp                               ##
##        From following tutorials on learnopengl.com                         ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include "model.h"
#include "qtkiosystem.h"
#include "scene.h"
#include "texture.h"

using namespace Qtk;

/** Static QHash used to store and access models globally. */
Model::ModelManager Model::mManager;

/*******************************************************************************
 * Public Member Functions
 ******************************************************************************/

void Model::draw() {
  for(auto & mesh : mMeshes) {
    mesh.mTransform = mTransform;
    mesh.draw();
  }
}

void Model::draw(QOpenGLShaderProgram & shader) {
  for(auto & mesh : mMeshes) {
    mesh.mTransform = mTransform;
    mesh.draw(shader);
  }
}

void Model::flipTexture(const std::string & fileName, bool flipX, bool flipY) {
  bool modified = false;
  std::string fullPath = mDirectory + '/' + fileName;
  for(auto & texture : mTexturesLoaded) {
    if(texture.mPath == fileName) {
      texture.mTexture->destroy();
      texture.mTexture->create();
      texture.mTexture->setData(
          *OpenGLTextureFactory::initImage(fullPath.c_str(), flipX, flipY));
      modified = true;
    }
  }
  if(!modified) {
    qDebug() << "Attempt to flip texture that doesn't exist: "
             << fullPath.c_str() << "\n";
  }
}

// Static function to access ModelManager for getting Models by name
Model * Qtk::Model::getInstance(const char * name) {
  return mManager[name];
}

/*******************************************************************************
 * Private Member Functions
 ******************************************************************************/

void Model::loadModel(const std::string & path) {
  Assimp::Importer import;
  // If using a Qt Resource path, use QtkIOSystem for file handling.
  if(path.front() == ':') {
    import.SetIOHandler(new QtkIOSystem());
  }
  // Used as base path for loading model textures.
  mDirectory = path.substr(0, path.find_last_of('/'));

  // Import the model, converting non-triangular geometry to triangles
  // + And flipping texture UVs, etc..
  // Assimp options: http://assimp.sourceforge.net/lib_html/postprocess_8h.html
  const aiScene * scene = import.ReadFile(
      path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs
                        | aiProcess_GenSmoothNormals
                        | aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes
                        | aiProcess_SplitLargeMeshes);

  // If there were errors, print and return
  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    qDebug() << "Error::ASSIMP::" << import.GetErrorString() << "\n";
    return;
  }

  // Pass the pointers to the root node and the scene to recursive function
  // + Base case breaks when no nodes left to process on model
  processNode(scene->mRootNode, scene);

  // Sort models by their distance from the camera
  // Optimizes drawing so that overlapping objects are not overwritten
  // + Since the topmost object will be drawn first
  sortModelMeshes();

  // Object finished loading, insert it into ModelManager
  mManager.insert(getName().c_str(), this);
}

void Model::processNode(aiNode * node, const aiScene * scene) {
  // Process each mesh that is available for this node
  for(GLuint i = 0; i < node->mNumMeshes; i++) {
    aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
    mMeshes.push_back(processMesh(mesh, scene));
  }

  // Process each child node for this mesh using recursion
  for(GLuint i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

ModelMesh Model::processMesh(aiMesh * mesh, const aiScene * scene) {
  ModelMesh::Vertices vertices;
  ModelMesh::Indices indices;
  ModelMesh::Textures textures;

  // For each vertex in the aiMesh
  for(GLuint i = 0; i < mesh->mNumVertices; i++) {
    // Create a local vertex object for positions, normals, and texture coords
    ModelVertex vertex;

    // Reuse this vector to initialize positions and normals
    QVector3D vector3D;

    // Initialize vertex position
    vector3D.setX(mesh->mVertices[i].x);
    vector3D.setY(mesh->mVertices[i].y);
    vector3D.setZ(mesh->mVertices[i].z);
    // Set the position of our local vertex to the local vector object
    vertex.mPosition = vector3D;

    if(mesh->HasNormals()) {
      // Initialize vertex normal
      vector3D.setX(mesh->mNormals[i].x);
      vector3D.setY(mesh->mNormals[i].y);
      vector3D.setZ(mesh->mNormals[i].z);
      // Set the normals of our local vertex to the local vector object
      vertex.mNormal = vector3D;
    }

    // Initialize texture coordinates, if any are available
    if(mesh->mTextureCoords[0]) {
      QVector2D vector2D;
      // Texture coordinates
      vector2D.setX(mesh->mTextureCoords[0][i].x);
      vector2D.setY(mesh->mTextureCoords[0][i].y);
      vertex.mTextureCoord = vector2D;

      // Tangents
      vector3D.setX(mesh->mTangents[i].x);
      vector3D.setY(mesh->mTangents[i].y);
      vector3D.setZ(mesh->mTangents[i].z);
      vertex.mTangent = vector3D;

      // Bitangents
      vector3D.setX(mesh->mBitangents[i].x);
      vector3D.setY(mesh->mBitangents[i].y);
      vector3D.setZ(mesh->mBitangents[i].z);
      vertex.mBitangent = vector3D;
    } else {
      vertex.mTextureCoord = {0.0f, 0.0f};
    }

    // Add the initialized vertex to our container of vertices
    vertices.push_back(vertex);
  }

  // For each face on the mesh, process its indices
  for(GLuint i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for(GLuint j = 0; j < face.mNumIndices; j++) {
      // Add the index to out container of indices
      indices.push_back(face.mIndices[j]);
    }
  }

  // Process material
  if(mesh->mMaterialIndex >= 0) {
    // Get the material attached to the model using Assimp
    aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];
    // Get all diffuse textures from the material
    ModelMesh::Textures diffuseMaps = loadMaterialTextures(
        material, aiTextureType_DIFFUSE, "texture_diffuse");
    // Insert all diffuse textures found into our textures container
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // Get all specular textures from the material
    ModelMesh::Textures specularMaps = loadMaterialTextures(
        material, aiTextureType_SPECULAR, "texture_specular");
    // Insert all specular textures found into our textures container
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // Get all normal textures from the material
    ModelMesh::Textures normalMaps =
        loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    // Insert all normal maps found into our textures container
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
  }

  return {
      vertices, indices, textures, mVertexShader.c_str(),
      mFragmentShader.c_str()};
}

ModelMesh::Textures Model::loadMaterialTextures(
    aiMaterial * mat, aiTextureType type, const std::string & typeName) {
  ModelMesh::Textures textures;

  for(GLuint i = 0; i < mat->GetTextureCount(type); i++) {
    // Call GetTexture to get the name of the texture file to load
    aiString fileName;
    mat->GetTexture(type, i, &fileName);

    // Check if we have already loaded this texture
    bool skip = false;
    for(auto & j : mTexturesLoaded) {
      // If the path to the texture already exists in m_texturesLoaded, skip it
      if(std::strcmp(j.mPath.data(), fileName.C_Str()) == 0) {
        textures.push_back(j);
        // If we have loaded the texture, do not load it again
        skip = true;
        break;
      }
    }

    // If the texture has not yet been loaded
    if(!skip) {
      ModelTexture texture;
      texture.mTexture = OpenGLTextureFactory::initTexture(
          std::string(mDirectory + '/' + fileName.C_Str()).c_str(), false,
          false);
      texture.mID = texture.mTexture->textureId();
      texture.mType = typeName;
      texture.mPath = fileName.C_Str();
      // Add the texture to the textures container
      textures.push_back(texture);
      // Add the texture to the loaded textures to avoid loading it twice
      mTexturesLoaded.push_back(texture);
    }
  }

  // Return the resulting textures
  return textures;
}

void Model::sortModelMeshes() {
  auto cameraPos = Scene::getCamera().getTransform();
  auto cameraDistance = [&cameraPos](const ModelMesh & a, const ModelMesh & b) {
    // Sort by the first vertex position in the model
    return (cameraPos.getTranslation().distanceToPoint(
               a.mVertices[0].mPosition))
           < (cameraPos.getTranslation().distanceToPoint(
               b.mVertices[0].mPosition));
  };
  std::sort(mMeshes.begin(), mMeshes.end(), cameraDistance);
}
