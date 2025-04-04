package com.example.distributedsystem;

public class Course {
    public String courseID;
    public String courseName;
    public String courseStatus;
    public int availNumSlots;
    public int totalNumSlots;

    public Course(String courseID, String courseName, String courseStatus, int availNumSlots, int totalNumSlots) {
        this.courseID = courseID;
        this.courseName = courseName;
        this.courseStatus = courseStatus;
        this.availNumSlots = availNumSlots;
        this.totalNumSlots = totalNumSlots;
    }

    @Override
    public String toString() {
        return String.format(
                "Course[courseID='%s', courseName='%s', courseStatus='%s', availNumSlots='%d', totalNumSlots='%d']",
                courseID, courseName, courseStatus, availNumSlots, totalNumSlots
        );
    }
}
