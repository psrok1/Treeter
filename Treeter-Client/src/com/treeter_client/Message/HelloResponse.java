package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class HelloResponse extends MessageResponse
{
    public HelloResponse(JSONObject jsonObj)
    {
        super(jsonObj);
    }

    public String getPublicKey()
    {
        return (String) jsonObject.get("publicKey");
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }
}