//
//  Camera.h
//  prototype
//
//  Created by Yue Huang on 2015-01-27.
//  Copyright (c) 2015 Yue Huang. All rights reserved.
//

#ifndef __prototype__Camera__
#define __prototype__Camera__

#include "glm.hpp"
#include <math.h> 

struct Camera{
    glm::mat4x4 matrix;
    glm::vec3 pos;
    glm::vec3 lookAt; //in 1st persion shooter...that is where the mouse is pointint
    glm::vec3 up;
    
    glm::vec3 x, y, z;
    
    Camera(glm::vec3 cameraPos, glm::vec3 cameraLookAt, glm::vec3 cameraUp);
    float array[16];
    void slide(glm::vec3 direction);
    void yaw(float angle); //left and right rotate based on center
    void pitch(float angle);
    void roll(float angle);
    
    float* get(){ return array;}
};



#endif /* defined(__prototype__Camera__) */
