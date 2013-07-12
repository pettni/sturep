#ifndef ACT_H
#define ACT_H

#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>
#include <memory>
#include <sstream>
#include <typeinfo>

#include "objects/container.h"
#include "help_functions.h"
#include "controller.h"

using namespace std;
namespace spel {

class Object;
class Environment;
class Container;

class Actor {

    friend void connect(Environment * p, Actor * a);
    friend void set_actor_info(const vector<string> &, map<string, unique_ptr<Object>> &, Actor *);
    friend class Controller;

public:    
    Actor(string);
    Actor(string, unique_ptr<Object> &, unique_ptr<Object> &);
    virtual ~Actor();
    
    string main_description() const;
    virtual string description() const;
    
    // Action
    virtual void action() { return; }

    void affect_hp(Actor *, double);
    virtual void get(unique_ptr<Object> &);

    void refuse() const { cout << Name << ": Nej tack." << endl; }
    
    // Information getters and setters
    double get_hp() const { return hp; }
    string get_id() const { return id; }
    string get_Name() const { return Name; }
    string get_type() const { return lowercase(Type); }
    string get_Type() const { return Type; }
    Environment * get_location() const { return location; }
    string get_prep() const { return lowercase(Prep); }     
    string get_Prep() const { return Prep; }
    bool get_angry() const { return angry; }
    Actor* get_enemy() const {return enemy; }

    void set_type(const string & s) { Type = s; }
    void set_hp(double d) { hp = d; }
    void set_action_lambda(function<int(void)> al) { action_lambda = al; }
    void set_get_lambda(function<int(unique_ptr<Object> &)> al) { get_lambda = al; }
    void set_death_lambda(function<void(void)> al) { death_lambda = al; }
    void set_Prep(const string & s)  { Prep = s; }
    void set_money(const double & d) { money = d; }
    void add_answer(const string & s ) { answers.push_back(s); }
        
    double get_promille() const {return promille; }
    void set_promille(double d) { promille = d; }
    void affect_promille(double);       
    
    virtual void save(ofstream &, int &, const int &) const = 0;
    string save_objects(ofstream &, int &) const;
    
    virtual void answer();

protected:
    // Actions    
    void drop();
    void pick_up(unique_ptr<Object> &);
    void put_on(unique_ptr<Container> &);
    void put_in_pocket(unique_ptr<Object> &);
    void attack(Actor *);
    void manual_attack(Actor *);
    void take_money(unique_ptr<Object> & mm);
    unique_ptr<Object> create_money(int);
    void give(unique_ptr<Object> &, Actor *) const;
    void use();
    
    virtual void death();
    
    // AI
    void look_for(const type_info &);

    // Attributes
    Environment * location; 
    unique_ptr<Object> hand;
    Container pocket;
    unique_ptr<Container> carriable;
    vector<string> answers;
    
    // Properties
    string id, Name, Type, Prep;
    double hp;
    double promille;
    int money;
    
    function<int(void)> action_lambda;
    function<int(unique_ptr<Object> &)> get_lambda;
    function<void(void)> death_lambda;
    
    bool angry;
    Actor * enemy;
    
    int lifetime;
};

} // End of namespace

#endif
