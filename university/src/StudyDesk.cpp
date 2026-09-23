#include "StudyDesk.h"
#include "RepositoryActions.h"
#include <array>
#include <charconv>
#include <fstream>
#include <sstream>
namespace pu {
namespace {
const std::array<std::string,2> ids{"u1-boundaries", "003-object-types"};
std::ifstream read(const std::filesystem::path& path) {
    rejectLinks(path);
    if (!std::filesystem::is_regular_file(path)) throw Error("Missing course file: " + path.filename().string());
    std::ifstream in(path, std::ios::binary);
    if (!in) throw Error("Cannot open course file: " + path.filename().string());
    return in;
}
std::string warnings(const LoadedProgress& p) {
    std::string out;
    for (const auto& w : p.warnings) out += "Recovery: " + w + "\n";
    return out;
}
}
std::size_t studyNumber(const std::string& text) {
    std::size_t number = 0;
    const auto [end, error] = std::from_chars(text.data(), text.data()+text.size(), number);
    if (error != std::errc{} || end != text.data()+text.size() || number == 0 || std::to_string(number) != text)
        throw Error("Use one exact lesson/step number; no paths, spaces or arguments.");
    return number;
}
StudyDesk::StudyDesk(std::filesystem::path root) {
    rejectLinks(root); root_ = std::filesystem::canonical(root);
}
Mission StudyDesk::definition(std::size_t n) const {
    if (n == 0 || n > ids.size()) throw Error("Lesson number outside the reviewed catalog. Use lessons.");
    auto in = read(root_ / "course/university/missions" / (ids[n-1]+".pum"));
    auto m = loadMission(in);
    if (m.id != ids[n-1]) throw Error("Registered lesson ID differs from its definition; nothing migrated.");
    return m;
}
LoadedProgress StudyDesk::progress(const Mission& m) const {
    auto dir = root_ / "course/university/progress" / m.id;
    rejectLinks(dir);
    return loadProgress(dir,m);
}
void StudyDesk::prerequisites(std::size_t n, const Mission& m) const {
    auto in = read(root_ / "course/university/pilot.puc");
    const auto pilot = loadPilot(in);
    for (const auto& required : m.prerequisites) {
        std::size_t registered = 0;
        for (std::size_t i=0; i<ids.size(); ++i) if(ids[i]==required) registered=i+1;
        if (registered) {
            if (registered >= n) throw Error("Prerequisite must be an earlier registered lesson.");
            const auto dependency = open(registered);
            if (currentStep(dependency.progress.value) != dependency.mission.steps.size())
                throw Error("Finish prepared steps in " + required + " before opening this lesson.");
            if (!dependency.progress.warnings.empty())
                throw Error("Prerequisite recovery needs review: " + warnings(dependency.progress));
        } else {
            Mission requiredOnly; requiredOnly.prerequisites={required};
            checkPrerequisites(requiredOnly,pilot);
        }
    }
}
void StudyDesk::validateDefinitions() const {
    auto in=read(root_/"course/university/pilot.puc");const auto pilot=loadPilot(in);
    for(std::size_t n=1;n<=ids.size();++n) {
        const auto m=definition(n);
        for(const auto& required:m.prerequisites) {
            std::size_t registered=0;
            for(std::size_t i=0;i<ids.size();++i) if(ids[i]==required)registered=i+1;
            if(registered && registered>=n)throw Error("Prerequisite must be an earlier registered lesson.");
            if(!registered) {Mission one;one.prerequisites={required};checkPrerequisites(one,pilot);}
        }
    }
}
StudyLesson StudyDesk::open(std::size_t n) const {
    auto m = definition(n); prerequisites(n,m);
    auto p = progress(m);
    return {n,std::move(m),std::move(p)};
}
std::vector<LessonCard> StudyDesk::catalog() const {
    std::vector<LessonCard> cards;
    for (std::size_t n=1;n<=ids.size();++n) {
        LessonCard card; card.number=n;card.id=ids[n-1];card.title=card.id;card.state="blocked";
        try {
            const auto m=definition(n);card.title=m.title;card.total=m.steps.size();
            const auto lesson=open(n);card.completed=currentStep(lesson.progress.value);
            card.state=card.completed==card.total?"recorded":(lesson.progress.generation?"in progress":"ready");
            card.detail=warnings(lesson.progress);
        } catch (const std::exception& e) {card.detail=e.what();}
        cards.push_back(std::move(card));
    }
    return cards;
}
StudyLesson StudyDesk::resume() const {
    const auto cards=catalog();
    for(const auto& card:cards) if(card.state=="in progress") return open(card.number);
    for(const auto& card:cards) if(card.state=="ready") return open(card.number);
    for(auto it=cards.rbegin();it!=cards.rend();++it) if(it->state=="recorded") return open(it->number);
    throw Error("No accessible lesson. Use the catalog diagnostics; preserve existing progress.");
}
void StudyDesk::save(StudyLesson& lesson, LoadedProgress candidate) const {
    // Recheck definition/prerequisites at the mutation boundary; never silently migrate.
    const auto current=open(lesson.number);
    if(current.mission.fingerprint!=lesson.mission.fingerprint || current.mission.id!=lesson.mission.id)
        throw DefinitionMismatch("Definition changed during study. Restart; no progress was migrated.");
    if(candidate.generation!=lesson.progress.generation) throw Error("Invalid study generation.");
    auto dir=root_/"course/university/progress"/lesson.mission.id;
    rejectLinks(dir);
    saveProgress(dir,lesson.mission,candidate);
    lesson.progress=std::move(candidate);
}
std::string StudyDesk::home() const {
    std::ostringstream out;
    out << "\nOFFLINE COURSE DESK\n";
    for(const auto& c:catalog()) {
        out<<c.number<<" | "<<c.title<<" | "<<c.state<<" "<<c.completed<<"/"<<c.total<<'\n';
        if(!c.detail.empty())out<<terminalText(c.detail)<<'\n';
    }
    out<<"Recorded means evidence collected, not correctness, independent mastery or reviewer acceptance.\n"
         "lesson <number> | resume | history | step <number> | report | feedback | help | quit\n"
         "Offline: prepared lessons, hints, saved responses, file views and explicit existing repository actions.\n"
         "Deferred: live mentor, proposal application, source writes, Rescue and Maintenance automation.\n";
    return out.str();
}
std::string StudyDesk::history(const StudyLesson& l) {
    std::ostringstream out;out<<"SAVED STUDY HISTORY: "<<l.mission.id<<'\n';
    for(std::size_t i=0;i<l.mission.steps.size();++i) {
        const auto& p=l.progress.value.steps[i];
        out<<i+1<<" | "<<l.mission.steps[i].title<<" | "<<(p.complete?"recorded":"pending")<<" | hints "<<p.hintsUsed<<'\n';
        for(const auto& a:p.attempts)out<<"Learner-reported: "<<terminalText(a)<<'\n';
    }
    return out.str();
}
std::string StudyDesk::reviewStep(const StudyLesson& l,std::size_t n) {
    const auto current=currentStep(l.progress.value);
    if(n==0 || n>l.mission.steps.size() || n>current+1)throw Error("Review only a recorded step or the active step; future steps are not unlocked.");
    const auto& s=l.mission.steps[n-1];
    return "READ-ONLY STEP REVIEW: "+s.title+"\n"+s.why+"\nAction: "+s.action+"\nExpected: "+s.expected+"\nResume position unchanged.\n";
}
std::string StudyDesk::report(const StudyLesson& l) {
    return "STUDY REPORT (display only; copy this text for review)\nLesson: "+l.mission.id+
        "\nDefinition: "+l.mission.fingerprint+"\nSnapshot: "+std::to_string(l.progress.generation)+"\n"+
        history(l)+"Authority: answers are learner-reported; response gates check presence only.\n"
        "No compiler pass, Git checkpoint, mastery or product completion is inferred.\n"
        "Machine action evidence remains separate in course/university/evidence/u2.\n";
}
std::string StudyDesk::feedback() {
    return "PREPARED OFFLINE TROUBLESHOOTING\n"
        "Missing header/type: check the include and declaration visible to that .cpp.\n"
        "Unresolved external at link: check exact declaration/definition signature and that the .cpp is compiled.\n"
        "Duplicate symbol: check whether a non-inline definition was copied into a shared header.\n"
        "Response missing: answer <one short observation>, then next; this is not code grading.\n"
        "Save conflict: close the older session and relaunch. Never delete snapshots to bypass it.\n"
        "Definition mismatch/recovery: preserve progress and matching definitions; request an explicit repair.\n"
        "run-check tests University, not your PandaBank refactor. Build PandaBank in Visual Studio.\n"
        "Stuck: use hint, more, syntax or professional. No AI credit is needed.\n";
}
}
