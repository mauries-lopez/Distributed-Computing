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
    @Autowired
    private GradesRepository gradesRepository;

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

    @PostMapping("fetchInDBPrevGrades")
    public List<Grades> fetchInDBPrevGrades(@RequestBody Map<String, String> emailData){
        try{
            List<Grades> studentGrades = gradesRepository.findByEmail(emailData.get("email"));
            return studentGrades;
        } catch ( Exception ex ){
            return null;
        }
    }

    // Requests
    @PostMapping("/reqInDBEnrollcourse")
    public String enrollACourseInDB(@RequestBody Map<String, String> enrollmentData) {
        try {
            String email = enrollmentData.get("email");
            String courseID = enrollmentData.get("courseID");

            // Find the course by its courseID
            List<Course> pendingEnlistedCourse = courseRepository.findCourseNameByCourseID(courseID);

            if (!pendingEnlistedCourse.isEmpty()) {
                Course course = pendingEnlistedCourse.getFirst();
                if (course.getCourseStatus().equalsIgnoreCase("Open") && course.getAvailNumSlots() > 0) {
                    List<Student> listOfStudents = studentRepository.findByEmail(email);
                    for (Student student : listOfStudents) {
                        if (course.getCourseName().equals(student.getEnlistedCourseName())) {
                            return "course enlisted already";
                        }
                    }
                    studentRepository.save(new Student(email, courseID, course.getCourseName()));
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

    // Submits
    @PostMapping("/submitToDBUploadGrade")
    public String submitToDBUploadGrade(@RequestBody Map<String, String> gradeData) {
        try {
            // Check if email is a valid student
            List<Student> listOfStudents = studentRepository.findByEmail(gradeData.get("email"));
            if (!listOfStudents.isEmpty()) {
                // Check if the given courseID is a valid course
                List<Course> listOfCourses = courseRepository.findByCourseID(gradeData.get("courseID"));
                if (!listOfCourses.isEmpty()) {
                    // Find the course name from the course repository using Course ID
                    List<Course> listOfCourseIDs = courseRepository.findCourseNameByCourseID(gradeData.get("courseID"));
                    String courseName = "";
                    for ( Course courseID : listOfCourseIDs){
                        if ( courseID.getCourseID().equals(gradeData.get("courseID"))){
                            courseName = courseID.getCourseName();
                        }
                    }

                    // Check if Student is graded already
                    List<Grades> listOfGrades = gradesRepository.findByEmail(gradeData.get("email"));
                    for (Grades grade : listOfGrades) {
                        if (grade.getCourseID().equals(gradeData.get("courseID"))) {
                            return "Student has been graded already";
                        }
                    }

                    // Check if Student is enlisted to the course
                    for ( Student student: listOfStudents ){
                        if (!student.getEnlistedCourseID().equals(gradeData.get("courseID"))){
                            return "Student is not enrolled to the course";
                        }
                    }

                    gradesRepository.save(new Grades(gradeData.get("email"), gradeData.get("courseID"), courseName, gradeData.get("courseGrade")));

                    return "success";
                } else {
                    return "Course not found";
                }
            } else {
                return "Student not enrolled/found/valid";
            }
        } catch (Exception ex) {
            return "error";
        }
    }
}
