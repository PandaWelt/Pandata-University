#pragma once
// Small bounded JSON value for the platform protocol, never used by the mission domain.
#include "Mission.h"
#include <map>
#include <vector>
#include <variant>
#include <charconv>
#include <cmath>
namespace pu::mentor {
struct Json {
    using Object=std::map<std::string,Json>; using Array=std::vector<Json>;
    std::variant<std::nullptr_t,bool,double,std::string,Object,Array> value=nullptr;
    Json()=default; Json(std::nullptr_t){} Json(bool x):value(x){} Json(int x):value(double(x)){}
    Json(double x):value(x){} Json(std::string x):value(std::move(x)){} Json(const char* x):value(std::string(x)){}
    Json(Object x):value(std::move(x)){} Json(Array x):value(std::move(x)){}
    bool null()const{return std::holds_alternative<std::nullptr_t>(value);}
    const Json& at(const std::string& key)const {static const Json empty;auto o=std::get_if<Object>(&value);if(!o)return empty;auto i=o->find(key);return i==o->end()?empty:i->second;}
    bool has(const std::string& key)const{auto o=std::get_if<Object>(&value);return o&&o->contains(key);}
    std::string str()const {auto s=std::get_if<std::string>(&value);if(!s)throw Error("Protocol string missing");return *s;}
    bool boolean()const {auto b=std::get_if<bool>(&value);if(!b)throw Error("Protocol boolean missing");return *b;}
    double number()const {auto n=std::get_if<double>(&value);if(!n)throw Error("Protocol number missing");return *n;}
    const Array& array()const {auto a=std::get_if<Array>(&value);if(!a)throw Error("Protocol array missing");return *a;}
    const Object& object()const {auto o=std::get_if<Object>(&value);if(!o)throw Error("Protocol object missing");return *o;}
    std::string dump()const;
    static Json parse(const std::string&);
};
inline Json obj(std::initializer_list<std::pair<const std::string,Json>> v){return Json::Object(v);}
inline Json arr(std::initializer_list<Json> v){return Json::Array(v);}
}
