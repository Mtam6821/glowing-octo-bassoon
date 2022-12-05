/**************************************************
Scene.inl contains the definition of the scene graph
*****************************************************/
#include "RTScene.h"
#include "RTCube.h"
#include "RTObj.h"

using namespace glm;
void RTScene::init(void){
    // Create a geometry palette
    geometry["cube"] = new RTCube;
    geometry["teapot"] = new RTObj;
    geometry["bunny"] = new RTObj;
    geometry["cube"] -> init();
    geometry["teapot"] -> init("models/teapot.obj");
    geometry["bunny"] -> init("models/bunny.obj");
    
    // Create a material palette
    material["wood"] = new Material;
    material["wood"] -> ambient = vec4(0.1f,0.1f,0.1f,1.0f);
    material["wood"] -> diffuse = vec4(0.3f,0.15f,0.1f,1.0f);
    material["wood"] -> specular = vec4(0.3f,0.15f,0.1f,1.0f);
    material["wood"] -> shininess = 100.0f;
    
    material["bulb"] = new Material;
    material["bulb"] -> ambient = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    material["bulb"] -> diffuse = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    material["bulb"] -> specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    material["bulb"] -> emision = vec4(0.3f, 0.4f, 1.0f,1.0f);
    material["bulb"] -> shininess = 200.0f;

    //Source: http://learnwebgl.brown37.net/10_surface_properties/surface_properties_color.html

    material["pbronze"] = new Material;
    material["pbronze"]->ambient = vec4(0.25f, 0.148f, 0.06475f, 1.0f);
    material["pbronze"]->diffuse = vec4(0.4f, 0.2368f, 0.1036f, 1.0f);
    material["pbronze"]->specular = vec4(0.774597f, 0.458561f, 0.200621f, 1.0f);
    material["pbronze"]->shininess = 76.8f;

    material["psilver"] = new Material;
    material["psilver"]->ambient = vec4(0.23125f, 0.23125f, 0.23125f, 1.0f);
    material["psilver"]->diffuse = vec4(0.2775f, 0.2775f, 0.2775f, 1.0f);
    material["psilver"]->specular = vec4(0.773911f, 0.773911f, 0.773911f, 1.0f);
    material["psilver"]->shininess = 89.6;

    material["pgold"] = new Material;
    material["pgold"]->ambient = vec4(0.24725f, 0.2245f, 0.0645f, 1.0f);
    material["pgold"]->diffuse = vec4(0.34615f, 0.3143f, 0.0903f, 1.0f);
    material["pgold"]->specular = vec4(0.797357f, 0.723991f, 0.208006f, 1.0f);
    material["pgold"]->shininess = 83.2;
    
    // Create a model palette
    model["wood cube"] = new RTModel;
    model["wood cube"] -> geometry = geometry["cube"];
    model["wood cube"] -> material = material["wood"];

    model["bunny"] = new RTModel;
    model["bunny"] -> geometry = geometry["bunny"];
    model["bunny"] -> material = material["psilver"];

    model["bulb"] = new RTModel;
    model["bulb"] -> geometry = geometry["cube"];
    model["bulb"] -> material = material["bulb"];

    model["teapot1"] = new RTModel;
    model["teapot1"]->geometry = geometry["teapot"];
    model["teapot1"]->material = material["pbronze"];

    model["teapot2"] = new RTModel;
    model["teapot2"]->geometry = geometry["teapot"];
    model["teapot2"]->material = material["pgold"];
    
    // Create a light palette
    light["sun"] = new Light;
    light["sun"] -> position = vec4(3.0f,2.0f,1.0f,0.0f);
    light["sun"] -> color = 1.0f*vec4(1.0f,1.0f,1.0f,1.0f);
    
    light["bulb"] = new Light;
    light["bulb"] -> position = vec4(-2.4f, 3.0f, 1.6f,0.0f);
    light["bulb"] -> color = vec4(0.3f,0.4f,1.0f,1.0f);
    
    // Build the scene graph
    node["teapot2"]  = new RTNode;
    node["bulb"]     = new RTNode;
    node["teapot1"]  = new RTNode;
    node["scaffold"] = new RTNode;
    node["surface"]  = new RTNode;
    node["bunny"]    = new RTNode;

    node["scaffold"]->childnodes.push_back(node["surface"]);        //third place
    node["scaffold"]->childtransforms.push_back(translate(vec3(1.4f, 0.0f, 0.0f)) * scale(vec3(1.0f, 1.0f, 1.0f)));
    node["scaffold"]->childnodes.push_back(node["surface"]);        //second place
    node["scaffold"]->childtransforms.push_back(translate(vec3(-1.4f, 0.4f, 0.0f)) * scale(vec3(1.0f, 2.0f, 1.0f)));
    node["scaffold"]->childnodes.push_back(node["surface"]);        //first place
    node["scaffold"]->childtransforms.push_back(translate(vec3(0.0f, 0.8f, 0.0f)) * scale(vec3(1.0f, 3.0f, 1.0f)));
    node["scaffold"]->childnodes.push_back(node["bunny"]);          //second-place bunny
    node["scaffold"]->childtransforms.push_back(translate(vec3(-1.4f, 0.8f, 0.0f)) * rotate(90.0f * float(M_PI) / 180.0f, vec3(0.0f, 1.0f, 0.0f)));
    node["scaffold"]->childnodes.push_back(node["teapot1"]);        //third place teapot
    node["scaffold"]->childtransforms.push_back(translate(vec3(1.4f, 0.2f, 0.0f)) * rotate(-120.0f * float(M_PI) / 180.0f, vec3(0.0f, 1.0f, 0.0f)));
    node["scaffold"]->childnodes.push_back(node["teapot2"]);        //first place teapot
    node["scaffold"]->childtransforms.push_back(translate(vec3(0.0f, 1.6f, 0.0f)) * rotate(-45.0f * float(M_PI) / 180.0f, vec3(0.0f, 0.0f, 1.0f)));

    node["surface"] -> models.push_back(model["wood cube"]);
    node["surface"] -> modeltransforms.push_back(translate(vec3(0.0f, -0.1f, 0.0f)) * scale(vec3(1.4f, 0.6f, 1.8f)));

    node["bunny"]->models.push_back(model["bunny"]);
    node["bunny"]->modeltransforms.push_back(scale(vec3(0.8f)) * translate(vec3(0.0f, 1.0f, 0.0f)));

    node["teapot1"]->models.push_back(model["teapot1"]);
    node["teapot1"]->modeltransforms.push_back(translate(vec3(-0.1f, 0.0f, 0.1f)) * scale(vec3(0.9f)));
    
    node["teapot2"]->models.push_back(model["teapot2"]);
    node["teapot2"]->modeltransforms.push_back(translate(vec3(-0.3f, -0.1f, -0.1f)) * scale(vec3(0.8f, 1.8f, 0.8f))* scale(vec3(0.9f)));

    node["world"] -> childnodes.push_back( node["scaffold"] );
    node["world"] -> childtransforms.push_back( mat4(1.0f) );
    node["world"] -> models.push_back(model["bulb"]);
    node["world"] -> modeltransforms.push_back(translate(vec3(-2.4f, 3.0f, 1.6f)) * scale(vec3(0.1f)));

    // Put a camera
    camera = new Camera;
    camera -> target_default = vec3( 0.0f, 1.0f, 0.0f );
    camera -> eye_default = vec3( 0.0f, 1.0f, 5.0f );
    camera -> up_default = vec3( 0.0f, 1.0f, 0.0f );
    camera -> reset();
    
    // Initialize shader
    shader = new SurfaceShader;
    shader -> read_source( "shaders/projective.vert", "shaders/lighting.frag" );
    shader -> compile();
    glUseProgram(shader -> program);
    shader -> initUniforms();
}
