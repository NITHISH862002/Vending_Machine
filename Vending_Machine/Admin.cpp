#include <iostream>
#include <string>
#include <fstream>
#include "C:/sanjeev/codes/rapidjson/document.h"
#include "C:/sanjeev/codes/rapidjson/writer.h"
#include "C:/sanjeev/codes/rapidjson/stringbuffer.h"
#include "C:/sanjeev/codes/rapidjson/istreamwrapper.h"
#include "C:/sanjeev/codes/rapidjson/ostreamwrapper.h"
#include "jsonhandler.cpp"

struct login_credentials
{
    std::string user_id;
    std::string password;

    bool operator== (const login_credentials &rhs)
    {
        return (user_id==rhs.user_id&&password==rhs.password);
    }
};

class Admin
{
    private:
    login_credentials secret; 
    JsonHandler json_handler;

    public:
    static void admin_page(){
        std::string admin_name{};
        std::string admin_pass{};
        std::cout<<"Enter user id : ";
        std::cin>>admin_name;
        std::cout<<"Enter password : ";
        std::cin>>admin_pass;
        Admin adm;
        login_credentials admin_struct;
        admin_struct.user_id=admin_name;
        admin_struct.password=admin_pass;
        if(adm.Admin_verification(admin_struct)){
            bool admin_flag=true;
            while(admin_flag){
                std::cout<<"1. Add product "<<std::endl;
                std::cout<<"2. Modify supplies "<<std::endl;
                std::cout<<"3. Back "<<std::endl;
                int admin_choice{};
                std::cout<<"Enter the choice : ";
                std::cin>>admin_choice;
                switch(admin_choice){
                    case 1:
                    {
                        std::string name{};
                        int price{};
                        int quantity{};
                        std::cout<<"Enter product name : "<<std::endl;
                        std::cin>>name;
                        std::cout<<"Enter product price : "<<std::endl;
                        std::cin>>price;
                        std::cout<<"Enter product Quantity : "<<std::endl;
                        std::cin>>quantity;
                        adm.Add_product(name,price,quantity);
                        std::cout<<name<<" added successfully..."<<std::endl;
                        break;
                    }
                    case 2:
                    {
                        std::string productid{};
                        int quantity{};
                        std::cout<<"Enter product id to modify : ";
                        std::cin>>productid;
                        std::cout<<"Enter Quantity of "<<productid<<" : ";
                        std::cin>>quantity;
                        adm.Modify_Supplies(productid,quantity);
                        break;
                    }
                    case 3:
                    {
                        admin_flag=false;
                        break;
                    }
                }
            }
        }
    }
    bool Admin_verification(login_credentials user){
        secret.user_id = "Admin";
        secret.password = "12345";
        return secret==user;
    }


    void Add_product(std::string name, int price, int quantity)
    {
        
        rapidjson:: Document json_document = json_handler.Get_input_document();
        auto End_collection_pointer = json_document.End()-1;
        std::string End_key = (*End_collection_pointer).MemberBegin()->name.GetString();
        int new_key = std::stoi(End_key)+1;
        std::string str_new_key = std::to_string(new_key);
        auto key_to_add = rapidjson::StringRef((char*)str_new_key.c_str());
        auto name_to_add = rapidjson::StringRef((char*)name.c_str());
        
        rapidjson::Value Inner_collection(rapidjson::kObjectType);
        Inner_collection.AddMember("Name",name_to_add,json_document.GetAllocator()); 
        Inner_collection.AddMember("Price",price,json_document.GetAllocator()); 
        Inner_collection.AddMember("Quantity",quantity,json_document.GetAllocator()); 

        rapidjson::Value Collection_to_add(rapidjson::kObjectType);
        Collection_to_add.AddMember(key_to_add,Inner_collection,json_document.GetAllocator());

        json_document.GetArray().PushBack(Collection_to_add,json_document.GetAllocator());
        json_handler.Store_to_json(json_document);
    }

    void Modify_Supplies(std::string id, int quantity)
    {
        rapidjson::Document json_document = json_handler.Get_input_document();
        bool flag = false;
        for(auto array_iterator = json_document.Begin(); array_iterator!=json_document.End();array_iterator++)
        {
            
            std::string target_member = array_iterator->MemberBegin()->name.GetString();
            if(target_member==id)
            {
                auto value_to_modify = array_iterator->MemberBegin()->value.GetObject();
                rapidjson::Value &quantity_to_modify = value_to_modify["Quantity"];
                quantity_to_modify.SetInt(quantity);
                json_handler.Store_to_json(json_document);
                flag = true;
                break;
            }
        }
        if(flag)
        {
            std::cout<<"Target value found and replaced"<<std::endl;
        }
        else{
             std::cout<<"Target value not found"<<std::endl;
        }
    }

    

};