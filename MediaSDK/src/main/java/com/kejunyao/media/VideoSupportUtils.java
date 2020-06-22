package com.kejunyao.media;

import android.media.MediaCodecList;

import java.util.HashMap;
import java.util.Map;

/**
 * 当前设备是否支持硬解码判断工具类
 *
 * @author kejunyao
 * @since 2020年05月30日
 */
final class VideoSupportUtils {

    private VideoSupportUtils() {
    }

    private static Map<String, String> sCodecMap = new HashMap<>();

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
