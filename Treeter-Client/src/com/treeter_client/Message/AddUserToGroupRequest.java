package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class AddUserToGroupRequest extends MessageRequest
{
    String username, path;

    public AddUserToGroupRequest(String username, String path)
    {
        this.username = username;
        this.path = path;
    }

    public String serialize()
    {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("request", "addUserToGroup");
        jsonObject.put("username", username);
        jsonObject.put("path", path);
        return jsonObject.toJSONString();
    }
}
