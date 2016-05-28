package com.treeter_client.Model;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

public class GroupMessageFormatter
{
    private String author;
    private String message;
    private Date timestamp;

    public GroupMessageFormatter(String author, String message, Date timestamp)
    {
        this.author = author;
        this.message = message;
        this.timestamp = timestamp;
    }

    @Override
    public String toString()
    {
        DateFormat df = new SimpleDateFormat("HH:mm:ss");
        String strTimestamp = df.format(this.timestamp);

        return String.format("[%s] %s: %s", strTimestamp, this.author, this.message);
    }
}
