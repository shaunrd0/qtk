/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Custom IO stream for Qtk to support Qt Resource paths in Assimp     ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <QFile>
#include <QFileInfo>

#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

#ifndef QTK_QTKIOSTREAM_H
#define QTK_QTKIOSTREAM_H

namespace Qtk {
  /**
   * Custom Assimp IO stream to support QtkIOSystem file handling.
   * Allows direct use of Qt Resource paths for loading models in Assimp.
   */
  class QtkIOStream : public Assimp::IOStream {
      friend class QtkIOSystem;

    protected:
      /** Constructor protected for private usage by QtkIOSystem */
      QtkIOStream(const char * pFile, const char * pMode);

    public:
      ~QtkIOStream() = default;

      /**
       * Reads data into pvBuffer in pCount batches of length pSize.
       * The final pvBuffer will contain data read from all batches.
       *
       * @param pvBuffer Buffer to read data into.
       * @param pSize Size in bytes for each read.
       * @param pCount Number of reads to perform.
       * @return Length of total bytes read into pvBuffer, or -1 on failure.
       */
      size_t Read(void * pvBuffer, size_t pSize, size_t pCount) override;

      /**
       * Writes data from pvBuffer in pCount batches of length pSize.
       * The final mFile member will contain all input data from pvBuffer.
       *
       * @param pvBuffer Buffer to write data from.
       * @param pSize Size in bytes for each write.
       * @param pCount Number of writes to perfom.
       * @return Length of total bytes wrote into buffer, or -1 on failure.
       */
      size_t Write(const void * pvBuffer, size_t pSize, size_t pCount) override;

      /**
       * Change the current read position in the mFile Qt resource.
       *
       * @param pOffset Offset position to set.
       * @param pOrigin Origin position to use for relative offset.
       * @return aiReturn_SUCCESS, or aiReturn_FAILURE on failure.
       */
      aiReturn Seek(size_t pOffset, aiOrigin pOrigin) override;

      /**
       * @return The current position in mFile.
       */
      [[nodiscard]] size_t Tell() const override;

      /**
       * @return The total size of mFile.
       */
      [[nodiscard]] size_t FileSize() const override;

      /**
       * Flushes buffered data to mFile.
       */
      void Flush() override;

    private:
      // Corresponding file for Qt Resource path.
      QFile mFile;
  };
}  // namespace Qtk

#endif  // QTK_QTKIOSTREAM_H
