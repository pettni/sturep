#include "salesman.h"
#include "../objects/money.h"
#include "../environment.h"

namespace spel {

Salesman::Salesman(string n,unique_ptr<Object> & obj,string d) : Worker(n), merch(move(obj)), 
                    Drop_location(d) {
                        obj.reset();                    
                    }
Salesman::~Salesman() {
//    cout << "salesman destructor: " << Name << endl;
    merch.reset();
}
// Salesman::Salesman(string n, unique_ptr<Object> & o1, unique_ptr<Object> & o2, unique_ptr<Object> & obj) : 
//           Worker(n,o1,o2), merch(move(obj)) {}

void Salesman::get(unique_ptr<Object> & obj) {
    if (obj && typeid(*obj) == typeid(Money) && obj->get_price() >= get_price()) {
        unique_ptr<Object> new_merch(move(merch->clone()));
        get_location()->add_object(new_merch,Drop_location);        
    }
    else 
        cout << Name << ": Nej tack." << endl;
}

void Salesman::save(ofstream & fs, int & obj_num, const int & act_num) const {
    merch->save(fs,obj_num);
    int merch_num = obj_num-1;
    string objects = Actor::save_objects(fs, obj_num);

    fs << "ACTOR" << act_num <<
        ":SALESMAN:"  <<
        Type << ":" <<
        Name << ":" <<
        Prep << ":" <<
        hp << ":" <<
        money << ":" <<
        angry << ":" <<
        lifetime <<
        objects << ":" <<
        join(answers) <<
        ":OBJECT" << merch_num << ":" <<
        Drop_location << endl;
}

void Salesman::answer() {
    Actor::answer();
    cout << "Ska du ha en " << merch->get_arttype() << "? Det kostar " << get_price() << " spänn." << endl;
}


} //End of namespace
