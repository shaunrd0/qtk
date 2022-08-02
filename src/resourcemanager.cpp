/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Manage files and resources used by qtk                              ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include "resourcemanager.h"
#include <algorithm>
#include <string>
#include <QtGlobal>

static std::string nixPath(std::string path)
{
#ifdef Q_OS_WINDOWS
    std::replace(path.begin(), path.end(), '\\', '/');
#endif
    return path;
}

std::string RM::resourcesDir =
    std::string(__FILE__).substr(0, nixPath(__FILE__).find("src/"))
    + "resources/";
