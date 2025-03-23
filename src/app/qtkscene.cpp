/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Example Qtk scene                                                   ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <QCoreApplication>
#include <QDir>
#include <QProcess>

#include "qtkscene.h"

using namespace Qtk;

/*******************************************************************************
 * Constructors, Destructors
 ******************************************************************************/

QtkScene::QtkScene()
{
  setSceneName("Qtk Scene");
  getCamera().getTransform().setTranslation(0.0f, 0.0f, 20.0f);
  getCamera().getTransform().setRotation(-5.0f, 0.0f, 1.0f, 0.0f);
}

QtkScene::~QtkScene()
{
  delete mTestPhong;
  delete mTestSpecular;
  delete mTestDiffuse;
  delete mTestAmbient;
}

/*******************************************************************************
 * Public Member Functions
 ******************************************************************************/

void QtkScene::init()
{
  // Clone qtk-resources if it doesn't already exist.
  QDir repoDir("resources/");
  if (!repoDir.exists()) {
    qDebug() << "Cloning qtk-resources repository to " << repoDir.absolutePath()
             << "...";

    // Run git clone
    QProcess gitProcess;
    gitProcess.setWorkingDirectory(QCoreApplication::applicationDirPath());
    gitProcess.start("git",
                     {"clone",
                      "https://git.shaunreed.com/shaunrd0/qtk-resources.git",
                      repoDir.absolutePath()});

    // Wait indefinitely for the process to complete
    gitProcess.waitForFinished(-1);
    // Check the result
    if (gitProcess.exitCode() == 0) {
      qDebug() << "Repository cloned successfully.";
    } else {
      qDebug() << "Git clone failed:" << gitProcess.readAllStandardError();
    }
  }

  // Add a skybox to the scene using default cube map images and settings.
  setSkybox(new Qtk::Skybox(":/textures/skybox/right.png",
                            ":/textures/skybox/top.png",
                            ":/textures/skybox/front.png",
                            ":/textures/skybox/left.png",
                            ":/textures/skybox/bottom.png",
                            ":/textures/skybox/back.png",
                            "Skybox"));

  /* Create a red cube with a mini master chief on top. */
  auto myCube = new MeshRenderer("My cube", Cube(Qtk::QTK_DRAW_ELEMENTS));
  myCube->setColor(RED);
  myCube->getTransform().setTranslation(5.0f, 0.0f, 0.0f);
  addObject(myCube);

  auto mySpartan = new Model("My spartan", QTK_SPARTAN);
  mySpartan->getTransform().setTranslation(5.0f, 0.5f, 0.0f);
  mySpartan->getTransform().setScale(0.5f);
  addObject(mySpartan);

  //
  // Create simple shapes using MeshRenderer class and data in mesh.h

  auto mesh = addObject(
      new Qtk::MeshRenderer("rightTriangle", Triangle(QTK_DRAW_ELEMENTS)));
  mesh->getTransform().setTranslation(-5.0f, 0.0f, -2.0f);

  mesh =
      addObject(new Qtk::MeshRenderer("centerCube", Cube(QTK_DRAW_ELEMENTS)));
  mesh->getTransform().setTranslation(-7.0f, 0.0f, -2.0f);

  mesh = addObject(
      new Qtk::MeshRenderer("leftTriangle", Triangle(QTK_DRAW_ELEMENTS)));
  mesh->getTransform().setTranslation(-9.0f, 0.0f, -2.0f);
  mesh->setDrawType(GL_LINE_LOOP);

  mesh = addObject(
      new Qtk::MeshRenderer("topTriangle", Triangle(QTK_DRAW_ELEMENTS)));
  mesh->getTransform().setTranslation(-7.0f, 2.0f, -2.0f);
  mesh->getTransform().scale(0.25f);

  mesh = addObject(
      new Qtk::MeshRenderer("bottomTriangle", Triangle(QTK_DRAW_ELEMENTS)));
  mesh->getTransform().setTranslation(-7.0f, -2.0f, -2.0f);
  mesh->getTransform().scale(0.25f);
  mesh->setDrawType(GL_LINE_LOOP);
  mesh->setColor(GREEN);


  //
  // 3D Model loading

  auto model = addObject(new Qtk::Model("backpack", QTK_BACKPACK));
  // Sometimes model textures need flipped in certain directions
  model->flipTexture("diffuse.jpg", false, true);
  model->getTransform().setTranslation(0.0f, 0.0f, -10.0f);

  model = addObject(new Qtk::Model("bird", QTK_BIRD));
  model->getTransform().setTranslation(2.0f, 2.0f, -10.0f);
  // Sometimes the models are very large
  model->getTransform().scale(0.0025f);
  model->getTransform().rotate(-110.0f, 0.0f, 1.0f, 0.0f);

  model = addObject(new Qtk::Model("alien", QTK_ALIEN));
  model->getTransform().setTranslation(2.0f, -1.0f, -5.0f);
  model->getTransform().scale(0.15f);

  model = addObject(new Qtk::Model("My scythe", QTK_SCYTHE));
  model->getTransform().setTranslation(-6.0f, 0.0f, -10.0f);
  model->getTransform().rotate(-90.0f, 1.0f, 0.0f, 0.0f);
  model->getTransform().rotate(90.0f, 0.0f, 1.0f, 0.0f);

  model = addObject(new Qtk::Model("masterChief", QTK_SPARTAN));
  model->getTransform().setTranslation(-1.5f, 0.5f, -2.0f);


  //
  // Simple cube lighting examples.

  /* Phong lighting example on a basic cube. */
  mTestPhong = addObject(new Qtk::MeshRenderer("phongCube", Qtk::Cube()));
  mTestPhong->getTransform().setTranslation(3.0f, 0.0f, -2.0f);
  // NOTE: You no longer need to manually bind shader program to set uniforms.
  // + You can still bind it if you want to for performance reasons.
  // + Qtk will only bind / release if the shader program is not already bound.
  mTestPhong->setShaders(":/shaders/solid-phong.vert",
                         ":/shaders/solid-phong.frag");

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
  mesh = addObject(
      new Qtk::MeshRenderer("phongLight", Triangle(QTK_DRAW_ELEMENTS)));
  mesh->getTransform().setTranslation(3.0f, 2.0f, -2.0f);
  mesh->getTransform().scale(0.25f);

  /* Example of a cube with no lighting applied */
  mesh = addObject(new Qtk::MeshRenderer("noLight", Cube(QTK_DRAW_ELEMENTS)));
  mesh->getTransform().setTranslation(5.0f, 0.0f, -2.0f);
  mesh->setShaders(":/shaders/solid-perspective.vert",
                   ":/shaders/solid-perspective.frag");
  mesh->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  // No light source needed for this lighting technique

  /* Initialize Ambient example cube */
  mTestAmbient = addObject(new Qtk::MeshRenderer("ambientCube", Cube()));
  mTestAmbient->getTransform().setTranslation(7.0f, 0.0f, -2.0f);
  mTestAmbient->setShaders(":/shaders/solid-ambient.vert",
                           ":/shaders/solid-ambient.frag");
  // Changing these uniform values will alter lighting effects.
  mTestAmbient->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  mTestAmbient->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
  mTestAmbient->setUniform("uAmbientStrength", 0.2f);
  mTestAmbient->reallocateNormals(mTestAmbient->getNormals());
  // No light source needed for this lighting technique

  /* Initialize Diffuse example cube */
  mTestDiffuse = addObject(new Qtk::MeshRenderer("diffuseCube", Cube()));
  mTestDiffuse->getTransform().setTranslation(9.0f, 0.0f, -2.0f);
  mTestDiffuse->setShaders(":/shaders/solid-diffuse.vert",
                           ":/shaders/solid-diffuse.frag");
  mTestDiffuse->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  mTestDiffuse->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
  mTestDiffuse->setUniform("uAmbientStrength", 0.2f);
  mTestDiffuse->reallocateNormals(mTestDiffuse->getNormals());

  // Diffuse lighting example light source. This is just for visual reference.
  mesh = addObject(
      new Qtk::MeshRenderer("diffuseLight", Triangle(QTK_DRAW_ELEMENTS)));
  mesh->getTransform().setTranslation(9.0f, 2.0f, -2.0f);
  mesh->getTransform().scale(0.25f);

  /* Initialize Specular example cube */
  mTestSpecular = addObject(new Qtk::MeshRenderer("specularCube", Cube()));
  mTestSpecular->getTransform().setTranslation(11.0f, 0.0f, -2.0f);
  mTestSpecular->setShaders(":/shaders/solid-specular.vert",
                            ":/shaders/solid-specular.frag");
  mTestSpecular->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  mTestSpecular->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
  mTestSpecular->setUniform("uAmbientStrength", 0.2f);
  mTestSpecular->setUniform("uSpecularStrength", 0.50f);
  mTestSpecular->setUniform("uSpecularShine", 256);
  mTestSpecular->reallocateNormals(mTestSpecular->getNormals());

  // Specular lighting example light source. This is just for visual reference.
  mesh = addObject(
      new Qtk::MeshRenderer("specularLight", Triangle(QTK_DRAW_ELEMENTS)));
  mesh->getTransform().setTranslation(11.0f, 2.0f, -2.0f);
  mesh->getTransform().scale(0.25f);


  /* Test basic cube with phong.vert and phong.frag shaders */
  mesh = addObject(new Qtk::MeshRenderer("testPhong", Cube(QTK_DRAW_ARRAYS)));
  mesh->getTransform().setTranslation(5.0f, 0.0f, 10.0f);
  mesh->setShaders(":/shaders/phong.vert", ":/shaders/phong.frag");
  // WARNING: Set color before reallocating normals.
  mesh->setColor(QVector3D(0.0f, 0.25f, 0.0f));
  mesh->reallocateNormals(mesh->getNormals());

  mesh->setUniform("uMaterial.ambient", QVector3D(0.0f, 0.3f, 0.0f));
  mesh->setUniform("uMaterial.diffuse", QVector3D(0.0f, 0.2f, 0.0f));
  mesh->setUniform("uMaterial.specular", QVector3D(1.0f, 1.0f, 1.0f));
  mesh->setUniform("uMaterial.ambientStrength", 1.0f);
  mesh->setUniform("uMaterial.diffuseStrength", 1.0f);
  mesh->setUniform("uMaterial.specularStrength", 1.0f);
  mesh->setUniform("uMaterial.shine", 64.0f);
  mesh->setUniform("uLight.ambient", QVector3D(0.25f, 0.2f, 0.075f));
  mesh->setUniform("uLight.diffuse", QVector3D(0.75f, 0.6f, 0.22f));
  mesh->setUniform("uLight.specular", QVector3D(0.62f, 0.55f, 0.37f));
  mesh->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));

  // Light source for testPhong cube
  mesh = addObject(
      new Qtk::MeshRenderer("testLight", Triangle(QTK_DRAW_ELEMENTS)));
  mesh->getTransform().setTranslation(5.0f, 1.25f, 10.0f);
  mesh->getTransform().scale(0.25f);
  mesh->setDrawType(GL_LINE_LOOP);
  mesh->setColor(RED);


  //
  // Building more complex objects for showing examples of lighting techniques

  /* Test alien Model with phong lighting and specular mapping. */
  model = addObject(new Qtk::Model("alienTest",
                                   QTK_ALIEN,
                                   ":/shaders/model-phong.vert",
                                   ":/shaders/model-phong.frag"));
  model->getTransform().setTranslation(3.0f, -1.0f, 10.0f);
  model->getTransform().scale(0.15f);
  model->setUniform("uMaterial.ambient", QVector3D(1.0f, 1.0f, 1.0f));
  model->setUniform("uMaterial.diffuse", QVector3D(1.0f, 1.0f, 1.0f));
  model->setUniform("uMaterial.specular", QVector3D(1.0f, 1.0f, 1.0f));
  model->setUniform("uMaterial.ambientStrength", 0.8f);
  model->setUniform("uMaterial.diffuseStrength", 0.8f);
  model->setUniform("uMaterial.specularStrength", 0.5f);
  model->setUniform("uMaterial.shine", 32.0f);

  model->setUniform("uLight.ambient", QVector3D(1.0f, 1.0f, 1.0f));
  model->setUniform("uLight.diffuse", QVector3D(1.0f, 1.0f, 1.0f));
  model->setUniform("uLight.specular", QVector3D(1.0f, 1.0f, 1.0f));

  // Light source for alienTest object.
  mesh = addObject(new Qtk::MeshRenderer("alienTestLight",
                                         Triangle(Qtk::QTK_DRAW_ELEMENTS)));
  mesh->getTransform().setTranslation(4.0f, 1.5f, 10.0f);
  mesh->getTransform().scale(0.25f);
  // This function changes values we have allocated in a buffer, so init() after
  mesh->setColor(GREEN);

  /* Test spartan Model with phong lighting, specular and normal mapping. */
  model = addObject(new Qtk::Model("spartanTest",
                                   QTK_SPARTAN,
                                   ":/shaders/model-phong.vert",
                                   ":/shaders/model-phong.frag"));
  model->getTransform().setTranslation(0.0f, -1.0f, 10.0f);
  model->getTransform().scale(2.0f);
  model->setUniform("uMaterial.ambient", QVector3D(1.0f, 1.0f, 1.0f));
  model->setUniform("uMaterial.diffuse", QVector3D(1.0f, 1.0f, 1.0f));
  model->setUniform("uMaterial.specular", QVector3D(1.0f, 1.0f, 1.0f));
  model->setUniform("uMaterial.ambientStrength", 1.0f);
  model->setUniform("uMaterial.diffuseStrength", 1.0f);
  model->setUniform("uMaterial.specularStrength", 1.0f);
  model->setUniform("uMaterial.shine", 128.0f);
  model->setUniform("uLight.ambient", QVector3D(1.0f, 1.0f, 1.0f));
  model->setUniform("uLight.diffuse", QVector3D(1.0f, 1.0f, 1.0f));
  model->setUniform("uLight.specular", QVector3D(1.0f, 1.0f, 1.0f));

  // Light source for spartanTest object.
  mesh = addObject(
      new Qtk::MeshRenderer("spartanTestLight", Triangle(QTK_DRAW_ELEMENTS)));
  mesh->getTransform().setTranslation(1.0f, 1.5f, 10.0f);
  mesh->getTransform().scale(0.25f);
  // This function changes values we have allocated in a buffer, so init() after
  mesh->setColor(GREEN);


  //
  // Test drawing simple geometry with various OpenGL drawing modes

  // RGB Normals cube to show normals are correct with QTK_DRAW_ARRAYS
  mesh = addObject(
      new Qtk::MeshRenderer("rgbNormalsCubeArraysTest", Cube(QTK_DRAW_ARRAYS)));
  mesh->getTransform().setTranslation(5.0f, 0.0f, 4.0f);
  mesh->setShaders(":/shaders/rgb-normals.vert", ":/shaders/rgb-normals.frag");
  mesh->reallocateNormals(mesh->getNormals());

  // RGB Normals cube to show normals are correct with QTK_DRAW_ELEMENTS_NORMALS
  mesh = addObject(new Qtk::MeshRenderer("rgbNormalsCubeElementsTest",
                                         Cube(QTK_DRAW_ELEMENTS_NORMALS)));
  mesh->getTransform().setTranslation(5.0f, 0.0f, 2.0f);
  mesh->setShaders(":/shaders/rgb-normals.vert", ":/shaders/rgb-normals.frag");
  mesh->reallocateNormals(mesh->getNormals());

  Texture crateTexture;
  crateTexture.setTexture(":/textures/crate.png");
  Cube cube;
  auto * m = new MeshRenderer("Test Crate", Cube(QTK_DRAW_ARRAYS));
  m->getTransform().setTranslation(0, 0, 13);
  m->setShaders(":/shaders/texture2d.vert", ":/shaders/texture2d.frag");
  m->setTexture(crateTexture);
  m->setUniform("uTexture", 0);
  m->reallocateTexCoords(cube.getTexCoords());
  addObject(m);

  // Texturing a cube using texture coordinates and glDrawArrays
  // + Texturing with UVs using glDrawElements requires
  // QTK_DRAW_ELEMENTS_NORMALS
  // + UVs required duplicating element position data from QTK_DRAW_ELEMENTS
  // + This is because the same position must use different UV coordinates
  mesh = addObject(
      new Qtk::MeshRenderer("uvCubeArraysTest", Cube(QTK_DRAW_ARRAYS)));
  mesh->getTransform().setTranslation(-3.0f, 0.0f, -2.0f);
  mesh->setShaders(":/shaders/texture2d.vert", ":/shaders/texture2d.frag");
  mesh->setTexture(crateTexture);
  mesh->setUniform("uTexture", 0);
  mesh->reallocateTexCoords(mesh->getTexCoords());

  // Test drawing a cube with texture coordinates using glDrawElements
  mesh = addObject(new Qtk::MeshRenderer("uvCubeElementsTest",
                                         Cube(QTK_DRAW_ELEMENTS_NORMALS)));
  mesh->getTransform().setTranslation(-1.7f, 0.0f, -2.0f);
  mesh->setTexture(":/textures/crate.png");
  mesh->setShaders(":/shaders/texture2d.vert", ":/shaders/texture2d.frag");
  mesh->bindShaders();
  mesh->setUniform("uTexture", 0);
  mesh->reallocateNormals(mesh->getNormals());
  mesh->reallocateTexCoords(mesh->getTexCoords(), 3);
  mesh->releaseShaders();
  mesh->getTransform().rotate(45.0f, 0.0f, 1.0f, 0.0f);

  // Texturing a cube using a cube map
  // + Cube map texturing works with both QTK_DRAW_ARRAYS and QTK_DRAW_ELEMENTS
  mesh =
      addObject(new Qtk::MeshRenderer("testCubeMap", Cube(QTK_DRAW_ELEMENTS)));
  mesh->getTransform().setTranslation(-3.0f, 1.0f, -2.0f);
  mesh->getTransform().setRotation(45.0f, 0.0f, 1.0f, 0.0f);
  mesh->setShaders(":/shaders/texture-cubemap.vert",
                   ":/shaders/texture-cubemap.frag");
  mesh->setCubeMap(":/textures/crate.png");
  mesh->setUniform("uTexture", 0);
  mesh->reallocateTexCoords(mesh->getTexCoords());

  // Create a cube with custom shaders
  // + Apply RGB normals shader and spin the cube for a neat effect
  mesh =
      addObject(new Qtk::MeshRenderer("rgbNormalsCube", Cube(QTK_DRAW_ARRAYS)));
  mesh->getTransform().setTranslation(5.0f, 2.0f, -2.0f);
  mesh->setShaders(":/shaders/rgb-normals.vert", ":/shaders/rgb-normals.frag");
  mesh->reallocateNormals(mesh->getNormals());

  // RGB Normals triangle to show normals are correct with QTK_DRAW_ARRAYS
  mesh = addObject(new Qtk::MeshRenderer("rgbTriangleArraysTest",
                                         Triangle(QTK_DRAW_ARRAYS)));
  mesh->getTransform().setTranslation(7.0f, 0.0f, 2.0f);
  mesh->setShaders(":/shaders/rgb-normals.vert", ":/shaders/rgb-normals.frag");
  mesh->reallocateNormals(mesh->getNormals());

  // RGB Normals triangle to show normals are correct with QTK_DRAW_ELEMENTS
  mesh = addObject(new Qtk::MeshRenderer("rgbTriangleElementsTest",
                                         Triangle(QTK_DRAW_ELEMENTS_NORMALS)));
  mesh->getTransform().setTranslation(7.0f, 0.0f, 4.0f);
  mesh->setShaders(":/shaders/rgb-normals.vert", ":/shaders/rgb-normals.frag");
  mesh->reallocateNormals(mesh->getNormals());

  // Test drawing triangle with glDrawArrays with texture coordinates
  mesh = addObject(
      new Qtk::MeshRenderer("testTriangleArraysUV", Triangle(QTK_DRAW_ARRAYS)));
  mesh->getTransform().setTranslation(-3.0f, 2.0f, -2.0f);
  mesh->setShaders(":/shaders/texture2d.vert", ":/shaders/texture2d.frag");

  mesh->setTexture(":/textures/crate.png");
  mesh->setUniform("uTexture", 0);
  mesh->reallocateTexCoords(mesh->getTexCoords());

  // Test drawing triangle with glDrawElements with texture coordinates
  mesh = addObject(new Qtk::MeshRenderer("testTriangleElementsUV",
                                         Triangle(QTK_DRAW_ELEMENTS_NORMALS)));
  mesh->getTransform().setTranslation(-2.5f, 0.0f, -1.0f);
  mesh->setShaders(":/shaders/texture2d.vert", ":/shaders/texture2d.frag");
  mesh->setTexture(":/textures/crate.png");
  mesh->setUniform("uTexture", 0);
  mesh->reallocateTexCoords(mesh->getTexCoords());
}

void QtkScene::draw()
{
  // WARNING: We must call the base class draw() function first.
  // + This will handle rendering core scene components like the Skybox.
  Scene::draw();
  const QVector3D cameraPosition = getCamera().getTransform().getTranslation();

  mTestPhong->bindShaders();
  mTestPhong->setUniform("uModelInverseTransposed",
                         mTestPhong->getTransform().toMatrix().normalMatrix());
  mTestPhong->setUniform(
      "uLightPosition",
      MeshRenderer::getInstance("phongLight")->getTransform().getTranslation());
  mTestPhong->setUniform("uCameraPosition", cameraPosition);
  mTestPhong->releaseShaders();
  mTestPhong->draw();

  mTestAmbient->bindShaders();
  mTestAmbient->setUniform("uCameraPosition", cameraPosition);
  mTestAmbient->releaseShaders();
  mTestAmbient->draw();

  mTestDiffuse->bindShaders();
  mTestDiffuse->setUniform(
      "uModelInverseTransposed",
      mTestDiffuse->getTransform().toMatrix().normalMatrix());
  mTestDiffuse->setUniform("uLightPosition",
                           MeshRenderer::getInstance("diffuseLight")
                               ->getTransform()
                               .getTranslation());
  mTestDiffuse->setUniform("uCameraPosition", cameraPosition);
  mTestDiffuse->releaseShaders();
  mTestDiffuse->draw();

  mTestSpecular->bindShaders();
  mTestSpecular->setUniform(
      "uModelInverseTransposed",
      mTestSpecular->getTransform().toMatrix().normalMatrix());
  mTestSpecular->setUniform("uLightPosition",
                            MeshRenderer::getInstance("specularLight")
                                ->getTransform()
                                .getTranslation());
  mTestSpecular->setUniform("uCameraPosition", cameraPosition);
  mTestSpecular->releaseShaders();
  mTestSpecular->draw();
}

void QtkScene::update()
{
  auto getModel = Model::getInstance;
  const QVector3D cameraPosition = getCamera().getTransform().getTranslation();

  // Models may have failed to load, so we should check before accessing.
  if (auto mySpartan = getModel("My spartan"); mySpartan) {
    mySpartan->getTransform().rotate(0.75f, 0.0f, 1.0f, 0.0f);
  }

  if (auto myCube = getModel("My cube"); myCube) {
    myCube->getTransform().rotate(-0.75f, 0.0f, 1.0f, 0.0f);
  }

  QMatrix4x4 posMatrix;
  if (auto alien = getModel("alienTest"); alien) {
    alien->setLightPosition("alienTestLight");

    alien->setUniform("uCameraPosition", cameraPosition);
    posMatrix = alien->getTransform().toMatrix();
    alien->setUniform("uMVP.normalMatrix", posMatrix.normalMatrix());
    alien->setUniform("uMVP.model", posMatrix);
    alien->setUniform("uMVP.view", QtkScene::getCamera().toMatrix());
    alien->setUniform("uMVP.projection", QtkScene::getProjectionMatrix());
    alien->getTransform().rotate(0.75f, 0.0f, 1.0f, 0.0f);
  }

  if (auto spartan = getModel("spartanTest"); spartan) {
    spartan->setLightPosition("spartanTestLight");

    spartan->setUniform("uCameraPosition", cameraPosition);
    posMatrix = spartan->getTransform().toMatrix();
    spartan->setUniform("uMVP.normalMatrix", posMatrix.normalMatrix());
    spartan->setUniform("uMVP.model", posMatrix);
    spartan->setUniform("uMVP.view", QtkScene::getCamera().toMatrix());
    spartan->setUniform("uMVP.projection", QtkScene::getProjectionMatrix());
    spartan->getTransform().rotate(0.75f, 0.0f, 1.0f, 0.0f);
  }

  if (auto phong = getModel("testPhong"); phong) {
    phong->setLightPosition("testLight");

    phong->getTransform().rotate(0.75f, 1.0f, 0.5f, 0.0f);
    phong->bindShaders();
    phong->setUniform("uCameraPosition", cameraPosition);
    posMatrix = phong->getTransform().toMatrix();
    phong->setUniform("uMVP.normalMatrix", posMatrix.normalMatrix());
    phong->setUniform("uMVP.model", posMatrix);
    phong->setUniform("uMVP.view", QtkScene::getCamera().toMatrix());
    phong->setUniform("uMVP.projection", QtkScene::getProjectionMatrix());
    phong->releaseShaders();
  }

  // MeshRenderers are lower level opengl objects baked into the source code.
  auto getMesh = MeshRenderer::getInstance;

  // Rotate lighting example cubes
  mTestPhong->getTransform().rotate(0.75f, 0.5f, 0.3f, 0.2f);
  getMesh("noLight")->getTransform().rotate(0.75f, 0.5f, 0.3f, 0.2f);
  mTestAmbient->getTransform().rotate(0.75f, 0.5f, 0.3f, 0.2f);
  mTestDiffuse->getTransform().rotate(0.75f, 0.5f, 0.3f, 0.2f);
  mTestSpecular->getTransform().rotate(0.75f, 0.5f, 0.3f, 0.2f);

  // Examples of various translations and rotations

  // Rotate in multiple directions simultaneously
  getMesh("rgbNormalsCube")->getTransform().rotate(0.75f, 0.2f, 0.4f, 0.6f);

  // Pitch forward and roll sideways
  getMesh("leftTriangle")->getTransform().rotate(0.75f, 1.0f, 0.0f, 0.0f);
  getMesh("rightTriangle")->getTransform().rotate(0.75f, 0.0f, 0.0f, 1.0f);

  // Move between two positions over time
  static float translateX = 0.025f;
  float limit = -9.0f;  // Origin position.x - 2.0f
  float posX = getMesh("topTriangle")->getTransform().getTranslation().x();
  if (posX < limit || posX > limit + 4.0f) {
    translateX = -translateX;
  }
  getMesh("topTriangle")->getTransform().translate(translateX, 0.0f, 0.0f);
  getMesh("bottomTriangle")->getTransform().translate(-translateX, 0.0f, 0.0f);
  // And lets rotate the triangles in two directions at once
  getMesh("topTriangle")->getTransform().rotate(0.75f, 0.2f, 0.0f, 0.4f);
  getMesh("bottomTriangle")->getTransform().rotate(0.75f, 0.0f, 0.2f, 0.4f);
  // And make the bottom triangle green, instead of RGB

  // Rotate center cube in several directions simultaneously
  // + Not subject to gimbal lock since we are using quaternions :)
  getMesh("centerCube")->getTransform().rotate(0.75f, 0.2f, 0.4f, 0.6f);
}
