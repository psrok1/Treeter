package com.treeter_client.Message;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.util.ArrayList;

public class AuthUserResponse extends MessageResponse
{
    public AuthUserResponse(JSONObject jsonObject)
    {
        super(jsonObject);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }

    public ArrayList<String> getPaths()
    {
        ArrayList<String> result = new ArrayList<String>();
        JSONArray jsonArray = (JSONArray) jsonObject.get("paths");
        for(int i=0; i<jsonArray.size(); i++)
        {
            result.add((String)jsonArray.get(i));
        }
        return result;
    }
}
