#ifndef OUT_H
#define OUT_H

#include "../environment.h"

namespace spel {

class Outdoor : public Environment {
    public:
    Outdoor(const string &);
    virtual void save(ofstream &, int &, const int &) const;


};


} // end of namespace

#endif