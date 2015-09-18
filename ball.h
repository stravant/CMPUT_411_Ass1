#ifndef BALL_H
#define BALL_H
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include "box.h"
const double PI  =3.141592653589793238463;
const int BALL_SEGMENTS = 16; // should be divisible by 4
class ball{
	GLfloat lastX, lastY;		// lastX and lastY
	GLfloat locX, locY, radius;// location and radius
	GLfloat r,g,b;	// ball colour
	GLfloat vX, vY; // ball direction
public:
	void setLoc(GLfloat x, GLfloat y, GLfloat rad, GLfloat red, GLfloat green, GLfloat blue){
		locX = x;
		locY = y;
		lastX = locX;
		lastY = locY;
		radius = rad;
		r = red;
		g = green;
		b = blue;
	};
	void setDir(GLfloat a, GLfloat b){
		vX = a;
		vY = b;
	};
	void move(){
		//<Ball Moving Code>
		// Save the current position (for collision detection)
		lastX = locX;
		lastY = locY;

		// Update to the new position
		locX += vX;
		locY += vY;
	};
	// inside is to return true if the point is inside the box
	// Note: the boundary must include the radius of the ball
	static bool inside(GLfloat x, GLfloat y, GLfloat ty, GLfloat by, GLfloat lx, GLfloat rx) {
		if (x > lx && x <  rx && y> by && y<  ty) {
			return true;
		}
		else {
			return false;
		}
	}
	void collision(box b, GLint *score) {
		GLfloat pt;
		GLfloat pb;
		GLfloat pl;
		GLfloat pr;
		GLfloat t, newX, newY;
		if (b.isFilled()) {		// paddle
			pt = b.top() + radius;
			pb = b.bottom() - radius;
			pl = b.left() - radius;
			pr = b.right() + radius;
			if (inside(locX, locY, pt, pb, pl, pr))
			{
				// there are 4 cases
				// case 1: the ball is initially above the paddle and is going down
				// case 2: the ball is initially below the paddle and is going up
				// case 3: the case is initially to the left of the paddle and is going right
				// case 4: the ball is initially to the right of the paddle and is going left
				// Add code below








				// add code above
			}
		}
		else { // box
			pt = b.top() - radius;
			pb = b.bottom() + radius;
			pl = b.left() + radius;
			pr = b.right() - radius;
			if (!inside(locX, locY, pt, pb, pl, pr)) {
				// there are 4 cases
				// case 1: the ball is initially above the bottom wall and is going down
				// case 2: the ball is initially below the top wall and is going up
				// case 3: the case is initially to the left of the right wall and is going right
				// case 4: the ball is initially to the right of the left wall and is going left
				// Add code below








				// add code above
			}
		}
	}
	
	void draw(){
		GLfloat angle;
		glColor3f(r,g,b);// color of ball
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_POLYGON); // The stack to draw the ball

		// <Circle Drawing Code>
		// Break the circle into a regular polygon with |BALL_SEGMENTS| 
		// vertices to draw it. Start at theta = 0 and walk in
		// 2pi/|BALL_SEGMENTS| sized steps around the circle adding verts.
		float delTheta = 2*PI/(float)BALL_SEGMENTS;
		for (int i = 0; i < BALL_SEGMENTS; ++i) {
			float theta = delTheta*i;
			float x = locX + radius*cos(theta);
			float y = locY + radius*sin(theta);
			glVertex2f(x, y);
		}

		glEnd();
		glFlush();
	}
};
#endif