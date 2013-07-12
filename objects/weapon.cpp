#include "weapon.h"

#include "../actor.h"

namespace spel {

Weapon::Weapon(string n) : Object(n) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    gen = mt19937(seed);
//     cout << "weapon constructor: " << n << endl;
}

string Weapon::description() const {
    return main_description() + " Den är farlig.";
}

double Weapon::action() {
    dist = weibull_distribution<double>(5.,action1/0.91);
    double damage = floor(dist(gen));
    return damage;
}

void Weapon::save(ofstream & fs, int & obj_cnt) const {
    fs << "OBJECT" << obj_cnt++ << 
        ":WEAPON:" << 
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


} // End of namespace
