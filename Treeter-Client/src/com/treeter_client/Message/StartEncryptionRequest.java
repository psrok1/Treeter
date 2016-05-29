package com.treeter_client.Message;

import com.treeter_client.Message.MessageRequest;
import org.json.simple.JSONObject;

public class StartEncryptionRequest extends MessageRequest
{
    String encryptedKey;

    public StartEncryptionRequest(String encryptedKey)
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
