//
//  main.cpp
//  Cub
//
//  Created by Yue Huang on 2014-12-08.
//  Copyright (c) 2014 Yue Huang. All rights reserved.
//

#include <OpenGL/OpenGL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GLUT/GLUT.h>
#include <stdio.h>
#include <string>
#include "glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include "Camera.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

inline void printVec4(glm::vec4 v){
    std::cout<<"x:"<<v.x<<" y:"<<v.y<<" z:"<<v.z<<" w:"<<v.w<<std::endl;
}



int main( int argc, char* args[] ){
    SDL_Window* gWindow = NULL;
    SDL_GLContext gContext;
    
    bool init_sucess = true;
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        init_sucess = false;
    }else{
        //Use OpenGL 2.1
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
        
        //Create window
        gWindow = SDL_CreateWindow( "Room", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
        if( gWindow == NULL ){
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            init_sucess = false;
        }else{
            //Create context
            gContext = SDL_GL_CreateContext( gWindow );
            if( gContext == NULL ){
                printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
                init_sucess = false;
            }else{
                //Use Vsync
                if( SDL_GL_SetSwapInterval( 1 ) < 0 ) printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
                
                //Initialize OpenGl
                {
                    glLoadIdentity();
                    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); //because the midal is (0,0,0)

                    glClearColor (0.0, 0.0, 0.0, 0.0);
                    glShadeModel (GL_SMOOTH);
                    glEnable(GL_LIGHTING);
                    glEnable(GL_LIGHT0);
                    glEnable(GL_DEPTH_TEST);

//                    glEnable(GL_COLOR_MATERIAL);
                    GLfloat light_position[] = { 1.0, 5, 1.0, 0 };
                    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

                    GLfloat lmodel_ambient[] = { 1, 1, 1, 0.5 };
                    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
                }
            }
        }
    }
    
    glm::vec3 cameraPosition(0.f,5.f,10.f);
    glm::vec3 cameraLookAt(0.f);
    glm::vec3 camerUp(0.f,1.f,0.f);
    
    glm::mat4x4 frustrum = glm::frustum(-1.f, 1.f, -1.f, 1.f, 1.5f, 20.f);
    glm::mat4x4 lookAt = glm::lookAt(cameraPosition, cameraLookAt, camerUp);
    glm::mat4x4 perspective = glm::perspective(45.f, 1.f, 1.f, 20.f);
    glm::mat4x4 orthogonal = glm::ortho(-5.f, 5.f, -5.f, 5.f, -1.5f, 20.f);

    glm::vec4 cube[8];
    cube[0] = glm::vec4(0,0,0,1.f);
    cube[1] = glm::vec4(0,1,0,1.f);
    cube[2] = glm::vec4(1,1,0,1.f);
    cube[3] = glm::vec4(1,0,0,1.f);
    
    cube[4] = glm::vec4(0,0,1,1.f);
    cube[5] = glm::vec4(0,1,1,1.f);
    cube[6] = glm::vec4(1,1,1,1.f);
    cube[7] = glm::vec4(1,0,1,1.f);

    
    
    //remeber this is column based....
    glm::mat4x4 shear_matrix(
        1, 0, 0, 0, //x
        0, 1, 0, 0, //y
        0.5, 0.8, 1, 0,
        0, 0, 0, 1
        //x , y, z, w
    );
   
    
    for (unsigned int i = 0; i < 8; i++) {
        
        cube[i] = frustrum * cube[i];
//        printVec4(cube[i]);
        cube[i] = shear_matrix * cube[i];
//        printVec4(cube[i]);
    
        cube[i].z = 1;
        cube[i].w = 1;
    }
    
    glm::vec4 plane(0,1,0,0);
    glm::vec4 lightPos( 1.0, 5, 1.0, 1);
    GLfloat dot = glm::dot(plane, lightPos);
    glm::vec4 something = plane * lightPos;
    glm::mat4x4 shadowMatrix(dot);
    glm::mat4x4 pressed_down = { lightPos.x * plane, lightPos.y * plane, lightPos.z * plane, lightPos.z * plane };
    shadowMatrix = shadowMatrix - pressed_down;
    
    
    Camera camera(cameraPosition, cameraLookAt, camerUp);
    glm::vec3 slide;
    GLfloat white[4] = {0.5,1,0,0.5};
    if(!init_sucess) printf( "Failed to initialize!\n" );
    else{
        //Main loop flag
        bool quit = false;
        SDL_Event event;
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(&frustrum[0][0]);
        
        while( !quit ){
            while ( SDL_PollEvent( &event )){
                if( event.type == SDL_QUIT ){
                    quit = true;
                    break;
                }
                /*else if(event.type == SDL_MOUSEMOTION){
                    int xrel =  event.motion.xrel;
                    int yrel = event.motion.yrel;
                    
                    //only apply them
                    if(std::abs(xrel) > 5) {
                        
                    }
                    if(std::abs(yrel)>5){
                        
                    }
                    
                    
                    //I think we need something in camera to translate that down into angel vectors and
                    //apply to x and y
                    //or z and y?
                    //because our lookAt is changes.right?
                    //yay
                }*/
                else if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP){
                    switch (event.key.keysym.sym) {
                            
                        case SDLK_ESCAPE: quit = true; break;
                        case SDLK_a:  camera.slide(glm::vec3(-0.5,0,0)); break;
                        case SDLK_d:  camera.slide(glm::vec3(+0.5,0,0)); break;
                        case SDLK_w:  camera.slide(glm::vec3(0,0,-0.5)); break;
                        case SDLK_s:  camera.slide(glm::vec3(0,0,+0.5)); break;
                        case SDLK_UP: camera.slide(glm::vec3(0,0.5,0)); break;
                        case SDLK_DOWN: camera.slide(glm::vec3(0,-0.5,0)); break;
                        case SDLK_r:  camera.roll(10.f); break;
                        case SDLK_y:  camera.yaw(10.f); break; //yaw is right
                        case SDLK_p:  camera.pitch(10.f); break; //something wrong....
                        default: break;
                    }
                   
                }
            }
         
            glMatrixMode(GL_MODELVIEW);
            glLoadMatrixf(camera.get());
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
             glBegin(GL_LINES);{
                glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(1, 0, 0);//red - x
                glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 1, 0);//green - y
                glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 1);//blue - z
            }glEnd();
           
            //draw shadow first
            glPushMatrix();{
                glMultMatrixf(&shadowMatrix[0][0]);
                glDisable(GL_LIGHT0);
                glColor3f(1, 0, 0);
                glTranslatef(-2, -2, 0);
                glutSolidCube(1);
                glEnable(GL_LIGHT0);
            }glPopMatrix();
            
            glPushMatrix();{
                //Plane
//                glColor3f(1, 1, 1);
               
                glTranslatef(-2, -2, 0);
                glBegin(GL_QUADS);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
                glNormal3f(0, 1, 0);
                glVertex3f(0, 0, 0);
                glVertex3f(5, 0, 0);
                glVertex3f(5, 0, 5);
                glVertex3f(0, 0, 5);
                glEnd();
            }glPopMatrix();
            
//            glColor3f(1, 0, 0);
//            glColorMaterial(<#GLenum face#>, <#GLenum mode#>)
//            glColorMaterialfv(GL_FRONT, GL_DIFFUSE, white);

            glutSolidCube(1);
            
            
            
            //Dont misleaded by out put graphic...its aparent in much more distant negative depth
           /* glPushMatrix();{
                glColor3f(0, 0, 1);
                glBegin(GL_LINE_LOOP);
                for (unsigned int i = 0; i < 4; i++) glVertex4fv(&cube[i][0]);
                glEnd();
                
                glBegin(GL_LINE_LOOP);
                for (unsigned int i = 0; i < 4; i++) glVertex4fv(&cube[i+4][0]);
                glEnd();
                
                glBegin(GL_LINES);
                for (unsigned int i = 0; i < 4; i++) {
                    glVertex4fv(&cube[i][0]); glVertex4fv(&cube[i+4][0]);
                }
                glEnd();
            }glPopMatrix();*/
            
            SDL_GL_SwapWindow( gWindow );
        }
    }
    
    //Close and free
    
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    
    //Quit SDL subsystems
    SDL_Quit();
    return 0;
}

/*else if(event.type == SDL_MOUSEBUTTONDOWN){ //Doing selection now
 auto view = cameraLookAt - cameraPosition; view = glm::normalize(view);
 auto h = glm::cross(view, camerUp);  h = glm::normalize(h);
 auto v = glm::cross(h, view); v = glm::normalize(v);
 
 float fov = 90.f;
 auto rad = fov * 3.14159f / 180.f;
 float nearClippingPlaneDistance = 0.f;//no idea what is this
 auto vlength = glm::tan(rad/2.f) * nearClippingPlaneDistance; //right?
 auto hlength = vlength * (SCREEN_WIDTH / SCREEN_HEIGHT);
 

 float x = event.button.x;
 float y = event.button.y;
 x -= SCREEN_WIDTH/2;
 y -= SCREEN_HEIGHT/2;
 
 y /= (SCREEN_HEIGHT/2);
 x /= (SCREEN_WIDTH/2);
 
 auto pos = cameraPosition + view * nearClippingPlaneDistance + h*x + v*y;
 auto dir = pos - cameraPosition;
 
 }*/

/*
 glLoadIdentity();
 gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z, cameraLookAt.x, cameraLookAt.y, cameraLookAt.z, 0.0, 1.0, 0.0);
 
   glClear(GL_COLOR_BUFFER_BIT);
 glPushMatrix();
 
 glTranslatef(-1.5f, -1.5f, -1.5f);
 //Rubic Cube
 {
 float space = 1.f;
 for (unsigned int x = 0; x < 3; x++) {
 for (unsigned int y = 0; y<3; y++) {
 for (unsigned int z = 0; z < 3; z++) {
 glPushMatrix();
 glColor3f(x/3.f, y/3.f, z/3.f);
 glTranslated(x*1.5+space, y*1.5+space, z*1.5+space);
 glutSolidCube(1.f);
 glColor3b(0, 0, 0);
 glutWireCube(1.f);
 glPopMatrix();
 }
 }
 }
 }
 glPopMatrix();*/

/*else{
 
 //Now we implement simple key event that help us to rotate from center
 switch (event.key.keysym.sym) {
 case SDLK_LSHIFT:
 case SDLK_RSHIFT:
 shift = (event.type == SDL_KEYDOWN);
 break;
 case SDLK_LALT:
 case SDLK_RALT:
 alt = (event.type == SDL_KEYDOWN);
 break;
 case SDLK_ESCAPE:
 quit = true;
 break;
 default: break;
 }
 
 //This is navigation mode...hmm
 if (event.type == SDL_MOUSEMOTION && shift) {
 if (event.motion.xrel < 0) {
 cameraPosition.x -= 0.1;
 }else if (event.motion.xrel > 0){
 cameraPosition.x += 0.1;
 }
 
 if (event.motion.yrel < 0){
 cameraPosition.y -= 0.1;
 }else if (event.motion.yrel > 0){
 cameraPosition.y += 0.1;
 }
 }else if(event.type == SDL_MOUSEMOTION && alt){
 if (event.motion.yrel < 0) {
 cameraPosition.z -= 0.1;
 }else if (event.motion.yrel > 0){
 cameraPosition.z += 0.1;
 }
 }
 
 }*/

/*
 //float left, right, bottom, up, near, far;
 float left = -3.f;
 float right = 3.f;
 float bottom = -3.f;
 float top = 3.f;
 float near =1.f;
 float far = 105.f;
 
 float A = (right + left)/(right-left);
 float B = (top+bottom)/(top-bottom);
 float C = -(far+near)/(far-near);
 float D = -2.0f*far*near/(far-near);
 float F = 2.0f*near/(top - bottom);
 float E = 2.0f*near/(right - left);*/

//                glBegin(GL_LINE_LOOP);
//                for (unsigned int i = 0; i < 4; i++) glVertex4fv(&cube[i][0]);
//                glEnd();
//
//                glBegin(GL_LINE_LOOP);
//                for (unsigned int i = 0; i < 4; i++) glVertex4fv(&cube[i+4][0]);
//                glEnd();
//
//                glBegin(GL_LINES);
//                for (unsigned int i = 0; i < 4; i++) {
//                    glVertex4fv(&cube[i][0]); glVertex4fv(&cube[i+4][0]);
//                }
//                glEnd();
