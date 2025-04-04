package com.example.enrollcourse;

import org.springframework.core.ParameterizedTypeReference;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.client.RestTemplate;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

@RestController
public class EnrollcourseController {

    private final RestTemplate restTemplate = new RestTemplate();

    // Receive Post Request from Web Application
    @PostMapping("/reqEnrollcourse")
    public String reqEnrollcourse(){
        return "enrollCourse";
    }

    // Receive Post Request from Web Application about enrolling a course
    @PostMapping("/reqEnrollACourse")
    public String enrollACourse(@RequestBody Map<String, String> enrollmentData){

        //System.out.println(enrollmentData.get("email"));
        //System.out.println(enrollmentData.get("courseID"));
        //System.out.println(enrollmentData.get("courseName"));

        // Hostname of Database Node
        String hostname = "192.168.55.144"; // <--------------------------------- CHANGE HOSTNAME to VM's IPv4 ADDRESS hosting the DATABASE

        // Send to Available Courses Node
        String tempUrl = "http://" + hostname + ":8080/reqInDBEnrollcourse";

        // Create Object using Map
        Map<String, String> enrollmentDataToDB = new HashMap<>();
        enrollmentDataToDB.put("email", enrollmentData.get("email"));
        enrollmentDataToDB.put("courseID", enrollmentData.get("courseID"));
        enrollmentDataToDB.put("courseName", enrollmentData.get("courseName"));

        // Send request and receive response
        String response = restTemplate.postForObject(tempUrl, enrollmentDataToDB, String.class); // Send request to the node

        return response;
    }

    @PostMapping("/fetchEnlistedCourses")
    public List<Student> fetchEnlistedCourses(@RequestBody Map<String, String> emailData){

        System.out.println(emailData.get("email"));

        // Hostname of Database Node
        String hostname = "192.168.55.144"; // <--------------------------------- CHANGE HOSTNAME to VM's IPv4 ADDRESS hosting the DATABASE

        // Send to Database Node
        String tempUrl = "http://"+ hostname + ":8080/fetchInDBEnlistedCourses";

        // Create Object using Map
        Map<String, String> emailDataToDB = new HashMap<>();
        emailDataToDB.put("email", emailData.get("email"));

        // Create the HttpEntity with the emailData as the body
        HttpEntity<Map<String, String>> reqEmailData = new HttpEntity<>(emailDataToDB);

        // Send the POST request and receive the List<Course>
        ResponseEntity<List<Student>> fetchedStudentEnlistedCourses = restTemplate.exchange(
                tempUrl,
                HttpMethod.POST,
                reqEmailData,
                new ParameterizedTypeReference<List<Student>>() {}
        );

        // Get the list of courses from the response
        List<Student> studentEnlistedCourses = fetchedStudentEnlistedCourses.getBody();

        return studentEnlistedCourses;
    }
}
