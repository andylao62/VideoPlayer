package com.kejunyao.media.callback;

/**
 * 预备Listener
 *
 * @author kejunyao
 * @since 2020年05月31日
 */
public interface OnPreparedListener {
    /**
     * 预备完毕回调方法
     * @param mediaWidth 媒体文件宽度
     * @param mediaHeight 媒体文件高度
     */
    void onPrepared(int mediaWidth, int mediaHeight);
}
