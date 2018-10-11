#ifndef __RAY_H
#define __RAY_H

#include <glm/glm.hpp>

using namespace glm;

class Ray
{
  public:
	Ray() {}
	Ray(const vec3 &ori, const vec3 &dir)
	{
		origin = ori;
		direction = dir;
	}
	vec3 Origin() const
	{
		return origin;
	}
	vec3 Direction() const
	{
		return direction;
	}
	vec3 GetPoint(float t) const
	{
		return origin + t * direction;
	}

  private:
  	vec3 origin;
	vec3 direction;

};

#endif