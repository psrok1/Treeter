package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class RemoveGroupResponse extends MessageResponse
{
    public RemoveGroupResponse(JSONObject jsonObj)
    {
        super(jsonObj);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }
}
