#ifndef IM_project_3_callbackFunctions_h
#define IM_project_3_callbackFunctions_h
#endif

void onKeyboard(unsigned char key, int x, int y){
	if(key =='M')
		key ='m';
    switch (key)
    {
        case 27:	exit(1);
					break;
        case 'm':	if(meshView)
						meshView=false;
					else
						meshView=true;
					glutPostRedisplay();
					break;
        default:	break;
    }
}

void onMouse(int button, int state, int x, int y){    
    GLint specialKey = glutGetModifiers();
    switch (button){
        case GLUT_LEFT_BUTTON:	if (state == GLUT_DOWN) {
									mouseDrag = 1;
									curX = x;
									curY = y;
									if (specialKey == GLUT_ACTIVE_CTRL){ //rotation using object center
										rotateByObjC=true;
										translate=false;
									}
									else if (specialKey == GLUT_ACTIVE_SHIFT){ ///translation
										translate=true; 
										rotateByObjC=false;
									}
									else{ //rotation using world center
										translate=false;
										rotateByObjC=false;
									}													
								}
								else if (state == GLUT_UP){
									mouseDrag=0;
								}
								break;
            
        case GLUT_RIGHT_BUTTON:	if (state == GLUT_DOWN)                
									break;            
        default:	break;            
    }    
    glutPostRedisplay();    
}

void onMouseMotion(int x, int y){
	if (mouseDrag == 1){
        if (translate){
			translate_x += static_cast<float>(x - curX) / 1000.f;
            translate_y += static_cast<float>(curY - y) / 1000.f;			
        }
        else{
			theta_y += static_cast<float>(x - curX) / 100.f;
            theta_x += static_cast<float>(y - curY) / 100.f;
        }        
        curX = x;
        curY = y;
    }
    glutPostRedisplay(); 
}

void renderAllTriangles(){	
	for(int i = 0; i < tg_list.size(); i++){
		if(meshView)
			glBegin(GL_LINE_LOOP);
		else
			glBegin(GL_TRIANGLES);
		int pt0 = tg_list[i].v1;
        int pt1 = tg_list[i].v2;
        int pt2 = tg_list[i].v3;
		glVertex3f(vt_list[pt0].x, vt_list[pt0].y, vt_list[pt0].z);
		glVertex3f(vt_list[pt1].x, vt_list[pt1].y, vt_list[pt1].z);
		glVertex3f(vt_list[pt2].x, vt_list[pt2].y, vt_list[pt2].z);
		glEnd();
	}
}

// 3D Rotation 
void rotate3DObject(){
	Mat Rx, Ry;
	Mat T,T1,P;

	//Initialize the translation and rotation matrix
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(i == j){
				Rx.elem[i][j] = 1.0f;
				Ry.elem[i][j] = 1.0f;
				if(rotateByObjC){
					T.elem[i][j] = 1.0f;
					T1.elem[i][j] = 1.0f;
				}
			}
            else{
				Rx.elem[i][j] = 0.0f;
				Ry.elem[i][j] = 0.0f;
				if(rotateByObjC){
					T.elem[i][j] = 0.0f;
					T1.elem[i][j] = 0.0f;
				}
			}
        }
	}
	
	//rotation matrix about x axis
	Rx.elem[1][1] = cos(theta_x);
	Rx.elem[1][2] = -sin(theta_x);
	Rx.elem[2][1] = sin(theta_x);
	Rx.elem[2][2] = cos(theta_x);
	
	//rotation matrix about y axis
	Ry.elem[0][0] = cos(theta_y);
	Ry.elem[0][2] = sin(theta_y);
	Ry.elem[2][0] = -sin(theta_y);
	Ry.elem[2][2] = cos(theta_y);

	//transaltion matrix for rotation by object center
	if(rotateByObjC){
		T.elem[0][3] = objC_X;
		T.elem[1][3] = objC_Y;
		T1.elem[0][3] = -objC_X; //for translating back to the origin
		T1.elem[1][3] = -objC_Y;
		P = T * Rx * Ry * T1;
	} 
	else
		P = Rx * Ry; //Final transformation matrix P

	//calculating new vertex by traversing all the vertex points in the vt_list
	for (int i = 0; i < vt_list.size(); i++){
		Vertex curV, newV;
		curV.data[0] = vt_list[i].x;
		curV.data[1] = vt_list[i].y;
		curV.data[2] = vt_list[i].z;
		curV.data[3] = 1.0f;
		newV = P * curV;
		vt_list[i].x = newV.data[0] / newV.data[3];
		vt_list[i].y = newV.data[1] / newV.data[3];
		vt_list[i].z = newV.data[2] / newV.data[3];		
	}	
	theta_x = 0;
	theta_y = 0;
}

//translating 3D object
void translate3DObject(){
	Mat T;
	//Initialize the translation matrix
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(i == j)
				T.elem[i][j] = 1.0f;
			else				
				T.elem[i][j] = 0.0f;
		}
	}
	T.elem[0][3] = translate_x;
	T.elem[1][3] = translate_y;

	Mat P = T; //Final transformation matrix P

	//calculating new vertex by traversing all the vertex points in the vt_list
	for(int i = 0; i < vt_list.size(); i++){
		Vertex curV, newV;
		curV.data[0] = vt_list[i].x;
		curV.data[1] = vt_list[i].y;
		curV.data[2] = vt_list[i].z;
		curV.data[3] = 1.0f;
		newV = P * curV;
		vt_list[i].x = newV.data[0] / newV.data[3];
		vt_list[i].y = newV.data[1] / newV.data[3];
		vt_list[i].z = newV.data[2] / newV.data[3];
	}	
	translate_x = 0;
	translate_y = 0;	
}

void onDisplay(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0); 
	if(mouseDrag == 1){
		if(translate)
			translate3DObject();
		else
			rotate3DObject();
	}
    renderAllTriangles();    
    glFlush(); //clear the memory
}