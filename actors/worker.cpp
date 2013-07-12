#include "worker.h"

#include "../objects/weapon.h"
#include "../objects/usable.h"

namespace spel {

Worker::Worker(string n) : Actor(n) {}

Worker::Worker(string n, unique_ptr<Object> & o1, unique_ptr<Object> & o2) : Actor(n, o1, o2) {}

void Worker::action() {
    ++lifetime;
    if (hp == 0)
        return;
    
    int choice = 0;
    if(action_lambda) {
        choice = action_lambda();    
    }
    
    //cout << Name << " valde " << choice << endl;
    
    switch (choice) {
        case 0 : { break; }
        case 4 : { 
            if ( (!hand) || (typeid(*(hand))!=typeid(Weapon)) ) {
                look_for(typeid(Weapon));
            }   
            if (enemy && enemy->get_location() == location) {
                attack(enemy);
            }
            break; 
        }
    }
}

void Worker::get(unique_ptr<Object> & usb) {
    Actor::get(usb);
}

void Worker::save(ofstream & fs, int & obj_num, const int & act_num) const {
    string objects = Actor::save_objects(fs, obj_num);

        fs << "ACTOR" << act_num <<
            ":WORKER:"  <<
            Type << ":" <<
            Name << ":" <<
            Prep << ":" <<
            hp << ":" <<
            money << ":" <<
            angry << ":" <<
            lifetime << 
            objects << ":" <<
            join(answers) << endl;
}

} // End of namespace
