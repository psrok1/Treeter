package com.treeter_client.Model;

import java.util.ArrayList;

/**
 * Created by psrok1 on 27.05.2016.
 */
public class GroupMessageListModel extends DataModel
{
    ArrayList<String> messageList;

    public void loadRemoteData(ArrayList<String> messageList)
    {
        this.messageList = messageList;
        this.setState(DataModelState.Synchronized);
    }

    public void addMessage(String message)
    {
        if(!this.getState().equals(DataModelState.Synchronized))
            return;

        this.messageList.add(message);
    }

    public String[] getData()
    {
        if(!this.getState().equals(DataModelState.Synchronized))
            return null;

        String[] arr = new String[messageList.size()];
        arr = messageList.toArray(arr);

        return arr;
    }
}
