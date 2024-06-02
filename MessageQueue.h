#include <queue>
#include <mutex>

class MessageQueue
{
public:
    void push(const std::string& message);
    std::string pop();
    bool empty();

private:
    std::queue<std::string> messages;
    std::mutex mtx;
};
