package com.example.smartpetfeedingsystem_java.Model;

public class DataDTO {
    private Float food_v;
    private Integer door;

    @Override
    public String toString() {
        return "DataDTO{" +
                "food_v=" + food_v +
                ", door=" + door +
                '}';
    }

    public Float getFood_v() {
        return food_v;
    }

    public void setFood_v(Float food_v) {
        this.food_v = food_v;
    }

    public Integer getDoor() {
        return door;
    }

    public void setDoor(Integer door) {
        this.door = door;
    }
}
