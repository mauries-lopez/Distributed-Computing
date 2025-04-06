package com.example.distributedsystem;

import org.springframework.core.ParameterizedTypeReference;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import org.springframework.security.oauth2.core.user.OAuth2User;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;
import org.springframework.stereotype.Controller;
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

    @GetMapping("/successLogout")
    public String successLogout(){
        return "logout";
    }

    // TO send request to the nodes
    @GetMapping("/reqLogout") // Receive request from the button
    public String reqLogin() {
        // Hostname of Logout Node
        String hostname = "localhost"; // <--------------------------------- CHANGE HOSTNAME to VM's HOSTNAME (run "hostname" on cmd at VM)

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
        String hostname = "PCFOUR"; // <--------------------------------- CHANGE HOSTNAME to VM's HOSTNAME (run "hostname" on cmd at VM)

        // Send to Available Courses Node
        String tempUrl = "http://"+ hostname + ":8080/fetchAvailablecourses";

        try{
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
        } catch (Exception ex){
            model.addAttribute("errorMessage", "Feature is unavailable");
            return "availableCourses";
        }
    }

    @PostMapping("/reqEnrollcourse")
    public String reqEnrollCourse(@RequestParam("email") String email, Model model){

        // Hostname of Available Courses Node
        String hostname = "PCTWO"; // <--------------------------------- CHANGE HOSTNAME to VM's HOSTNAME (run "hostname" on cmd at VM)

        // Send to Available Courses Node
        String tempUrl = "http://"+ hostname + ":8080/reqEnrollcourse";

        try{
            // Send request and receive response
            String response = restTemplate.postForObject(tempUrl, null, String.class); // Send request to the node

            if ("enrollCourse".equals(response)) // Ask the node to give me my enlisted courses by verifying if the node is up.
            {
                helperFetchEnlistedCourses(hostname, email, model);
            }
            return "enrollCourse";
        } catch (Exception ex){
            model.addAttribute("errorMessage", "Feature is unavailable");
            return "enrollCourse";
        }
    }

    // Get the POST request from the 'Enlist' form submit button from enrollCourse.html
    // This is related to reqEnrollCourse
    @PostMapping("/submitEnrollACourse")
    public String submitEnrollACourse(@RequestParam("email") String email, @RequestParam("courseID") String courseID, Model model){

        //System.out.println(email);
        //System.out.println(courseID);
        //System.out.println(courseName);

        // Hostname of Available Courses Node
        String hostname = "PCTWO"; // <--------------------------------- CHANGE HOSTNAME to VM's HOSTNAME (run "hostname" on cmd at VM)

        // Send to Available Courses Node
        String tempUrl = "http://" + hostname + ":8080/reqEnrollACourse";

        // Create Object using Map
        Map<String, String> enrollmentData = new HashMap<>();
        enrollmentData.put("email", email);
        enrollmentData.put("courseID", courseID);

        try{
            // Send request and receive response
            String response = restTemplate.postForObject(tempUrl, enrollmentData, String.class);

            // If Success, fetch all enlisted courses made by the STUDENT!
            if ("success".equals(response)) // Ask the node to give me my enlisted courses
            {
                helperFetchEnlistedCourses(hostname, email, model);
            } else {
                helperFetchEnlistedCourses(hostname, email, model);
                model.addAttribute("errorMessage", response);
            }
            return "enrollCourse";
        } catch ( Exception ex ){
            model.addAttribute("errorMessage", "Feature is unavailable");
            return "uploadGrades";
        }
    }

    private void helperFetchEnlistedCourses(String hostname, String email, Model model){
        String tempUrl = "http://"+ hostname + ":8080/fetchEnlistedCourses";

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
    }

    @PostMapping("/reqViewprevgrades")
    public String reqViewPrevGrades(@RequestParam("email") String email, Model model){

        // Hostname of View Preview Grades Node
        String hostname = "PCONE"; // <--------------------------------- CHANGE HOSTNAME to VM's HOSTNAME (run "hostname" on cmd at VM)

        // Send to Available Courses Node
        String tempUrl = "http://"+ hostname + ":8080/reqViewprevgrades";

        try{
            // Send request and receive response
            String response = restTemplate.postForObject(tempUrl, null, String.class); // Send request to the node

            // Hostname of Available Courses Node
            if ( "success".equals(response)){
                helperFetchPrevGrades(hostname, email, model);
                return "viewPrevGrades";
            } else {
                helperFetchPrevGrades(hostname, email, model);
                model.addAttribute("errorMessage", response);
                return "viewPrevGrades";
            }
        } catch (Exception ex){
            model.addAttribute("errorMessage", "Feature is unavailable");
            return "viewPrevGrades";
        }
    }

    private void helperFetchPrevGrades(String hostname, String email, Model model){
        // Send to Available Courses Node
        String tempUrl = "http://"+ hostname + ":8080/fetchPrevGrades";

        // Create Object using Map
        Map<String, String> emailData = new HashMap<>();
        emailData.put("email", email);

        // Create the HttpEntity with the emailData as the body
        HttpEntity<Map<String, String>> reqEmailData = new HttpEntity<>(emailData);

        // Send POST request and receive List<Course>
        ResponseEntity<List<Grades>> studentPrevGradesResponse = restTemplate.exchange(
                tempUrl,
                HttpMethod.POST,
                reqEmailData,
                new ParameterizedTypeReference<List<Grades>>() {}
        );

        List<Grades> studentPrevGrades = studentPrevGradesResponse.getBody();

        // Add data to the model to be used in the HTML view
        model.addAttribute("studentPrevGrades", studentPrevGrades);
        //System.out.println(studentPrevGrades);
    }

    @GetMapping("/reqUploadgrades")
    public String reqUploadGrades(Model model){

        // Hostname of Faculty Upload Grades Node
        String hostname = "PCTWO"; // <--------------------------------- CHANGE HOSTNAME to VM's HOSTNAME (run "hostname" on cmd at VM)

        // Send to Available Courses Node
        String tempUrl = "http://"+ hostname + ":8080/reqUploadgrades";

        try{
            // Send request and receive response
            String response = restTemplate.postForObject(tempUrl, null, String.class); // Send request to the node
            return "uploadGrades";
        } catch (Exception ex){
            model.addAttribute("errorMessage", "Feature is unavailable");
            return "uploadGrades";
        }
    }

    // This is related to reqUploadGrades
    @PostMapping("/submitUploadGrade")
    public String submitUploadGrades(@RequestParam("email") String email, @RequestParam("courseID") String courseID, @RequestParam("courseGrade") String courseGrade, Model model){

        // Hostname of Available Courses Node
        String hostname = "PCTWO"; // <--------------------------------- CHANGE HOSTNAME to VM's HOSTNAME (run "hostname" on cmd at VM)

        // Send to Available Courses Node
        String tempUrl = "http://" + hostname + ":8080/submitUploadGrade";

        // Create Object using Map
        Map<String, String> gradeData = new HashMap<>();
        gradeData.put("email", email);
        gradeData.put("courseID", courseID);
        gradeData.put("courseGrade", courseGrade);

        try{
            // Send request and receive response
            String response = restTemplate.postForObject(tempUrl, gradeData, String.class);

            if ("success".equals(response)){
                // Add data to the model to be used in the HTML view
                model.addAttribute("response", "Successfully Graded!");
            } else {
                model.addAttribute("response", response);
            }
            return "uploadGrades";
        } catch (Exception ex){
            model.addAttribute("errorMessage", "Feature is unavailable");
            return "uploadGrades";
        }
    }

    @GetMapping("/reqDeployNewCourse")
    public String reqDeployNewCourse(Model model){

        // Hostname of Deploy New Course Node
        String hostname = "PCONE"; // <--------------------------------- CHANGE HOSTNAME to VM's HOSTNAME (run "hostname" on cmd at VM)

        // Send to Available Courses Node
        String tempUrl = "http://"+ hostname + ":8080/reqDeployNewCourse";

        try{
            // Send request and receive response
            String response = restTemplate.postForObject(tempUrl, null, String.class); // Send request to the node
            return response;
        } catch ( Exception ex ){
            model.addAttribute("errorMessage", "Feature is unavailable");
            return "deployCourse";
        }
    }

    @PostMapping("/submitNewCourse")
    public String submitNewCourse(@RequestParam("courseID") String courseID, @RequestParam("courseName") String courseName, @RequestParam("numTotalSlots") String numTotalSlots, Model model){
        // Hostname of Available Courses Node
        String hostname = "PCONE"; // <--------------------------------- CHANGE HOSTNAME to VM's HOSTNAME (run "hostname" on cmd at VM)

        // Send to Available Courses Node
        String tempUrl = "http://" + hostname + ":8080/submitNewCourse";

        // Create Object using Map
        Map<String, String> newCourseData = new HashMap<>();
        newCourseData.put("courseID", courseID);
        newCourseData.put("courseName", courseName);
        newCourseData.put("numTotalSlots", numTotalSlots);

        try{
            // Send request and receive response
            String response = restTemplate.postForObject(tempUrl, newCourseData, String.class);

            if ("success".equals(response)){
                // Add data to the model to be used in the HTML view
                model.addAttribute("response", "Successfully Course Deployed!");
            } else {
                model.addAttribute("response", response);
            }
            return "deployCourse";
        } catch (Exception ex){
            model.addAttribute("errorMessage", "Feature is unavailable");
            return "deployCourse";
        }
    }
}