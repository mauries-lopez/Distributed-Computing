package com.example.distributedsystem;

import org.springframework.core.ParameterizedTypeReference;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import org.springframework.security.oauth2.core.user.OAuth2User;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.client.RestTemplate;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

//https://www.youtube.com/watch?v=xAaGxhDiGg8 OAuth2
@Controller
public class APIController {

    private final RestTemplate restTemplate = new RestTemplate();

    // Automatically redirect to index after logging in OAuth2.0
    @GetMapping("/")
    public String index(){
        return "index";
    }

    // TO send request to the nodes
    @GetMapping("/reqLogout") // Receive request from the button
    public String reqLogin() {
        // Hostname of Logout Node
        String hostname = "DESKTOP-95D2LUU"; // <--------------------------------- CHANGE HOSTNAME to VM's HOSTNAME (run "hostname" on cmd at VM)

        // Send to Logout Node
        String tempUrl = "http://"+ hostname + ":8080/reqLogout";

        // Send request and receive response
        String response = restTemplate.postForObject(tempUrl, null, String.class); // Send request to the node

        //System.out.println(IPConfig.ipv4Address); <-- To test IPv4 Address Connection
        return response;
    }

    @GetMapping("/reqAvailablecourses")
    public String reqAvailableCourses(Model model){

        // Hostname of Available Courses Node
        String hostname = "PCONE"; // <--------------------------------- CHANGE HOSTNAME to VM's HOSTNAME (run "hostname" on cmd at VM)

        // Send to Available Courses Node
        String tempUrl = "http://"+ hostname + ":8080/fetchAvailablecourses";

        // Send POST request and receive List<Course>
        ResponseEntity<List<Course>> response = restTemplate.exchange(
                tempUrl,
                HttpMethod.POST,
                HttpEntity.EMPTY,
                new ParameterizedTypeReference<List<Course>>() {}
        );

        List<Course> courses = response.getBody();

        // Add data to the model to be used in the HTML view
        model.addAttribute("availableCourses", courses);
        System.out.println(courses);
        return "availableCourses";
    }

    @PostMapping("/reqEnrollcourse")
    public String reqEnrollCourse(@RequestParam("email") String email, Model model){

        // Hostname of Available Courses Node
        String hostname = "PCTWO"; // <--------------------------------- CHANGE HOSTNAME to VM's HOSTNAME (run "hostname" on cmd at VM)

        // Send to Available Courses Node
        String tempUrl = "http://"+ hostname + ":8080/reqEnrollcourse";

        // Send request and receive response
        String response = restTemplate.postForObject(tempUrl, null, String.class); // Send request to the node
        System.out.println(response);

        if ("enrollCourse".equals(response)) // Ask the node to give me my enlisted courses by verifying if the node is up.
        {
            tempUrl = "http://"+ hostname + ":8080/fetchEnlistedCourses";

            // Create Object using Map
            Map<String, String> emailData = new HashMap<>();
            emailData.put("email", email);

            // Create the HttpEntity with the emailData as the body
            HttpEntity<Map<String, String>> reqEmailData = new HttpEntity<>(emailData);

            // Send the POST request and receive the List<Course>
            ResponseEntity<List<Student>> fetchedStudentEnlistedCourses = restTemplate.exchange(
                    tempUrl,
                    HttpMethod.POST,
                    reqEmailData,
                    new ParameterizedTypeReference<List<Student>>() {}
            );

            //System.out.println("Response Status: " + fetchedStudentEnlistedCourses.getStatusCode());
            //System.out.println("Response Body: " + fetchedStudentEnlistedCourses.getBody());

            // Get the list of courses from the response
            List<Student> studentEnlistedCourses = fetchedStudentEnlistedCourses.getBody();

            // Add data to the model to be used in the HTML view
            model.addAttribute("studentEnlistedCourses", studentEnlistedCourses);

            return "enrollCourse";
        }

        //System.out.println(IPConfig.ipv4Address); <-- To test IPv4 Address Connection
        //return "enrollCourse";
        return "enrollCourse";

    }

    // Get the POST request from the 'Enlist' form submit button from enrollCourse.html
    @PostMapping("/submitEnrollACourse")
    public String submitEnrollACourse(@RequestParam("email") String email, @RequestParam("courseID") String courseID, @RequestParam("courseName") String courseName, Model model){

        System.out.println(email);
        System.out.println(courseID);
        System.out.println(courseName);

        // Hostname of Available Courses Node
        String hostname = "PCTWO"; // <--------------------------------- CHANGE HOSTNAME to VM's HOSTNAME (run "hostname" on cmd at VM)

        // Send to Available Courses Node
        String tempUrl = "http://" + hostname + ":8080/reqEnrollACourse";

        // Create Object using Map
        Map<String, String> enrollmentData = new HashMap<>();
        enrollmentData.put("email", email);
        enrollmentData.put("courseID", courseID);
        enrollmentData.put("courseName", courseName);

        // Send request and receive response
        String response = restTemplate.postForObject(tempUrl, enrollmentData, String.class); // Append the values in the query parameters in URL

        // If Success, fetch all enlisted courses made by the STUDENT!
        if ("success".equals(response)) // Ask the node to give me my enlisted courses
        {
            tempUrl = "http://"+ hostname + ":8080/fetchEnlistedCourses";

            // Create Object using Map
            Map<String, String> emailData = new HashMap<>();
            emailData.put("email", email);

            // Create the HttpEntity with the emailData as the body
            HttpEntity<Map<String, String>> reqEmailData = new HttpEntity<>(emailData);

            // Send the POST request and receive the List<Course>
            ResponseEntity<List<Student>> fetchedStudentEnlistedCourses = restTemplate.exchange(
                    tempUrl,
                    HttpMethod.POST,
                    reqEmailData,
                    new ParameterizedTypeReference<List<Student>>() {}
            );

            System.out.println("Response Status: " + fetchedStudentEnlistedCourses.getStatusCode());
            System.out.println("Response Body: " + fetchedStudentEnlistedCourses.getBody());

            // Get the list of courses from the response
            List<Student> studentEnlistedCourses = fetchedStudentEnlistedCourses.getBody();

            // Add data to the model to be used in the HTML view
            model.addAttribute("studentEnlistedCourses", studentEnlistedCourses);

            return "enrollCourse";
        } else {
            return "error";
        }
    }

    @GetMapping("/reqViewprevgrades")
    public String reqViewPrevGrades(){
        /*
        // Hostname of View Preview Grades Node
        String hostname = ""; // <--------------------------------- CHANGE HOSTNAME to VM's HOSTNAME (run "hostname" on cmd at VM)

        // Send to Available Courses Node
        String tempUrl = "http://"+ hostname + ":8080/reqViewprevgrades";

        // Send request and receive response
        String response = restTemplate.postForObject(tempUrl, null, String.class); // Send request to the node

        //System.out.println(IPConfig.ipv4Address); <-- To test IPv4 Address Connection
        return response;
         */

        return "viewPrevGrades";
    }

    @GetMapping("/reqUploadgrades")
    public String reqUploadGrades(){
        /*
        // Hostname of Faculty Upload Grades Node
        String hostname = ""; // <--------------------------------- CHANGE HOSTNAME to VM's HOSTNAME (run "hostname" on cmd at VM)

        // Send to Available Courses Node
        String tempUrl = "http://"+ hostname + ":8080/reqUploadgrades";

        // Send request and receive response
        String response = restTemplate.postForObject(tempUrl, null, String.class); // Send request to the node

        //System.out.println(IPConfig.ipv4Address); <-- To test IPv4 Address Connection
        return response;
        */

        return "uploadGrades";
    }
}