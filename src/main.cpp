#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
// Use of degrees is deprecated. Use radians for GLM functions
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "Screenshot.h"
#include "RTScene.h"
#include "Image.h"
#include "Scene.h"

#include "Ray.h"
#include "RayTracer.h"

static bool rt_mode = false;
//static const int width = 144;
//static const int height = 108;
static const int width = 200;
static const int height = 150;
static const char* title = "Scene viewer";
static const glm::vec4 background(0.1f, 0.2f, 0.3f, 1.0f);
static Scene scene;
static RTScene rtscene;
static Image image(width, height);

#include "hw3AutoScreenshots.h"

void printHelp(){
    std::cout << R"(
    Available commands:
      press 'H' to print this message again.
      press Esc to quit.
      press 'O' to save a screenshot.
      press the arrow keys to rotate camera.
      press 'A'/'Z' to zoom.
      press 'R' to reset camera.
      press 'L' to turn on/off the lighting.
    
      press Spacebar to generate images for hw3 submission.
    
)";
}

void initialize(void){
    printHelp();
    glClearColor(background[0], background[1], background[2], background[3]); // background color
    glViewport(0,0,width,height);
    
    // Initialize scene
    scene.init();

    rtscene.init();
    rtscene.buildTriangleSoup();

    image.init();

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    std::cout << R"(Drawing...)";

    if (rt_mode) {
        RayTracer::Raytrace(*rtscene.camera, rtscene, image);
        image.draw();
    }
    else {
        scene.draw();
    }

    std::cout << R"(Done)" << std::endl;

    glutSwapBuffers();
    glFlush();
    
}

void saveScreenShot(const char* filename = "test.png"){
    int currentwidth = glutGet(GLUT_WINDOW_WIDTH);
    int currentheight = glutGet(GLUT_WINDOW_HEIGHT);
    Screenshot imag = Screenshot(currentwidth,currentheight);
    imag.save(filename);
}

void keyboard(unsigned char key, int x, int y){
    switch(key){
        case 'd' :
            std::cerr << rtscene.triangle_soup.size();
            break;
        case 'i':
            rt_mode = !rt_mode;
            std::cout << R"(Toggled the raytracer)" << std::endl;
            //glutPostRedisplay();
            break;
        case 27: // Escape to quit
            exit(0);
            break;
        case 'h': // print help
            printHelp();
            break;
        case 'o': // save screenshot
            std::cout << R"(Photo)" << std::endl;
            saveScreenShot();
            break;
        case 'O': // save screenshots
            std::cout << R"(Photos)" << std::endl;
            hw3AutoScreenshots();
            break;
        case 'r':
            rtscene.camera->aspect_default = float(glutGet(GLUT_WINDOW_WIDTH)) / float(glutGet(GLUT_WINDOW_HEIGHT));
            rtscene.camera->reset();
            scene.camera -> aspect_default = float(glutGet(GLUT_WINDOW_WIDTH))/float(glutGet(GLUT_WINDOW_HEIGHT));
            scene.camera -> reset();
            std::cout << R"(Reset)" << std::endl;
            //glutPostRedisplay();
            break;
        case 'a':
            rtscene.camera->zoom(0.9f);
            scene.camera->zoom(0.9f);
            std::cout << R"(Zoom in)" << std::endl;
            //glutPostRedisplay();
            break;
        case 'z':
            rtscene.camera->zoom(1.1f);
            scene.camera->zoom(1.1f);
            std::cout << R"(Zoom out)" << std::endl;
            //glutPostRedisplay();
            break;
        case 'l':
            //scene.shader -> enablelighting = !(scene.shader -> enablelighting);
            //glutPostRedisplay();
            break;
        case ' ': 
            //hw3AutoScreenshots();
            std::cout << R"(Calling draw())" << std::endl;
            glutPostRedisplay();
            break;
        default:
            //glutPostRedisplay();
            break;
    }
}
void specialKey(int key, int x, int y){
    switch (key) {
        case GLUT_KEY_UP: // up
            scene.camera -> rotateUp(-10.0f);
            rtscene.camera->rotateUp(-10.0f);
            std::cout << R"(Up)" << std::endl;
            //glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN: // down
            scene.camera -> rotateUp(10.0f);
            rtscene.camera->rotateUp(10.0f);
            std::cout << R"(Down)" << std::endl;
            //glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT: // right
            scene.camera -> rotateRight(-10.0f);
            rtscene.camera->rotateRight(-10.0f);
            std::cout << R"(Right)" << std::endl;
            //glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT: // left
            scene.camera -> rotateRight(10.0f);
            rtscene.camera->rotateRight(10.0f);
            std::cout << R"(Left)" << std::endl;
            //glutPostRedisplay();
            break;
    }
}

int main(int argc, char** argv)
{
    // BEGIN CREATE WINDOW
    glutInit(&argc, argv);
    
    glutInitContextVersion(3,1);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize(width, height);
    glutCreateWindow(title);
    glewExperimental = GL_TRUE;
    GLenum err = glewInit() ;
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    // END CREATE WINDOW
    
    initialize();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKey);
    
    glutMainLoop();
	return 0;   /* ANSI C requires main to return int. */
}
