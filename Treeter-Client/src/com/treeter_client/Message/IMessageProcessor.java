package com.treeter_client.Message;

// metody obslugi response
public interface IMessageProcessor
{
    void processMessage(HelloResponse helloResponse);
    void processMessage(EchoResponse echoResponse);
    void processMessage(StartEncryptionResponse startEncryptionResponse);
    void processMessage(AddMeToGroupResponse response);
    void processMessage(AddUserToGroupResponse response);
    void processMessage(CreateAccountResponse response);
    void processMessage(CreateSubgroupResponse response);
    void processMessage(AuthUserResponse response);
    void processMessage(GetGroupUsersResponse response);
    void processMessage(GetMessagesResponse response);
    void processMessage(GetSubgroupsResponse response);
    void processMessage(RemoveSubgroupResponse response);
    void processMessage(RemoveUserFromGroupResponse response);
    void processMessage(SendMessageResponse response);
    void processMessage(SetMemberPermissionResponse setMemberPermissionResponse);
}