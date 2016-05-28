package com.treeter_client.Message;

import com.treeter_client.Model.MemberRole;
import org.json.simple.JSONObject;

/**
 * Created by psrok1 on 28.05.2016.
 */
public class ModifiedMemberPermissionNotification extends MessageResponse
{
    public ModifiedMemberPermissionNotification(JSONObject jsonObj)
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

    public String getUsername()
    {
        return (String)this.jsonObject.get("username");
    }

    public MemberRole getRole()
    {
        return MemberRole.fromString((String)this.jsonObject.get("role"));
    }
}

