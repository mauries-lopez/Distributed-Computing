package com.example.distributedsystem;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.stereotype.Controller;
import org.springframework.web.client.RestTemplate;

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
    public String reqAvailableCourses(){
        // Send to availableCourses Node
        return "availableCourses";
    }

    @GetMapping("/reqEnrollcourse")
    public String reqEnrollCourse(){
        // Send to enrollCourse Node
        return "enrollCourse";
    }

    @GetMapping("/reqViewprevgrades")
    public String reqViewPrevGrades(){
        // Send to enrollCourse Node
        return "viewPrevGrades";
    }

    @GetMapping("/reqUploadgrades")
    public String reqUploadGrades(){
        return "uploadGrades";
    }
}