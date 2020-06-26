package com.kejunyao.media.util;

import android.app.Activity;
import android.content.Context;
import android.content.ContextWrapper;
import android.os.Build;

/**
 * {@link Activity}工具类
 *
 * @author kejunyao
 * @since 2020年06月26日
 */
public final class ActivityUtils {

    private ActivityUtils() {
    }

    public static boolean isFinishing(Activity activity) {
        return activity == null || activity.isFinishing()
                || (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1 && activity.isDestroyed());
    }

    private static Activity getActivity(Context context) {
        if (context instanceof Activity) {
            return (Activity) context;
        }
        while (context instanceof ContextWrapper) {
            context = ((ContextWrapper) context).getBaseContext();
            if (context instanceof Activity) {
                return (Activity) context;
            }
        }
        return null;
    }

    public static boolean isFinishing(Context context) {
        if (context == null) {
            return true;
        }
        Activity activity = getActivity(context);
        return isFinishing(activity);
    }
}
