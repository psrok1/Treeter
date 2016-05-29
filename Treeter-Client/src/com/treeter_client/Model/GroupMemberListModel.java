package com.treeter_client.Model;

import java.lang.reflect.Member;
import java.security.acl.Group;
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

        GroupMember member = new GroupMember();
        member.login = login;
        member.role = role;
        this.memberList.add(member);
    }

    private GroupMember findElementByLogin(String login)
    {
        for(GroupMember member: this.memberList)
            if(member.login.equals(login))
                return member;

        return null;
    }

    public void removeMember(String login)
    {
        if(this.getState() != DataModelState.Synchronized)
            return;

        GroupMember member = findElementByLogin(login);
        if(member != null)
            this.memberList.remove(member);
    }

    public void setMemberPermissions(String login, MemberRole role)
    {
        if(this.getState() != DataModelState.Synchronized)
            return;

        GroupMember member = findElementByLogin(login);
        if(member != null)
            member.role = role;
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
