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
				// <Ball-Paddle Collision Code> 
				
				// Which case has the smallest penetration? (resolve 
				// using that one, so that we bounce correctly when running 
				// into a corner of the box head-on. Not an entirely correct
				// method, but a good heuristic in non-extreme cases where the
				// ball's velocity is small comared to the size of the box.
				// Ideally we would check whether the ball's path crossed over 
				// the diagonal of that corner of the box inside, or outside
				// the box, or not at all, to determine which of the faces it
				// trully collided with)
				int smallestPenetrationCase = 0;
				float smallestPenetrationDistance = 10000.0f;

				// case 1: the ball is initially above the paddle and is going down
				if (lastY > pt && vY < 0) {
					float dist = (pt - locY);
					smallestPenetrationDistance = dist;
					smallestPenetrationCase = 1;
				}

				// case 2: the ball is initially below the paddle and is going up
				if (lastY < pb && vY > 0) {
					float dist = (locY - pb);
					if (dist < smallestPenetrationDistance) {
						smallestPenetrationDistance = dist;
						smallestPenetrationCase = 2;
					}
				}

				// case 3: the case is initially to the left of the paddle and is going right
				if (lastX > pr && vX < 0) {
					float dist = (locX - pr);
					if (dist < smallestPenetrationDistance) {
						smallestPenetrationDistance = dist;
						smallestPenetrationCase = 3;
					}
				}

				// case 4: the ball is initially to the right of the paddle and is going left
				if (lastX < pl && vX > 0) {
					float dist = (pl - locX);
					if (dist < smallestPenetrationDistance) {
						smallestPenetrationDistance = dist;
						smallestPenetrationCase = 4;
					}
				}

				// Case handling now that we know which case to resolve
				float penetrationFrac;
				switch (smallestPenetrationCase) {
				case 1:
				case 2:
					penetrationFrac = smallestPenetrationDistance / vY;
					locX -= penetrationFrac*vX;
					locY -= penetrationFrac*vY;
					vY = -vY;
					locX += penetrationFrac*vX;
					locY += penetrationFrac*vY;
					break;

				case 3: 
				case 4:
					penetrationFrac = smallestPenetrationDistance / vX;
					locX -= penetrationFrac*vX;
					locY -= penetrationFrac*vY;
					vX = -vX;
					locX += penetrationFrac*vX;
					locY += penetrationFrac*vY;
					break;

				default:
					break; // Nothing to do
				}

				// Add to the score if the collision was handled
				// NOTE: Velocity direction test + reflected velocity will
				// ensure that we never double-count a paddle hit, since even
				// if the position is just touching after resolving the
				// collision, the reflected velocity will mean we never enter
				// any of the cases that next iteration, and we won't 
				// score a second time.
				if (smallestPenetrationCase != 0) {
					++*score;
				}
			}
		}
		else { // box
			pt = b.top() - radius;
			pb = b.bottom() + radius;
			pl = b.left() + radius;
			pr = b.right() - radius;
			if (!inside(locX, locY, pt, pb, pl, pr)) {
				// <Ball-Wall Collision Code>

				// In case any edge penetrates, we need to go back and check the 
				// other ones again, since fixing one penetration may induce 
				// one further one when the ball bounces near the corners.
				// NOTE: It does not matter which order we resolve the
				// penetrations in when there are multiple ones, we will get
				// the same, correct, result regardless of the order. We simply
				// need to cover each one once. Given that each case checks
				// for both collision *and* velocity direction, and goes on to
				// reflect the velocity direction, each can only be done once.
			penetrationCases:

				// there are 4 cases
				// case 1: the ball is initially above the bottom wall and is going down
				if (locY < pb && vY < 0) {
					// In each case, first find the fraction of the velocity
					// by which the ball penetrated into the wall.
					float penetrationFrac = (pb - locY)/vY;

					// Then rewind the movement by that much.
					locX -= penetrationFrac*vX;
					locY -= penetrationFrac*vY;

					// Apply the collision "bounce" to the velocity
					vY = -vY;

					// Move the ball along the (now) correct path using the
					// remaining velocity.
					locX += (1-penetrationFrac)*vX;
					locY += (1-penetrationFrac)*vY;

					// Also update the score in the case of hitting the bottom
					// wall. (The user loses a point)
					--*score;

					// Finally, retest all of the cases for further collisions.
					goto penetrationCases;
				}

				// case 2: the ball is initially below the top wall and is going up
				if (locY > pt && vY > 0) {
					float penetrationFrac = (locY - pt)/vY;
					locX -= penetrationFrac*vX;
					locY -= penetrationFrac*vY;
					vY = -vY;
					locX += (1-penetrationFrac)*vX;
					locY += (1-penetrationFrac)*vY;
					goto penetrationCases;
				}

				// case 3: the case is initially to the left of the right wall and is going right
				if (locX > pr && vX > 0) {
					float penetrationFrac = (locX - pr)/vX;
					locX -= penetrationFrac*vX;
					locY -= penetrationFrac*vY;
					vX = -vX;
					locX += (1-penetrationFrac)*vX;
					locY += (1-penetrationFrac)*vY;
					goto penetrationCases;
				}

				// case 4: the ball is initially to the right of the left wall and is going left
				if (locX < pl && vX < 0) {
					float penetrationFrac = (pl - locX)/vX;
					locX -= penetrationFrac*vX;
					locY -= penetrationFrac*vY;
					vX = -vX;
					locX += (1-penetrationFrac)*vX;
					locY += (1-penetrationFrac)*vY;					
					goto penetrationCases;
				}
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