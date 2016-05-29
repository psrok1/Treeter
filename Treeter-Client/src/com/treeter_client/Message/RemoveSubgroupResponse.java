package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class RemoveSubgroupResponse extends MessageResponse
{
    public RemoveSubgroupResponse(JSONObject jsonObj)
    {
        super(jsonObj);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }
}
