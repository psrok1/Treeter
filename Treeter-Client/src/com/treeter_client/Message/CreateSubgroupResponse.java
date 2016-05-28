package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class CreateSubgroupResponse extends MessageResponse
{
    public CreateSubgroupResponse(JSONObject jsonObj)
    {
        super(jsonObj);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }
}
