/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Classes for managing objects and data within a scene                ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <abstractscene.h>
#include <camera3d.h>
#include <meshrenderer.h>
#include <model.h>
#include <resourcemanager.h>
#include <scene.h>
#include <texture.h>

using namespace Qtk;

/*******************************************************************************
 * Constructors, Destructors
 ******************************************************************************/

ExampleScene::ExampleScene()
{
  Camera().transform().setTranslation(0.0f, 0.0f, 20.0f);
  Camera().transform().setRotation(-5.0f, 0.0f, 1.0f, 0.0f);
}

ExampleScene::~ExampleScene()
{
  delete mTestPhong;
  delete mTestSpecular;
  delete mTestDiffuse;
  delete mTestAmbient;
  for (auto & mesh : mMeshes) delete mesh;
  for (auto & model : mModels) delete model;
  delete mSkybox;
}


/*******************************************************************************
 * Public Member Functions
 ******************************************************************************/

void ExampleScene::init()
{
  Qtk::Skybox * sb = new Qtk::Skybox("Skybox");
  setSkybox(sb);

  // Initialize Phong example cube
  mTestPhong = new Qtk::MeshRenderer("phong", Qtk::Cube());
  mTestPhong->mTransform.setTranslation(3.0f, 0.0f, -2.0f);
  mTestPhong->setShaders(":/solid-phong.vert", ":/solid-phong.frag");
  mTestPhong->init();
  mTestPhong->mProgram.bind();
  mTestPhong->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  mTestPhong->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
  mTestPhong->setUniform("uAmbientStrength", 0.2f);
  mTestPhong->setUniform("uSpecularStrength", 0.50f);
  mTestPhong->setUniform("uSpecularShine", 256);

  mTestPhong->mVAO.bind();
  mTestPhong->mNBO.create();
  mTestPhong->mNBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
  mTestPhong->mNBO.bind();
  mTestPhong->mNBO.allocate(mTestPhong->normals().data(),
                            mTestPhong->normals().size()
                            * sizeof(mTestPhong->normals()[0]));
  mTestPhong->mProgram.enableAttributeArray(1);
  mTestPhong->mProgram.setAttributeBuffer(1, GL_FLOAT, 0,
                                          3, sizeof(QVector3D));
  mTestPhong->mNBO.release();
  mTestPhong->mVAO.release();
  mTestPhong->mProgram.release();


  // Initialize Ambient example cube
  mTestAmbient = new Qtk::MeshRenderer("ambient", Cube());
  mTestAmbient->mTransform.setTranslation(7.0f, 0.0f, -2.0f);
  mTestAmbient->setShaders(":/solid-ambient.vert", ":/solid-ambient.frag");
  mTestAmbient->init();
  mTestAmbient->mProgram.bind();
  mTestAmbient->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  mTestAmbient->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
  mTestAmbient->setUniform("uAmbientStrength", 0.2f);

  mTestAmbient->mVAO.bind();
  mTestAmbient->mNBO.create();
  mTestAmbient->mNBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
  mTestAmbient->mNBO.bind();
  mTestAmbient->mNBO.allocate(mTestAmbient->normals().data(),
                              mTestAmbient->normals().size()
                              * sizeof(mTestAmbient->normals()[0]));
  mTestAmbient->mProgram.enableAttributeArray(1);
  mTestAmbient->mProgram.setAttributeBuffer(1, GL_FLOAT, 0,
                                            3, sizeof(QVector3D));
  mTestAmbient->mNBO.release();
  mTestAmbient->mVAO.release();
  mTestAmbient->mProgram.release();

  // Initialize Diffuse example cube
  mTestDiffuse = new Qtk::MeshRenderer("diffuse", Cube());
  mTestDiffuse->mTransform.setTranslation(9.0f, 0.0f, -2.0f);
  mTestDiffuse->setShaders(":/solid-diffuse.vert", ":/solid-diffuse.frag");
  mTestDiffuse->init();
  mTestDiffuse->mProgram.bind();
  mTestDiffuse->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  mTestDiffuse->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
  mTestDiffuse->setUniform("uAmbientStrength", 0.2f);

  mTestDiffuse->mVAO.bind();
  mTestDiffuse->mNBO.create();
  mTestDiffuse->mNBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
  mTestDiffuse->mNBO.bind();
  mTestDiffuse->mNBO.allocate(mTestDiffuse->normals().data(),
                              mTestDiffuse->normals().size()
                              * sizeof(mTestDiffuse->normals()[0]));
  mTestDiffuse->mProgram.enableAttributeArray(1);
  mTestDiffuse->mProgram.setAttributeBuffer(1, GL_FLOAT, 0,
                                            3, sizeof(QVector3D));
  mTestDiffuse->mNBO.release();
  mTestDiffuse->mVAO.release();
  mTestDiffuse->mProgram.release();

  // Initialize Specular example cube
  mTestSpecular = new Qtk::MeshRenderer("specular", Cube());
  mTestSpecular->mTransform.setTranslation(11.0f, 0.0f, -2.0f);
  mTestSpecular->setShaders(":/solid-specular.vert", ":/solid-specular.frag");
  mTestSpecular->init();
  mTestSpecular->mProgram.bind();
  mTestSpecular->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  mTestSpecular->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
  mTestSpecular->setUniform("uAmbientStrength", 0.2f);
  mTestSpecular->setUniform("uSpecularStrength", 0.50f);
  mTestSpecular->setUniform("uSpecularShine", 256);

  mTestSpecular->mVAO.bind();
  mTestSpecular->mNBO.create();
  mTestSpecular->mNBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
  mTestSpecular->mNBO.bind();
  mTestSpecular->mNBO.allocate(mTestSpecular->normals().data(),
                               mTestSpecular->normals().size()
                               * sizeof(mTestSpecular->normals()[0]));
  mTestSpecular->mProgram.enableAttributeArray(1);
  mTestSpecular->mProgram.setAttributeBuffer(1, GL_FLOAT, 0,
                                             3, sizeof(QVector3D));
  mTestSpecular->mNBO.release();
  mTestSpecular->mVAO.release();
  mTestSpecular->mProgram.release();

  //
  // Model loading

  mModels.push_back(new Qtk::Model("backpack", ":/models/backpack/backpack.obj"));
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

  mModels.push_back(new Qtk::Model("alien", ":/models/alien-hominid/alien.obj"));
  mModels.back()->mTransform.setTranslation(2.0f, -1.0f, -5.0f);
  mModels.back()->mTransform.scale(0.15f);

  mModels.push_back(new Qtk::Model("scythe", ":/models/scythe/scythe.obj"));
  mModels.back()->mTransform.setTranslation(-6.0f, 0.0f, -10.0f);
  mModels.back()->mTransform.rotate(-90.0f, 1.0f, 0.0f, 0.0f);
  mModels.back()->mTransform.rotate(90.0f, 0.0f, 1.0f, 0.0f);

  mModels.push_back(new Qtk::Model("masterChief", ":/models/spartan/spartan.obj"));
  mModels.back()->mTransform.setTranslation(-1.5f, 0.5f, -2.0f);


  //
  // Building example mesh objects

  // Render an alien with specular
  // Test alien Model with phong lighting and specular mapping
  mMeshes.push_back(
      new Qtk::MeshRenderer("alienTestLight", Triangle(Qtk::QTK_DRAW_ELEMENTS)));
  mMeshes.back()->mTransform.setTranslation(4.0f, 1.5f, 10.0f);
  mMeshes.back()->mTransform.scale(0.25f);
  // This function changes values we have allocated in a buffer, so init() after
  mMeshes.back()->setColor(GREEN);
  mMeshes.back()->init();

  mModels.push_back(
      new Qtk::Model("alienTest", ":/models/alien-hominid/alien.obj",
                ":/model-specular.vert", ":/model-specular.frag")
  );
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
      new Qtk::MeshRenderer("spartanTestLight", Triangle(QTK_DRAW_ELEMENTS))
  );
  mMeshes.back()->mTransform.setTranslation(1.0f, 1.5f, 10.0f);
  mMeshes.back()->mTransform.scale(0.25f);
  // This function changes values we have allocated in a buffer, so init() after
  mMeshes.back()->setColor(GREEN);
  mMeshes.back()->init();

  mModels.push_back(
      new Qtk::Model("spartanTest", ":/models/spartan/spartan.obj",
                ":/model-normals.vert", ":/model-normals.frag")
  );
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
  mMeshes.back()->init();

  mMeshes.push_back(
      new Qtk::MeshRenderer("testPhong", Cube(QTK_DRAW_ARRAYS)));
  mMeshes.back()->mTransform.setTranslation(5.0f, 0.0f, 10.0f);
  mMeshes.back()->setShaders(":/phong.vert", ":/phong.frag");
  mMeshes.back()->setColor(QVector3D(0.0f, 0.25f, 0.0f));
  mMeshes.back()->init();
  mMeshes.back()->mProgram.bind();

  mMeshes.back()->mVAO.bind();
  mMeshes.back()->mNBO.create();
  mMeshes.back()->mNBO.bind();

  mMeshes.back()->mNBO.allocate(mMeshes.back()->normals().data(),
                                mMeshes.back()->normals().size()
                                * sizeof(mMeshes.back()->normals()[0]));
  mMeshes.back()->mProgram.enableAttributeArray(1);
  mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0,
                                              3, sizeof(QVector3D));

  mMeshes.back()->mNBO.release();
  mMeshes.back()->mVAO.release();

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

  mMeshes.back()->mProgram.release();

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
  mMeshes.back()->init();

  //
  // Testing for normals, texture coordinates

  // RGB Normals cube to show normals are correct with QTK_DRAW_ARRAYS
  mMeshes.push_back(
      new Qtk::MeshRenderer("rgbNormalsCubeArraysTest", Cube(QTK_DRAW_ARRAYS)));
  mMeshes.back()->mTransform.setTranslation(5.0f, 0.0f, 4.0f);
  mMeshes.back()->setShaders(":/rgb-normals.vert", ":/rgb-normals.frag");
  mMeshes.back()->init();
  mMeshes.back()->mVAO.bind();
  mMeshes.back()->mNBO.create();
  mMeshes.back()->mNBO.bind();
  mMeshes.back()->mProgram.bind();

  mMeshes.back()->mNBO.allocate(mMeshes.back()->normals().data(),
                                mMeshes.back()->normals().size()
                                * sizeof(mMeshes.back()->normals()[0]));
  mMeshes.back()->mProgram.enableAttributeArray(1);
  mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0,
                                              3, sizeof(QVector3D));

  mMeshes.back()->mProgram.release();
  mMeshes.back()->mNBO.release();
  mMeshes.back()->mVAO.release();

  // RGB Normals cube to show normals are correct with QTK_DRAW_ELEMENTS_NORMALS
  mMeshes.push_back(
      new Qtk::MeshRenderer("rgbNormalsCubeElementsTest", Cube(QTK_DRAW_ELEMENTS_NORMALS)));
  mMeshes.back()->mTransform.setTranslation(5.0f, 0.0f, 2.0f);
  mMeshes.back()->setShaders(":/rgb-normals.vert", ":/rgb-normals.frag");
  mMeshes.back()->init();
  mMeshes.back()->mVAO.bind();
  mMeshes.back()->mNBO.create();
  mMeshes.back()->mNBO.bind();
  mMeshes.back()->mProgram.bind();

  mMeshes.back()->mNBO.allocate(mMeshes.back()->normals().data(),
                                mMeshes.back()->normals().size()
                                * sizeof(mMeshes.back()->normals()[0]));
  mMeshes.back()->mProgram.enableAttributeArray(1);
  mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0,
                                              3, sizeof(QVector3D));

  mMeshes.back()->mProgram.release();
  mMeshes.back()->mNBO.release();
  mMeshes.back()->mVAO.release();

  // Texturing a cube using texture coordinates and glDrawArrays
  // + Texturing with UVs using glDrawElements requires QTK_DRAW_ELEMENTS_NORMALS
  // + UVs required duplicating element position data from QTK_DRAW_ELEMENTS
  // + This is because the same position must use different UV coordinates
  mMeshes.push_back(
      new Qtk::MeshRenderer("uvCubeArraysTest", Cube(QTK_DRAW_ARRAYS)));
  mMeshes.back()->mTransform.setTranslation(-3.0f, 0.0f, -2.0f);
  mMeshes.back()->setShaders(":/texture2d.vert", ":/texture2d.frag");
  mMeshes.back()->init();
  mMeshes.back()->mProgram.bind();

  mMeshes.back()->setTexture(Texture::initTexture2D(":/crate.png"));
  mMeshes.back()->setUniform("uTexture", 0);
  mMeshes.back()->texture().bind();

  mMeshes.back()->texture().release();

  mMeshes.back()->mVAO.bind();
  mMeshes.back()->mNBO.destroy();
  mMeshes.back()->mNBO.create();
  mMeshes.back()->mNBO.bind();
  mMeshes.back()->mNBO.allocate(mMeshes.back()->mShape.texCoords().data(),
                                mMeshes.back()->mShape.texCoords().size()
                                * sizeof(mMeshes.back()->mShape.texCoords()[0]));
  mMeshes.back()->mProgram.enableAttributeArray(1);
  mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0,
                                              2, sizeof(QVector2D));
  mMeshes.back()->mNBO.release();
  mMeshes.back()->mVAO.release();
  mMeshes.back()->mProgram.release();

  // Test drawing a cube with texture coordinates using glDrawElements
  mMeshes.push_back(
      new Qtk::MeshRenderer("uvCubeElementsTest", Cube(QTK_DRAW_ELEMENTS_NORMALS)));
  mMeshes.back()->mTransform.setTranslation(-1.7f, 0.0f, -2.0f);
  mMeshes.back()->setShaders(":/texture2d.vert", ":/texture2d.frag");
  mMeshes.back()->init();
  mMeshes.back()->mVAO.bind();
  mMeshes.back()->mNBO.create();
  mMeshes.back()->mNBO.bind();
  mMeshes.back()->mProgram.bind();

  mMeshes.back()->mNBO.allocate(mMeshes.back()->texCoords().data(),
                                mMeshes.back()->texCoords().size()
                                * sizeof(mMeshes.back()->texCoords()[0]));
  mMeshes.back()->mProgram.enableAttributeArray(1);
  mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0,
                                              3, sizeof(QVector3D));
  mMeshes.back()->setTexture(Texture::initTexture2D(":/crate.png"));

  mMeshes.back()->mProgram.setUniformValue("uTexture", 0);
  mMeshes.back()->mProgram.release();
  mMeshes.back()->mNBO.release();
  mMeshes.back()->mVAO.release();
  mMeshes.back()->mTransform.rotate(45.0f, 0.0f, 1.0f, 0.0f);

  // Texturing a cube using a cube map
  // + Cube map texturing works with both QTK_DRAW_ARRAYS and QTK_DRAW_ELEMENTS
  mMeshes.push_back(
      new Qtk::MeshRenderer("testCubeMap", Cube(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->mTransform.setTranslation(-3.0f, 1.0f, -2.0f);
  mMeshes.back()->mTransform.setRotation(45.0f, 0.0f, 1.0f, 0.0f);
  mMeshes.back()->setShaders(":/texture-cubemap.vert", ":/texture-cubemap.frag");
  mMeshes.back()->init();
  mMeshes.back()->mProgram.bind();

  mMeshes.back()->setTexture(Texture::initCubeMap(":/crate.png"));
  mMeshes.back()->setUniform("uTexture", 0);

  mMeshes.back()->mVAO.bind();
  mMeshes.back()->mNBO.destroy();
  mMeshes.back()->mNBO.create();
  mMeshes.back()->mNBO.bind();
  mMeshes.back()->mNBO.allocate(mMeshes.back()->mShape.texCoords().data(),
                                mMeshes.back()->mShape.texCoords().size()
                                * sizeof(mMeshes.back()->mShape.texCoords()[0]));
  mMeshes.back()->mProgram.enableAttributeArray(1);
  mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0,
                                              2, sizeof(QVector2D));
  mMeshes.back()->mNBO.release();
  mMeshes.back()->mVAO.release();
  mMeshes.back()->mProgram.release();

  // Create a cube with custom shaders
  // + Apply RGB normals shader and spin the cube for a neat effect
  mMeshes.push_back(
      new Qtk::MeshRenderer("rgbNormalsCube", Cube(QTK_DRAW_ARRAYS)));
  mMeshes.back()->mTransform.setTranslation(5.0f, 2.0f, -2.0f);
  mMeshes.back()->setShaders(":/rgb-normals.vert", ":/rgb-normals.frag");
  mMeshes.back()->init();
  mMeshes.back()->mVAO.bind();
  mMeshes.back()->mNBO.create();
  mMeshes.back()->mNBO.bind();
  mMeshes.back()->mProgram.bind();

  mMeshes.back()->mNBO.allocate(mMeshes.back()->normals().data(),
                                mMeshes.back()->normals().size()
                                * sizeof(mMeshes.back()->normals()[0]));
  mMeshes.back()->mProgram.enableAttributeArray(1);
  mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0,
                                              3, sizeof(QVector3D));

  mMeshes.back()->mProgram.release();
  mMeshes.back()->mNBO.release();
  mMeshes.back()->mVAO.release();

  // RGB Normals triangle to show normals are correct with QTK_DRAW_ARRAYS
  mMeshes.push_back(
      new Qtk::MeshRenderer("rgbTriangleArraysTest", Triangle(QTK_DRAW_ARRAYS)));
  mMeshes.back()->mTransform.setTranslation(7.0f, 0.0f, 2.0f);
  mMeshes.back()->setShaders(":/rgb-normals.vert", ":/rgb-normals.frag");
  mMeshes.back()->init();
  mMeshes.back()->mProgram.bind();

  mMeshes.back()->mVAO.bind();
  mMeshes.back()->mNBO.create();
  mMeshes.back()->mNBO.bind();
  mMeshes.back()->mNBO.allocate(mMeshes.back()->normals().data(),
                                mMeshes.back()->normals().size()
                                * sizeof(mMeshes.back()->normals()[0]));
  mMeshes.back()->mProgram.enableAttributeArray(1);
  mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0,
                                              3, sizeof(QVector3D));
  mMeshes.back()->mNBO.release();
  mMeshes.back()->mVAO.release();
  mMeshes.back()->mProgram.release();

  // RGB Normals triangle to show normals are correct with QTK_DRAW_ELEMENTS
  mMeshes.push_back(
      new Qtk::MeshRenderer("rgbTriangleElementsTest",
                       Triangle(QTK_DRAW_ELEMENTS_NORMALS)));
  mMeshes.back()->mTransform.setTranslation(7.0f, 0.0f, 4.0f);
  mMeshes.back()->setShaders(":/rgb-normals.vert", ":/rgb-normals.frag");
  mMeshes.back()->init();
  mMeshes.back()->mProgram.bind();

  mMeshes.back()->mVAO.bind();
  mMeshes.back()->mNBO.create();
  mMeshes.back()->mNBO.bind();
  mMeshes.back()->mNBO.allocate(mMeshes.back()->normals().data(),
                                mMeshes.back()->normals().size()
                                * sizeof(mMeshes.back()->normals()[0]));
  mMeshes.back()->mProgram.enableAttributeArray(1);
  mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0,
                                              3, sizeof(QVector3D));
  mMeshes.back()->mNBO.release();
  mMeshes.back()->mVAO.release();
  mMeshes.back()->mProgram.release();

  // Test drawing triangle with glDrawArrays with texture coordinates
  mMeshes.push_back(
      new Qtk::MeshRenderer("testTriangleArraysUV", Triangle(QTK_DRAW_ARRAYS)));
  mMeshes.back()->mTransform.setTranslation(-3.0f, 2.0f, -2.0f);
  mMeshes.back()->setShaders(":/texture2d.vert", ":/texture2d.frag");
  mMeshes.back()->init();
  mMeshes.back()->mProgram.bind();

  mMeshes.back()->setTexture(Texture::initTexture2D(":/crate.png"));
  mMeshes.back()->setUniform("uTexture", 0);
  mMeshes.back()->texture().bind();

  mMeshes.back()->texture().release();

  mMeshes.back()->mVAO.bind();
  mMeshes.back()->mNBO.destroy();
  mMeshes.back()->mNBO.create();
  mMeshes.back()->mNBO.bind();
  mMeshes.back()->mNBO.allocate(mMeshes.back()->mShape.texCoords().data(),
                                mMeshes.back()->mShape.texCoords().size()
                                * sizeof(mMeshes.back()->mShape.texCoords()[0]));
  mMeshes.back()->mProgram.enableAttributeArray(1);
  mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0,
                                              2, sizeof(QVector2D));
  mMeshes.back()->mNBO.release();
  mMeshes.back()->mVAO.release();
  mMeshes.back()->mProgram.release();

  // Test drawing triangle with glDrawElements with texture coordinates
  mMeshes.push_back(
      new Qtk::MeshRenderer("testTriangleElementsUV",
                       Triangle(QTK_DRAW_ELEMENTS_NORMALS)));
  mMeshes.back()->mTransform.setTranslation(-2.5f, 0.0f, -1.0f);
  mMeshes.back()->setShaders(":/texture2d.vert", ":/texture2d.frag");
  mMeshes.back()->init();
  mMeshes.back()->mProgram.bind();

  mMeshes.back()->setTexture(Texture::initTexture2D(":/crate.png"));
  mMeshes.back()->setUniform("uTexture", 0);
  mMeshes.back()->texture().bind();

  mMeshes.back()->texture().release();

  mMeshes.back()->mVAO.bind();
  mMeshes.back()->mNBO.destroy();
  mMeshes.back()->mNBO.create();
  mMeshes.back()->mNBO.bind();
  mMeshes.back()->mNBO.allocate(mMeshes.back()->mShape.texCoords().data(),
                                mMeshes.back()->mShape.texCoords().size()
                                * sizeof(mMeshes.back()->mShape.texCoords()[0]));
  mMeshes.back()->mProgram.enableAttributeArray(1);
  mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0,
                                              2, sizeof(QVector2D));
  mMeshes.back()->mNBO.release();
  mMeshes.back()->mVAO.release();
  mMeshes.back()->mProgram.release();

  //
  // Lighting cube examples

  // Example of a cube with no lighting applied
  mMeshes.push_back(
      new Qtk::MeshRenderer("noLight", Cube(QTK_DRAW_ELEMENTS)));
  mMeshes.back()->mTransform.setTranslation(5.0f, 0.0f, -2.0f);
  mMeshes.back()->setShaders(":/solid-perspective.vert",
                             ":/solid-perspective.frag");
  mMeshes.back()->init();
  mMeshes.back()->mProgram.bind();
  mMeshes.back()->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  mMeshes.back()->mProgram.release();

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

void ExampleScene::draw()
{
  mSkybox->draw();

  for (auto & model : mModels) model->draw();

  for (const auto &mesh : mMeshes) mesh->draw();

  mTestPhong->mProgram.bind();
  mTestPhong->setUniform("uModelInverseTransposed",
                         mTestPhong->mTransform.toMatrix().normalMatrix());
  mTestPhong->setUniform(
      "uLightPosition",
      MeshRenderer::getInstance("phongLight")->mTransform.translation());
  mTestPhong->setUniform("uCameraPosition",
                         ExampleScene::Camera().transform().translation());
  mTestPhong->mProgram.release();
  mTestPhong->draw();

  mTestAmbient->mProgram.bind();
  mTestAmbient->setUniform("uCameraPosition",
                           ExampleScene::Camera().transform().translation());
  mTestAmbient->mProgram.release();
  mTestAmbient->draw();

  mTestDiffuse->mProgram.bind();
  mTestDiffuse->setUniform("uModelInverseTransposed",
                           mTestDiffuse->mTransform.toMatrix().normalMatrix());
  mTestDiffuse->setUniform(
      "uLightPosition",
      MeshRenderer::getInstance("diffuseLight")->mTransform.translation());
  mTestDiffuse->setUniform("uCameraPosition", ExampleScene::Camera().transform().translation());
  mTestDiffuse->mProgram.release();
  mTestDiffuse->draw();

  mTestSpecular->mProgram.bind();
  mTestSpecular->setUniform(
      "uModelInverseTransposed",
      mTestSpecular->mTransform.toMatrix().normalMatrix());
  mTestSpecular->setUniform(
      "uLightPosition",
      MeshRenderer::getInstance("specularLight")->mTransform.translation());
  mTestSpecular->setUniform("uCameraPosition", ExampleScene::Camera().transform().translation());
  mTestSpecular->mProgram.release();
  mTestSpecular->draw();
}

void ExampleScene::update()
{
  auto position = MeshRenderer::getInstance("alienTestLight")->mTransform.translation();
  Model::getInstance("alienTest")->setUniform(
      "uLight.position", position);
  Model::getInstance("alienTest")->setUniform(
      "uCameraPosition", ExampleScene::Camera().transform().translation());
  auto posMatrix = Model::getInstance("alienTest")->mTransform.toMatrix();
  Model::getInstance("alienTest")->setUniform(
      "uMVP.normalMatrix", posMatrix.normalMatrix());
  Model::getInstance("alienTest")->setUniform(
      "uMVP.model", posMatrix);
  Model::getInstance("alienTest")->setUniform(
      "uMVP.view", ExampleScene::Camera().toMatrix());
  Model::getInstance("alienTest")->setUniform(
      "uMVP.projection", ExampleScene::Projection());
  Model::getInstance("alienTest")->mTransform.rotate(0.75f, 0.0f, 1.0f, 0.0f);

  position = MeshRenderer::getInstance("spartanTestLight")->mTransform.translation();
  Model::getInstance("spartanTest")->setUniform(
      "uLight.position", position);
  Model::getInstance("spartanTest")->setUniform(
      "uCameraPosition", ExampleScene::Camera().transform().translation());
  posMatrix = Model::getInstance("spartanTest")->mTransform.toMatrix();
  Model::getInstance("spartanTest")->setUniform(
      "uMVP.normalMatrix", posMatrix.normalMatrix());
  Model::getInstance("spartanTest")->setUniform(
      "uMVP.model", posMatrix);
  Model::getInstance("spartanTest")->setUniform(
      "uMVP.view", ExampleScene::Camera().toMatrix());
  Model::getInstance("spartanTest")->setUniform(
      "uMVP.projection", ExampleScene::Projection());
  Model::getInstance("spartanTest")->mTransform.rotate(0.75f, 0.0f, 1.0f, 0.0f);



  MeshRenderer::getInstance("testPhong")->mTransform.rotate(
      0.75f, 1.0f, 0.5f, 0.0f);
  MeshRenderer::getInstance("testPhong")->mProgram.bind();
  position = MeshRenderer::getInstance("testLight")->mTransform.translation();
  MeshRenderer::getInstance("testPhong")->setUniform(
      "uLight.position", position);
  MeshRenderer::getInstance("testPhong")->setUniform(
      "uCameraPosition", ExampleScene::Camera().transform().translation());
  posMatrix = MeshRenderer::getInstance("testPhong")->mTransform.toMatrix();
  MeshRenderer::getInstance("testPhong")->setUniform(
      "uMVP.normalMatrix", posMatrix.normalMatrix());
  MeshRenderer::getInstance("testPhong")->setUniform(
      "uMVP.model", posMatrix);
  MeshRenderer::getInstance("testPhong")->setUniform(
      "uMVP.view", ExampleScene::Camera().toMatrix());
  MeshRenderer::getInstance("testPhong")->setUniform(
      "uMVP.projection", ExampleScene::Projection());
  MeshRenderer::getInstance("testPhong")->mProgram.release();

  // Rotate lighting example cubes
  mTestPhong->mTransform.rotate(0.75f, 0.5f, 0.3f, 0.2f);
  MeshRenderer::getInstance("noLight")->mTransform.rotate(
      0.75f, 0.5f, 0.3f, 0.2f);
  mTestAmbient->mTransform.rotate(0.75f, 0.5f, 0.3f, 0.2f);
  mTestDiffuse->mTransform.rotate(0.75f, 0.5f, 0.3f, 0.2f);
  mTestSpecular->mTransform.rotate(0.75f, 0.5f, 0.3f, 0.2f);

  // Examples of various translations and rotations

  // Rotate in multiple directions simultaneously
  MeshRenderer::getInstance("rgbNormalsCube")->mTransform.rotate(
      0.75f, 0.2f, 0.4f, 0.6f);

  // Pitch forward and roll sideways
  MeshRenderer::getInstance("leftTriangle")->mTransform.rotate(
      0.75f, 1.0f, 0.0f, 0.0f);
  MeshRenderer::getInstance("rightTriangle")->mTransform.rotate(
      0.75f, 0.0f, 0.0f, 1.0f);

  // Move between two positions over time
  static float translateX = 0.025f;
  float limit = -9.0f; // Origin position.x - 2.0f
  float posX =
      MeshRenderer::getInstance("topTriangle")->mTransform.translation().x();
  if (posX < limit || posX > limit + 4.0f) {
    translateX = -translateX;
  }
  MeshRenderer::getInstance("topTriangle")->mTransform.translate(
      translateX, 0.0f, 0.0f);
  MeshRenderer::getInstance("bottomTriangle")->mTransform.translate(
      -translateX, 0.0f, 0.0f);
  // And lets rotate the triangles in two directions at once
  MeshRenderer::getInstance("topTriangle")->mTransform.rotate(
      0.75f, 0.2f, 0.0f, 0.4f);
  MeshRenderer::getInstance("bottomTriangle")->mTransform.rotate(
      0.75f, 0.0f, 0.2f, 0.4f);
  // And make the bottom triangle green, instead of RGB

  // Rotate center cube in several directions simultaneously
  // + Not subject to gimbal lock since we are using quaternions :)
  MeshRenderer::getInstance("centerCube")->mTransform.rotate(
      0.75f, 0.2f, 0.4f, 0.6f);
}
