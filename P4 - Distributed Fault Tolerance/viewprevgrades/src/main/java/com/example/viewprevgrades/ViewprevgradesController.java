package com.example.viewprevgrades;

import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class ViewprevgradesController {

    // Receive Post Request from Web Application
    @PostMapping("/reqViewprevgrades")
    public String reqViewprevgrades(){
        return "viewPrevGrades";
    }

}
