#include "Flocking.h"

int Flocking::update()
{
    int i;
    Vec2f centroid(0,0);
    for(i = 0; i < boids.size(); i++)
    {
    	centroid+=boids[i].loc;

        if(boids[i].isHit(destination.x,destination.y,destinationArea))
        {
            boids[i].reachedDestination = true;
        }

        if(useCollisionFromSDF)
        {
            Vec2f dir = partialDerivaties[(int)boids[i].loc.x][(int)boids[i].loc.y];

            float val = collisionSDF[(int)boids[i].loc.x][(int)boids[i].loc.y];
            
            if(val==0)
            	val=0.001;

			boids[i].seek(dir+boids[i].loc,dir.length()*collisionWeight/val);

        }

	
        if(sceneMap->getCell(boids[i].loc.x,boids[i].loc.y))
        {
        /*	
            vector<Vec2f> path;
            path = pathFinder.getPath(sceneMap,boids[i].loc);
            if(path.size()>1)
            {
                Vec2f dest = path[min((int)path.size()-1,10)]; //!@#
                boids[i].seek(dest,destWeight); //seek the Goal !@#
            }
	*/		

            
        }
        else
        {
            boids[i].hitObstacle = true;
        }


        //boids[i].seek(destinationSeek,destWeight); //seek the Goal !@#
	       boids[i].seek(destination,destWeight); //seek the Goal !@#
        boids[i].update(boids);

        if(boids[i].reachedDestination)
            removeBoid(boids[i].loc.x,boids[i].loc.y,1);  //ineffcient way to remove boids

    }

    centroid /=flockSize();

    /*
    if(sceneMap->getCell(centroid.x,centroid.y))
    	{
        	
            vector<Vec2f> path;
            path = pathFinder.getPath(sceneMap,centroid);
            if(path.size()>1)
            {
                destinationSeek = path[min((int)path.size()-1,5)]; 
                
            }
			

            
        }
*/
        

    if(flockSize()==0)
        return 0;
    else
        return 1;
}


void Flocking::addBoid(int x, int y)
{
    boids.push_back(Boid(x, y, x_bound, y_bound, boundaryPadding, 
    											maxSpeed 		,
												maxForce 		,
												flockSepWeight	,
												flockAliWeight	,
												flockCohWeight	,
												flockSepRadius	,
												flockAliRadius	,
												flockCohRadius	));
}

void Flocking::removeBoid(int x, int y, int radius)
{
    int i;
    for (i=0; i<boids.size(); i++)
    {
        if(boids[i].isHit(x, y, radius))
        {
            boids.erase(boids.begin()+i);
        }
    }
}

int Flocking::flockSize()
{
    return boids.size();
}


void Flocking::setBounds(int x, int y)
{
    x_bound = x;
    y_bound = y;
}


void Flocking::setDestination(int x, int y,float area)
{
    destination.setval((float)x,(float)y);
    destinationArea = area;
    destinationSeek=destination;
}


void Flocking::setDestination(Vec2f dest,float area)
{
    destination=dest;
    destinationArea=area;
}


void Flocking::setSceneMap(Scene* scene)
{
    sceneMap = scene;
}

void Flocking::useCollisionSDF(bool val)
{
    useCollisionFromSDF = val;

    if(useCollisionFromSDF)
    {
        collisionSDF = sceneMap->getSDFhandle();
    }
}

vector<Boid> Flocking::getBoids()
{
    return boids;
}

Vec2f** Flocking::calculatePartialDerivaties()
{
	partialDerivaties = (Vec2f**)malloc(sizeof(Vec2f*) * x_bound);
	for(int i=0;i<x_bound;i++)
	{
		partialDerivaties[i]=(Vec2f*)malloc(sizeof(Vec2f*) * y_bound);
		memset(partialDerivaties[i],0,y_bound);
	}

	int padding=1;

	for(int i=padding;i<x_bound-padding;i++)
	{
		for(int j=padding;j<y_bound-padding;j++)
		{
			float dify=0;
			if(collisionSDF[i][j]==-999||collisionSDF[i][j-padding]==-999)
				dify=0;
			else
				dify=collisionSDF[i][j]-collisionSDF[i][j-padding];

			float difx=0;
			if(collisionSDF[i][j]==-999||collisionSDF[i-padding][j]==-999)
				difx=0;
			else
				difx=collisionSDF[i][j]-collisionSDF[i-padding][j];

			partialDerivaties[i][j].setval(difx,dify);

			
			
		}
	}
	



	return partialDerivaties;
		
}



void Flocking::setSimulationParameters(
    		int 	mboundaryPadding 	,
            float 	mmaxSpeed 			,
            float 	mmaxForce 			,
          	float 	mflockSepWeight 	,
          	float 	mflockAliWeight 	,
          	float 	mflockCohWeight 	,
          	float 	mcollisionWeight 	,
          	float 	mflockSepRadius 	,
          	float 	mflockAliRadius 	,
          	float 	mflockCohRadius 	,
          	float 	mdestWeight 		)
{
	boundaryPadding 	=     mboundaryPadding 	;
	maxSpeed 			=     mmaxSpeed 		;	
	maxForce 			=     mmaxForce 		;	
	flockSepWeight     	=     mflockSepWeight	;
	flockAliWeight     	=     mflockAliWeight	;
	flockCohWeight     	=     mflockCohWeight	;
	collisionWeight 	=     mcollisionWeight	; 	
	flockSepRadius     	=     mflockSepRadius	;
	flockAliRadius     	=     mflockAliRadius	;
	flockCohRadius     	=     mflockCohRadius	;
	destWeight 			=     mdestWeight 		;
}
