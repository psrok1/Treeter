package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class CreateAccountResponse extends MessageResponse
{
    public CreateAccountResponse(JSONObject jsonObj)
    {
        super(jsonObj);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }
}
