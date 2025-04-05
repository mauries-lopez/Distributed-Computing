package com.example.database;

import org.springframework.data.mongodb.repository.MongoRepository;
import org.springframework.data.mongodb.repository.Query;

import java.util.List;
import java.util.Optional;

public interface CourseRepository extends MongoRepository<Course, String> {

    Optional<Course> findByCourseIDAndCourseName(String courseID, String courseName);

    List<Course> findCourseNameByCourseID(String courseID);

    List<Course> findByCourseID(String courseID);
}
