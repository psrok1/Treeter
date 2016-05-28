package com.treeter_client.Message;

import org.json.simple.JSONObject;

/**
 * Created by psrok1 on 28.05.2016.
 */
public class RemovedFromGroupNotification extends MessageResponse
{
    public RemovedFromGroupNotification(JSONObject jsonObj)
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

    public String getUsername()
    {
        return (String)this.jsonObject.get("username");
    }
}
