#include "MessageQueue.h"

void MessageQueue::push(const std::string& message)
{
    std::lock_guard<std::mutex> lock(mtx);
    messages.push(message);
}

std::string MessageQueue::pop()
{
    std::lock_guard<std::mutex> lock(mtx);
    if (!messages.empty())
    {
        std::string message = messages.front();
        messages.pop();
        return message;
    }
    else
    {
        return "";
    }
}

bool MessageQueue::empty()
{
    std::lock_guard<std::mutex> lock(mtx);
    return messages.empty();
}
