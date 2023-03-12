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
  bool read = mode.contains('r');
  bool write = mode.contains('w');
  if(read && write) {
    mFile.open(QIODevice::ReadWrite);
  } else if(read) {
    mFile.open(QIODevice::ReadOnly);
  } else if(write) {
    mFile.open(QIODevice::WriteOnly);
  } else {
    qDebug() << "[Qtk::QtkIOStream] Invalid file open mode: " << mode << "\n";
  }
}

/*******************************************************************************
 * Public Member Functions
 ******************************************************************************/

size_t QtkIOStream::Read(void * pvBuffer, size_t pSize, size_t pCount) {
  size_t read = 0;
  do {
    auto readSize = mFile.read((char *)pvBuffer + read, pSize);
    if(readSize < 0) {
      qDebug() << "[Qtk::QtkIOStream] Failed to read (" << pSize
               << ") bytes from file at: " << mFile.filesystemFileName().c_str()
               << "\n";
      return -1;
    }
    read += readSize;
  } while(pCount--);
  return read;
}

size_t QtkIOStream::Write(const void * pvBuffer, size_t pSize, size_t pCount) {
  size_t wrote = 0;
  do {
    auto writeSize = mFile.write((char *)pvBuffer + wrote, pSize);
    if(writeSize < 0) {
      qDebug() << "[Qtk::QtkIOStream] Failed to write buffer with size ("
               << pSize
               << ") to file at: " << mFile.filesystemFileName().c_str()
               << "\n";
      return -1;
    }
    wrote += writeSize;
  } while(pCount--);
  return wrote;
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
