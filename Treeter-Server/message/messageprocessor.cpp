#include "messageprocessor.h"
#include "messageincoming.h"
#include "messageoutgoing.h"
#include "../server.h"

#include "model/datamodel.h"
#include "model/group.h"
#include "model/user.h"

MessageProcessor::MessageProcessor(Connection::Reference conn): connection(conn) { }

/** EchoRequest **/

bool MessageProcessor::processRequest(const EchoRequest& req)
{
    MessageOutgoing::Reference response(new EchoResponse(req.getMessage()));

    this->connection->sendMessage(response);
    return true;
}

/** HelloRequest **/

bool MessageProcessor::processRequest(const HelloRequest &)
{
    connection->rsaContext = Crypto::RSAProvider::getKey();

    MessageOutgoing::Reference response(new HelloResponse(connection->rsaContext.getEncodedPublicKey()));

    this->connection->sendMessage(response);
    return true;
}

/** StartEncryptionRequest **/

bool MessageProcessor::processRequest(const StartEncryptionRequest &req)
{
    connection->aesContext = connection->rsaContext.decodeAESKey(req.getEncryptedKey());

    MessageOutgoing::Reference response(new StartEncryptionResponse());

    this->connection->sendMessage(response);
    return true;
}

/** AuthUserRequest **/

bool MessageProcessor::processRequest(const AuthUserRequest &req)
{
    auto userRef = this->connection->model->getUserByLogin(req.getLogin());

    if(!userRef || !userRef->verifyPassword(req.getPassword()))
    {
        MessageOutgoing::Reference response(new AuthUserResponse(ResponseErrorCode::AccessDenied));
        this->connection->sendMessage(response);
        return false;
    }

    // Add relation to current connection
    if(!userRef->registerConnection(this->connection))
    {
        MessageOutgoing::Reference response(new AuthUserResponse(ResponseErrorCode::MultipleSessionsNotAllowed));
        this->connection->sendMessage(response);
        return false;
    }

    // Add relation connection -> user (sometimes can be useful)
    this->connection->user = userRef;

    // Established!
    // Get group paths list
    std::list<std::string> groupPaths = userRef->listGroupPaths();

    MessageOutgoing::Reference response(new AuthUserResponse(groupPaths));
    this->connection->sendMessage(response);

    return true;
}

/** CreateAccountRequest **/

bool MessageProcessor::processRequest(const CreateAccountRequest &req)
{
    if(!Model::User::validateLogin(req.getLogin()))
    {
        MessageOutgoing::Reference response(new CreateAccountResponse(ResponseErrorCode::BadName));
        this->connection->sendMessage(response);
        return false;
    }

    auto userRef = this->connection->model->addUser(req.getLogin(), req.getPassword());

    if(!userRef)
    {
        MessageOutgoing::Reference response(new CreateAccountResponse(ResponseErrorCode::UserExist));
        this->connection->sendMessage(response);
        return false;
    }

    MessageOutgoing::Reference response(new CreateAccountResponse());
    this->connection->sendMessage(response);
    return true;
}

/** CreateSubgroupRequest **/

bool MessageProcessor::processRequest(const CreateSubgroupRequest &req)
{
    std::shared_ptr<Model::User> userRef = this->connection->user;

    if(!userRef)
    {
        MessageOutgoing::Reference response(new CreateSubgroupResponse(ResponseErrorCode::NotLoggedIn));
        this->connection->sendMessage(response);
        return false;
    }

    std::shared_ptr<Model::Group> groupRef = userRef->getGroupByPath(req.getParentPath());

    if(!groupRef)
    {
        MessageOutgoing::Reference response(new CreateSubgroupResponse(ResponseErrorCode::NotAMember));
        this->connection->sendMessage(response);
        return false;
    }

    if(groupRef->getMemberPermission(userRef->getLogin()) != MemberRole::Moderator)
    {
        MessageOutgoing::Reference response(new CreateSubgroupResponse(ResponseErrorCode::AccessDenied));
        this->connection->sendMessage(response);
        return false;
    }

    std::shared_ptr<Model::Group> newGroupRef = groupRef->createGroup(req.getSubgroupName());
    if(!newGroupRef)
    {
        MessageOutgoing::Reference response(new CreateSubgroupResponse(ResponseErrorCode::GroupExist));
        this->connection->sendMessage(response);
        return false;
    }

    newGroupRef->addMember(newGroupRef, userRef, MemberRole::Moderator);

    MessageOutgoing::Reference response(new CreateSubgroupResponse());
    this->connection->sendMessage(response);
    return true;
}

/** RemoveSubgroupRequest **/

bool MessageProcessor::processRequest(const RemoveSubgroupRequest &req)
{
    std::shared_ptr<Model::User> userRef = this->connection->user;

    if(!userRef)
    {
        MessageOutgoing::Reference response(new RemoveSubgroupResponse(ResponseErrorCode::NotLoggedIn));
        this->connection->sendMessage(response);
        return false;
    }

    std::shared_ptr<Model::Group> groupRef = userRef->getGroupByPath(req.getParentPath());

    if(!groupRef)
    {
        MessageOutgoing::Reference response(new RemoveSubgroupResponse(ResponseErrorCode::NotAMember));
        this->connection->sendMessage(response);
        return false;
    }

    if(groupRef->getMemberPermission(userRef->getLogin()) != MemberRole::Moderator)
    {
        MessageOutgoing::Reference response(new RemoveSubgroupResponse(ResponseErrorCode::AccessDenied));
        this->connection->sendMessage(response);
        return false;
    }

    if(!groupRef->deleteGroup(req.getSubgroupName()))
    {
        MessageOutgoing::Reference response(new RemoveSubgroupResponse(ResponseErrorCode::GroupNotExist));
        this->connection->sendMessage(response);
        return false;
    }

    MessageOutgoing::Reference response(new RemoveSubgroupResponse());
    this->connection->sendMessage(response);
    return true;
}

/** GetSubgroupsRequest **/

bool MessageProcessor::processRequest(const GetSubgroupsRequest &req)
{
    std::shared_ptr<Model::User> userRef = this->connection->user;

    if(!userRef)
    {
        MessageOutgoing::Reference response(new GetSubgroupsResponse(ResponseErrorCode::NotLoggedIn));
        this->connection->sendMessage(response);
        return false;
    }

    std::shared_ptr<Model::Group> groupRef = userRef->getGroupByPath(req.getPath());

    if(!groupRef)
    {
        MessageOutgoing::Reference response(new GetSubgroupsResponse(ResponseErrorCode::NotAMember));
        this->connection->sendMessage(response);
        return false;
    }

    if(groupRef->getMemberPermission(userRef->getLogin()) == MemberRole::PendingApproval)
    {
        MessageOutgoing::Reference response(new GetSubgroupsResponse(
                                                req.getPath(),
                                                std::list<std::string>(),
                                                ResponseErrorCode::AccessDenied));
        this->connection->sendMessage(response);
        return false;
    }
\
    std::list<std::string> groupNames = groupRef->listGroupNames();

    MessageOutgoing::Reference response(new GetSubgroupsResponse(req.getPath(), groupNames));
    this->connection->sendMessage(response);
    return true;
}

/** AddUserToGroupRequest **/

bool MessageProcessor::processRequest(const AddUserToGroupRequest &req)
{   
    std::shared_ptr<Model::User> userRef = this->connection->user;

    if(!userRef)
    {
        MessageOutgoing::Reference response(new AddUserToGroupResponse(ResponseErrorCode::NotLoggedIn));
        this->connection->sendMessage(response);
        return false;
    }

    if(req.getPath() == "/")
    {
        MessageOutgoing::Reference response(new AddUserToGroupResponse(ResponseErrorCode::RootNotAllowed));
        this->connection->sendMessage(response);
        return false;
    }

    std::shared_ptr<Model::Group> groupRef = userRef->getGroupByPath(req.getPath());

    if(!groupRef)
    {
        MessageOutgoing::Reference response(new AddUserToGroupResponse(ResponseErrorCode::NotAMember));
        this->connection->sendMessage(response);
        return false;
    }

    if(groupRef->getMemberPermission(userRef->getLogin()) != MemberRole::Moderator)
    {
        MessageOutgoing::Reference response(new AddUserToGroupResponse(ResponseErrorCode::AccessDenied));
        this->connection->sendMessage(response);
        return false;
    }

    std::shared_ptr<Model::User> reqUserRef = this->connection->model->getUserByLogin(req.getUsername());

    if(!reqUserRef)
    {
        MessageOutgoing::Reference response(new AddUserToGroupResponse(ResponseErrorCode::UserNotExist));
        this->connection->sendMessage(response);
        return false;
    }

    if(!groupRef->addMember(groupRef, reqUserRef, MemberRole::Member))
    {
        MessageOutgoing::Reference response(new AddUserToGroupResponse(ResponseErrorCode::MemberExist));
        this->connection->sendMessage(response);
        return false;
    }

    MessageOutgoing::Reference response(new AddUserToGroupResponse());
    this->connection->sendMessage(response);

    return true;
}

/** RemoveUserFromGroupRequest **/

bool MessageProcessor::processRequest(const RemoveUserFromGroupRequest &req)
{
    std::shared_ptr<Model::User> userRef = this->connection->user;

    if(!userRef)
    {
        MessageOutgoing::Reference response(new RemoveUserFromGroupResponse(ResponseErrorCode::NotLoggedIn));
        this->connection->sendMessage(response);
        return false;
    }

    if(req.getPath() == "/")
    {
        MessageOutgoing::Reference response(new RemoveUserFromGroupResponse(ResponseErrorCode::RootNotAllowed));
        this->connection->sendMessage(response);
        return false;
    }

    std::shared_ptr<Model::Group> groupRef = userRef->getGroupByPath(req.getPath());

    if(!groupRef)
    {
        MessageOutgoing::Reference response(new RemoveUserFromGroupResponse(ResponseErrorCode::NotAMember));
        this->connection->sendMessage(response);
        return false;
    }

    if(groupRef->getMemberPermission(userRef->getLogin()) != MemberRole::Moderator)
    {
        MessageOutgoing::Reference response(new RemoveUserFromGroupResponse(ResponseErrorCode::AccessDenied));
        this->connection->sendMessage(response);
        return false;
    }

    std::shared_ptr<Model::User> reqUserRef = this->connection->model->getUserByLogin(req.getUsername());

    if(!reqUserRef)
    {
        MessageOutgoing::Reference response(new RemoveUserFromGroupResponse(ResponseErrorCode::UserNotExist));
        this->connection->sendMessage(response);
        return false;
    }

    if(!groupRef->deleteMember(req.getUsername()))
    {
        MessageOutgoing::Reference response(new RemoveUserFromGroupResponse(ResponseErrorCode::MemberNotExist));
        this->connection->sendMessage(response);
        return false;
    }

    MessageOutgoing::Reference response(new RemoveUserFromGroupResponse());
    this->connection->sendMessage(response);

    return true;
}

/** GetGroupUsersRequest **/

bool MessageProcessor::processRequest(const GetGroupUsersRequest &req)
{
    std::shared_ptr<Model::User> userRef = this->connection->user;

    if(!userRef)
    {
        MessageOutgoing::Reference response(new GetGroupUsersResponse(ResponseErrorCode::NotLoggedIn));
        this->connection->sendMessage(response);
        return false;
    }

    std::shared_ptr<Model::Group> groupRef = userRef->getGroupByPath(req.getPath());

    if(!groupRef)
    {
        MessageOutgoing::Reference response(new GetGroupUsersResponse(ResponseErrorCode::NotAMember));
        this->connection->sendMessage(response);
        return false;
    }

    if(groupRef->getMemberPermission(userRef->getLogin()) == MemberRole::PendingApproval)
    {
        MessageOutgoing::Reference response(new GetGroupUsersResponse(
                                                req.getPath(),
                                                std::list<std::pair<std::string,MemberRole>>(),
                                                ResponseErrorCode::AccessDenied));
        this->connection->sendMessage(response);
        return false;
    }

    auto membersList = groupRef->listOfMembers();

    MessageOutgoing::Reference response(new GetGroupUsersResponse(req.getPath(), membersList));
    this->connection->sendMessage(response);
    return true;
}

/** AddMeToGroupRequest **/

bool MessageProcessor::processRequest(const AddMeToGroupRequest &req)
{
    std::shared_ptr<Model::User> userRef = this->connection->user;

    if(!userRef)
    {
        MessageOutgoing::Reference response(new AddMeToGroupResponse(ResponseErrorCode::NotLoggedIn));
        this->connection->sendMessage(response);
        return false;
    }

    std::shared_ptr<Model::Group> parentGroupRef = userRef->getGroupByPath(req.getParentPath());

    if(!parentGroupRef)
    {
        MessageOutgoing::Reference response(new AddMeToGroupResponse(ResponseErrorCode::NotAMember));
        this->connection->sendMessage(response);
        return false;
    }

    if(parentGroupRef->getMemberPermission(userRef->getLogin()) == MemberRole::PendingApproval)
    {
        MessageOutgoing::Reference response(new AddMeToGroupResponse(ResponseErrorCode::AccessDenied));
        this->connection->sendMessage(response);
        return false;
    }

    std::shared_ptr<Model::Group> groupRef = parentGroupRef->getGroupByName(req.getSubgroup());

    if(!groupRef)
    {
        MessageOutgoing::Reference response(new AddMeToGroupResponse(ResponseErrorCode::GroupNotExist));
        this->connection->sendMessage(response);
        return false;
    }

    if(!groupRef->addMember(groupRef, userRef, MemberRole::PendingApproval))
    {
        MessageOutgoing::Reference response(new AddMeToGroupResponse(ResponseErrorCode::MemberExist));
        this->connection->sendMessage(response);
        return false;
    }

    MessageOutgoing::Reference response(new AddMeToGroupResponse());
    this->connection->sendMessage(response);

    return true;
}

/** SendMessageRequest **/

bool MessageProcessor::processRequest(const SendMessageRequest &req)
{
    std::shared_ptr<Model::User> userRef = this->connection->user;

    if(!userRef)
    {
        MessageOutgoing::Reference response(new SendMessageResponse(ResponseErrorCode::NotLoggedIn));
        this->connection->sendMessage(response);
        return false;
    }

    std::shared_ptr<Model::Group> groupRef = userRef->getGroupByPath(req.getPath());

    if(!groupRef)
    {
        MessageOutgoing::Reference response(new SendMessageResponse(ResponseErrorCode::NotAMember));
        this->connection->sendMessage(response);
        return false;
    }

    if(groupRef->getMemberPermission(userRef->getLogin()) == MemberRole::PendingApproval)
    {
        MessageOutgoing::Reference response(new SendMessageResponse(ResponseErrorCode::AccessDenied));
        this->connection->sendMessage(response);
        return false;
    }

    groupRef->sendMessage(Model::GroupMessage(userRef->getLogin(), req.getMessage()));

    MessageOutgoing::Reference response(new SendMessageResponse());
    this->connection->sendMessage(response);
    return true;
}

/** GetMessagesRequest **/

bool MessageProcessor::processRequest(const GetMessagesRequest &req)
{
    std::shared_ptr<Model::User> userRef = this->connection->user;

    if(!userRef)
    {
        MessageOutgoing::Reference response(new GetMessagesResponse(ResponseErrorCode::NotLoggedIn));
        this->connection->sendMessage(response);
        return false;
    }

    std::shared_ptr<Model::Group> groupRef = userRef->getGroupByPath(req.getPath());

    if(!groupRef)
    {
        MessageOutgoing::Reference response(new GetMessagesResponse(ResponseErrorCode::NotAMember));
        this->connection->sendMessage(response);
        return false;
    }

    if(groupRef->getMemberPermission(userRef->getLogin()) == MemberRole::PendingApproval)
    {
        MessageOutgoing::Reference response(new GetMessagesResponse(
                                                req.getPath(),
                                                std::list<Model::GroupMessage>(),
                                                ResponseErrorCode::AccessDenied));
        this->connection->sendMessage(response);
        return false;
    }

    std::list<Model::GroupMessage> messages = groupRef->getMessages();

    MessageOutgoing::Reference response(new GetMessagesResponse(req.getPath(), messages));
    this->connection->sendMessage(response);
    return true;
}

/** SetMemberPermissionRequest **/

bool MessageProcessor::processRequest(const SetMemberPermissionRequest &req)
{
    std::shared_ptr<Model::User> userRef = this->connection->user;

    if(!userRef)
    {
        MessageOutgoing::Reference response(new SetMemberPermissionResponse(ResponseErrorCode::NotLoggedIn));
        this->connection->sendMessage(response);
        return false;
    }

    std::shared_ptr<Model::Group> groupRef = userRef->getGroupByPath(req.getPath());

    if(!groupRef)
    {
        MessageOutgoing::Reference response(new SetMemberPermissionResponse(ResponseErrorCode::NotAMember));
        this->connection->sendMessage(response);
        return false;
    }

    if(groupRef->getMemberPermission(userRef->getLogin()) != MemberRole::Moderator)
    {
        MessageOutgoing::Reference response(new SetMemberPermissionResponse(ResponseErrorCode::AccessDenied));
        this->connection->sendMessage(response);
        return false;
    }

    if(!groupRef->setMemberPermission(req.getUsername(), req.getRole()))
    {
        MessageOutgoing::Reference response(new SetMemberPermissionResponse(ResponseErrorCode::MemberNotExist));
        this->connection->sendMessage(response);
        return false;
    }

    MessageOutgoing::Reference response(new SetMemberPermissionResponse());
    this->connection->sendMessage(response);
    return true;
}
