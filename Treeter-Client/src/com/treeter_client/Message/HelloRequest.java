package com.treeter_client.Message;


import org.json.simple.JSONObject;

public class HelloRequest extends MessageRequest
{
    public String serialize()
    {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("request", "hello");
        return jsonObject.toJSONString();
    }
}
