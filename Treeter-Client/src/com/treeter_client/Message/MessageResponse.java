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

    private static final Map<String,MessageResponseFactory> responseMap =
            Collections.unmodifiableMap(new HashMap<String,MessageResponseFactory>() {{
                put("echo",            (JSONObject jsonObj) -> new EchoResponse(jsonObj));
                put("hello",           (JSONObject jsonObj) -> new HelloResponse(jsonObj));
                put("startEncryption", (JSONObject jsonObj) -> new StartEncryptionResponse(jsonObj));
                put("addMeToGroup", (JSONObject jsonObject) -> new AddMeToGroupResponse(jsonObject));
                put("addUserToGroup", (JSONObject jsonObject) -> new AddUserToGroupResponse(jsonObject));
                put("authUser", (JSONObject jsonObject) -> new AuthUserResponse(jsonObject));
                put("createAccount", (JSONObject jsonObject) -> new CreateAccountResponse(jsonObject));
                put("getGroupUsers", (JSONObject jsonObject) -> new GetGroupUsersResponse(jsonObject));
                put("getMessages", (JSONObject jsonObject) -> new GetMessagesResponse(jsonObject));
                put("getSubgroups", (JSONObject jsonObject) -> new GetSubgroupsResponse(jsonObject));
                put("createSubgroup", (JSONObject jsonObject) -> new CreateSubgroupResponse(jsonObject));
                put("removeSubgroup", (JSONObject jsonObject) -> new RemoveSubgroupResponse(jsonObject));
                put("removeUserFromGroup", (JSONObject jsonObject) -> new RemoveUserFromGroupResponse(jsonObject));
                put("sendMessage", (JSONObject jsonObject) -> new SendMessageResponse(jsonObject));
                put("setMemberPermission", (JSONObject jsonObject) -> new SetMemberPermissionResponse(jsonObject));
            }});

    private static final Map<String,MessageResponseFactory> notificationMap =
            Collections.unmodifiableMap(new HashMap<String,MessageResponseFactory>() {{
                put("newMessage", (JSONObject jsonObject) -> new NewMessageNotification(jsonObject));
                put("addedToGroup", (JSONObject jsonObject) -> new AddedToGroupNotification(jsonObject));
                put("removedFromGroup", (JSONObject jsonObject) -> new RemovedFromGroupNotification(jsonObject));
                put("modifiedMemberPermission", (JSONObject jsonObject) -> new ModifiedMemberPermissionNotification(jsonObject));
                put("addedSubgroup", (JSONObject jsonObject) -> new AddedSubgroupNotification(jsonObject));
                put("removedSubgroup", (JSONObject jsonObject) -> new RemovedSubgroupNotification(jsonObject));
            }});

    public static MessageResponse deserialize(String message) throws ParseException
    {
        JSONParser jsonParser = new JSONParser();
        message = message.substring(0,message.lastIndexOf("}")+1);
        JSONObject jsonObject = (JSONObject) jsonParser.parse(message);

        if(jsonObject.containsKey("response"))
        {
            String responseType = (String) jsonObject.get("response");
            return responseMap.get(responseType).create(jsonObject);
        }
        else if(jsonObject.containsKey("notification"))
        {
            String notificationType = (String) jsonObject.get("notification");
            return notificationMap.get(notificationType).create(jsonObject);
        }
        return null;
    }

    public MessageResponse(JSONObject jsonObj)
    {
        jsonObject = jsonObj;
    }

    public ErrorCodeResponse getErrorCode()
    {
        if(jsonObject.containsKey("error"))
            return ErrorCodeResponse.OK;
        else
            return ErrorCodeResponse.valueOf((int)jsonObject.get("error"));
    }

    public abstract void process(IMessageProcessor processor);
}