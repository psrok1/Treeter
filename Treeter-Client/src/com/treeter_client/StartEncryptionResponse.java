package com.treeter_client;

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