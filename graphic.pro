QT       += core gui opengl

TARGET = opengl1
TEMPLATE = app
#QMAKE_LIBS+=-static -lgomp -lpthread
#QMAKE_CXXFLAGS+=-fopenmp
QMAKE_CXXFLAGS+=-fopenmp
QMAKE_LFLAGS += -fopenmp

SOURCES += \
    externals/EasyBMP/src/EasyBMP.cpp \
    src/Scene.cpp \
    src/Tracer.cpp \
    src/Mesh.cpp \
    src/Main.cpp \
    src/Camera.cpp \
    src/Light.cpp \
    externals/glm/gtx/intersect.inl


#mainwindow.cpp

HEADERS  += mainwindow.h \
    externals/EasyBMP/include/EasyBMP.h \
    externals/tinyobjloader/tiny_obj_loader.h \
    externals/glm/gtx/intersect.hpp \
    externals/glm/gtx/vec3.hpp \
    externals/glm/glm.hpp \
    externals/glm/gtx/rotate_vector.hpp \
    externals/glm/geometric.hpp \
    include/Camera.h \
    include/Mesh.h \
    include/Scene.h \
    include/Tracer.h \
    include/Ray.h \
    include/Light.h \
    include/Poligon.h \
    include/Antena.h

DISTFILES += \
    models/house.obj \
    configs/conf_up.txt \
    configs/conf_1.txt \
    configs/conf_2.txt
