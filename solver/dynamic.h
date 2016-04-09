#include <iostream>
#include <vector>
#include "Vector.h"

using namespace std;
using namespace math;

int signum(float);
//vector<Vec3f> generatePath(Vec3f, Vec3f);

class DynamicObj{


public:
	vector<Vec3f> holes;
	vector<Vec3f> objPath;
	vector<Vec3f> generatePath(Vec3f, Vec3f);
	vector<Vec3f> getDynamicBox();
};