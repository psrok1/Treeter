package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class AuthUserResponse extends MessageResponse
{
    public AuthUserResponse(JSONObject jsonObject)
    {
        super(jsonObject);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }
}
