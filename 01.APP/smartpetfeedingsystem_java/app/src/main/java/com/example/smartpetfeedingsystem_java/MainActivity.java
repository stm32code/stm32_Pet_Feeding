package com.example.smartpetfeedingsystem_java;

import static com.example.smartpetfeedingsystem_java.Utils.Common.DeviceOnline;
import static com.example.smartpetfeedingsystem_java.Utils.Common.PushTopic;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.app.TimePickerDialog;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.SeekBar;
import android.widget.TimePicker;

import com.blankj.utilcode.util.LogUtils;
import com.example.smartpetfeedingsystem_java.Model.DataDTO;
import com.example.smartpetfeedingsystem_java.Model.Receive;
import com.example.smartpetfeedingsystem_java.Model.Send;
import com.example.smartpetfeedingsystem_java.Utils.BeatingAnimation;
import com.example.smartpetfeedingsystem_java.Utils.Common;
import com.example.smartpetfeedingsystem_java.Utils.DeviceIsOnline;
import com.example.smartpetfeedingsystem_java.Utils.MToast;
import com.example.smartpetfeedingsystem_java.Utils.TimeCycle;
import com.example.smartpetfeedingsystem_java.Utils.ViewSetIsClick;
import com.example.smartpetfeedingsystem_java.databinding.ActivityMainBinding;
import com.google.gson.Gson;
import com.gyf.immersionbar.ImmersionBar;
import com.itfitness.mqttlibrary.MQTTHelper;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    private ActivityMainBinding binding;
    private boolean isDebugView = false;//是否显示debug界面
    private final List<String> arrayList = new ArrayList<String>();// debug消息数据
    private ArrayAdapter adapter = null; // debug消息适配器
    private SharedPreferences sharedPreferences;
    private SharedPreferences.Editor editor;
    private boolean isAnorexia = false; //是否缺食物
    private int time = 0;
    private int setTime = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        sharedPreferences = getSharedPreferences("Location", MODE_PRIVATE);
        editor = sharedPreferences.edit();
        mqttConfig();
        initView();
        isOnline();
        eventManager();
    }

    /***
     * 初始化视图
     */
    private void initView() {
        setSupportActionBar(binding.toolbar);
        binding.toolbarLayout.setTitle(getTitle());
        ImmersionBar.with(this).init();
        debugView();
        warringLayout(false, "");
        binding.waterImage.setEnabled(false);
        binding.eatImage.setEnabled(true);
        int value = sharedPreferences.getInt("theTime", -1);
        if (value != -1) {
            timeCover(value);
            setTime = value;
            time = value;
        }
        eventManager();
    }


    /***
     * 修改显示的时间
     * @param time 分钟
     */
    private void timeCover(int time) {
        int hours = time / 60;
        int minutes = time % 60;
        String timeText = hours > 0 ? hours + " 小时" + minutes + " 分钟" : minutes + " 分钟";
        binding.timeText.setText(timeText);
    }

    /***
     *事件监听
     */
    private void eventManager() {
        binding.timeBtn.setOnClickListener(view -> {
            binding.timeBtn.setSelected(!binding.timeBtn.isSelected());
            if (binding.timeBtn.isSelected()) {
                if (time != 0) {
                    ViewSetIsClick.setViewIsClick(binding.timeText, false);
                    new Thread(() -> {
                        try {
                            while (binding.timeBtn.isSelected()) {
                                while (time > 0  && binding.timeBtn.isSelected()) {
                                    runOnUiThread(() -> timeCover(time));
                                    Thread.sleep(59000);
                                    //Thread.sleep(10000);
                                    time--;
                                }
                                if (time <= 0 && isAnorexia ) {//说明到时间了
                                    sendMessage(2, "2");
                                }
                                time = setTime;
                                Thread.sleep(5000);
                            }
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }).start();
                } else {
                    MToast.mToast(this, "请先设置时间");
                    binding.timeBtn.setSelected(!binding.timeBtn.isSelected());
                }
            } else {
                time = 0;
                ViewSetIsClick.setViewIsClick(binding.timeText, true);
            }
        });
        binding.timeText.setOnClickListener(view -> {
            // 获取当前时间
            Calendar currentTime = Calendar.getInstance();
            // 创建 TimePickerDialog 对话框
            TimePickerDialog timePickerDialog = new TimePickerDialog(this, (view1, hourOfDay, minute) -> {
                // 当前选择的时间
                Calendar selectedTime = Calendar.getInstance();
                selectedTime.set(Calendar.HOUR_OF_DAY, hourOfDay);
                selectedTime.set(Calendar.MINUTE, minute);

                // 计算时间差（分钟）
                long diffInMillis = Math.abs(selectedTime.getTimeInMillis() - currentTime.getTimeInMillis());
                long diffInMinutes = diffInMillis / (60 * 1000);
                time = (int) diffInMinutes;
                // 设置时间
                setTime = time;
                timeCover(time);
                editor.putInt("theTime", time);
                editor.commit();
            }, currentTime.get(Calendar.HOUR_OF_DAY), currentTime.get(Calendar.MINUTE), true);

            // 显示 TimePickerDialog 对话框
            timePickerDialog.show();
        });
        binding.powerButton.setOnClickListener(v -> {
            if (Common.mqttHelper != null && Common.mqttHelper.getConnected()) {
                binding.powerButton.setSelected(!binding.powerButton.isSelected());
                sendMessage(2, binding.powerButton.isSelected() ? "1" : "0");
            } else {
                MToast.mToast(MainActivity.this, "请先建立连接");
            }
        });

        binding.upperSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if (Common.mqttHelper != null && Common.mqttHelper.getConnected()) {
                    binding.SeekText.setText(String.valueOf(progress));
                } else {
                    MToast.mToast(MainActivity.this, "请先建立连接");
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                // Not implemented
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                if (Common.mqttHelper != null && Common.mqttHelper.getConnected()) {
                    String progress = String.valueOf(seekBar.getProgress());
                    binding.SeekText.setText(progress);
                    sendMessage(1, progress);
                } else {
                    MToast.mToast(MainActivity.this, "请先建立连接");
                }
            }
        });
        binding.warringLayout.setOnClickListener(v -> binding.warringLayout.setVisibility(View.GONE));
    }

    /**
     * @brief 将接受到的数据解析，并根据解析的数据分析做出响应
     */
    private void analysisOfData(Receive data) {
        try {
            if (data.getDoor() != null && !data.getDoor().isEmpty()) {
                binding.powerButton.setSelected(data.getDoor().equals("1"));
            }
            if (data.getTemp() != null && !data.getTemp().isEmpty()) {
                binding.tempText.setText(String.format("%s ℃", data.getTemp()));
            }
            if (data.getHumi() != null && !data.getDoor().isEmpty()) {
                binding.humiText.setText(String.format("%s %%", data.getHumi()));
            }
            if (data.getEat() != null && !data.getDoor().isEmpty()) {
                if (data.getEat().equals("1")) {
                    binding.eatText.setText("正在进食");
                    binding.eatImage.setSelected(true);
                } else {
                    binding.eatText.setText("未在进食");
                    binding.eatImage.setSelected(false);
                }
            }
            if (data.getFood() != null && !data.getDoor().isEmpty()) {
                binding.foodText.setText(String.format("%s 克", data.getFood()));
                isAnorexia = Float.parseFloat(data.getFood()) <= 5f;//大于5g则不缺食
            }

            if (data.getFood_v() != null && !data.getDoor().isEmpty()) {
                binding.upperSeekBar.setProgress((int) Float.parseFloat(data.getFood_v()));
            }
            if (data.getWater() != null && !data.getDoor().isEmpty()) {
                if (data.getWater().equals("1")) {
                    binding.waterText.setText("充足");
                    binding.waterImage.setSelected(true);
                    warringLayout(false, "缺水中。。");
                } else {
                    binding.waterText.setText("缺水");
                    binding.waterImage.setSelected(false);
                    warringLayout(true, "缺水中。。");
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            MToast.mToast(this, "数据解析失败");
        }

    }

    /***
     * 判断硬件是否在线
     */

    private void isOnline() {
        new Thread(() -> {
            int i = 0;
            try {
                while (true) {
                    if (i > 3) {
                        i = 0;
                        runOnUiThread(() -> binding.online.setText(DeviceOnline ? "在线" : "离线"));
                        DeviceOnline = false;
                    }
                    i++;
                    Thread.sleep(8000);

                }
            } catch (Exception e) {
                e.printStackTrace();
            }

        }).start();

    }
    /**
     * @param visibility 是否显示
     * @param str        显示内容
     * @brief 显示警告弹窗和设置弹窗内容
     */
    private void warringLayout(boolean visibility, String str) {
        if (visibility) {
            binding.warringLayout.setVisibility(View.VISIBLE);
            binding.warringText.setText(str);
            new BeatingAnimation().onAnimation(binding.warringImage);
        } else {
            binding.warringLayout.setVisibility(View.GONE);
        }
    }

    /**
     * @param str  如果为 1 添加发送数据到界面   为 2 添加接受消息到界面
     * @param data 数据字符串
     * @brief debug界面数据添加
     */
    private void debugViewData(int str, String data) {
        if (arrayList.size() >= 255) {
            arrayList.clear();
        }
        runOnUiThread(() -> {
            switch (str) {
                case 1: //发送的消息
                    arrayList.add("目标主题:" + Common.ReceiveTopic + " \n时间:" + TimeCycle.getDateTime() + "\n发送消息:" + data);
                    break;
                case 2:
                    arrayList.add("来自主题:" + Common.ReceiveTopic + " \n时间:" + TimeCycle.getDateTime() + "\n接到消息:" + data);
                    break;
            }
            // 在添加新数据之后调用以下方法，滚动到列表底部
            binding.debugView.post(() -> {
                binding.debugView.setSelection(adapter != null ? adapter.getCount() - 1 : 0);
            });
            if (adapter != null) {
                adapter.notifyDataSetChanged();
            }
        });
    }

    /**
     * @brief debug界面的初始化
     */
    private void debugView() {
        adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, arrayList);
        binding.debugView.setAdapter(adapter);
    }

    /**
     * @param message 需要发送的消息
     * @brief 再次封装MQTT发送
     */
    private void sendMessage(int cmd, String message) {
        if (Common.mqttHelper != null && Common.mqttHelper.getConnected()) {
            Send send = new Send();
            DataDTO dto = new DataDTO();
            switch (cmd) {
                case 1:
                    dto.setFood_v(Float.parseFloat(message));
                    break;
                case 2:
                    dto.setDoor(Integer.parseInt(message));
                    break;
            }
            send.setCmd(cmd);
            send.setData(dto);
            String str = new Gson().toJson(send);
            new Thread(() -> Common.mqttHelper.publish(PushTopic, str, 1)).start();
            debugViewData(1, str);
        }
    }

    /***
     * mqtt配置
     */
    private void mqttConfig() {
        Common.mqttHelper = new MQTTHelper(this, Common.Sever, Common.DriveID, Common.DriveName, Common.DrivePassword, true, 30, 60);

        try {
            Common.mqttHelper.connect(Common.ReceiveTopic, 1, true, new MqttCallback() {
                @Override
                public void connectionLost(Throwable cause) {

                }

                @Override
                public void messageArrived(String topic, MqttMessage message) {
                    //收到消息
                    Receive data = new Gson().fromJson(message.toString(), Receive.class);
                    LogUtils.eTag("接收到消息", message.getPayload() != null ? new String(message.getPayload()) : "");
//                    onlineFlag = true;
                    binding.online.setText("在线");
                    debugViewData(2, message.getPayload() != null ? new String(message.getPayload()) : "");
                    System.out.println(data);
                    analysisOfData(data);
                }

                @Override
                public void deliveryComplete(IMqttDeliveryToken token) {

                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
            MToast.mToast(this, "MQTT连接错误");
        }
    }

    //填充右上角目录
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_scrolling, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        if (item.getItemId() == R.id.setDebugView) {
            isDebugView = !isDebugView;
            binding.debugView.setVisibility(isDebugView ? View.VISIBLE : View.GONE);
        }
        return super.onOptionsItemSelected(item);
    }
}