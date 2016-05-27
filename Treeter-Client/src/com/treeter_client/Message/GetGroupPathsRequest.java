package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class GetGroupPathsRequest extends MessageRequest
{
    public String serialize()
    {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("request", "getGroupPaths");
        return jsonObject.toJSONString();
    }
}
