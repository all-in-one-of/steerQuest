#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <unistd.h>     // Header File for sleeping.
#include <string.h>
#include <X11/Xlib.h>
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
#include <iostream>
#include "Simulation.h"


//#include "Simulation.h"

/* ASCII code for the escape key. */
#define ESCAPE 27

/* The number of our GLUT window */
int window;

Flocking* flockDisplay; 
Scene* sceneDisplay;
std::ofstream positionFile;
char data[4096];
int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;
char buffer[256];
char *message;
int count = 0;
int gl;
//std::vector<DispFish> display_fishes;
std::fstream fd;
bool bWriteToPipe=false;
void error(const char* msg)
{
	perror(msg);
	exit(0);
}

void openPipe(char* pipeFile)
{
	if(pipeFile == nullptr) {
        // Don't open the pipe if the pipe file arg wasn't passed
        // Useful when debugging
		bWriteToPipe = false;
		return;
	}

	printf("The pipefile is %s\n", pipeFile);
    /* create the FIFO (named pipe) */
	mkfifo(pipeFile, 0700);
	fd.open(pipeFile, std::fstream::out);
	bWriteToPipe = true;
}

int simMain(int argc, char* argv[])
{

	if(argc<15)
	{
		printf("Ideally these are the parameters you should be passing to run the solver\n");
		printf("%s <map_path> <sleep_time (ms)> <fish_count>			\
			<boundary_padding> <max_speed> <max_force>			\
			<flocking_separation_force_weight>					\
			<flocking_alignment_force_weight>					\
			<flocking_cohesion_force_weight>						\
			<collision_avoidance_weight>							\
			<flocking_separation_radius>							\
			<flocking_alignment_radius>							\
			<flocking_cohesion_radius>							\
			<destination_weight>									\
			<random_seed>										\
	        <gl flag                                               \
            \
			\n", argv[0]);
		exit(0);
		return 1;
	}

    // The command line parameters for this would look like this:

    char* mapFile 											= argv[1];	//path to the vdb file
    char* sleep_time 										= argv[2];	//eventually you'll make this 0 to see how fast your solver really is.
    char* fish_count 										= argv[3];
    char* boundary_padding 									= argv[4];
    char* max_speed 										= argv[5];
    char* max_force 										= argv[6];
    char* flocking_separation_force_weight 					= argv[7];
    char* flocking_alignment_force_weight					= argv[8];
    char* flocking_cohesion_force_weight					= argv[9];
    char* collision_avoidance_weight						= argv[10];
    char* flocking_separation_radius						= argv[11];
    char* flocking_alignment_radius							= argv[12];
    char* flocking_cohesion_radius							= argv[13];
    char* destination_weight								= argv[14];
    char* random_seed										= argv[15];



    long sleepTime 				= stoi(sleep_time);
    int fishCount 				= stoi(fish_count);
    int boundaryPadding 		= stoi(boundary_padding);
    float maxSpeed 				= stof(max_speed);
    float maxForce 				= stof(max_force);
    float flockSepWeight 		= stof(flocking_separation_force_weight);
    float flockAliWeight 		= stof(flocking_alignment_force_weight);
    float flockCohWeight 		= stof(flocking_cohesion_force_weight);
    float collisionWeight 		= stof(collision_avoidance_weight);
    float flockSepRadius 		= stof(flocking_separation_radius);
    float flockAliRadius 		= stof(flocking_alignment_radius);
    float flockCohRadius 		= stof(flocking_cohesion_radius);
    float destWeight 			= stof(destination_weight);
    int randSeed 				= stoi(random_seed);


    char* pipeFile = nullptr;
    if(argc==18)
    {
    	pipeFile = argv[17];
    	std::cout << "\nPipe File " << pipeFile << "\n";
    }

    openPipe(pipeFile);

    Simulation simulation;
    simulation.loadScene(mapFile);
    simulation.init(pipeFile,sleepTime,
    	fishCount,boundaryPadding,
    	maxSpeed,maxForce,
    	flockSepWeight,
    	flockAliWeight,
    	flockCohWeight,
    	collisionWeight,
    	flockSepRadius,
    	flockAliRadius,
    	flockCohRadius,
    	destWeight,
    	randSeed
    	);

    flockDisplay = simulation.getFlockHandle();
    sceneDisplay = simulation.getSceneHandle();

    simulation.run();


    std::cout << "Total Simulation Time : " << simulation.totalTime() << std::endl;

    exit(0);
    return 0;
}

void drawRegion(float x, float y,float z,float radius,float r,float g, float b)
{
	glTranslatef(x,y,z);
	glBegin(GL_POLYGON);
	for(int i=0; i < 360; i+=1)
	{
	    //convert degrees into radians
		float degInRad = i*PI/180;
		glColor3f(r,g,b);
		glVertex3f(cos(degInRad)*radius,sin(degInRad)*radius,0);
	}
	glEnd();

}

void drawFish(float x, float y,float z,float orient,float r,float g, float b)
{
	glTranslatef(x,y,z);
	float scale=1;
    glRotatef(orient-90,0,0,1); //180_ to fix counter clockwise. fish point up Y at orientation 0 so _90

    // draw a triangle (in smooth coloring mode)
    glBegin(GL_POLYGON);				// start drawing a polygon
    glColor3f(r,g,b);
    glVertex3f( 0.0f, scale*2, 0.0f);		// Top
    glColor3f(r,g,b);
    glVertex3f( scale,-scale, 0.0f);		// Bottom Right
    glColor3f(r,g,b);
    glVertex3f(-scale,-scale, 0.0f);		// Bottom Left
    glEnd();					// we're done with the polygon (smooth color interpolation)


}

void drawCell(float x, float y,float z,float r,float g, float b)
{
	glTranslatef(x,y,z);
	float scale=5;
    //drawing a cell
    glBegin(GL_POLYGON);				// start drawing a polygon
    glColor3f(r,g,b);
    glVertex3f(-scale,scale, 0.0f);		// Top Left
    glColor3f(r,g,b);
    glVertex3f(scale,scale,0.0f);		// Top Right
    glColor3f(r,g,b);
    glVertex3f(scale,-scale, 0.0f);		// Bottom Right
    glColor3f(r,g,b);
    glVertex3f(-scale,-scale,0.0f);		// Bottom Left
    glEnd();


}

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


/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
    glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);				// The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
    glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();				// Reset The Projection Matrix

    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

    glMatrixMode(GL_MODELVIEW);
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
    if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
    	Height=1;

    glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100000.0f);
    glMatrixMode(GL_MODELVIEW);
}



/* The main drawing function. */
void DrawGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
    glLoadIdentity();				// Reset The View

    glTranslated(-100,-100,0);  //Reposition Camera
    float zDepth = -250;
    double cosTheta;
    double sinTheta;


    if(sceneDisplay) {
    	Vec2i bounds = sceneDisplay->getBounds();
        //Start
    	glPushMatrix();
    	drawRegion(sceneDisplay->getStartPosition().x, sceneDisplay->getStartPosition().y,zDepth,sceneDisplay->getStartRadius(),0,0,1);
    	glPopMatrix();
        //End
    	glPushMatrix();
    	drawRegion(sceneDisplay->getEndPosition().x, sceneDisplay->getEndPosition().y,zDepth,sceneDisplay->getEndRadius(),0,0.7,0);
    	glPopMatrix();
    	for(int i = 0 ; i < bounds.x;i+=10) {
    		for(int j = 0 ; j < bounds.y;j+=10) {
    			glPushMatrix();
    			if(sceneDisplay->getCell(i,j)) {
                    drawCell(i,j,zDepth-1,0.9f,0.9f,1.0f);// -1 to put them below fishes
                } else {
                    drawCell(i,j,zDepth-1,0.7f,0.5f,0.2f);// -1 to put them below fishes
                }
                glPopMatrix();
            }
        }   
        if(flockDisplay) {
        	vector<Boid>& boids = flockDisplay->boids;
        	for(int i =0; i< boids.size() ; i++) {
                  //  FishSim fish;
                  //  Fish 0 is the path information
                  //  fish.set_fish_id(i);
                  //  fish.set_pos_x(boids[i].loc.x);
                  //  fish.set_pos_y(boids[i].loc.y);
                  //  char sz = fish.ByteSize();
                  //  if(bWriteToPipe) {
                  //     fd.write(&sz, sizeof(char));
                  //     fish.SerializeToOstream(&fd);
                  // }


        		std::string myString;
        		glPushMatrix();
        		if(!boids[i].reachedDestination && !count == 10) {
        			cosTheta = cos(boids[i].orient);
        			sinTheta = sin(boids[i].orient);
        			if(boids[i].hitObstacle) {
        				drawFish(boids[i].loc.x,boids[i].loc.y,zDepth+1,boids[i].orient,1,0,0);
        		//		if (gl == false)
                            myString = std::to_string(count) + " " + std::to_string(i) + " " + std::to_string(boids[i].loc.x) + " " + std::to_string(boids[i].loc.y) + " " + std::to_string(-sinTheta) + " " + std::to_string(0) + " " + std::to_string(sinTheta) + " " + std::to_string(cosTheta) + " " + std::to_string(0) + " " + std::to_string(0) + " " + std::to_string(1) + " " + "$$";
                        // positionFile << count << " " << i << " " << boids[i].loc.x << " " << boids[i].loc.y<< " " << cosTheta << " " << -sinTheta << " " << '0' << sinTheta << " " << cosTheta << '0' << " " << '0' << '0' << '1' << " true \n"; //hit = true
        			} else {
                    	//TODO change for testing hit
        				drawFish(boids[i].loc.x,boids[i].loc.y,zDepth+1,boids[i].orient,0,0.5,1);
        		//		if (gl == false)
                            myString = std::to_string(count) + " " + std::to_string(i) + " " + std::to_string(boids[i].loc.x) + " " + std::to_string(boids[i].loc.y) + " " + std::to_string(-sinTheta) + " " + std::to_string(0) + " " + std::to_string(sinTheta) + " " + std::to_string(cosTheta) + " " + std::to_string(0) + " " + std::to_string(0) + " " + std::to_string(1) + " " + "$$";
                        // positionFile << count << " " << i << " " << boids[i].loc.x << " " << boids[i].loc.y<< " " << cosTheta << " " << -sinTheta << " " << '0' << sinTheta << " " << cosTheta << '0' << " " << '0' << '0' << '1' << " false \n"; //hit = true
        			}
        		} else {
        			drawFish(boids[i].loc.x,boids[i].loc.y,zDepth+1,boids[i].orient,0,1,0);
        		//		if (gl == false)
                            myString = std::to_string(count) + " " + std::to_string(i) + " " + std::to_string(boids[i].loc.x) + " " + std::to_string(boids[i].loc.y) + " " + std::to_string(-sinTheta) + " " + std::to_string(0) + " " + std::to_string(sinTheta) + " " + std::to_string(cosTheta) + " " + std::to_string(0) + " " + std::to_string(0) + " " + std::to_string(1) + " " + "$$";
                        // positionFile << count << " " << i << " " << boids[i].loc.x << " " << boids[i].loc.y<< " " << cosTheta << " " << -sinTheta << " " << '0' << sinTheta << " " << cosTheta << '0' << " " << '0' << '0' << '1' << " false \n"; //hit = true
        		}
                //while ((read = getline(&data, &len, in)) != -1){
                //printf("Sending: %s\n", myString.c_str());   
                //if (gl == false)
        		  if((gl == 0) && ( send(sockfd , myString.c_str(), strlen(myString.c_str()),0 )<0)) {
        			 cout<<"Send failed";
        		  }
        		glPopMatrix();
        	}
        	count++;
        }
    }

    fd.flush();


    // we need to swap the buffer to display our drawing.
    glutSwapBuffers();
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y)
{
    /* sleep to avoid thrashing this procedure */
	usleep(100);

    /* If escape is pressed, kill everything. */
	if (key == ESCAPE)
	{
        /* shut down our window */
		glutDestroyWindow(window);

        /* exit the program...normal termination. */
		exit(0);
	}
}




int main(int argc, char **argv)
{

	flockDisplay=NULL;
	sceneDisplay=NULL;
	positionFile.open("Position.txt");
	XInitThreads();
    std::thread simThread(simMain,argc,argv); // Start the sim thread
    gl = atoi(argv[16]);
    printf("argv %s\n", argv[15]);
    printf("printing gl! %d\n", gl);


    /* Initialize GLUT state _ glut will take any command line arguments that pertain to it or
       X Windows _ look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */
    glutInit(&argc, argv);

     // Select type of Display mode:
     //   Double buffer
     //   RGBA color
     //   Alpha components supported
     //   Depth buffer 
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

    /* get a 640 x 480 window */
    glutInitWindowSize(800,800);

    /* the window starst at the upper left corner of the screen */
    glutInitWindowPosition(0, 0);

    /* Open a window */
    window = glutCreateWindow("DWA Hacks Test Viewer");

    /*call socket initialization functions*/
    if (gl == 0){
        printf("Initializing socket\n");
        initSocket();
    }
    
    /* Register the function to do all our OpenGL drawing. */
    glutDisplayFunc(&DrawGLScene);

    /* Go fullscreen.  This is as soon as possible. */
    //glutFullScreen();

    /* Even if there are no events, redraw our gl scene. */
    glutIdleFunc(&DrawGLScene);

    /* Register the function called when our window is resized. */
    glutReshapeFunc(&ReSizeGLScene);

    /* Register the function called when the keyboard is pressed. */
    glutKeyboardFunc(&keyPressed);

    /* Initialize our window. */
    InitGL(640, 480);

    /* Start Event Processing Engine */
    glutMainLoop();

    return 1;
}

