package com.treeter_client.Model;

import java.util.ArrayList;

/**
 * Created by psrok1 on 27.05.2016.
 */
public class GroupSubgroupListModel extends DataModel
{
    ArrayList<String> subgroupNames;

    public void loadRemoteData(ArrayList<String> subgroupNamesList)
    {
        this.subgroupNames = subgroupNamesList;
        this.setState(DataModelState.Synchronized);
    }

    public void addSubgroup(String subgroupName)
    {
        if(!this.getState().equals(DataModelState.Synchronized))
            return;

        this.subgroupNames.add(subgroupName);
    }

    public void deleteSubgroup(String subgroupName)
    {
        if(!this.getState().equals(DataModelState.Synchronized))
            return;

        this.subgroupNames.remove(subgroupName);
    }

    public String[] getData()
    {
        if(!this.getState().equals(DataModelState.Synchronized))
            return null;

        String[] arr = new String[subgroupNames.size()];
        arr = subgroupNames.toArray(arr);

        return arr;
    }
}
