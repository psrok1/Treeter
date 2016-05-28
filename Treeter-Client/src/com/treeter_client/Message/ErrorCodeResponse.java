package com.treeter_client.Message;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by psrok1 on 28.05.2016.
 */
public enum ErrorCodeResponse
{
    OK(0);

    private int errNo;

    private static Map<Integer, ErrorCodeResponse> map = new HashMap<Integer, ErrorCodeResponse>();

    static {
        for (ErrorCodeResponse errorCodeEnum : ErrorCodeResponse.values()) {
            map.put(errorCodeEnum.errNo, errorCodeEnum);
        }
    }

    private ErrorCodeResponse(final int _errNo) { errNo = _errNo; }

    public static ErrorCodeResponse valueOf(int errNo) {
        return map.get(errNo);
    }
}
