#include "container.h"

namespace spel {

    Container::Container(string n, double wc, double vc) : Object(n) { 
        weight_cap = weight_free = wc;
        vol_cap = vol_free = vc;
    }
    
    unique_ptr<Object> Container::clone() const {
        unique_ptr<Container> copy(new Container(Name, weight_cap, vol_cap));
        for_each (objects.begin(), objects.end(), [this, &copy] (const unique_ptr<Object> & obj) {
            copy->objects.push_back(move(obj->clone()));
        });
        
        copy->id = id;
        copy->Name = Name;
        copy->article = article;
        copy->type = type;
        copy->Definitive = Definitive;
        copy->action_type = action_type;
        copy->Prep = Prep;
        copy->weight = weight;
        copy->volume = volume;
        copy->price = price;

        copy->vol_free = vol_free;
        copy->weight_free = weight_free;

        return move(copy);
    }

    Container::~Container() {
        for (auto i = objects.begin(); i != objects.end(); ++i)
            i->reset();
        objects.clear();
    }

    Container & Container::operator=(Container && rhs) {
        if (this==&rhs)
            return *this;

        objects.clear();
        
        id = rhs.id;
        Name = rhs.Name;
        article = rhs.article;
        type = rhs.type;
        Definitive = rhs.Definitive;
        action_type = rhs.action_type;
        Prep = rhs.Prep;
        weight = rhs.weight;
        volume = rhs.volume;
        price = rhs.price;

        vol_cap = rhs.vol_cap;
        weight_cap = rhs.weight_cap;

        vol_free = rhs.vol_free;
        weight_free = rhs.weight_free;

        for_each (rhs.objects.begin(), rhs.objects.end(), [this] (unique_ptr<Object> & obj) {
                objects.push_back(move(obj));
            });
        rhs.objects.clear();
        return *this;
    }
    
    bool Container::fits(const unique_ptr<Object> & o) {
        if (o->get_weight() <= weight_free && o->get_volume() <= vol_free)
            return true;
        else
            return false;
    }

    bool Container::add_object(unique_ptr<Object> & o) {
        if (fits(o)) {
            weight_free -= o->get_weight();
            weight += o->get_weight();

            vol_free -= o->get_volume();        
            objects.push_back(move(o));
            o = nullptr;
            return true;
        } else {
            return false;
        }
    }

    int Container::find_object(string s) const {
        int ret = -1;
        for (auto it = objects.begin(); it != objects.end(); ++it) {
            if (s == (**it).get_id()|| s == (**it).get_type()) {
                ret = it - objects.begin();     
                break;       
            }
        }
        return ret;
    }

    int Container::find_type(const type_info & type) const {
        int ret = -1;
        for (auto it = objects.begin(); it != objects.end(); ++it) {
            if (type == typeid(**it)) {
                ret = it - objects.begin();     
                break;       
            }
        }
        return ret;
    }

    unique_ptr<Object> Container::release_object(string s) {
        unique_ptr<Object> ret = nullptr;
        auto it = objects.begin();
        for (; it != objects.end(); ++it) {
            if ((**it).get_id() == s || (**it).get_type() == s) {
                break;
            }
        }
        if (it != objects.end()) {
            int pos = it - objects.begin();
            ret = release_object(pos);
        }
        return move(ret);
    }

    unique_ptr<Object> Container::release_object(int i) {
        unique_ptr<Object> ret = nullptr;
        if (i >= 0 && i < objects.size() ) {
            ret = move(objects[i]);
            objects.erase(objects.begin() + i);
            weight_free += ret->get_weight();
            weight -= ret->get_weight();
            vol_free += ret->get_volume();
        }
        return move(ret);
    }

    const unique_ptr<Object> & Container::look_at_object(int i) const {
        if (i < 0 || i >= objects.size() ) {
            throw out_of_range("Object index out of bounds");
        }
        return objects[i];
    }

    string Container::description() const {
        stringstream s;
        s << main_description() << " Den kan innehålla upp till " << weight_cap << 
            "kg samt upp till " << vol_cap << " liter. Just nu är det "<< setprecision(5) << weight_free <<
            " kg och " << setprecision(5) << vol_free << " liter kvar. ";
        if (objects.size() > 0) {
            s << "I " << get_definitive() << " finns ";
            for (auto it=objects.begin(); it!=objects.end(); ++it)
                s << (**it).get_arttype() << ", ";
        } else {
            s << get_definitive() << " är tom.";
        }
        return s.str();
    }

    void Container::save(ofstream & fs, int & obj_cnt) const {
        vector<int> objs;
        for_each (objects.begin(), objects.end(), 
                  [& fs, &obj_cnt, & objs] (const unique_ptr<Object> & obj) {
                      obj->save(fs, obj_cnt); 
                      objs.push_back(obj_cnt-1);
                  } );
    
        fs << "OBJECT" << obj_cnt++ << 
            ":CONTAINER:" << 
            Name << ":" << 
            article << ":" << 
            type << ":" << 
            Definitive << ":" << 
            action_type << ":" << 
            Prep << ":" << 
            weight << ":" << 
            volume << ":" << 
            price << ":" << 
            action1 << ":" << 
            action2 << ":" << 
            weight_cap << ":" << 
            vol_cap << ":" << 
            join(objs) <<
            endl;
    }

} // End of namespace
