/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: QtkWidget for Qt desktop application                                ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <QKeyEvent>
#include <QMainWindow>
#include <QMimeData>

#include "qtk/input.h"
#include "qtk/scene.h"
#include "qtk/shape.h"

#include "debugconsole.h"
#include "qtkwidget.h"

using namespace Qtk;

/// Static manager for all QtkWidget instances.
QtkWidgetManager QtkWidget::mWidgetManager;

/*******************************************************************************
 * Constructors, Destructors
 ******************************************************************************/

QtkWidget::QtkWidget(QWidget * parent) : QtkWidget(parent, "QtkWidget") {}

QtkWidget::QtkWidget(QWidget * parent, const QString & name) :
    QtkWidget(parent, name, Q_NULLPTR)
{
}

QtkWidget::QtkWidget(QWidget * parent, const QString & name, Scene * scene) :
    QOpenGLWidget(parent), mDebugLogger(Q_NULLPTR),
    mConsole(new DebugConsole(this, name)), mScene(Q_NULLPTR)
{
  setObjectName(name);
  mWidgetManager.add_widget(this);

  setAcceptDrops(true);
  setScene(scene);
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setVersion(4, 6);
  // Set the number of samples used for glEnable(GL_MULTISAMPLING)
  format.setSamples(4);
  // Set the size of the depth bufer for glEnable(GL_DEPTH_TEST)
  format.setDepthBufferSize(16);
  // If QTK_DEBUG is set, enable debug context
  format.setOption(QSurfaceFormat::DebugContext);
  setFormat(format);
  setFocusPolicy(Qt::ClickFocus);
}

QtkWidget::~QtkWidget()
{
  makeCurrent();
  teardownGL();
}

/*******************************************************************************
 * Public Methods
 ******************************************************************************/

QAction * QtkWidget::getActionToggleConsole()
{
  auto action = new QAction(mScene->getSceneName() + " debug console");
  action->setCheckable(true);
  action->setChecked(mConsoleActive);
  action->setStatusTip("Add a debug console for this QtkWidget.");
  connect(action, &QAction::triggered, this, &QtkWidget::toggleConsole);
  return action;
}

void QtkWidget::initializeGL()
{
  initializeOpenGLFunctions();
  // Connect the frameSwapped signal to call the update() function
  connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

  // Add the debug console widget to the window and set its hidden state.
  if (mMainWindow != nullptr) {
    mMainWindow->addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea,
                               mConsole);
  }
  mConsole->setHidden(!mConsoleActive);

  // Initialize OpenGL debug context
  mDebugLogger = new QOpenGLDebugLogger(this);
  if (mDebugLogger->initialize()) {
    qDebug() << "GL_DEBUG Debug Logger" << mDebugLogger << "\n";
    connect(mDebugLogger,
            SIGNAL(messageLogged(QOpenGLDebugMessage)),
            this,
            SLOT(messageLogged(QOpenGLDebugMessage)));
    mDebugLogger->startLogging();
    mConsole->sendLog(
        "Object files such as .obj can be dragged into the scene to load new "
        "models.",
        DebugContext::Warn);
    mConsole->sendLog("Click and hold LMB or RMB to move the camera with WASD.",
                      DebugContext::Warn);
    mConsole->sendLog(
        "Click an object name in the side panel to view or modify properties.",
        DebugContext::Warn);
    mConsole->sendLog(
        "Double click an object name to move the camera to it's position.",
        DebugContext::Warn);
  }

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
}

void QtkWidget::resizeGL(int width, int height)
{
  Scene::getProjectionMatrix().setToIdentity();
  Scene::getProjectionMatrix().perspective(
      45.0f, float(width) / float(height), 0.1f, 1000.0f);
}

void QtkWidget::paintGL()
{
  // Clear buffers and draw the scene if it is valid.
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  if (mScene != Q_NULLPTR) {
    mScene->draw();
  }
}

void QtkWidget::setScene(Scene * scene)
{
  delete mScene;


  mScene = scene;
  if (mScene != Q_NULLPTR) {
    mConsole->setTitle(mScene->getSceneName());
  } else {
    mConsole->setTitle("Null Scene");
  }
}

void QtkWidget::toggleConsole()
{
  mConsole->setHidden(mConsoleActive);
  mConsoleActive = !mConsoleActive;
}

/*******************************************************************************
 * Protected Methods
 ******************************************************************************/

void QtkWidget::dragEnterEvent(QDragEnterEvent * event)
{
  if (event->mimeData()->hasFormat("text/plain")) {
    event->acceptProposedAction();
  }
}

void QtkWidget::dropEvent(QDropEvent * event)
{
  mConsole->sendLog(event->mimeData()->text());
  auto urls = event->mimeData()->urls();
  if (!urls.isEmpty()) {
    if (urls.size() > 1) {
      qDebug() << "Cannot accept drop of multiple files.";
      event->ignore();
      return;
    }

    // TODO: Support other object types.
    auto url = urls.front();
    if (url.fileName().endsWith(".obj")) {
      mScene->loadModel(url);
      event->acceptProposedAction();
    } else {
      qDebug() << "Unsupported file type: " + url.fileName() + "\n";
      event->ignore();
    }
  }
}

void QtkWidget::keyPressEvent(QKeyEvent * event)
{
  if (event->isAutoRepeat()) {
    // Do not repeat input while a key is held down
    event->ignore();
  } else {
    Input::registerKeyPress(event->key());
  }
}

void QtkWidget::keyReleaseEvent(QKeyEvent * event)
{
  if (event->isAutoRepeat()) {
    event->ignore();
  } else {
    Input::registerKeyRelease(event->key());
  }
}

void QtkWidget::mousePressEvent(QMouseEvent * event)
{
  Input::registerMousePress(event->button());
}

void QtkWidget::mouseReleaseEvent(QMouseEvent * event)
{
  Input::registerMouseRelease(event->button());
}

void QtkWidget::update()
{
  updateCameraInput();

  if (mScene != Q_NULLPTR) {
    mScene->update();
  }

  QWidget::update();
}

void QtkWidget::messageLogged(const QOpenGLDebugMessage & msg)
{
  QString error;

  DebugContext context;
  // Format based on severity
  switch (msg.severity()) {
    case QOpenGLDebugMessage::NotificationSeverity:
      error += "--";
      context = Status;
      break;
    case QOpenGLDebugMessage::HighSeverity:
      error += "!!";
      context = Fatal;
      break;
    case QOpenGLDebugMessage::MediumSeverity:
      error += "!~";
      context = Error;
      break;
    case QOpenGLDebugMessage::LowSeverity:
      error += "~~";
      context = Warn;
      break;
  }

  error += " (";

  // Format based on source
#define CASE(c)                \
  case QOpenGLDebugMessage::c: \
    error += #c;               \
    break
  switch (msg.source()) {
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
#define CASE(c)                \
  case QOpenGLDebugMessage::c: \
    error += #c;               \
    break
  switch (msg.type()) {
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

  error += ")\n" + msg.message() + "\n";
  qDebug() << qPrintable(error);
  sendLog("(OpenGL) " + error.replace("\n", "\n(OpenGL) "), context);
}

/*******************************************************************************
 * Private Methods
 ******************************************************************************/

void QtkWidget::teardownGL()
{ /* Nothing to teardown yet... */
}

void QtkWidget::updateCameraInput()
{
  Input::update();
  // Camera Transformation
  if (Input::buttonPressed(Qt::LeftButton)
      || Input::buttonPressed(Qt::RightButton)) {
    static const float transSpeed = 0.1f;
    static const float rotSpeed = 0.5f;

    // Handle rotations
    Scene::getCamera().getTransform().rotate(
        -rotSpeed * Input::mouseDelta().x(), Camera3D::LocalUp);
    Scene::getCamera().getTransform().rotate(
        -rotSpeed * Input::mouseDelta().y(), Scene::getCamera().getRight());

    // Handle translations
    QVector3D translation;
    if (Input::keyPressed(Qt::Key_W)) {
      translation += Scene::getCamera().getForward();
    }
    if (Input::keyPressed(Qt::Key_S)) {
      translation -= Scene::getCamera().getForward();
    }
    if (Input::keyPressed(Qt::Key_A)) {
      translation -= Scene::getCamera().getRight();
    }
    if (Input::keyPressed(Qt::Key_D)) {
      translation += Scene::getCamera().getRight();
    }
    if (Input::keyPressed(Qt::Key_Q)) {
      translation -= Scene::getCamera().getUp() / 2.0f;
    }
    if (Input::keyPressed(Qt::Key_E)) {
      translation += Scene::getCamera().getUp() / 2.0f;
    }
    Scene::getCamera().getTransform().translate(transSpeed * translation);
  }
}

void QtkWidget::printContextInformation()
{
  QString glType;
  QString glVersion;
  QString glProfile;

  QString glVendor;
  QString glRenderer;

  // Get Version Information
  glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
  glVersion = reinterpret_cast<const char *>(glGetString(GL_VERSION));
  glVendor = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
  glRenderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

  // Get Profile Information
#define CASE(c)           \
  case QSurfaceFormat::c: \
    glProfile = #c;       \
    break
  switch (format().profile()) {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
  }
#undef CASE

  auto message = QString(glType) + glVersion + "(" + glProfile + ")"
                 + "\nOpenGL Vendor: " + glVendor
                 + "\nRendering Device: " + glRenderer;
  qDebug() << qPrintable(message);
  sendLog("(OpenGL) " + message.replace("\n", "\n(OpenGL) "), Status);
}
