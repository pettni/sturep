#include "../environment.h"
#include "../actor.h"

#ifndef IND_H
#define IND_H

namespace spel {

class Indoor : public Environment {
    public:
    Indoor(string);    

    virtual void save(ofstream &, int &, const int &) const;


};

} //end of namespace

#endif