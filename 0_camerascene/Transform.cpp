// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
  mat3 ret;
  // YOUR CODE FOR HW2 HERE
  // Please implement this.  Likely the same as in HW 1.

	// convert to radians
	float rad = degrees / 180. * pi;

	// decompose R as sum of three components
	mat3 R1 = mat3(1.0) * cos(rad);
	mat3 R2 = mat3(axis.x*axis.x, axis.x*axis.y, axis.x*axis.z,
								 axis.x*axis.y, axis.y*axis.y, axis.y*axis.z,
								 axis.x*axis.z, axis.y*axis.z, axis.z*axis.z) * (1 - cos(rad));
	mat3 R3 = mat3(0, axis.z, -axis.y,
								 -axis.z, 0, axis.x,
								 axis.y, -axis.x, 0) * sin(rad);
	ret = R1 + R2 + R3;

  return ret;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.
	
	eye = rotate(degrees, up) * eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
  // YOUR CODE FOR HW2 HERE 
  // Likely the same as in HW 1.

	vec3 axis = glm::normalize(glm::cross(eye, up));
	eye = rotate(degrees, axis) * eye;
	up = rotate(degrees, axis) * up;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
  mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.

	// Factorize M as product of R and T

	// Rotation
	mat4 R(1.0);
	vec3 u = glm::normalize(glm::cross(center-eye, up));
	vec3 v = glm::normalize(up);
	vec3 w = glm::cross(u, v);
	for (int c = 0; c < 3; c++)
	{
		R[c][0] = u[c];
		R[c][1] = v[c];
		R[c][2] = w[c];
	}

	// Translation
	mat4 T(1.0);
	T[3] = vec4(-eye.x, -eye.y, -eye.z, 1.0);

	// Final matrix
	ret = R * T;

  return ret;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
  mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // New, to implement the perspective transform as well.

	ret = mat4(0.0);
	float d = 1.0f / tan(fovy / 180.0f * pi / 2.0f);
	ret[0][0] = d / aspect;
	ret[1][1] = d;
	ret[2][2] = -(zNear + zFar) / (zFar - zNear);
	ret[3][2] = -2.0f * (zNear * zFar) / (zFar - zNear);
	ret[2][3] = -1.0f;
  return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
  mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // Implement scaling
	ret[0][0] = sx;
	ret[1][1] = sy;
	ret[2][2] = sz;
  return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
  mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // Implement translation
	ret = mat4(1.0);
	ret[3][0] = tx;
	ret[3][1] = ty;
	ret[3][2] = tz;
  return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
  vec3 x = glm::cross(up,zvec); 
  vec3 y = glm::cross(zvec,x); 
  vec3 ret = glm::normalize(y); 
  return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
