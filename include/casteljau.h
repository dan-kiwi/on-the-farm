#include <list>
#include <vector>
#include <algorithm>

#include "point.h"

point evaluate(float t, std::list<point> P)
{
	std::list<point> Q = P;
	while (Q.size() > 1)
	{
		std::list<point> R;

		std::list<point>::iterator p1 = Q.begin();
		std::list<point>::iterator p2 = Q.begin();
		p2++;
		
		while (p2 != Q.end())
		{
			point newP = ((1 - t) * (*p1)) + (t * (*p2));
			R.push_back(newP);
			p1++, p2++;
		}
		Q.clear();
		Q = R;
	}
	return Q.front();
}

std::vector<point> EvaluateBezierCurve(std::vector<point>ctrl_points, int num_evaluations)
{
	float offset = 1 / ((float) num_evaluations);
	std::vector<point> curve;
	curve.push_back(ctrl_points.front());
	
	std::list<point> list_ctrl_points(ctrl_points.begin(), ctrl_points.end());
	for (int i = 0; i < num_evaluations; i++)
	{
		point p = evaluate(offset * (i + 1), list_ctrl_points);
		curve.push_back(p);
	}

	return curve;
}

float* MakeFloatsFromVector(std::vector<point> curve, int &num_verts, int &num_floats, float r, float g, float b)
{
	num_verts = curve.size();
	if (num_verts == 0)
		return NULL;

	num_floats = num_verts * 6;

	float* vertexAtr = (float*)malloc(num_floats * sizeof(float));

	int i = 0;
	for (std::vector<point>::iterator it = curve.begin(); it != curve.end(); ++it)
	{
		vertexAtr[i++] = it->x;
		vertexAtr[i++] = it->y;
		vertexAtr[i++] = it->z;
		vertexAtr[i++] = r;
		vertexAtr[i++] = g;
		vertexAtr[i++] = b;
	}

	return vertexAtr;
}

