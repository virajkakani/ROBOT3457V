#include "Err_Types.h"

#include <iostream>


ERROR::ERROR() : err(ERRTYPE::OK), message(std::string()) {}
ERROR::ERROR(const ERRTYPE& e, const std::string& s) {
    if (e == ERRTYPE::OK){
        err = e;
        message = std::string();
    }
    err = e; 
    message = s;
}

ERROR::ERROR(const ERRTYPE& e){
    err = e;
    message = std::string();

    show_err();
}

bool ERROR::operator!=(const ERROR& other) const {
    return err != other.err;
}

bool ERROR::operator!=(const ERRTYPE& other) const {
    return err != other;
}

void ERROR::show_err(){
    switch(err){
        case(ERRTYPE::OK):
            return;
        case(ERRTYPE::IA):
            std::cout << "INVALID_ARGUMENT: ";
            break;
        case(ERRTYPE::OOR):
            std::cout << "OUT_OF_RANGE: ";
            break;
        case(ERRTYPE::DE):
            std::cout << "DOMAIN_ERROR: ";
            break;
        case(ERRTYPE::OE):
            std::cout << "OVERFLOW_ERROR: "; 
            break;
        case(ERRTYPE::UE):
            std::cout << "UNDERFLOW_ERROR: ";
            break;
        case(ERRTYPE::RE):
            std::cout << "RANGE_ERROR: ";
            break;
        case (ERRTYPE::SE):
            std::cout << "SYSTEM_ERROR: ";
            break;
        default:
            return;
    }
    std::cout << message << "\n";
}

ERROR INVALID_ARGUMENT(std::string s) {return ERROR(ERRTYPE::IA, s);}
ERROR OUT_OF_RANGE(std::string s) {return ERROR(ERRTYPE::OOR, s);}
ERROR DOMAIN_ERROR(std::string s) {return ERROR(ERRTYPE::DE, s);}
ERROR OVERFLOW_ERROR(std::string s){return ERROR(ERRTYPE::OE, s);}
ERROR UNDERFLOW_ERROR(std::string s){return ERROR(ERRTYPE::UE, s);}
ERROR RANGE_ERROR(std::string s){return ERROR(ERRTYPE::RE, s);}
ERROR SYSTEM_ERROR(std::string s){return ERROR(ERRTYPE::SE, s);}
