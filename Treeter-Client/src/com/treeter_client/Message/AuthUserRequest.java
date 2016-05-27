package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class AuthUserRequest extends MessageRequest
{
    String login, password;

    public AuthUserRequest(String login, String password)
    {
        this.login = login;
        this.password = password;
    }

    public String serialize()
    {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("request", "authUser");
        jsonObject.put("login", login);
        jsonObject.put("password", password);
        return jsonObject.toJSONString();
    }
}
