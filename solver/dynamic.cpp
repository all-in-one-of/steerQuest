#include "dynamic.h"
#include "constants.h"
#include <cstdlib>

int diag_size;
int diag_done;
int box_size;
int box_done;
vector<Vec3f> dynamicDiag;
vector<Vec3f> dynamicBox;
vector<Vec3f> obstacles;
Vec3f end_l0 = Vec3f(810.0, 309.0, 70.0);

int signum(float x){
	if (x > 0) return 1;
	if (x < 0) return -1;
	return 0;
}

vector<Vec3f> DynamicObj::getDynamicBox(){
	Vec2f endPos = Vec2f(end_l0.x, end_l0.y);
	unsigned int endRadius = end_l0.z;
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
	point1.z = 10.0;

	point2.x = endPos.x - endRadius - offset;
	point2.y = endPos.y - endRadius - offset;
	point2.z = 10.0;
	holes = generatePath(point1, point2);


	//cout<<"holes end"<< " "<<holes.size()<<endl;
	

	point3.x = endPos.x + endRadius + offset;
	point3.y = endPos.y - endRadius - offset;
	point3.z = 10.0;
	temp = generatePath(point2, point3);
	holes.insert(holes.end(), temp.begin(), temp.end());
	//cout<<"holes end"<<" "<<holes.size()<<endl;

	point4.x = endPos.x + endRadius + offset;
	point4.y = endPos.y + endRadius + offset;
	point4.z = 10.0;
	temp = generatePath(point3, point4);
	holes.insert(holes.end(), temp.begin(), temp.end());

	//cout<<"holes end"<<" "<<holes.size()<<endl;

	temp = generatePath(point4, point1);
	holes.insert(holes.end(), temp.begin(), temp.end());
	//cout<<"holes end"<< " "<<holes.size()<<endl;
	//cout << point1.x <<  " "<<point1.y << ","<< point2.x<<" " << point2.y << ","<< point3.x << " "<<point3.y << ","<< point4.x <<" "<< point4.y<<endl;
	//cout <<"HOLES!!"<<endl;
	/*for (int i = 0; i < holes.size(); i++)
		cout << holes[i].x << " " << holes[i].y << " " << holes[i].z << endl;
*/
	return holes;

}

vector<Vec3f> DynamicObj::getDiagonal(){
	vector<Vec3f> dynPath;
	int tx = end_l0.x *2;
	int ty = end_l0.y*2;
	Vec3f point1 = Vec3f( 1000,tx %1000, 10);
	Vec3f point2 = Vec3f( ty %1000,0, 10); 
	dynPath = generatePath(point1, point2);
	diag_size = dynPath.size();
	diag_done = 0;
	return dynPath;
}


vector<Vec3f> DynamicObj::generatePath(Vec3f start, Vec3f stop){
	vector<Vec3f> dynPath;
	int from=10;
	int to = 5;
	//cout << "Start" << start.x << " " << start.y << "End " << stop.x<<" "<<stop.y<<endl;
	int rad = rand() % from + to;
	if ((start.x == stop.x) && (start.y == stop.y)){
		cout << "Start and stop is same!!" << endl;
		exit(1);
	} 
	if (start.x == stop.x){
		if (stop.y > start.y){
			for (int y = start.y; y <= stop.y; y++){
				rad = rand() % from + to;
				dynPath.push_back(Vec3f(start.x,y,rad));
			}
		}
		else{
			for (int y = start.y; y >= stop.y; y--){
				rad = rand() % from + to;
				dynPath.push_back(Vec3f(start.x,y,rad));
			}			
		}
		return dynPath;
	}
	else if (start.y == stop.y){
		if (stop.x > start.x){
			for (int x = start.x; x <= stop.x; x++){
				rad = rand() % from + to;
				dynPath.push_back(Vec3f(x,start.y,rad));
			}
		}
		else{
			for (int x = start.x; x >= stop.x; x--){
				rad = rand() % from + to;
				dynPath.push_back(Vec3f(x,start.y,rad));
			}

		}
		return dynPath;
	}

	float deltax = stop.x - start.x;
	float deltay = stop.y - start.y; 
	
	float deltaerr = abs(deltay/deltax);
	
	float error = deltaerr - 1;

	int y = start.y;

	if (deltax >= 0){
		for (int x = start.x; x <= stop.x; x++){
			rad = rand() % from + to;
			dynPath.push_back(Vec3f(x,y,rad));
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
			rad = rand() % from + to;
			dynPath.push_back(Vec3f(x,y,rad));
			//error = error + deltaerr;
			if (error >= 0) {
				//objPath.push_back(Vec3f(x, y));
				y = y + signum(stop.y - start.y);
				error = error - 1.0;
			}
			error += deltaerr;
		}
	}
	 /*for (int i = 0; i < dynPath.size(); i++)
	 	cout << dynPath[i].x << "  " << dynPath[i].y << endl;*/
	return dynPath;
}


// bool DynamicObj::detectDynamicCollision(vector<Boid> boids){

// 	//cout << "enappa";
// 	for (int i=0; i< boids.size(); i++){
// 		//cout << "Boid data" << boids[i].loc.x << " "<<boids[i].loc.x<< endl;
		
// 		//cout << "Dynamic done" <<dynamic_done<<endl;
// 		if (colDetect(boids[i],dynamic_done)) {
// 			cout << "Sheep hit "<<i<<endl;
// 			return true;
// 		}
// 		if (dynamic_done < dynamic_size){
// 			dynamic_done +=2;
// 		}
// 		else
// 			dynamic_done = 0;
// 	}
// 	return false;

// }

bool colDetect(Boid b){
	float x = b.loc.x ;
	float y = b.loc.y;

	for(int i = 0; i<obstacles.size();i++){
		float cx = obstacles[i].x;
		float cy = obstacles[i].y;
		float r  = obstacles[i].z;
		//cout <<"Obs: "<< cx<<" " << " "<<cy << " "<<r<<endl;
		if (pow((cx-x),2) + pow((cy-y),2) <= pow(r,2) ){
			cout << "Hole hit"<<endl;
			return true;
		}
	}
	return false;	
}

void levelZero(){

    

}

void levelOne(){
	initObstacles(1);

}

void levelTwo(){
	initObstacles(2);

    
}

void levelHandler(int level){
	cout << "levelhandler!!!" << level << endl;
	if(level == 0){}

	else if (level ==1)			
		levelOne();

	else if (level == 2){
		levelOne();	
		levelTwo();
	}
	else
		cout << "There are three levels 0, 1 and 2! Enter a valid level." << endl;


}



void generateObstacles(int level){
	if (level != 0){
		int idx = rand() % 5 + 1;
		diag_done = (diag_done + idx) % diag_size;
		obstacles.push_back(dynamicDiag[diag_done]);		
	}
	if(level == 2){
		int idx = rand() % 5 + 1;
		box_done = (box_done + idx) % box_size;
		obstacles.push_back(dynamicBox[box_done]);
	}
}



void initObstacles(int level){
    DynamicObj d;    
    dynamicDiag = d.getDiagonal();


    if (level == 2){
    	dynamicBox = d.getDynamicBox();
    	box_size = dynamicBox.size();
	}
}