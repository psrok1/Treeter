package com.treeter_client.Message;

import com.treeter_client.Message.EchoResponse;
import com.treeter_client.Message.HelloResponse;
import com.treeter_client.Message.StartEncryptionResponse;

// metody obslugi response
public interface IMessageProcessor
{
    void processMessage(EchoResponse response);
    void processMessage(HelloResponse helloResponse);
    void processMessage(StartEncryptionResponse startEncryptionResponse);
}