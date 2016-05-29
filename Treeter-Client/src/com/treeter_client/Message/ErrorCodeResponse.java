package com.treeter_client.Message;

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
    ObjectExist(2),
    AccessDenied(3),
    MultipleSessionsNotAllowed(4);


    private int errNo;

    private static Map<Integer, ErrorCodeResponse> map = new HashMap<Integer, ErrorCodeResponse>();

    private static final Map<ErrorCodeResponse, String> errorStringMap =
            Collections.unmodifiableMap(new HashMap<ErrorCodeResponse, String>() {{
                put(OK, "OK... Ej... Dlaczego pokazuję ten komunikat?");
                put(BadName, "Podana nazwa jest nieprawidłowa.");
                put(ObjectExist, "Obiekt już istnieje.");
                put(AccessDenied, "Dostęp zabroniony.");
                put(MultipleSessionsNotAllowed, "Już istnieje zalogowana sesja dla tego użytkownika.");
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
