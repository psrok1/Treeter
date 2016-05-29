package com.treeter_client.Message;

import com.treeter_client.Model.GroupMessageFormatter;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.util.ArrayList;
import java.util.Date;

public class GetMessagesResponse extends MessageResponse
{
    public GetMessagesResponse(JSONObject jsonObj)
    {
        super(jsonObj);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }

    public String getPath()
    {
        return (String) jsonObject.get("path");
    }

    public ArrayList<String> getMessages()
    {
        ArrayList<String> result = new ArrayList<String>();
        JSONArray jsonArray = (JSONArray) jsonObject.get("messages");
        for(int i=0; i<jsonArray.size(); i++)
        {
            JSONObject msgObject = (JSONObject)jsonArray.get(i);
            String msgAuthor = (String)msgObject.get("author");
            String msgContent = (String)msgObject.get("content");
            Date msgTimestamp = new Date((long)msgObject.get("timestamp") * 1000);
            result.add((new GroupMessageFormatter(msgAuthor, msgContent, msgTimestamp)).toString());
        }
        return result;
    }
}
