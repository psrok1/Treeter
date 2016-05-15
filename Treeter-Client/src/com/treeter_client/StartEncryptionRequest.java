package com.treeter_client;

import org.json.simple.JSONObject;

public class StartEncryptionRequest extends MessageRequest
{
    String encryptedKey;

    StartEncryptionRequest(String encryptedKey)
    {
        this.encryptedKey = encryptedKey;
    }

    public String serialize()
    {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("request", "startEncryption");
        jsonObject.put("encryptedKey", encryptedKey);
        return jsonObject.toJSONString();
    }
}
