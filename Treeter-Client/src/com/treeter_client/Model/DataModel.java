package com.treeter_client.Model;

/**
 * Created by psrok1 on 27.05.2016.
 */
public class DataModel
{
    private DataModelState state = DataModelState.Unsynchronized;

    public DataModelState getState()
    {
        return state;
    }

    public void setState(DataModelState newState)
    {
        state = newState;
    }
}
