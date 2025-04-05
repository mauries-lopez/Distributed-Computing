package com.example.distributedsystem;

public class Grades {

    public String email;
    public String courseID;
    public String courseName;
    public String courseGrade;

    public Grades(String email, String courseID, String courseName, String courseGrade){
        this.email = email;
        this.courseID = courseID;
        this.courseName = courseName;
        this.courseGrade = courseGrade;
    }

    @Override
    public String toString(){
        return String.format(
                "Grades[email='%s', courseID='%s', courseName='%s', courseGrade='%s']",
                email, courseID, courseName, courseGrade
        );
    }
}
