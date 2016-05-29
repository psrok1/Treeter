package com.treeter_client.Message;

import com.treeter_client.Model.GroupMember;
import com.treeter_client.Model.MemberRole;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.util.ArrayList;

public class GetGroupUsersResponse extends MessageResponse
{
    public GetGroupUsersResponse(JSONObject jsonObj)
    {
        super(jsonObj);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }

    public String getPath()
    {
        return (String) jsonObject.get("path");
    }

    public ArrayList<GroupMember> getMemberList()
    {
        ArrayList<GroupMember> result = new ArrayList<GroupMember>();
        JSONArray jsonArray = (JSONArray) jsonObject.get("users");
        for(int i=0; i<jsonArray.size(); i++)
        {
            GroupMember groupMember = new GroupMember();
            JSONObject jsonMemberObject = (JSONObject) jsonArray.get(i);

            groupMember.login = (String)jsonMemberObject.get("login");

            String jsonMemberRole = (String)jsonMemberObject.get("role");
            groupMember.role = MemberRole.fromString(jsonMemberRole);

            result.add(groupMember);
        }
        return result;
    }
}
