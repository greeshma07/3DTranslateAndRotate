#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream> 
#include <fstream>
#include <vector>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

using namespace std;

void loadObjFile(char *fileName);

#include "variables.h"
#include "callbackFunctions.h"

int main(int argc,  char * argv[]) {
    
    /*Initialize glut stuff*/
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_RGB|GLUT_SINGLE);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("Bunny");
    
    //Load the "bunny_low.obj" or "bunny_high.obj" file
    loadObjFile("bunny_low.obj");

	//calculating the object center for rotating 3D object using object center
	for (int i = 0; i < vt_list.size(); i++){
		objC_X = objC_X + vt_list[i].x;
	}
	for (int i = 0; i < vt_list.size(); i++){
		objC_Y = objC_Y + vt_list[i].y;
	}
	objC_X=objC_X/vt_list.size();
	objC_Y=objC_Y/vt_list.size();
	  
    /*Initialize gl stuff*/
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.4, 0.4, -0.4 * .48 / .64, 0.4 * .48 / .64, 2, 10); //parallel projection
    gluLookAt( 0, 0, 5, 0, 0.2, 0, 0, 1, 0); 
    
    /*Register GL stuff with the GLUT*/
    glutDisplayFunc(onDisplay);
    glutMouseFunc(onMouse);
    glutMotionFunc(onMouseMotion);
    glutKeyboardFunc(onKeyboard);    
    
    /*Initialize the loop*/
    glutMainLoop();    
    return 0;
}

//load 3D data and store it into vertex and triangle data structures
void loadObjFile(char *fileName){
	ifstream file;
    file.open(fileName);
	if(file.fail()){
		cout<<"Failed to load object file "<<fileName<<endl;
		exit(1);
	}	
	string line,v,f;
	float x,y,z;
	int v1,v2,v3;
    while(!file.eof()){
        if(getline(file,line)){ 
			istringstream iss(line);
			if(line[0] == 'v'){
				iss >> v >> x >> y >> z;
				vertex.x=x;
				vertex.y=y;
				vertex.z=z;
				vt_list.push_back(vertex);
			}
			if(line[0] == 'f'){
				iss >> f >> v1 >> v2 >> v3;
				triangle.v1=v1-1;
				triangle.v2=v2-1;
				triangle.v3=v3-1;
				tg_list.push_back(triangle);
			}
		}
	}
}

