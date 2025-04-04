package com.example.availablecourse;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.ParameterizedTypeReference;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.client.RestTemplate;

import java.util.List;

@RestController
public class AvailablecourseController {

    private final RestTemplate restTemplate = new RestTemplate();

    // Receive Post Request from Web Application
    @PostMapping("/fetchAvailablecourses")
    public List<Course> fetchAvailablecourses(){

        // Hostname of Database Node
        String hostname = "192.168.55.144"; // <--------------------------------- CHANGE HOSTNAME to VM's IPv4 ADDRESS hosting the DATABASE

        // Send to Database Node
        String tempUrl = "http://"+ hostname + ":8080/fetchInDBAvailablecourses";

        // Send POST request and receive List<Course>
        ResponseEntity<List<Course>> response = restTemplate.exchange(
                tempUrl,
                HttpMethod.POST,
                HttpEntity.EMPTY,
                new ParameterizedTypeReference<List<Course>>() {}
        );

        List<Course> courses = response.getBody();
        System.out.println(courses);
        return courses;
    }

    /*
    @GetMapping("/reqAvailablecourses")
    public String reqAvailablecourses(){
        return "reqAvailablecourses"; // Send "reqAvailablecourses" string
    }
    */

}