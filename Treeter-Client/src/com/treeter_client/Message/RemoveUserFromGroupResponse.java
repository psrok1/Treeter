package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class RemoveUserFromGroupResponse extends MessageResponse
{
    public RemoveUserFromGroupResponse(JSONObject jsonObj)
    {
        super(jsonObj);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }
}
