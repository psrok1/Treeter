package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class GetMessagesRequest extends MessageRequest
{
    String path;

    public GetMessagesRequest(String path)
    {
        this.path = path;
    }

    public String serialize()
    {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("request", "getMessages");
        jsonObject.put("path", path);
        return jsonObject.toJSONString();
    }
}
