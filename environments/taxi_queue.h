#ifndef TAXI_H
#define TAXI_H

#include "outdoor.h"

namespace spel {
class Taxi_Queue : public Outdoor {

    public:
    Taxi_Queue(const string &);
    
    virtual void save(ofstream &, int &, const int &) const;

    
    protected:
    


};
} // end of namespace

#endif