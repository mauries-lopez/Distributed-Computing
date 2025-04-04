package com.example.distributedsystem;

public class Student {

    public String email; // DLSU Email
    public String enlistedCourseID;
    public String enlistedCourseName;

    public Student(String email, String enlistedCourseID, String enlistedCourseName){
        this.email = email;
        this.enlistedCourseID = enlistedCourseID;
        this.enlistedCourseName = enlistedCourseName;
    }

    @Override
    public String toString() {
        return String.format(
                "Student[email='%s', enlistedCourseID='%s', enlistedCourseName='%s'",
                email, enlistedCourseID, enlistedCourseName
        );
    }
}

