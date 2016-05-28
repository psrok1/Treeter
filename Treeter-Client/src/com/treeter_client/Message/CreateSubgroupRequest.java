package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class CreateSubgroupRequest extends MessageRequest
{
    String path, subgroup;

    public CreateSubgroupRequest(String path, String subgroup)
    {
        this.path = path;
        this.subgroup = subgroup;
    }

    public String serialize()
    {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("request", "createSubgroup");
        jsonObject.put("path", path);
        jsonObject.put("subgroup", subgroup);
        return jsonObject.toJSONString();
    }
}
