#include "dynamic.h"
#include "constants.h"

int signum(float x){
	if (x > 0) return 1;
	if (x < 0) return -1;
	return 0;
}

vector<Vec3f> DynamicObj::getDynamicBox(){
	Vec2f endPos = endP;
	unsigned int endRadius = endR;
	/*cout << "In dynamic" << endl;
	cout << "EndP: " << endP.x << " " << endP.y << endl;
	cout << "EndR: " << endR << endl;
*/	int offset = 10;
	vector<Vec3f> temp;

	Vec3f point1;
	Vec3f point2;
	Vec3f point3;
	Vec3f point4;

	point1.x = endPos.x - endRadius - offset;
	point1.y = endPos.y + endRadius + offset;
	point1.z = 1.0;

	point2.x = endPos.x - endRadius - offset;
	point2.y = endPos.y - endRadius - offset;
	point2.z = 1.0;
	holes = generatePath(point1, point2);
	

	point3.x = endPos.x + endRadius + offset;
	point3.y = endPos.y - endRadius - offset;
	point3.z = 1.0;
	temp = generatePath(point2, point3);
	holes.insert(holes.end(), temp.begin(), temp.end());

	point4.x = endPos.x + endRadius + offset;
	point4.y = endPos.y + endRadius + offset;
	point4.z = 1.0;
	temp = generatePath(point3, point4);
	holes.insert(holes.end(), temp.begin(), temp.end());

	temp = generatePath(point4, point1);
	holes.insert(holes.end(), temp.begin(), temp.end());
	//cout <<"HOLES!!"<<endl;
	/*for (int i = 0; i < holes.size(); i++)
		cout << holes[i].x << " " << holes[i].y << " " << holes[i].z << endl;
*/
	return holes;

}

vector<Vec3f> DynamicObj::generatePath(Vec3f start, Vec3f stop){
	vector<Vec3f> dynPath;
	if ((start.x == stop.x) && (start.y == stop.y)){
		cout << "Start and stop is same!!" << endl;
		exit(1);
	} 
	float deltax = stop.x - start.x;
	float deltay = stop.y - start.y; 
	
	float deltaerr = abs(deltay/deltax);
	int y = start.y;
	float error = deltaerr - 1;
	if (deltax >= 0){
		for (int x = start.x; x <= stop.x; x++){
			dynPath.push_back(Vec3f(x,y,start.z));
			//error = error + deltaerr;
			if (error >= 0) {
				//objPath.push_back(Vec3f(x, y));
				y = y + signum(stop.y - start.y);
				error = error - 1.0;
			}
			error += deltaerr;
		}
	}
	else{
		for (int x = start.x; x >= stop.x; x--){
			dynPath.push_back(Vec3f(x,y,start.z));
			//error = error + deltaerr;
			if (error >= 0) {
				//objPath.push_back(Vec3f(x, y));
				y = y + signum(stop.y - start.y);
				error = error - 1.0;
			}
			error += deltaerr;
		}
	}
	// for (int i = 0; i < objPath.size(); i++)
	// 	cout << objPath[i].x << "  " << objPath[i].y << endl;
	return dynPath;
}



