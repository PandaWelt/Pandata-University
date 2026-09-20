#pragma once
#include <filesystem>
#include <memory>
#include <string>
#include <vector>
namespace pu::mentor {
struct Channel {
    virtual ~Channel()=default;
    virtual void send(const std::string& line)=0;
    virtual std::string receive()=0;
};
// Direct argv, separate stderr (discarded, never interpreted as protocol), bounded lifetime.
// Destroying the channel kills the entire child job/process group.
std::unique_ptr<Channel> spawn(const std::filesystem::path&,const std::vector<std::string>&,
                               const std::filesystem::path&,int deadlineMs=45000);
}
