#ifndef VIS_H
#define VIS_H

#include <random>
#include <chrono>
#include <algorithm>

#include "../actor.h"

namespace spel {

class Salesman;

class Visitor : public Actor {

    friend class Controller;
   
public:
    Visitor(string);
    Visitor(string, unique_ptr<Object> &, unique_ptr<Object> &);

    virtual string description() const;
     
    virtual void action();
    void buy(Salesman *);
    void buy(const type_info &);            
    virtual void save(ofstream & fs, int &, const int &) const;
   
protected:
    void go(Environment *);
};

} //end of namespace

#endif
