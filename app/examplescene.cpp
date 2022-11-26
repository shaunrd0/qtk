/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Classes for managing objects and data within a scene                ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <camera3d.h>
#include <examplescene.h>
#include <meshrenderer.h>
#include <model.h>
#include <resourcemanager.h>
#include <scene.h>
#include <texture.h>

using namespace Qtk;

/*******************************************************************************
 * Constructors, Destructors
 ******************************************************************************/

ExampleScene::ExampleScene() {
  getCamera().getTransform().setTranslation(0.0f, 0.0f, 20.0f);
  getCamera().getTransform().setRotation(-5.0f, 0.0f, 1.0f, 0.0f);
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
  // Add a skybox to the scene using default cube map images and settings.
  setSkybox(new Qtk::Skybox("Skybox"));

  /* Create a red cube with a mini master chief on top. */
  auto myCube = new MeshRenderer("My cube", Cube(Qtk::QTK_DRAW_ELEMENTS));
  myCube->setColor(RED);
  mMeshes.push_back(myCube);

  auto mySpartan = new Model("My spartan", ":/models/spartan/spartan.obj");
  mySpartan->getTransform().setTranslation(0.0f, 0.5f, 0.0f);
  mySpartan->getTransform().setScale(0.5f);
  mModels.push_back(mySpartan);

  //
  // Create simple shapes using MeshRenderer class and data in mesh.h

  mMeshes.push_back(
      new Qtk::MeshRenderer("rightTriangle", Triangle(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->getTransform().setTranslation(-5.0f, 0.0f, -2.0f);

  mMeshes.push_back(
      new Qtk::MeshRenderer("centerCube", Cube(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->getTransform().setTranslation(-7.0f, 0.0f, -2.0f);

  mMeshes.push_back(
      new Qtk::MeshRenderer("leftTriangle", Triangle(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->getTransform().setTranslation(-9.0f, 0.0f, -2.0f);
  mMeshes.back()->setDrawType(GL_LINE_LOOP);

  mMeshes.push_back(
      new Qtk::MeshRenderer("topTriangle", Triangle(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->getTransform().setTranslation(-7.0f, 2.0f, -2.0f);
  mMeshes.back()->getTransform().scale(0.25f);

  mMeshes.push_back(
      new Qtk::MeshRenderer("bottomTriangle", Triangle(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->getTransform().setTranslation(-7.0f, -2.0f, -2.0f);
  mMeshes.back()->getTransform().scale(0.25f);
  mMeshes.back()->setDrawType(GL_LINE_LOOP);
  mMeshes.back()->setColor(GREEN);


  //
  // 3D Model loading

  mModels.push_back(
      new Qtk::Model("backpack", ":/models/backpack/backpack.obj"));
  // Sometimes model textures need flipped in certain directions
  mModels.back()->flipTexture("diffuse.jpg", false, true);
  mModels.back()->getTransform().setTranslation(0.0f, 0.0f, -10.0f);

  mModels.push_back(new Qtk::Model("bird", ":/models/bird/bird.obj"));
  mModels.back()->getTransform().setTranslation(2.0f, 2.0f, -10.0f);
  // Sometimes the models are very large
  mModels.back()->getTransform().scale(0.0025f);
  mModels.back()->getTransform().rotate(-110.0f, 0.0f, 1.0f, 0.0f);

  mModels.push_back(new Qtk::Model("lion", ":/models/lion/lion.obj"));
  mModels.back()->getTransform().setTranslation(-3.0f, -1.0f, -10.0f);
  mModels.back()->getTransform().scale(0.15f);

  mModels.push_back(
      new Qtk::Model("alien", ":/models/alien-hominid/alien.obj"));
  mModels.back()->getTransform().setTranslation(2.0f, -1.0f, -5.0f);
  mModels.back()->getTransform().scale(0.15f);

  mModels.push_back(new Qtk::Model("scythe", ":/models/scythe/scythe.obj"));
  mModels.back()->getTransform().setTranslation(-6.0f, 0.0f, -10.0f);
  mModels.back()->getTransform().rotate(-90.0f, 1.0f, 0.0f, 0.0f);
  mModels.back()->getTransform().rotate(90.0f, 0.0f, 1.0f, 0.0f);

  mModels.push_back(
      new Qtk::Model("masterChief", ":/models/spartan/spartan.obj"));
  mModels.back()->getTransform().setTranslation(-1.5f, 0.5f, -2.0f);


  //
  // Simple cube lighting examples.

  /* Phong lighting example on a basic cube. */
  mTestPhong = new Qtk::MeshRenderer("phong", Qtk::Cube());
  mTestPhong->getTransform().setTranslation(3.0f, 0.0f, -2.0f);
  // NOTE: You no longer need to manually bind shader program to set uniforms.
  // + You can still bind it if you want to for performance reasons.
  // + Qtk will only bind / release if the shader program is not already bound.
  mTestPhong->setShaders(":/solid-phong.vert", ":/solid-phong.frag");

  // For example this would technically not be efficient, because each one of
  // these calls will bind, set, release. We could instead bind, set N uniforms,
  // and release when we are finished.
  //  mTestPhong->bindShaders();
  mTestPhong->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  mTestPhong->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
  mTestPhong->setUniform("uAmbientStrength", 0.2f);
  mTestPhong->setUniform("uSpecularStrength", 0.50f);
  mTestPhong->setUniform("uSpecularShine", 256);
  //  mTestPhong->releaseShaders();
  mTestPhong->reallocateNormals(mTestPhong->getNormals());
  // NOTE: This is only an example and I won't worry about this kind of
  // efficiency while initializing the following objects.

  // Phong lighting example light source. This is just for visual reference.
  // + We refer to the position of this object in draw() to update lighting.
  mMeshes.push_back(
      new Qtk::MeshRenderer("phongLight", Triangle(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->getTransform().setTranslation(3.0f, 2.0f, -2.0f);
  mMeshes.back()->getTransform().scale(0.25f);

  /* Example of a cube with no lighting applied */
  mMeshes.push_back(new Qtk::MeshRenderer("noLight", Cube(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->getTransform().setTranslation(5.0f, 0.0f, -2.0f);
  mMeshes.back()->setShaders(
      ":/solid-perspective.vert", ":/solid-perspective.frag");
  mMeshes.back()->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  // No light source needed for this lighting technique

  /* Initialize Ambient example cube */
  mTestAmbient = new Qtk::MeshRenderer("ambient", Cube());
  mTestAmbient->getTransform().setTranslation(7.0f, 0.0f, -2.0f);
  mTestAmbient->setShaders(":/solid-ambient.vert", ":/solid-ambient.frag");
  // Changing these uniform values will alter lighting effects.
  mTestAmbient->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  mTestAmbient->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
  mTestAmbient->setUniform("uAmbientStrength", 0.2f);
  mTestAmbient->reallocateNormals(mTestAmbient->getNormals());
  // No light source needed for this lighting technique

  /* Initialize Diffuse example cube */
  mTestDiffuse = new Qtk::MeshRenderer("diffuse", Cube());
  mTestDiffuse->getTransform().setTranslation(9.0f, 0.0f, -2.0f);
  mTestDiffuse->setShaders(":/solid-diffuse.vert", ":/solid-diffuse.frag");
  mTestDiffuse->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  mTestDiffuse->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
  mTestDiffuse->setUniform("uAmbientStrength", 0.2f);
  mTestDiffuse->reallocateNormals(mTestDiffuse->getNormals());

  // Diffuse lighting example light source. This is just for visual reference.
  mMeshes.push_back(
      new Qtk::MeshRenderer("diffuseLight", Triangle(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->getTransform().setTranslation(9.0f, 2.0f, -2.0f);
  mMeshes.back()->getTransform().scale(0.25f);

  /* Initialize Specular example cube */
  mTestSpecular = new Qtk::MeshRenderer("specular", Cube());
  mTestSpecular->getTransform().setTranslation(11.0f, 0.0f, -2.0f);
  mTestSpecular->setShaders(":/solid-specular.vert", ":/solid-specular.frag");
  mTestSpecular->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  mTestSpecular->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
  mTestSpecular->setUniform("uAmbientStrength", 0.2f);
  mTestSpecular->setUniform("uSpecularStrength", 0.50f);
  mTestSpecular->setUniform("uSpecularShine", 256);
  mTestSpecular->reallocateNormals(mTestSpecular->getNormals());

  // Specular lighting example light source. This is just for visual reference.
  mMeshes.push_back(
      new Qtk::MeshRenderer("specularLight", Triangle(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->getTransform().setTranslation(11.0f, 2.0f, -2.0f);
  mMeshes.back()->getTransform().scale(0.25f);


  /* Test basic cube with phong.vert and phong.frag shaders */
  mMeshes.push_back(new Qtk::MeshRenderer("testPhong", Cube(QTK_DRAW_ARRAYS)));
  mMeshes.back()->getTransform().setTranslation(5.0f, 0.0f, 10.0f);
  mMeshes.back()->setShaders(":/phong.vert", ":/phong.frag");
  // WARNING: Set color before reallocating normals.
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

  // Light source for testPhong cube
  mMeshes.push_back(
      new Qtk::MeshRenderer("testLight", Triangle(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->getTransform().setTranslation(5.0f, 1.25f, 10.0f);
  mMeshes.back()->getTransform().scale(0.25f);
  mMeshes.back()->setDrawType(GL_LINE_LOOP);
  mMeshes.back()->setColor(RED);


  //
  // Building more complex objects for showing examples of lighting techniques

  /* Test alien Model with phong lighting and specular mapping. */
  mModels.push_back(new Qtk::Model(
      "alienTest", ":/models/alien-hominid/alien.obj", ":/model-specular.vert",
      ":/model-specular.frag"));
  mModels.back()->getTransform().setTranslation(3.0f, -1.0f, 10.0f);
  mModels.back()->getTransform().scale(0.15f);
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

  // Light source for alienTest object.
  mMeshes.push_back(new Qtk::MeshRenderer(
      "alienTestLight", Triangle(Qtk::QTK_DRAW_ELEMENTS)));
  mMeshes.back()->getTransform().setTranslation(4.0f, 1.5f, 10.0f);
  mMeshes.back()->getTransform().scale(0.25f);
  // This function changes values we have allocated in a buffer, so init() after
  mMeshes.back()->setColor(GREEN);

  /* Test spartan Model with phong lighting, specular and normal mapping. */
  mModels.push_back(new Qtk::Model(
      "spartanTest", ":/models/spartan/spartan.obj", ":/model-normals.vert",
      ":/model-normals.frag"));
  mModels.back()->getTransform().setTranslation(0.0f, -1.0f, 10.0f);
  mModels.back()->getTransform().scale(2.0f);
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

  // Light source for spartanTest object.
  mMeshes.push_back(
      new Qtk::MeshRenderer("spartanTestLight", Triangle(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->getTransform().setTranslation(1.0f, 1.5f, 10.0f);
  mMeshes.back()->getTransform().scale(0.25f);
  // This function changes values we have allocated in a buffer, so init() after
  mMeshes.back()->setColor(GREEN);


  //
  // Test drawing simple geometry with various OpenGL drawing modes

  // RGB Normals cube to show normals are correct with QTK_DRAW_ARRAYS
  mMeshes.push_back(
      new Qtk::MeshRenderer("rgbNormalsCubeArraysTest", Cube(QTK_DRAW_ARRAYS)));
  mMeshes.back()->getTransform().setTranslation(5.0f, 0.0f, 4.0f);
  mMeshes.back()->setShaders(":/rgb-normals.vert", ":/rgb-normals.frag");
  mMeshes.back()->reallocateNormals(mMeshes.back()->getNormals());

  // RGB Normals cube to show normals are correct with QTK_DRAW_ELEMENTS_NORMALS
  mMeshes.push_back(new Qtk::MeshRenderer(
      "rgbNormalsCubeElementsTest", Cube(QTK_DRAW_ELEMENTS_NORMALS)));
  mMeshes.back()->getTransform().setTranslation(5.0f, 0.0f, 2.0f);
  mMeshes.back()->setShaders(":/rgb-normals.vert", ":/rgb-normals.frag");
  mMeshes.back()->reallocateNormals(mMeshes.back()->getNormals());

  Texture crateTexture;
  crateTexture.setTexture(":/crate.png");
  Cube cube;
  auto * m = new MeshRenderer("Test Crate", Cube(QTK_DRAW_ARRAYS));
  m->getTransform().setTranslation(0, 0, 13);
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
  mMeshes.back()->getTransform().setTranslation(-3.0f, 0.0f, -2.0f);
  mMeshes.back()->setShaders(":/texture2d.vert", ":/texture2d.frag");
  mMeshes.back()->setTexture(crateTexture);
  mMeshes.back()->setUniform("uTexture", 0);
  mMeshes.back()->reallocateTexCoords(mMeshes.back()->getTexCoords());

  // Test drawing a cube with texture coordinates using glDrawElements
  mMeshes.push_back(new Qtk::MeshRenderer(
      "uvCubeElementsTest", Cube(QTK_DRAW_ELEMENTS_NORMALS)));
  mMeshes.back()->getTransform().setTranslation(-1.7f, 0.0f, -2.0f);
  mMeshes.back()->setTexture(":/crate.png");
  mMeshes.back()->setShaders(":/texture2d.vert", ":/texture2d.frag");
  mMeshes.back()->bindShaders();
  mMeshes.back()->setUniform("uTexture", 0);
  mMeshes.back()->reallocateNormals(mMeshes.back()->getNormals());
  mMeshes.back()->reallocateTexCoords(mMeshes.back()->getTexCoords(), 3);
  mMeshes.back()->releaseShaders();
  mMeshes.back()->getTransform().rotate(45.0f, 0.0f, 1.0f, 0.0f);

  // Texturing a cube using a cube map
  // + Cube map texturing works with both QTK_DRAW_ARRAYS and QTK_DRAW_ELEMENTS
  mMeshes.push_back(
      new Qtk::MeshRenderer("testCubeMap", Cube(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->getTransform().setTranslation(-3.0f, 1.0f, -2.0f);
  mMeshes.back()->getTransform().setRotation(45.0f, 0.0f, 1.0f, 0.0f);
  mMeshes.back()->setShaders(
      ":/texture-cubemap.vert", ":/texture-cubemap.frag");
  mMeshes.back()->setCubeMap(":/crate.png");
  mMeshes.back()->setUniform("uTexture", 0);
  mMeshes.back()->reallocateTexCoords(mMeshes.back()->getTexCoords());

  // Create a cube with custom shaders
  // + Apply RGB normals shader and spin the cube for a neat effect
  mMeshes.push_back(
      new Qtk::MeshRenderer("rgbNormalsCube", Cube(QTK_DRAW_ARRAYS)));
  mMeshes.back()->getTransform().setTranslation(5.0f, 2.0f, -2.0f);
  mMeshes.back()->setShaders(":/rgb-normals.vert", ":/rgb-normals.frag");
  mMeshes.back()->reallocateNormals(mMeshes.back()->getNormals());

  // RGB Normals triangle to show normals are correct with QTK_DRAW_ARRAYS
  mMeshes.push_back(new Qtk::MeshRenderer(
      "rgbTriangleArraysTest", Triangle(QTK_DRAW_ARRAYS)));
  mMeshes.back()->getTransform().setTranslation(7.0f, 0.0f, 2.0f);
  mMeshes.back()->setShaders(":/rgb-normals.vert", ":/rgb-normals.frag");
  mMeshes.back()->reallocateNormals(mMeshes.back()->getNormals());

  // RGB Normals triangle to show normals are correct with QTK_DRAW_ELEMENTS
  mMeshes.push_back(new Qtk::MeshRenderer(
      "rgbTriangleElementsTest", Triangle(QTK_DRAW_ELEMENTS_NORMALS)));
  mMeshes.back()->getTransform().setTranslation(7.0f, 0.0f, 4.0f);
  mMeshes.back()->setShaders(":/rgb-normals.vert", ":/rgb-normals.frag");
  mMeshes.back()->reallocateNormals(mMeshes.back()->getNormals());

  // Test drawing triangle with glDrawArrays with texture coordinates
  mMeshes.push_back(
      new Qtk::MeshRenderer("testTriangleArraysUV", Triangle(QTK_DRAW_ARRAYS)));
  mMeshes.back()->getTransform().setTranslation(-3.0f, 2.0f, -2.0f);
  mMeshes.back()->setShaders(":/texture2d.vert", ":/texture2d.frag");

  mMeshes.back()->setTexture(":/crate.png");
  mMeshes.back()->setUniform("uTexture", 0);
  mMeshes.back()->reallocateTexCoords(mMeshes.back()->getTexCoords());

  // Test drawing triangle with glDrawElements with texture coordinates
  mMeshes.push_back(new Qtk::MeshRenderer(
      "testTriangleElementsUV", Triangle(QTK_DRAW_ELEMENTS_NORMALS)));
  mMeshes.back()->getTransform().setTranslation(-2.5f, 0.0f, -1.0f);
  mMeshes.back()->setShaders(":/texture2d.vert", ":/texture2d.frag");
  mMeshes.back()->setTexture(":/crate.png");
  mMeshes.back()->setUniform("uTexture", 0);
  mMeshes.back()->reallocateTexCoords(mMeshes.back()->getTexCoords());
}

void ExampleScene::draw() {
  // WARNING: We must call the base class draw() function first.
  // + This will handle rendering core scene components like the Skybox.
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
      mTestPhong->getTransform().toMatrix().normalMatrix());
  mTestPhong->setUniform(
      "uLightPosition",
      MeshRenderer::getInstance("phongLight")->getTransform().getTranslation());
  mTestPhong->setUniform(
      "uCameraPosition",
      ExampleScene::getCamera().getTransform().getTranslation());
  mTestPhong->releaseShaders();
  mTestPhong->draw();

  mTestAmbient->bindShaders();
  mTestAmbient->setUniform(
      "uCameraPosition",
      ExampleScene::getCamera().getTransform().getTranslation());
  mTestAmbient->releaseShaders();
  mTestAmbient->draw();

  mTestDiffuse->bindShaders();
  mTestDiffuse->setUniform(
      "uModelInverseTransposed",
      mTestDiffuse->getTransform().toMatrix().normalMatrix());
  mTestDiffuse->setUniform(
      "uLightPosition", MeshRenderer::getInstance("diffuseLight")
                            ->getTransform()
                            .getTranslation());
  mTestDiffuse->setUniform(
      "uCameraPosition",
      ExampleScene::getCamera().getTransform().getTranslation());
  mTestDiffuse->releaseShaders();
  mTestDiffuse->draw();

  mTestSpecular->bindShaders();
  mTestSpecular->setUniform(
      "uModelInverseTransposed",
      mTestSpecular->getTransform().toMatrix().normalMatrix());
  mTestSpecular->setUniform(
      "uLightPosition", MeshRenderer::getInstance("specularLight")
                            ->getTransform()
                            .getTranslation());
  mTestSpecular->setUniform(
      "uCameraPosition",
      ExampleScene::getCamera().getTransform().getTranslation());
  mTestSpecular->releaseShaders();
  mTestSpecular->draw();
}

void ExampleScene::update() {
  auto mySpartan = Model::getInstance("My spartan");
  mySpartan->getTransform().rotate(0.75f, 0.0f, 1.0f, 0.0f);

  auto myCube = MeshRenderer::getInstance("My cube");
  myCube->getTransform().rotate(-0.75f, 0.0f, 1.0f, 0.0f);

  auto position = MeshRenderer::getInstance("alienTestLight")
                      ->getTransform()
                      .getTranslation();
  auto alien = Model::getInstance("alienTest");
  alien->setUniform("uLight.position", position);
  alien->setUniform(
      "uCameraPosition",
      ExampleScene::getCamera().getTransform().getTranslation());
  auto posMatrix = alien->getTransform().toMatrix();
  alien->setUniform("uMVP.normalMatrix", posMatrix.normalMatrix());
  alien->setUniform("uMVP.model", posMatrix);
  alien->setUniform("uMVP.view", ExampleScene::getCamera().toMatrix());
  alien->setUniform("uMVP.projection", ExampleScene::getProjectionMatrix());
  alien->getTransform().rotate(0.75f, 0.0f, 1.0f, 0.0f);

  position = MeshRenderer::getInstance("spartanTestLight")
                 ->getTransform()
                 .getTranslation();
  auto spartan = Model::getInstance("spartanTest");
  spartan->setUniform("uLight.position", position);
  spartan->setUniform(
      "uCameraPosition",
      ExampleScene::getCamera().getTransform().getTranslation());
  posMatrix = spartan->getTransform().toMatrix();
  spartan->setUniform("uMVP.normalMatrix", posMatrix.normalMatrix());
  spartan->setUniform("uMVP.model", posMatrix);
  spartan->setUniform("uMVP.view", ExampleScene::getCamera().toMatrix());
  spartan->setUniform("uMVP.projection", ExampleScene::getProjectionMatrix());
  spartan->getTransform().rotate(0.75f, 0.0f, 1.0f, 0.0f);

  auto phong = MeshRenderer::getInstance("testPhong");
  phong->getTransform().rotate(0.75f, 1.0f, 0.5f, 0.0f);
  phong->bindShaders();
  position =
      MeshRenderer::getInstance("testLight")->getTransform().getTranslation();
  phong->setUniform("uLight.position", position);
  phong->setUniform(
      "uCameraPosition",
      ExampleScene::getCamera().getTransform().getTranslation());
  posMatrix = phong->getTransform().toMatrix();
  phong->setUniform("uMVP.normalMatrix", posMatrix.normalMatrix());
  phong->setUniform("uMVP.model", posMatrix);
  phong->setUniform("uMVP.view", ExampleScene::getCamera().toMatrix());
  phong->setUniform("uMVP.projection", ExampleScene::getProjectionMatrix());
  phong->releaseShaders();

  // Rotate lighting example cubes
  mTestPhong->getTransform().rotate(0.75f, 0.5f, 0.3f, 0.2f);
  MeshRenderer::getInstance("noLight")->getTransform().rotate(
      0.75f, 0.5f, 0.3f, 0.2f);
  mTestAmbient->getTransform().rotate(0.75f, 0.5f, 0.3f, 0.2f);
  mTestDiffuse->getTransform().rotate(0.75f, 0.5f, 0.3f, 0.2f);
  mTestSpecular->getTransform().rotate(0.75f, 0.5f, 0.3f, 0.2f);

  // Examples of various translations and rotations

  // Rotate in multiple directions simultaneously
  MeshRenderer::getInstance("rgbNormalsCube")
      ->getTransform()
      .rotate(0.75f, 0.2f, 0.4f, 0.6f);

  // Pitch forward and roll sideways
  MeshRenderer::getInstance("leftTriangle")
      ->getTransform()
      .rotate(0.75f, 1.0f, 0.0f, 0.0f);
  MeshRenderer::getInstance("rightTriangle")
      ->getTransform()
      .rotate(0.75f, 0.0f, 0.0f, 1.0f);

  // Move between two positions over time
  static float translateX = 0.025f;
  float limit = -9.0f;  // Origin position.x - 2.0f
  float posX = MeshRenderer::getInstance("topTriangle")
                   ->getTransform()
                   .getTranslation()
                   .x();
  if(posX < limit || posX > limit + 4.0f) {
    translateX = -translateX;
  }
  MeshRenderer::getInstance("topTriangle")
      ->getTransform()
      .translate(translateX, 0.0f, 0.0f);
  MeshRenderer::getInstance("bottomTriangle")
      ->getTransform()
      .translate(-translateX, 0.0f, 0.0f);
  // And lets rotate the triangles in two directions at once
  MeshRenderer::getInstance("topTriangle")
      ->getTransform()
      .rotate(0.75f, 0.2f, 0.0f, 0.4f);
  MeshRenderer::getInstance("bottomTriangle")
      ->getTransform()
      .rotate(0.75f, 0.0f, 0.2f, 0.4f);
  // And make the bottom triangle green, instead of RGB

  // Rotate center cube in several directions simultaneously
  // + Not subject to gimbal lock since we are using quaternions :)
  MeshRenderer::getInstance("centerCube")
      ->getTransform()
      .rotate(0.75f, 0.2f, 0.4f, 0.6f);
}
