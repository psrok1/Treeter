package com.treeter_client.Model;

/**
 * Created by psrok1 on 27.05.2016.
 */
public class GroupModel extends DataModel
{
    GroupMemberListModel memberListModel;
    GroupMessageListModel messageListModel;
    GroupSubgroupListModel subgroupListModel;

    String name;
    String absolutePath;

    GroupUIState uiState;
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
}
