/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Custom IO system for Qtk to support Qt Resource paths in Assimp     ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

#include "qtkapi.h"
#include "qtkiostream.h"

#ifndef QTK_QTKIOSYSTEM_H
#define QTK_QTKIOSYSTEM_H

namespace Qtk {
  /**
   * Assimp IO system for loading models with assimp, using Qt Resource paths.
   */
  class QtkIOSystem : public Assimp::IOSystem {
    public:
      QtkIOSystem() = default;
      ~QtkIOSystem() = default;

      /**
       * @param pFile File path to check.
       * @return True if the file exists, else false.
       */
      bool Exists(const char * pFile) const override;

      /**
       * @return Path separator for platform OS.
       */
      [[nodiscard]] char getOsSeparator() const override;

      /**
       * @param pFile File to open for read / writing.
       * @param pMode Mode to open file. See `man fopen`.
       * @return QtkIOStream for the opened file.
       */
      Assimp::IOStream * Open(
          const char * pFile, const char * pMode = "rb") override;

      /**
       * @param pFile File to close.
       */
      void Close(Assimp::IOStream * pFile) override;
  };
}  // namespace Qtk

#endif  // QTK_QTKIOSYSTEM_H
