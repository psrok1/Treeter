#ifndef GROUPMESSAGE_H
#define GROUPMESSAGE_H

#include <string>
#include <chrono>

namespace Model
{
    struct GroupMessage
    {
        typedef std::chrono::system_clock::time_point Timestamp;
    private:
        const std::string author;
        const std::string message;
        const Timestamp   timestamp;
    public:
        GroupMessage(std::string author, std::string message,
                     Timestamp timestamp = std::chrono::system_clock::now()):
            author(author), message(message), timestamp(timestamp) { }

        const std::string getAuthor() const { return author; }
        const std::string getMessage() const { return message; }
        const Timestamp   getTimestamp() const { return timestamp; }
    };
}

#endif // GROUPMESSAGE_H
