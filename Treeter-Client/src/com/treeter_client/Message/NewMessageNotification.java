package com.treeter_client.Message;

import com.treeter_client.Model.GroupMessageFormatter;
import org.json.simple.JSONObject;

import java.util.Date;

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
        JSONObject msgObject = (JSONObject)this.jsonObject.get("message");
        String msgAuthor = (String)msgObject.get("author");
        String msgContent = (String)msgObject.get("content");
        Date msgTimestamp = new Date((long)msgObject.get("timestamp") * 1000);

        return (new GroupMessageFormatter(msgAuthor, msgContent, msgTimestamp)).toString();
    }
}
