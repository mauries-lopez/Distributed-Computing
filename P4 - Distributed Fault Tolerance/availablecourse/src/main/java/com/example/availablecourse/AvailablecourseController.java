package com.example.availablecourse;

import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class AvailablecourseController {

    // Receive Post Request from Web Application
    @PostMapping("/reqAvailablecourses")
    public String reqAvailablecourses(){
        return "reqAvailablecourses"; // Send "reqAvailablecourses" string
    }
}
