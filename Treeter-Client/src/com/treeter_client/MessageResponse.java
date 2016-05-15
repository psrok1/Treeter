package com.treeter_client;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

public abstract class MessageResponse
{
    protected JSONObject jsonObject;

    // tworzenie obiektow response
    private interface MessageResponseFactory {
        MessageResponse create(JSONObject jsonObj);
    }

    private static final Map<String,MessageResponseFactory> factoryMap =
            Collections.unmodifiableMap(new HashMap<String,MessageResponseFactory>() {{
                put("echo",            (JSONObject jsonObj) -> new EchoResponse(jsonObj));
                put("hello",           (JSONObject jsonObj) -> new HelloResponse(jsonObj));
                put("startEncryption", (JSONObject jsonObj) -> new StartEncryptionResponse(jsonObj));
            }});

    static MessageResponse deserialize(String message) throws ParseException
    {
        JSONParser jsonParser = new JSONParser();
        message = message.substring(0,message.lastIndexOf("}")+1);
        JSONObject jsonObject = (JSONObject) jsonParser.parse(message);
        String responseType = (String) jsonObject.get("response");
        return factoryMap.get(responseType).create(jsonObject);
    }

    public MessageResponse(JSONObject jsonObj)
    {
        jsonObject = jsonObj;
    }

    public abstract void process(IMessageProcessor processor);
}

// metody obslugi response
interface IMessageProcessor
{
    void processMessage(EchoResponse response);
    void processMessage(HelloResponse helloResponse);
    void processMessage(StartEncryptionResponse startEncryptionResponse);
}