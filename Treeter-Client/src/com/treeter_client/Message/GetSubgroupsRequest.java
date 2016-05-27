package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class GetSubgroupsRequest extends MessageRequest
{
    String path;

    public GetSubgroupsRequest(String path)
    {
        this.path = path;
    }

    public String serialize()
    {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("request", "getSubgroups");
        jsonObject.put("path", path);
        return jsonObject.toJSONString();
    }
}
