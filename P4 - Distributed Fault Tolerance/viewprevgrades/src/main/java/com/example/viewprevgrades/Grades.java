package com.example.viewprevgrades;

import com.fasterxml.jackson.annotation.JsonProperty;

public class Grades {

    private String email;
    private String courseID;
    private String courseName;
    private String courseGrade;

    public Grades(String email, String courseID, String courseName, String courseGrade){
        this.email = email;
        this.courseID = courseID;
        this.courseName = courseName;
        this.courseGrade = courseGrade;
    }

    public Grades() {}

    @JsonProperty("email")
    public String getEmail(){
        return email;
    }

    @JsonProperty("courseID")
    public String getCourseID(){
        return courseID;
    }

    @JsonProperty("courseName")
    public String getCourseName(){
        return courseName;
    }

    @JsonProperty("courseGrade")
    public String getCourseGrade(){
        return courseGrade;
    }

    @Override
    public String toString(){
        return String.format(
                "Grades[email='%s', courseID='%s', courseName='%s', courseGrade='%s']",
                email, courseID, courseName, courseGrade
        );
    }
}
