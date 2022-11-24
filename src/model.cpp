/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Model classes for importing with Assimp                             ##
##        From following tutorials on learnopengl.com                         ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <QFileInfo>

#include <abstractscene.h>
#include <model.h>
#include <resourcemanager.h>
#include <texture.h>

using namespace Qtk;

Model::ModelManager Model::mManager;

// Static function to access ModelManager for getting Models by name
Model * Model::getInstance(const char * name) {
  return mManager[name];
}

/*******************************************************************************
 * ModelMesh Private Member Functions
 ******************************************************************************/

void ModelMesh::initMesh(const char * vert, const char * frag) {
  initializeOpenGLFunctions();

  // Create VAO, VBO, EBO
  mVAO->create();
  mVBO->create();
  mEBO->create();

  mVAO->bind();

  // Allocate VBO
  mVBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
  mVBO->bind();

  mVBO->allocate(mVertices.data(), mVertices.size() * sizeof(mVertices[0]));

  // Allocate EBO
  mEBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
  mEBO->bind();
  mEBO->allocate(mIndices.data(), mIndices.size() * sizeof(mIndices[0]));
  mEBO->release();

  // Load and link shaders
  mProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vert);
  mProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, frag);
  mProgram->link();
  mProgram->bind();

  // Positions
  mProgram->enableAttributeArray(0);
  mProgram->setAttributeBuffer(
      0, GL_FLOAT, offsetof(ModelVertex, mPosition), 3, sizeof(ModelVertex));

  // Normals
  mProgram->enableAttributeArray(1);
  mProgram->setAttributeBuffer(
      1, GL_FLOAT, offsetof(ModelVertex, mNormal), 3, sizeof(ModelVertex));

  // Texture Coordinates
  mProgram->enableAttributeArray(2);
  mProgram->setAttributeBuffer(
      2, GL_FLOAT, offsetof(ModelVertex, mTextureCoord), 2,
      sizeof(ModelVertex));

  // Vertex tangents
  mProgram->enableAttributeArray(3);
  mProgram->setAttributeBuffer(
      3, GL_FLOAT, offsetof(ModelVertex, mTangent), 3, sizeof(ModelVertex));

  // Vertex bitangents
  mProgram->enableAttributeArray(4);
  mProgram->setAttributeBuffer(
      4, GL_FLOAT, offsetof(ModelVertex, mBitangent), 3, sizeof(ModelVertex));

  mProgram->release();
  mVBO->release();
  mVAO->release();
}

/*******************************************************************************
 * ModelMesh Public Member Functions
 ******************************************************************************/

void ModelMesh::draw(QOpenGLShaderProgram & shader) {
  mVAO->bind();
  // Bind shader
  shader.bind();

  // Set Model View Projection values
  shader.setUniformValue("uModel", mTransform.toMatrix());
  shader.setUniformValue("uView", Scene::View());
  shader.setUniformValue("uProjection", Scene::Projection());

  GLuint diffuseCount = 1;
  GLuint specularCount = 1;
  GLuint normalCount = 1;
  for(GLuint i = 0; i < mTextures.size(); i++) {
    // Activate the current texture index by adding offset to GL_TEXTURE0
    glActiveTexture(GL_TEXTURE0 + i);
    mTextures[i].mTexture->bind();

    // Get a name for the texture using a known convention -
    // Diffuse:   material.texture_diffuse1, material.texture_diffuse2, ...
    // Specular:   material.texture_specular1, material.texture_specular2, ...
    std::string number;
    std::string name = mTextures[i].mType;
    if(name == "texture_diffuse") {
      number = std::to_string(diffuseCount++);
    }
    if(name == "texture_specular") {
      number = std::to_string(specularCount++);
    }
    if(name == "texture_normal") {
      number = std::to_string(normalCount++);
    }

    // Set the uniform to track this texture ID using our naming convention
    shader.setUniformValue((name + number).c_str(), i);
  }

  // Draw the mesh
  glDrawElements(
      GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, mIndices.data());

  // Release shader, textures
  for(const auto & texture : mTextures) {
    texture.mTexture->release();
  }
  shader.release();
  mVAO->release();
  glActiveTexture(GL_TEXTURE0);
}

/*******************************************************************************
 * Model Public Member Functions
 ******************************************************************************/

void Model::draw() {
  for(auto & mMeshe : mMeshes) {
    mMeshe.mTransform = mTransform;
    mMeshe.draw();
  }
}

void Model::draw(QOpenGLShaderProgram & shader) {
  for(auto & mMeshe : mMeshes) {
    mMeshe.mTransform = mTransform;
    mMeshe.draw(shader);
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

/*******************************************************************************
 * Model Private Member Functions
 ******************************************************************************/

/**
 * Loads a model in .obj, .fbx, .gltf, and other formats
 * For a full list of formats see assimp documentation:
 *  https://github.com/assimp/assimp/blob/master/doc/Fileformats.md
 *
 * Models should not be loaded into Qt resource system
 * Instead pass an *absolute* path to this function
 * Relative paths will break if Qtk is executed from different locations
 *
 * Models can also be loaded from the `qtk/resource` directory using qrc format
 *  loadModel(":/models/backpack/backpack.obj")
 * See resourcemanager.h for more information
 *
 * @param path Absolute path to a model .obj or other format accepted by assimp
 */
void Model::loadModel(const std::string & path) {
  Assimp::Importer import;

  // JIC a relative path was used, get the absolute file path
  QFileInfo info(path.c_str());
  info.makeAbsolute();
  mDirectory = path[0] == ':' ? RM::getPath(path)
                              : info.absoluteFilePath().toStdString();

  // Import the model, converting non-triangular geometry to triangles
  // + And flipping texture UVs, etc..
  // Assimp options: http://assimp.sourceforge.net/lib_html/postprocess_8h.html
  const aiScene * scene = import.ReadFile(
      mDirectory, aiProcess_Triangulate | aiProcess_FlipUVs
                      | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace
                      | aiProcess_OptimizeMeshes | aiProcess_SplitLargeMeshes);

  // If there were errors, print and return
  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    qDebug() << "Error::ASSIMP::" << import.GetErrorString() << "\n";
    return;
  }
  // If there were no errors, find the directory that contains this model
  mDirectory = mDirectory.substr(0, mDirectory.find_last_of('/'));

  // Pass the pointers to the root node and the scene to recursive function
  // + Base case breaks when no nodes left to process on model
  processNode(scene->mRootNode, scene);

  // Sort models by their distance from the camera
  // Optimizes drawing so that overlapping objects are not overwritten
  // + Since the topmost object will be drawn first
  sortModels();

  // Object finished loading, insert it into ModelManager
  mManager.insert(mName, this);
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

  return {vertices, indices, textures, mVertexShader, mFragmentShader};
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
      texture.mTexture = OpenGLTextureFactory::initTexture2D(
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

void Model::sortModels() {
  auto cameraPos = Scene::Camera().transform();
  auto cameraDistance = [&cameraPos](const ModelMesh & a, const ModelMesh & b) {
    // Sort by the first vertex position in the model
    return (cameraPos.getTranslation().distanceToPoint(
               a.mVertices[0].mPosition))
           < (cameraPos.getTranslation().distanceToPoint(
               b.mVertices[0].mPosition));
  };
  std::sort(mMeshes.begin(), mMeshes.end(), cameraDistance);
}
