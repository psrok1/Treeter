package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class AddMeToGroupResponse extends MessageResponse
{
    public AddMeToGroupResponse(JSONObject jsonObject)
    {
        super(jsonObject);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }
}
