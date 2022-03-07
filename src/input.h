/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Input class from tutorials followed at trentreed.net                ##
##                                                                            ##
## Contact: shaunrd0@gmail.com	| URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef QTOPENGL_INPUT_H
#define QTOPENGL_INPUT_H

#include <QPoint>
#include <Qt>


class Input {
  friend class MainWidget;
public:

  // Possible key states
  enum InputState
  {
    InputInvalid,
    InputRegistered,
    InputUnregistered,
    InputTriggered,
    InputPressed,
    InputReleased
  };

  // State checking
  inline static bool keyTriggered(Qt::Key key)
  { return keyState(key) == InputTriggered;}
  inline static bool keyPressed(Qt::Key key)
  { return keyState(key) == InputPressed;}
  inline static bool keyReleased(Qt::Key key)
  { return keyState(key) == InputReleased;}
  inline static bool buttonTriggered(Qt::MouseButton button)
  { return buttonState(button) == InputTriggered;}
  inline static bool buttonPressed(Qt::MouseButton button)
  { return buttonState(button) == InputPressed;}
  inline static bool buttonReleased(Qt::MouseButton button)
  { return buttonState(button) == InputReleased;}

  // Implementation
  static InputState keyState(Qt::Key key);
  static InputState buttonState(Qt::MouseButton button);

  static QPoint mousePosition();
  static QPoint mouseDelta();

private:

  // State updating
  static void update();
  static void registerKeyPress(int key);
  static void registerKeyRelease(int key);
  static void registerMousePress(Qt::MouseButton button);
  static void registerMouseRelease(Qt::MouseButton button);
  static void reset();
};

#endif // QTOPENGL_INPUT_H
