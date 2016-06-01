package com.treeter_client;

import com.treeter_client.Message.*;
import com.treeter_client.Model.*;
import com.treeter_client.View.ConnectView;
import com.treeter_client.View.MainView;

import javax.swing.*;
import java.util.ArrayList;

public class MainController
{
    ConnectView connectView;
    MainView mainView;

    boolean createUserFirst;
    String loggedUser;

    Client client;
    GroupTreeModel model;

    private MessageProcessor messageProcessor;

    MainController()
    {
        messageProcessor = new MessageProcessor(this);
        model = new GroupTreeModel();
    }

    public void connect(String address, boolean createUserFirst)
    {
        this.createUserFirst = createUserFirst;
        client = new Client();
        client.onConnect(new Client.EventListener()
        {
            @Override
            public void action()
            {
                try
                {
                    System.out.println("Action!");
                    HelloRequest helloRequest = new HelloRequest();
                    client.send(new HelloRequest());
                } catch(Exception e)
                {
                    client.close();
                }
            }
        });
        client.onMessage(new Client.MessageEventListener()
        {
            @Override
            public void action(MessageResponse message)
            {
                message.process(messageProcessor);
            }
        });

        client.onError(new Client.EventListener()
        {
            @Override
            public void action()
            {
                client.close();
                mainView.hide();
                connectView.show();
            }
        });
        client.open(address);
    }

    public static void main(String args[])
    {
        MainController controller = new MainController();

        controller.connectView = new ConnectView();
        controller.mainView = new MainView(controller.model);

        controller.connectView.attachController(controller);
        controller.mainView.attachController(controller);

        controller.connectView.show();
    }

    public void disconnect()
    {
        client.close();
        mainView.hide();
        connectView.show();
    }

    public GroupTreeModel createNewModel()
    {
        GroupTreeModel newModel = new GroupTreeModel();
        this.model = newModel;
        this.mainView.setModel(newModel);
        return newModel;
    }

    public void updateGroup(GroupModel groupModel)
    {
        if(model.isActiveGroup(groupModel))
        {
            if(groupModel.getPermissions() == MemberRole.PendingApproval)
                mainView.lockWithWaitingMessage("Oczekiwanie na akceptację przez moderatora grupy");
            else
                this.selectGroup(groupModel);
        }
        else
            model.notifyGroup(groupModel);
    }

    public void hideGroupIfActive(GroupModel groupModel)
    {
        if(model.isActiveGroup(groupModel))
        {
            model.setActiveGroup(null);
            mainView.lockWithEmptyLayout();
        }
    }

    public void selectGroup(GroupModel group)
    {
        this.model.setActiveGroup(group);
        if(group.isSynchronized())
        {
            mainView.updateGroup(group);
        } else
            mainView.lockWithWaitingMessage("Trwa pobieranie danych z serwera.");

        if(group.needSynchronization())
        {
            group.synchronizationStarted();
            this.client.send(new GetGroupUsersRequest(group.absolutePath));
            this.client.send(new GetMessagesRequest(group.absolutePath));
            this.client.send(new GetSubgroupsRequest(group.absolutePath));
        }
    }

    public void addSubgroup(String subgroup) {
        String path = model.getActiveGroup().absolutePath;
        this.client.send(new CreateSubgroupRequest(path, subgroup));
    }

    public void joinSubgroup(String subgroup) {
        String path = model.getActiveGroup().absolutePath;
        this.client.send(new AddMeToGroupRequest(path, subgroup));
    }

    public void deleteSubgroup(String subgroup)
    {
        String path = model.getActiveGroup().absolutePath;
        this.client.send(new RemoveSubgroupRequest(path, subgroup));
    }

    public void addMember(String memberName) {
        String path = model.getActiveGroup().absolutePath;
        this.client.send(new AddUserToGroupRequest(memberName, path));
    }

    public void rejectMember(String login) {
        String path = model.getActiveGroup().absolutePath;
        this.client.send(new RemoveUserFromGroupRequest(path, login));
    }

    public void makeModeratorMember(String login) {
        String path = model.getActiveGroup().absolutePath;
        this.client.send(new SetMemberPermissionRequest(path, login, MemberRole.Moderator));
    }

    public void makeStandardMember(String login)
    {
        String path = model.getActiveGroup().absolutePath;
        this.client.send(new SetMemberPermissionRequest(path, login, MemberRole.Standard));
    }

    public void acceptMember(String login)
    {
        String path = model.getActiveGroup().absolutePath;
        this.client.send(new SetMemberPermissionRequest(path, login, MemberRole.Standard));
    }

    public void sendMessage(String message)
    {
        String path = model.getActiveGroup().absolutePath;
        this.client.send(new SendMessageRequest(path, message));
    }
}

class MessageProcessor implements IMessageProcessor
{
    MainController controller;

    MessageProcessor(MainController mainController)
    {
        this.controller = mainController;
    }

    @Override
    public void processMessage(EchoResponse echoResponse)
    {
        return;
    }

    @Override
    public void processMessage(HelloResponse helloResponse)
    {
        try
        {
            controller.client.getCryptoProvider().importRSAPublicKey(helloResponse.getPublicKey());
            controller.client.getCryptoProvider().generateAESKey();
            String aesKey = controller.client.getCryptoProvider().exportAESKey();

            StartEncryptionRequest startEncryptionRequest = new StartEncryptionRequest(aesKey);
            controller.client.enableCryptoForNextResponse();
            controller.client.send(startEncryptionRequest);
        } catch(Exception e)
        {
            e.printStackTrace();
            controller.client.close();
        }
    }

    @Override
    public void processMessage(StartEncryptionResponse startEncryptionResponse)
    {
        String login = controller.connectView.getNick();
        String password = controller.connectView.getPassword();

        MessageRequest nextRequest;

        if(controller.createUserFirst)
            nextRequest = new CreateAccountRequest(login, password);
        else
            nextRequest = new AuthUserRequest(login, password);

        controller.client.send(nextRequest);
    }

    @Override
    public void processMessage(AuthUserResponse response)
    {
        if(response.getErrorCode() != ErrorCodeResponse.OK)
        {
            controller.connectView.showError(response.getErrorCode());
            controller.client.close();
            return;
        }

        ArrayList<String> paths = response.getPaths();

        GroupTreeModel model = this.controller.createNewModel();
        for(String path: paths)
            model.addGroup(path);

        controller.loggedUser = controller.connectView.getNick();

        controller.connectView.hide();
        controller.mainView.show();
        controller.selectGroup(model.getGroupByPath("/", false));
    }

    @Override
    public void processMessage(CreateAccountResponse response)
    {
        if(response.getErrorCode() != ErrorCodeResponse.OK)
        {
            controller.connectView.showError(response.getErrorCode());
            controller.client.close();
            return;
        }

        String login = controller.connectView.getNick();
        String password = controller.connectView.getPassword();

        MessageRequest nextRequest = new AuthUserRequest(login, password);
        controller.client.send(nextRequest);
    }

    @Override
    public void processMessage(AddMeToGroupResponse response)
    {
        if(response.getErrorCode() != ErrorCodeResponse.OK)
        {
            controller.mainView.showError(response.getErrorCode());
            return;
        }
    }

    @Override
    public void processMessage(AddUserToGroupResponse response)
    {
        if(response.getErrorCode() != ErrorCodeResponse.OK)
        {
            controller.mainView.showError(response.getErrorCode());
            return;
        }
    }

    @Override
    public void processMessage(CreateSubgroupResponse response)
    {
        if(response.getErrorCode() != ErrorCodeResponse.OK)
        {
            controller.mainView.showError(response.getErrorCode());
            return;
        }
    }

    @Override
    public void processMessage(GetGroupUsersResponse response)
    {
        if(response.getErrorCode() == ErrorCodeResponse.AccessDenied)
        {
            GroupModel group = controller.model.getGroupByPath(response.getPath(), false);
            group.setPermissions(MemberRole.PendingApproval);
            controller.updateGroup(group);
            group.getMemberList().setState(DataModelState.Unsynchronized);
            return;
        }

        if(response.getErrorCode() != ErrorCodeResponse.OK)
        {
            controller.mainView.showError(response.getErrorCode());
            return;
        }

        GroupModel group = controller.model.getGroupByPath(response.getPath(), false);
        GroupMemberListModel groupMembers = group.getMemberList();

        groupMembers.loadRemoteData(response.getMemberList());
        groupMembers.setState(DataModelState.Synchronized);

        if(group.isSynchronized())
            controller.updateGroup(group);
    }

    @Override
    public void processMessage(GetMessagesResponse response)
    {
        if(response.getErrorCode() == ErrorCodeResponse.AccessDenied)
        {
            GroupModel group = controller.model.getGroupByPath(response.getPath(), false);
            group.setPermissions(MemberRole.PendingApproval);
            controller.updateGroup(group);
            group.getMessageList().setState(DataModelState.Unsynchronized);
            return;
        }

        if(response.getErrorCode() != ErrorCodeResponse.OK)
        {
            controller.mainView.showError(response.getErrorCode());
            return;
        }

        GroupModel group = controller.model.getGroupByPath(response.getPath(), false);
        GroupMessageListModel groupMessages = group.getMessageList();

        groupMessages.loadRemoteData(response.getMessages());
        groupMessages.setState(DataModelState.Synchronized);

        if(group.isSynchronized())
            controller.updateGroup(group);
    }

    @Override
    public void processMessage(GetSubgroupsResponse response)
    {
        if(response.getErrorCode() == ErrorCodeResponse.AccessDenied)
        {
            GroupModel group = controller.model.getGroupByPath(response.getPath(), false);
            group.setPermissions(MemberRole.PendingApproval);
            controller.updateGroup(group);
            group.getSubgroupList().setState(DataModelState.Unsynchronized);
            return;
        }

        if(response.getErrorCode() != ErrorCodeResponse.OK)
        {
            controller.mainView.showError(response.getErrorCode());
            return;
        }

        GroupModel group = controller.model.getGroupByPath(response.getPath(), false);
        GroupSubgroupListModel subgroups = group.getSubgroupList();

        subgroups.loadRemoteData(response.getSubgroups());
        subgroups.setState(DataModelState.Synchronized);

        if(group.isSynchronized())
            controller.updateGroup(group);
    }

    @Override
    public void processMessage(RemoveSubgroupResponse response)
    {
        if(response.getErrorCode() != ErrorCodeResponse.OK)
        {
            controller.mainView.showError(response.getErrorCode());
            return;
        }
    }

    @Override
    public void processMessage(RemoveUserFromGroupResponse response)
    {
        if(response.getErrorCode() != ErrorCodeResponse.OK)
        {
            controller.mainView.showError(response.getErrorCode());
            return;
        }
    }

    @Override
    public void processMessage(SendMessageResponse response)
    {
        if(response.getErrorCode() != ErrorCodeResponse.OK)
        {
            controller.mainView.showError(response.getErrorCode());
            return;
        }
    }

    @Override
    public void processMessage(SetMemberPermissionResponse response)
    {
        if(response.getErrorCode() != ErrorCodeResponse.OK)
        {
            controller.mainView.showError(response.getErrorCode());
            return;
        }
    }

    @Override
    public void processMessage(NewMessageNotification newMessageNotification)
    {
        // Get notification elements
        String path = newMessageNotification.getPath();
        String message = newMessageNotification.getMessage();
        // Get group object from model
        GroupModel groupModel = controller.model.getGroupByPath(path, false);
        if(groupModel == null)
            return;
        // Append message to group model
        groupModel.getMessageList().addMessage(message);
        // Update view
        controller.updateGroup(groupModel);
    }

    @Override
    public void processMessage(AddedToGroupNotification addedToGroupNotification)
    {
        // Get notification elements
        String path = addedToGroupNotification.getPath();
        GroupMember member = addedToGroupNotification.getMember();
        // Is it me?
        if(member.login.equals(controller.loggedUser))
        {
            // I subscribe new group!
            controller.model.addGroup(path);
        } else
        {
            // Get group object from model
            GroupModel groupModel = controller.model.getGroupByPath(path, false);
            if(groupModel == null)
                return;
            // Add member to group model
            groupModel.getMemberList().addMember(member.login, member.role);
            // Update view
            controller.updateGroup(groupModel);
        }
    }

    @Override
    public void processMessage(RemovedFromGroupNotification removedFromGroupNotification)
    {
        // Get notification elements
        String path = removedFromGroupNotification.getPath();
        String username = removedFromGroupNotification.getUsername();
        // Get group object from model
        GroupModel groupModel = controller.model.getGroupByPath(path, false);
        if(groupModel == null)
            return;
        // Is it me?
        if(username.equals(controller.loggedUser))
        {
            // I have to unsubscribe group...
            controller.hideGroupIfActive(groupModel);
            controller.model.deleteGroup(path);
        } else
        {
            // Remove member from group model
            groupModel.getMemberList().removeMember(username);
            // Update view
            controller.updateGroup(groupModel);
        }
    }

    @Override
    public void processMessage(ModifiedMemberPermissionNotification modifiedMemberPermissionNotification)
    {
        // Get notification elements
        String path = modifiedMemberPermissionNotification.getPath();
        String username = modifiedMemberPermissionNotification.getUsername();
        MemberRole role = modifiedMemberPermissionNotification.getRole();
        // Get group object from model
        GroupModel groupModel = controller.model.getGroupByPath(path, false);
        if(groupModel == null)
            return;
        // Is it me?
        if(path.equals(controller.loggedUser))
        {
            groupModel.setPermissions(role);
        }
        // Set member permissions
        groupModel.getMemberList().setMemberPermissions(username, role);
        // Update view
        controller.updateGroup(groupModel);
    }

    @Override
    public void processMessage(AddedSubgroupNotification addedSubgroupNotification)
    {
        // Get notification elements
        String path = addedSubgroupNotification.getPath();
        String subgroup = addedSubgroupNotification.getSubgroup();

        // Get group object from model
        GroupModel groupModel = controller.model.getGroupByPath(path, false);
        if(groupModel == null)
            return;

        groupModel.getSubgroupList().addSubgroup(subgroup);
        // Update view
        controller.updateGroup(groupModel);
    }

    @Override
    public void processMessage(RemovedSubgroupNotification removedSubgroupNotification)
    {
        // Get notification elements
        String path = removedSubgroupNotification.getPath();
        String subgroup = removedSubgroupNotification.getSubgroup();

        // Get group object from model
        GroupModel groupModel = controller.model.getGroupByPath(path, false);
        if(groupModel == null)
            return;

        groupModel.getSubgroupList().deleteSubgroup(subgroup);
        // Update view
        controller.updateGroup(groupModel);
    }
}
