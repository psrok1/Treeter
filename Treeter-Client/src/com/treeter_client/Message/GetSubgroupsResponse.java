package com.treeter_client.Message;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.util.ArrayList;

public class GetSubgroupsResponse extends MessageResponse
{
    public GetSubgroupsResponse(JSONObject jsonObj)
    {
        super(jsonObj);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }

    public ArrayList<String> getSubgroups()
    {
        ArrayList<String> result = new ArrayList<String>();
        JSONArray jsonArray = (JSONArray) jsonObject.get("subgroups");
        for(int i=0; i<jsonArray.size(); i++)
        {
            result.add((String)jsonArray.get(i));
        }
        return result;
    }
}
