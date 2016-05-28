package com.treeter_client.Model;

import java.lang.reflect.Member;

/**
 * Created by psrok1 on 27.05.2016.
 */
public enum MemberRole
{
    Standard,
    Moderator,
    PendingApproval,
    Unknown;

    @Override
    public String toString()
    {
        switch(this)
        {
            case Standard: return "standard";
            case Moderator: return "moderator";
            case PendingApproval: return "pending";
            default: return null;
        }
    }

    public static MemberRole fromString(String memberRole)
    {
        if(memberRole.equalsIgnoreCase("standard"))
            return MemberRole.Standard;
        else if(memberRole.equalsIgnoreCase("moderator"))
            return MemberRole.Moderator;
        else if(memberRole.equalsIgnoreCase("pending"))
            return MemberRole.PendingApproval;
        else
            return MemberRole.Unknown;
    }
}
