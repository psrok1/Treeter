package com.treeter_client.Message;


import org.json.simple.JSONObject;

public class EchoRequest extends MessageRequest
{
    String message;

    public EchoRequest(String message)
    {
        this.message = message;
    }

    public String serialize()
    {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("request", "echo");
        jsonObject.put("message", message);
        return jsonObject.toJSONString();
    }
}