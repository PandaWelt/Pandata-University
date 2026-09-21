#include "mentor/Mentor.h"
#include "RepositoryActions.h"
#include <iostream>
#include <fstream>
#include <deque>
#include <thread>
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif
namespace m=pu::mentor;namespace fs=std::filesystem;
int checks=0;
void check(bool ok,const char* label){if(!ok)throw pu::Error(std::string("FAIL: ")+label);++checks;}
template<class F>void rejects(F f,const char* label){bool threw=false;try{f();}catch(const std::exception&){threw=true;}check(threw,label);}
m::Json goodConfig(){auto features=m::Json::Object{};for(auto key:{"apps","shell_tool","unified_exec","hooks","multi_agent","memories","remote_plugin","skill_mcp_dependency_install","goals","fast_mode"})features[key]=false;
    return m::obj({{"forced_login_method","chatgpt"},{"model_provider","openai"},{"sandbox_mode","read-only"},{"approval_policy","untrusted"},{"web_search","disabled"},{"features",features}});
}
m::Json goodQuota(){return m::obj({{"rateLimits",m::obj({{"limitId","codex"},{"credits",m::obj({{"hasCredits",false},{"unlimited",false}})},
    {"primary",m::obj({{"usedPercent",10},{"resetsAt",4102444800.0}})},{"secondary",m::obj({{"usedPercent",20},{"resetsAt",4102444800.0}})}})}});}
struct Fake:m::Channel {
    m::Json cfg=goodConfig(),quota=goodQuota(),acct=m::obj({{"account",m::obj({{"type","chatgpt"},{"planType","plus"}})}});
    m::Json features=m::arr({}),skills=m::arr({});
    std::string fault;std::deque<std::string> pending;std::vector<m::Json> sent;
    void send(const std::string& line)override{auto j=m::Json::parse(line);sent.push_back(j);if(!j.has("method")||j.at("method").str()=="initialized")return;
        auto method=j.at("method").str();m::Json r=m::obj({});
        if(method=="getAuthStatus")r=m::obj({{"authMethod",fault=="external"?"chatgptAuthTokens":"chatgpt"},{"authToken",nullptr}});
        else if(method=="config/read")r=m::obj({{"config",cfg}});
        else if(method=="experimentalFeature/list")r=m::obj({{"data",features},{"nextCursor",nullptr}});
        else if(method=="skills/list")r=m::obj({{"data",skills}});
        else if(method=="account/read")r=acct;
        else if(method=="account/rateLimits/read")r=quota;
        else if(method=="model/list")r=m::obj({{"data",m::arr({m::obj({{"model",fault=="model"?"other":"gpt-5.6-terra"},{"hidden",false}})})}});
        else if(method=="thread/start")r=m::obj({{"sandbox",m::obj({{"type",fault=="sandbox"?"workspaceWrite":"readOnly"}})},{"approvalPolicy","untrusted"},{"model","gpt-5.6-terra"},{"modelProvider","openai"},{"instructionSources",m::arr({})},{"thread",m::obj({{"id","thread-1"},{"ephemeral",true}})}});
        else if(method=="turn/start")r=m::obj({{"turn",m::obj({{"id","turn-1"}})}});
        pending.push_back(m::obj({{"id",j.at("id")},{"result",r}}).dump());
        if(method=="turn/start"){
            if(fault=="write"||fault=="command"||fault=="tokens")pending.push_back(m::obj({{"id",71},{"method",fault=="write"?"item/fileChange/requestApproval":fault=="command"?"item/commandExecution/requestApproval":"account/chatgptAuthTokens/refresh"},{"params",m::obj({})}}).dump());
            if(fault=="tool")pending.push_back(m::obj({{"method","item/started"},{"params",m::obj({{"item",m::obj({{"type","mcpToolCall"}})}})}}).dump());
            if(fault=="authchange")pending.push_back(m::obj({{"method","account/updated"},{"params",m::obj({{"authMode","apikey"},{"planType","plus"}})}}).dump());
            if(fault=="ratelimit")pending.push_back(m::obj({{"method","account/rateLimits/updated"},{"params",m::obj({{"rateLimits",m::obj({{"rateLimitReachedType","limit"}})}})}}).dump());
            if(fault=="truncated")pending.push_back("{\"method\":");
            if(fault=="death")return;
            pending.push_back(m::obj({{"method","item/completed"},{"params",m::obj({{"threadId",fault=="wrongid"?"other":"thread-1"},{"turnId","turn-1"},{"item",m::obj({{"type","agentMessage"},{"text","A saved response is not proof of mastery. What would you check independently?"}})}})}}).dump());
            pending.push_back(m::obj({{"method","turn/completed"},{"params",m::obj({{"threadId","thread-1"},{"turn",m::obj({{"id","turn-1"},{"status",fault=="cancel"?"interrupted":fault=="failure"?"failed":"completed"}})}})}}).dump());
            if(fault=="early"){auto response=pending.front();pending.pop_front();pending.push_back(response);}
        }
    }
    std::string receive()override{if(pending.empty())throw pu::Error("simulated child death/deadline");auto s=pending.front();pending.pop_front();return s;}
    bool called(const std::string& method){for(auto& j:sent)if(j.has("method")&&j.at("method").str()==method)return true;return false;}
};
int main(int argc,char** argv){try{
    if(argc==4&&std::string(argv[1])=="--schema-report"){
        std::cout<<m::inspectCompatibility(fs::path(argv[2]),argv[3]).dump()<<'\n';return 0;
    }
    if(argc==4&&std::string(argv[1])=="--schema-check"){
        const auto mode=m::checkSchema(fs::path(argv[2]),argv[3]);
        std::cout<<"PASS: installed schema policy = "<<mode<<'\n';return 0;
    }
    if(argc>1&&std::string(argv[1])=="--pipe-child"){
#ifdef _WIN32
        _setmode(_fileno(stdout),_O_BINARY);
#endif
        std::cerr<<"not JSON; must never enter the protocol\n";std::string line;std::getline(std::cin,line);
        std::cout<<m::obj({{"echo",line},{"literal",argc>2?argv[2]:""},{"secretInherited",std::getenv("OPENAI_API_KEY")!=nullptr}}).dump()<<'\n'<<std::flush;return 0;
    }
    if(argc>1&&std::string(argv[1])=="--hang"){std::this_thread::sleep_for(std::chrono::seconds(5));return 0;}
    if(argc!=2)throw pu::Error("Usage: mentor-tests repository-root");
    auto root=fs::canonical(argv[1]);
    for(auto invalid:{"{","[]x","{\"a\":1,\"a\":2}","\"\\ud800\"","01","1e","[NaN]","\"\n\"","1e9999"})rejects([&]{m::Json::parse(invalid);},"strict JSON refuses malformed/ambiguous input");
    auto unicode=m::Json::parse("\"caf\\u00e9 \\ud83d\\udc3c\"");check(m::Json::parse(unicode.dump()).str()==unicode.str(),"UTF-8 roundtrip");
    rejects([&]{m::Json::parse(std::string(60,'[')+std::string(60,']'));},"JSON depth bounded");
    rejects([&]{m::Json::parse(std::string(1048577,' '));},"JSON bytes bounded");
    std::ifstream in(root/"course/university/missions/u1-boundaries.pum");auto mission=pu::loadMission(in);auto p=pu::freshProgress(mission);auto before=pu::serializeProgress(mission,p);auto bundle=m::context(mission,p);
    check(bundle.size()<10000&&bundle.find("u1-boundaries")!=std::string::npos,"bounded mission context");
    check(bundle.find("readOnly")==std::string::npos&&bundle.find(root.string())==std::string::npos,"context omits path lists and absolute repository");
    check(before==pu::serializeProgress(mission,p),"context cannot change progress");
    Fake success;auto answer=m::interact(success,bundle,"isolated",true);check(answer.available&&!answer.answer.empty(),"one advisory interaction succeeds with protocol fixture");
    check(before==pu::serializeProgress(mission,p),"mentor answer cannot grade or change progress");
    check(success.called("initialize")&&success.called("turn/start"),"real protocol request sequence");
    Fake early;early.fault="early";check(m::interact(early,bundle,"isolated",true).available,"events before turn acknowledgement are retained");
    Fake status;check(m::interact(status,bundle,"isolated",false).available&&!status.called("thread/start"),"status sends no model request");
    for(auto fault:{"write","command","tokens","tool","authchange","ratelimit","truncated","death","wrongid","cancel","failure","sandbox","model","external"}){
        Fake f;f.fault=fault;auto r=m::interact(f,bundle,"isolated",true);check(!r.available&&r.answer.empty(),fault);
        if(f.fault=="write"||f.fault=="command"||f.fault=="tokens")check(f.sent.back().has("error"),"server capability explicitly denied");
        if(f.fault=="sandbox"||f.fault=="model")check(!f.called("turn/start"),"unsupported capability never receives context");
    }
    for(auto type:{"apiKey","chatgptAuthTokens","amazonBedrock","unknown"}){Fake f;f.acct=m::obj({{"account",m::obj({{"type",type},{"planType","plus"}})}});check(!m::interact(f,bundle,"isolated",true).available&&!f.called("turn/start"),"wrong auth mode fails offline");}
    {Fake f;f.acct=m::obj({{"account",nullptr}});check(!m::interact(f,bundle,"isolated",true).available,"signed-out/failed-login stays offline");}
    {Fake f;f.acct=m::obj({{"account",m::obj({{"type","chatgpt"},{"planType","unknown"}})}});check(!m::interact(f,bundle,"isolated",true).available,"unknown plan refused");}
    {Fake f;f.quota=m::obj({});check(!m::interact(f,bundle,"isolated",true).available,"unknown quota refuses inference");}
    {Fake f;auto& q=std::get<m::Json::Object>(f.quota.value).at("rateLimits");std::get<m::Json::Object>(q.value)["credits"]=m::obj({{"hasCredits",true},{"unlimited",false}});check(!m::interact(f,bundle,"isolated",true).available,"purchased credits never used");}
    {Fake f;auto& q=std::get<m::Json::Object>(f.quota.value).at("rateLimits");std::get<m::Json::Object>(q.value)["primary"]=m::obj({{"usedPercent",100},{"resetsAt",4102444800.0}});check(!m::interact(f,bundle,"isolated",true).available,"exhausted quota refused");}
    {Fake f;std::get<m::Json::Object>(f.cfg.value)["mcp_servers"]=m::obj({{"unsafe",m::obj({})}});check(!m::interact(f,bundle,"isolated",true).available&&!f.called("thread/start"),"configured connectors refuse thread startup");}
    {Fake f;f.features=m::arr({m::obj({{"name","future_tool"},{"enabled",true}})});check(!m::interact(f,bundle,"isolated",true).available&&!f.called("thread/start"),"unknown enabled feature refuses model request");}
    {Fake f;f.skills=m::arr({m::obj({{"errors",m::arr({})},{"skills",m::arr({m::obj({{"enabled",true}})})}})});check(!m::interact(f,bundle,"isolated",true).available&&!f.called("thread/start"),"skills cannot add hidden context");}
    rejects([&]{m::checkSchema(root/"no-such-schema");},"missing schema closes gate");
    // Actual native process transport, not a fake protocol test.
    auto exe=fs::canonical(argv[0]);auto scratch=fs::temp_directory_path()/("pu-u3 space & test-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));fs::create_directory(scratch);
    struct Cleanup{fs::path p;~Cleanup(){std::error_code e;fs::remove_all(p,e);}}cleanup{scratch};
    fs::create_directories(scratch/"schema/v2");
    std::ofstream(scratch/"schema/GetAuthStatusParams.json")<<m::obj({{"properties",m::obj({{"includeToken",m::obj({})},{"refreshToken",m::obj({})}})}}).dump();
    std::ofstream(scratch/"schema/GetAuthStatusResponse.json")<<m::obj({{"properties",m::obj({{"authMethod",m::obj({{"enum",m::arr({"chatgpt"})}})}})}}).dump();
    auto makeThread=[&](const char* mode){return m::obj({{"properties",m::obj({
        {"sandbox",m::obj({{"$ref","#/definitions/Mode"}})},
        {"approvalPolicy",m::obj({{"enum",m::arr({"untrusted"})}})},
        {"ephemeral",m::obj({})},{"baseInstructions",m::obj({})},{"developerInstructions",m::obj({})},{"config",m::obj({})}})},
        {"definitions",m::obj({{"Mode",m::obj({{"enum",m::arr({mode})}})}})}});};
    auto turnSchema=m::obj({{"properties",m::obj({{"sandboxPolicy",m::obj({})},{"approvalPolicy",m::obj({})},{"input",m::obj({})},{"threadId",m::obj({})},{"readableRoots",m::obj({})},{"includePlatformDefaults",m::obj({})}})},
        {"enum",m::arr({"readOnly","restricted","untrusted"})}});
    std::ofstream(scratch/"schema/v2/TurnStartParams.json")<<turnSchema.dump();
    for(auto mode:{"read-only","readOnly"}){std::ofstream(scratch/"schema/v2/ThreadStartParams.json")<<makeThread(mode).dump();check(m::checkSchema(scratch/"schema")==mode,"thread permission spelling taken from installed schema reference");}
    std::ofstream(scratch/"schema/v2/ThreadStartParams.json")<<makeThread("workspace-write").dump();
    rejects([&]{m::checkSchema(scratch/"schema");},"write-only schema cannot pass read-only gate");
    // 0.155.1 omits v1 JSON exports. Runtime auth checks remain unchanged.
    std::ofstream(scratch/"schema/v2/ThreadStartParams.json")<<makeThread("read-only").dump();
    const auto paramsPath=scratch/"schema/GetAuthStatusParams.json";
    const auto responsePath=scratch/"schema/GetAuthStatusResponse.json";
    fs::remove(paramsPath);fs::remove(responsePath);
    const auto notificationPath=scratch/"schema/v2/AccountUpdatedNotification.json";
    const auto notification=m::obj({{"definitions",m::obj({
        {"AuthMode",m::obj({{"oneOf",m::arr({m::obj({{"enum",m::arr({"chatgpt"})}}),m::obj({{"enum",m::arr({"chatgptAuthTokens"})}})})}})},
        {"Plan",m::obj({{"enum",m::arr({"plus"})}})}})},
        {"properties",m::obj({{"authMode",m::obj({{"$ref","#/definitions/AuthMode"}})},{"planType",m::obj({{"$ref","#/definitions/Plan"}})}})}}).dump();
    std::ofstream(notificationPath)<<notification;
    check(m::checkSchema(scratch/"schema","codex-cli 0.155.1")=="read-only","reviewed 0.155.1 omitted-v1 export accepted");
    for(auto version:{"","codex-cli 0.155.0","codex-cli 0.155.10","codex-cli 0.155.1 extra"})
        rejects([&]{m::checkSchema(scratch/"schema",version);},"unknown version cannot bypass missing auth schemas");
    auto refusesWith=[&](const std::string& fragment,const char* label){
        std::string error;try{m::checkSchema(scratch/"schema","codex-cli 0.155.1");}catch(const std::exception& e){error=e.what();}
        check(error.find(fragment)!=std::string::npos,label);
    };
    std::ofstream(paramsPath)<<"{}";
    refusesWith("Missing App Server schema:","partial legacy schema pair cannot fall back");fs::remove(paramsPath);
    fs::remove(notificationPath);
    refusesWith("AccountUpdatedNotification.json","missing diagnostic names exact required schema");
    std::ofstream(notificationPath)<<"{";
    refusesWith("Invalid App Server JSON schema:","malformed JSON remains rejected with specific diagnostic");
    std::ofstream(notificationPath)<<m::obj({{"properties",m::obj({{"authMode",m::obj({{"enum",m::arr({"chatgpt"})}})},{"planType",m::obj({{"enum",m::arr({"plus"})}})}})}}).dump();
    refusesWith("cannot distinguish","generic ChatGPT-only label cannot satisfy external-mode distinction");
    std::ofstream(notificationPath)<<notification<<std::string(1048576-notification.size(),' ');
    check(m::checkSchema(scratch/"schema","codex-cli 0.155.1")=="read-only","exact 1 MiB schema boundary accepted");
    std::ofstream(notificationPath,std::ios::app)<<' ';
    refusesWith("1048577 bytes","oversize diagnostic reports actual size without raising limit");
    std::ofstream(notificationPath)<<notification;
    std::ofstream(scratch/"schema/v2/ThreadStartParams.json")<<makeThread("workspace-write").dump();
    refusesWith("cannot represent read-only","version compatibility cannot bypass read-only policy");
    std::ofstream(scratch/"schema/v2/ThreadStartParams.json")<<makeThread("read-only").dump();
    auto missingRoots=turnSchema;std::get<m::Json::Object>(std::get<m::Json::Object>(missingRoots.value).at("properties").value).erase("readableRoots");
    std::ofstream(scratch/"schema/v2/TurnStartParams.json")<<missingRoots.dump();
    refusesWith("lacks readableRoots","missing restricted roots remains a specific capability blocker");
    // Named profiles replace the old wire fields, but schema recognition is NOT isolation proof.
    auto profileThread=makeThread("read-only"),profileTurn=missingRoots;
    auto property=[](m::Json& schema,const char* name,m::Json value){
        std::get<m::Json::Object>(std::get<m::Json::Object>(schema.value).at("properties").value)[name]=value;
    };
    const auto stringOrNull=m::obj({{"type",m::arr({"string","null"})}});
    property(profileThread,"permissions",stringOrNull);property(profileTurn,"permissions",stringOrNull);
    const auto responseSchema=m::obj({{"properties",m::obj({{"activePermissionProfile",m::obj({{"anyOf",m::arr({
        m::obj({{"$ref","#/definitions/ActivePermissionProfile"}}),m::obj({{"type","null"}})})}})}})},
        {"definitions",m::obj({{"ActivePermissionProfile",m::obj({{"properties",m::obj({{"id",m::obj({{"type","string"}})}})}})}})}});
    auto writeProfiles=[&]{
        std::ofstream(scratch/"schema/v2/ThreadStartParams.json")<<profileThread.dump();
        std::ofstream(scratch/"schema/v2/TurnStartParams.json")<<profileTurn.dump();
        std::ofstream(scratch/"schema/v2/ThreadStartResponse.json")<<responseSchema.dump();
    };
    writeProfiles();
    auto diagnostic=m::inspectCompatibility(scratch/"schema","codex-cli 0.155.1");
    check(diagnostic.at("mechanism").str()=="experimental_named_permission_profiles","actual named-profile property contract recognized");
    check(!diagnostic.at("contextOnlyIsolationAccepted").boolean()&&!diagnostic.at("modelRequestSent").boolean(),"profile recognition cannot authorize inference");
    check(diagnostic.at("files").array().size()==3&&diagnostic.at("perFileLimitBytes").number()==1048576,"bounded per-schema measurements retained");
    refusesWith("no verified context-only native backend","recognized profile still fails closed at isolation gate");
    rejects([&]{m::inspectCompatibility(scratch/"schema","codex-cli 0.155.10");},"profile diagnostic is exact-version scoped");
    property(profileTurn,"permissions",m::obj({{"type","object"}}));writeProfiles();
    check(!m::inspectCompatibility(scratch/"schema","codex-cli 0.155.1").at("namedProfileRequests").boolean(),"wrong permissions type cannot masquerade as profile selection");
    std::get<m::Json::Object>(std::get<m::Json::Object>(profileTurn.value).at("properties").value).erase("permissions");
    std::get<m::Json::Object>(profileTurn.value)["definitions"]=m::obj({{"decoy",m::obj({{"properties",m::obj({{"permissions",stringOrNull}})}})}});
    writeProfiles();check(!m::inspectCompatibility(scratch/"schema","codex-cli 0.155.1").at("namedProfileRequests").boolean(),"nested decoy property cannot satisfy the request contract");
    property(profileTurn,"permissions",stringOrNull);writeProfiles();
    std::ofstream(scratch/"schema/v2/ThreadStartResponse.json")<<"{}";
    check(!m::inspectCompatibility(scratch/"schema","codex-cli 0.155.1").at("activeProfileProvenance").boolean(),"missing profile provenance is not reported as supported");
    std::ofstream(scratch/"schema/v2/ThreadStartResponse.json")<<std::string(1048577,' ');
    rejects([&]{m::inspectCompatibility(scratch/"schema","codex-cli 0.155.1");},"profile diagnostics retain one MiB limit");
    std::ofstream(scratch/"schema/v2/ThreadStartResponse.json")<<"{";
    rejects([&]{m::inspectCompatibility(scratch/"schema","codex-cli 0.155.1");},"malformed profile response schema rejected");
    writeProfiles();fs::remove(scratch/"schema/v2/ThreadStartResponse.json");
    rejects([&]{m::inspectCompatibility(scratch/"schema","codex-cli 0.155.1");},"missing profile response schema rejected");
    writeProfiles();answer.compatibility=diagnostic;
    auto saved=m::record(scratch,bundle,answer);std::ifstream evidence(saved);auto record=m::Json::parse(std::string(std::istreambuf_iterator<char>(evidence),{}));
    check(record.at("context").str()==bundle&&!record.at("progressAuthority").boolean(),"append-only advisory evidence has no grading authority");
    check(record.at("compatibility").dump()==diagnostic.dump(),"schema diagnosis preserved in machine-readable evidence");
    check(m::record(scratch,bundle,answer)!=saved&&fs::exists(saved),"new record preserves previous evidence");
#ifdef _WIN32
    _putenv_s("OPENAI_API_KEY","not-a-secret-test-value");
#else
    setenv("OPENAI_API_KEY","not-a-secret-test-value",1);
#endif
    const std::string literal="space & | < > % ^ \"quote\" trailing\\";
    {auto ch=m::spawn(exe,{"--pipe-child",literal},scratch,3000);ch->send("literal & input");auto response=m::Json::parse(ch->receive());check(response.at("echo").str()=="literal & input"&&response.at("literal").str()==literal,"native argv and stdin remain literal; stderr isolated");check(!response.at("secretInherited").boolean(),"provider secret not inherited");}
    {auto ch=m::spawn(exe,{"--hang"},scratch,100);rejects([&]{ch->receive();},"native child timeout");}
    rejects([&]{auto ch=m::spawn(scratch/"missing.exe",{},scratch,1000);ch->receive();},"missing executable safe failure");
    std::cout<<"PASS: "<<checks<<" U3 checks (fake provider + real process transport; no live account/model request)\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
