package com.treeter_client.Message;

import org.json.simple.JSONObject;

public class CreateAccountRequest extends MessageRequest
{
    String login, password;

    public CreateAccountRequest(String login, String password)
    {
        this.login = login;
        this.password = password;
    }

    public String serialize()
    {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("request", "createAccount");
        jsonObject.put("login", login);
        jsonObject.put("password", password);
        return jsonObject.toJSONString();
    }
}
