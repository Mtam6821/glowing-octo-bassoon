/**************************************************
Cube is subclass class of Geometry
that represents a 3D cube.
*****************************************************/
#ifndef __RTCUBE_H__
#define __RTCUBE_H__

#include "RTGeometry.h"

class RTCube : public RTGeometry {
public:

    void init(void){
        // vertex positions
        const GLfloat positions[24][3] ={
            // Front face
            { -0.5f, -0.5f, 0.5f },{ -0.5f, 0.5f, 0.5f },{ 0.5f, 0.5f, 0.5f },{ 0.5f, -0.5f, 0.5f },
            // Back face
            { -0.5f, -0.5f, -0.5f },{ -0.5f, 0.5f, -0.5f },{ 0.5f, 0.5f, -0.5f },{ 0.5f, -0.5f, -0.5f },
            // Left face
            { -0.5f, -0.5f, 0.5f },{ -0.5f, 0.5f, 0.5f },{ -0.5f, 0.5f, -0.5f },{ -0.5f, -0.5f, -0.5f },
            // Right face
            { 0.5f, -0.5f, 0.5f },{ 0.5f, 0.5f, 0.5f },{ 0.5f, 0.5f, -0.5f },{ 0.5f, -0.5f, -0.5f },
            // Top face
            { 0.5f, 0.5f, 0.5f },{ -0.5f, 0.5f, 0.5f },{ -0.5f, 0.5f, -0.5f },{ 0.5f, 0.5f, -0.5f },
            // Bottom face
            { 0.5f, -0.5f, 0.5f },{ -0.5f, -0.5f, 0.5f },{ -0.5f, -0.5f, -0.5f },{ 0.5f, -0.5f, -0.5f }
        };
        // vertex normals
        const GLfloat normals[24][3] = {
            // Front face
            { 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },
            // Back face
            { 0.0f, 0.0f, -1.0f },{ 0.0f, 0.0f, -1.0f },{ 0.0f, 0.0f, -1.0f },{ 0.0f, 0.0f, -1.0f },
            // Left face
            { -1.0f, 0.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },
            // Right face
            { 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },
            // Top face
            { 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },
            // Bottom face
            { 0.0f, -1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f }
        };
        // Cube indices
        const GLuint indices[36] = {
            0, 1, 2, 0, 2, 3, // Front face
            4, 5, 6, 4, 6, 7, // Back face
            8, 9, 10, 8, 10, 11, // Left face
            12, 13, 14, 12, 14, 15, // Right face
            16, 17, 18, 16, 18, 19, // Top face
            20, 21, 22, 20, 22, 23 // Bottom face
        };

        for (int ind = 0; ind < sizeof(indices) / sizeof(indices[0]); ind += 3) {
            Triangle t = Triangle();
            t.P.push_back(glm::vec3(
                positions[indices[ind]][0], positions[indices[ind]][1], positions[indices[ind]][2]
            ));
            t.P.push_back(glm::vec3(
                positions[indices[ind+1]][0], positions[indices[ind+1]][1], positions[indices[ind+1]][2]
            ));
            t.P.push_back(glm::vec3(
                positions[indices[ind+2]][0], positions[indices[ind+2]][1], positions[indices[ind+2]][2]
            ));
            t.N.push_back(glm::vec3(
                normals[indices[ind]][0], normals[indices[ind]][1], normals[indices[ind]][2]
            ));
            t.N.push_back(glm::vec3(
                normals[indices[ind + 1]][0], normals[indices[ind + 1]][1], normals[indices[ind + 1]][2]
            ));
            t.N.push_back(glm::vec3(
                normals[indices[ind + 2]][0], normals[indices[ind + 2]][1], normals[indices[ind + 2]][2]
            ));

            elements.push_back(t);
            count++;
        }
    }
};

#endif 
