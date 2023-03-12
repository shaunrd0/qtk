/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Input class from tutorials followed at trentreed.net                ##
##                                                                            ##
## Contact: shaunrd0@gmail.com	| URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef QTOPENGL_INPUT_H
#define QTOPENGL_INPUT_H

#include <QPoint>
#include <Qt>

#include "qtkapi.h"

namespace Qtk {
  class QTKAPI Input {
    public:
      /*************************************************************************
       * Typedefs
       ************************************************************************/

      /**
       * Possible key states. See UpdateStates for state transitions.
       *
       * When a key is pressed we enter states Registered->Triggered->Pressed.
       * InputTriggered state will be met only once if a key is pressed or held.
       * While a key is held down the state is InputPressed.
       *
       * When a key is released we enter InputUnregistered->InputReleased
       * When an active InputInstance for a key has this state it is removed.
       */
      enum InputState {
        InputInvalid,
        InputRegistered,    // Initial state. Transitions to InputTriggered
        InputUnregistered,  // Transition to InputReleased
        InputTriggered,     // Transition to InputPressed
        InputPressed,       // State of a key while it is held down.
        InputReleased       // Released keys are removed from state containers.
      };

      /*************************************************************************
       * Public Methods
       ************************************************************************/

      //
      // State updating.

      /**
       * Update state for all mouse button and key instances.
       */
      static void update();

      /**
       * @param key Key to set InputRegistered state.
       */
      static void registerKeyPress(int key);

      /**
       * @param key Key to set InputReleased state.
       */
      static void registerKeyRelease(int key);

      /**
       * @param button Mouse button to set InputRegistered state.
       */
      static void registerMousePress(Qt::MouseButton button);

      /**
       * @param button Mouse button to set InputReleased state.
       */
      static void registerMouseRelease(Qt::MouseButton button);

      /**
       * Reset input state for all key and mouse buttons.
       */
      static void reset();

      //
      // State Checking.

      /**
       * @param key Key to check state.
       * @return True if the key is in InputTriggered state.
       */
      inline static bool keyTriggered(Qt::Key key) {
        return keyState(key) == InputTriggered;
      }

      /**
       * @param key Key to check state.
       * @return True if the key is in InputPressed state.
       */
      inline static bool keyPressed(Qt::Key key) {
        return keyState(key) == InputPressed;
      }

      /**
       * @param key Key to check state.
       * @return True if the key is in InputReleased state.
       */
      inline static bool keyReleased(Qt::Key key) {
        return keyState(key) == InputReleased;
      }

      /**
       * @param button Mouse button to check state.
       * @return True if the key is in InputTriggered state.
       */
      inline static bool buttonTriggered(Qt::MouseButton button) {
        return buttonState(button) == InputTriggered;
      }

      /**
       * @param button Mouse button to check state.
       * @return True if the key is in InputPressed state.
       */
      inline static bool buttonPressed(Qt::MouseButton button) {
        return buttonState(button) == InputPressed;
      }

      /**
       * @param button Mouse button to check state.
       * @return True if the key is in InputReleased state.
       */
      inline static bool buttonReleased(Qt::MouseButton button) {
        return buttonState(button) == InputReleased;
      }

      /**
       * @param key The key to check InputState.
       * @return The current InputState for the given key.
       */
      static InputState keyState(Qt::Key key);

      /**
       * @param button The mouse button to check InputState.
       * @return The current InputState for the mouse button.
       */
      static InputState buttonState(Qt::MouseButton button);

      /**
       * @return QPoint representing the mouse position within the widget.
       */
      static QPoint mousePosition();

      /**
       * @return Delta movement of mouse from previous to current position.
       */
      static QPoint mouseDelta();
  };
}  // namespace Qtk

#endif  // QTOPENGL_INPUT_H
