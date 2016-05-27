package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class AddUserToGroupResponse extends MessageResponse
{
    public AddUserToGroupResponse(JSONObject jsonObject)
    {
        super(jsonObject);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }
}
