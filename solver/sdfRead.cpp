#include <openvdb/openvdb.h>
#include <stdlib.h>

int main(int argv,char** argc)
{

openvdb::initialize();
// Create a VDB file object.
openvdb::io::File file("maps/map1_obstacles.vdb");
// Open the file. This reads the file header, but not any grids.
file.open();
// Loop over all grids in the file and retrieve a shared pointer
// to the one named "LevelSetSphere". (This can also be done
// more simply by calling file.readGrid("LevelSetSphere").)
openvdb::GridBase::Ptr baseGrid;
baseGrid = file.readGrid("SDF");
file.close();

//cast the generic grid pointer to a FloatGrid pointer.
openvdb::FloatGrid::Ptr grid = openvdb::gridPtrCast<openvdb::FloatGrid>(baseGrid);

int x_bound = 100;
int y_bound = 10;
int z_bound = 100;

float grid_array[x_bound][y_bound][z_bound];

for(int i=0;i<x_bound;i++)
for(int j=0;j<y_bound;j++)
for(int k=0;k<z_bound;k++)
{
	grid_array[i][j][k]=-999;
}

for (openvdb::FloatGrid::ValueOnCIter iter = grid->cbeginValueOn(); iter; ++iter) {
//std::cout << "Grid" << iter.getCoord() << " = " << *iter << std::endl;

openvdb::Coord pos = iter.getCoord();

if(pos.x()>=x_bound||pos.y()>=y_bound||pos.z()>=z_bound||pos.x()<0||pos.y()<0||pos.z()<0)
{
	//grid_array[pos.x()][pos.y()][pos.z()]=-999;
	continue;
}

grid_array[pos.x()][pos.y()][pos.z()]=*iter;


}



//Display Grid
int y = atoi(argc[1]);
for(int x =x_bound-1;x>=0;x-=1)
{
	for(int z =0;z<z_bound;z+=1)
	{	
		float val;
		
		//if(grid_array[x][y][z]<0)
			
			printf("%.1f  ",grid_array[x][y][z]);
		//else

		//	printf("   ");

	}
	std::cout<<"\n";
}


}
