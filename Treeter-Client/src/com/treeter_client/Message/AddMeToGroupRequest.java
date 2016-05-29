package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class AddMeToGroupRequest extends MessageRequest
{
    String path, subgroup;

    public AddMeToGroupRequest(String path, String subgroup)
    {
        this.path = path;
        this.subgroup = subgroup;
    }

    public String serialize()
    {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("request", "addMeToGroup");
        jsonObject.put("path", path);
        jsonObject.put("subgroup", subgroup);
        return jsonObject.toJSONString();
    }
}
