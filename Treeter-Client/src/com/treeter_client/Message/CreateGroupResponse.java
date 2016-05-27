package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class CreateGroupResponse extends MessageResponse
{
    public CreateGroupResponse(JSONObject jsonObj)
    {
        super(jsonObj);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }
}
