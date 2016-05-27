package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class SendMessageRequest extends MessageRequest
{
    String path, message;

    public SendMessageRequest(String path, String message)
    {
        this.path = path;
        this.message = message;
    }

    public String serialize()
    {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("request", "sendMessage");
        jsonObject.put("path", path);
        jsonObject.put("message", message);
        return jsonObject.toJSONString();
    }
}
