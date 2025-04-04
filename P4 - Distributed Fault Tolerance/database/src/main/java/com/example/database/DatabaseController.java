package com.example.database;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.io.Console;
import java.util.List;
import java.util.Map;
import java.util.Optional;

@RestController
public class DatabaseController {

    @Autowired
    private CourseRepository courseRepository;

    @Autowired
    private StudentRepository studentRepository;

    // Fetches
    @PostMapping("/fetchInDBAvailablecourses")
    public List<Course> fetchInDBAvailablecourses(){
        return courseRepository.findAll();
    }

    @PostMapping("/fetchInDBEnlistedCourses")
    public List<Student> fetchInDBEnlistedCourses(@RequestBody Map<String, String> emailData){
        //System.out.println(emailData.get("email"));
        List<Student> students = studentRepository.findByEmail(emailData.get("email"));
        //System.out.println(students);
        return students;
    }

    // Requests
    @PostMapping("/reqInDBEnrollcourse")
    public String enrollACourseInDB(@RequestBody Map<String, String> enrollmentData) {
        try {
            String email = enrollmentData.get("email");
            String courseID = enrollmentData.get("courseID");
            String courseName = enrollmentData.get("courseName");

            Optional<Course> pendingEnlistedCourse = courseRepository.findByCourseIDAndCourseName(courseID, courseName);

            if (pendingEnlistedCourse.isPresent()) {
                Course course = pendingEnlistedCourse.get();

                // Check if course is open and has available slots
                if (course.getCourseStatus().equalsIgnoreCase("Open") && course.getAvailNumSlots() > 0) {
                    studentRepository.save(new Student(email, courseID, courseName));
                    course.setAvailNumSlots(course.getAvailNumSlots() - 1);
                    if (course.getAvailNumSlots() == 0) {
                        course.setCourseStatus("Closed");
                    }
                    courseRepository.save(course);
                    return "success";
                } else {
                    return "course is full or closed";
                }
            } else {
                return "course not found or mismatched";
            }
        } catch (Exception ex) {
            return "error";
        }
    }
}
