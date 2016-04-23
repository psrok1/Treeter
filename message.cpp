#include "message.h"
#include "connection.h"

void Message::send()
{
    this->connection->sendMessage(this->toString());
}
