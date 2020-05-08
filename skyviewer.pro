# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: .
# Target is an application:  

FORMS += mainwindow.ui \
         histogramwidget.ui \
         rangecontrol.ui \
         controldialog.ui 

HEADERS += mainwindow.h \
           str_funcs.h \
           map_exception.h \
           debug.h \
           outlog.h \
           heal.h \
           pixel.h \
           skymap.h \
           healpixmap.h \
           colortable.h \
           define_colortable.h \
           glpoint.h \
           face.h \
           boundary.h \
           rigging.h \
           skytexture.h \
           polarargline.h \
           skyviewer.h \
           histogram.h \
           histoview.h \
           histogramwidget.h \
           enums.h \
           rangecontrol.h \
           controldialog.h \
           selectedpixelmodel.h 
SOURCES += main.cpp \
           mainwindow.cpp \
           str_funcs.cpp \
           map_exception.cpp \
           debug.cpp \
           outlog.cpp \
           heal.cpp \
           pixel.cpp \
           skymap.cpp \
           healpixmap.cpp \
           colortable.cpp \
           face.cpp \
           boundary.cpp \
           rigging.cpp \
           skytexture.cpp \
           polarargline.cpp \
           skyviewer.cpp \
           histogram.cpp \
           histogramwidget.cpp \
           histoview.cpp \
           rangecontrol.cpp \
           controldialog.cpp \
           selectedpixelmodel.cpp 
RESOURCES = skyviewer.qrc
QT_VERSION=$$[QT_VERSION]
LANGUAGE = C++
TEMPLATE = app
CONFIG += release warn_on qt thread
CONFIG += thread
LIBS += -lcfitsio -lQGLViewer-qt4
LIBS += -lchealpix
contains( QT_VERSION, "^4\..*" ){
  QT *= xml opengl
}
unix{
  UI_DIR = ui
  MOC_DIR = moc
  OBJECTS_DIR = obj
  INCLUDE_PATH = $$INCLUDE_DIR
  INCLUDEPATH *= $$INCLUDE_PATH
  DEPENDPATH *= $$INCLUDE_PATH
  isEmpty( PREFIX ){
    PREFIX=/usr/local
  }
  isEmpty( LIB_DIR ){
    LIB_DIR = $$PREFIX/lib
  }
  isEmpty( INCLUDE_DIR ){
    INCLUDE_DIR = $$PREFIX/include
  }
#  !exists( $$INCLUDE_DIR/QGLViewer/qglviewer.h ){
#    message( Unable to find QGLViewer/qglviewer.h in $$INCLUDE_PATH )
#    message( Use qmake INCLUDE_DIR~Path/To/QGLViewer/HeaderFiles )
#    error( Replace the ~ by the "equals" character in the above line )
#  }
#  !exists( $$LIB_DIR/libQGLViewer.so* ){
#    message( Unable to find libQGLViewer.so in $$LIB_DIR )
#    message( You should run qmake LIB_DIR~Path/To/QGLViewer/Lib.so )
#    error( Replace the ~ by the "equals" character in the above line )
#  }
  unix{
    LIBS *= -L$$LIB_DIR -lQGLViewer-qt4
  }
  macx{
    LIBS *= -lobjc
  }
}
win32{
  QMAKE_CXXFLAGS = -TP -W1
  QMAKE_CXXFLAGS_RELEASE = -O2
  DEFINES *= QT_THREAD_SUPPORT
  LIBS *= libQGLViewer
}
