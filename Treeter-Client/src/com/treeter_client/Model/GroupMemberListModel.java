package com.treeter_client.Model;

import java.lang.reflect.Member;
import java.util.ArrayList;

/**
 * Created by psrok1 on 27.05.2016.
 */
public class GroupMemberListModel extends DataModel
{
    ArrayList<GroupMember> memberList;

    public void loadRemoteData(ArrayList<GroupMember> memberList)
    {
        this.memberList = memberList;
        this.setState(DataModelState.Synchronized);
    }

    public void addMember(String login, MemberRole role)
    {
        if(this.getState() != DataModelState.Synchronized)
            return;
    }

    public void removeMember(String login)
    {
        if(this.getState() != DataModelState.Synchronized)
            return;
    }

    public void setMemberPermissions(String login, MemberRole role)
    {
        if(this.getState() != DataModelState.Synchronized)
            return;
    }

    public GroupMember[] getData()
    {
        if(this.getState() != DataModelState.Synchronized)
            return null;

        GroupMember[] arr = new GroupMember[memberList.size()];
        arr = memberList.toArray(arr);

        return arr;
    }
}
