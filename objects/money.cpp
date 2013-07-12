#include "money.h"

namespace spel {

Money::Money(int i) : Object(to_string(i) + " kronor") {
    set_type("en bunt", "pengar", "Pengarna");
    set_info(double(i)/50000. , double(i)*0.0011/50, double(i));
}

void Money::save(ofstream & fs, int & obj_cnt) const {
    fs << "OBJECT" << obj_cnt++ << 
        ":MONEY:" << 
        Name << ":" << 
        article << ":" << 
        type << ":" << 
        Definitive << ":" << 
        action_type << ":" << 
        Prep << ":" << 
        weight << ":" << 
        volume << ":" << 
        price << ":" << 
        action1 << ":" << 
        action2 << ":" <<
        endl;
}


} // end of namespace