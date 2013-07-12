#ifndef MAC_H
#define MAC_H

#include <typeinfo>

#include "visitor.h"

namespace spel {

class Main_Actor : public Visitor {
    
    friend void look_for_object(Main_Actor *, const string &); 

public:
    Main_Actor(std::string);
    bool look_wrapper(string & s) { return look(s); }
    bool pick_up(string &);
    bool put(string &);
    bool drop(string &);
    bool use(string &);
    bool go(string &);   
    bool give(string &);
    bool buy(string &);
    bool manual_attack(string &);
    bool talk_to(string &);
    
    bool check_win() const;
    
    void transport_to(Environment * env); 

    virtual void save(ofstream & fs, int &, const int &) const;    

protected:
    virtual void death();
    bool look(string &) const;        
};

} //end of namespace

#endif
