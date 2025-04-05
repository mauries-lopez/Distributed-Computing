package com.example.viewprevgrades;

import org.springframework.core.ParameterizedTypeReference;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.client.RestTemplate;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

@RestController
public class ViewprevgradesController {

    private final RestTemplate restTemplate = new RestTemplate();

    // Receive Post Request from Web Application
    @PostMapping("/reqViewprevgrades")
    public String reqViewprevgrades(){
        return "success";
    }

    @PostMapping("/fetchPrevGrades")
    public List<Grades> fetchPrevGrades(@RequestBody Map<String, String> emailData){
        // Hostname of Database Node
        String hostname = "192.168.55.144"; // <--------------------------------- CHANGE HOSTNAME to VM's IPv4 ADDRESS hosting the DATABASE

        // Send to Database Node
        String tempUrl = "http://"+ hostname + ":8080/fetchInDBPrevGrades";

        // Create Object using Map
        Map<String, String> emailDataToDB = new HashMap<>();
        emailDataToDB.put("email", emailData.get("email"));

        // Create the HttpEntity with the emailData as the body
        HttpEntity<Map<String, String>> reqEmailData = new HttpEntity<>(emailDataToDB);

        // Send the POST request and receive the List<Course>
        ResponseEntity<List<Grades>> fetchedStudentPrevGrades = restTemplate.exchange(
                tempUrl,
                HttpMethod.POST,
                reqEmailData,
                new ParameterizedTypeReference<List<Grades>>() {}
        );

        // Get the list of courses from the response
        List<Grades> studentPrevGrades = fetchedStudentPrevGrades.getBody();

        return studentPrevGrades;
    }

}