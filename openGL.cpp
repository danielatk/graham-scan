#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>

using namespace std;

GLint window_width  = 800;
GLint window_height = 600;
vector<double> X;
vector<double> Y;

//string path = "";
ofstream points;

/*
vector<double> getVertice(string const& line)
{
	istringstream iss(line);

	return vector<double>{istream_iterator<double>(iss), istream_iterator<double>()};
}
*/

void loop(char *path)
{
	ifstream loop(path);
	string line;
	vector<double> vertice;
	double x, y;
	
	glBegin(GL_LINE_LOOP);
	while(loop >> x)
	{
		loop >> y;
		glVertex2f(x , y);
		//vertice = getVertice(line);
		//glVertex2f(vertice[0] , vertice[1]);
	}
	glEnd();
	loop.close();
	glutSwapBuffers();
}

void display (void)
{
	double x, y;
	
	glPointSize(5);
	glColor3f(1.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBegin(GL_POINTS);
	for (int i = 0; i < X.size(); i++)
	{
		x = X.at(i);
		y = Y.at(i);
		glVertex2f(x , y);
	}
	glEnd();
	
	glutSwapBuffers();
	//glFlush();
}

void mouse(int bin, int state , int x , int y)
{
	if(bin == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//cout << "x:" << x << " y:" << y << "\n";
		y = window_height - y;
		
		X.push_back(x);
		Y.push_back(y);
		
		points.open ("points.txt", ios::app);
		points << x << " " << y << "\n";
		points.close();
		
		glPointSize(5);
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
			glVertex2f(x , y);
		glEnd();
		glutSwapBuffers();
	}	
}

void reshape(GLint width, GLint height)
{
    window_width  = width  ;
    window_height = height ;
    glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    	case 13:			// ENTER KEY
    		loop("loop.txt");
    		break;
        case 27:			// ESCAPE key
            exit (0);
            break;
    }
}

int main (int argc,char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
	glutInitWindowSize(window_width,window_height);
	/*Set the posotion of window*/
	glutInitWindowPosition(0,0);
	glutCreateWindow("Triangulation");
	
	glClearColor (0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport( 0,0, window_width, window_height);
	glMatrixMode( GL_PROJECTION );
	glOrtho( 0.0, window_width, 0.0, window_height, 1.0, -1.0 );
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);

	/* initialize viewing values */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	
	points.open ("points.txt"); // Esvazia arquivo
	points.close();
	
	glutMainLoop();
}
