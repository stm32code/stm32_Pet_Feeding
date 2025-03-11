package com.example.smartpetfeedingsystem_java.Model;

public class Send {
    private Integer cmd;
    private DataDTO data;

    @Override
    public String toString() {
        return "Send{" +
                "cmd=" + cmd +
                ", data=" + data +
                '}';
    }

    public Integer getCmd() {
        return cmd;
    }

    public void setCmd(Integer cmd) {
        this.cmd = cmd;
    }

    public DataDTO getData() {
        return data;
    }

    public void setData(DataDTO data) {
        this.data = data;
    }
}
