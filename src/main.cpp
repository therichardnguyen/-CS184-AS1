#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>

//Include our math library
#include <algebra3.h>

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

using namespace std;

enum {NOTHING, REDC, GREENC, BLUEC, YELLOWC, MAGENTAC, CYANC, WHITEC};
const float PI = 3.1415;

int changeColor(int);
//****************************************************
// Some Classes
//****************************************************
class Viewport {
public:
	int w, h; // width and height
};

class Vertex {
public:
	Vertex(): pos(0.0,0.0) {
		//Constuctor
	}
	Vertex(double a, double b): pos(a,b) {
    }
	float x() {return pos[0];}
	float y() {return pos[1];}
private:
	vec2 pos;
};

class Polygon {
public:
	Polygon() {
		done = false;
		color = WHITEC;
	}
	void finished() {done = true;}
	void draw() {
		int saved = changeColor(color);
		if (vertices.size()>1) {
			if (!done) {
				glBegin(GL_LINE_STRIP);
			} else {
				glBegin(GL_LINE_LOOP);
			}
			for (i = 0;i<vertices.size();i++) {
				x = vertices.at(i).x();
				y = vertices.at(i).y();
				glVertex2f(x,y);
			}
			glEnd();
		}
		changeColor(saved);
	}
	void draw(double a, double b) {
		draw();
		if (vertices.size() > 0) {
			glEnable(GL_LINE_STIPPLE);
			glBegin(GL_LINES);
			glVertex2f(vertices.back().x(),vertices.back().y());
			glVertex2f(a,b);
			glEnd();
			glDisable(GL_LINE_STIPPLE);
		}
	}
	void addVertex(Vertex v) {
		//cout << "Adding: (" << v.x() << ", " << v.y() << ")\n";
		vertices.push_back(v);
	}
	Vertex getEndVertex() {return vertices.back();}
	void print() {
		cout << "Vertices made" << vertices.size()<< "\n";
		unsigned int i;
		for(i = 0;i<vertices.size();i++) {
			cout << "("<< vertices.at(i).x() << ", " << vertices.at(i).y() <<"), ";
		}
		cout << "\n";
	}
	void outputFile(string str) {
		unsigned int i,j,vcount=1;
		ofstream obj(str.c_str(), ios::out);
		for (i=0;i<vertices.size();i++) {
			obj << "v " << vertices.at(i).x() << " " << vertices.at(i).y() << endl;
		}
		if (isLine()){
			for (j = 0;j<(vertices.size()-1);j++) {
				obj << "l " << vcount++ << " " << vcount << endl;
			}
			vcount++;
		} else if (isFace()){
 			obj << "f";
	        for (j=0;j<vertices.size(); j++) {
	            obj << " " << vcount++ ;
	        }
	        obj << endl;
		}
		obj.close();
	}
    Vertex get(int i) {
        return vertices.at(i);
    }
	unsigned int numVertices() {return vertices.size();}
	void setColor(int c) {
		color = c;
	}
	bool isLine() {
		return !done && vertices.size() >= 2;
	}
	bool isFace() {
		return done && vertices.size() >= 2;
	}
	void pop_back() {
		vertices.pop_back();
	}
	bool empty() {
		return vertices.empty();
	}
private:
	vector<Vertex> vertices;
	int color;
	bool done;
	double i,x,y;
};
//****************************************************
// Global Variables
//****************************************************
Viewport viewport;
Polygon * polygon;
vector<Polygon> polys;
Vertex * tempVertex;
bool mouseDown = false;
int polysMade = 0, color = WHITEC;
GLfloat r,b,g;

//-------------------------------------------------------------------------------
void outputFile() {
    unsigned int vcount = 1;
    unsigned int i,j;
    ofstream f("polygon.obj",ios::out);
    for (i=0;i<polys.size();i++){
        for (j=0;j<polys.at(i).numVertices(); j++) {
            f << "v " << polys.at(i).get(j).x() << " " << polys.at(i).get(j).y() << endl;
        }
    }
    for (i=0;i<polys.size();i++){
		if (polys.at(i).isLine()){
			for (j = 0;j<(polys.at(i).numVertices()-1);j++) {
				f << "l " << vcount++ << " " << vcount << endl;
			}
			vcount++;
		} else if (polys.at(i).isFace()){
 			f << "f";
	        for (j=0;j<polys.at(i).numVertices(); j++) {
	            f << " " << vcount++ ;
	        }
			f <<  endl;
		}
    }
	f.close();
}

void outputFiles() {
	unsigned int i = 0;
	string polygon = "polygon", obj = ".obj", filename;
	ostringstream convert;
	for (i=0;i<polys.size();i++) {
		if (i!=0) {
			convert << i;
		}
		filename = polygon.append(convert.str()).append(obj);
		polys.at(i).outputFile(filename);
		polygon ="polygon";
		convert.str("");
	}
}

void changeColor() {
    switch (color) {
        case REDC:
            r = 1.0; g = 0; b = 0;
            break;
        case GREENC:
            r = 0; g = 1.0; b = 0;
            break;
        case BLUEC:
            r = 0; g = 0; b = 1.0;
            break;
        case YELLOWC:
            r = 1.0; g = 1.0; b  = 0;
            break;
        case MAGENTAC:
            r = 1.0; g = 0; b = 1.0;
            break;
        case CYANC:
            r = 0; g = 1.0; b = 1.0;
            break;
        case WHITEC:
            r = 1.0; g = 1.0; b = 1.0;
            break;
        default:
            break;
    }
	polygon->setColor(color);
}
void nextColor() {
	switch(color) {
		case REDC: color = GREENC; break;
		case GREENC: color = BLUEC; break;
		case BLUEC: color = YELLOWC; break;
		case YELLOWC: color = MAGENTAC; break;
		case MAGENTAC: color = CYANC; break;
		case CYANC: color = WHITEC; break;
		case WHITEC: color = REDC; break;
	}
	changeColor();
}
int changeColor(int c ) {
	int rtn = color;
	color = c;
	changeColor();
	glColor3f(r,g,b);
	return rtn;
}

void increaseWidth() {
		float currentw[1], max = 10.0f;
	glGetFloatv(GL_LINE_WIDTH,currentw);
	if (currentw[0] < max) {
		glLineWidth(currentw[0]+1.0f);
	}
	glutPostRedisplay();
}
void decreaseWidth() {
	float currentw[1], min = 1.0f;
	glGetFloatv(GL_LINE_WIDTH,currentw);
	if (currentw[0] > min) {
		glLineWidth(currentw[0]-1.0f);
	}
	glutPostRedisplay();
}
void undo() {
	if(mouseDown) {
        delete tempVertex;
		mouseDown = false;
	} else if (!polygon->empty()){
		polygon->pop_back();
	}
	glutPostRedisplay();
}

void finishedLine() {
	polys.push_back(*polygon);
	outputFiles();
	polygon = new Polygon();
}

void drawCircle() {
	float i;
	
	for(i=0;i<=2*PI;i+=PI/40) {
		Vertex v(.7*cos(i),.7*sin(i));
		polygon->addVertex(v);
	}
}
//-------------------------------------------------------------------------------
void display()
{
	
	//Clear Buffers     
    glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);					// indicate we are specifying camera transformations
	glLoadIdentity();							// make sure transformation is "zero'd"
	
	
    unsigned int i;
    for(i=0;i<polys.size();i++) {
        polys.at(i).draw();
    }
	polygon->draw();
	if (mouseDown)
		polygon->draw(tempVertex->x(),tempVertex->y());
	//Now that we've drawn on the buffer, swap the drawing buffer and the displaying buffer.
	glutSwapBuffers();
}

//-------------------------------------------------------------------------------
/// \brief	Called when the screen gets resized
/// 
void reshape(int w, int h)
{
	//Set up the viewport to ignore any size change.
	glViewport(0,0,viewport.w,viewport.h);
	
	//Set up the PROJECTION transformationstack to be a simple orthographic [-1, +1] projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, 1, -1);	// resize type = stretch
	
	//Set the MODELVIEW transformation stack to the identity.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//-------------------------------------------------------------------------------
///

void myKeyboardFunc (unsigned char key, int x, int y) {
	switch (key) {
		case 27:			// Escape key
			cout << "Exit\n";
			exit(0);
			break;
        case 9:
            nextColor();
            cout << "Changed Colors to " << color << "\n";
			glutPostRedisplay();
            break;
		case 8:
			cout << "Clear All\n";
			polys.clear();
			delete polygon;
			polygon = new Polygon();
			glutPostRedisplay();
			break;
		case 45:
			cout << "Decrease Width\n";
			decreaseWidth();
			break;
		case 61: 
			cout << "Increase Width\n";
			increaseWidth();
			break;
		case 122:
			cout << "undo\n";
			undo();
			break;
		case 32:
			cout << "Finish line";
			finishedLine();
		break;
	}
}

//-------------------------------------------------------------------------------
///
void myMouseFunc( int button, int state, int x, int y ) {	
	float xPos = ((float)x - viewport.w/2)/((float)(viewport.w/2));
	float yPos = ((float)y - viewport.h/2)/((float)(viewport.h/2));
	//Flip the values to get the correct position relative to our coordinate axis.
	yPos = -yPos;
	if ( button==GLUT_LEFT_BUTTON && state==GLUT_DOWN ) {
		tempVertex = new  Vertex(xPos,yPos);
		mouseDown = true;
	} else if (button==GLUT_LEFT_BUTTON && state== GLUT_UP) {
		if (mouseDown)	{	
			tempVertex = new Vertex(xPos,yPos);
			polygon->addVertex(*tempVertex);
	        delete tempVertex;
			mouseDown = false;
		}
	} else if (button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN) {
		polygon->finished();
        polys.push_back(*polygon);
        outputFiles();
        polygon = new Polygon();
	}
	glutPostRedisplay();
}

void myMoveFunc (int x, int y) {
	if (mouseDown){
		float xPos = ((float)x - viewport.w/2)/((float)(viewport.w/2));
		float yPos = ((float)y - viewport.h/2)/((float)(viewport.h/2));
		//Flip the values to get the correct position relative to our coordinate axis.
		yPos = -yPos;
		tempVertex = new Vertex(xPos, yPos);
		glutPostRedisplay();
	}
}

//-------------------------------------------------------------------------------
///
int main(int argc,char** argv)
{
	//Initialize OpenGL
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	
	//Set up global variables
	viewport.w = 600;
	viewport.h = 600;
    polygon = new Polygon();

	//drawCircle();
	//polygon->finished();
	//polys.push_back(*polygon);
	//outputFiles();
	//polygon = new Polygon();
    
	
	//Create OpenGL Window
	glutInitWindowSize(viewport.w,viewport.h);
	glutInitWindowPosition(0,0);
	glutCreateWindow("CS184! - Richard Nguyen");
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	glLineStipple(1, (short) 0x5555);
	
	//Register event handlers with OpenGL.
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(myKeyboardFunc);
	glutMouseFunc(myMouseFunc);
	glutMotionFunc(myMoveFunc);
	
	//And Go!
	glutMainLoop();
}