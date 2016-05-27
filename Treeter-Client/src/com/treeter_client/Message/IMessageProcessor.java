package com.treeter_client.Message;

// metody obslugi response
public interface IMessageProcessor
{
    void processMessage(HelloResponse helloResponse);
    void processMessage(StartEncryptionResponse startEncryptionResponse);
    void processMessage(AddMeToGroupResponse response);
    void processMessage(AddUserToGroupResponse response);
    void processMessage(CreateAccountResponse response);
    void processMessage(CreateGroupResponse response);
    void processMessage(AuthUserResponse response);
    void processMessage(GetGroupPathsResponse response);
    void processMessage(GetGroupPendingUsersResponse response);
}