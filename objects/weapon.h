#ifndef WEP_H
#define WEP_H

#include <vector>
#include <random>

#include "../object.h"

namespace spel {

class Actor;

class Weapon : public Object {
public:
    Weapon(string);
    virtual ~Weapon() { 
//         cout << "weapon destructor: " << Name << endl; 
}
    Weapon & operator=(const Weapon &) = delete;
    virtual unique_ptr<Object> clone() const { return move(unique_ptr<Object>(new Weapon(*this))); }    
    double action();
    virtual string description() const;


    virtual void save(ofstream &, int &) const;
    
private:
    std::mt19937 gen;
    weibull_distribution<double> dist;    
};


} // End of namespace

#endif