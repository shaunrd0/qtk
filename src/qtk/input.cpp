/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Input class from tutorials followed at trentreed.net                ##
##                                                                            ##
## Contact: shaunrd0@gmail.com	| URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <algorithm>
#include <vector>

#include <QCursor>

#include "input.h"

using namespace Qtk;

/*******************************************************************************
 * Static Helper Structs
 ******************************************************************************/

/**
 * Struct to hold key input state. When a key is pressed we construct this and
 * store it within a KeyContainer (or ButtonContainer for mouse buttons).
 *
 * @tparam T Qt::Key or Qt::MouseButton input type for this instance.
 */
template <typename T> struct InputInstance : std::pair<T, Input::InputState> {
    typedef std::pair<T, Input::InputState> base_class;

    // Disable clang-tidy from marking this ctor explicit
    // NOLINTNEXTLINE
    inline InputInstance(T value) : base_class(value, Input::InputInvalid) {}

    inline InputInstance(T value, Input::InputState state) :
        base_class(value, state) {}

    // Allows use of std::find to search for a key's InputInstance
    inline bool operator==(const InputInstance & rhs) const {
      return this->first == rhs.first;
    }
};

// Key, button instance typedefs
typedef InputInstance<Qt::Key> KeyInstance;
typedef InputInstance<Qt::MouseButton> ButtonInstance;

// Key, button instance container typedefs
typedef std::vector<KeyInstance> KeyContainer;
typedef std::vector<ButtonInstance> ButtonContainer;

// Static containers for key, button instances
static KeyContainer sg_keyInstances;
static ButtonContainer sg_buttonInstances;
// Static containers for mouse data
static QPoint sg_mouseCurrPosition;
static QPoint sg_mousePrevPosition;
static QPoint sg_mouseDelta;

/*******************************************************************************
 * Static Inline Helper Functions
 ******************************************************************************/

/**
 * Search for the InputInstance of a key.
 *
 * @param value The key to search for.
 * @return Iterator to the found element or the end iterator if not found.
 */
static inline KeyContainer::iterator FindKey(Qt::Key value) {
  return std::find(sg_keyInstances.begin(), sg_keyInstances.end(), value);
}

/**
 * Search for the InputInstance of a mouse button.
 *
 * @param value The mouse button to search for.
 * @return Iterator to the found element or the end iterator if not found.
 */
static inline ButtonContainer::iterator FindButton(Qt::MouseButton value) {
  return std::find(sg_buttonInstances.begin(), sg_buttonInstances.end(), value);
}

/**
 * Check an InputInstance for the InputReleased state.
 *
 * @tparam TPair KeyInstance or ButtonInstance
 * @param instance Instance to check for InputReleased state.
 * @return True if the InputInstance is in the released state.
 */
template <typename TPair>
static inline bool CheckReleased(const TPair & instance) {
  return instance.second == Input::InputReleased;
}

/**
 * Updates an InputInstance and applies transitions if needed.
 *
 * @tparam TPair KeyInstance or ButtonInstance.
 * @param instance The InputInstance to update.
 */
template <typename TPair> static inline void UpdateStates(TPair & instance) {
  switch(instance.second) {
    case Input::InputRegistered:
      instance.second = Input::InputTriggered;
      break;
    case Input::InputTriggered:
      instance.second = Input::InputPressed;
      break;
    case Input::InputUnregistered:
      instance.second = Input::InputReleased;
      break;
    default:
      break;
  }
}

/**
 * Updates InputInstance containers to track input state.
 *
 * @tparam Container The type of container, KeyContainer or ButtonContainer.
 * @param container The InputInstance container to update.
 */
template <typename Container> static inline void Update(Container & container) {
  typedef typename Container::iterator Iter;
  typedef typename Container::value_type TPair;

  // Remove old data
  auto remove =
      std::remove_if(container.begin(), container.end(), &CheckReleased<TPair>);
  container.erase(remove, container.end());

  // Update existing data
  std::for_each(container.begin(), container.end(), &UpdateStates<TPair>);
}

/*******************************************************************************
 * Static Public Methods
 ******************************************************************************/

void Input::update() {
  // Update Mouse Delta
  sg_mousePrevPosition = sg_mouseCurrPosition;
  sg_mouseCurrPosition = QCursor::pos();
  sg_mouseDelta = sg_mouseCurrPosition - sg_mousePrevPosition;

  // Update KeyState values
  Update(sg_buttonInstances);
  Update(sg_keyInstances);
}

void Input::registerKeyPress(int k) {
  auto it = FindKey((Qt::Key)k);
  if(it == sg_keyInstances.end()) {
    sg_keyInstances.push_back(KeyInstance((Qt::Key)k, InputRegistered));
  }
}

void Input::registerKeyRelease(int k) {
  auto it = FindKey((Qt::Key)k);
  if(it != sg_keyInstances.end()) {
    it->second = InputUnregistered;
  }
}

void Input::registerMousePress(Qt::MouseButton btn) {
  auto it = FindButton(btn);
  if(it == sg_buttonInstances.end()) {
    sg_buttonInstances.push_back(ButtonInstance(btn, InputRegistered));
  }
}

void Input::registerMouseRelease(Qt::MouseButton btn) {
  auto it = FindButton(btn);
  if(it != sg_buttonInstances.end()) {
    it->second = InputUnregistered;
  }
}

void Input::reset() {
  sg_keyInstances.clear();
  sg_buttonInstances.clear();
}

Input::InputState Input::keyState(Qt::Key k) {
  auto it = FindKey(k);
  return (it != sg_keyInstances.end()) ? it->second : InputInvalid;
}

Input::InputState Input::buttonState(Qt::MouseButton k) {
  auto it = FindButton(k);
  return (it != sg_buttonInstances.end()) ? it->second : InputInvalid;
}

QPoint Input::mousePosition() {
  return QCursor::pos();
}

QPoint Input::mouseDelta() {
  return sg_mouseDelta;
}
