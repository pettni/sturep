#ifndef SALES_H
#define SALES_H

#include "worker.h"

namespace spel {

class Salesman : public Worker {

public:    
    Salesman(string,unique_ptr<Object> &,string);
    virtual ~Salesman();

    
    double get_price() const { return merch->get_price(); }
        
    virtual void get(unique_ptr<Object> &);
    const unique_ptr<Object> & look_at_merch() const { return merch; }
    
    void save(ofstream &, int &, const int &) const;
    virtual void answer();
    
protected:
    string Drop_location;
    unique_ptr<Object> merch;

};
} // End of namespace

#endif
