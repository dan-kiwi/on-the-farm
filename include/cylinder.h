#pragma once

#define PI 3.14159265

std::vector<point> createCircle(point center, float radius, int num_points)
{
	std::vector<point> circle;
	float angle = 0;
	const float angle_offset = 2 * PI / num_points;
	for (int i = 0; i < num_points; i++)
	{
		point p;
		p.x = center.x + radius * cos(angle);
		p.y = center.y;
		p.z = center.z + radius * sin(angle);
		circle.push_back(p);
		angle += angle_offset;
	}
	return circle;
}


void MakeCyclinderSlice(float* cyl, int& i, point center, point curr, point curr_last, float height, float angle)
{
	//Top of cylinder
	cyl[i++] = center.x;
	cyl[i++] = center.y;
	cyl[i++] = center.z;
	cyl[i++] = 0.5f;
	cyl[i++] = 0.f;
	cyl[i++] = 0.f;
	cyl[i++] = 1.f; 
	cyl[i++] = 0.f;
	cyl[i++] = curr_last.x;
	cyl[i++] = curr_last.y;
	cyl[i++] = curr_last.z;
	cyl[i++] = 0.f;
	cyl[i++] = 1.f;
	cyl[i++] = 0.f;
	cyl[i++] = 1.f;
	cyl[i++] = 0.f;
	cyl[i++] = curr.x;
	cyl[i++] = curr.y;
	cyl[i++] = curr.z;
	cyl[i++] = 1.f;
	cyl[i++] = 1.f;
	cyl[i++] = 0.f;
	cyl[i++] = 1.f;
	cyl[i++] = 0.f;


	//Side of cylinder
	point curr_last_deep = curr_last;
	curr_last_deep.y -= height;
	point curr_deep = curr;
	curr_deep.y -= height;
	cyl[i++] = curr_last.x;
	cyl[i++] = curr_last.y;
	cyl[i++] = curr_last.z;
	cyl[i++] = 0.f;
	cyl[i++] = 0.9f;
	cyl[i++] = curr.x;
	cyl[i++] = 0.f;
	cyl[i++] = curr.z;
	cyl[i++] = curr_last_deep.x;
	cyl[i++] = curr_last_deep.y;
	cyl[i++] = curr_last_deep.z;
	cyl[i++] = 0.f;
	cyl[i++] = 1.f;
	cyl[i++] = curr.x;
	cyl[i++] = 0.f;
	cyl[i++] = curr.z;
	cyl[i++] = curr_deep.x;
	cyl[i++] = curr_deep.y;
	cyl[i++] = curr_deep.z;
	cyl[i++] = 1.f;
	cyl[i++] = 1.f;
	cyl[i++] = curr.x;
	cyl[i++] = 0.f;
	cyl[i++] = curr.z;

	cyl[i++] = curr_last.x;
	cyl[i++] = curr_last.y;
	cyl[i++] = curr_last.z;
	cyl[i++] = 0.f;
	cyl[i++] = 0.9f;
	cyl[i++] = curr.x;
	cyl[i++] = 0.f;
	cyl[i++] = curr.z;
	cyl[i++] = curr_deep.x;
	cyl[i++] = curr_deep.y;
	cyl[i++] = curr_deep.z;
	cyl[i++] = 1.f;
	cyl[i++] = 1.f;
	cyl[i++] = curr.x;
	cyl[i++] = 0.f;
	cyl[i++] = curr.z;
	cyl[i++] = curr.x;
	cyl[i++] = curr.y;
	cyl[i++] = curr.z;
	cyl[i++] = 1.f;
	cyl[i++] = 0.9f;
	cyl[i++] = curr.x;
	cyl[i++] = 0.f;
	cyl[i++] = curr.z;
}

float* MakeCyclinder(std::vector<point> points, float height, int& num_verts, int& num_floats)
{
	int num_triangles = 3 * points.size();
	num_verts = 3 * num_triangles;
	num_floats = num_verts * 8;
	float* cyl = (float*)malloc(sizeof(float) * num_floats);

	const float angle_offset = 2 * PI / points.size();
	float angle = angle_offset / 2;

	std::vector<point>::iterator it = points.begin();
	const point first = *it;
	point curr = *it;
	point curr_last;
	it++;

	point center = point(0.f, 0.f, 0.f);

	int i = 0;
	while (it != points.end())
	{
		curr_last = curr;
		curr = *it;
		MakeCyclinderSlice(cyl, i, center, curr, curr_last, height, angle);
		it++;
		angle += angle_offset;
	}
	MakeCyclinderSlice(cyl, i, center, first, curr, height, 0);
	return cyl;
}