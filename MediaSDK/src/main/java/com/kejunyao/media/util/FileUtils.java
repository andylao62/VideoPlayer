package com.kejunyao.media.util;

import java.io.File;

/**
 * 文件工具类
 *
 * @author kejunyao
 * @since 2020年06月26日
 */
public final class FileUtils {

    private FileUtils() {
    }

    /**
     * 是否为本地文件
     * @param source 文件来源
     * @return true，本地文件；false，非本地文件
     */
    public static boolean isLocalFile(String source) {
        return new File(source).exists();
    }
}
