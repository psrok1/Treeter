package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class SendMessageResponse extends MessageResponse
{
    public SendMessageResponse(JSONObject jsonObj)
    {
        super(jsonObj);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }
}
