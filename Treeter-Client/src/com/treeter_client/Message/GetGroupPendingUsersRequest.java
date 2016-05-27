package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class GetGroupPendingUsersRequest extends MessageRequest
{
    String path;

    public GetGroupPendingUsersRequest(String path)
    {
        this.path = path;
    }

    public String serialize()
    {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("request", "getGroupPendingUsers");
        jsonObject.put("path", path);
        return jsonObject.toJSONString();
    }
}
