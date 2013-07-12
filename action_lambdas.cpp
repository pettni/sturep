namespace spel {

function<int(void)> create_ib_action_l(Visitor* ret) {
    function<int(void)> action_l = [ret]() {
        //Random generator
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        mt19937 gen (seed);  
                                
        //Action types:
        
        // 0: inget
        // 1: go random
        // 2: sök bar
        // 3: sök mcd 
        // 4: leta vapen + slå
        // 5: leta powerup + använd
        // 6: slå slumpmässig
        // 7: sök dansgolv
              
        //If low on hp
        discrete_distribution<int> toss {1,1};    
        if(ret->get_hp() < 30) {                    
            if(ret->get_enemy() && ret->get_enemy()->get_location() == ret->get_location()) {
                switch(toss(gen)) {
                    case 0: return 1;  //Escape if enemy in room
                    case 1: return 4;  //Fight back
                }                        
            }
            else {
                return 5;  //Find powerup
            }                            
        }    
                                        
        //If angry
        if(ret->get_angry()) {
            //If enemy in room
            if(ret->get_enemy()->get_location() == ret->get_location()) {
                return 4; //Find a weapon and fight       
            }
            else {
                return 1; //Search randomly in anger
            }
        }
        
        //If not drunk        
        if(ret->get_promille() < 1) {  
            //If not in bar
            if(!(ret->get_location()->get_Name().size() >= 3 && ret->get_location()->get_Name().substr(0,3) == "bar")) {
                return 2; //Go and look for a bar
            }
            else {
                return 5; //Get a drink
            }
            
        }
        
        //If moderately drunk
        discrete_distribution<int> toss1 {3,1,1,1};        
        if(ret->get_promille() >= 1 && ret->get_promille() < 2) {
            // Do nothing/Get a drink/Walk around/Fight someone
            switch(toss1(gen)) {
                case 0: return 0;
                case 1: return 5;
                case 2: return 1;
                case 3: return 6;            
            }                    
        }
        
        //If drunk
        discrete_distribution<int> toss2 {4,2,1,2};        
        if(ret->get_promille() >= 2 && ret->get_promille() < 3) {
            // Do nothing/Get a drink/Walk around/Fight someone
            switch(toss2(gen)) {
                case 0: return 0;
                case 1: return 5;
                case 2: return 1;
                case 3: return 6;            
            }  
        }
        // If very drunk
        discrete_distribution<int> toss3 {4,3,2};             
        if(ret->get_promille() >= 3) {   
            // Do nothing/Find McDonalds/Fight someone
            switch(toss3(gen)) {
                case 0: return 0;
                case 1: return 3;
                case 2: return 6;
            }
        }
        return 0;
    };
    return action_l;
}

function<int(void)> create_gf_action_l(Visitor * ret) { 
}

function<int(void)> create_bingo_action_l(Visitor * ret) { 
    function<int(void)> action_l = [ret]() {
        //Random generator
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        mt19937 gen (seed);  
                
        //Action types:
        
        // 0: inget
        // 1: go random
        // 2: sök bar
        // 3: sök mcd 
        // 4: leta vapen + slå
        // 5: leta powerup + använd
        // 6: slå slumpmässig
        // 7: sök dansgolv
        // 8: gå inomhus
        // 9: muta
        
        //If low on hp
        if(ret->get_hp() < 30) {            
            if(ret->get_enemy() && ret->get_enemy()->get_location() == ret->get_location()) {
                return 8;  //Escape if enemy in room                        
            }
            else {
                return 5;  //Find powerup
            }                            
        }    
                                        
        //If angry
        if(ret->get_angry()) {
            //If enemy in room
            if(ret->get_enemy()->get_location() == ret->get_location()) {
                return 9; //Bribe
            }
            else {
                return 8; //Walk inside
            }
        }
        
        //If not drunk        
        if(ret->get_promille() < 0.2) {
            //If not in bar
            if(!(ret->get_location()->get_Name().size() >= 3 && ret->get_location()->get_Name().substr(0,3) == "bar")) {
                return 2; //Go and look for a bar
            }
            else {
                return 5; //Get a drink
            }
            
        }
        
        //If moderately drunk
        discrete_distribution<int> toss1 {3,1,8,1};        
        if(ret->get_promille() >= 0.2 && ret->get_promille() < 0.6) {
            // Do nothing/Get a drink/Go dance/Walk around/Fight someone
            switch(toss1(gen)) {
                case 0: return 0;
                case 1: return 5;
                case 2: return 7;
                case 3: return 8;            
            }                    
        }
        
        //If drunk
        discrete_distribution<int> toss2 {4,2,1};        
        if(ret->get_promille() >= 0.6 && ret->get_promille() < 0.9) {
            // Do nothing/Get a drink/Walk around/Fight someone
            switch(toss2(gen)) {
                case 0: return 0;
                case 1: return 5;
                case 2: return 8;           
            }  
        }
        // If very drunk             
        if(ret->get_promille() >= 0.9) {      
            return 0;
        }
        return 0;
    };
    return action_l;
}

function<int(void)> create_worker_action_l(Worker* ret) {
    function<int(void)> action_l = [ret]() {
                        
        //Action types:
        
        // 0: inget
        // 1: go random
        // 2: sök bar
        // 3: sök mcd 
        // 4: leta vapen + slå
        // 5: leta powerup + använd
        // 6: slå slumpmässig
        // 7: sök dansgolv       
                                        
        //If angry
        
        if(ret->get_angry() && ret->get_enemy()->get_location() == ret->get_location())
            return 4; //Find a weapon and fight
        else
            return 0; //Do nothing            
    };
    return action_l;
} 

 function<int(unique_ptr<Object>&)> create_worker_get_l (Worker* ret) {
    function<int(unique_ptr<Object>&)> get_l = [ret](unique_ptr<Object>& obj) {
                        
        //Action types:
        // 0: refuse
        // 1: accept and put in pocket
        // 2: accept and use   
        
        if(typeid(*obj) == typeid(Money)) {
            if (obj->get_price() >= 500) {
                cout << ret->get_Name() << ": Tackar. Fattar inte att vi nånsin bråkat." << endl;
                return 1;
            }
            else {
                return 0;        
            }
        }
        
        if(typeid(*obj) == typeid(Usable))
            return 0;
            
        if(typeid(*obj) == typeid(Weapon))
            return 0;
                        
        if(typeid(*obj) == typeid(Container))
            return 0;                     
    };
    return get_l;
}

 function<int(unique_ptr<Object>&)> create_visitor_get_l (Visitor* ret) {
    function<int(unique_ptr<Object>&)> get_l = [ret](unique_ptr<Object>& obj) {
                        
        //Action types:
        // 0: refuse
        // 1: accept and put in pocket
        // 2: accept and use   
        
        if(typeid(*obj) == typeid(Money)) {
            if (obj->get_price() >= 500) {
                cout << ret->get_Name() << ": Schysst. Du är en riktig polare." << endl;
                return 1;
            }
            else {
                return 0;        
            }
        }
        
        if(typeid(*obj) == typeid(Usable))
            cout << ret->get_Name() << ": Schysst. Du är en riktig polare." << endl;        
            return 2;
            
        if(typeid(*obj) == typeid(Weapon))
            return 0;
                        
        if(typeid(*obj) == typeid(Container))
            return 0;                     
    };
    return get_l;
}

} //End of namespace
