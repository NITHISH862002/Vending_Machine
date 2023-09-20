#include "C:/sanjeev/codes/rapidjson/document.h"
#include "C:/sanjeev/codes/rapidjson/writer.h"
#include "C:/sanjeev/codes/rapidjson/stringbuffer.h"
#include "C:/sanjeev/codes/rapidjson/istreamwrapper.h"
#include "C:/sanjeev/codes/rapidjson/ostreamwrapper.h"
#include <iostream>
#include<fstream>
#include "Welcome_page.cpp"
#include "Menu_page.cpp"
#include "Admin.cpp"
using namespace rapidjson;

void open_file(Document &d){
    std::ifstream file{"Vending_library.json"};
    if(!file){
        std::cout<<"error in opening file";
    }
    else{
        IStreamWrapper is(file);
        d.ParseStream(is);
    }
}

int main(){
    Document d;
    open_file(d);
    Welcome_page::welcome(d);
    int user_choice{};
    bool menu_flag=true;
    User user=(User());
    while(menu_flag){
        user_choice=(Menu_page::general_menu());
        switch(user_choice){
            case 1:
            {
                (user).set_user_balance((user).get_user_balance()+Money_handler::enter_amount());
                 std::cout<<"Your balance is : "<<user.get_user_balance()<<std::endl;
                Menu_page::user_menu(user,d);
            
                break;
            }
            case 2:
            {
                Admin::admin_page();
                break;
            }
            case 3:
            {
                menu_flag=false;
                break;
            }
        }
    }


    
}