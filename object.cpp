#include "object.h"

namespace spel {

	Object::Object(string n) : id(n), Name(n), Prep("ligger"), action1(0), action2(0) {
		id = format(id);    
	}

	string Object::main_description() const {
		stringstream s;
		s << get_Definitive() << " är " << article << " " << Name << " som väger " << weight << "kg och upptar " 
		  << volume << " liter. Den är värd " << price << "kr.";
		return s.str();
	}

	string Object::description() const {
		return main_description();
	}

void Object::save(ofstream & fs, int & obj_cnt) const {
    fs << "OBJECT" << obj_cnt++ << 
        ":OBJECT:" << 
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
        action2 << 
        endl;
}



} // end of namespace
