package com.kejunyao.media.util;

import android.Manifest;
import android.app.Activity;
import android.content.Context;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import static android.content.pm.PackageManager.PERMISSION_GRANTED;

/**
 * 权限工具类
 *
 * @author kejunyao
 * @since 2020年06月26日
 */
public final class PermissionUtils {

    /**
     * 外部存储设备读取权限请求码
     */
    private static final int REQUEST_CODE_PERMISSION_READ_EXTERNAL_STORAGE = 16121;

    /**
     * Private Constructor
     */
    private PermissionUtils() {
    }

    /**
     * 是否有指定的权限
     *
     * @param context    {@link Context}
     * @param permission 指定的权限标志
     * @return true，有权限；false，无权限
     */
    private static boolean hasPermission(Context context, String permission) {
        return ContextCompat.checkSelfPermission(context, permission) == PERMISSION_GRANTED;
    }

    /**
     * 请求一组权限
     *
     * @param activity    {@link Activity}
     * @param permissions 指定权限标志组
     */
    private static void requestPermission(Activity activity, String[] permissions, int requestCode) {
        ActivityCompat.requestPermissions(
                activity,
                permissions,
                requestCode
        );
    }

    /**
     * 请求外部设备读取权限
     * @param activity {@link Activity}
     */
    public static void requestReadExternalStoragePermission(Activity activity) {
        requestPermission(
                activity,
                new String[]{Manifest.permission.READ_EXTERNAL_STORAGE},
                REQUEST_CODE_PERMISSION_READ_EXTERNAL_STORAGE
        );
    }

    /**
     * 是否有外部设备读取权限
     *
     * @param context {@link Context}
     * @return true，有权限；false，无权限
     */
    public static boolean hasReadExternalStoragePermission(Context context) {
        return hasPermission(context, Manifest.permission.READ_EXTERNAL_STORAGE);
    }

    /**
     * 是否有外部设备写入权限
     *
     * @param context {@link Context}
     * @return true，有权限；false，无权限
     */
    public static boolean hasWriteExternalStoragePermission(Context context) {
        return hasPermission(context, Manifest.permission.WRITE_EXTERNAL_STORAGE);
    }

    /**
     * 请求外部设备读取权限是否成功
     *
     * @param context     {@link Context}
     * @param requestCode 请求权限标志码
     * @return true，请求成功；false，请求失败
     */
    public static boolean isRequestReadExternalStoragePermissionSuccess(Context context, int requestCode) {
        if (requestCode == REQUEST_CODE_PERMISSION_READ_EXTERNAL_STORAGE) {
            return hasReadExternalStoragePermission(context);
        }
        return false;
    }

}
