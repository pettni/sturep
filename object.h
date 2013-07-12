#ifndef OBJ_H
#define OBJ_H

#include <memory>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

#include "help_functions.h"

using namespace std;
namespace spel {

class Object {

public:
    
    Object(string);
    Object (const Object &) = default;
    Object & operator=(const Object &) = delete;
    
    virtual unique_ptr<Object> clone() const { return move(unique_ptr<Object>(new Object(*this))); }
    virtual ~Object() { 
    //cout << "object destructor: " << Name << endl; 
    } 
    
    // Exempel
    // Name        id        article  type    definitive  action_type
    // Sig Sauer   sigsauer    en       pistol  pistolen    sköt
    //                                                                   
    
    string get_id() const { return id; }
    string get_Name() const { return Name; }
    string get_article() const { return article; }
    string get_type() const { return type; }
    string get_arttype() const { return article + " " +type; }
    string get_action_type() const { return action_type; }
    string get_definitive() const { return lowercase(Definitive); }     
    string get_Definitive() const { return Definitive; }
    string get_prep() const { return lowercase(Prep); }     
    string get_Prep() const { return Prep; }

    double get_weight() const { return weight; }
    double get_volume() const { return volume; }
    double get_price() const { return price; }
    
    void set_info(double w, double v, double p) { weight = w; volume = v; price = p; }
    void set_type(const string & s1, const string & s2, const string & s3 ) { article = s1; type = s2; Definitive = s3; }
    void set_Prep(const string & s) { Prep = s; }
    void set_action(double s1, double s2) { action1 = s1; action2 = s2;}
    void set_action_type(const string & s) { action_type = s;}
    
    virtual string description() const;
    
    virtual void save(ofstream &, int &) const;
    
protected:
    string main_description() const;
    
    string id, Name, article, type, Definitive, action_type, Prep;
    double weight, volume, price;  
    
    double action1, action2;  
};

} // end of namespace

#endif
