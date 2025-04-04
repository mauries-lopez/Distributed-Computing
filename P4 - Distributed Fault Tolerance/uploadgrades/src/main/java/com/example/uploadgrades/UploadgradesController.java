package com.example.uploadgrades;

import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class UploadgradesController {

    @PostMapping("/reqUploadgrades")
    public String uploadGrades(){
        return "uploadGrades";
    }
}