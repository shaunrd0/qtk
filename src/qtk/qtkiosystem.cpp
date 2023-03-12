/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Custom IO system for Qtk to support Qt Resource paths in Assimp     ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include "qtkiosystem.h"

using namespace Qtk;

/*******************************************************************************
 * Public Member Functions
 ******************************************************************************/

bool QtkIOSystem::Exists(const char * pFile) const {
  return QFileInfo::exists(pFile);
}

char QtkIOSystem::getOsSeparator() const {
#ifndef _WIN32
  return '/';
#else
  return '\\';
#endif
}

Assimp::IOStream * QtkIOSystem::Open(const char * pFile, const char * pMode) {
  if(!QFileInfo::exists(pFile)) {
    qDebug() << "[Qtk::QtkIOSystem] failed to open file: " << pFile << "\n";
    return nullptr;
  }
  return new QtkIOStream(pFile, pMode);
}

void QtkIOSystem::Close(Assimp::IOStream * pFile) {
  delete pFile;
}
