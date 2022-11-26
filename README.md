# Qtk
[![All Builds](https://github.com/shaunrd0/qtk/actions/workflows/all-builds.yml/badge.svg)](https://github.com/shaunrd0/qtk/actions/workflows/all-builds.yml)
[![Linting](https://github.com/shaunrd0/qtk/actions/workflows/linting.yml/badge.svg)](https://github.com/shaunrd0/qtk/actions/workflows/linting.yml)

Model loader using [Assimp](https://assimp.org/) within a Qt widget application.

You can import your own models within `app/examplescene.cpp`, inside the
`ExampleScene::init()` function. Rotations and translations
happen in `ExampleScene::update()`.

To get textures loading on models look into [material files](http://www.paulbourke.net/dataformats/mtl/)
and see some examples in the `resources/models/` directory.

The syntax for adding shapes and models is seen in the example below.
This would result in a scene with a red cube and a miniature spartan model placed on top.

```C++
// From: qtk/app/examplescene.cpp

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
}
```

If we want to make our spartan spin, we need to apply rotation in `update`

```C++
// From: qtk/app/examplescene.cpp

void ExampleScene::update() {
  auto mySpartan = Model::getInstance("My spartan");
  mySpartan->getTransform().rotate(0.75f, 0.0f, 1.0f, 0.0f);

  auto myCube = MeshRenderer::getInstance("My cube");
  myCube->getTransform().rotate(-0.75f, 0.0f, 1.0f, 0.0f);
}
```

### Source Builds

Builds are configured for CLion or [Qt Creator](https://github.com/qt-creator/qt-creator).
Simply open the root `CMakeLists.txt` with either of these editors and configurations will be loaded.

This project has been ported to Qt6, which is not yet available in Ubuntu apt repositories.
To run this project, you will *need* to install [Qt6 Open Source Binaries](https://www.qt.io/download-qt-installer) for your system.
Be sure to take note of the Qt6 installation directory, as we will need it to correctly set our `CMAKE_PREFIX_PATH` in the next steps.

#### Linux

Once Qt6 is installed, to build and run `qtk` on Ubuntu -
```bash
sudo apt update -y && sudo apt install libassimp-dev cmake build-essential git
git clone https://gitlab.com/shaunrd0/qtk
cmake -DCMAKE_PREFIX_PATH=$HOME/Qt/6.3.1/gcc_64 -S qtk/ -B qtk/build/ && cmake --build qtk/build/ -j $(nproc --ignore=2) --target qtk-main
./qtk/build/qtk-main
```

By default, the build will initialize Assimp as a git submodule and build from source.
We can turn this off by setting the `-DQTK_UPDATE_SUBMODULES=OFF` flag when running CMake.
This will greatly increase build speed, but we will need to make sure Assimp is available either system-wide or using a custom `CMAKE_PREFIX_PATH`.
Using `-DQTK_UPDATE_SUBMODULES=ON` supports providing assimp on cross-platform builds (Windows / Mac / Linux) and may be easier to configure.

```bash
sudo apt update -y && sudo apt install freeglut3-dev libassimp-dev cmake build-essential git
git clone https://gitlab.com/shaunrd0/qtk
cmake -DQTK_UPDATE_SUBMODULES=OFF -DCMAKE_PREFIX_PATH=$HOME/Qt/6.3.1/gcc_64 -S qtk/ -B qtk/build/ && cmake --build qtk/build/ -j $(nproc --ignore=2) --target qtk-main
# We can also provide a path to assimp -
#cmake -DQTK_UPDATE_SUBMODULES=OFF -DCMAKE_PREFIX_PATH=$HOME/Qt/6.3.1/gcc_64;/path/to/assimp/ -S qtk/ -B qtk/build/ && cmake --build qtk/build/ -j $(nproc --ignore=2) --target qtk-main
./qtk/build/qtk-main
```

#### Windows / MacOS

If you are building on **Windows / Mac** and bringing your own installation of Assimp, consider setting the `-DASSIMP_NEW_INTERFACE` build flag.
```bash
cmake -DASSIMP_NEW_INTERFACE=ON -DQTK_UPDATE_SUBMODULES=OFF -DCMAKE_PREFIX_PATH=$HOME/Qt/6.3.1/gcc_64;/path/to/assimp/ -S qtk/ -B qtk/build/ && cmake --build qtk/build/ -j $(nproc --ignore=2) --target qtk-main
```


#### Development

This project uses version `15.0.5` of `clang-format`.
Before merging any branch we should run `clang-tidy` followed by `clang-format`.

```bash
git clone git@github.com:llvm/llvm-project.git -b llvmorg-15.0.5
cd llvm-project
cmake -B build -DLLVM_ENABLE_PROJECTS=clang -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi" -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" llvm
cmake --build build -j $(nproc --ignore=2)
sudo cmake --build build -j $(nproc --ignore=2) --target install
```

If this version is any earlier than `15.0.0`, running `clang-format` will fail because this project uses configuration options made available since `15.0.0`.

```bash
clang-format --version
clang-format version 15.0.5 (git@github.com:llvm/llvm-project.git 154e88af7ec97d9b9f389e55d45bf07108a9a097)
```

CLion has integration for IDE code reformatting actions with `clang-format`.
If you're using CLion, the `.clang-format` configuration will be picked up by CLion automatically.

`clang-tidy` can be run with the following commands.

```bash
# Move to the root of the repo
cd qtk
# Build
cmake -B build && cmake --build build 
clang-tidy -p build/ --fix --config-file=.clang-tidy src/*.cpp src/*.h app/*.cpp app/*.h
```

Last we need to run `clang-format`, this can be done with the command directly.
This will reformat all the code in the repository.

```bash
clang-format -i --style=file:.clang-format src/*.cpp src/*.h app/*.cpp app/*.h
```

`clang-format` can be run with git integration (or CLion if you prefer).
Git will only reformat the lines you modified, which can be useful.

```bash
# If we want to format the last N commits
# git clang-format HEAD~N
# 3 commits
git clang-format HEAD~3
changed files:
    app/examplescene.h
    app/mainwindow.h
    src/abstractscene.cpp
    src/skybox.h
    src/texture.cpp
    src/texture.h
    src/transform3D.h
```

### Controls

You can fly around the scene if you hold the right mouse button and use WASD.
If you see a small triangle floating by a model it represents the light source
that is being used for the shader rendering the model. These appear on models
using phong, specular, and diffuse lighting techniques.

![](resources/screenshot.png)

Spartan with no normals -

![](resources/spartan-specular.png)

Spartan with normals -

![](resources/spartan-normals.png)


### QtkWidget in Qt Creator

We can add more QtkWidgets to view and render the scene from multiple perspectives.
There is still some work to be done here, so there isn't a builtin way to add an additional view within the application.

![](resources/qtk-views.png)

After building Qtk, we can drag and drop an `OpenGL Widget` onto the `mainwindow.ui`.
Then right-click the new OpenGLWidget and `Promote To->QtkWidget` to add a second view.

![](resources/qtk-views-setup.png)

If we demote or delete all widgets in `mainwindow.ui` and rebuild the project, Qt Creator will drop `QtkWidget` from the list of possible promoted widgets.
Add an `OpenGL Widget` to the UI, right-click it and navigate to `Promote Widget...` and enter the information below.

![](resources/qtk-reference.png)

After you fill out the `New Promoted Class` form, click `Add` *and*`Promote`, then rebuild.
After following these steps Qt Creator will list `QtkWidget` as an option to promote `OpenGL Widgets` again.


## Model Artists

"Alien Hominid" (https://skfb.ly/onStx) by Nwilly_art is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

"Scythe World Of Warcraft" (https://skfb.ly/6UooG) by Warcraft-3D-Models is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

"Spartan Armour MKV - Halo Reach" (https://skfb.ly/6QVvM) by McCarthy3D is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

"Survival Guitar Backpack (Low Poly)" (https://skfb.ly/6RnCB) by Berk Gedik is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
Model by Berk Gedik, from: https://sketchfab.com/3d-models/survival-guitar-backpack-low-poly-799f8c4511f84fab8c3f12887f7e6b36
Modified (learnopengl.com) material assignment (Joey de Vries) for easier load in OpenGL model loading chapter, and renamed albedo to diffuse and metallic to specular to match non-PBR lighting setup.

"Terror-bird (NHMW-Geo 2012/0007/0001)" (https://skfb.ly/onAWy) by Natural History Museum Vienna is licensed under Creative Commons Attribution-NonCommercial (http://creativecommons.org/licenses/by-nc/4.0/).

"Golden Lion Sitting OBJ Low Poly FREE" (https://skfb.ly/onZAH) by LordSamueliSolo is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
