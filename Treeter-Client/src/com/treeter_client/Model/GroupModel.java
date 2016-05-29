package com.treeter_client.Model;

/**
 * Created by psrok1 on 27.05.2016.
 */
public class GroupModel extends DataModel
{
    GroupMemberListModel memberListModel;
    GroupMessageListModel messageListModel;
    GroupSubgroupListModel subgroupListModel;

    public String name;
    public String absolutePath;

    public GroupUIState uiState;
    MemberRole permissions;

    public GroupModel(String path)
    {
        this.absolutePath = path;
        this.name = path.substring(path.lastIndexOf('/')+1);

        memberListModel = new GroupMemberListModel();
        messageListModel = new GroupMessageListModel();
        subgroupListModel = new GroupSubgroupListModel();

        permissions = MemberRole.Unknown;
        uiState = GroupUIState.Default;
    }

    public boolean isPendingApproval()
    {
        return permissions.equals(MemberRole.PendingApproval);
    }

    public boolean isSynchronized()
    {
        return  memberListModel.equals(DataModelState.Synchronized) &&
                messageListModel.equals(DataModelState.Synchronized) &&
                subgroupListModel.equals(DataModelState.Synchronized);
    }

    public boolean needSynchronization()
    {
        return  memberListModel.equals(DataModelState.Unsynchronized) &&
                messageListModel.equals(DataModelState.Unsynchronized) &&
                subgroupListModel.equals(DataModelState.Unsynchronized);
    }

    public GroupMemberListModel getMemberList()
    {
        return memberListModel;
    }

    public GroupMessageListModel getMessageList()
    {
        return messageListModel;
    }

    public GroupSubgroupListModel getSubgroupList()
    {
        return subgroupListModel;
    }

    public void setPermissions(MemberRole permissions)
    {
        this.permissions = permissions;
    }

    public MemberRole getPermissions()
    {
        return this.permissions;
    }

    public void synchronizationStarted()
    {
        this.setState(DataModelState.Incomplete);
        messageListModel.setState(DataModelState.Incomplete);
        subgroupListModel.setState(DataModelState.Incomplete);
        memberListModel.setState(DataModelState.Incomplete);
    }
}
