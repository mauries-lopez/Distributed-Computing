package com.example.enrollcourse;

import com.fasterxml.jackson.annotation.JsonProperty;

public class Student {

    private String email; // DLSU Email
    private String enlistedCourseID;
    private String enlistedCourseName;

    // Constructor
    public Student(String email, String enlistedCourseID, String enlistedCourseName){
        this.email = email;
        this.enlistedCourseID = enlistedCourseID;
        this.enlistedCourseName = enlistedCourseName;
    }

    // Default constructor for Jackson (optional but recommended)
    public Student() {}

    // Getter methods for Jackson serialization
    @JsonProperty("email")
    public String getEmail() {
        return email;
    }

    @JsonProperty("enlistedCourseID")
    public String getEnlistedCourseID() {
        return enlistedCourseID;
    }

    @JsonProperty("enlistedCourseName")
    public String getEnlistedCourseName() {
        return enlistedCourseName;
    }

    // toString method for better logging/debugging
    @Override
    public String toString() {
        return String.format(
                "Student[email='%s', enlistedCourseID='%s', enlistedCourseName='%s']",
                email, enlistedCourseID, enlistedCourseName
        );
    }
}