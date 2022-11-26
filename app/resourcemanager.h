/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Manage files and resources used by qtk                              ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <string>

#include <src/qtkresources.h>

#ifndef QTK_RESOURCEMANAGER_H
#define QTK_RESOURCEMANAGER_H

/**
 * ResourceManager class used to construct absolute paths to files within the Qt
 * resources path. There is no need to manually call this method.
 * Model::loadModel(...) will use this method if a Qt resource path is provided.
 * The Model constructor behaves the same. If a path is prefixed with `:/` this
 * static method will be used to resolve a full system path.
 *
 * This will likely be deprecated. It has a single call site and it is not
 * meant for public use. It is public only for convenience.
 *
 * RM::getPath(":/models/alien-hominid/alien.obj") =
 *      /full/path/to/models/alien-hominid/alien.obj
 */
typedef class ResourceManager {
  public:
    /**
     * Takes a path using qrc format and constructs full system path to qtk
     * assets Qrc format prefix ':/' is trimmed from the path for the caller
     * Assets used with RM may (or may not) appear in qtk/resources.qrc
     *
     * @param path Path relative to qtk/resources/; ie)
     * ':/models/backpack/backpack.obj' An asset at location
     * qtk/resources/path/to/asset.obj Should be given in qrc format:
     * ':/path/to/asset.obj'
     * @return Absolute system path to a qtk asset
     */
    static std::string getPath(const std::string & path) {
      // Only construct qtk resource path if in qrc format; else return it as-is
      return path[0] == ':' ? QTK_RESOURCES + path.substr(1) : path;
    }
} RM;

#endif  // QTK_RESOURCEMANAGER_H
