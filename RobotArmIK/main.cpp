#include <vgl.h>
#include <InitShader.h>
#include <mat.h>
#include "MyCube.h"
#include "MyPyramid.h"
#include "MyTarget.h"
#include <list>

using namespace std;


#define PI 3.141592

MyCube cube;
MyPyramid pyramid;
MyTarget target(&cube);

GLuint program;
GLuint uMat;

mat4 CTM;



bool bPlay = true;
bool bChasingTarget= true;
bool bDrawTarget = true;

float ang1 = 0;
float ang2 = 0;
float ang3 = 0;



void diesplayManual() {
	printf("\n\n\n");

	printf("==============GPU Base Raytracer without RT-core---=============\n");
	printf("\t1. Space: Pause/Play\n");
	printf("\t2. 1: Toggle IK\n");
	printf("\t3. 2: Toggle Draw target\n");
	printf("\t4. 3: Toggle Random Target Position\n");
	printf("================================================================\n");

}

void myInit()
{
	cube.Init();
	pyramid.Init();	
	target.toggleRandom();
	diesplayManual();

	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

}

float eTime = 0;


void drawRobotArm(float ang1, float ang2, float ang3)
{
	mat4 CTM(1.0);
	
	list<mat4> jointAngStk;


	// BASE
	mat4 M(1.0);
	CTM = Translate(0, -0.4, 0) * RotateY(5*eTime);
	//CTM = Translate(0, -0.4, 0) * RotateY(90);
	jointAngStk.push_back(CTM);

	
	{

		// Panel
		CTM = jointAngStk.back();
		CTM *= Translate(0, 0, 0.15);
		M = Scale(0.3, 0.3, 0.2);
		glUniformMatrix4fv(uMat, 1, true, CTM * M);
		pyramid.Draw(program);

		{
			CTM = jointAngStk.back();
			CTM *= Translate(0,0,-0.010) * RotateZ(ang1);
			jointAngStk.push_back(CTM);

			//Upper Arm Joint
			M = Translate(0, 0.2, 0) * Scale(0.15, 0.55, 0.1);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			cube.Draw(program);

			//Upper Arm
			CTM = jointAngStk.back();
			M = Scale(0.2, 0.2, 0.12);
			glUniformMatrix4fv(uMat, 1, true, CTM * M);
			cube.Draw(program);

			{
				CTM *= Translate(0, 0.4, 0) * RotateZ(ang2);
				jointAngStk.push_back(CTM);
				
				//Lower Arm Joint
				M = Scale(0.05, 0.05, 0.25);
				glUniformMatrix4fv(uMat, 1, true, CTM * M);
				cube.Draw(program);

				//Lower Arm1
				M = Translate(0, 0.2, 0.075) * Scale(0.1, 0.5, 0.05);
				glUniformMatrix4fv(uMat, 1, true, CTM * M);
				cube.Draw(program);

				//Lower Arm2
				M = Translate(0, 0.2, -0.075) * Scale(0.1, 0.5, 0.05);
				glUniformMatrix4fv(uMat, 1, true, CTM * M);
				cube.Draw(program);

				{
					CTM = jointAngStk.back();
					CTM *= Translate(0, 0.4, 0) * RotateZ(ang3);
					jointAngStk.push_back(CTM);


					//Wrist Joint
					M = Scale(0.05, 0.05, 0.25);
					glUniformMatrix4fv(uMat, 1, true, CTM * M);
					cube.Draw(program);

					//Wrist Pin
					M = Translate(0,0.05,0.0) * Scale(0.07, 0.15, 0.05);
					glUniformMatrix4fv(uMat, 1, true, CTM * M);
					cube.Draw(program);

					//Hand
					M = Translate(0.05, 0.1, 0) * RotateZ(-90) * Scale(0.15, 0.2f, 0.1);
					glUniformMatrix4fv(uMat, 1, true, CTM * M);
					pyramid.Draw(program);

				}
			}
		}
	}


}


float jr = 25;

void computeAngle()
{

	float ang1ToRad = ang1 * (PI / 180);
	float ang2ToRad = ang2 * (PI / 180);
	float ang3ToRad = ang3 * (PI / 180); // Angle값들을 Radian 값으로 변환

	float x = -0.4 * sin(ang1ToRad) -
		0.4 * sin(ang2ToRad + ang1ToRad) +
		0.15 * cos(ang3ToRad + ang2ToRad + ang1ToRad) - 0.1 * sin(ang3ToRad + ang2ToRad + ang1ToRad);

	float y = 0.4 * cos(ang1ToRad) +
		0.4 * cos(ang2ToRad + ang1ToRad) +
		0.15 * sin(ang3ToRad + ang2ToRad + ang1ToRad) + 0.1 * cos(ang3ToRad + ang2ToRad + ang1ToRad);
	// 위의 최적화 공식의 theta값들은 모두 World좌표계 각도이기 때문에 
	// Local 좌표계인 ang1, ang2, ang3 값들을 World 좌표계 값으로 계산해줌

	float xDist = target.GetPosition(eTime).x - x;
	float yDist = target.GetPosition(eTime).y - y;
	// Output = (tarPos.x -x)^2 + (tarPos.y -y)^2 을 xDist, yDist로 표현

	float dxdang1 = -(2 * xDist * -0.4 * cos(ang1ToRad));
	float dxdang2 = -(2 * xDist * -0.4 * cos(ang2ToRad + ang1ToRad));
	float dxdang3 = -(2 * xDist *
		(-0.15 * sin(ang3ToRad + ang2ToRad + ang1ToRad) - 0.1 * cos(ang3ToRad + ang2ToRad + ang1ToRad)));


	float dydang1 = -(2 * yDist * -0.4 * sin(ang1ToRad));
	float dydang2 = -(2 * yDist * -0.4 * sin(ang2ToRad + ang1ToRad));
	float dydang3 = -(2 * yDist *
		(0.15 * cos(ang3ToRad + ang2ToRad + ang1ToRad) - 0.1 * sin(ang3ToRad + ang2ToRad + ang1ToRad)));
	// Output = xDist^2 + yDist^2 공식을 ang1, ang2, ang3에 대해 편미분한 값을 각각 구해줌


	ang1 -= jr * dxdang1;
	ang2 -= jr * dxdang2;
	ang3 -= jr * dxdang3;

	ang1 -= jr * dydang1;
	ang2 -= jr * dydang2;
	ang3 -= jr * dydang3;
	// jr = Jump Rate 만큼씩 관절의 각도를 변화
}


void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	GLuint uColor = glGetUniformLocation(program, "uColor");
	glUniform4f(uColor, -1,-1,-1,-1);

	
	uMat = glGetUniformLocation(program, "uMat");
	CTM = Translate(0, -0.4, 0) * RotateY(eTime*5);
	drawRobotArm(ang1, ang2, ang3);	
	
	glUniform4f(uColor, 1,0,0,1);
	if(bDrawTarget == true)
		target.Draw(program, CTM, eTime);
	
	glutSwapBuffers();
}

void myIdle()
{
	if(bPlay)
	{
		eTime += 1/60.0f;
		Sleep(1/60.0f*1000);

		if(bChasingTarget == false)
		{
			ang1 = 45 * sin(eTime* PI);
			ang2 = 60 * sin(eTime*2* PI);
			ang3 = 30 * sin(eTime* PI);

		}
		else
			computeAngle();

		glutPostRedisplay();
	}
}

void myKeyboard(unsigned char c, int x, int y)
{

	switch(c)
	{
	case '1':
		bChasingTarget = !bChasingTarget;
		break;
	case '2':
		bDrawTarget = !bDrawTarget;
		break;
	case '3':
		target.toggleRandom();
		break;
	case ' ':
		bPlay = !bPlay;
		break;
	default:
		break;
	}
}


int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutCreateWindow("Simple Robot Arm");

	glewExperimental = true;
	glewInit();

	myInit();
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutIdleFunc(myIdle);

	glutMainLoop();

	return 0;
}