package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class RemoveUserFromGroupRequest extends MessageRequest
{
    String path, username;

    public RemoveUserFromGroupRequest(String path, String username)
    {
        this.path = path;
        this.username = username;
    }

    public String serialize()
    {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("request", "removeUserFromGroup");
        jsonObject.put("username", username);
        jsonObject.put("path", path);
        return jsonObject.toJSONString();
    }
}
