/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Main window for Qt6 OpenGL widget application                       ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/
#ifndef QTK_QTKAPI_H
#define QTK_QTKAPI_H

#include <QtCore/QtGlobal>

#ifdef QTK_SHARED
#if defined(QTK_EXPORT)
#define QTKAPI Q_DECL_EXPORT
#else
#define QTKAPI Q_DECL_IMPORT
#endif
#else
#define QTKAPI
#endif

#endif  // QTK_QTKAPI_H
