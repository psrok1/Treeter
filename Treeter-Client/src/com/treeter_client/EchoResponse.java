package com.treeter_client;

import org.json.simple.JSONObject;

public class EchoResponse extends MessageResponse
{

    public EchoResponse(JSONObject jsonObj)
    {
        super(jsonObj);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }

    public String getMessage()
    {
        return (String)jsonObject.get("message");
    }
}