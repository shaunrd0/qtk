# Qtk

Practice project for learning about using OpenGL in Qt5 widget applications. 
Model loader using [Assimp](https://assimp.org/) within a Qt5 widget application.

You can import your own models within `mainwdget.cpp`, inside the
`MainWidget::initObjects()` function. I've commented throughout the code there
to explain which model or example I'm modifying. Rotations and translations
happen in `MainWidget::update()`, to get textures loading on models look into 
[material files](http://www.paulbourke.net/dataformats/mtl/) and see some 
examples in the `resources/models/` directory.

Can be built with cmake manually or using 
[Qt Creator](https://github.com/qt-creator/qt-creator).

To build and run `qtk` on Ubuntu -
```bash
# Qt Creator
sudo apt update -y && sudo apt install qttools5-dev freeglut3-dev libassimp-dev cmake build-essential
git clone https://gitlab.com/shaunrd0/qtk && cd qtk
mkdir build && cd build
cmake .. && cmake --build .
./qtk
```

You can fly around the scene if you hold the right mouse button and use WASD.
If you see a small triangle floating by a model it represents the light source
that is being used for the shader rendering the model. These appear on models
using phong, specular, and diffuse lighting techniques.

![](resources/screenshot.png)

Spartan with no normals -

![](resources/spartan-specular.png)

Spartan with normals -

![](resources/spartan-normals.png)

## Model Artists


"Alien Hominid" (https://skfb.ly/onStx) by Nwilly_art is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

"Scythe World Of Warcraft" (https://skfb.ly/6UooG) by Warcraft-3D-Models is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

"Spartan Armour MKV - Halo Reach" () by McCarthy3D is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

"Survival Guitar Backpack (Low Poly)" (https://skfb.ly/6RnCB) by Berk Gedik is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
Model by Berk Gedik, from: https://sketchfab.com/3d-models/survival-guitar-backpack-low-poly-799f8c4511f84fab8c3f12887f7e6b36
Modified (learnopengl.com) material assignment (Joey de Vries) for easier load in OpenGL model loading chapter, and renamed albedo to diffuse and metallic to specular to match non-PBR lighting setup.

"Terror-bird (NHMW-Geo 2012/0007/0001)" (https://skfb.ly/onAWy) by Natural History Museum Vienna is licensed under Creative Commons Attribution-NonCommercial (http://creativecommons.org/licenses/by-nc/4.0/).

"Golden Lion Sitting OBJ Low Poly FREE" (https://skfb.ly/onZAH) by LordSamueliSolo is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

