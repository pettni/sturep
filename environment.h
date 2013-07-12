#ifndef ENV_H
#define ENV_H

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <random>
#include <chrono>
#include <memory>
#include <typeinfo>

#include "object.h"

#include "help_functions.h"

using namespace std;

namespace spel {

class Object;
class Actor;

class Environment {

    friend void connect(Environment *, string, Environment *, string);
    friend void connect_oneway(Environment *, string, Environment *);
    friend Environment * create_environment(const vector<string> &, map<string,unique_ptr<Object>> &);
    friend class Controller;
    
public:
    Environment(const string &);
    
    virtual ~Environment() {
//        cout << "environment destructor: " << Name << endl;
    }
    
    string visitors_description() const;
    string objects_description() const;
    string exits_description() const;
    
    virtual string description() const;
    virtual string short_description() const;
    
    // Objects
    virtual void add_object(unique_ptr<Object> &);
    virtual void add_object(unique_ptr<Object> &, const string &);  
    
    int number_of_objects() const;
    int find_object(const string &) const;  
    int find_type(const type_info &) const;  
    const unique_ptr<Object> & look_at_object(int i) const;
    const string & look_at_object_place(int i) const;
    unique_ptr<Object> release_object(const string &);  
    unique_ptr<Object> release_object(int);  

    // Neighbor environments
    virtual Environment * find_exit(const string &) const;
    Environment * random_exit() const;
    
    // Visitors
    Actor * find_visitor(const string &) const;
    void remove_visitor(Actor *);
    void add_visitor(Actor *);
    Actor * random_visitor() const;
    Actor * random_other_visitor(Actor *) const;
    vector<Actor *> all_visitors_of_type(const type_info &) const;
        
    // Information getters and setters
    string get_id() const { return id; }
    string get_Name() const { return Name; }
    string get_prep() const { return lowercase(Prep); }
    string get_Prep() const { return Prep; }
    string get_drop_location() const { return lowercase(Drop_location); }
    string get_Drop_location() const { return Drop_location; }

    void set_Prep(const string & s) { Prep = s; }
    void set_Drop_location(const string & s) { Drop_location = s; }

    virtual void save(ofstream &, int &, const int &) const;
    string save_objects(ofstream &, int &) const;

protected:  
    string id, Name, Prep;
    string Drop_location;

    // Visitors array
    vector<Actor *> visitors;
    // Neighbors   
    map<string, Environment*> neighbors;
    // Objects
    vector<pair< unique_ptr<Object>, string> > objects;
};

void connect(Environment *, string, Environment *, string);

void connect_oneway(Environment *, string, Environment *);

void connect(Environment *, Actor *);
 
} //End of namespace

#endif
