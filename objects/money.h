#ifndef MON_H
#define MON_H

#include "../object.h"

namespace spel {

class Money : public Object {
public :
    Money(int);
    virtual unique_ptr<Object> clone() const { return move(unique_ptr<Object>(new Money(*this))); }    
    virtual void save(ofstream &, int &) const;
};

} //end of namespace

#endif