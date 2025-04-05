package com.example.database;

import lombok.Data;
import org.springframework.data.mongodb.core.mapping.Document;

@Data
@Document(collection = "grades")
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

    @Override
    public String toString(){
        return String.format(
                "Grades[email='%s', courseID='%s', courseName='%s', courseGrade='%s']",
                email, courseID, courseName, courseGrade
        );
    }
}
