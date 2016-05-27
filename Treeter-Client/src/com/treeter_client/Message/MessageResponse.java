package com.treeter_client.Message;

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
                put("addMeToGroup", (JSONObject jsonObject) -> new AddMeToGroupResponse(jsonObject));
                put("addUserToGroup", (JSONObject jsonObject) -> new AddUserToGroupResponse(jsonObject));
                put("authUser", (JSONObject jsonObject) -> new AuthUserResponse(jsonObject));
                put("createAccount", (JSONObject jsonObject) -> new CreateAccountResponse(jsonObject));
                put("createGroup", (JSONObject jsonObject) -> new CreateGroupResponse(jsonObject));
                put("getGroupPaths", (JSONObject jsonObject) -> new GetGroupPathsResponse(jsonObject));
                put("getGroupPendingUsers", (JSONObject jsonObject) -> new GetGroupPendingUsersResponse(jsonObject));
                put("getGroupUsers", (JSONObject jsonObject) -> new GetGroupUsersResponse(jsonObject));
                put("getMessages", (JSONObject jsonObject) -> new GetMessagesResponse(jsonObject));
                put("getSubgroups", (JSONObject jsonObject) -> new GetSubgroupsResponse(jsonObject));
                put("removeGroup", (JSONObject jsonObject) -> new RemoveGroupResponse(jsonObject));
                put("removeUserFromGroup", (JSONObject jsonObject) -> new RemoveUserFromGroupResponse(jsonObject));
                put("sendMessage", (JSONObject jsonObject) -> new SendMessageResponse(jsonObject));
            }});

    public static MessageResponse deserialize(String message) throws ParseException
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