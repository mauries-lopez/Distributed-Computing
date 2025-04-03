package com.example.logout;

import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class LogoutController {

    // Receive Request Login from Web Application
    @PostMapping("/reqLogout")
    public String reqLogout(){
        return "index"; // Send "index" string
    }
}