//
//  Camera.cpp
//  prototype
//
//  Created by Yue Huang on 2015-01-27.
//  Copyright (c) 2015 Yue Huang. All rights reserved.
//

#include "Camera.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

//void printMatrix(glm::mat4x4& matrix){
//    for (unsigned int i = 0; i < 4; i++) {
//        std::cout<<"| " <<matrix[0][i]<<" "<<matrix[1][i]<<" "<<matrix[2][i]<<" "<<matrix[3][i]<<" |\n";
//    }
//}

void printMatrix(float* matrix){
    for (unsigned int i = 0; i < 4; i++) {
        std::cout<<"| ";
        for (unsigned int j = 0; j < 4; j++) {
            std::cout<<matrix[4*j+i]<<" ";
        }
        std::cout<<"|\n";
//        std::cout<<"| " <<matrix[0][i]<<" "<<matrix[1][i]<<" "<<matrix[2][i]<<" "<<matrix[3][i]<<" |\n";
    }
}

void printVec(glm::vec3 v){
    std::cout<<"x:"<<v.x<<" y:"<<v.y<<" z:"<<v.z<<"\n";
}

//void setLookUpMatrixWith(glm::mat4x4 & matrix, glm::vec3 x, glm::vec3 y, glm::vec3 z, glm::vec3 pos){
//    glm::row(matrix, 0) = glm::vec4(x,0);
//    glm::row(matrix, 1) = glm::vec4(y,0);
//    glm::row(matrix, 2) = glm::vec4(z,0);
//    glm::column(matrix, 3) = glm::vec4(glm::dot(x, -pos), glm::dot(y, -pos), glm::dot(z, -pos),1);
//}

//weird...the slide suggest negative position....
void setLookUpMatrixWith(float * array, glm::vec3 x, glm::vec3 y, glm::vec3 z, glm::vec3 pos){
    array[0] = x.x; array[4] = x.y; array[8]  = x.z; array[12] = glm::dot(x, pos);
    array[1] = y.x; array[5] = y.y; array[9]  = y.z; array[13] = glm::dot(y, pos);
    array[2] = z.x; array[6] = z.y; array[10] = z.z; array[14] = glm::dot(z, pos);
    array[3] = 0;   array[7] = 0;   array[11] = 0;   array[15] = 1.f;
}


Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraLookAt, glm::vec3 cameraUp):pos(cameraPos), lookAt(cameraLookAt), up(cameraUp){
    //calculate based on up, pos and lookAt
    z = glm::normalize(lookAt - pos);
    x = glm::normalize(glm::cross(up, z));
    y = glm::cross(z, x);
//    setLookUpMatrixWith(matrix, x, y, z, pos);
//    printVec(x);
//    printVec(y);
//    printVec(z);
    setLookUpMatrixWith(array, x, y, z, pos);
//    printMatrix(array);
//    matrix = glm::lookAt(cameraPos, cameraLookAt, cameraUp);

//    std::cout<<"What we should get ?\n";
//    printMatrix(matrix);
}

void Camera::slide(glm::vec3 dir){
//    auto add = glm::column(matrix, 3);
//    glm::column(matrix, 3) = add +  glm::vec4(glm::dot(x, dir), glm::dot(y, dir), glm::dot(z,dir), 1);
    array[12] = array[12] + glm::dot(x, dir);
    array[13] = array[13] + glm::dot(y, dir);
    array[14] = array[14] + glm::dot(z, dir);
//    printMatrix(array);
    
//    printMatrix(matrix);
}

//x->u
//y->v
//z->n
//Q:Clockwise or counter clockwise?
//Roll works..but not sure why...
void Camera::roll(float angle){ //rotate on z axis, so x and y is the only one change
    float cs = std::cos(glm::pi<float>()/180.0 * angle);
    float sn = std::sin(glm::pi<float>()/180.0 * angle);
    
    glm::vec3 temp_x = x; //this is rotation, right?
    x = temp_x * cs + (-sn) * y;
    y = temp_x * sn + cs * y;
    setLookUpMatrixWith(array, x, y, z, pos);
//    printMatrix(array);
    
//    setLookUpMatrixWith(matrix, x, y, z, pos);
//    glm::row(matrix, 0) = glm::vec4(x,0);
//    glm::row(matrix, 1) = glm::vec4(y,0);
//    glm::column(matrix, 3) = glm::vec4(glm::dot(x, -pos), glm::dot(y, -pos), glm::dot(z, -pos),1);
}

void Camera::yaw(float angle){ //left and right rotate on y axis
    float cs = std::cos(glm::pi<float>()/180.0 * angle);
    float sn = std::sin(glm::pi<float>()/180.0 * angle);
    
    //x = cos x - sin z and z = cos x + sin z
    glm::vec3 temp_x = x;
    x = cs * temp_x + (-sn) * z;
    z = sn * temp_x + cs * z;

    setLookUpMatrixWith(array, x, y, z, pos);
//    printMatrix(array);
//    setLookUpMatrixWith(matrix, x, y, z, pos);
}

void Camera::pitch(float angle){ //up and down rotate on x axis
    float cs = std::cos(glm::pi<float>()/180.0 * angle);
    float sn = std::sin(glm::pi<float>()/180.0 * angle);
    
    //y and z
    //similarly, y and z is
    glm::vec3 temp_y = y;
    y = cs * temp_y + (-sn) * z;
    z = sn * temp_y + cs * z;
    
    setLookUpMatrixWith(array, x, y, z, pos);
//    printMatrix(array);
//    setLookUpMatrixWith(matrix, x, y, z, pos);
}


