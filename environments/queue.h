#ifndef QUE_H
#define QUE_H

#include "outdoor.h"

namespace spel {

class Queue : public Outdoor {
    
    friend Environment * create_queue(const vector<string> &, map<string,unique_ptr<Object>> &);
    friend class Controller;
    
public:
    Queue(const string &);
    Queue(const string &, Environment *);
    
    virtual Environment * find_exit(const string &) const;
    void open() { closed = false; }
    
    virtual void save(ofstream &, int &, const int &) const;
    
private:
    Environment * entrance;
    bool closed;
};

}

#endif
