package com.example.database;

import lombok.Data;
import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.mapping.Document;

@Data
@Document(collection = "student")
public class Student {

    private String email; // DLSU Email
    private String enlistedCourseID;
    private String enlistedCourseName;

    public Student (String email, String enlistedCourseID, String enlistedCourseName){
        this.email = email;
        this.enlistedCourseID = enlistedCourseID;
        this.enlistedCourseName = enlistedCourseName;
    }

    @Override
    public String toString() {
        return String.format(
                "Student[email='%s', enlistedCourseID='%s', enlistedCourseName='%s']",
                email, enlistedCourseID, enlistedCourseName
        );
    }
}

