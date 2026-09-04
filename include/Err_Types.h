#ifndef ERR_TYPES_H
#define ERR_TYPES_H

#include <cstdint>
#include <string>

enum class ERRTYPE : std::uint8_t;
struct ERROR;
template <typename T>
struct TypeandERR;
template<>
struct TypeandERR<void>;



enum class ERRTYPE : std::uint8_t{
    OK = 0,
    IA, // INVALID_ARGUMENT
    OOR, // OUT_OF_RANGE
    DE, // DOMAIN_ERROR
    OE, // OVERFLOW_ERROR
    UE, // UNDERFLOW_ERROR
    RE, // RANGE_ERROR
    SE // SYSTEM_ERROR
};   

struct ERROR{
    ERRTYPE err;
    std::string message;

    ERROR();
    ERROR(const ERRTYPE& e, const std::string& s);
    ERROR(const ERRTYPE& e);

    void show_err();
    bool operator!=(const ERROR& other) const;
    bool operator!=(const ERRTYPE& other) const;
};

ERROR INVALID_ARGUMENT(std::string s);
ERROR OUT_OF_RANGE(std::string s);
ERROR DOMAIN_ERROR(std::string s);
ERROR OVERFLOW_ERROR(std::string s);
ERROR UNDERFLOW_ERROR(std::string s);
ERROR RANGE_ERROR(std::string s);
ERROR SYSTEM_ERROR(std::string s);

#define inv_arg(x) return INVALID_ARGUMENT(x)
#define not_in_range(x) return OUT_OF_RANGE(x)
#define domain_err(x) return DOMAIN_ERROR(x)
#define overflow_err(x) return OVERFLOW_ERROR(x)
#define underflow_err(x) return UNDERFLOW_ERROR(x)
#define range_err(x) return RANGE_ERROR(x)
#define sys_err(x) return SYSTEM_ERROR(x)
#define okay return ERROR()

template <typename T>
struct TypeandERR{
    T val;
    ERROR err;

 
    TypeandERR(const T& value) : val(value), err(ERRTYPE::OK) {}

    
    TypeandERR(const ERROR& error) : err(error), val(T()) {}


    T* operator->() { 
        return &val; 
    }

    
    const T* operator->() const{ 
        return &val; 
    }

    
    operator T() const{
        return val;
    }

    
    T& operator*(){
        return val;
    }

    
    const T& operator*() const{
        return val;
    }

    
    ERROR get_err() const{
        return err;
    }

    
    TypeandERR(const TypeandERR& other){
        val = other.val;
        err = other.err;
    }

    
    bool is_err(){
        if (err != ERRTYPE::OK){
            return true;
        }
        return false;
    }

    T get_val() const{
        return val;
    }

};

template <>
struct TypeandERR<void>{
    ERROR err;

    
    TypeandERR(const ERROR& error) : err(error) {}
    TypeandERR() : err(ERRTYPE::OK) {}

    
    ERROR get_err() const{
        return err;
    }

    bool is_err(){
        return err != ERRTYPE::OK;
    }
};

#endif // ERR_TYPES_H
