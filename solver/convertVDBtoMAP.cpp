#include <openvdb/openvdb.h>
#include <stdlib.h>
#include <fstream>
using namespace std;


int main(int argc,char** argv)
{
    // If the argument isn't give, display the helpful message
    if(argc == 1) {
        printf("Usage: %s <vdb>\n", argv[0]);
        return;
    }
    char* filename = argv[1];

    openvdb::initialize();

    // Create a VDB file object.
    openvdb::io::File file(filename);

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
    int z_bound = 100;
    int y_bound = 1;

    float grid_array[x_bound][z_bound];

    for(int i=0;i<x_bound;i++)
        for(int k=0;k<z_bound;k++) {
            grid_array[i][k]=-999;
        }

    for (openvdb::FloatGrid::ValueOnCIter iter = grid->cbeginValueOn(); iter; ++iter) {
        openvdb::Coord pos = iter.getCoord();

        if((pos.x()>=x_bound||pos.z()>=z_bound||pos.x()<0||pos.z()<0)||pos.y()!=y_bound) {
            continue;
        }

        grid_array[pos.x()][pos.z()]=*iter;
    }


    ofstream outMapFile;
    std::string outFileName = std::string(filename)+".map";
        outMapFile.open (outFileName);
        
        for(int x =x_bound-1;x>=0;x-=1) {
            for(int z =0;z<z_bound;z+=1) {	
                if(grid_array[x][z]<=0&&grid_array[x][z]!=-999)
                    outMapFile<<"#";
                else
                    outMapFile<<".";
                
                //printf("%0.1f",grid_array[x][z]);
                
            }
            outMapFile<<"\n";
                //printf("\n");
        }
        
        outMapFile.close();
}
