#include "actor.h"

#include "environment.h"
#include "objects/weapon.h"
#include "objects/money.h"
#include "objects/usable.h"

namespace spel {

    Actor::Actor(string s) : id(s), Name(s), hp(100.), promille(0), pocket("ficka", 1, 0.5), 
                             lifetime(0), enemy(nullptr) 
    { 
        id = format(id);
        pocket.set_type("en","liten ficka","Fickan");
        pocket.set_info(0,0,0);
        Prep = "Står";
    }

    Actor::Actor(string s, unique_ptr<Object> & h, unique_ptr<Object> & p) : id(s), Name(s), 
                                            hp(100.), promille(0), pocket("ficka", 1, 0.5), 
                                            lifetime(0), enemy(nullptr) {
        if (h)
            if (typeid(h) == typeid(Container)) {
                unique_ptr<Container> cont(static_cast<Container *>(h.release()));
                h.reset();
                put_on(cont);
            } else {
                hand = move(h);
            }
        if (p && pocket.fits(p))
            pocket.add_object(p);
        else
            p.reset();
    }

    Actor::~Actor() {
        if (hand)
            hand.reset();
        if (carriable) {
            carriable.reset();
        }
    }

    string Actor::description() const {
        return main_description();
    }

    string Actor::main_description() const {
        stringstream s;
        s << Type << " " << Name << " har " << hp << " hp";
        string hand_obj;
        (hand) ? hand_obj = hand->get_arttype() : hand_obj="inget";
        s << " och " << hand_obj << " i sin hand.";
        return s.str();
    }

    void Actor::affect_hp(Actor * attacker, double effect) {
        if (hp == 0)
            return;
        hp += effect;
        if (effect <= 0) {
            enemy = attacker;
            angry = true;
        }

        if (hp <= 0) {
            hp = 0;
            cout << typeid(*this).name() << endl;
            this->death();
        }
        if (hp > 100) {
            hp = 100;
        }  
    }

    void Actor::pick_up(unique_ptr<Object> & o) {
        if (typeid(*o) == typeid(Money)) {
            take_money(o);
            return;
        }
        if (hand)
            put_in_pocket(hand);
        if (hand)
            drop();  
        if (o)      
            hand = move(o);
    }

    void Actor::answer() { 
        if (!answers.empty()) { 
            random_shuffle(answers.begin(), answers.end()); 
            cout << Name << ": " << answers[0] << endl;  
        }
    }

    void Actor::attack(Actor * target) {

        unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
        mt19937 gen (seed1);
        double success_proba = 1-promille/5;
        bernoulli_distribution dist(success_proba);
        
        if (hand) {
            if (typeid(*hand) == typeid(Weapon)) {
                unique_ptr<Weapon> wpn(static_cast<Weapon *>(hand.release()));
                hand.reset();
                if (dist(gen)) {
                    double damage = wpn->action();
                    cout << Name << " " << wpn->get_action_type() << " " 
                         << target->get_Name() << " med " << wpn->get_article() 
                         <<  " " << wpn->get_Name() << " och orsakade " <<
                        damage << " skada." << endl;
                    target->affect_hp(this, -damage);
                } else {
                    cout << Name << " " << wpn->get_action_type() << " mot " 
                         << target->get_Name() << " med " << wpn->get_article() 
                         <<  " " << wpn->get_Name() << " men missade. " << endl;
                    target->affect_hp(this,0);
                }
                hand = move(wpn);        
            }
        } else {
            manual_attack(target);
        }
    }

    void Actor::manual_attack(Actor * target) {
        unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
        mt19937 gen (seed1);
        double success_proba = 1-promille/5;
        bernoulli_distribution dist(success_proba);

        if (dist(gen)) {
            cout << Name << " slog " << target->get_Name() << " med sin knytnäve och orsakade " << 0.05*hp << " skada." << endl;
            target->affect_hp(this, -0.05*hp);
        } else {
            cout << Name << " försökte slå " << target->get_Name() << " men var för full." << endl;
            target->affect_hp(this,0);
        }

    }

    void Actor::drop() {
        if (hand)
            location->add_object(hand);
    }

    void Actor::put_on(unique_ptr<Container> & bag) {
        if (carriable) {
            unique_ptr<Object> old_bag(static_cast<Object *>(carriable.release()));
            carriable.reset();
            location->add_object(old_bag);
        }
        carriable = move(bag);
        bag = nullptr;
    }

    void Actor::take_money(unique_ptr<Object> & mm) {
        if (typeid(*mm) == typeid(Money)) {
            money += mm->get_price();
            mm.reset();
        }
    }

    unique_ptr<Object> Actor::create_money(int i) {
        unique_ptr<Object> ret = nullptr;
        if (money>=i) {
            ret = unique_ptr<Object>(new Money(i));
            money -= i;
        }   
        return move(ret);
    }

    void Actor::give(unique_ptr<Object> & obj, Actor * receiver) const {
        if (receiver)
            receiver->get(obj);
    }
    
    void Actor::get(unique_ptr<Object> & obj) {
        int choice = 0;
        if (get_lambda)
            choice = get_lambda(obj);
        
        // Get lambda choice:
        // 0: refuse
        // 1: accept and put in pocket. Stop being angry
        // 2: accept and use. Stop being angry
        // 3: accept and give object in pocket in return
        
        switch (choice) {
            case 0: { 
                cout << Name << ": Nej tack." << endl;
                break;
            }       
            case 1: { 
                if (typeid(*obj) == typeid(Money)) {
                    take_money(obj);                    
                }
                else {
                    put_in_pocket(obj);                    
                }
                angry = false;
                break; 
            }
            case 2: {             
                pick_up(obj);
                use();
                angry = false;                    
                break;
            }
            case 3: {
                pick_up(obj);
                use();
                angry = false;
                look_for(typeid(Object));
                drop();
            }
        }
    }
    
    void Actor::use() {
    if (hand && typeid(*hand) == typeid(Usable)) {
        unique_ptr<Usable> usb(static_cast<Usable *>(hand.release()));
        hand.reset();
        cout << Name << " " << usb->get_action_type() << " " << 
            usb->get_arttype() << "." << endl;
        unique_ptr<Object> new_obj(usb->action(this));
        usb.reset();
        hand = move(new_obj);
    }
}

    void Actor::look_for(const type_info & type) {
        if (hand && typeid(*hand) == type)
            return;
        
        int found = pocket.find_type(type);
        if (found > -1) {
            unique_ptr<Object> obj = pocket.release_object(found);
            pick_up(obj);
            return;
        }
        if (carriable) {
            found = carriable->find_type(type);
            if (found > -1) {
                unique_ptr<Object> obj = carriable->release_object(found);
                pick_up(obj);
                return;
            }
        }
        found = location->find_type(type);
        if (found > -1) {
            unique_ptr<Object> obj = location->release_object(found);
            pick_up(obj);
            return;
    
        }
    }

    void Actor::put_in_pocket(unique_ptr<Object> & obj) {
        if (pocket.fits(obj)) {
            pocket.add_object(obj);
        }
    }
    
    void Actor::affect_promille(double effect) {
        if (hp == 0)
            return;
        promille += effect;
        if (promille < 0) 
            promille = 0;
    }    

    void Actor::death() {
        cout << Name << " dog" << endl;
        if (death_lambda) {
            death_lambda();
        }

        // Drop stuff

        bool lost = false;
        string drop_string = " och tappade ";
        if (hand) {
            lost = true;
            drop_string += hand->get_arttype();
            drop();
        }
        // Create money object
        if (money) {
            if (lost)
                drop_string += " och ";
            unique_ptr<Object> mon = create_money(money);
            drop_string += mon->get_arttype();
            get_location()->add_object(mon);
            lost = true;
        }           
        if (lost)
            cout << drop_string << "." << endl;  
        Type = "Den döde " + Type;  
        Name = Name + "s lik";
        Prep = "ligger";
    }

    string Actor::save_objects(ofstream & fs, int & obj_num) const {


        stringstream h_s;
        if (hand) {
            hand->save(fs, obj_num);
            h_s << ":OBJECT" << obj_num-1;
        } else
            h_s << ":";

        pocket.save(fs, obj_num);
        stringstream p_s;
        p_s << ":OBJECT" << obj_num-1;
        
        stringstream c_s;
        if (carriable) {
            carriable->save(fs, obj_num);
            c_s << ":OBJECT" << obj_num-1;
        } else
            c_s << ":";

    
        stringstream ss;
        ss << h_s.str() << p_s.str() << c_s.str();
        return ss.str();
    }

} // end of namespace spel
