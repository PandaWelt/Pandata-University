#include "mentor/Json.h"
namespace pu::mentor {
namespace {
void utf8(std::string& out,unsigned c) {
    if(c<128)out+=char(c);
    else if(c<2048){out+=char(192|(c>>6));out+=char(128|(c&63));}
    else if(c<65536){out+=char(224|(c>>12));out+=char(128|((c>>6)&63));out+=char(128|(c&63));}
    else {out+=char(240|(c>>18));out+=char(128|((c>>12)&63));out+=char(128|((c>>6)&63));out+=char(128|(c&63));}
}
struct Parser {
    const std::string& s;std::size_t i=0;
    [[noreturn]] void bad(){throw Error("Invalid or unsupported JSONL from Codex; mentor is offline");}
    void ws(){while(i<s.size()&&(s[i]==' '||s[i]=='\r'||s[i]=='\n'||s[i]=='\t'))++i;}
    bool take(char c){ws();if(i<s.size()&&s[i]==c){++i;return true;}return false;}
    unsigned hex(){unsigned n=0;for(int k=0;k<4;++k){if(i==s.size())bad();char c=s[i++];n*=16;if(c>='0'&&c<='9')n+=c-'0';else if(c>='a'&&c<='f')n+=c-'a'+10;else if(c>='A'&&c<='F')n+=c-'A'+10;else bad();}return n;}
    std::string string(){if(!take('"'))bad();std::string out;
        while(i<s.size()) {unsigned char c=s[i++];if(c=='"')return out;if(c<32)bad();
            if(c=='\\'){if(i==s.size())bad();c=s[i++];switch(c){
            case '"':case '\\':case '/':out+=char(c);break;case 'b':out+='\b';break;case 'f':out+='\f';break;case 'n':out+='\n';break;case 'r':out+='\r';break;case 't':out+='\t';break;
            case 'u':{unsigned cp=hex();if(cp>=0xd800&&cp<=0xdbff){if(i+2>s.size()||s.substr(i,2)!="\\u")bad();i+=2;unsigned low=hex();if(low<0xdc00||low>0xdfff)bad();cp=0x10000+((cp-0xd800)<<10)+(low-0xdc00);}else if(cp>=0xdc00&&cp<=0xdfff)bad();utf8(out,cp);break;}default:bad();}}
            else if(c<128)out+=char(c);
            else {int n=c>=0xc2&&c<=0xdf?1:c>=0xe0&&c<=0xef?2:c>=0xf0&&c<=0xf4?3:0;if(!n)bad();unsigned cp=c&((1<<(6-n))-1);for(int k=0;k<n;++k){if(i==s.size())bad();unsigned char d=s[i++];if((d&192)!=128)bad();cp=(cp<<6)|(d&63);}if(cp<(n==1?128u:n==2?2048u:65536u)||cp>0x10ffff||(cp>=0xd800&&cp<=0xdfff))bad();utf8(out,cp);}
        }bad();
    }
    Json read(int depth=0){if(depth>48)bad();ws();if(i==s.size())bad();
        if(s[i]=='"')return string();
        if(take('{')){Json::Object o;if(take('}'))return o;do {auto key=string();if(!take(':')||o.contains(key))bad();o.emplace(std::move(key),read(depth+1));if(take('}'))return o;}while(take(','));bad();}
        if(take('[')){Json::Array a;if(take(']'))return a;do {a.push_back(read(depth+1));if(take(']'))return a;}while(take(','));bad();}
        for(auto word:{"true","false","null"}){std::string w=word;if(s.compare(i,w.size(),w)==0){i+=w.size();if(w=="null")return {};return w=="true";}}
        auto begin=i;if(s[i]=='-')++i;if(i==s.size())bad();if(s[i]=='0')++i;else {if(s[i]<'1'||s[i]>'9')bad();while(i<s.size()&&s[i]>='0'&&s[i]<='9')++i;}
        if(i<s.size()&&s[i]=='.'){++i;auto p=i;while(i<s.size()&&s[i]>='0'&&s[i]<='9')++i;if(i==p)bad();}
        if(i<s.size()&&(s[i]=='e'||s[i]=='E')){++i;if(i<s.size()&&(s[i]=='+'||s[i]=='-'))++i;auto p=i;while(i<s.size()&&s[i]>='0'&&s[i]<='9')++i;if(i==p)bad();}
        double n=0;auto [end,ec]=std::from_chars(s.data()+begin,s.data()+i,n);if(ec!=std::errc{}||end!=s.data()+i||!std::isfinite(n))bad();return n;
    }
};
}
Json Json::parse(const std::string& text){if(text.size()>1024*1024)throw Error("JSON size limit exceeded");Parser p{text};auto j=p.read();p.ws();if(p.i!=text.size())p.bad();return j;}
std::string Json::dump()const {
    if(null())return "null";
    if(auto b=std::get_if<bool>(&value))return *b?"true":"false";
    if(auto n=std::get_if<double>(&value)){char buf[64];auto [end,ec]=std::to_chars(buf,buf+64,*n);if(ec!=std::errc{}||!std::isfinite(*n))throw Error("Invalid JSON number");return std::string(buf,end);}
    if(auto s=std::get_if<std::string>(&value)){std::string out="\"";for(unsigned char c:*s){if(c=='"'||c=='\\'){out+='\\';out+=char(c);}else if(c<32){const char* h="0123456789abcdef";out+="\\u00";out+=h[c>>4];out+=h[c&15];}else out+=char(c);}return out+'"';}
    std::string out;if(auto a=std::get_if<Array>(&value)){out="[";for(auto& v:*a){if(out.size()>1)out+=',';out+=v.dump();}return out+']';}
    out="{";for(auto& [k,v]:object()){if(out.size()>1)out+=',';out+=Json(k).dump()+':'+v.dump();}return out+'}';
}
}
