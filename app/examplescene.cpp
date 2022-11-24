/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Classes for managing objects and data within a scene                ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <abstractscene.h>
#include <camera3d.h>
#include <examplescene.h>
#include <meshrenderer.h>
#include <model.h>
#include <resourcemanager.h>
#include <texture.h>

using namespace Qtk;

/*******************************************************************************
 * Constructors, Destructors
 ******************************************************************************/

ExampleScene::ExampleScene() {
  Camera().transform().setTranslation(0.0f, 0.0f, 20.0f);
  Camera().transform().setRotation(-5.0f, 0.0f, 1.0f, 0.0f);
}

ExampleScene::~ExampleScene() {
  delete mTestPhong;
  delete mTestSpecular;
  delete mTestDiffuse;
  delete mTestAmbient;
  for(auto & mesh : mMeshes) {
    delete mesh;
  }
  for(auto & model : mModels) {
    delete model;
  }
  delete mSkybox;
}

/*******************************************************************************
 * Public Member Functions
 ******************************************************************************/

void ExampleScene::init() {
  auto * sb = new Qtk::Skybox("Skybox");
  setSkybox(sb);

  // Initialize Phong example cube
  mTestPhong = new Qtk::MeshRenderer("phong", Qtk::Cube());
  mTestPhong->mTransform.setTranslation(3.0f, 0.0f, -2.0f);
  mTestPhong->setShaders(":/solid-phong.vert", ":/solid-phong.frag");

  // You no longer need to manually bind shader program.
  // + But, you can still bind it if you want to.
  //  mTestPhong->bindShaders();
  mTestPhong->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  mTestPhong->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
  mTestPhong->setUniform("uAmbientStrength", 0.2f);
  mTestPhong->setUniform("uSpecularStrength", 0.50f);
  mTestPhong->setUniform("uSpecularShine", 256);
  mTestPhong->reallocateNormals(mTestPhong->getNormals());
  //  mTestPhong->releaseShaders();

  // Initialize Ambient example cube
  mTestAmbient = new Qtk::MeshRenderer("ambient", Cube());
  mTestAmbient->mTransform.setTranslation(7.0f, 0.0f, -2.0f);
  mTestAmbient->setShaders(":/solid-ambient.vert", ":/solid-ambient.frag");
  mTestAmbient->init();
  mTestAmbient->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  mTestAmbient->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
  mTestAmbient->setUniform("uAmbientStrength", 0.2f);
  mTestAmbient->reallocateNormals(mTestAmbient->getNormals());

  // Initialize Diffuse example cube
  mTestDiffuse = new Qtk::MeshRenderer("diffuse", Cube());
  mTestDiffuse->mTransform.setTranslation(9.0f, 0.0f, -2.0f);
  mTestDiffuse->setShaders(":/solid-diffuse.vert", ":/solid-diffuse.frag");
  mTestDiffuse->init();
  mTestDiffuse->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  mTestDiffuse->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
  mTestDiffuse->setUniform("uAmbientStrength", 0.2f);
  mTestDiffuse->reallocateNormals(mTestDiffuse->getNormals());

  // Initialize Specular example cube
  mTestSpecular = new Qtk::MeshRenderer("specular", Cube());
  mTestSpecular->mTransform.setTranslation(11.0f, 0.0f, -2.0f);
  mTestSpecular->setShaders(":/solid-specular.vert", ":/solid-specular.frag");
  mTestSpecular->init();
  mTestSpecular->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  mTestSpecular->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
  mTestSpecular->setUniform("uAmbientStrength", 0.2f);
  mTestSpecular->setUniform("uSpecularStrength", 0.50f);
  mTestSpecular->setUniform("uSpecularShine", 256);
  mTestSpecular->reallocateNormals(mTestSpecular->getNormals());


  //
  // Model loading

  mModels.push_back(
      new Qtk::Model("backpack", ":/models/backpack/backpack.obj"));
  // Sometimes model textures need flipped in certain directions
  mModels.back()->flipTexture("diffuse.jpg", false, true);
  mModels.back()->mTransform.setTranslation(0.0f, 0.0f, -10.0f);

  mModels.push_back(new Qtk::Model("bird", ":/models/bird/bird.obj"));
  mModels.back()->mTransform.setTranslation(2.0f, 2.0f, -10.0f);
  // Sometimes the models are very large
  mModels.back()->mTransform.scale(0.0025f);
  mModels.back()->mTransform.rotate(-110.0f, 0.0f, 1.0f, 0.0f);

  mModels.push_back(new Qtk::Model("lion", ":/models/lion/lion.obj"));
  mModels.back()->mTransform.setTranslation(-3.0f, -1.0f, -10.0f);
  mModels.back()->mTransform.scale(0.15f);

  mModels.push_back(
      new Qtk::Model("alien", ":/models/alien-hominid/alien.obj"));
  mModels.back()->mTransform.setTranslation(2.0f, -1.0f, -5.0f);
  mModels.back()->mTransform.scale(0.15f);

  mModels.push_back(new Qtk::Model("scythe", ":/models/scythe/scythe.obj"));
  mModels.back()->mTransform.setTranslation(-6.0f, 0.0f, -10.0f);
  mModels.back()->mTransform.rotate(-90.0f, 1.0f, 0.0f, 0.0f);
  mModels.back()->mTransform.rotate(90.0f, 0.0f, 1.0f, 0.0f);

  mModels.push_back(
      new Qtk::Model("masterChief", ":/models/spartan/spartan.obj"));
  mModels.back()->mTransform.setTranslation(-1.5f, 0.5f, -2.0f);

  //
  // Building example mesh objects

  // Render an alien with specular
  // Test alien Model with phong lighting and specular mapping
  mMeshes.push_back(new Qtk::MeshRenderer(
      "alienTestLight", Triangle(Qtk::QTK_DRAW_ELEMENTS)));
  mMeshes.back()->mTransform.setTranslation(4.0f, 1.5f, 10.0f);
  mMeshes.back()->mTransform.scale(0.25f);
  // This function changes values we have allocated in a buffer, so init() after
  mMeshes.back()->setColor(GREEN);

  mModels.push_back(new Qtk::Model(
      "alienTest", ":/models/alien-hominid/alien.obj", ":/model-specular.vert",
      ":/model-specular.frag"));
  mModels.back()->mTransform.setTranslation(3.0f, -1.0f, 10.0f);
  mModels.back()->mTransform.scale(0.15f);
  mModels.back()->setUniform("uMaterial.ambient", QVector3D(1.0f, 1.0f, 1.0f));
  mModels.back()->setUniform("uMaterial.diffuse", QVector3D(1.0f, 1.0f, 1.0f));
  mModels.back()->setUniform("uMaterial.specular", QVector3D(1.0f, 1.0f, 1.0f));
  mModels.back()->setUniform("uMaterial.ambientStrength", 0.8f);
  mModels.back()->setUniform("uMaterial.diffuseStrength", 0.8f);
  mModels.back()->setUniform("uMaterial.specularStrength", 1.0f);
  mModels.back()->setUniform("uMaterial.shine", 32.0f);

  mModels.back()->setUniform("uLight.ambient", QVector3D(1.0f, 1.0f, 1.0f));
  mModels.back()->setUniform("uLight.diffuse", QVector3D(1.0f, 1.0f, 1.0f));
  mModels.back()->setUniform("uLight.specular", QVector3D(1.0f, 1.0f, 1.0f));

  // Test spartan Model with phong lighting, specular and normal mapping
  mMeshes.push_back(
      new Qtk::MeshRenderer("spartanTestLight", Triangle(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->mTransform.setTranslation(1.0f, 1.5f, 10.0f);
  mMeshes.back()->mTransform.scale(0.25f);
  // This function changes values we have allocated in a buffer, so init() after
  mMeshes.back()->setColor(GREEN);

  mModels.push_back(new Qtk::Model(
      "spartanTest", ":/models/spartan/spartan.obj", ":/model-normals.vert",
      ":/model-normals.frag"));
  mModels.back()->mTransform.setTranslation(0.0f, -1.0f, 10.0f);
  mModels.back()->mTransform.scale(2.0f);
  mModels.back()->setUniform("uMaterial.ambient", QVector3D(1.0f, 1.0f, 1.0f));
  mModels.back()->setUniform("uMaterial.diffuse", QVector3D(1.0f, 1.0f, 1.0f));
  mModels.back()->setUniform("uMaterial.specular", QVector3D(1.0f, 1.0f, 1.0f));
  mModels.back()->setUniform("uMaterial.ambientStrength", 1.0f);
  mModels.back()->setUniform("uMaterial.diffuseStrength", 1.0f);
  mModels.back()->setUniform("uMaterial.specularStrength", 1.0f);
  mModels.back()->setUniform("uMaterial.shine", 128.0f);
  mModels.back()->setUniform("uLight.ambient", QVector3D(1.0f, 1.0f, 1.0f));
  mModels.back()->setUniform("uLight.diffuse", QVector3D(1.0f, 1.0f, 1.0f));
  mModels.back()->setUniform("uLight.specular", QVector3D(1.0f, 1.0f, 1.0f));

  // Test basic cube with phong.vert and phong.frag shaders
  mMeshes.push_back(
      new Qtk::MeshRenderer("testLight", Triangle(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->mTransform.setTranslation(5.0f, 1.25f, 10.0f);
  mMeshes.back()->mTransform.scale(0.25f);
  mMeshes.back()->setDrawType(GL_LINE_LOOP);
  // This function changes values we have allocated in a buffer, so init() after
  mMeshes.back()->setColor(GREEN);

  mMeshes.push_back(new Qtk::MeshRenderer("testPhong", Cube(QTK_DRAW_ARRAYS)));
  mMeshes.back()->mTransform.setTranslation(5.0f, 0.0f, 10.0f);
  mMeshes.back()->setShaders(":/phong.vert", ":/phong.frag");
  mMeshes.back()->setColor(QVector3D(0.0f, 0.25f, 0.0f));
  mMeshes.back()->reallocateNormals(mMeshes.back()->getNormals());

  mMeshes.back()->setUniform("uMaterial.ambient", QVector3D(0.0f, 0.3f, 0.0f));
  mMeshes.back()->setUniform("uMaterial.diffuse", QVector3D(0.0f, 0.2f, 0.0f));
  mMeshes.back()->setUniform("uMaterial.specular", QVector3D(1.0f, 1.0f, 1.0f));
  mMeshes.back()->setUniform("uMaterial.ambientStrength", 1.0f);
  mMeshes.back()->setUniform("uMaterial.diffuseStrength", 1.0f);
  mMeshes.back()->setUniform("uMaterial.specularStrength", 1.0f);
  mMeshes.back()->setUniform("uMaterial.shine", 64.0f);
  mMeshes.back()->setUniform("uLight.ambient", QVector3D(0.25f, 0.2f, 0.075f));
  mMeshes.back()->setUniform("uLight.diffuse", QVector3D(0.75f, 0.6f, 0.22f));
  mMeshes.back()->setUniform("uLight.specular", QVector3D(0.62f, 0.55f, 0.37f));
  mMeshes.back()->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));


  //
  // Create simple shapes using MeshRenderer class and data in mesh.h

  mMeshes.push_back(
      new Qtk::MeshRenderer("rightTriangle", Triangle(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->mTransform.setTranslation(-5.0f, 0.0f, -2.0f);

  mMeshes.push_back(
      new Qtk::MeshRenderer("centerCube", Cube(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->mTransform.setTranslation(-7.0f, 0.0f, -2.0f);

  mMeshes.push_back(
      new Qtk::MeshRenderer("leftTriangle", Triangle(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->mTransform.setTranslation(-9.0f, 0.0f, -2.0f);
  mMeshes.back()->setDrawType(GL_LINE_LOOP);

  mMeshes.push_back(
      new Qtk::MeshRenderer("topTriangle", Triangle(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->mTransform.setTranslation(-7.0f, 2.0f, -2.0f);
  mMeshes.back()->mTransform.scale(0.25f);

  mMeshes.push_back(
      new Qtk::MeshRenderer("bottomTriangle", Triangle(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->mTransform.setTranslation(-7.0f, -2.0f, -2.0f);
  mMeshes.back()->mTransform.scale(0.25f);
  mMeshes.back()->setDrawType(GL_LINE_LOOP);
  // This function changes values we have allocated in a buffer, so init() after
  mMeshes.back()->setColor(GREEN);

  //
  // Testing for normals, texture coordinates

  // RGB Normals cube to show normals are correct with QTK_DRAW_ARRAYS
  mMeshes.push_back(
      new Qtk::MeshRenderer("rgbNormalsCubeArraysTest", Cube(QTK_DRAW_ARRAYS)));
  mMeshes.back()->mTransform.setTranslation(5.0f, 0.0f, 4.0f);
  mMeshes.back()->setShaders(":/rgb-normals.vert", ":/rgb-normals.frag");
  mMeshes.back()->reallocateNormals(mMeshes.back()->getNormals());

  // RGB Normals cube to show normals are correct with QTK_DRAW_ELEMENTS_NORMALS
  mMeshes.push_back(new Qtk::MeshRenderer(
      "rgbNormalsCubeElementsTest", Cube(QTK_DRAW_ELEMENTS_NORMALS)));
  mMeshes.back()->mTransform.setTranslation(5.0f, 0.0f, 2.0f);
  mMeshes.back()->setShaders(":/rgb-normals.vert", ":/rgb-normals.frag");
  mMeshes.back()->reallocateNormals(mMeshes.back()->getNormals());

  Texture crateTexture;
  crateTexture.setTexture(":/crate.png");
  Cube cube;
  auto * m = new MeshRenderer("Test Crate", Cube(QTK_DRAW_ARRAYS));
  m->mTransform.setTranslation(0, 0, 13);
  m->setShaders(":/texture2d.vert", ":/texture2d.frag");
  m->setTexture(crateTexture);
  m->setUniform("uTexture", 0);
  m->reallocateTexCoords(cube.getTexCoords());
  mMeshes.push_back(m);

  // Texturing a cube using texture coordinates and glDrawArrays
  // + Texturing with UVs using glDrawElements requires
  // QTK_DRAW_ELEMENTS_NORMALS
  // + UVs required duplicating element position data from QTK_DRAW_ELEMENTS
  // + This is because the same position must use different UV coordinates
  mMeshes.push_back(
      new Qtk::MeshRenderer("uvCubeArraysTest", Cube(QTK_DRAW_ARRAYS)));
  mMeshes.back()->mTransform.setTranslation(-3.0f, 0.0f, -2.0f);
  mMeshes.back()->setShaders(":/texture2d.vert", ":/texture2d.frag");
  mMeshes.back()->setTexture(crateTexture);
  mMeshes.back()->setUniform("uTexture", 0);
  mMeshes.back()->reallocateTexCoords(mMeshes.back()->getTexCoords());

  // Test drawing a cube with texture coordinates using glDrawElements
  mMeshes.push_back(new Qtk::MeshRenderer(
      "uvCubeElementsTest", Cube(QTK_DRAW_ELEMENTS_NORMALS)));
  mMeshes.back()->mTransform.setTranslation(-1.7f, 0.0f, -2.0f);
  mMeshes.back()->setTexture(":/crate.png");
  mMeshes.back()->setShaders(":/texture2d.vert", ":/texture2d.frag");
  mMeshes.back()->bindShaders();
  mMeshes.back()->setUniform("uTexture", 0);
  mMeshes.back()->reallocateNormals(mMeshes.back()->getNormals());
  mMeshes.back()->reallocateTexCoords(mMeshes.back()->getTexCoords(), 3);
  mMeshes.back()->releaseShaders();
  mMeshes.back()->mTransform.rotate(45.0f, 0.0f, 1.0f, 0.0f);

  // Texturing a cube using a cube map
  // + Cube map texturing works with both QTK_DRAW_ARRAYS and QTK_DRAW_ELEMENTS
  mMeshes.push_back(
      new Qtk::MeshRenderer("testCubeMap", Cube(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->mTransform.setTranslation(-3.0f, 1.0f, -2.0f);
  mMeshes.back()->mTransform.setRotation(45.0f, 0.0f, 1.0f, 0.0f);
  mMeshes.back()->setShaders(
      ":/texture-cubemap.vert", ":/texture-cubemap.frag");
  mMeshes.back()->setCubeMap(":/crate.png");
  mMeshes.back()->setUniform("uTexture", 0);
  mMeshes.back()->reallocateTexCoords(mMeshes.back()->getTexCoords());

  // Create a cube with custom shaders
  // + Apply RGB normals shader and spin the cube for a neat effect
  mMeshes.push_back(
      new Qtk::MeshRenderer("rgbNormalsCube", Cube(QTK_DRAW_ARRAYS)));
  mMeshes.back()->mTransform.setTranslation(5.0f, 2.0f, -2.0f);
  mMeshes.back()->setShaders(":/rgb-normals.vert", ":/rgb-normals.frag");
  mMeshes.back()->reallocateNormals(mMeshes.back()->getNormals());

  // RGB Normals triangle to show normals are correct with QTK_DRAW_ARRAYS
  mMeshes.push_back(new Qtk::MeshRenderer(
      "rgbTriangleArraysTest", Triangle(QTK_DRAW_ARRAYS)));
  mMeshes.back()->mTransform.setTranslation(7.0f, 0.0f, 2.0f);
  mMeshes.back()->setShaders(":/rgb-normals.vert", ":/rgb-normals.frag");
  mMeshes.back()->reallocateNormals(mMeshes.back()->getNormals());

  // RGB Normals triangle to show normals are correct with QTK_DRAW_ELEMENTS
  mMeshes.push_back(new Qtk::MeshRenderer(
      "rgbTriangleElementsTest", Triangle(QTK_DRAW_ELEMENTS_NORMALS)));
  mMeshes.back()->mTransform.setTranslation(7.0f, 0.0f, 4.0f);
  mMeshes.back()->setShaders(":/rgb-normals.vert", ":/rgb-normals.frag");
  mMeshes.back()->reallocateNormals(mMeshes.back()->getNormals());

  // Test drawing triangle with glDrawArrays with texture coordinates
  mMeshes.push_back(
      new Qtk::MeshRenderer("testTriangleArraysUV", Triangle(QTK_DRAW_ARRAYS)));
  mMeshes.back()->mTransform.setTranslation(-3.0f, 2.0f, -2.0f);
  mMeshes.back()->setShaders(":/texture2d.vert", ":/texture2d.frag");

  mMeshes.back()->setTexture(":/crate.png");
  mMeshes.back()->setUniform("uTexture", 0);
  mMeshes.back()->reallocateTexCoords(mMeshes.back()->getTexCoords());

  // Test drawing triangle with glDrawElements with texture coordinates
  mMeshes.push_back(new Qtk::MeshRenderer(
      "testTriangleElementsUV", Triangle(QTK_DRAW_ELEMENTS_NORMALS)));
  mMeshes.back()->mTransform.setTranslation(-2.5f, 0.0f, -1.0f);
  mMeshes.back()->setShaders(":/texture2d.vert", ":/texture2d.frag");
  mMeshes.back()->setTexture(":/crate.png");
  mMeshes.back()->setUniform("uTexture", 0);
  mMeshes.back()->reallocateTexCoords(mMeshes.back()->getTexCoords());

  //
  // Lighting cube examples

  // Example of a cube with no lighting applied
  mMeshes.push_back(new Qtk::MeshRenderer("noLight", Cube(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->mTransform.setTranslation(5.0f, 0.0f, -2.0f);
  mMeshes.back()->setShaders(
      ":/solid-perspective.vert", ":/solid-perspective.frag");
  mMeshes.back()->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));

  // Create objects that represent light sources for lighting examples
  mMeshes.push_back(
      new Qtk::MeshRenderer("phongLight", Triangle(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->mTransform.setTranslation(3.0f, 2.0f, -2.0f);
  mMeshes.back()->mTransform.scale(0.25f);

  mMeshes.push_back(
      new Qtk::MeshRenderer("diffuseLight", Triangle(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->mTransform.setTranslation(9.0f, 2.0f, -2.0f);
  mMeshes.back()->mTransform.scale(0.25f);

  mMeshes.push_back(
      new Qtk::MeshRenderer("specularLight", Triangle(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->mTransform.setTranslation(11.0f, 2.0f, -2.0f);
  mMeshes.back()->mTransform.scale(0.25f);
}

void ExampleScene::draw() {
  Scene::draw();

  for(const auto & model : mModels) {
    model->draw();
  }

  for(const auto & mesh : mMeshes) {
    mesh->draw();
  }

  mTestPhong->bindShaders();
  mTestPhong->setUniform(
      "uModelInverseTransposed",
      mTestPhong->mTransform.toMatrix().normalMatrix());
  mTestPhong->setUniform(
      "uLightPosition",
      MeshRenderer::getInstance("phongLight")->mTransform.getTranslation());
  mTestPhong->setUniform(
      "uCameraPosition", ExampleScene::Camera().transform().getTranslation());
  mTestPhong->releaseShaders();
  mTestPhong->draw();

  mTestAmbient->bindShaders();
  mTestAmbient->setUniform(
      "uCameraPosition", ExampleScene::Camera().transform().getTranslation());
  mTestAmbient->releaseShaders();
  mTestAmbient->draw();

  mTestDiffuse->bindShaders();
  mTestDiffuse->setUniform(
      "uModelInverseTransposed",
      mTestDiffuse->mTransform.toMatrix().normalMatrix());
  mTestDiffuse->setUniform(
      "uLightPosition",
      MeshRenderer::getInstance("diffuseLight")->mTransform.getTranslation());
  mTestDiffuse->setUniform(
      "uCameraPosition", ExampleScene::Camera().transform().getTranslation());
  mTestDiffuse->releaseShaders();
  mTestDiffuse->draw();

  mTestSpecular->bindShaders();
  mTestSpecular->setUniform(
      "uModelInverseTransposed",
      mTestSpecular->mTransform.toMatrix().normalMatrix());
  mTestSpecular->setUniform(
      "uLightPosition",
      MeshRenderer::getInstance("specularLight")->mTransform.getTranslation());
  mTestSpecular->setUniform(
      "uCameraPosition", ExampleScene::Camera().transform().getTranslation());
  mTestSpecular->releaseShaders();
  mTestSpecular->draw();
}

void ExampleScene::update() {
  auto position =
      MeshRenderer::getInstance("alienTestLight")->mTransform.getTranslation();
  auto alien = Model::getInstance("alienTest");
  alien->setUniform("uLight.position", position);
  alien->setUniform(
      "uCameraPosition", ExampleScene::Camera().transform().getTranslation());
  auto posMatrix = alien->mTransform.toMatrix();
  alien->setUniform("uMVP.normalMatrix", posMatrix.normalMatrix());
  alien->setUniform("uMVP.model", posMatrix);
  alien->setUniform("uMVP.view", ExampleScene::Camera().toMatrix());
  alien->setUniform("uMVP.projection", ExampleScene::Projection());
  alien->mTransform.rotate(0.75f, 0.0f, 1.0f, 0.0f);

  position = MeshRenderer::getInstance("spartanTestLight")
                 ->mTransform.getTranslation();
  auto spartan = Model::getInstance("spartanTest");
  spartan->setUniform("uLight.position", position);
  spartan->setUniform(
      "uCameraPosition", ExampleScene::Camera().transform().getTranslation());
  posMatrix = spartan->mTransform.toMatrix();
  spartan->setUniform("uMVP.normalMatrix", posMatrix.normalMatrix());
  spartan->setUniform("uMVP.model", posMatrix);
  spartan->setUniform("uMVP.view", ExampleScene::Camera().toMatrix());
  spartan->setUniform("uMVP.projection", ExampleScene::Projection());
  spartan->mTransform.rotate(0.75f, 0.0f, 1.0f, 0.0f);

  auto phong = MeshRenderer::getInstance("testPhong");
  phong->mTransform.rotate(0.75f, 1.0f, 0.5f, 0.0f);
  phong->bindShaders();
  position =
      MeshRenderer::getInstance("testLight")->mTransform.getTranslation();
  phong->setUniform("uLight.position", position);
  phong->setUniform(
      "uCameraPosition", ExampleScene::Camera().transform().getTranslation());
  posMatrix = phong->mTransform.toMatrix();
  phong->setUniform("uMVP.normalMatrix", posMatrix.normalMatrix());
  phong->setUniform("uMVP.model", posMatrix);
  phong->setUniform("uMVP.view", ExampleScene::Camera().toMatrix());
  phong->setUniform("uMVP.projection", ExampleScene::Projection());
  phong->releaseShaders();

  // Rotate lighting example cubes
  mTestPhong->mTransform.rotate(0.75f, 0.5f, 0.3f, 0.2f);
  MeshRenderer::getInstance("noLight")->mTransform.rotate(
      0.75f, 0.5f, 0.3f, 0.2f);
  mTestAmbient->mTransform.rotate(0.75f, 0.5f, 0.3f, 0.2f);
  mTestDiffuse->mTransform.rotate(0.75f, 0.5f, 0.3f, 0.2f);
  mTestSpecular->mTransform.rotate(0.75f, 0.5f, 0.3f, 0.2f);

  // Examples of various translations and rotations

  // Rotate in multiple directions simultaneously
  MeshRenderer::getInstance("rgbNormalsCube")
      ->mTransform.rotate(0.75f, 0.2f, 0.4f, 0.6f);

  // Pitch forward and roll sideways
  MeshRenderer::getInstance("leftTriangle")
      ->mTransform.rotate(0.75f, 1.0f, 0.0f, 0.0f);
  MeshRenderer::getInstance("rightTriangle")
      ->mTransform.rotate(0.75f, 0.0f, 0.0f, 1.0f);

  // Move between two positions over time
  static float translateX = 0.025f;
  float limit = -9.0f;  // Origin position.x - 2.0f
  float posX =
      MeshRenderer::getInstance("topTriangle")->mTransform.getTranslation().x();
  if(posX < limit || posX > limit + 4.0f) {
    translateX = -translateX;
  }
  MeshRenderer::getInstance("topTriangle")
      ->mTransform.translate(translateX, 0.0f, 0.0f);
  MeshRenderer::getInstance("bottomTriangle")
      ->mTransform.translate(-translateX, 0.0f, 0.0f);
  // And lets rotate the triangles in two directions at once
  MeshRenderer::getInstance("topTriangle")
      ->mTransform.rotate(0.75f, 0.2f, 0.0f, 0.4f);
  MeshRenderer::getInstance("bottomTriangle")
      ->mTransform.rotate(0.75f, 0.0f, 0.2f, 0.4f);
  // And make the bottom triangle green, instead of RGB

  // Rotate center cube in several directions simultaneously
  // + Not subject to gimbal lock since we are using quaternions :)
  MeshRenderer::getInstance("centerCube")
      ->mTransform.rotate(0.75f, 0.2f, 0.4f, 0.6f);
}
