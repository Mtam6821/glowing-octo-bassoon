/**************************************************
Obj is subclass class of Geometry
that loads an obj file.
*****************************************************/
#define GLM_FORCE_RADIANS

#ifndef __RTOBJ_H__
#define __RTOBJ_H__

#include <glm/glm.hpp>
#include "RTGeometry.h"

class RTObj : public RTGeometry {
public:

    void init(const char * filename);
        
};

#endif 
