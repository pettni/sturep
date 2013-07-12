#ifndef WOR_H
#define WOR_H

#include "../actor.h"

namespace spel {

class Worker : public Actor {

public:
    Worker(string);
    Worker(string, unique_ptr<Object> &, unique_ptr<Object> &);
    
    virtual void action();
    virtual void get(unique_ptr<Object> &);
   
    virtual void save(ofstream & fs, int &, const int &) const;
private:
};

} //end of namespace

#endif