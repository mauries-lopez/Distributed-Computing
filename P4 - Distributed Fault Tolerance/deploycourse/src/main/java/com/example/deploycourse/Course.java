package com.example.deploycourse;

public class Course {

    private String courseID;
    private String courseName;
    private String courseStatus;
    private int availNumSlots;
    private int totalNumSlots;

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
