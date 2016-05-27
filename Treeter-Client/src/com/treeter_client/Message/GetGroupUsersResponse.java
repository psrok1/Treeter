package com.treeter_client.Message;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.util.ArrayList;

public class GetGroupUsersResponse extends MessageResponse
{
    public GetGroupUsersResponse(JSONObject jsonObj)
    {
        super(jsonObj);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }

    public ArrayList<String> getUsers()
    {
        ArrayList<String> result = new ArrayList<String>();
        JSONArray jsonArray = (JSONArray) jsonObject.get("users");
        for(int i=0; i<jsonArray.size(); i++)
        {
            result.add((String)jsonArray.get(i));
        }
        return result;
    }

    public ArrayList<String> getModerators()
    {
        ArrayList<String> result = new ArrayList<String>();
        JSONArray jsonArray = (JSONArray) jsonObject.get("moderators");
        for(int i=0; i<jsonArray.size(); i++)
        {
            result.add((String)jsonArray.get(i));
        }
        return result;
    }
}
