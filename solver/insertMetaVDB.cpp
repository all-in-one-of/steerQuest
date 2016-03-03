#include <openvdb/openvdb.h>
#include <stdlib.h>
#include <fstream>
using namespace std;


int main(int argc,char** argv)
{
    // If the argument isn't give, display the helpful message
    if(argc <4 ) {
        printf("Usage: %s <in_vdb> <out_vdb> <metadata>\n", argv[0]);
        return 1;
    }
    char* in_vdbfilename = argv[1];
    char* out_vdbfilename = argv[2];
    char* metadata_filename = argv[3];

    openvdb::initialize();

    // Create a VDB file object.
    openvdb::io::File invdbfile(in_vdbfilename);
    openvdb::io::File outvdbfile(out_vdbfilename);
    FILE *metaFile = fopen(metadata_filename, "r");
    invdbfile.open();

    openvdb::GridBase::Ptr baseGrid=invdbfile.readGrid("SDF");

    fseek(metaFile, 0, SEEK_SET);
    char line[1024];

    std::cout<<"\nWriting Metadata:\n";
    while(fgets(line, 1024, metaFile))
    {
        string data(line);
        int sep = data.find(":");
        if(sep==0)
        {
            printf("\nNo ':' separator found\n");
            return 1;
        }

        string item=data.substr(0,sep);
        string value=data.substr(sep+1);

        baseGrid->insertMeta(item, openvdb::StringMetadata(value));

        std::cout<<item<<" : "<<value<<std::endl;
    }



    openvdb::GridPtrVec grids;
    grids.push_back(baseGrid);

    // Write out the contents of the container.
    outvdbfile.write(grids);

    invdbfile.close();
    outvdbfile.close();
    fclose(metaFile);

    return 0;

}

