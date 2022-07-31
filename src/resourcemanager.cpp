/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Manage files and resources used by qtk                              ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include "resourcemanager.h"

std::string RM::resourcesDir =
    std::string(__FILE__).substr(0, std::string(__FILE__).find("src/"))
    + "resources/";
