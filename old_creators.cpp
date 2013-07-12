namespace spel {

    Worker * create_cop(const string & name) {
        std::unique_ptr<Object> batong(new Weapon("kraftig teleskopbatong"));
        batong->set_type("en","batong","Batongen");
        batong->set_action_type("slog");
        batong->set_action(30,0);
        batong->set_info(0.8, 2, 800);

        unique_ptr<Object> np = nullptr;
    
        Worker * polis = new Worker(name, batong, np);
        polis->set_type("Polisen");
        polis->set_hp(100);
        function<void(void)> borje_al = [polis]() {cout << polis->get_hp() << endl; };
        return move(polis);
    }

    unique_ptr<Object> create_beer(const string & name) {
        std::unique_ptr<Object> olflaska(new Weapon("krossad glasflaska"));
        olflaska->set_type("en", "glasflaska", "Glasflaskan");
        olflaska->set_action_type("skar");
        olflaska->set_info(0.2, 0.33, 0); 
        olflaska->set_action(14, 0);
        olflaska->set_Prep("ligger");

        std::unique_ptr<Object> ol(new Usable(name,olflaska));
        olflaska.reset();
        
        ol->set_type("en", "öl", "Ölen");
        ol->set_action_type("drack");
        ol->set_info(0.5, 0.33, 50); 
        ol->set_action(10, 0.2);
        ol->set_Prep("Står");
        return move(ol);
    }
    
    Worker * create_doorman(const string & name) {
        std::unique_ptr<Object> sprej(new Weapon("en burk pepparsprej"));
        sprej->set_type("en","pepparsprej","Pepparsprejen");
        sprej->set_action_type("sprejade");
        sprej->set_action(20,0);
        sprej->set_info(0.3, 0.5, 500);

        unique_ptr<Object> np = nullptr;
        Worker * doorman = new Worker(name, sprej, np);
        doorman->set_type("Vakten");
        doorman->set_hp(90);
    
        function<void(void)> death_l = [doorman]() { 
            if(typeid(*(doorman->get_location())) == typeid(Queue)) { 
                Queue* q = static_cast<Queue*>(doorman->get_location());
                q->open();            
            } 
        };
        doorman->set_death_lambda(death_l);
        return move(doorman);
    }

    Visitor * create_ib(const string & name) {
        Visitor * ib1 = new Visitor(name);
        ib1->set_type("Investment bankern");
        ib1->set_promille(0.2);
        ib1->set_hp(60);
        ib1->set_money(2000);
        return move(ib1);    
    }

    Visitor * create_girlfriend(const string & name) {
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        mt19937 gen (seed); 
        weibull_distribution<double> dist(2.0,4.0);
               
        std::unique_ptr<Object> butterfly(new Weapon("en diskret butterflykniv"));
        butterfly->set_type("en","kniv","Butterflykniven");
        butterfly->set_action_type("högg");
        butterfly->set_action(50,0);
        butterfly->set_info(0.1, 0.2, 300);

        unique_ptr<Container> vaska(new Container("en moderiktig handväska",2,2));
        vaska->set_info(1.5, 2, 3000);
        vaska->set_type("en","handväska","Väskan");
        vaska->add_object(butterfly);
    
        unique_ptr<Object> vaska_(static_cast<Object *>(vaska.release()));
        vaska.reset();
        
        unique_ptr<Object> np(nullptr);
        Visitor * girlfriend = new Visitor(name, vaska_, np);
        girlfriend->set_type("Tjejpolaren");
        girlfriend->set_promille(dist(gen)/4);
        girlfriend->set_hp(60);
        girlfriend->set_money(500);        
        return move(girlfriend);    
    }
}

