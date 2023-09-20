#include<iostream>
#include<iomanip>
#include <algorithm>
#include "User.cpp"
#include <map>
#include "Money_handler.cpp"
#include "jsonhandler.cpp"
#include "billgenerator.cpp"
JsonHandler jhandler;
class Menu_page{
    public:
    static int general_menu(){
        std::cout<<std::endl;
        std::cout<<std::setw(3)<<std::left<<std::setfill(' ')<<"|";
        std::cout<<std::setw(48)<<std::right<<std::setfill(' ')<<"MENU"<<std::setw(49)<<std::right<<"|"<<std::endl;
        std::cout<<std::setw(100)<<std::setfill('-')<<""<<std::endl;
        std::cout<<std::setw(30)<<std::left<<std::setfill(' ')<<" "<<std::setw(20)<<"1. Enter Amount "<<std::endl;
        std::cout<<std::setw(30)<<" "<<std::setw(20)<<"2. Admin Login "<<std::endl;
        std::cout<<std::setw(30)<<" "<<std::setw(20)<<"3. Quit "<<std::endl;
        int choice;
        std::cout<<"Please enter your choice : ";
        std::cin>>choice;
        while(choice>3 || choice<1){
            std::cout<<"Please enter a valid choice.. : ";
            std::cin>>choice;
        }
        return choice;
    }
    static void user_add_product(User &user){
        std::string product_id;
        std::cout<<"Enter your product id :";
        std::cin>>product_id;
        if(jhandler.contains(product_id)){
            if(jhandler.supply_manager(product_id,-1)){
                if(user.get_user_balance()-jhandler.find<Product>(product_id).price>=0){
                    user.set_user_balance(user.get_user_balance()-jhandler.find<Product>(product_id).price);
                    user.products.push_back(jhandler.find<Product>(product_id));
                }
                else{
                     std::cout<<"Sorry insufficient balance...."<<std::endl;
                }
            }
            else{
                std::cout<<"Sorry product insufficient.."<<std::endl;
            }
        }
        else{
                std::cout<<"Sorry Invalid product"<<std::endl;
            }
    }
    static void user_cancellation(User &user){
        bool flag=true;
        while(flag){
         std::cout<<"Are you sure to Cancel (y/n) : ";
        char choice;
        std::cin>>choice;
        tolower(choice);
        if(choice=='y'){
            for(auto products: user.products){
                jhandler.supply_manager(products.id,1);
            }
            flag=false;
            user.set_user_balance(Money_handler::return_total_money(user));
            break;
        }
        else if(choice =='n'){
            flag=false;
            break;
        }
        else{
            std::cout<<"Enter valid choice  ";
            break;

        }
        }
    }
    static void delete_user_product(User &user){
        std::string productid{};
        std::cout<<"Enter the product id : ";
        std::cin>>productid;
        auto it=find(user.products.begin(),user.products.end(),jhandler.find<Product>(productid));
        jhandler.supply_manager((*it).id,1);
        user.set_user_balance(user.get_user_balance()+(*it).get_price());
        std::cout<<(*it).name<<" Deleted"<<std::endl;
        user.products.erase(it);


    }
    static bool cart_page(User &user){
        bool completion_status=false;
        std::map<Product,int> Map;
        for(auto product_iterator: user.products)
        {
            if(Map.count(product_iterator)>0)
            {
                Map[product_iterator] = Map.at(product_iterator)+1;
            }
            else{
                Map[product_iterator] =1;
            }
        }
        std::cout<<std::setw(100)<<std::setfill('-')<<""<<std::endl;
        std::cout<<std::setw(30)<<std::setfill(' ')<<"Your Cart"<<std::endl;
        std::cout<<std::setw(100)<<std::setfill('-')<<""<<std::setfill(' ')<<std::endl;
        for(auto i:Map)
            std::cout<<std::setw(20)<<i.first<<"   x"<<i.second<<std::endl;
        std::cout<<std::setw(100)<<std::setfill('-')<<""<<std::endl;
        bool cart_flag=true;
        while(cart_flag){
        std::cout<<std::setw(20)<<std::left<<std::setfill(' ')<<"1. Buy now "<<std::endl;
        std::cout<<std::setw(20)<<std::setfill(' ')<<"2. Delete product "<<std::endl;
        std::cout<<std::setw(20)<<std::setfill(' ')<<"3. Back "<<std::endl;
        std::cout<<std::setw(100)<<std::setfill('-')<<""<<std::setfill(' ')<<std::endl;
        int cartchoice{};
        std::cout<<"Enter choice : ";
        std::cin>>cartchoice;
        switch(cartchoice){
            case 1:
            {
                bill_generator::bill(user.products);
                cart_flag=false;
                completion_status=true;
                break;
            }
            case 2:{
                delete_user_product(user);
                //cart_flag=false;
                break;
            }
            case 3:
            {
                cart_flag=false;
                break;
            }
            default:
                std::cout<<"Enter valid choice : ";
        }
        }
        return completion_status;

    } 
    
    static void user_menu(User &user,Document &d){
        bool menu_flag=true;
        int users_choice{};
        Welcome_page::welcome(d);
        while(menu_flag){
            std::cout<<std::setw(100)<<std::setfill('-')<<""<<std::endl;
            std::cout<<"1. Add product to cart "<<std::endl;
            std::cout<<"2. Go to cart "<<std::endl;
            std::cout<<"3. Add Extra amount "<<std::endl;
            std::cout<<"4. Cancel and Exit "<<std::endl;
            std::cout<<"4. Back"<<std::endl;
            std::cout<<" Your account balance is : $"<<user.get_user_balance()<<std::endl;
            std::cout<<std::setw(100)<<std::setfill('-')<<""<<std::endl;
            std::cout<<"Enter your choice : ";
            std::cin>>users_choice;
            switch(users_choice){
                case 1:{
                    user_add_product(user);
                    break;
                }
                case 2:
                {
                    if(cart_page(user))
                        Money_handler::return_changes(user);
                        user=User();
                        menu_flag=false;
                    break;
                }
                case 3:
                {
                    (user).set_user_balance((user).get_user_balance()+Money_handler::enter_amount());
                    std::cout<<"Your balance is : "<<user.get_user_balance()<<std::endl;
                    break;
                }
                case 4:{
                   user_cancellation(user);
                   Money_handler::return_changes(user);
                   user=User();
                   menu_flag=false;
                   break;

                }
                case 5:
                {
                    menu_flag=false;
                    break;
                }
            }
        }

    }

};