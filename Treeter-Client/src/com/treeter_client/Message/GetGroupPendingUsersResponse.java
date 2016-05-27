package com.treeter_client.Message;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.util.ArrayList;

public class GetGroupPendingUsersResponse extends MessageResponse
{
    public GetGroupPendingUsersResponse(JSONObject jsonObj)
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
}
