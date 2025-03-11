package com.example.smartpetfeedingsystem_java.Utils;

import java.text.DateFormat;
import java.text.ParsePosition;
import java.text.SimpleDateFormat;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Date;
import java.util.TimeZone;

public class TimeCycle {
    /**
     * 比较两个"yyyy-MM-dd HH:mm:ss"字符串大小
     *
     * @param dateTime1 String
     * @param dateTime2 String
     * @return 如果返回值小于 0，则表示 dateTime1 较小；如果返回值大于 0，则表示 dateTime1 较大；如果返回值等于 0，则表示两个日期时间相等
     */
    public static int compareDateTime(String dateTime1, String dateTime2) {
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        LocalDateTime dt1 = LocalDateTime.parse(dateTime1, formatter);
        LocalDateTime dt2 = LocalDateTime.parse(dateTime2, formatter);

        return dt1.compareTo(dt2);
    }

    /**
     * 获取当前时间
     * return yyyy-MM-dd HH:mm:ss
     */
    public static String getDateTime() {
        //转换为中国时区
        TimeZone.setDefault(TimeZone.getTimeZone("Etc/GMT-8"));
        return transToString(System.currentTimeMillis());
    }

    /**
     * 时间戳转字符串
     *
     * @param time
     * @return String
     */
    public static String transToString(Long time) {
        Date date = new Date();
        DateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        return format.format(date);
    }

    /**
     * 字符串转时间戳
     *
     * @param date
     * @return Timestamp
     */
    public static Long transToTimeStamp(String date) {
        return new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").parse(date, new ParsePosition(0)).getTime();
    }
}
