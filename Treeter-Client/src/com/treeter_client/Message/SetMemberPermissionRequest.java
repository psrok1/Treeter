package com.treeter_client.Message;

import com.treeter_client.Model.MemberRole;
import org.json.simple.JSONObject;

/**
 * Created by psrok1 on 28.05.2016.
 */
public class SetMemberPermissionRequest extends MessageRequest
{
    String path;
    String username;
    String role;

    public SetMemberPermissionRequest(String path, String username, MemberRole role)
    {
        this.path = path;
        this.username = username;
        this.role = role.toString();
    }

    public String serialize()
    {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("request", "setMemberPermission");
        jsonObject.put("username", username);
        jsonObject.put("role", role);
        jsonObject.put("path", path);
        return jsonObject.toJSONString();
    }
}
