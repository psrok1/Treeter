package com.treeter_client.Message;

import com.treeter_client.Message.IMessageProcessor;
import com.treeter_client.Message.MessageResponse;
import org.json.simple.JSONObject;

public class StartEncryptionResponse extends MessageResponse
{

    public StartEncryptionResponse(JSONObject jsonObj)
    {
        super(jsonObj);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }
}