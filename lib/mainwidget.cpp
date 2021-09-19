/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2021 Shaun Reed, all rights reserved                ##
## About: Main window for Qt5 OpenGL widget application                       ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <QKeyEvent>

#include <input.h>
#include <mesh.h>
#include <object.h>
#include <scene.h>

#include <mainwidget.h>


/*******************************************************************************
 * Constructors, Destructors
 ******************************************************************************/

MainWidget::MainWidget() : mDebugLogger(Q_NULLPTR)
{
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setVersion(4, 5);
  format.setDepthBufferSize(16);
  // If QTK_DEBUG is set, enable debug context
#ifdef QTK_DEBUG
  format.setOption(QSurfaceFormat::DebugContext);
#endif
}

MainWidget::MainWidget(const QSurfaceFormat &format)
    : mDebugLogger(Q_NULLPTR)
{
  setFormat(format);
  resize(QSize(800, 600));
  show();
}

MainWidget::~MainWidget()
{
  makeCurrent();
  teardownGL();
}


/*******************************************************************************
 * Private Member Functions
 ******************************************************************************/

void MainWidget::teardownGL()
{
  // Nothing to teardown yet...
}

void MainWidget::initObjects()
{
  mScene = new Scene;

  // Drawing a primitive object using Qt and OpenGL
  // The Object class only stores basic QOpenGL* members and shape data
  // + Within mainwidget, mObject serves as a basic QOpenGL example
  mObject = new Object("testObject");
  mObject->setVertices(Cube(QTK_DRAW_ELEMENTS).vertices());
  mObject->setIndices(Cube(QTK_DRAW_ELEMENTS).indices());
  mObject->mProgram.create();
  mObject->mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                            ":/solid-ambient.vert");
  mObject->mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                            ":/solid-ambient.frag");
  mObject->mProgram.link();
  mObject->mProgram.bind();

  mObject->mVAO.create();
  mObject->mVAO.bind();

  mObject->mVBO.create();
  mObject->mVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
  mObject->mVBO.bind();

  mObject->mVBO.allocate(mObject->vertices().data(),
                         mObject->vertices().size()
                         * sizeof(mObject->vertices()[0]));

  mObject->mProgram.enableAttributeArray(0);
  mObject->mProgram.setAttributeBuffer(0, GL_FLOAT, 0,
                                       3, sizeof(mObject->vertices()[0]));
  mObject->mProgram.setUniformValue("uColor", QVector3D(0.0f, 0.25f, 0.0f));
  mObject->mProgram.setUniformValue("uLightColor", WHITE);
  mObject->mProgram.setUniformValue("uAmbientStrength", 0.2f);

  mObject->mVBO.release();
  mObject->mVAO.release();
  mObject->mProgram.release();

  mObject->mTransform.setTranslation(13.0f, 0.0f, -2.0f);
}


/*******************************************************************************
 * Inherited Virtual Member Functions
 ******************************************************************************/

void MainWidget::paintGL()
{
  // Clear buffers
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  // Draw the scene first, since it handles drawing our skybox
  mScene->draw();

  // Draw any additional objects within mainwidget manually
  mObject->mProgram.bind();
  mObject->mVAO.bind();
  mObject->mProgram.setUniformValue("uModel", mObject->mTransform.toMatrix());
  mObject->mProgram.setUniformValue("uView", Scene::Camera().toMatrix());
  mObject->mProgram.setUniformValue("uProjection", Scene::Projection());
  glDrawElements(GL_TRIANGLES, mObject->indices().size(),
                 GL_UNSIGNED_INT, mObject->indices().data());
  mObject->mVAO.release();
  mObject->mProgram.release();
}

void MainWidget::initializeGL()
{
  initializeOpenGLFunctions();
  // Connect the frameSwapped signal to call the update() function
  connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

  // Initialize OpenGL debug context
#ifdef QTK_DEBUG
  mDebugLogger = new QOpenGLDebugLogger(this);
  if (mDebugLogger->initialize()) {
    qDebug() << "GL_DEBUG Debug Logger" << mDebugLogger << "\n";
    connect(mDebugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)),
            this, SLOT(messageLogged(QOpenGLDebugMessage)));
    mDebugLogger->startLogging();
  }
#endif // QTK_DEBUG

  printContextInformation();

// Initialize opengl settings
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.1f, 1.0f);
  glClearDepth(1.0f);
  glClearColor(0.0f, 0.25f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Initialize default objects within the scene
  initObjects();
}

void MainWidget::resizeGL(int width, int height)
{
  Scene::Projection().setToIdentity();
  Scene::Projection().perspective(45.0f,
                          float(width) / float(height),
                          0.1f, 1000.0f);
}


/*******************************************************************************
 * Protected Slots
 ******************************************************************************/

void MainWidget::update()
{
  updateCameraInput();

  mScene->update();

  QWidget::update();
}

void MainWidget::messageLogged(const QOpenGLDebugMessage &msg)
{
  QString error;

  // Format based on severity
  switch (msg.severity())
  {
    case QOpenGLDebugMessage::NotificationSeverity:
      error += "--";
      break;
    case QOpenGLDebugMessage::HighSeverity:
      error += "!!";
      break;
    case QOpenGLDebugMessage::MediumSeverity:
      error += "!~";
      break;
    case QOpenGLDebugMessage::LowSeverity:
      error += "~~";
      break;
  }

  error += " (";

  // Format based on source
#define CASE(c) case QOpenGLDebugMessage::c: error += #c; break
  switch (msg.source())
  {
    CASE(APISource);
    CASE(WindowSystemSource);
    CASE(ShaderCompilerSource);
    CASE(ThirdPartySource);
    CASE(ApplicationSource);
    CASE(OtherSource);
    CASE(InvalidSource);
  }
#undef CASE

  error += " : ";

// Format based on type
#define CASE(c) case QOpenGLDebugMessage::c: error += #c; break
  switch (msg.type())
  {
    CASE(InvalidType);
    CASE(ErrorType);
    CASE(DeprecatedBehaviorType);
    CASE(UndefinedBehaviorType);
    CASE(PortabilityType);
    CASE(PerformanceType);
    CASE(OtherType);
    CASE(MarkerType);
    CASE(GroupPushType);
    CASE(GroupPopType);
  }
#undef CASE

  error += ")";
  qDebug() << qPrintable(error) << "\n" << qPrintable(msg.message()) << "\n";
}


/*******************************************************************************
 * Protected Helpers
 ******************************************************************************/

void MainWidget::keyPressEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat()) {
    event->ignore();
  } else {
    Input::registerKeyPress(event->key());
  }
}

void MainWidget::keyReleaseEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat()) {
    event->ignore();
  } else {
    Input::registerKeyRelease(event->key());
  }
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
  Input::registerMousePress(event->button());
}

void MainWidget::mouseReleaseEvent(QMouseEvent *event)
{
  Input::registerMouseRelease(event->button());
}


/*******************************************************************************
 * Private Helpers
 ******************************************************************************/

void MainWidget::printContextInformation()
{
  QString glType;
  QString glVersion;
  QString glProfile;

  QString glVendor;
  QString glRenderer;


  // Get Version Information
  glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
  glVersion = reinterpret_cast<const char *>(glGetString(GL_VERSION));
  glVendor =
      reinterpret_cast<const char *>(glGetString(GL_VENDOR));
  glRenderer =
      reinterpret_cast<const char *>(glGetString(GL_RENDERER));

  // Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
  switch (format().profile()) {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
  }
#undef CASE

// qPrintable() will print our QString w/o quotes around it.
  qDebug() << qPrintable(glType) << qPrintable(glVersion) << "("
           << qPrintable(glProfile) << ")"
           << "\nOpenGL Vendor: " << qPrintable(glVendor)
           << "\nRendering Device: " << qPrintable(glRenderer) << "\n";


}

void MainWidget::updateCameraInput()
{
  Input::update();
  // Camera Transformation
  if (Input::buttonPressed(Qt::RightButton)) {
    static const float transSpeed = 0.1f;
    static const float rotSpeed = 0.5f;

    // Handle rotations
    Scene::Camera().transform().rotate(-rotSpeed * Input::mouseDelta().x(),
                               Camera3D::LocalUp);
    Scene::Camera().transform().rotate(-rotSpeed * Input::mouseDelta().y(),
                               Scene::Camera().right());

    // Handle translations
    QVector3D translation;
    if (Input::keyPressed(Qt::Key_W)) {
      translation += Scene::Camera().forward();
    }
    if (Input::keyPressed(Qt::Key_S)) {
      translation -= Scene::Camera().forward();
    }
    if (Input::keyPressed(Qt::Key_A)) {
      translation -= Scene::Camera().right();
    }
    if (Input::keyPressed(Qt::Key_D)) {
      translation += Scene::Camera().right();
    }
    if (Input::keyPressed(Qt::Key_Q)) {
      translation -= Scene::Camera().up() / 2.0f;
    }
    if (Input::keyPressed(Qt::Key_E)) {
      translation += Scene::Camera().up() / 2.0f;
    }
    Scene::Camera().transform().translate(transSpeed * translation);
  }
}
