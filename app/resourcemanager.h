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
     * @return Absoulte system path to a qtk asset
     */
    static std::string getPath(const std::string & path) {
      // Only construct qtk resource path if in qrc format; else return it as-is
      return path[0] == ':' ? QTK_RESOURCES + path.substr(1) : path;
    }
} RM;

#endif  // QTK_RESOURCEMANAGER_H
