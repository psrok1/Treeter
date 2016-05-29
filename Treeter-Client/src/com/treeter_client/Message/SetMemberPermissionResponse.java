package com.treeter_client.Message;

import org.json.simple.JSONObject;

/**
 * Created by psrok1 on 28.05.2016.
 */
public class SetMemberPermissionResponse extends MessageResponse
{
    public SetMemberPermissionResponse(JSONObject jsonObj)
    {
        super(jsonObj);
    }

    public void process(IMessageProcessor processor)
    {
        processor.processMessage(this);
    }

}
