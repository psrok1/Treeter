package com.treeter_client.Message;

import org.json.simple.JSONObject;

/**
 * Created by psrok1 on 28.05.2016.
 */
public class NewMessageNotification extends MessageResponse
{
    public NewMessageNotification(JSONObject jsonObj)
    {
        super(jsonObj);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }

    public String getPath()
    {
        return (String)this.jsonObject.get("path");
    }

    public String getMessage()
    {
        // @TODO Message format
        return (String)this.jsonObject.get("message");
    }
}
