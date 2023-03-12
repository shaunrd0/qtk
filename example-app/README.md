This is an example application that is using the Qtk API to create custom Qt
OpenGL widgets. This is very similar to `QtkWidget` in the Qtk desktop
application source code, but could be modified for different uses if needed.

There are no camera controls supported in this example. The camera is fixed.
If these controls are desired, they can be implemented by the client.

You can import your own models within `examplescene.cpp`, inside the
`ExampleScene::init()` function. Rotations and translations
are applied in `ExampleScene::update()`.

The syntax for adding shapes and models is seen in the example below.
This would result in a scene with a red cube and a miniature spartan model
placed on top.

```C++
void ExampleScene::init() {
  // Add a skybox to the scene using default cube map images and settings.
  setSkybox(new Qtk::Skybox("Skybox"));

  /* Create a red cube with a mini master chief on top. */
  auto myCube = new MeshRenderer("My cube", Cube(Qtk::QTK_DRAW_ELEMENTS));
  myCube->setColor(RED);
  mMeshes.push_back(myCube);

  auto mySpartan = new Model("My spartan", "/path/to/spartan/spartan.obj");
  mySpartan->getTransform().setTranslation(0.0f, 0.5f, 0.0f);
  mySpartan->getTransform().setScale(0.5f);
  mModels.push_back(mySpartan);
}
```

If we want to make our spartan spin, we need to apply rotation in `update`

```C++
void ExampleScene::update() {
  auto mySpartan = Model::getInstance("My spartan");
  mySpartan->getTransform().rotate(0.75f, 0.0f, 1.0f, 0.0f);

  auto myCube = MeshRenderer::getInstance("My cube");
  myCube->getTransform().rotate(-0.75f, 0.0f, 1.0f, 0.0f);
}
```

Other examples can be found in the source files for this example project.

## Build Instructions

Currently, this application requires manual build and installation of Qtk.
In the future, once a release is published, I will be able to use `FetchContent`
or similar cmake functionality to remove this requirement.

For Qtk build instructions, see the README in the root of this repository.

```bash
cmake -S /path/to/qtk/example-app/ -B /path/to/qtk/example-app/build
cmake --build /path/to/qtk/example-app/build
```

If Qtk was not installed system-wide, we can set `QTK_PATH` to point to the
custom installation directory.

```bash
cmake -S /path/to/qtk/example-app/ -B /path/to/qtk/example-app/build -DQTK_PATH=/path/to/qtk/install/
cmake --build /path/to/qtk/example-app/build
```

After this, we can run the example application -

```bash
./path/to/qtk/example-app/build/bin/example
```
