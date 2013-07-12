#include "indoor.h"

namespace spel {

Indoor::Indoor(string s1) : Environment(s1) {
    Drop_location = "På golvet";
};   

void Indoor::save(ofstream & fs, int & obj_num, const int & env_num) const {
    string objects = Environment::save_objects(fs, obj_num);
    fs << "ENVIRONMENT" << env_num << ":" <<
        "INDOOR" << ":" <<
        Name << ":" << 
        Prep << ":" << 
        Drop_location << 
        objects << 
        endl;
}



} // end of namespace
