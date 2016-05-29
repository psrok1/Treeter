package com.treeter_client.Message;

import com.treeter_client.Model.GroupMember;
import com.treeter_client.Model.MemberRole;
import org.json.simple.JSONObject;

/**
 * Created by psrok1 on 28.05.2016.
 */
public class AddedToGroupNotification extends MessageResponse
{
    public AddedToGroupNotification(JSONObject jsonObj)
    {
        super(jsonObj);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }

    public String getPath()
    {
        return (String)this.jsonObject.get("path");
    }

    public GroupMember getMember()
    {
        GroupMember member = new GroupMember();
        member.login = (String)this.jsonObject.get("username");
        member.role = MemberRole.fromString((String)this.jsonObject.get("role"));
        return member;
    }
}

