package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class AddUserToGroupRequest extends MessageRequest
{
    String username, path;
    boolean moderator;

    public AddUserToGroupRequest(String username, String path, boolean moderator)
    {
        this.username = username;
        this.path = path;
        this.moderator = moderator;
    }

    public String serialize()
    {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("request", "addUserToGroup");
        jsonObject.put("username", username);
        jsonObject.put("path", path);
        jsonObject.put("moderator", moderator);
        return jsonObject.toJSONString();
    }
}
