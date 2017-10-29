/*
 * rotation.h
 *
 *  Created on: 29.10.2017
 *      Author: Nassim
 *      rotate a 2d point with rotationmatrix
 */

#ifndef ROTATION_H_
#define ROTATION_H_
#include <datatypes.h>
#include <math.h>

#ifndef PI
#define PI 3.14159265

// Elemets of the Rotationmatrix
 UNS2 grad;

#endif /* ROTATION_H_ */


 void rotate_2d(UNS8 x, UNS8 y, UNS8 grad){ // Hier muss dann vll n array übergeben werden oder so

	 UNS8 rad = grad*PI/180; // for calculation with sin and cos
	 x = x*sin(rad) - y*cos(rad);
	 y = x*sin(rad) + y*cos(rad);

	 //return dann direkt ins Array schreiben oder in ein struct


 }
