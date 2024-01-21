/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Custom IO stream for Qtk to support Qt Resource paths in Assimp     ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include "qtkiostream.h"

using namespace Qtk;

/*******************************************************************************
 * Constructors, Destructors
 ******************************************************************************/

QtkIOStream::QtkIOStream(const char * pFile, const char * pMode) :
    mFile(pFile) {
  QString mode(pMode);
  bool open = false;
  if (mode == "w" || mode == "wb") {
    open = mFile.open(QIODeviceBase::WriteOnly);
  } else if (mode == "r" || mode == "rb") {
    open = mFile.open(QIODeviceBase::ReadOnly);
  } else if (mode == "wt") {
    open = mFile.open(QIODeviceBase::WriteOnly | QIODeviceBase::Text);
  } else if (mode == "rt") {
    open = mFile.open(QIODeviceBase::ReadOnly | QIODeviceBase::Text);
  } else {
    open = false;
    qDebug() << "[Qtk::QtkIOStream] Invalid file open mode: " << mode << "\n";
  }
  if(!open) {
    qDebug() << "[Qtk::QtkIOStream] Could not open file: " << QString(pFile) << "\n";
  }
}

/*******************************************************************************
 * Public Member Functions
 ******************************************************************************/

size_t QtkIOStream::Read(void * pvBuffer, size_t pSize, size_t pCount) {
  qint64 readSize = mFile.read((char*)pvBuffer, pSize * pCount);
  if(readSize < 0) {
    qDebug() << "[Qtk::QtkIOStream] Failed to read (" << pSize
             << ") bytes from file at: " << mFile.filesystemFileName().c_str()
             << "\n";
    return -1;
  }
  return readSize;
}

size_t QtkIOStream::Write(const void * pvBuffer, size_t pSize, size_t pCount) {
  qint64 writeSize = mFile.write((char*)pvBuffer, pSize * pCount);
  if(writeSize < 0) {
    qDebug() << "[Qtk::QtkIOStream] Failed to write buffer with size ("
             << pSize
             << ") to file at: " << mFile.filesystemFileName().c_str()
             << "\n";
    return -1;
  }
  return writeSize;
}

aiReturn QtkIOStream::Seek(size_t pOffset, aiOrigin pOrigin) {
  return mFile.seek(pOffset) ? aiReturn_SUCCESS : aiReturn_FAILURE;
}

size_t QtkIOStream::Tell() const {
  return mFile.pos();
}

size_t QtkIOStream::FileSize() const {
  return mFile.size();
}

void QtkIOStream::Flush() {
  mFile.flush();
}
