/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Object class for storing object data                                ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include "object.h"

using namespace Qtk;

std::string Object::getShaderSourceCode(
    QOpenGLShader::ShaderType shader_type) const
{
  for (const auto & shader : mProgram.shaders()) {
    if (shader->shaderType() == shader_type) {
      return shader->sourceCode().toStdString();
    }
  }
  qDebug() << "Failed to find shader of type " << shader_type;
  return "";
}
