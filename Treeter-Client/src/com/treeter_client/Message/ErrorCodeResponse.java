package com.treeter_client.Message;

import com.sun.java.util.jar.pack.Package;
import com.sun.org.apache.xpath.internal.operations.Mult;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by psrok1 on 28.05.2016.
 */
public enum ErrorCodeResponse
{
    OK(0),
    BadName(1),
    NotAMember(2),
    AccessDenied(3),
    MultipleSessionsNotAllowed(4),
    RootNotAllowed(5),
    UserExist(6),
    UserNotExist(7),
    GroupExist(8),
    GroupNotExist(9),
    MemberExist(10),
    MemberNotExist(11),
    NotLoggedIn(12);

    private int errNo;

    private static Map<Integer, ErrorCodeResponse> map = new HashMap<Integer, ErrorCodeResponse>();

    private static final Map<ErrorCodeResponse, String> errorStringMap =
            Collections.unmodifiableMap(new HashMap<ErrorCodeResponse, String>() {{
                put(OK, "OK");
                put(BadName, "Podana nazwa jest nieprawidłowa.");
                put(AccessDenied, "Dostęp zabroniony.");
                put(MultipleSessionsNotAllowed, "Już istnieje zalogowana sesja dla tego użytkownika.");
                put(NotAMember, "Nie jesteś uczestnikiem grupy.");
                put(RootNotAllowed, "Operacja niedozwolona na grupie głównej.");
                put(UserExist, "Użytkownik już istnieje");
                put(UserNotExist, "Użytkownik nie istnieje");
                put(GroupExist, "Grupa już istnieje");
                put(GroupNotExist, "Grupa nie istnieje");
                put(MemberExist, "Użytkownik już jest uczestnikiem");
                put(MemberNotExist, "Użytkownik nie jest uczestnikiem");
                put(NotLoggedIn, "Nie jesteś zalogowany!");
            }});

    static {
        for (ErrorCodeResponse errorCodeEnum : ErrorCodeResponse.values()) {
            map.put(errorCodeEnum.errNo, errorCodeEnum);
        }
    }

    private ErrorCodeResponse(final int _errNo) { errNo = _errNo; }

    public String toString()
    {
        return ErrorCodeResponse.errorStringMap.get(this);
    }

    public static ErrorCodeResponse valueOf(Long errNo) {
        return map.get(errNo.intValue());
    }
}
