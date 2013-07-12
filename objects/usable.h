#ifndef USA_H
#define USA_H

#include "../object.h"
#include "../actor.h"

namespace spel {

class Visitor;

class Usable : public Object {
public:
    Usable(string);
    Usable(string, unique_ptr<Object> &);    
    Usable(const Usable &);
    virtual ~Usable();
    
    Usable & operator=(const Usable &) = delete;
    
    virtual unique_ptr<Object> clone() const { 
        unique_ptr<Object> clone(new Usable(*this));
        return move(clone); 
    }
    
    unique_ptr<Object> action(Actor *);
    
    virtual void save(ofstream &, int &) const;

private:
    unique_ptr<Object> container;
};

} // end of namespace

#endif