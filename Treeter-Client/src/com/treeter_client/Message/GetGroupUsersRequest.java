package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class GetGroupUsersRequest extends MessageRequest
{
    String path;

    public GetGroupUsersRequest(String path)
    {
        this.path = path;
    }

    public String serialize()
    {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("request", "getGroupUsers");
        jsonObject.put("path", path);
        return jsonObject.toJSONString();
    }
}
