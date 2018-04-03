#include <QTime>

#include "include/Tracer.h"
#include "stdio.h"
#include "stdio.h"

#include "externals/glm/glm.hpp"

using namespace std;

int main(int argc, char** argv)
{
    int xResFromFile = 500;
    int yResFromFile = 500;
    char buf_input_obj[20];
    char buf_output[20];
    int size_x, size_y, size_z;
    glm::vec3 pos, dir, right, up, pos_light, pos_ant;
    glm::vec2 view;
    float pos_x, pos_y, pos_z, f;
    bool anti, post;
    float post_val;
    if(argc == 2) // There is input file in parameters
    {
        cout << "OK" << endl;
        FILE* file = fopen(argv[1], "r");
        if(file)
        {
            //cout << "OK" << endl;
            fscanf(file, "%d %d", &xResFromFile, &yResFromFile);//size img
            fscanf(file, "%s%d%d%d", &buf_input_obj, &size_x, &size_y, &size_z);//parametrs scen
            //cout << size_x << ' ' << size_y << ' ' << size_z << endl;
            fscanf(file, "%f%f%f", &pos_x, &pos_y, &pos_z);
            pos = glm::vec3(pos_x, pos_y, pos_z);
            cout << pos_x << ' ' << pos_y << ' ' << pos_z << endl;
            fscanf(file, "%f%f%f", &pos_x, &pos_y, &pos_z);
            dir = glm::vec3(pos_x, pos_y, pos_z);
            cout << pos_x << ' ' << pos_y << ' ' << pos_z << endl;
            fscanf(file, "%f%f%f", &pos_x, &pos_y, &pos_z);
            right = glm::vec3(pos_x, pos_y, pos_z);
            //cout << pos_x << ' ' << pos_y << ' ' << pos_z << endl;
            fscanf(file, "%f%f%f", &pos_x, &pos_y, &pos_z);
            up = glm::vec3(pos_x, pos_y, pos_z);
            //cout << pos_x << ' ' << pos_y << ' ' << pos_z << endl;
            fscanf(file, "%f%f", &pos_x, &pos_y);
            view = glm::vec2(pos_x, pos_y);
            //cout << pos_x << ' ' << pos_y << ' ' << pos_z << endl;
            fscanf(file, "%f%f%f", &pos_x, &pos_y, &pos_z);
            pos_light = glm::vec3(pos_x, pos_y, pos_z);
            //cout << pos_x << ' ' << pos_y << ' ' << pos_z << endl;
            fscanf(file, "%f%f%f", &pos_x, &pos_y, &pos_z);
            pos_ant = glm::vec3(pos_x, pos_y, pos_z);
            fscanf(file, "%f", &f);
            //cout << f << endl;
            fscanf(file, "%d", &size_x);
            anti = size_x;
            //cout << anti << endl;
            fscanf(file, "%d%f", &size_x, &pos_x);
            post = size_x;
            post_val = pos_x;
            fscanf(file, "%s", &buf_output);
            //cout << buf_output << endl;
                    //Camera (glm::vec3 pos, glm::vec3 dir, glm::vec3 right, glm::vec3 up, glm::vec2 view);
            fclose(file);
        } else
          printf("Invalid config path! Using default parameters.\r\n");
     } else
        printf("No config! Using default parameters.\r\n");

    //cout << buf << ' ' << size_x << ' ' << size_y << ' ' << size_z << std::endl;

    QTime t;
    t.start();

    Tracer tracer;
    //Scene scene;

    tracer.xRes = xResFromFile;
    tracer.yRes = yResFromFile;
    tracer.scene = Scene(buf_input_obj, size_x, size_y, size_z);
    tracer.camera = Camera(pos, dir, right, up, view);
    tracer.light = Light(pos_light, glm::vec3(1, 1, 1));
    tracer.antena = Antena(pos_ant, glm::vec3(1, 1, 1), f);
    tracer.anti_aliasing = anti;
    //tracer._scene = &scene;
    tracer.RenderImage();
    if(post)
        tracer.autocontrast(post_val);
    tracer.SaveImageToFile(buf_output);
    qDebug("Time elapsed: %d ms", t.elapsed());
    return 0;
}
