package com.example.database;

import org.springframework.data.mongodb.repository.MongoRepository;

import java.util.Optional;

public interface CourseRepository extends MongoRepository<Course, String> {

    Optional<Course> findByCourseIDAndCourseName(String courseID, String courseName);

}
