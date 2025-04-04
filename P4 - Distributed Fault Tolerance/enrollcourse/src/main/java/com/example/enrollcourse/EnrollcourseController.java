package com.example.enrollcourse;

import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class EnrollcourseController {

    // Receive Post Request from Web Application
    @PostMapping("/reqEnrollcourse")
    public String reqEnrollcourse(){
        return "reqEnrollcourse";
    }
}
