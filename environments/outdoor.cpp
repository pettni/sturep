#include "outdoor.h"

namespace spel {

Outdoor::Outdoor(const string & s1) : Environment(s1) {
    Drop_location = "på marken";
}

void Outdoor::save(ofstream & fs, int & obj_num, const int & env_num) const {
    string objects = Environment::save_objects(fs, obj_num);
    fs << "ENVIRONMENT" << env_num << ":" <<
        "OUTDOOR" << ":" <<
        Name << ":" << 
        Prep << ":" << 
        Drop_location << 
        objects << 
        endl;
}


} //end of namespace
