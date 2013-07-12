#ifndef CON_H
#define CON_H

#include <vector>
#include <sstream>
#include <iomanip>
#include <typeinfo>
#include <stdexcept>

#include "../object.h"

using namespace std;
namespace spel {

class Container : public Object {

public:
    Container(string,double,double);
    virtual unique_ptr<Object> clone() const;
    virtual ~Container();

    Container & operator=(Container &&);
    
    bool fits(const unique_ptr<Object> &);  
    bool add_object(unique_ptr<Object> &);
    
    int number_of_objects() const { return objects.size(); }
    const unique_ptr<Object> & look_at_object(int) const;
    int find_object(string) const;  
    int find_type(const type_info &) const;
    unique_ptr<Object> release_object(string);  
    unique_ptr<Object> release_object(int);
    
    virtual string description() const;
    
    virtual void save(ofstream &, int &) const;
    
private:
    vector<unique_ptr<Object>> objects;
    double weight_cap, vol_cap, weight_free, vol_free; 
};


} // End of namespace

#endif
