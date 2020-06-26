package com.kejunyao.media.util;

import android.media.MediaCodecList;
import androidx.collection.ArrayMap;

/**
 * 兼容工具类
 *
 * @author kejunyao
 * @since 2020年05月30日
 */
public final class SupportUtils {

    private SupportUtils() {
    }

    private static ArrayMap<String, String> sCodecMap = new ArrayMap<>();

    static {
        sCodecMap.put("h264", "video/avc");
    }

    public static String findCodecName(String codecName) {
        String result = sCodecMap.get(codecName);
        return result == null ? "" : result;
    }

    public static boolean isSupport(String codecName) {
        for (int i = 0, count = MediaCodecList.getCodecCount(); i < count; i++) {
            String[] supportedTypes = MediaCodecList.getCodecInfoAt(i).getSupportedTypes();
            for (int j = 0, length = supportedTypes == null ? 0 : supportedTypes.length; j < length; j++) {
                if (findCodecName(codecName).equals(supportedTypes[j])) {
                    return true;
                }
            }
        }
        return false;
    }

}
