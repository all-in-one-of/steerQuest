
#include <vector>

class DispFish
{
public:
    DispFish(int fid,float fx,float fy,float forientation)
    :id(fid) ,
     x(fx) ,
     y(fy) ,
     orientation(forientation)
     {}


    int id;
    float x,y;
    float orientation;

};


extern std::vector<DispFish> display_fishes;
