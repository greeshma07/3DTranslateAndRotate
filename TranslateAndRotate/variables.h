#ifndef IM_project_3_variables_h
#define IM_project_3_variables_h
#endif

#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif


struct Vertex{
	float x, y, z;
	float data[4];   
}vertex;

struct Triangle{
	int v1,v2,v3;
}triangle;

vector<Vertex> vt_list; 
vector<Triangle> tg_list;

bool meshView=false; //determines which view(triangle or mesh)to be displayed

struct Mat{
	float elem[4][4];

	/*Mat times another Mat function*/
	const Mat operator *(const Mat& right) const
	{
		Mat result;
		for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                result.elem[i][j] = 0;
                for(int k = 0; k < 4; k++){
					result.elem[i][j] += elem[i][k] * right.elem[k][j];
                }
            }
		}
		return result;
	}

	/*Mat times a vertex and return another vertex*/
	const Vertex operator *(const Vertex& v) const
	{
		Vertex result;
		for(int i = 0; i < 4; i++){            
            result.data[i] = 0;
            for(int k = 0; k < 4; k++){
                result.data[i] += elem[i][k] * v.data[k];
            }            
        }
		return result;
	}
	
};

float theta_x = 0.0f;
float theta_y = 0.0f;
float translate_x = 0.0f;
float translate_y = 0.0f;
float objC_X=0.0f;
float objC_Y= 0.0f;
int mouseDrag = 0; 
bool translate = false; //true for translation
bool rotateByObjC=false; //true for rotation by object center
int curX = 0, curY = 0;
