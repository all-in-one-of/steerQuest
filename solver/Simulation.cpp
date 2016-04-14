#include "Simulation.h"
#include <unistd.h>     // Header File for sleeping.
#include "constants.h"
#include "dynamic.h"

#include <unistd.h>     // Header File for sleeping.
#include <string.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <iomanip> 

using namespace math;
void ToBlender();

int count = 0;
char data[4096];
int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;
char buffer[256];
char *message;
bool flag = false;
std::fstream fd;
int level;

void error(const char* msg)
{
    perror(msg);
    exit(0);
}

Vec2f endP;
unsigned int endR; 

void initSocket()
{
    //Socket
    portno = 8010;
    server = gethostbyname("localhost");
    std::cout << server << std::endl;
    //cout << server->
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting"); 
}


void ToBlender(Flocking* flockDisplay ){
// Don't modify this code
generateObstacles(level);
float zDepth = -1200;
float orientRadian;
std::string myString;
std::setprecision(3) ;
for(int i = 0; i < obstacles.size();i++) {
    myString = myString+"DO "
    + std::to_string(i + 1) + " "
    + std::to_string( obstacles[i].x) + " "
    + std::to_string(obstacles[i].y) + " "
    + std::to_string(obstacles[i].z) + "$$";
    /*if( send(sockfd , myString.c_str(), strlen(myString.c_str()), 0 ) < 0) {

                //cout<<"Send failed";
    }*/
}



if(flockDisplay) {

    vector<Boid>& boids = flockDisplay->boids;
    //flag = d.detectDynamicCollision(boids);
    for(int i =0; i< boids.size() ; i++) {        
        
        if(!boids[i].reachedDestination ) {
            orientRadian = (boids[i].orient - 180) * 3.14 / 180.0;
            if(boids[i].hitObstacle) {
                
                myString = myString + "Boid "
                         + std::to_string(count) + " " 
                         + std::to_string(i) + " " 
                         + std::to_string(boids[i].loc.x) + " "
                         + std::to_string(boids[i].loc.y) + " "
                         + std::to_string(orientRadian) + " "
                         + "false" + "$$";
                         
            } 
            else {
                        myString = myString + "Boid "
                         + std::to_string(count) + " " 
                         + std::to_string(i) + " " 
                         + std::to_string(boids[i].loc.x) + " "
                         + std::to_string(boids[i].loc.y) + " "
                         + std::to_string(orientRadian) + " "
                         + "false" + "$$";

            }
        } 
        else {
                myString = myString + "Boid "
                         + std::to_string(count) + " " 
                         + std::to_string(i) + " " 
                         + std::to_string(boids[i].loc.x) + " "
                         + std::to_string(boids[i].loc.y) + " "
                         + std::to_string(orientRadian) + " "
                         + "true" + "$$";
        }
        //cout << "Sending sheep" << endl;
        /*if( send(sockfd , myString.c_str(), strlen(myString.c_str()),0 )<0) {
            cout<<"Send failed";
        }*/
        //Hidden
        if(colDetect(boids[i])){
            cout <<"Boid "<<i<< "hit" << endl;

            myString = myString + "Hit "
            + std::to_string(i) + "$$";
            cout << "Sending hitssheep" << endl;
            /*if( send(sockfd , myString.c_str(), strlen(myString.c_str()),0 )<0) {
            // //cout<<"Send failed";
             }*/
            // Validate this in Blender
            //drawFish(boids[i].loc.x,boids[i].loc.y,zDepth+2,boids[i].orient,1,0,0);
        }
   
    }

    count++;
}
else{
    cout << "no flock handle!"<<endl;
}

if( send(sockfd , myString.c_str(), strlen(myString.c_str()),0 )<0) {
            // //cout<<"Send failed";
}
obstacles.clear();
fd.flush();
}


void
Simulation::loadScene(char* mapFile)
{

	auto startTime = std::chrono::steady_clock::now();
	mStartTime = std::chrono::steady_clock::now();
  cout << "In loadScene" << endl;
  bool **data = ml.loadVDBMap(mapFile);   //Loading VDB files now
  cout << "Loaded map"<< endl;
	startPosition = ml.getStartPosition();
	endPosition = ml.getEndPosition();
  endP = endPosition;
  endR = ml.getEndRadius();
  
  /*cout << "EndP: " << endP.x << " " << endP.y << endl;
  cout << "EndR: " << endR << endl;*/

    x_bound = ml.getx_boundary();
    y_bound = ml.gety_boundary();

    printf("The grid is %d*%d\n", x_bound, y_bound);
    printf("Start Position %f*%f\n", startPosition.x, startPosition.y);
    printf("End Position %f*%f\n", endPosition.x, endPosition.y);

	bool* passData = new bool[x_bound * y_bound];

	for(int y=0;y<y_bound;y++) {
        for(int x=0;x<x_bound;x++) {
            passData[y*x_bound + x] = data[x][y];
            // if(passData[y*x_bound + x]) {
            //     printf(".");
            // } else {
            //     printf("#");
            // }
        }
        printf("\n");
    }

	Grid<bool> mapData(x_bound, y_bound, passData);
	mScene = new Scene(startPosition, endPosition, mapData , ml.getStartRadius(), ml.getEndRadius());

    mScene->setSDFhandle(ml.getSDF());

	auto endTime = std::chrono::steady_clock::now();
	std::cout << "Map Loading Time (ms) : " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << std::endl;

	/*
	for(int x=0;x<x_bound;x++) 
	for(int y=0;y<y_bound;y++) {
        {
            std::cout<<"Grid array(in Simulation) "<<ml.getSDF()[x][y]<<"\n";
        }
        //printf("\n");
    }
    */
}

bool
Simulation::frame()
{
    int status = flock.update();

    usleep(sleepTime); 

    ToBlender(getFlockHandle());
    if(status)
        return true;
    else
        return false;
}


void
Simulation::init()

{
  cout << "In Simulation Init" << endl;
	auto startTime = std::chrono::steady_clock::now();
	auto endTime = std::chrono::steady_clock::now();


      // sleepTime			= msleepTime		;
      // fishCount 			= mfishCount 		;
      // boundaryPadding 	= mboundaryPadding	;
      // maxSpeed 			= mmaxSpeed 		;
      // maxForce 			= mmaxForce 		;
      // flockSepWeight 		= mflockSepWeight 	;
      // flockAliWeight 		= mflockAliWeight 	;
      // flockCohWeight 		= mflockCohWeight 	;
      // collisionWeight 	= mcollisionWeight	;
      // flockSepRadius 		= mflockSepRadius 	;	
      // flockAliRadius 		= mflockAliRadius 	;
      // flockCohRadius 		= mflockCohRadius 	;
      // destWeight 			= mdestWeight 		;
      // randSeed 			= mrandSeed 		;
	

	startPositionRadius=mScene->getStartRadius();
	endPositionRadius=mScene->getEndRadius();

    int startPosMinX = min((int)(startPosition.x-startPositionRadius),0);
    int startPosMaxX = min((int)(startPosition.x+startPositionRadius),(int)x_bound);
    int startPosMinY = min((int)(startPosition.y-startPositionRadius),0);
    int startPosMaxY = min((int)(startPosition.y+startPositionRadius),(int)y_bound);


    flock.setBounds(x_bound,y_bound);
    //Assign appropriate values to these, or get them from command line:
    boundaryPadding = 0;
    maxForce  = 0;
    flockSepWeight = 0;  
    flockAliWeight = 0;
    flockCohWeight = 0;
    collisionWeight = 0;
    flockSepRadius = 0;
    flockAliRadius = 0;
    flockCohRadius = 0;
    destWeight = 0;

    flock.setSimulationParameters(boundaryPadding	,
      			maxSpeed 		,
  				maxForce 		,
				flockSepWeight 	,
				flockAliWeight 	,
				flockCohWeight 	,
    			collisionWeight ,
				flockSepRadius 	,
				flockAliRadius 	,
				flockCohRadius 	,
    			destWeight		); 		
    
    flock.setDestination(endPosition,endPositionRadius);
    flock.setSceneMap(mScene);
    flock.useCollisionSDF(true);
    flock.calculatePartialDerivaties();

    int seed=randSeed;
    
	for(int i = 0; i < fishCount; ++i) 
    {
        float rand_radius = (float)randomRange(0,(int)(startPositionRadius*100),seed+i)/100;
        float theta = (float)randomRange(0,360,seed+i+1);//Arbritary +1. just to change seed
        flock.addBoid(startPosition.x+rand_radius*cos(theta*PI/180),startPosition.y+rand_radius*sin(theta*PI/180));
    }
      //initSocket();
	  if (DEBUG !=1 ){
        initSocket();
    }
    std::cout << "Init Time (ms) : " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << std::endl;

    
}



void
Simulation::run()
{
  cout << "Inside sim run";
	bool continueRunning = true;
	static long long simTime = 0;



	while (continueRunning) {
		auto startTime = std::chrono::steady_clock::now();

		continueRunning = frame();

		auto endTime = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

		simTime+=duration;

	}
	std::cout << "Flocking simulation time (ms) : "<<simTime<<std::endl;
	mEndTime = std::chrono::steady_clock::now();
}


long long
Simulation::totalTime()
{
	return std::chrono::duration_cast<std::chrono::seconds>(mEndTime - mStartTime).count();
}


Flocking* Simulation::getFlockHandle()
{
        return &flock;
}

// Scene* Simulation::getSceneHandle()
// {
//         return mScene;
// }

