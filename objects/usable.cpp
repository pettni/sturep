#include "usable.h"

#include "../actor.h"

namespace spel {

Usable::Usable(string s) : Object(s), container(nullptr) { 
    action_type = "använde";
//    cout << "usable constructor 1: " << s << endl;     
    }

Usable::Usable(const Usable & orig) : Object(orig) {
    if (orig.container)
        container = orig.container->clone();
}

Usable::Usable(string s, unique_ptr<Object> & cont) : Object(s), container(move(cont)) { 
//    cout << "usable constructor 2: " << s << endl;
    cont.reset();
    action_type = "använde"; }

unique_ptr<Object> Usable::action(Actor * target) {
    unique_ptr<Object> ret = nullptr;
    target->affect_hp( nullptr, action1);
    target->affect_promille(action2);
    if (container)
        ret = move(container);
    return move(ret);
}

Usable::~Usable() {
    container.reset();
//    cout << "usable destructor: " << Name << endl;
}

void Usable::save(ofstream & fs, int & obj_cnt) const {
    stringstream c_s;
    if (container) {
        container->save(fs, obj_cnt);
        c_s << ":OBJECT" << obj_cnt-1;
    } else {
        c_s << ":";
    }

    fs << "OBJECT" << obj_cnt++ << 
        ":USABLE:" << 
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
        c_s.str() <<
        endl;
}


} //end of namespace

